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
  $items['waywire-create-cached-json'] = array(
    'description' => 'Goes through all the waywire videos and sets up the cache table',
  );

  return $items;
}

/**
 * @see waywire_leadership_create_entities_for_all_companies().
 */
function drush_waywire_leadership_waywire_create_all() {
  waywire_leadership_create_entities_for_all_companies();
}

/**
 *
 * @see waywire_leadership_create_json().
 */
function drush_waywire_leadership_waywire_create_cached_json() {
  waywire_leadership_create_json();
}
