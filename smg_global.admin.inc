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
  $form['smg_global_settings']['site'] = array(
    '#type' => 'select',
    '#title' => 'Specify the brand for this site',
    '#options' => $sites,
    '#default_value' => variable_get('smg_global_site', 'pw'),
  );
  $form['#submit'][] = 'smg_global_settings_form_submit';
  
  return system_settings_form($form);
}

/**
 *
 */
function smg_global_settings_form_submit($form, &$form_state){
  variable_set('smg_global_site', $form_state['values']['site']);
}