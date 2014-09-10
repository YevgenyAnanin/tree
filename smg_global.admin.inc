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
  $form['smg_global_settings']['site'] = array(
    '#type' => 'select',
    '#title' => 'Specify the brand for this site',
    '#options' => $sites,
    '#default_value' => variable_get('smg_global_site', 'pw'),
  );
  $form['smg_global_settings']['purf_standard'] = array(
    '#type' => 'select',
    '#title' => 'Select a PURF to use as the standard lead gen PURF',
    '#options' => $purf_node_options,
    '#empty_option' => t('None'),
    '#default_value' => variable_get('smg_global_standard_purf')
  );
  $form['smg_global_settings']['smg_global_social_media_watch_admin'] = array(
    '#type' => 'textfield',
    '#title' => 'Social Media Watch Admin',
    '#description' => 'Everytime an article is posted of a company that has social media watch, a notification will sent out to this email.',
    '#default_value' => variable_get('smg_global_social_media_watch_admin'),
  );

  $form['#submit'][] = 'smg_global_settings_form_submit';

  return system_settings_form($form);
}

/**
 *
 */
function smg_global_settings_form_submit($form, &$form_state){
  variable_set('smg_global_site', $form_state['values']['site']);
  variable_set('smg_global_standard_purf', $form_state['values']['purf_standard']);
}
