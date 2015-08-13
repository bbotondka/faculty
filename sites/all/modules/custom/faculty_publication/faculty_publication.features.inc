<?php
/**
 * @file
 * faculty_publication.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function faculty_publication_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_eck_bundle_info().
 */
function faculty_publication_eck_bundle_info() {
  $items = array(
    'publication_book' => array(
      'machine_name' => 'publication_book',
      'entity_type' => 'publication',
      'name' => 'book',
      'label' => 'Book',
    ),
    'publication_case_study' => array(
      'machine_name' => 'publication_case_study',
      'entity_type' => 'publication',
      'name' => 'case_study',
      'label' => 'Case study',
    ),
    'publication_scientific_article' => array(
      'machine_name' => 'publication_scientific_article',
      'entity_type' => 'publication',
      'name' => 'scientific_article',
      'label' => 'Scientific article',
    ),
    'publication_symposium_statement' => array(
      'machine_name' => 'publication_symposium_statement',
      'entity_type' => 'publication',
      'name' => 'symposium_statement',
      'label' => 'Symposium statement',
    ),
    'publication_undefined_type' => array(
      'machine_name' => 'publication_undefined_type',
      'entity_type' => 'publication',
      'name' => 'undefined_type',
      'label' => 'Undefined type',
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function faculty_publication_eck_entity_type_info() {
  $items = array(
    'publication' => array(
      'name' => 'publication',
      'label' => 'Publication',
      'properties' => array(
        'title' => array(
          'label' => 'Title',
          'type' => 'text',
          'behavior' => 'title',
        ),
        'uid' => array(
          'label' => 'Author',
          'type' => 'integer',
          'behavior' => 'author',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'changed' => array(
          'label' => 'Changed',
          'type' => 'integer',
          'behavior' => 'changed',
        ),
        'language' => array(
          'label' => 'Entity language',
          'type' => 'language',
          'behavior' => 'language',
        ),
        'publicationid' => array(
          'label' => 'publicationid',
          'type' => 'integer',
          'behavior' => '',
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_node_info().
 */
function faculty_publication_node_info() {
  $items = array(
    'book' => array(
      'name' => t('Book'),
      'base' => 'node_content',
      'description' => t('Content type "Book" used to render publication of type "book" from json files.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'case_study' => array(
      'name' => t('Case study'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
