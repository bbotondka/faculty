<?php

/**
 * Class MigrateFacultyProfessorMigration
 *
 * First version of migration class.
 */
class MigrateFacultyProfessorMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import users from json file');

		// Define unique ID from source used to match content
		// with drupal entities (profile2).
		$this->map = new MigrateSQLMap($this->machineName,
			array(
				FACULTY_MIGRATE_USER_SOURCE_ID => array(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => TRUE,
				)
			),
			MigrateDestinationProfile2::getKeySchema()
		);

		global $base_url;
		// Fix bug with http host on local environment.
		if ($_SERVER['HTTP_HOST'] === 'default') {
			// Override local url.
			$base_url = 'http://faculty.dev';
		}

		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/profs.json';

		// Second parameter is the unique ID from the source file.
		$this->source = new MigrateSourceJSON($source_url, FACULTY_MIGRATE_USER_SOURCE_ID, $this->fields_from_json());

		// Here 'main' is machine name of the default profile2 type.
		// Call profile2_get_types() to get the list.
		$this->destination = new MigrateDestinationProfile2('main');

		// @Todo get the user id (if doesn't exists before we create it).

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

		// Degree
		$this->addFieldMapping('field_degree', 'degree');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
			'degree' => 'Degree source data',
		);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		// Get professor id from current row.
		$current_professor_id = $row->profid;

		// Check if related user exists.
		if (empty($row->migrate_map_destid1)) {
			// Now we try to create user.
			$new_user = $this->faculty_migrate_create_user();

			if (empty($new_user)) {
				// Skip this row because creation of user failed.
				return FALSE;
			}

			// We save existing user id related to current profile2.
			$row->related_user_id = $new_user->uid;
		}
		elseif($profile_object = profile2_load($row->migrate_map_destid1)) {
			// Here user potentially exists.

			// Try to load existing user.
			$existing_user_object = user_load($profile_object->uid);

			if (empty($existing_user_object)) {
				// Skip this row because loading of user failed.
				return FALSE;
			}

			// @Todo: Update existing user field (not profile2 field).

			// We save existing user id related to current profile2.
			$row->related_user_id = $profile_object->uid;
		}

		// Pass this row.
		return TRUE;
	}

	private function faculty_migrate_create_user() {
		//This will generate a random password, you could set your own here
		$password = user_password(8);

		// @Todo: pass name, mail and other variables in param of this function.

		//set up the user fields
		$fields = array(
			'name' => 'user_name',
			'mail' => 'user_name@example.com',
			'pass' => $password,
			'status' => 1,
			'init' => 'email address',
			'roles' => array(
				DRUPAL_AUTHENTICATED_RID => 'authenticated user',
			),
		);

		//the first parameter is left blank so a new user is created
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

	public function prepare($profile2, stdClass $row) {
		var_dump(array('new just after' => $row));

		if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}

		// Test faculty_migrate_get_json() function.
		global $base_url;
		// Fix bug with http host on local environment.
		if ($_SERVER['HTTP_HOST'] === 'default') {
			// Override local url.
			$base_url = 'http://faculty.dev';
		}

		$item_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/prof_' . $row->profid . '.json';

		var_dump(array(
			'item_url' => $item_url,
			'data' => $this->faculty_migrate_get_json($item_url),
		));

		// @Todo: Update profile2 fields with
		// @Todo: $this->faculty_migrate_get_json($item_url) data result.

	}

	public function complete($profile2, stdClass $row) {}

	/**
	 * Get json from $url.
	 *
	 * @param $url
	 *   Json url.
	 *
	 * @return bool
	 *   Array of data of FALSE.
	 */
	private function faculty_migrate_get_json($url) {
		// Request on json file.
		$request = drupal_http_request($url);

		if (isset($request->error)) {
			return FALSE;
		}

		try {
			// Custom version of drupal_json_decode() with error catcher.
			return $this->faculty_migrate_json_decoder($request->data, TRUE);
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
	private function faculty_migrate_json_decoder($json,$assoc = false,$depth = 512,$options = 0) {

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

}
