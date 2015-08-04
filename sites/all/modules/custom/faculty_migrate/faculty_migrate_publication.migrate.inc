<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyPublicationMigration
 *
 * First version of migration class.
 */
class MigrateFacultyPublicationMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import publications from json file');

		// Define unique ID from source used to match content
		// with drupal entities (profile2).
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
		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/publications.json';

		// Define source of migration.
		$this->source = new MigrateSourceJSON($source_url, FACULTY_MIGRATE_PUBLICATION_SOURCE_ID, $this->fields_from_json());

		// Destination of migration: eck/publication entity type.
		$this->destination = new MigrateDestinationEntityAPI('publication', 'undefined_type');

		// Destination - Source mapping.
		// Save original id from json file to retrieve publication.
		//$this->addFieldMapping('field_publication_id', FACULTY_MIGRATE_PUBLICATION_SOURCE_ID);
		// TMP admin userid.
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
			FACULTY_MIGRATE_PUBLICATION_SOURCE_ID => 'Unique ID for each source data row',
			'type' => 'bundle type json file'
		);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		return TRUE;
	}

	public function prepare($eck_publication_entity, stdClass $row) {

		// Switch type (bundle).
		if (!empty($row->type)) {
			$eck_publication_entity->type = $row->type;
		}

		// Add property infos in eck_publication table.
		if (!empty($row->publicationid)) {
			// Set publicationid.
			$eck_publication_entity->publicationid = intval($row->publicationid);
			// Set title.
			$eck_publication_entity->title = t('publication detail') . ' ' . $row->publicationid;
		}

	}

	public function complete($profile2, stdClass $row) {}

}
