<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

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

		// Get base url.
		$base_url = faculty_migrate_get_base_url();
		// Path to json files.
		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/allProf.json';

        // Get an array of json source to build custom item object.
        $source_response = drupal_http_request($source_url);
        $source_json = $source_response->data;
        $decoded_source_response = drupal_json_decode($source_json);

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyProfessorListJSON($source_url),
			new FacultyProfessorItemJSON($source_url, $decoded_source_response),
			$this->fields_from_json()
		);

		// Destination of migration: profile2 entity type.
		$this->destination = new MigrateDestinationProfile2(FACULTY_MIGRATE_PROFESSOR_PROFILE_MACHINE_NAME);

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

		// Firstname.
		$this->addFieldMapping('field_firstname', 'firstname');
		// Lastname.
		$this->addFieldMapping('field_lastname', 'lastname');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
			'firstname' => 'Firstname from source data',
			'lastname' => 'Lastname from source data',
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
		if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}

	}

	public function complete($profile2, stdClass $row) {}

}

/**
 * Class FacultyProfessorListJSON
 */
class FacultyProfessorListJSON extends MigrateListJSON {
  /**
   * Originally this class assumed that the results would be all that was in the JSON
   * object. This class has been modified to look inside the JSON object and find the
   * expected data within the ['results'] member.
   */
  protected function getIDsFromJSON(array $data) {
    $ids = array();

    // Pass main key entry.
    $data = $data[FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY];

    foreach ($data[FACULTY_MIGRATE_PROFESSOR_SINGULAR_KEY_ENTRY] as $item) {
      $ids[] = $item[FACULTY_MIGRATE_USER_SOURCE_ID];
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
        $data = $data[FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY];

        $count = count($data[FACULTY_MIGRATE_PROFESSOR_SINGULAR_KEY_ENTRY]);
      }
    }
    return $count;
  }
}

/**
 * Class FacultyProfessorItemJSON
 */
class FacultyProfessorItemJSON extends MigrateItemJSON {
  protected $data = array();

  public function getItem($id) {

    if (empty($this->data)) {
      $professor_entries = $this->httpOptions[FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY];
      $final_professor_array = array();

      // For each professor.
      foreach ($professor_entries[FACULTY_MIGRATE_PROFESSOR_SINGULAR_KEY_ENTRY] as $dataobj){
        $final_professor_array[$dataobj[FACULTY_MIGRATE_USER_SOURCE_ID]] = $dataobj;
      }

      // We cache the parsed JSON at $this->data.
      $this->data = $final_professor_array;

    }

    // Convert array in object.
    $current_row_object = $this->parseArrayToObject(array($this->data[$id]));

    return $current_row_object;
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
