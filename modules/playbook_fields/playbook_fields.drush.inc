<?php

/**
 * @file 
 * Drush commands for the Playbook Fields module
 */

/**
 * Implements hook_drush_command()
 */
function playbook_fields_drush_command() {
  $items = array();
  $items['playbook-fields-newsletter-update'] = array(
    'description' => 'Go through webform nodes, remove all individually attached hidden newsletter fields, and replace them with the smg_newsletter field',
  );
  return $items;
}

/**
 * This function goes through every webform enabled node.  It goes through
 * the node's webform components and searches for hidden newsletter fields
 * (i.e. "NL PW Shelf Impact").  Once it finds a newsletter field, it will
 * delete it from the node.  If it finds AT LEAST ONE newsletter field, 
 * then it will automatically add an smg_newsletter component to the node.
 */
function drush_playbook_fields_newsletter_update(){
  // First we call hook_playbook_fields_get_newsletters to get the list of
  // newsletters for the current brand/site
  $newsletters = array();
  foreach(module_implements('playbook_fields_get_newsletters') as $module){
    $function = $module . '_playbook_fields_get_newsletters';
    $function($newsletters);
  }
  
  // Get the list of content types that are webform enabled
  $webform_types = webform_variable_get('webform_node_types');
  // Delete 'stage_one_form' from the array
  $key = array_search('stage_one_form', $webform_types);
  if($key)
    unset($webform_types[$key]);
  
  // Use EntityFieldQuery class to get all webform nodes
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type','node');
  $query->entityCondition('bundle', $webform_types, 'IN');
  $result = $query->execute();
  if(isset($result['node'])){
    $nids = array_keys($result['node']);
    foreach($nids as $nid){
      // Set a boolean variable to track whether this node has AT LEAST ONE
      // newsletter field.  Also set a boolean to track if this node already
      // has an smg_newsletter field, so we don't add another one
      $has_newsletter = false;
      $has_smg_newsletter = false;
      
      $node = node_load($nid);
      // Go through the node's webform components and look for newsletters
      foreach($node->webform['components'] as $key => $value){
        if(in_array($value['extra']['silverpop_field'], $newsletters) || strpos($value['extra']['silverpop_field'], 'NL ') !== false){
          $has_newsletter = true;
          unset($node->webform['components'][$key]);
        }        
        if($value['type'] == 'smg_newsletter')
          $has_smg_newsletter = true;
      }
      
      // If node has newsletter(s), add the smg_newsletter component and save
      // the node
      if($has_newsletter){
        if(!$has_smg_newsletter){
          $newsletter_component = array(
            'nid' => $nid,
            'name' => 'Newsletters',
            'form_key' => 'newsletters',
            'type' => 'smg_newsletter',
            'mandatory' => '0',
            'pid' => '0',
            'weight' => 50,
            'value' => 'Yes',
            'extra' => array(
              'conditional_operator' => '=',
              'private' => 0,
              'hidden_type' => 'value',
              'conditional_component' => '',
              'conditional_values' => '',
            ),
          );
          $node->webform['components'][] = $newsletter_component;
        }
        if(property_exists($node, 'field_viddler_id') && isset($node->field_viddler_id['und'])){
          drush_log('Failed to save node because its viddler field is populated.  Node: ' . $nid, 'warning');
        }
        else{
          try {
            node_save($node);
          }
          catch (Exception $e){
            drush_log('failed to save node: ' . $nid, 'error');
          }
        }
      }
    }
  }
}