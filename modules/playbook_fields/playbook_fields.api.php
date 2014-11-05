<?php
/**
 * @file
 * Hooks provided by the Playbook Fields Module
 */

/**
 * Make any last any updates to the the fields.
 *
 * @param object $node
 *  Webform node object
 * @param array $fields
 *  an array holding all the data for all the fields. Here is an example field:
 *
 *  playbook_name => array(
 *   "name" => "Playbook name",
 *   "type" => "smg_hidden",
 *   "accela_id" => "",
 *   "leadworks_id" => "source",
 *   "silverpop_field" => "",
 *   "salesforce_field" => "",
 *   "gotowebinar_field" => "",
 *   "values" => array(
 *      0  => "Food Safety Playbook",
 *    ),
 *   "multiple" => FALSE,
 *   "prepend" => FALSE,
 *   "dependency" => FALSE,
 *  ),
 *
 * @return $fields
 *  an array of all the fields (with any updates needed)
 */
function hook_playbook_fields_update_fields_before_post($node, $fields) {
  return $fields;
}

/**
 * Specify all of the newsletters for this brand
 *
 * @param array $newsletters
 *  Array of newsletters in the form array("Newsletter_1", "Newsletter_2", "Newsletter_3")
 */
function hook_playbook_fields_get_newsletters(&$newsletters){
  
}