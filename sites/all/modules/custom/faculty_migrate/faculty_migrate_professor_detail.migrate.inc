<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');
// Use custom function FacultyProfessorListJSON().
include_once('faculty_migrate_professor.migrate.inc');

/**
 * Class MigrateFacultyProfessorDetailMigration
 *
 * Second version of migration class.
 */
class MigrateFacultyProfessorDetailMigration extends Migration {
    protected $pseudo_field_name = '';

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);

        // Set $pseudo_field_name.
        $this->pseudo_field_name = FACULTY_MIGRATE_DEPTPED_SINGULAR_KEY_ENTRY . '_code';

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
		$list_url = $base_url . '/' . $faculty_migrate_path . '/data/allProf.json';
		$item_url = $base_url . '/' . $faculty_migrate_path . '/data/cv_:id.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyProfessorListJSON($list_url),
			new FacultyProfessorDetailItemJSON($item_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: profile2 entity type.
		$this->destination = new MigrateDestinationProfile2(FACULTY_MIGRATE_PROFESSOR_DETAIL_PROFILE_MACHINE_NAME);

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid', FACULTY_MIGRATE_USER_SOURCE_ID)
             ->sourceMigration('FacultyUser');

		// Deptped.
		$this->addFieldMapping('field_deptped_ref', 'deptped_code')
             ->sourceMigration('FacultyDeptped');
        $this->addFieldMapping('field_deptped_ref:source_type')
             ->defaultValue('tid');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
            $this->pseudo_field_name => 'deptped code prepared in prepareRow()'
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
		/*if (faculty_migrate_user_profile_custom_prepare_row($row) === FALSE) {
          return FALSE;
        }*/

        // Prepare deptped_code.
        if (faculty_migrate_prepare_pseudo_field_with_code($row, FACULTY_MIGRATE_DEPTPED_SINGULAR_KEY_ENTRY , FACULTY_MIGRATE_DEPTPED_SOURCE_ID) === FALSE) {
          return FALSE;
        }

	}

	public function prepare($profile2, stdClass $row) {

		/*if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}*/

	}

	public function complete($profile2, stdClass $row) {}

}

/**
 * Class FacultyProfessorItemJSON
 */
class FacultyProfessorDetailItemJSON extends MigrateItemJSON {
	protected $data = array();

	public function getItem($id) {

		$item_url = $this->constructItemUrl($id);
		// Get the JSON object at the specified URL
		$json = $this->loadJSONUrl($item_url);
		// Return json object in prof.
		if ($json && $json->prof) {
			return $json->prof;
		}
		else {
			$migration = Migration::currentMigration();
			$message =  t('Loading of !objecturl failed:', array('!objecturl' => $item_url));
			$migration->getMap()->saveMessage(
				array($id), $message, MigrationBase::MESSAGE_ERROR);
			return NULL;
		}
	}

}
