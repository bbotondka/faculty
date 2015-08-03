<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyPublicationDetailMigration
 *
 * Second version of migration class.
 */
class MigrateFacultyPublicationDetailMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import publication detail from json file');

		// Define unique ID from source used to match content
		// with drupal entities (eck/publication).
		$this->map = new MigrateSQLMap($this->machineName,
			array(
				FACULTY_MIGRATE_PUBLICATION_SOURCE_ID => array(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => TRUE,
				)
			),
			MigrateDestinationEntityAPI::getKeySchema('publication')
		);

		// Get base url.
		$base_url = faculty_migrate_get_base_url();
		// Path to json files.
		$faculty_migrate_path = drupal_get_path('module', 'faculty_migrate');
		$list_url = $base_url . '/' . $faculty_migrate_path . '/data/publications.json';
		$item_url = $base_url . '/' . $faculty_migrate_path . '/data/publication_:id.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new MigrateListJSON($list_url),
			new MigrateItemJSON($item_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: eck/publication entity type.
		$this->destination = new MigrateDestinationEntityAPI('publication', 'undefined_type');

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		//$this->addFieldMapping('field_publication_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
			'type' => 'bundle type json file'
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
		//return faculty_migrate_user_profile_custom_prepare_row($row);
	}

	public function prepare($eck_publication_entity, stdClass $row) {

		// Switch type (bundle).
		if (!empty($row->type)) {
			$eck_publication_entity->type = $row->type;
		}

		// Set title.
		if (!empty($row->publicationid)) {
			$eck_publication_entity->title = t('publication detail') . ' ' . $row->publicationid;
		}

	}

	public function complete($profile2, stdClass $row) {}

}
