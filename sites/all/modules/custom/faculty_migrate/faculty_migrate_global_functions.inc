<?php


/**
 * Load user or profile2 by professor_id (field in profile2).
 *
 * @param $professor_id
 *   External user id.
 *
 * @return bool|mixed
 */
function faculty_migrate_user_load_by_professor_id($professor_id, $profile_type, $return_profile2 = FALSE) {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'profile2')
	      ->entityCondition('bundle', $profile_type)
	      ->fieldCondition('field_professor_id', 'value', intval($professor_id), '=');

	$result = $query->execute();

	if (isset($result['profile2'])) {
		$profile2_id = reset(array_keys($result['profile2']));
		$profile2_loaded = entity_load_single('profile2', $profile2_id);

		if (empty($profile2_loaded)) {
			return FALSE;
		}

		// Return profile2 object.
		if ($return_profile2) {
			return $profile2_loaded;
		}

		// Return user object.
		return user_load($profile2_loaded->uid);
	}

	return FALSE;
}

function faculty_migrate_change_user_status_from_pids($profile_ids, $user_status = 0) {
	$user_ids = db_select('profile', 'p')
		->fields('p', 'uid')
		->condition('pid', $profile_ids,'IN')
		->execute()
		->fetchAll();

	var_dump($user_ids);
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

        // Try to update name and mail of user.
        faculty_migrate_update_user($account, $name, $mail);

		return $account;
	}

	// User name already used.
	if ($account = user_load_by_name($name)) {
		// Log error.
		watchdog('faculty_migrate', 'User already exists: %name. See faculty_migrate_create_user()', array(
			'%name' => $name,
		), WATCHDOG_ERROR);

        // Try to update name and mail of user.
        faculty_migrate_update_user($account, $name, $mail);

		return $account;
	}

	// User mail already used.
	if ($account = user_load_by_mail($mail)) {
		// Log error.
		watchdog('faculty_migrate', 'User already exists: %mail. See faculty_migrate_create_user()', array(
			'%mail' => $mail,
		), WATCHDOG_ERROR);

        // Try to update name and mail of user.
        faculty_migrate_update_user($account, $name, $mail);

		return $account;
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
 * Try to update name and mail of user account.
 *
 * @param $account
 *   Account object.
 * @param $name
 *   New name of user
 * @param $mail
 *   New mail of user.
 */
function faculty_migrate_update_user($account, $name, $mail) {
  $update_status = FALSE;

  if ($account->name !== $name) {
    // Update name.
    $account->name = $name;

    $update_status = TRUE;
  }

  if ($account->mail !== $mail) {
    // Update mail.
    $account->mail = $mail;

    $update_status = TRUE;
  }

  if (!$update_status) {
    // Skip if data not updated.
    return;
  }

  // Save user.
  if (!user_save($account)) {
    // Log error.
    watchdog('faculty_migrate', 'Error when trying to update name (%name) and mail (%mail) of user (%uid): %mail. See faculty_migrate_update_user()', array(
        '%uid' => $account->uid,
        '%name' => $name,
        '%mail' => $mail,
      ), WATCHDOG_ERROR);
  }
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

    // Include pathauto.inc file to use pathauto_cleanstring().
    module_load_include('inc', 'pathauto');

	// Check if related user exists.
	if (empty($row->migrate_map_destid1)) {
		// Potentially user isn't already created.

		// @Todo: check format of data before insert.

		// Get professor id from current row.
		$current_professor_id = $row->{FACULTY_MIGRATE_USER_SOURCE_ID};

        if (empty($row->email)) {
          // Log error.
          watchdog('faculty_migrate', 'Email is required to import this row: <pre>@row</pre>.', array(
              '@row' => print_r($row, TRUE),
            ), WATCHDOG_ERROR);

          // Mail is required.
          return FALSE;
        }

		// Get mail from current row.
		$mail = $row->email;
        // Generate username from mail.
        $name = pathauto_cleanstring($mail);

		// Now we try to create user.
		$new_user = faculty_migrate_create_user($name, $mail, $current_professor_id);

		if (empty($new_user)) {
            // Log error.
            watchdog('faculty_migrate', 'User creation: Skip this row because creation of user failed: name: %name, mail: %mail, current_professor_id: %current_professor_id', array(
                '%name' => $name,
                '%mail' => $mail,
                '%current_professor_id' => $current_professor_id,
              ), WATCHDOG_ERROR);

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

        if (empty($row->email)) {
          // Log error.
          watchdog('faculty_migrate', 'profile2_load: Email is required to import this row: <pre>@row</pre>.', array(
              '@row' => print_r($row, TRUE),
            ), WATCHDOG_ERROR);

          // Mail is required.
          return FALSE;
        }

        // Try to update name and mail of user.
        $username = pathauto_cleanstring($row->email);
        faculty_migrate_update_user($existing_user_object, $username, $row->email);

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

/**
 * Set eck_publication reference (populate field_publication_ref).
 */
function faculty_migrate_set_eck_publication_reference($node, $row, $eck_publication_allowed) {
	$node_book_wrapper = entity_metadata_wrapper('node', $node);

	if (!empty($node_book_wrapper->field_publication_ref)) {
		$node_book_wrapper->field_publication_ref->set(intval($eck_publication_allowed[$row->publicationid]));
		$node_book_wrapper->save();
	}

}

/**
 * Set authors (internal and external).
 *
 * @param $node
 * @param $row
 *
 * @return bool
 */
function faculty_migrate_set_authors($node, $row) {

	$internal_authors = array();
	$external_authors = array();

	$node_book_wrapper = entity_metadata_wrapper('node', $node);

	// For each author.
	foreach($row->authors as $author) {

		// External author.
		if (empty($author->authorid)) {

			// Check author name.
			if (empty($author->name)) {

				// Log error.
				watchdog('faculty_migrate', 'No name and author id: <pre>@row</pre> <pre>@node</pre>.', array(
					'@node' => print_r($node, TRUE),
					'@row' => print_r($row, TRUE),
				), WATCHDOG_ERROR);

				// Next author.
				continue;
			}

			// Add external author to the list.
			$external_authors[] = $author->name;

		}
		elseif ($user_loaded = faculty_migrate_user_load_by_professor_id($author->authorid, FACULTY_MIGRATE_PROFESSOR_PROFILE_MACHINE_NAME)) {
			// Internal user.
			// Try to load user.

			$internal_authors[] = $user_loaded;
		}

	}

	// Set field_authors.
	if ((!empty($internal_authors))
     && (!empty($node_book_wrapper->field_authors))) {
		$node_book_wrapper->field_authors->set($internal_authors);
		$node_book_wrapper->save();
	}

	// Set field_external_authors.
	if ((!empty($external_authors))
     && (!empty($node_book_wrapper->field_external_authors))) {
		$node_book_wrapper->field_external_authors->set($external_authors);
		$node_book_wrapper->save();
	}

	return TRUE;

}

/**
 * Node publication custom prepare row.
 *
 * @param $row
 *   Row from json file.
 *
 * @return bool
 *   TRUE: pass - FALSE: next row.
 */
function faculty_migrate_node_publication_prepare_row($row) {

	// Check if row author exists.
	if (empty($row->authors)) {
		// Log error.
		watchdog('faculty_migrate', 'Related authors do not referenced: <pre>@row</pre>.', array(
			'@row' => print_r( $row, TRUE),
		), WATCHDOG_ERROR);

		return FALSE;
	}

	// Default result.
	return TRUE;
}

/**
 * Try to attach current image to related user.
 *
 * @param $image
 * @param $row
 */
function faculty_migrate_attach_image_to_related_user($image, $row) {

	// Skip if image hasn't fid.
	if (empty($image->fid)) {
		return FALSE;
	}

	// Skip empty destination_file in row.
	if (empty($row->destination_file)) {
		return FALSE;
	}

	// Skip if external id not find.
	if (!$external_user_id = faculty_migrate_extract_id_from_filename($row->destination_file)) {
		return FALSE;
	}

	// Skip if user not loaded.
	if (!$user_loaded = faculty_migrate_user_load_by_professor_id($external_user_id, FACULTY_MIGRATE_PROFESSOR_PROFILE_MACHINE_NAME)) {
		return FALSE;
	}

	// We can attach image now.

	// Error with entity_metadata_wrapper 'picture' property.
	// See https://www.drupal.org/node/2224645#comment-10161102.
	$user_loaded->picture = $image;

	$user_wrapper = entity_metadata_wrapper('user', $user_loaded);
	$user_wrapper->save();

	return TRUE;
}

/**
 * Try to attach current cv to related profile2.
 *
 * @param $image
 * @param $row
 */
function faculty_migrate_attach_cv_to_related_profile2($cv, $row) {

	// Skip if image hasn't fid.
	if (empty($cv->fid)) {
		return FALSE;
	}

	// Skip empty destination_file in row.
	if (empty($row->destination_file)) {
		return FALSE;
	}

	// Skip if external id not find.
	if (!$external_user_id = faculty_migrate_extract_id_from_filename($row->destination_file)) {
		return FALSE;
	}

	// Skip if profile2 (bundle professor_detail) not loaded.
	if (!$profile2_loaded = faculty_migrate_user_load_by_professor_id($external_user_id, FACULTY_MIGRATE_PROFESSOR_DETAIL_PROFILE_MACHINE_NAME, TRUE)) {
		return FALSE;
	}

	// We can attach cv now.

	$user_wrapper = entity_metadata_wrapper('profile2', $profile2_loaded);
	$user_wrapper->field_cv->file->set($cv);

	$user_wrapper->save();

	return TRUE;
}

/**
 * Extract id from filename.
 *
 * @param $filename
 *   Name of file.
 *
 * @return bool|mixed
 *   Return an id or FALSE.
 */
function faculty_migrate_extract_id_from_filename($filename) {
	// Remove extension if exists.
	$temp = explode('.', $filename);
	$ext = array_pop( $temp );
	$filename = implode('.', $temp);

	// Extract id at the end of file.
	$temp = explode('_', $filename);
	$id = end($temp);

	if (is_numeric($id)) {
		return $id;
	}

	// Default result.
	return FALSE;
}
