<?php
/**
 * @file
 * faculty_basic_page.views_default.inc
 */

/**
 * Implements hook_views_api().
 *
 * Already declared by features.
 */
/*function faculty_basic_page_views_api() {
	list($module, $api) = func_get_args();
	if ($module == "views" && $api == "views_default") {
		return array("api" => "3.0");
	}
}*/

/**
 * Implements hook_views_default_views().
 */
function faculty_basic_page_views_default_views() {
	$export = array();



	if (empty(views_get_view('home'))) {

		$view = new view();
		$view->name = 'home';
		$view->description = 'Emulates the default Drupal front page (/home path)';
		$view->tag = '';
		$view->base_table = 'node';
		$view->human_name = 'Home';
		$view->core = 0;
		$view->api_version = '3.0';
		$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

		/* Display: Master */
		$handler = $view->new_display('default', 'Master', 'default');
		$handler->display->display_options['use_more_always'] = FALSE;
		$handler->display->display_options['access']['type'] = 'perm';
		$handler->display->display_options['cache']['type'] = 'none';
		$handler->display->display_options['query']['type'] = 'views_query';
		$handler->display->display_options['query']['options']['query_comment'] = FALSE;
		$handler->display->display_options['exposed_form']['type'] = 'basic';
		$handler->display->display_options['pager']['type'] = 'some';
		$handler->display->display_options['pager']['options']['items_per_page'] = '1';
		$handler->display->display_options['pager']['options']['offset'] = '0';
		$handler->display->display_options['style_plugin'] = 'default';
		$handler->display->display_options['row_plugin'] = 'node';
		/* Sort criterion: Content: Sticky */
		$handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
		$handler->display->display_options['sorts']['sticky']['table'] = 'node';
		$handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
		$handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
		/* Sort criterion: Content: Post date */
		$handler->display->display_options['sorts']['created']['id'] = 'created';
		$handler->display->display_options['sorts']['created']['table'] = 'node';
		$handler->display->display_options['sorts']['created']['field'] = 'created';
		$handler->display->display_options['sorts']['created']['order'] = 'DESC';
		/* Filter criterion: Content: Language */
		$handler->display->display_options['filters']['language']['id'] = 'language';
		$handler->display->display_options['filters']['language']['table'] = 'node';
		$handler->display->display_options['filters']['language']['field'] = 'language';
		$handler->display->display_options['filters']['language']['value'] = array(
			'***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
		);
		$handler->display->display_options['filters']['language']['group'] = 1;
		/* Filter criterion: Content: Promoted to front page */
		$handler->display->display_options['filters']['promote']['id'] = 'promote';
		$handler->display->display_options['filters']['promote']['table'] = 'node';
		$handler->display->display_options['filters']['promote']['field'] = 'promote';
		$handler->display->display_options['filters']['promote']['value'] = '1';
		$handler->display->display_options['filters']['promote']['group'] = 1;
		$handler->display->display_options['filters']['promote']['expose']['operator'] = FALSE;
		/* Filter criterion: Content: Published */
		$handler->display->display_options['filters']['status']['id'] = 'status';
		$handler->display->display_options['filters']['status']['table'] = 'node';
		$handler->display->display_options['filters']['status']['field'] = 'status';
		$handler->display->display_options['filters']['status']['value'] = '1';
		$handler->display->display_options['filters']['status']['group'] = 1;
		$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
		/* Filter criterion: Content: Type */
		$handler->display->display_options['filters']['type']['id'] = 'type';
		$handler->display->display_options['filters']['type']['table'] = 'node';
		$handler->display->display_options['filters']['type']['field'] = 'type';
		$handler->display->display_options['filters']['type']['value'] = array(
			'page' => 'page',
		);
		$handler->display->display_options['filters']['type']['group'] = 1;

		/* Display: Page */
		$handler = $view->new_display('page', 'Page', 'page');
		$handler->display->display_options['path'] = 'home';
		$translatables['home'] = array(
			t('Master'),
			t('more'),
			t('Apply'),
			t('Reset'),
			t('Sort by'),
			t('Asc'),
			t('Desc'),
			t('Page'),
		);
		$export['home'] = $view;

	}


	if (empty(views_get_view('home_blocks'))) {

		$view = new view();
		$view->name = 'home_blocks';
		$view->description = '';
		$view->tag = 'default';
		$view->base_table = 'node';
		$view->human_name = 'Home blocks';
		$view->core = 7;
		$view->api_version = '3.0';
		$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

		/* Display: Master */
		$handler = $view->new_display('default', 'Master', 'default');
		$handler->display->display_options['title'] = 'Some figures';
		$handler->display->display_options['use_more_always'] = FALSE;
		$handler->display->display_options['access']['type'] = 'perm';
		$handler->display->display_options['cache']['type'] = 'time';
		$handler->display->display_options['cache']['results_lifespan'] = '518400';
		$handler->display->display_options['cache']['results_lifespan_custom'] = '0';
		$handler->display->display_options['cache']['output_lifespan'] = '518400';
		$handler->display->display_options['cache']['output_lifespan_custom'] = '0';
		$handler->display->display_options['query']['type'] = 'views_query';
		$handler->display->display_options['exposed_form']['type'] = 'basic';
		$handler->display->display_options['pager']['type'] = 'some';
		$handler->display->display_options['pager']['options']['items_per_page'] = '1';
		$handler->display->display_options['pager']['options']['offset'] = '0';
		$handler->display->display_options['style_plugin'] = 'default';
		$handler->display->display_options['row_plugin'] = 'fields';
		/* Field: Content: Some figures */
		$handler->display->display_options['fields']['field_some_figures']['id'] = 'field_some_figures';
		$handler->display->display_options['fields']['field_some_figures']['table'] = 'field_data_field_some_figures';
		$handler->display->display_options['fields']['field_some_figures']['field'] = 'field_some_figures';
		$handler->display->display_options['fields']['field_some_figures']['label'] = '';
		$handler->display->display_options['fields']['field_some_figures']['element_label_colon'] = FALSE;
		$handler->display->display_options['fields']['field_some_figures']['settings'] = array(
			'edit' => '',
			'delete' => '',
			'add' => '',
			'description' => 0,
			'view_mode' => 'full',
		);
		$handler->display->display_options['fields']['field_some_figures']['delta_offset'] = '0';
		$handler->display->display_options['fields']['field_some_figures']['multi_type'] = 'ul';
		/* Sort criterion: Content: Post date */
		$handler->display->display_options['sorts']['created']['id'] = 'created';
		$handler->display->display_options['sorts']['created']['table'] = 'node';
		$handler->display->display_options['sorts']['created']['field'] = 'created';
		$handler->display->display_options['sorts']['created']['order'] = 'DESC';
		/* Filter criterion: Content: Published */
		$handler->display->display_options['filters']['status']['id'] = 'status';
		$handler->display->display_options['filters']['status']['table'] = 'node';
		$handler->display->display_options['filters']['status']['field'] = 'status';
		$handler->display->display_options['filters']['status']['value'] = 1;
		$handler->display->display_options['filters']['status']['group'] = 1;
		$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
		/* Filter criterion: Content: Type */
		$handler->display->display_options['filters']['type']['id'] = 'type';
		$handler->display->display_options['filters']['type']['table'] = 'node';
		$handler->display->display_options['filters']['type']['field'] = 'type';
		$handler->display->display_options['filters']['type']['value'] = array(
			'page' => 'page',
		);
		/* Filter criterion: Content: Promoted to front page */
		$handler->display->display_options['filters']['promote']['id'] = 'promote';
		$handler->display->display_options['filters']['promote']['table'] = 'node';
		$handler->display->display_options['filters']['promote']['field'] = 'promote';
		$handler->display->display_options['filters']['promote']['value'] = '1';
		/* Filter criterion: Content: Language */
		$handler->display->display_options['filters']['language']['id'] = 'language';
		$handler->display->display_options['filters']['language']['table'] = 'node';
		$handler->display->display_options['filters']['language']['field'] = 'language';
		$handler->display->display_options['filters']['language']['value'] = array(
			'***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
		);

		/* Display: Some figures */
		$handler = $view->new_display('block', 'Some figures', 'block');
		$handler->display->display_options['display_description'] = 'Some figures block';
		$handler->display->display_options['block_caching'] = '8';

		/* Display: News */
		$handler = $view->new_display('block', 'News', 'block_1');
		$handler->display->display_options['defaults']['title'] = FALSE;
		$handler->display->display_options['title'] = 'News';
		$handler->display->display_options['display_description'] = 'news (article)';
		$handler->display->display_options['defaults']['pager'] = FALSE;
		$handler->display->display_options['pager']['type'] = 'some';
		$handler->display->display_options['pager']['options']['items_per_page'] = '3';
		$handler->display->display_options['pager']['options']['offset'] = '0';
		$handler->display->display_options['defaults']['fields'] = FALSE;
		/* Field: Content: Title */
		$handler->display->display_options['fields']['title']['id'] = 'title';
		$handler->display->display_options['fields']['title']['table'] = 'node';
		$handler->display->display_options['fields']['title']['field'] = 'title';
		$handler->display->display_options['fields']['title']['label'] = '';
		$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
		/* Field: Content: Image */
		$handler->display->display_options['fields']['field_image']['id'] = 'field_image';
		$handler->display->display_options['fields']['field_image']['table'] = 'field_data_field_image';
		$handler->display->display_options['fields']['field_image']['field'] = 'field_image';
		$handler->display->display_options['fields']['field_image']['label'] = '';
		$handler->display->display_options['fields']['field_image']['element_label_colon'] = FALSE;
		$handler->display->display_options['fields']['field_image']['click_sort_column'] = 'fid';
		$handler->display->display_options['fields']['field_image']['settings'] = array(
			'image_style' => '',
			'image_link' => '',
		);
		/* Field: Content: Mobile image */
		$handler->display->display_options['fields']['field_mobile_image']['id'] = 'field_mobile_image';
		$handler->display->display_options['fields']['field_mobile_image']['table'] = 'field_data_field_mobile_image';
		$handler->display->display_options['fields']['field_mobile_image']['field'] = 'field_mobile_image';
		$handler->display->display_options['fields']['field_mobile_image']['label'] = '';
		$handler->display->display_options['fields']['field_mobile_image']['element_label_colon'] = FALSE;
		$handler->display->display_options['fields']['field_mobile_image']['click_sort_column'] = 'fid';
		$handler->display->display_options['fields']['field_mobile_image']['settings'] = array(
			'image_style' => '',
			'image_link' => '',
		);
		$handler->display->display_options['defaults']['sorts'] = FALSE;
		/* Sort criterion: Content: Sticky */
		$handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
		$handler->display->display_options['sorts']['sticky']['table'] = 'node';
		$handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
		$handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
		/* Sort criterion: Content: Post date */
		$handler->display->display_options['sorts']['created']['id'] = 'created';
		$handler->display->display_options['sorts']['created']['table'] = 'node';
		$handler->display->display_options['sorts']['created']['field'] = 'created';
		$handler->display->display_options['sorts']['created']['order'] = 'DESC';
		$handler->display->display_options['defaults']['filter_groups'] = FALSE;
		$handler->display->display_options['defaults']['filters'] = FALSE;
		/* Filter criterion: Content: Published */
		$handler->display->display_options['filters']['status']['id'] = 'status';
		$handler->display->display_options['filters']['status']['table'] = 'node';
		$handler->display->display_options['filters']['status']['field'] = 'status';
		$handler->display->display_options['filters']['status']['value'] = 1;
		$handler->display->display_options['filters']['status']['group'] = 1;
		$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
		/* Filter criterion: Content: Type */
		$handler->display->display_options['filters']['type']['id'] = 'type';
		$handler->display->display_options['filters']['type']['table'] = 'node';
		$handler->display->display_options['filters']['type']['field'] = 'type';
		$handler->display->display_options['filters']['type']['value'] = array(
			'article' => 'article',
		);
		/* Filter criterion: Content: Language */
		$handler->display->display_options['filters']['language']['id'] = 'language';
		$handler->display->display_options['filters']['language']['table'] = 'node';
		$handler->display->display_options['filters']['language']['field'] = 'language';
		$handler->display->display_options['filters']['language']['value'] = array(
			'***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
		);
		$handler->display->display_options['block_caching'] = '8';

		/* Display: Videos */
		$handler = $view->new_display('block', 'Videos', 'block_2');
		$handler->display->display_options['defaults']['title'] = FALSE;
		$handler->display->display_options['title'] = 'Videos';
		$handler->display->display_options['display_description'] = 'Videos block';
		$handler->display->display_options['defaults']['pager'] = FALSE;
		$handler->display->display_options['pager']['type'] = 'none';
		$handler->display->display_options['pager']['options']['offset'] = '0';
		$handler->display->display_options['defaults']['relationships'] = FALSE;
		/* Relationship: Entity Reference: Referenced Entity */
		$handler->display->display_options['relationships']['field_videos_ref_target_id']['id'] = 'field_videos_ref_target_id';
		$handler->display->display_options['relationships']['field_videos_ref_target_id']['table'] = 'field_data_field_videos_ref';
		$handler->display->display_options['relationships']['field_videos_ref_target_id']['field'] = 'field_videos_ref_target_id';
		$handler->display->display_options['relationships']['field_videos_ref_target_id']['label'] = 'Entité Contenu réferencée par field_videos_ref';
		$handler->display->display_options['defaults']['fields'] = FALSE;
		/* Field: Content: Title */
		$handler->display->display_options['fields']['title']['id'] = 'title';
		$handler->display->display_options['fields']['title']['table'] = 'node';
		$handler->display->display_options['fields']['title']['field'] = 'title';
		$handler->display->display_options['fields']['title']['relationship'] = 'field_videos_ref_target_id';
		$handler->display->display_options['fields']['title']['label'] = '';
		$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
		$handler->display->display_options['defaults']['filter_groups'] = FALSE;
		$handler->display->display_options['defaults']['filters'] = FALSE;
		/* Filter criterion: Content: Published */
		$handler->display->display_options['filters']['status']['id'] = 'status';
		$handler->display->display_options['filters']['status']['table'] = 'node';
		$handler->display->display_options['filters']['status']['field'] = 'status';
		$handler->display->display_options['filters']['status']['value'] = 1;
		$handler->display->display_options['filters']['status']['group'] = 1;
		$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
		/* Filter criterion: Content: Type */
		$handler->display->display_options['filters']['type']['id'] = 'type';
		$handler->display->display_options['filters']['type']['table'] = 'node';
		$handler->display->display_options['filters']['type']['field'] = 'type';
		$handler->display->display_options['filters']['type']['value'] = array(
			'page' => 'page',
		);
		/* Filter criterion: Content: Language */
		$handler->display->display_options['filters']['language']['id'] = 'language';
		$handler->display->display_options['filters']['language']['table'] = 'node';
		$handler->display->display_options['filters']['language']['field'] = 'language';
		$handler->display->display_options['filters']['language']['value'] = array(
			'***CURRENT_LANGUAGE***' => '***CURRENT_LANGUAGE***',
		);
		/* Filter criterion: Content: Promoted to front page */
		$handler->display->display_options['filters']['promote']['id'] = 'promote';
		$handler->display->display_options['filters']['promote']['table'] = 'node';
		$handler->display->display_options['filters']['promote']['field'] = 'promote';
		$handler->display->display_options['filters']['promote']['value'] = '1';
		$handler->display->display_options['block_caching'] = '8';
		$translatables['home_blocks'] = array(
			t('Master'),
			t('Some figures'),
			t('more'),
			t('Apply'),
			t('Reset'),
			t('Sort by'),
			t('Asc'),
			t('Desc'),
			t('Some figures block'),
			t('News'),
			t('news (article)'),
			t('Videos'),
			t('Videos block'),
			t('Entité Contenu réferencée par field_videos_ref'),
		);
		$export['home_blocks'] = $view;

	}

	return $export;
}
