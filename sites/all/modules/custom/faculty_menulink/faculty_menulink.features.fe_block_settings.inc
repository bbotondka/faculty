<?php
/**
 * @file
 * faculty_menulink.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function faculty_menulink_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['megamenu-main-menu'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'main-menu',
    'module' => 'megamenu',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'bartik',
        'weight' => 0,
      ),
      'faculty' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'faculty',
        'weight' => -24,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}
