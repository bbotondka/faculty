<?php
/**
 * @file
 * Declares our migrations.
 */

/**
 * Implements hook_migrate_api().
 */
function faculty_migrate_migrate_api() {
	$api = array(
		'api' => 2,
		'groups' => array(
			'faculty' => array(
				'title' => t('Faculty Migrations'),
			),
		),
		'migrations' => array(
			// Import all entries in list of professors (main json file).
			// profs.json
			'FacultyProfessor' => array(
				'class_name' => 'MigrateFacultyProfessorMigration',
				'group_name' => 'faculty',
			),
			// Import detailed information about each professor.
			// Independant json file (prof_[profid].json).
			'FacultyProfessorDetail' => array(
				'class_name' => 'MigrateFacultyProfessorDetailMigration',
				'group_name' => 'faculty',
				'dependencies' => array(
					'FacultyProfessor',
				),
			),
			'FacultyPublication' => array(
				'class_name' => 'MigrateFacultyPublicationMigration',
				'group_name' => 'faculty',
			),
			'FacultyPublicationBookDetail' => array(
				'class_name' => 'MigrateFacultyPublicationBookDetailMigration',
				'group_name' => 'faculty',
				'dependencies' => array(
					'FacultyPublication',
				),
			),
		),
	);
	return $api;
}
