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
				//'dependencies' => array(
				'soft_dependencies' => array(
					'FacultyProfessor',
				),
			),
			'FacultyProfessorImage' => array(
				'class_name' => 'MigrateFacultyProfessorImageMigration',
				'group_name' => 'faculty',
                //'dependencies' => array(
                'soft_dependencies' => array(
					'FacultyProfessor',
				),
			),
			'FacultyProfessorCv' => array(
				'class_name' => 'MigrateFacultyProfessorCvMigration',
				'group_name' => 'faculty',
                //'dependencies' => array(
                'soft_dependencies' => array(
					'FacultyProfessor',
				),
			),
			'FacultyPublication' => array(
				'class_name' => 'MigrateFacultyPublicationMigration',
				'group_name' => 'faculty',
                //'dependencies' => array(
                'soft_dependencies' => array(
                  'FacultyProfessor',
                ),
			),
			'FacultyPublicationBookDetail' => array(
				'class_name' => 'MigrateFacultyPublicationBookDetailMigration',
				'group_name' => 'faculty',
                //'dependencies' => array(
                'soft_dependencies' => array(
					'FacultyPublication',
				),
			),
			'FacultyPublicationCaseStudyDetail' => array(
				'class_name' => 'MigrateFacultyPublicationCaseStudyDetailMigration',
				'group_name' => 'faculty',
                //'dependencies' => array(
                'soft_dependencies' => array(
					'FacultyPublication',
				),
			),
		),
	);
	return $api;
}
