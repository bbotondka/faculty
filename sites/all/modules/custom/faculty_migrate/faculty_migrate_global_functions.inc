<?php


/**
 * Load user by professor_id (field in profile2).
 *
 * @param $professor_id
 *   External user id.
 *
 * @return bool|mixed
 */
function faculty_migrate_user_load_by_professor_id($professor_id, $profile_type) {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'profile2')
	      ->entityCondition('bundle', $profile_type)
	      ->fieldCondition('field_professor_id', 'value', intval($professor_id), '=');

	$result = $query->execute();

	if (isset($result['profile2'])) {
		$profile2_id = reset(array_keys($result['profile2']));
		$profile2_loaded = entity_load_single('profile2', $profile2_id);

		return user_load($profile2_loaded->uid);
	}

	return FALSE;
}

/**
 * Try to create user.
 *
 * Caution: Duplicate profile2 by user if $return_account = TRUE.
 *
 * @param $name
 *   Username of user.
 * @param $mail
 *   Mail of user.
 * @param $professor_id
 *   External id of professor.
 * @param $profile_type
 *   Profile type - profile2 bundle.
 * @param $return_existing_account
 *   Return existing account or FALSE.
 *
 * @return bool|mixed|void
 *   Return an account or FALSE.
 *
 * @throws Exception
 */
function faculty_migrate_create_user($name, $mail, $professor_id) {

	// We search if external prof id match with existing
	// user profile 'professor'.
	// Caution: we don't use 'professor_detail' profile here because
	// 'professor' is a first class which potentially create user/
	// See dependency in faculty_migrate_migrate_api().
	if ($account = faculty_migrate_user_load_by_professor_id($professor_id, FACULTY_MIGRATE_PROFESSOR_PROFILE_MACHINE_NAME)) {
		// Log error.
		watchdog('faculty_migrate', 'User already exists with same professor_id: %professor_id. See faculty_migrate_create_user()', array(
			'%professor_id' => $professor_id,
		), WATCHDOG_ERROR);

		return $account;
	}

	// User name already used.
	if ($account = user_load_by_name($name)) {
		// Log error.
		watchdog('faculty_migrate', 'User already exists: %name. See faculty_migrate_create_user()', array(
			'%name' => $name,
		), WATCHDOG_ERROR);

		return FALSE;
	}

	// User mail already used.
	if ($account = user_load_by_mail($mail)) {
		// Log error.
		watchdog('faculty_migrate', 'User already exists: %mail. See faculty_migrate_create_user()', array(
			'%mail' => $mail,
		), WATCHDOG_ERROR);

		return FALSE;
	}

	// This will generate a random password, you could set your own here.
	$password = user_password(8);

	//set up the user fields
	$fields = array(
		'name' => $name,
		'mail' => $mail,
		'pass' => $password,
		'status' => 1,
		'init' => 'email address',
		'roles' => array(
			DRUPAL_AUTHENTICATED_RID => 'authenticated user',
		),
	);

	// The first parameter is left blank so a new user is created.
	$account = user_save('', $fields);

	if (empty($account)) {
		// user_save() fail.
		// Exit from function.
		return FALSE;
	}

	// If you want to send the welcome email, use the following code

	// Manually set the password so it appears in the e-mail.
	$account->password = $fields['pass'];

	// Send the e-mail through the user module.
	//drupal_mail('user', 'register_no_approval_required', $email, NULL, array('account' => $account), variable_get('site_mail', 'noreply@example..com'));

	return $account;
}

/**
 * Get base_url.
 *
 * Prevent error when user execute drush migrate commands.
 *
 * @return string
 */
function faculty_migrate_get_base_url() {
	global $base_url;
	// Fix bug with http host on local environment.
	if ($_SERVER['HTTP_HOST'] === 'default') {
		// Override local url.
		$base_url = FACULTY_MIGRATE_BASE_URL;
	}

	return $base_url;
}

/**
 * Custom prepare row with user creation.
 *
 * @param $row
 *   Current row (each professor)
 *
 * @return bool
 *   TRUE: pass and FALSE: Skip this row.
 */
