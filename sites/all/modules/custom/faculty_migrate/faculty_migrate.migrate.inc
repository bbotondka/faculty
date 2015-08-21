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
            // Import terms of taxonomies.
            'FacultyCategory' => array(
              'class_name' => 'MigrateFacultyCategoryMigration',
              'group_name' => 'faculty',
            ),
            'FacultyTerm' => array(
              'class_name' => 'MigrateFacultyTermMigration',
              'group_name' => 'faculty',
              'dependencies' => array(
                'FacultyCategory',
              ),
            ),
            'FacultyDeptped' => array(
              'class_name' => 'MigrateFacultyDeptpedMigration',
              'group_name' => 'faculty',
            ),
            'FacultyProfessorImage' => array(
              'class_name' => 'MigrateFacultyProfessorImageMigration',
              'group_name' => 'faculty',
            ),
            'FacultyProfessorCv' => array(
              'class_name' => 'MigrateFacultyProfessorCvMigration',
              'group_name' => 'faculty',
            ),
            'FacultyUser' => array(
              'class_name' => 'MigrateFacultyUserMigration',
              'group_name' => 'faculty',
              //'dependencies' => array(
              'soft_dependencies' => array(
                'FacultyProfessorImage',
              ),
            ),
			// Import all entries in list of professors (main json file).
			// profs.json
			'FacultyProfessor' => array(
				'class_name' => 'MigrateFacultyProfessorMigration',
				'group_name' => 'faculty',
              'soft_dependencies' => array(
                'FacultyUser',
                'FacultyProfessorCv',
              ),
			),
			// Import detailed information about each professor.
			// Independant json file (prof_[profid].json).
			'FacultyProfessorDetail' => array(
				'class_name' => 'MigrateFacultyProfessorDetailMigration',
				'group_name' => 'faculty',
				'soft_dependencies' => array(
                    'FacultyDeptped',
					'FacultyUser',
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
