<?php
/**
 * @file
 * Hooks provided by the SMG Global Module
 */

/**
 * Silverpop Webtracking Page View Fields
 * @see http://dev.summitpublish.com/ticket/2301
 * We extract properties from a node (nid, company nids, source
 * type, term tids), and we add them to the "Name" field of
 * silverpop's page view tracking.  The SMG Global module handles
 * the nid, company nids, source type, content type, and subtype,
 * but the term ids need to be added using this hook.
 *
 * @param object $node
 *  Node object for a node view page
 * @param array $silverpop_tracking
 *  Array containing properties of the node that we want
 *  to pass into the Silverpop page view tracking
 */
function hook__silverpop_tracking_add($node, &$silverpop_tracking){
  // Any node information that you want to track, just added it to
  // $silverpop_tracking array.  For example:
  $silverpop_tracking['field_to_track'] = $node->field_to_track['und'][0]['value'];
}
