<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyUserMigration
 *
 * First version of migration class.
 */
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
          MigrateDestinationUser::getKeySchema()
		);

		// Get base url.
		$base_url = faculty_migrate_get_base_url();
		// Path to json files.
		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/allProf.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyUserListJSON($source_url),
			new FacultyUserItemJSON($source_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: User.
		$this->destination = new MigrateDestinationUser(array(
          'md5_passwords' => 1,
          'language' => 'fr',
          'text_format' => 'filtered_html'
        ));

        // Mapped fields
        $this->addFieldMapping('name', 'prepare_username')
             ->description(t('Created from email in prepare method'));
        $this->addFieldMapping('status')
             ->defaultValue(0);
        $this->addFieldMapping('mail', 'email');
        $this->addFieldMapping('picture', 'prepare_imageid')
          ->sourceMigration('FacultyProfessorImage')
          ->description(t('Created from @photofac in prepare method'));
        // Note that these date/time values are coming in as ISO strings, but
        // Drupal uses UNIX timestamps. The user destination automatically
        // translates them as necessary.
        /*$this->addFieldMapping('created', 'posted');
        $this->addFieldMapping('access', 'last_access');
        $this->addFieldMapping('login', 'last_login');
        $this->addFieldMapping('pass', 'password');
        $this->addFieldMapping('roles', 'positions')
          ->separator(',')
          ->sourceMigration('WineRole');
        $this->addFieldMapping('signature', 'sig');
        $this->addFieldMapping('signature_format')
          ->defaultValue($this->basicFormat->format);
        $this->addFieldMapping('init', 'original_mail');
        $this->addFieldMapping('field_migrate_example_gender', 'sex')
          ->description(t('Map from M/F to 0/1 in prepare method'));*/

      // Unmapped source fields

      // Unmapped destination fields
      $this->addUnmigratedDestinations(array(
          'data',
          'is_new',
          'language',
          'role_names',
          'theme',
          'timezone',
        ));

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
			'prepare_username' => 'Created from email in prepare method',
			'prepare_imageid' => 'Created from @photofac in prepare method',
		);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

        // Check if email exists.
		if (empty($row->email)) {
            return FALSE;
        }

        // Prepare imageid used to attach image to current user.
        if (isset($row->{'@photofac'})
         && $prepare_imageid = $this->get_image_with_extension($row->{'@photofac'})
        ) {
          $row->prepare_imageid = $prepare_imageid;
        }

        // Prepare username from email.
        // Include pathauto.inc file to use pathauto_cleanstring().
        module_load_include('inc', 'pathauto');
        $row->prepare_username = pathauto_cleanstring($row->email);

	}


  /**
   * Get sourceid of file image.
   *
   * @param $filename
   *   Filename without extension.
   *
   * @return bool
   *   Filename with extension or FALSE.
   */
  protected function get_image_with_extension($filename) {
      $result = db_select('migrate_map_facultyprofessorimage', 'mmfpi')
        ->fields('mmfpi', array(
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
		if (!empty($row->related_user_id)) {
			$profile2->uid = $row->related_user_id;
		}

	}

}

/**
 * Class FacultyUserMigrateDestinationProfile2.
 */
class FacultyUserMigrateDestinationProfile2 extends MigrateDestinationProfile2 {
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
 * Class FacultyUserListJSON.
 */
class FacultyUserListJSON extends MigrateListJSON {
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
 * Class FacultyUserItemJSON
 */
class FacultyUserItemJSON extends MigrateItemJSON {
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
				// For each user in json.
				foreach ($json->{FACULTY_MIGRATE_PROFESSOR_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_PROFESSOR_SINGULAR_KEY_ENTRY} as $prof) {
					$this->data[$prof->{FACULTY_MIGRATE_USER_SOURCE_ID}] = $prof;
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
