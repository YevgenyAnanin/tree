<?php
/**
 * @file
 * Drush commands
 */

/**
 * Implements waywire_video_command().
 */
function waywire_video_drush_command() {
  $items = array();
  $items['waywire-video-delete'] = array(
    'description' => 'Delete Waywire Videos',
    'arguments' => array(
      'node_id' => 'Company node id that is associated to the company.',
    ),
  );
  return $items;
}

/**
 * Deletes the waywire videos entityes.
 *
 * @param integer $node_id
 *   The node id (company node id).
 */
function drush_waywire_video_delete($node_id) {
  if (drush_confirm('This may delete all waywire videos. Are you sure you want to continue?')) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'waywire_video');
    $query->propertyCondition('node_id', $node_id);
    $result = $query->execute();

    if ($result['waywire_video']) {
      $ids = array_keys($result['waywire_video']);
      $waywire_videos = waywire_video_load_multiple($ids);
      foreach ($waywire_videos as $waywire_video) {
        echo "Deleted {$waywire_video->name}\n";
        waywire_video_delete($waywire_video);
      }
    }
  }
}
