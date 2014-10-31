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
  $form['smg_global_site'] = array(
    '#type' => 'select',
    '#title' => 'Specify the brand for this site',
    '#options' => $sites,
    '#default_value' => variable_get('smg_global_site', 'pw'),
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
}