<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyDeptpedMigration
 *
 * First version of migration class.
 */
class MigrateFacultyDeptpedMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import deptped from json file');

		// Define unique ID from source used to match content
		// with drupal taxonomy_term.
		$this->map = new MigrateSQLMap($this->machineName,
			array(
              FACULTY_MIGRATE_DEPTPED_SOURCE_ID => array(
                    'type' => 'varchar',
                    'length' => '32',
					'not null' => TRUE,
				)
			),
          MigrateDestinationTerm::getKeySchema()
		);

		// Get base url.
		$base_url = faculty_migrate_get_base_url();
		// Path to json files.
		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/catDeptped.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyDeptpedListJSON($source_url),
			new FacultyDeptpedItemJSON($source_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: taxonomy term.
		$this->destination = new MigrateDestinationTerm(FACULTY_MIGRATE_DEPTPED_VOC_MACHINE_NAME);

		// Destination - Source mapping.
        $this->addFieldMapping('name', 'lib_fr');
        $this->addFieldMapping('field_code', 'lib_fr');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_DEPTPED_SOURCE_ID => 'Unique ID for each source data row',
            'lib_fr' => 'French term name'
		);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

        // Skype empty name.
        if (empty($row->lib_fr)) {
          return FALSE;
        }
	}

	public function prepare($term, stdClass $row) {}

}

/**
 * Class FacultyDeptpedListJSON.
 */
class FacultyDeptpedListJSON extends MigrateListJSON {
  /**
   * Originally this class assumed that the results would be all that was in the JSON
   * object. This class has been modified to look inside the JSON object and find the
   * expected data within the ['results'] member.
   */
  protected function getIDsFromJSON(array $data) {
    $ids = array();

    // Pass main key entry.
    $data = $data[FACULTY_MIGRATE_DEPTPED_PLURAL_KEY_ENTRY];

    foreach ($data[FACULTY_MIGRATE_DEPTPED_SINGULAR_KEY_ENTRY] as $item) {
      $ids[] = $item[FACULTY_MIGRATE_DEPTPED_SOURCE_ID];
    }
    return $ids;
  }

  /**
   * If this is the case then the count will be off as well and we will have to overriden as well.
   */
  public function computeCount() {
    $count = 0;
    if (empty($this->httpOptions)) {
      $json = file_get_contents($this->listUrl);
    }
    else {
      $response = drupal_http_request($this->listUrl, $this->httpOptions);
      $json = $response->data;
    }
    if ($json) {
      $data = drupal_json_decode($json);
      if ($data) {
        // Pass main key entry.
        $data = $data[FACULTY_MIGRATE_DEPTPED_PLURAL_KEY_ENTRY];

        $count = count($data[FACULTY_MIGRATE_DEPTPED_SINGULAR_KEY_ENTRY]);
      }
    }
    return $count;
  }
}

/**
 * Class FacultyDeptpedItemJSON
 */
class FacultyDeptpedItemJSON extends MigrateItemJSON {
  protected $data = array();

	public function getItem($id) {

		if (empty($this->data)) {

			$item_url = $this->constructItemUrl($id);
			// Get the JSON object at the specified URL
			$json = $this->loadJSONUrl($item_url);
			// Return json object in term.
			if ($json
			    && $json->{FACULTY_MIGRATE_DEPTPED_PLURAL_KEY_ENTRY}
			    && $json->{FACULTY_MIGRATE_DEPTPED_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_DEPTPED_SINGULAR_KEY_ENTRY}
			) {
				// For each term in json.
				foreach ($json->{FACULTY_MIGRATE_DEPTPED_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_DEPTPED_SINGULAR_KEY_ENTRY} as $term) {
					$this->data[$term->{'@code'}] = $term;
				}

			}
			else {
				$migration = Migration::currentMigration();
				$message =  t('Loading of !objecturl failed:', array('!objecturl' => $item_url));
				$migration->getMap()->saveMessage(
					array($id), $message, MigrationBase::MESSAGE_ERROR);
				return NULL;
			}

		}

		// Check existing data.
		if (empty($this->data[$id])) {
			return NULL;
		}

		return $this->data[$id];
	}

  /**
   * Convert array to object.
   *
   * @param $array
   * @return object|stdClass
   */
  public function parseArrayToObject($array) {
    $object= new stdClass();
    if (is_array($array) && count($array) > 0) {
      foreach ($array as $name => $value) {
        $name = trim($name);
        if (is_array($value)){
          $object = (object) $value;
        }
      }
    }
    return $object;
  }

}
