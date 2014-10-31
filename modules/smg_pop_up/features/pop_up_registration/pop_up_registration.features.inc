<?php
/**
 * @file
 * pop_up_registration.features.inc
 */

/**
 * Implementation of hook_node_info().
 */
function pop_up_registration_node_info() {
  $items = array(
    'pop_up_registration' => array(
      'name' => t('Pop-Up Registration'),
      'base' => 'node_content',
      'description' => t('Leadwise Pop Up Registration box.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
