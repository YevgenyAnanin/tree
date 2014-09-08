<?php
/**
 * @file
 * Drush commands
 */

/**
 * Implements hook_drush_command().
 */
function waywire_leadership_drush_command() {
  $items['waywire-create-all'] = array(
    'description' => 'Create/Update Waywire videos for all Youtube amplify companies.',
  );

  return $items;
}

/**
 * See waywire_leadership_create_entities_for_all_companies().
 */
function drush_waywire_leadership_waywire_create_all() {
  waywire_leadership_create_entities_for_all_companies();
}
