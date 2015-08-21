<?php
/**
 * @file
 * faculty_general.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function faculty_general_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'taxonomy_term-deptped-field_code'
  $field_instances['taxonomy_term-deptped-field_code'] = array(
    'bundle' => 'deptped',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_code',
    'label' => 'code',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 31,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('code');

  return $field_instances;
}
