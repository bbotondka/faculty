<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

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

		// Get base url.
		$base_url = faculty_migrate_get_base_url();
		// Path to json files.
		$faculty_migrate_path = drupal_get_path('module', 'faculty_migrate');
		$list_url = $base_url . '/' . $faculty_migrate_path . '/data/profs.json';
		$item_url = $base_url . '/' . $faculty_migrate_path . '/data/prof_:id.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new MigrateListJSON($list_url),
			new MigrateItemJSON($item_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: profile2 entity type.
		$this->destination = new MigrateDestinationProfile2(FACULTY_MIGRATE_PROFESSOR_DETAIL_PROFILE_MACHINE_NAME);

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

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

		// Custom prepare row.
		// Try to create user in this function.
		return faculty_migrate_user_profile_custom_prepare_row($row);
	}

	public function prepare($profile2, stdClass $row) {
		var_dump(array('new just after' => $row));

		if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}

	}

	public function complete($profile2, stdClass $row) {}

}
