<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyTermMigration
 *
 * Second version of migration class.
 */
class MigrateFacultyTermMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);

		// Add description of the current class.
		$this->description = t('Import terms from json file');

		// Define unique ID from source used to match content
		// with drupal entities (profile2).
		$this->map = new MigrateSQLMap($this->machineName,
			array(
              FACULTY_MIGRATE_CATEGORY_SOURCE_ID => array(
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
		$faculty_migrate_path = drupal_get_path('module', 'faculty_migrate');
		$list_url = $base_url . '/' . $faculty_migrate_path . '/data/allTypeUnit.json';
		$item_url = $base_url . '/' . $faculty_migrate_path . '/data/category_:id.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyTermListJSON($list_url, array(
              'item_url' => $item_url
            )),
			new FacultyTermItemJSON($item_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: profile2 entity type.
        // Tmp bundle "tags".
		$this->destination = new MigrateDestinationTerm('tags');

		// Destination - Source mapping.
        // Dynamically set vocabulary name.
        $this->addFieldMapping('vocabulary_machine_name', 'vname');
             //->sourceMigration('FacultyCategory');

        $this->addFieldMapping('name', 'lib_fr');

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_CATEGORY_SOURCE_ID => 'Unique ID for each source data row',
            'lib_fr' => 'French term name',
            'vname' => 'Vocabulary machine name (here voc code)'
		);
	}

	public function prepareRow($row) {
        // Extract voc and term code from $id.
        //$voc_term_code = explode('_', $row->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID});
        //$voc_code = $voc_term_code[0];
        // Set term code.
        //$row->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID} = $voc_term_code[1];

		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

        if (empty($row->lib_fr)) {
            return FALSE;
        }

        if (empty($row->vocabulary_code)) {
          return FALSE;
        }
        // Dynamically set vocabulary name.
        $row->vname = $row->vocabulary_code;

	}

	public function prepare($profile2, stdClass $row) {}

	public function complete($profile2, stdClass $row) {}

}

/**
 * Class FacultyTermListJSON.
 */
class FacultyTermListJSON extends MigrateListJSON {

  protected $build_data = array();

  /**
   * Originally this class assumed that the results would be all that was in the JSON
   * object. This class has been modified to look inside the JSON object and find the
   * expected data within the ['results'] member.
   */
  protected function getIDsFromJSON(array $data) {
    $ids = array();

    // Pass main key entry.
    $data = $data[FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY];

    // For each vocabulary.
    foreach ($data[FACULTY_MIGRATE_CATEGORY_SINGULAR_KEY_ENTRY] as $item) {
      $current_vocabulary_code = $item[FACULTY_MIGRATE_CATEGORY_SOURCE_ID];

      // Get build data from cache.
      if (!empty($this->build_data[$current_vocabulary_code])) {
        $terms_code_from_cache = array_keys($this->build_data[$current_vocabulary_code]);

        foreach ($terms_code_from_cache as $term_code) {
          $ids[] = $current_vocabulary_code . '_' . $term_code;
        }

        // Skip now with ids from cache.
        return $ids;
      }

      // No build data from cache.

      $json = $this->getJSONFromVocabularyCode($current_vocabulary_code);

      if ($json
        && $json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}
        && $json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY}
      ) {
        // For each term in json.
        foreach ($json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY} as $term) {
          // Add current category code.
          $term->vocabulary_code = $current_vocabulary_code;
          // Populate build data.
          $this->build_data[$current_vocabulary_code][$term->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID}] = $term;
          //
          $ids[] = $current_vocabulary_code . '_' . $term->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID};

        }

      }

    }

    return $ids;
  }

  /**
   * Get JSON from vocabulary code.
   *
   * @param $vocabulary_code
   * @return mixed|null
   */
  protected function getJSONFromVocabularyCode($vocabulary_code) {
    $item_url = $this->constructItemUrl($vocabulary_code, $this->httpOptions['item_url']);
    // Get the JSON object at the specified URL
    $json = $this->loadJSONUrl($item_url);
    if ($json) {
      return $json;
    }
    else {
      $migration = Migration::currentMigration();
      $message =  t('Loading of !objecturl failed:', array(
          '!objecturl' => $item_url
        ));
      $migration->getMap()->saveMessage(
        array($vocabulary_code), $message, MigrationBase::MESSAGE_ERROR);
    }

    return NULL;
  }

  /**
   * Default JSON loader - just pull and decode. This can be overridden for
   * preprocessing of JSON (removal of unwanted elements, caching of JSON if the
   * source service is slow, etc.)
   */
  protected function loadJSONUrl($item_url) {
    if (empty($this->httpOptions)) {
      $json = file_get_contents($item_url);
    }
    else {
      $response = drupal_http_request($item_url, $this->httpOptions);
      $json = $response->data;
    }
    return json_decode($json);
  }

  /**
   * The default implementation simply replaces the :id token in the URL with
   * the ID obtained from MigrateListJSON. Override if the item URL is not
   * so easily expressed from the ID.
   *
   * @param mixed $id
   */
  protected function constructItemUrl($id, $itemUrl) {
    return preg_replace(array_fill(0, count($id), '/:id/'), $id, $itemUrl, 1);
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
        $data = $data[FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY];

        // For each vocabulary.
        foreach ($data[FACULTY_MIGRATE_CATEGORY_SINGULAR_KEY_ENTRY] as $item) {
          $current_vocabulary_code = $item[FACULTY_MIGRATE_CATEGORY_SOURCE_ID];

          $json = $this->getJSONFromVocabularyCode($current_vocabulary_code);

          if ($json
            && $json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}
            && $json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY}
          ) {

            // For each term in json.
            foreach ($json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY} as $term) {
              // Add current category code.
              $term->vocabulary_code = $current_vocabulary_code;

              // Populate buid data to optimize perf.
              $this->build_data[$current_vocabulary_code][$term->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID}] = $term;

            }

            $count += count($json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY});

          }

        }

      }
    }
    return $count;
  }
}

/**
 * Class FacultyTermItemJSON
 */
class FacultyTermItemJSON extends MigrateItemJSON {
  protected $build_data = array();

  public function getItem($id) {

    // Extract voc and term code from $id.
    $voc_term_code = explode('_', $id);
    $voc_code = $voc_term_code[0];
    $term_code = $voc_term_code[1];

    if (empty($this->build_data[$voc_code])) {

      $item_url = $this->constructItemUrl($voc_code);
      // Get the JSON object at the specified URL
      $json = $this->loadJSONUrl($item_url);
      // Return json object in prof.
      if ($json
        && $json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}
        && $json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY}
      ) {
        // For each professor in json.
        foreach ($json->{FACULTY_MIGRATE_TERM_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_TERM_SINGULAR_KEY_ENTRY} as $term) {
          // Add current vocabulary code.
          $term->vocabulary_code = $voc_code;

          $this->build_data[$voc_code][$term->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID}] = $term;
        }

      }
      else {
        $migration = Migration::currentMigration();
        $message =  t('Loading of !objecturl failed:', array('!objecturl' => $item_url));
        $migration->getMap()->saveMessage(
          array($voc_code), $message, MigrationBase::MESSAGE_ERROR);
        return NULL;
      }

    }

    // Check existing data.
    if (empty($this->build_data[$voc_code][$term_code])) {
      return NULL;
    }

    return $this->build_data[$voc_code][$term_code];
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
