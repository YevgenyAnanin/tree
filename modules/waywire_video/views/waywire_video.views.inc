<?php

/**
 * @file
 * Providing extra functionality for the Waywire Video UI via views.
 */

/**
 * Implements hook_views_data()
 */
function waywire_video_views_data_alter(&$data) {
  $data['waywire_video']['link_waywire_video'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the waywire video.'),
      'handler' => 'waywire_video_handler_link_field',
    ),
  );
  $data['waywire_video']['edit_waywire_video'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the waywire video.'),
      'handler' => 'waywire_video_handler_edit_link_field',
    ),
  );
  $data['waywire_video']['delete_waywire_video'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the waywire video.'),
      'handler' => 'waywire_video_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows waywire_videos/waywire_video/%id/op
  $data['waywire_video']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this waywire video.'),
      'handler' => 'waywire_video_handler_waywire_video_operations_field',
    ),
  );
}

/**
 * Implements hook_views_default_views().
 */
function waywire_video_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'waywire_videos';
  $view->description = 'A list of all waywire videos.';
  $view->tag = 'waywire_videos';
  $view->base_table = 'waywire_video';
  $view->human_name = 'Waywire Video';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Waywire Videos';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any waywire video type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'id' => 'id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No waywire videos have been created yet';
  /* Field: Waywire Video: Waywire Video ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'waywire_video';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['id']['empty_zero'] = 0;
  /* Field: Waywire Video: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'waywire_video';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: Waywire Video: Node Id */
  $handler->display->display_options['fields']['node_id']['id'] = 'node_id`';
  $handler->display->display_options['fields']['node_id']['table'] = 'waywire_video';
  $handler->display->display_options['fields']['node_id']['field'] = 'node_id';
  $handler->display->display_options['fields']['node_id']['label'] = 'Company Node Id';
  $handler->display->display_options['fields']['node_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['node_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['node_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['node_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['node_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['node_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['node_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['node_id']['empty_zero'] = 0;
  /* Field: Waywire Video: Link */
  $handler->display->display_options['fields']['link_waywire_video']['id'] = 'link_waywire_video';
  $handler->display->display_options['fields']['link_waywire_video']['table'] = 'waywire_video';
  $handler->display->display_options['fields']['link_waywire_video']['field'] = 'link_waywire_video';
  $handler->display->display_options['fields']['link_waywire_video']['label'] = 'View';
  $handler->display->display_options['fields']['link_waywire_video']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_waywire_video']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_waywire_video']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_waywire_video']['empty_zero'] = 0;
  /* Field: Waywire Video: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'waywire_video';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'waywire_videos_admin_page');
  $handler->display->display_options['path'] = 'admin/content/waywire_videos/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Waywire Videos';
  $handler->display->display_options['tab_options']['description'] = 'Manage waywire videos';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['waywire_videos'] = array(
    t('Master'),
    t('Waywire Videos'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty '),
    t('No waywire video have been created yet'),
    t('Waywire Video ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views[] = $view;
  return $views;

}
