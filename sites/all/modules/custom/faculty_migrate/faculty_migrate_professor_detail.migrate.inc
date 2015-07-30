<?php

/**
 * Class MigrateFacultyProfessorDetailMigration
 *
 * Second version of migration class.
 */
class MigrateFacultyProfessorDetailMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import professors from json file');

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

		$faculty_migrate_path = drupal_get_path('module', 'faculty_migrate');
		$list_url = $base_url . '/' . $faculty_migrate_path . '/data/profs.json';
		$item_url = $base_url . '/' . $faculty_migrate_path . '/data/prof_:id.json';

		// Second parameter is the unique ID from the source file.
		$this->source = new MigrateSourceList(
			new MigrateListJSON($list_url),
			new MigrateItemJSON($item_url, array()),
			$this->fields_from_json()
		);

		// Here 'main' is machine name of the default profile2 type.
		// Call profile2_get_types() to get the list.
		$this->destination = new MigrateDestinationProfile2('main');

		// @Todo get the user id (if doesn't exists before we create it).

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

		// Degree.
		$this->addFieldMapping('field_degree', 'degree');
		// Other entry.
		$this->addFieldMapping('field_other_entry', 'other_entry');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
			'other_entry' => 'Other entry from separated json file'
		);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		global $base_url;
		// Fix bug with http host on local environment.
		if ($_SERVER['HTTP_HOST'] === 'default') {
			// Override local url.
			$base_url = 'http://faculty.dev';
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
				// Log error.
				watchdog('faculty_migrate', 'User does not exist: <pre>@profile_object</pre>', array(
					'@profile_object' => print_r( $profile_object, TRUE),
				), WATCHDOG_ERROR);

				// Skip this row because loading of user failed.
				return FALSE;
			}

			// @Todo: Update existing user field (not profile2 field).

			// We save existing user id related to current profile2.
			$row->related_user_id = $profile_object->uid;
		}
		else {
			// Exception.

			// Log error.
			watchdog('faculty_migrate', 'Fail to load profile2: %uid', array(
				'%uid' => $row->migrate_map_destid1,
			), WATCHDOG_ERROR);

			return FALSE;
		}

		var_dump($row);

		// Pass this row.
		return TRUE;
	}

	private function faculty_migrate_create_user() {

		// @Todo: pass name, mail and other variables in param of this function.
		$name = 'user_name';
		$mail = 'user_name@example.com';

		// User name already used.
		if ($account = user_load_by_name($name)) {
			// Log error.
			watchdog('faculty_migrate', 'User already exists: %name', array(
				'%name' => $name,
			), WATCHDOG_ERROR);

			return $account;
		}

		// User name already used.
		if ($account = user_load_by_mail($mail)) {
			// Log error.
			watchdog('faculty_migrate', 'User already exists: %mail', array(
				'%mail' => $mail,
			), WATCHDOG_ERROR);

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

	public function prepare($profile2, stdClass $row) {
		var_dump(array('new just after' => $row));

		if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}

	}

	public function complete($profile2, stdClass $row) {}

}
