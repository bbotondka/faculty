<?php
/**
 * @file
 * faculty_basic_page.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function faculty_basic_page_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_blocs|field_collection_item|field_section|form';
  $field_group->group_name = 'group_blocs';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_section';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Blocs',
    'weight' => '3',
    'children' => array(
      0 => 'field_bloc',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Blocs',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-blocs field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_blocs|field_collection_item|field_section|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|field_collection_item|field_bloc|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bloc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '1',
    'children' => array(
      0 => 'field_content',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Content',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-content field-group-tab',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_content|field_collection_item|field_bloc|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_image|field_collection_item|field_bloc|form';
  $field_group->group_name = 'group_image';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bloc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Image',
    'weight' => '2',
    'children' => array(
      0 => 'field_image',
      1 => 'field_mobile_image',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-image field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_image|field_collection_item|field_bloc|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_infos|field_collection_item|field_bloc|form';
  $field_group->group_name = 'group_infos';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bloc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Infos & options',
    'weight' => '0',
    'children' => array(
      0 => 'field_description',
      1 => 'field_size',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-infos field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_infos|field_collection_item|field_bloc|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_reference|field_collection_item|field_bloc|form';
  $field_group->group_name = 'group_reference';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bloc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Reference',
    'weight' => '5',
    'children' => array(
      0 => 'field_reference',
      1 => 'field_link',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-reference field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_reference|field_collection_item|field_bloc|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_video|field_collection_item|field_bloc|form';
  $field_group->group_name = 'group_video';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bloc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Vidéo',
    'weight' => '4',
    'children' => array(
      0 => 'field_youtube',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-video field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_video|field_collection_item|field_bloc|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_views|field_collection_item|field_bloc|form';
  $field_group->group_name = 'group_views';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bloc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Views',
    'weight' => '12',
    'children' => array(
      0 => 'field_views',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-views field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_views|field_collection_item|field_bloc|form'] = $field_group;

  return $export;
}
