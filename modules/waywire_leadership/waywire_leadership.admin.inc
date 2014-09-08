<?php
/**
 * @file
 * Admininstration for the Waywire Leadership integration
 */

/**
 * Waywire Setting form.
 */
function waywire_leadership_settings_form() {
  $form['waywire_leadership_url'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Waywire URL'),
    '#description' => 'Enter the home page Waywire URL. Example: http://summitmediagroup.magnify.net',
    '#required' => TRUE,
    '#default_value' => variable_get('waywire_leadership_url'),
  );
  $form['waywire_leadership_importer_id'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Feed Id'),
    '#description' => 'Enter the feed name that you will use to import waywire videos.',
    '#required' => TRUE,
    '#default_value' => variable_get('waywire_leadership_importer_id'),
  );

  return system_settings_form($form);
}
