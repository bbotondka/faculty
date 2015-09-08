<?php

function faculty_preprocess_page(&$variables) {
  drupal_add_js(drupal_get_path('theme', 'faculty') .'/build/scripts/vendor/modernizr-2.8.3.min.js', array("group" => JS_LIBRARY, "weight" => 115));
}


/**
 * Implements hook_css_alter().
 * Forces style.css to be included with <link> tag instead of drupals default @import
 * - because browserSync doesn't support @import.
 * Code from: https://www.drupal.org/project/link_css
 */

function faculty_css_alter(&$css) {
  $count = 0;

  foreach ($css as $key => $value) {
    // Skip core files.
    $is_core = (strpos($value['data'], 'misc/') === 0 || strpos($value['data'], 'modules/') === 0);
    if (!$is_core) {
      // This option forces embeding with a link element.
      $css[$key]['preprocess'] = FALSE;
      $count++;
    }
  }
}

/**
 * Implements hook_js_alter().
 */

function faculty_js_alter(&$javascript) {
  if (isset($javascript['sites/all/modules/contrib/tb_megamenu/js/tb-megamenu-frontend.js'])) {
    $javascript['sites/all/modules/contrib/tb_megamenu/js/tb-megamenu-frontend.js']['data'] = drupal_get_path('theme', 'faculty') . '/src/scripts/js/megamenu.js';
  }
}

/**
 * Implements hook_theme().
 */
function faculty_theme() {
	$template_path = drupal_get_path('theme', 'faculty') . '/templates';

	return array(
		'faculty_top_image_content' => array(
			'variables' => array(),
			'template' => 'faculty-top-image-content',
			'path' => $template_path,
		)
	);
}

/**
 * Implements hook_preprocess_node.
 *
 * Add theme hook suggestion
 */
function faculty_preprocess_node(&$vars) {
	// Add node--[type]--[view_mode].tpl.php.
	array_unshift($vars['theme_hook_suggestions'], 'node__' . $vars['node']->type . '__' . $vars['view_mode']);
	// Add node--[nid]--[view_mode].tpl.php.
	array_unshift($vars['theme_hook_suggestions'], 'node__' . $vars['node']->nid . '__' . $vars['view_mode']);
	// Add node--[view_mode].tpl.php.
	array_unshift($vars['theme_hook_suggestions'], 'node__' . $vars['view_mode']);
}

/**
 * Implements theme_field().
 *
 * Sanitize field_image of node page type.
 * We render image url without div and space
 * to use background-image css property.
 *
 * See https://api.drupal.org/api/drupal/modules!field!field.module/function/theme_field/7.
 */
function faculty_field__field_image__page($variables) {
	return _faculty_sanitize_image_url_when_rendering($variables);
}

/**
 * Implements theme_field().
 *
 * Sanitize field_image of node page type.
 * We render image url without div and space
 * to use background-image css property.
 *
 * See https://api.drupal.org/api/drupal/modules!field!field.module/function/theme_field/7.
 */
function faculty_field__field_mobile_image__page($variables) {
	return _faculty_sanitize_image_url_when_rendering($variables);
}

/**
 * Implements theme_links().
 *
 * Alter language switcher block.
 * See https://www.drupal.org/node/1369090.
 *
 * @param $vars
 *
 * @return string
 */
function faculty_links__locale_block(&$vars) {
	foreach($vars['links'] as $language => $langInfo) {
		// Get first part of langcode.
		$abbr = explode('-', $langInfo['language']->language);
		$abbr = (!empty($abbr[0])) ?$abbr[0] : $language;

		$name = $langInfo['language']->native;
		$vars['links'][$language]['title'] = '<abbr title="' . $name . '">' . $abbr . '</abbr>';
		$vars['links'][$language]['html'] = TRUE;
	}
	$content = theme_links($vars);
	return $content;
}

/**
 * Sanitize image_url_formatter. We remove div and space.
 *
 * @param $variables
 *   Variables to render items.
 *
 * @return string
 *   Sanitized output or output.
 */
function _faculty_sanitize_image_url_when_rendering($variables) {
	$output = '';
	$sanitized_output = '';
	$is_image_url_formatter = FALSE;

	// Render the label, if it's not hidden.
	if (!$variables['label_hidden']) {
		$output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables ['label'] . ':&nbsp;</div>';
	}

	// Render the items.
	$output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
	foreach ($variables['items'] as $delta => $item) {

		// Detection of image_url_formatter.
		if ((!$is_image_url_formatter)
		 && (!empty($item['#theme']))
		 && ($item['#theme'] === 'image_url_formatter')
		) {
			$is_image_url_formatter = TRUE;
		}

		$classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
		$output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
		// Just render item without div.
		$sanitized_output .= drupal_render($item);
	}
	$output .= '</div>';

	// Render the top-level DIV.
	$output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

	// Return $sanitized_output or input.
	return $is_image_url_formatter ? $sanitized_output : $output;
}

/**
 * Implements template_preprocess_entity().
 *
 * Here we add faculty_preprocess_field_collection_item().
 */
function faculty_preprocess_entity(&$variables, $hook) {
  $function = 'faculty_preprocess_' . $variables['entity_type'];
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}

/**
 * Field Collection-specific implementation of template_preprocess_entity().
 */
function faculty_preprocess_field_collection_item(&$vars) {

  switch($vars['elements']['#entity']->field_name) {
    case 'field_section':
      // We use custom tpl to render this collection.
      $vars['theme_hook_suggestions'][] = 'field__collection_sections';

      // @Todo: Add field to extract here.
      $field_array = array(
        'field_section_title',
        'field_section_desc',
        'field_section_option',
        'field_bloc' => array(
          'field_size',
          'field_content'
        )
      );

      _faculty_basic_page_rows_from_field_section($vars, $field_array);

      break;
  }

}
