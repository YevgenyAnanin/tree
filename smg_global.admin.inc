<?php

/**
 *
 * @file
 * Contains global configuration settings for this site
 *
 */

function smg_global_settings_form(){
  $form = array();
  $sites = array(
    'aw' => 'AW',
    'pw' => 'PW',
    'hcp' => 'HCP',
  );
  // #57 Get all PURF nodes, so user can select the standard lead gen purf
  $purf_nids = db_select('node','n')
    ->fields('n', array('nid'))
    ->fields('n', array('type'))
    ->condition('n.type', 'pop_up_registration')
    ->execute()
    ->fetchCol();
  $purf_nodes = node_load_multiple($purf_nids);
  $purf_node_options = array();
  foreach ($purf_nodes as $nid => $value) {
    $purf_node_options[$nid] = $value->title;
  }
  $form['smg_global_site'] = array(
    '#type' => 'select',
    '#title' => 'Specify the brand for this site',
    '#options' => $sites,
    '#default_value' => variable_get('smg_global_site', 'pw'),
  );
  $form['smg_global_standard_purf'] = array(
    '#type' => 'select',
    '#title' => 'Select a PURF to use as the standard lead gen PURF',
    '#options' => $purf_node_options,
    '#empty_option' => t('None'),
    '#default_value' => variable_get('smg_global_standard_purf')
  );
  $form['smg_global_social_media_watch_admin'] = array(
    '#type' => 'textfield',
    '#title' => 'Social Media Watch Admin',
    '#description' => 'Everytime an article is posted of a company that has social media watch, a notification will sent out to this email.',
    '#default_value' => variable_get('smg_global_social_media_watch_admin'),
  );
  $form['smg_global_is_production'] = array(
    '#type' => 'checkbox',
    '#title' => 'Is current instance of site a Production site',
    '#description' => 'Toggling this value will determine, for example, whether or not to show minified/concatenated versions of certain js files',
    '#default_value' => variable_get('smg_global_is_production', false),
  );
  //$form['#submit'][] = 'smg_global_settings_form_submit';

  return system_settings_form($form);
}

/**
 *
 */
function smg_global_settings_form_submit($form, &$form_state){
  variable_set('smg_global_site', $form_state['values']['site']);
  variable_set('smg_global_standard_purf', $form_state['values']['purf_standard']);
}

/**
 * Configure settings for the Expo Countdown Widget (Ticket #125)
 *
 * @see http://redmine.summitmediagroup.com/issues/125
 */
function smg_expo_countdown_settings_form() {
  $form['enable_expo_countdown'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable Expo Countdown Widget',
    '#default_value' => variable_get('enable_expo_countdown'),
  );
  $form['expo_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Name of Expo',
    '#default_value' => variable_get('expo_name'),
  );
  // Default date for expo
  $expo_date = variable_get('expo_date', date('Y-m-d H'));
  $form['expo_date'] = array(
    '#type' => 'date_select',
    '#title' => 'Expo Date',
    '#default_value' => $expo_date,
    '#date_format' => 'd-F-Y g:i a',
    '#date_timezone' => 'America/Chicago',
    '#date_label_position' => 'above',
    '#date_year_range' => '-3:+3',
  );
  $timezone = variable_get('expo_time_zone', '');
  $form['expo_time_zone'] = array(
    '#type' => 'select',
    '#title' => 'Expo TimeZone',
    '#default_value' => $timezone,
    '#options' => system_time_zones(),
  );
  $form['expo_logo'] = array(
    '#type' => 'managed_file',
    '#title' => 'Expo Logo',
    '#default_value' => variable_get('expo_logo'),
    '#upload_location' => 'public://expo_countdown/',
  );
  $form['expo_enable_hours'] = array(
    '#type' => 'checkbox',
    '#title' => 'Show how many hours until the show',
    '#default_value' => variable_get('expo_enable_hours'),
  );
  $form['expo_enable_min'] = array(
    '#type' => 'checkbox',
    '#title' => 'Show how many minutes until the show',
    '#default_value' => variable_get('expo_enable_min'),
  );
  $form['expo_register_url'] = array(
    '#type' => 'textfield',
    '#title' => 'URL for Registration Page',
    '#description' => 'INCLUDE THE http://',
    '#default_value' => variable_get('expo_register_url'),
  );  
  $form['expo_exclude'] = array(
    '#type' => 'textarea',
    '#title' => 'Pages to Exclude Countdown Widget From',
    '#description' => t('Add pages, each on a new line.  Don\'t include forward slash'),
    '#default_value' => variable_get('expo_exclude'), 
  );
  $form['#submit'][] = 'expo_countdown_custom_submit';
  return system_settings_form($form);
}

/**
 * Custom submit handler for expo countdown widget settings.  Should
 * work in addition to system_settings_form default submit handler.
 */
function expo_countdown_custom_submit($form, &$form_state) {
  $file = file_load($form_state['values']['expo_logo']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  file_usage_add($file, 'expo_settings', 'expo_settings','1');
}