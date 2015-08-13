<?php
/**
 * @file
 * faculty_general.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function faculty_general_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-copyright-menu.
  $menus['menu-copyright-menu'] = array(
    'menu_name' => 'menu-copyright-menu',
    'title' => 'Copyright menu',
    'description' => 'Copyright menu in footer region.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-header-menu.
  $menus['menu-header-menu'] = array(
    'menu_name' => 'menu-header-menu',
    'title' => 'Header menu',
    'description' => 'Secondary menu in header region.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-partner-menu.
  $menus['menu-partner-menu'] = array(
    'menu_name' => 'menu-partner-menu',
    'title' => 'Partner menu',
    'description' => 'Partner links in footer menu.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-quick-menu.
  $menus['menu-quick-menu'] = array(
    'menu_name' => 'menu-quick-menu',
    'title' => 'Quick menu',
    'description' => 'Quick links in footer region.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Exported menu: menu-social-menu.
  $menus['menu-social-menu'] = array(
    'menu_name' => 'menu-social-menu',
    'title' => 'Social menu',
    'description' => 'Social menu in footer.',
    'language' => 'und',
    'i18n_mode' => 5,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Copyright menu');
  t('Copyright menu in footer region.');
  t('Header menu');
  t('Main menu');
  t('Partner links in footer menu.');
  t('Partner menu');
  t('Quick links in footer region.');
  t('Quick menu');
  t('Secondary menu in header region.');
  t('Social menu');
  t('Social menu in footer.');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');


  return $menus;
}
