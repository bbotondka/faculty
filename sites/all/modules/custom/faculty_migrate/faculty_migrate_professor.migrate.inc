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

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyProfessorListJSON($source_url),
			new FacultyProfessorItemJSON($source_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: profile2 entity type.
		$this->destination = new MigrateDestinationProfile2(FACULTY_MIGRATE_PROFESSOR_PROFILE_MACHINE_NAME);

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid', FACULTY_MIGRATE_USER_SOURCE_ID)
             ->sourceMigration('FacultyUser');

		// Firstname.
		$this->addFieldMapping('field_firstname', 'firstname');
		// Lastname.
		$this->addFieldMapping('field_lastname', 'lastname');

        // Attach CV file.
        $this->addFieldMapping('field_cv', 'prepare_cvid')
          ->sourceMigration('FacultyProfessorCv');
        $this->addFieldMapping('field_cv:file_class')
          ->defaultValue('MigrateFileFid');

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
            'prepare_cvid' => 'Created from @cv in prepare method',
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

        // Prepare cvid used to attach image to current user.
        if (isset($row->{'@cv'})
          && $prepare_cvid = $this->get_cv_with_extension($row->{'@cv'})
        ) {
          // Array is required because field can be multiple.
          $row->prepare_cvid = array($prepare_cvid);
        }

	}

    /**
     * Get cvid of pdf file.
     *
     * @param $filename
     *   Filename without extension.
     *
     * @return bool
     *   Filename with extension or FALSE.
     */
    protected function get_cv_with_extension($filename) {
      $result = db_select('migrate_map_facultyprofessorcv', 'mmfpcv')
        ->fields('mmfpcv', array(
            'sourceid1'
          ))
        ->condition('sourceid1', '%' . db_like($filename) . '%', 'LIKE')
        ->range(0,1)
        ->execute()
        ->fetchAssoc();

      if (isset($result['sourceid1'])) {
        return $result['sourceid1'];
      }

      return FALSE;
    }

	public function prepare($profile2, stdClass $row) {
		/*if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}*/

	}

}

/**
 * Class FacultyProfessorMigrateDestinationProfile2.
 */
class FacultyProfessorMigrateDestinationProfile2 extends MigrateDestinationProfile2 {
	/**
	 * Give handlers a shot at cleaning up before an entity has been rolled back.
	 *
	 * @param $entity_id
	 *  ID of the entity about to be deleted..
	 */
	public function prepareRollback($profile_ids) {
		parent::prepareRollback($profile_ids);

		if (empty($profile_ids)) {
			return;
		}

		// We disable account related to $profile_ids.

		// Change user status (status set to 0).
		faculty_migrate_change_user_status_from_pids($profile_ids);
	}
}

/**
 * Class FacultyProfessorListJSON.
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

			$item_url = $this->constructItemUrl($id);
			// Get the JSON object at the specified URL
			$json = $this->loadJSONUrl($item_url);
			// Return json object in prof.
			if ($json
			    && $json->{FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY}
			    && $json->{FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_PROFESSOR_SINGULAR_KEY_ENTRY}
			) {
				// For each professor in json.
				foreach ($json->{FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_PROFESSOR_SINGULAR_KEY_ENTRY} as $prof) {
					$this->data[$prof->{'@profid'}] = $prof;
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
