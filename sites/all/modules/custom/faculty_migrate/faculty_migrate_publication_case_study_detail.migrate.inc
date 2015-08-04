<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyPublicationDetailMigration
 *
 * Second version of migration class.
 */
class MigrateFacultyPublicationCaseStudyDetailMigration extends Migration {

	// Array of allowed publication
	// key: eck_publication_id - value: publicationid from json.
	protected $eck_publication_allowed;

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import publication detail from json file');
		// Set default value of $eck_publication_allowed.
		$this->eck_publication_allowed = array();

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
			MigrateDestinationNode::getKeySchema()
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

		// Destination of migration: node case_study.
		$this->destination = new MigrateDestinationNode(FACULTY_MIGRATE_NODE_CASE_STUDY_MACHINE_NAME);

		// Destination - Source mapping.
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

		$this->addFieldMapping('field_case_study', 'case');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_PUBLICATION_SOURCE_ID => 'Unique ID for each source data row',
			'title' => 'Title of current row'
		);
	}

	public function preImport() {
		parent::preImport();
		// Code to execute before first article row is imported.

		// Execute request to get eck/publication list
		// Specific bundle in param.
		$this->eck_publication_allowed = faculty_migrate_get_allowed_publication(FACULTY_MIGRATE_NODE_CASE_STUDY_MACHINE_NAME);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		// Skip this row because publicationid isn't
		// found in eck_publication_allowed array.
		if (empty($this->eck_publication_allowed[$row->publicationid])) {
			return FALSE;
		}

		// Node publication custom prepare row.
		return faculty_migrate_node_publication_prepare_row($row);
	}

	public function prepare($node_case_study, stdClass $row) {

		// Set title from source.
		if (!empty($row->title)) {
			$node_case_study->title = $row->title;
		}

	}

	public function postImport() {
		parent::postImport();
		// Code to execute after the last article row has been imported
	}

	public function preRollback() {
		parent::preRollback();
		// Code to execute before first article row is deleted
	}

	public function postRollback() {
		parent::postRollback();
		// Code to execute after the last article row has been deleted
	}

	public function complete($node_case_study, stdClass $row) {

		// Set eck_publication reference (populate field_publication_ref).
		faculty_migrate_set_eck_publication_reference($node_case_study, $row, $this->eck_publication_allowed);

		// Set authors (external & internal).
		faculty_migrate_set_authors($node_case_study, $row);
	}

}
