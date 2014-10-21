<?php 

/**
 * Implements hook_drush_command().
 */
function smg_global_drush_command() {
  $items['smg-global-360-add-fields'] = array(
    'description' => 'Add new fields to 360 content type',
  );
  $items['smg-global-remove-webform-component'] = array(
    'description' => 'Specify a webform component to be deleted from ALL WEBFORMS on the site.  Use the form_key parameter.',
    'arguments' => array(
      'component' => 'The webform component',
    ),
  );
  return $items;
}

/**
 * Add extra fields to the 360_package_spin_rotate content type
 *
 * NOTE: this function expects that the multiupload_filefield_widget is installed
 * and enabled
 */
function drush_smg_global_360_add_fields(){
  
  // Create the file multi upload field
  if(!field_info_field('field_360_multi_upload')){
    $multi_field = array(
      'field_name' => 'field_360_multi_upload',
      'type' => 'file',
      'cardinality' => '-1',
    );
    field_create_field($multi_field);
  }
  if(!field_info_instance('node', 'field_360_multi_upload', '360_package_spin_rotate')){
    $multi_instance = array(
      'field_name' => 'field_360_multi_upload',
      'entity_type' => 'node',
      'label' => 'Upload Images for 360 Gallery',
      'bundle' => '360_package_spin_rotate',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'widget' => array(
        'type' => 'file_mfw',
        'module' => 'multiupload_filefield_widget',
        'settings' => array(
          'progress_indicator' => 'throbber'
        ),
      ),
      'settings' => array(
        'file_directory' => '360_gallery',
        'file_extensions' => 'txt jpg png gif jpeg'
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden'
        ),
        'teaser' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
      'description' => 'Please upload all of the <strong>LARGE IMAGES</strong> that were provided for this 360.  This is a multi-upload field, so select ALL of the files at once',
    );
    field_create_instance($multi_instance);
  }
  
  // Has the field_360_images field collection been created, if not create it
  if(!field_info_field('field_360_images')){
    $images_field = array(
      'field_name' => 'field_360_images',
      'type' => 'field_collection',
      'cardinality' => '-1',
    );
    field_create_field($images_field);
  }
  if(!field_info_instance('node', 'field_360_images', '360_package_spin_rotate')){
    $images_instance = array(
      'field_name' => 'field_360_images',
      'entity_type' => 'node',
      'label' => '360 Images',
      'bundle' => '360_package_spin_rotate',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'widget' => array(
        'type' => 'field_collection_embed'
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
    );
    field_create_instance($images_instance);
  }
  
  if(!(field_info_field('field_360_fc_large'))){
    $fc_large_field = array(
      'field_name' => 'field_360_fc_large',
      'type' => 'image'
    );
    field_create_field($fc_large_field);
  }
  if(!field_info_instance('field_collection_item', 'field_360_fc_large', 'field_360_images')){
    $fc_large_instance = array(
      'field_name' => 'field_360_fc_large',
      'entity_type' => 'field_collection_item',
      'label' => 'Large Image',
      'settings' => array(
        'user_register_form' => 0,
        'file_extensions' => 'png gif jpg jpeg',
      ),
      'widget' => array(
        'type' => 'image_image',        
      ),
      'bundle' => 'field_360_images',
    );
    field_create_instance($fc_large_instance);
  }
  
  if(!(field_info_field('field_360_fc_small'))){
    $fc_small_field = array(
      'field_name' => 'field_360_fc_small',
      'type' => 'image'
    );
    field_create_field($fc_small_field);
  }
  if(!field_info_instance('field_collection_item', 'field_360_fc_small', 'field_360_images')){
    $fc_small_instance = array(
      'field_name' => 'field_360_fc_small',
      'entity_type' => 'field_collection_item',
      'label' => 'Small Image',
      'settings' => array(
        'user_register_form' => 0,
        'file_extensions' => 'png gif jpg jpeg',
      ),
      'widget' => array(
        'type' => 'image_image',        
      ),
      'bundle' => 'field_360_images',
    );
    field_create_instance($fc_small_instance);
  }
  
  if(!(field_info_field('field_360_fc_nav'))){
    $fc_nav_field = array(
      'field_name' => 'field_360_fc_nav',
      'type' => 'image'
    );
    field_create_field($fc_nav_field);
  }
  if(!field_info_instance('field_collection_item', 'field_360_fc_nav', 'field_360_images')){
    $fc_nav_instance = array(
      'field_name' => 'field_360_fc_nav',
      'entity_type' => 'field_collection_item',
      'label' => 'Navigator Image',
      'settings' => array(
        'user_register_form' => 0,
        'file_extensions' => 'png gif jpg jpeg',
      ),
      'widget' => array(
        'type' => 'image_image',        
      ),
      'bundle' => 'field_360_images',
    );
    field_create_instance($fc_nav_instance);
  }
  
  if(!field_info_field('field_360_fc_x')){
    $fc_x_field = array(
      'field_name' => 'field_360_fc_x',
      'type' => 'text',
    );
    field_create_field($fc_x_field);
  }
  if(!field_info_instance('field_collection_item', 'field_360_fc_x', 'field_360_images')){
    $fc_x_instance = array(
      'field_name' => 'field_360_fc_x',
      'entity_type' => 'field_collection_item',
      'label' => 'X Coordinate',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_360_images',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($fc_x_instance);
  }
  
  if(!field_info_field('field_360_fc_y')){
    $fc_y_field = array(
      'field_name' => 'field_360_fc_y',
      'type' => 'text',
    );
    field_create_field($fc_y_field);
  }
  if(!field_info_instance('field_collection_item', 'field_360_fc_y', 'field_360_images')){
    $fc_y_instance = array(
      'field_name' => 'field_360_fc_y',
      'entity_type' => 'field_collection_item',
      'label' => 'Y Coordinate',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_360_images',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($fc_y_instance);
  }  
}

/**
 * Removes one or more webform components
 */
function drush_smg_global_remove_webform_component() {
  $args = func_get_args(); // Get arguments

  // Get all webform nodes
  $webform_types = webform_variable_get('webform_node_types');
  $results = array();
  if ($webform_types) {
    $results = db_select('node', 'n')
      ->fields('n')
      ->condition('n.type', $webform_types, 'IN')
      ->execute()
      ->fetchAllAssoc('nid');
  }
  $nids = array_keys($results);
  $nodes = node_load_multiple($nids);

  // Go through each node and look for the supplied components arg(s)
  foreach ($nodes as $node) {
    $found_component = FALSE;
    foreach ($node->webform['components'] as $key => $component) {
      if (in_array($component['form_key'], $args)) {
        $found_component = TRUE;
        unset($node->webform['components'][$key]);
        drush_log(dt('Removed component ' . $component['form_key'] . ' from ' . $node->title), 'ok');
      }
    }
    if ($found_component) 
      node_save($node);
  }
  
}

