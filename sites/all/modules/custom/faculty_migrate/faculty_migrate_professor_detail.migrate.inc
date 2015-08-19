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
    protected $term_code_by_row = array();

	public function __construct($arguments) {
		// Call parent constructor.
		parent::__construct($arguments);
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
            FacultyProfessorMigrateDestinationProfile2::getKeySchema()
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
		$this->destination = new FacultyProfessorMigrateDestinationProfile2(FACULTY_MIGRATE_PROFESSOR_DETAIL_PROFILE_MACHINE_NAME);

		// Destination - Source mapping.
		// Save original id from json file to retrieve user related to this profile.
		$this->addFieldMapping('field_professor_id', FACULTY_MIGRATE_USER_SOURCE_ID);
		$this->addFieldMapping('uid')
		     ->defaultValue(1);

		// Deptped.
		$this->addFieldMapping('field_deptped_ref', 'deptped')
             ->callbacks(array($this, 'get_lib_fr'));
        // Try to create term if doesn't exist.
        $this->addFieldMapping('field_deptped_ref:create_term')
          ->defaultValue(TRUE);
        //$this->addFieldMapping('field_deptped_ref:ignore_case')
        //  ->defaultValue(TRUE);

	}

    /**
     * Get lib_fr from category entry.
     *
     * @param $value
     * @return mixed
     */
    protected function get_lib_fr($value) {
      $current_key = $this->source->getCurrentKey();

      if (empty($current_key['@profid'])) {
        watchdog('faculty_migrate', 'Invalid @profid on <pre>@current_key</pre>. Value: <pre>@value</pre>.', array(
            '@value' => print_r($value, TRUE),
            '@current_key' => print_r($current_key, TRUE)
          ), WATCHDOG_ERROR);

        return t('Uncategorized');
      }

      if (empty($value->lib_fr)) {
        watchdog('faculty_migrate', 'Invalid lib_fr entry on <pre>@current_key</pre>. Value: <pre>@value</pre>.', array(
            '@value' => print_r($value, TRUE),
            '@current_key' => print_r($current_key, TRUE)
          ), WATCHDOG_ERROR);

        return t('Uncategorized');
      }

      if (empty($value->{'@code'})) {
        watchdog('faculty_migrate', 'Invalid @code entry on <pre>@current_key</pre>. Value: <pre>@value</pre>.', array(
            '@value' => print_r($value, TRUE),
            '@current_key' => print_r($current_key, TRUE)
          ), WATCHDOG_ERROR);

        return t('Uncategorized');
      }

      // @Todo: Bad perf here. Best way : create new class to import all terms before.
      // We save terms by row and code.
      if ($current_field_mapping_key = $this->get_current_field_mapping($value->{'@code'})) {
        $this->term_code_by_row[$current_key['@profid']][$current_field_mapping_key][$value->{'@code'}] = $value->lib_fr;
      }

      return $value->lib_fr;
    }

  /**
   * Get current field mapping key from @code.
   *
   * @param $code
   *   '@code' entry.
   * @return bool|int|string
   *   Return key or FALSE.
   */
  protected function get_current_field_mapping($code) {
    $current_row = $this->source->current();

    // For each row.
    foreach($current_row as $key => $entry) {
      if (!is_object($entry)) {
        continue;
      }

      if (empty($entry->{'@code'})) {
        continue;
      }

      if ($entry->{'@code'} !== $code) {
        continue;
      }

      // Key match with $code.
      return $key;
    }

    // Default value;
    return FALSE;
  }

	/**
	 * Get entry of each row from json source.
	 *
	 * @return array
	 */
	public function fields_from_json() {
		return array(
			FACULTY_MIGRATE_USER_SOURCE_ID => 'Unique ID for each source data row',
			//'other_entry' => 'Other entry from separated json file'
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

	public function complete($profile2, stdClass $row) {

      // @Todo: Bad perf here. Best way : create new class to import all terms before.
      $this->set_code_in_related_terms($profile2, $row);
    }

  /**
   * Set field_code for related terms.
   *
   * @Todo: Bad perf here. Best way : create new class to import all terms before.
   *
   * @param $profile2
   * @param stdClass $row
   */
  protected function set_code_in_related_terms($profile2, stdClass $row) {
      // Skip empty term on current row.
      if (empty($this->term_code_by_row[$row->{'@profid'}])) {
        return;
      }

      // For each field_name to attach @code.
      foreach($this->term_code_by_row[$row->{'@profid'}] as $key => $term_value) {
        $term_field_name = 'field_' . $key . '_ref';

        // For each term.
        foreach($term_value as $code => $term_name) {

          $profile2_wrapper = entity_metadata_wrapper('profile2', $profile2);

          // Wrap term
          if ($profile2_wrapper->{$term_field_name}->getIdentifier()) {
            $term_wrapper = entity_metadata_wrapper('taxonomy_term', $profile2_wrapper->{$term_field_name}->tid->value());

            $term_wrapper->field_code->set($code);
            $term_wrapper->save();
          }

        }

      }
    }

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
