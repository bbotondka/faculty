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
		/*
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		$related_data = $this->getRelatedData($row->id);
		// If marked as spam in the related data, skip this row
		if ($related_data->spam) {
			return FALSE;
		}

		// Add the related data of interest
		$row->foo = $related_data->foo;
		$row->bar = $related_data->bar;
		*/

		// Get professor id from current row.
		$current_professor_id = $row->profid;

		// Check if related user exists.
		// Create new profile2 in next step.
		if (empty($row->migrate_map_destid1)) {
			$row->new_user = 'success test';

			// Now we create user.
			var_dump(array('new just before' => $row));
		}

		// Pass this row.
		return TRUE;
	}

	public function prepare($entity, stdClass $row) {
		var_dump(array('new just after' => $row));
	}

	public function complete($entity, stdClass $row) {}
}