function faculty_migrate_user_profile_custom_prepare_row($row) {
	// @Todo: Write this exception case:
	// @Todo: Check if existing profil exists for current user
	// @Todo: but destination_id doesn't exists in table of mapping.

	// Check if related user exists.
	if (empty($row->migrate_map_destid1)) {
		// Potentially user isn't already created.

		// @Todo: check format of data before insert.

		// Get professor id from current row.
		$current_professor_id = $row->profid;
		// Get username from current row.
		$name = $row->name;
		// Get mail from current row.
		$mail = $row->mail;

		// Now we try to create user.
		$new_user = faculty_migrate_create_user($name, $mail, $current_professor_id);

		if (empty($new_user)) {
			// Skip this row because creation of user failed.
			return FALSE;
		}

		// We save existing user id related to current profile2.
		$row->related_user_id = $new_user->uid;

		// Pass this row now.
		return TRUE;
	}
	elseif ($profile_object = profile2_load($row->migrate_map_destid1)) {
		// Here user potentially exists.

		// Try to load existing user.
		$existing_user_object = user_load($profile_object->uid);

		if (empty($existing_user_object)) {
			// Log error.
			watchdog('faculty_migrate', 'User does not exist: <pre>@profile_object</pre>', array(
				'@profile_object' => print_r( $profile_object, TRUE),
			), WATCHDOG_ERROR);

			// Skip this row because loading of user failed.
			return FALSE;
		}

		// We save existing user id related to current profile2.
		$row->related_user_id = $profile_object->uid;

		// Pass this row now.
		return TRUE;
	}
	else {
		// Exception.

		// Log error.
		watchdog('faculty_migrate', 'Fail to load profile2: %uid', array(
			'%uid' => $row->migrate_map_destid1,
		), WATCHDOG_ERROR);

		return FALSE;
	}

	// Skip this row by default.
	return FALSE;
}

/**
 * Get json from $url.
 *
 * @param $url
 *   Json url.
 *
 * @return bool
 *   Array of data of FALSE.
 */
function faculty_migrate_get_json($url) {
	// Request on json file.
	$request = drupal_http_request($url);

	if (isset($request->error)) {
		return FALSE;
	}

	try {
		// Custom version of drupal_json_decode() with error catcher.
		return faculty_migrate_json_decoder($request->data, TRUE);
	}
	catch (Exception $e) {
		watchdog('faculty_migrate', 'An error happened while decoding the data : %error_message (%url)', array(
			'%error_message' => $e->getMessage(),
			'%url' => $url,
		), WATCHDOG_ERROR);
	}

	// Default value.
	return FALSE;
}

/**
 * Custom json decoder with catcher (human readable version).
 *
 * See json_decode().
 *
 * @param $json
 * @param bool $assoc
 * @param int $depth
 * @param int $options
 *
 * @return mixed|null
 * @throws Exception
 */
function faculty_migrate_json_decoder($json,$assoc = false,$depth = 512,$options = 0) {

	$decode_result = json_decode($json,$assoc = false,$depth = 512,$options = 0);

	if ($decode_result === NULL) {
		$err = t('Unknown Error');
		switch (json_last_error()) {
			case JSON_ERROR_NONE:
				$err = t('No error');
				break;
			case JSON_ERROR_DEPTH:
				$err =  t('Maximum recursion limit has been reached');
				break;
			case JSON_ERROR_STATE_MISMATCH:
				$err = t('Inadequate modes');
				break;
			case JSON_ERROR_CTRL_CHAR:
				$err = t('Special chars control error');
				break;
			case JSON_ERROR_SYNTAX:
				$err = t('Syntax error: JSON malformed');
				break;
			case JSON_ERROR_UTF8:
				$err = t('UTF8 chars are malformed; This might be an encoding problem');
				break;
		}
		throw new Exception($err);
		return null;
	}

	return $decode_result;
}

/**
 * Execute request to get eck/publication allowed list.
 *
 * @param $bundle
 *   Specific bundle of eck/publication entity.
 *
 * @return array
 */
function faculty_migrate_get_allowed_publication($bundle) {
	$allowed_publication = array();

	$query = db_select('eck_publication', 'eckp');

	$query->condition('type', $bundle);
	$query->fields('eckp',array('id', 'publicationid'));

	$result = $query->execute();

	// For each record.
	while($record = $result->fetchAssoc()) {
		// Publicationid (from json) in key.
		$publicationid = $record['publicationid'];

		// Add record to $allowed_publication array.
		$allowed_publication[$publicationid] = $record['id'];
	}

	return $allowed_publication;
}
