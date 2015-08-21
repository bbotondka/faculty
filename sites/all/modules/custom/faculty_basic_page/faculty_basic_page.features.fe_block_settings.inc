<?php
/**
 * @file
 * faculty_basic_page.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function faculty_basic_page_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['views-home_blocks-block'] = array(
    'cache' => 8,
    'custom' => 0,
    'delta' => 'home_blocks-block',
    'module' => 'views',
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
        'region' => 'bottom_grey_content',
        'status' => 1,
        'theme' => 'faculty',
        'weight' => 0,
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

  $export['views-home_blocks-block_1'] = array(
    'cache' => 8,
    'custom' => 0,
    'delta' => 'home_blocks-block_1',
    'module' => 'views',
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
        'region' => 'middle_white_content',
        'status' => 1,
        'theme' => 'faculty',
        'weight' => 0,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '<none>',
    'visibility' => 0,
  );

  $export['views-home_blocks-block_2'] = array(
    'cache' => 8,
    'custom' => 0,
    'delta' => 'home_blocks-block_2',
    'module' => 'views',
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
        'region' => 'middle_grey_content',
        'status' => 1,
        'theme' => 'faculty',
        'weight' => 0,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '<none>',
    'visibility' => 0,
  );

  return $export;
}
