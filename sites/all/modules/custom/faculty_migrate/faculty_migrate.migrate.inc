<?php
/**
 * @file
 * Declares our migrations.
 */

define('FACULTY_MIGRATE_USER_SOURCE_ID', 'profid');

/**
 * Implements hook_migrate_api().
 */
function faculty_migrate_migrate_api() {
	$api = array(
		'api' => 2,
		'groups' => array(
			'faculty' => array(
				'title' => t('Faculty Migrations'),
			),
		),
		'migrations' => array(
			'FacultyUser' => array(
				'class_name' => 'MigrateFacultyUserMigration',
				'group_name' => 'faculty',
			),
		),
	);
	return $api;
}

class MigrateFacultyUserMigration extends Migration {

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

		//$source_url = realpath(drupal_get_path('module', 'faculty_migrate') . '/data/users.json');
		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/users.json';

		// Second parameter is the unique ID from the source file
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

			// @Todo: Update existing user field (not profile2 field).
			//$existing_user_object = user_load($profile_object->uid);

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
	}

	public function complete($profile2, stdClass $row) {}
}
