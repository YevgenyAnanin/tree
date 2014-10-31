<?php

/**
 * Implements hook_drush_command().
 */
function smg_pop_up_drush_command() {
  $items = array();
  $items['smg-pop-up-add-field-collection'] = array(
    'description' => 'Add the field_pop_up_registration field collection to a node',
    'arguments' => array(
      'content_type' => t('The content type to which you would like to add the field collection'),
    ),
  );
  $items['smg-pop-up-add-cache-table'] = array(
    'description' => 'Creates the cache_purf table in the database',
  );
  return $items;
}

function drush_smg_pop_up_add_field_collection($content_type = NULL){

  if($content_type){
    if(!array_key_exists($content_type, node_type_get_names())){
      drush_set_error('smg_pop_up', $content_type . ' does not appear to be a valid content type');
      exit;
    }
  }else{
    // @todo Create an interactive dialogue where the user is given a list of content types, perhaps
    // keyed by an integer (i.e. 1, 2, 3, ...), and by pressing the key they can choose the content type.
    drush_log('You haven\'t provided a content type', 'notice');
  }

  // Has the field_pop_up_registration field been added/defined?  If not, then we'll create the field
  if(!field_info_field('field_pop_up_registration')){
    $field = array(
      'field_name' => 'field_pop_up_registration',
      'type' => 'field_collection',
    );
    field_create_field($field);
  }

  if($content_type){
    // Make sure the field_pop_up_registration field instance hasn't been created already for the content type
    if(!field_info_instance('node', 'field_pop_up_registration', $content_type)){
      // Create an instance of the field_pop_up_registration field and add it to the passed in content type
      $pop_up_field_instance = array(
        'field_name' => 'field_pop_up_registration',
        'entity_type' => 'node',
        'label' => 'Pop Up Registration',
        'bundle' => $content_type,
        'settings' => array(
          'user_register_form' => 0,
        ),
        'widget' => array(
          'type' => 'field_collection_embed',
        ),
        'display' => array(
          'default' => array(
            'label' => 'hidden',
            'type' => 'hidden',
          ),
        ),
      );
      field_create_instance($pop_up_field_instance);
    }
  }
  if(!field_info_field('field_smg_pop_type')){
    $type_field = array(
      'field_name' => 'field_smg_pop_type',
      'settings' => array(
        'allowed_values' => array(
          'page_load' => 'On page load',
          'video_click' => 'When user clicks video',
          'link_click' => 'When user clicks link',
        ),
        'allowed_values_function' => '',
      ),
      'type' => 'list_text',
    );
    field_create_field($type_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_type', 'field_pop_up_registration')){
    $type_instance = array(
      'field_name' => 'field_smg_pop_type',
      'entity_type' => 'field_collection_item',
      'label' => 'Type of Pop-Up',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'display' => array(
        'default' => array(
        ),
      ),
      'weight' => 0,
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'options_buttons',
        'module' => 'options',
        'active' => 1,
        'weight' => 0,
      ),
      'description' => 'Specify how this pop-up will load, either automatically on page-load, or based on the user\'s action (clicking video or inline link).',
    );
    field_create_instance($type_instance);
  }

  if(!field_info_field('field_smg_pop_headline_new_usr')){
    $new_usr_field = array(
      'field_name' => 'field_smg_pop_headline_new_usr',
      'type' => 'text',
    );
    field_create_field($new_usr_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_headline_new_usr', 'field_pop_up_registration')){
    $new_usr_instance = array(
      'field_name' => 'field_smg_pop_headline_new_usr',
      'entity_type' => 'field_collection_item',
      'label' => 'Headline for New Users',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($new_usr_instance);
  }

  if(!field_info_field('field_smg_pop_headline_exist_usr')){
    $exist_usr_field = array(
      'field_name' => 'field_smg_pop_headline_exist_usr',
      'type' => 'text',
    );
    field_create_field($exist_usr_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_headline_exist_usr', 'field_pop_up_registration')){
    $exist_usr_instance = array(
      'field_name' => 'field_smg_pop_headline_exist_usr',
      'entity_type' => 'field_collection_item',
      'label' => 'Headline for Existing Users',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($exist_usr_instance);
  }

  if(!field_info_field('field_smg_pop_registration_form')){
    $reg_field = array(
      'field_name' => 'field_smg_pop_registration_form',
      'type' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'pop_up_registration' => 'pop_up_registration',
        ),
      ),
    );
    field_create_field($reg_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_registration_form', 'field_pop_up_registration')){
    $reg_field_instance = array(
      'field_name' => 'field_smg_pop_registration_form',
      'entity_type' => 'field_collection_item',
      'label' => 'Registration Form',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'options_select'
      ),
    );
    field_create_instance($reg_field_instance);
  }

  if(!field_info_field('field_smg_pop_submit_text')){
    $submit_text_field = array(
      'field_name' => 'field_smg_pop_submit_text',
      'type' => 'text',
    );
    field_create_field($submit_text_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_submit_text', 'field_pop_up_registration')){
    $submit_text_instance = array(
      'field_name' => 'field_smg_pop_submit_text',
      'entity_type' => 'field_collection_item',
      'label' => 'Text for Submit Button',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($submit_text_instance);
  }

  if(!field_info_field('field_smg_pop_leadworks_id')){
    $leadworks_id_field = array(
      'field_name' => 'field_smg_pop_leadworks_id',
      'type' => 'number_integer',
    );
    if(field_create_field($leadworks_id_field)){
      drush_log('field_smg_pop_leadworks_id has been created', 'notice');
    }
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_leadworks_id', 'field_pop_up_registration')){
    $leadworks_id_instance = array(
      'field_name' => 'field_smg_pop_leadworks_id',
      'entity_type' => 'field_collection_item',
      'label' => 'Master Leadworks ID',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($leadworks_id_instance);
  }

  if(!field_info_field('field_smg_pop_ad_id')){
    $ad_id_field = array(
      'field_name' => 'field_smg_pop_ad_id',
      'type' => 'text',
    );
    field_create_field($ad_id_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_ad_id', 'field_pop_up_registration')){
    $ad_id_instance = array(
      'field_name' => 'field_smg_pop_ad_id',
      'entity_type' => 'field_collection_item',
      'label' => 'Ad Leadworks ID',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($ad_id_instance);
  }

  if(!field_info_field('field_smg_pop_source_code')){
    $source_code_field = array(
      'field_name' => 'field_smg_pop_source_code',
      'type' => 'text',
    );
    field_create_field($source_code_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_source_code', 'field_pop_up_registration')){
    $source_code_instance = array(
      'field_name' => 'field_smg_pop_source_code',
      'entity_type' => 'field_collection_item',
      'label' => 'Source Code',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
      'description' => 'This value will be appended to the CRM Lead Source and History Slug fields in Silverpop',
    );
    field_create_instance($source_code_instance);
  }

  if(!field_info_field('field_smg_pop_data')){
    $data_field = array(
      'field_name' => 'field_smg_pop_data',
      'type' => 'text',
    );
    field_create_field($data_field);
  }
  if(!field_info_instance('field_collection_item', 'field_smg_pop_data', 'field_pop_up_registration')){
    $data_instance = array(
      'field_name' => 'field_smg_pop_data',
      'entity_type' => 'field_collection_item',
      //'label' => 'Ad Leadworks ID',
      'settings' => array(
        'user_register_form' => 0,
      ),
      'bundle' => 'field_pop_up_registration',
      'widget' => array(
        'type' => 'textfield',
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'weight' => 1,
          'type' => 'hidden',
        ),
      ),
    );
    field_create_instance($data_instance);
  }

  // If a content type was supplied, check to see if it has a Viddler field.  If so, add
  // a checkbox field to allow production people to attach a PURF to the viddler video(s)
  if($content_type){
    if(field_info_instance('node', 'field_viddler_id', $content_type)){
      $viddler_instance = field_info_instance('node', 'field_viddler_id', $content_type);
      $viddler_widget_weight = (isset($viddler_instance['widget']['weight'])) ? (int)($viddler_instance['widget']['weight']) : 49;
      $smg_viddler_widget_weight = (string)($viddler_widget_weight + 1);
      if(!field_info_field('field_smg_pop_viddler')){
        $viddler_field = array(
          'field_name' => 'field_smg_pop_viddler',
          'type' => 'list_boolean',
          'settings' => array(
            'allowed_values' => array('0','1'),
          ),
        );
        field_create_field($viddler_field);
      }
      if(!field_info_instance('node', 'field_smg_pop_viddler', $content_type)){
        $viddler_instance = array(
          'field_name' => 'field_smg_pop_viddler',
          'entity_type' => 'node',
          'label' => 'Apply PURF to Viddler Video?',
          //'label' => 'Ad Leadworks ID',
          'settings' => array(
            'user_register_form' => 0,
          ),
          'bundle' => $content_type,
          'widget' => array(
            'weight' => $smg_viddler_widget_weight,
            'type' => 'options_onoff',
            'module' => 'options',
            'active' => 1,
            'settings' => array(
              'display_label' => 1
            ),
          ),
          'display' => array(
            'default' => array(
              'label' => 'hidden',
              'type' => 'hidden',
            ),
          ),
          'default_value' => array(
            0 => array(
              'value' => 0,
            ),
          ),
        );
        field_create_instance($viddler_instance);
      }
    }
  }
}

function drush_smg_pop_up_add_cache_table(){
  db_create_table('cache_purf', drupal_get_schema_unprocessed('system', 'cache'));
}








