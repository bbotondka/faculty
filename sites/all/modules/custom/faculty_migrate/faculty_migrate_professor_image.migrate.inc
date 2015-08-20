<?php

// Fix bug with global functions file not found.
include_once('faculty_migrate_global_functions.inc');

/**
 * Migration for files.
 */
class MigrateFacultyProfessorImageMigration extends Migration {

	protected $baseDir;

	public function __construct($arguments) {
		parent::__construct($arguments);
		$this->description = t('Import images of professors.');
		$this->baseDir = drupal_get_path('module', 'faculty_migrate') . '/data';

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'sourceid' => array(
					'type' => 'varchar',
					'length' => 255,
					'not null' => TRUE,
					'description' => t('Source ID'),
				),
			),
			MigrateDestinationFile::getKeySchema()
		);

		$directories = array(
			$this->baseDir,
		);

		// Edit to include the desired extensions.
		$allowed = 'jpg jpeg gif png';
		if (module_exists('file_entity')) {
			$allowed = variable_get('file_entity_default_allowed_extensions', $allowed);
		}
		$file_mask = '/^photo_[0-9]*\.(' . str_replace(array(',', ' '), '|', $allowed) . ')$/i';
		$list = new MigrateListFiles($directories, $this->baseDir, $file_mask);
		// Send FALSE as second argument to prevent loading of file data, which we
		// don't need.
		$item = new MigrateItemFile($this->baseDir, FALSE);
		$fields = array('sourceid' => t('File name with path'));
		$this->source = new MigrateSourceList($list, $item, $fields);
		$this->destination = new MigrateDestinationFile('file', 'MigrateFileUri');

        // File exists replace.
        $this->addFieldMapping('file_replace')
          ->defaultValue(FILE_EXISTS_REPLACE);

		// Save to the default file scheme.
		$this->addFieldMapping('destination_dir')
		     ->defaultValue(variable_get('file_default_scheme', 'public') . '://');
		// Use the full file path in the file name so that we retain the directory
		// structure.
		$this->addFieldMapping('destination_file', 'destination_file');
		// Set the value to the file name, including path.
		$this->addFieldMapping('value', 'file_uri');
	}

	public function prepareRow($row) {
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		$row->file_uri = $this->baseDir . $row->sourceid;

		// Remove the leading forward slash.
		$row->destination_file = substr($row->sourceid, 1);

	}

	public function complete($image, stdClass $row) {

		// Try to attach current image to related user.
		//faculty_migrate_attach_image_to_related_user($image, $row);

	}

}
