<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Class MigrateFacultyCategoryMigration
 *
 * First version of migration class.
 */
class MigrateFacultyCategoryMigration extends Migration {

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
		// Add description of the current class.
		$this->description = t('Import category from json file');

		// Define unique ID from source used to match content
		// with drupal taxonomy_term.
		$this->map = new MigrateSQLMap($this->machineName,
			array(
              FACULTY_MIGRATE_CATEGORY_SOURCE_ID => array(
                  'type' => 'int',
                  'unsigned' => TRUE,
                  'not null' => TRUE,
				)
			),
          MigrateDestinationTaxonomyVocabulary::getKeySchema('taxonomy_vocabulary')
		);

		// Get base url.
		$base_url = faculty_migrate_get_base_url();
		// Path to json files.
		$source_url = $base_url . '/' . drupal_get_path('module', 'faculty_migrate') . '/data/allTypeUnit.json';

		// Define source of migration.
		$this->source = new MigrateSourceList(
			new FacultyCategoryListJSON($source_url),
			new FacultyCategoryItemJSON($source_url, array()),
			$this->fields_from_json()
		);

		// Destination of migration: taxonomy term.
		$this->destination = new MigrateDestinationTaxonomyVocabulary('taxonomy_vocabulary');

		// Destination - Source mapping.
        $this->addFieldMapping('name', 'lib_fr');
        $this->addFieldMapping('machine_name', '@code');
        $this->addFieldMapping('hierarchy')
             ->defaultValue(0);
        $this->addFieldMapping('module')
             ->defaultValue('taxonomy');
        $this->addFieldMapping('weight')
             ->defaultValue(0);

	}

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_CATEGORY_SOURCE_ID => 'Unique ID for each source data row',
		);
	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

        // Skip empty name.
        if (empty($row->lib_fr)) {
          return FALSE;
        }

	}

	public function prepare($term, stdClass $row) {}

}

/**
 * Class FacultyCategoryListJSON.
 */
class FacultyCategoryListJSON extends MigrateListJSON {
  /**
   * Originally this class assumed that the results would be all that was in the JSON
   * object. This class has been modified to look inside the JSON object and find the
   * expected data within the ['results'] member.
   */
  protected function getIDsFromJSON(array $data) {
    $ids = array();

    // Pass main key entry.
    $data = $data[FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY];

    foreach ($data[FACULTY_MIGRATE_CATEGORY_SINGULAR_KEY_ENTRY] as $item) {
      $ids[] = $item[FACULTY_MIGRATE_CATEGORY_SOURCE_ID];
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
        $data = $data[FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY];

        $count = count($data[FACULTY_MIGRATE_CATEGORY_SINGULAR_KEY_ENTRY]);
      }
    }
    return $count;
  }
}

/**
 * Class FacultyCategoryItemJSON
 */
class FacultyCategoryItemJSON extends MigrateItemJSON {
  protected $data = array();

	public function getItem($id) {

		if (empty($this->data)) {

			$item_url = $this->constructItemUrl($id);
			// Get the JSON object at the specified URL
			$json = $this->loadJSONUrl($item_url);
			// Return json object in term.
			if ($json
			    && $json->{FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY}
			    && $json->{FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_CATEGORY_SINGULAR_KEY_ENTRY}
			) {
				// For each term in json.
				foreach ($json->{FACULTY_MIGRATE_CATEGORY_PLURAL_KEY_ENTRY}->{FACULTY_MIGRATE_CATEGORY_SINGULAR_KEY_ENTRY} as $term) {
					$this->data[$term->{FACULTY_MIGRATE_CATEGORY_SOURCE_ID}] = $term;
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

/**
 * Class MigrateDestinationTaxonomyVocabulary.
 *
 * Try to remove related terms when rollback vocabulary.
 */
class MigrateDestinationTaxonomyVocabulary extends MigrateDestinationTable {

  /**
   * Delete a single row.
   *
   * @param array $ids
   *   The primary key values of the row to be deleted.
   */
  public function rollback(array $ids) {
    migrate_instrument_start('table rollback');
    $keys = array_keys(self::getKeySchema($this->tableName));
    $values = array_combine($keys, $ids);
    $this->prepareRollback($values);
    /*$delete = db_delete($this->tableName);
    foreach ($values as $key => $value) {
      $delete->condition($key, $value);
    }
    $delete->execute();*/

    // For each vocabulary id to delete.
    foreach ($ids as $vid) {
      // Try to remove properly related terms.
      taxonomy_vocabulary_delete($vid);
    }

    $this->completeRollback($values);
    migrate_instrument_stop('table rollback');
  }

}
