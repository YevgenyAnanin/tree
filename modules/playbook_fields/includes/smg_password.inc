<?php

/**
 * @file
 * Webform module fieldset component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_smg_password() {
  return array(
      'name' => '',
      'form_key' => NULL,
      'pid' => 0,
      'weight' => 0,
      'value' => '',
      'mandatory' => 0,
      'extra' => array(
          'width' => '',
          'maxlength' => '',
          'field_prefix' => '',
          'field_suffix' => '',
          'disabled' => 0,
          'unique' => 0,
          'title_display' => 0,
          'description' => '',
          'attributes' => array(),
          'private' => FALSE,
      ),
  );
}

/**
 * Implements _webform_theme_component()
 */
function _webform_theme_smg_password(){
  return array(
    'webform_display_smg_password' => array(
      'render element' => 'element',
      'file' => 'includes/smg_password.inc',
      'path' => drupal_get_path('module', 'playbook_fields'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_smg_password($component) {
  $form = array();
  $form['display']['collapsible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsible'),
    '#default_value' => $component['extra']['collapsible'],
    '#description' => t('If this fieldset is collapsible, the user may open or close the fieldset.'),
    '#weight' => 0,
    '#parents' => array('extra', 'collapsible'),
  );
  $form['display']['collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsed by Default'),
    '#default_value' => $component['extra']['collapsed'],
    '#description' => t('Collapsible fieldsets are "open" by default. Select this option to default the fieldset to "closed."'),
    '#weight' => 3,
    '#parents' => array('extra', 'collapsed'),
  );
  $form['custom_question'] = array(
    '#type' => 'checkbox',
    '#title' => t('Custom Question'),
    '#default_value' => $component['extra']['custom_question'],
    '#description' => t('Please specify if this form item is custom question on a registration form'),
    '#weight' => 0,
    '#parents' => array('extra', 'custom_question'),
  );
  $form['smg_pop_no_autosubmit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude from PURF submission'),
    '#default_value' => $component['extra']['smg_pop_no_autosubmit'],
    '#description' => '<strong>For PURFs Only:</strong> Exclude this question from PURF submissions <em>unless</em> it is a full PURF (i.e. all of the standard lead-gen questions and the custom questions).',
    '#weight' => 0,
    '#parents' => array('extra', 'smg_pop_no_autosubmit'),
  );
  $form['display']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $component['extra']['width'],
      '#description' => t('Width of the textfield.') . ' ' . t('Leaving blank will use the default size.'),
      '#size' => 5,
      '#maxlength' => 10,
      '#weight' => 0,
      '#parents' => array('extra', 'width'),
  );
  $form['display']['field_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix text placed to the left of the textfield'),
      '#default_value' => $component['extra']['field_prefix'],
      '#description' => t('Examples: $, #, -.'),
      '#size' => 20,
      '#maxlength' => 127,
      '#weight' => 1.1,
      '#parents' => array('extra', 'field_prefix'),
  );
  $form['display']['field_suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Postfix text placed to the right of the textfield'),
      '#default_value' => $component['extra']['field_suffix'],
      '#description' => t('Examples: lb, kg, %.'),
      '#size' => 20,
      '#maxlength' => 127,
      '#weight' => 1.2,
      '#parents' => array('extra', 'field_suffix'),
  );
  $form['validation']['unique'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unique'),
      '#return_value' => 1,
      '#description' => t('Check that all entered values for this field are unique. The same value is not allowed to be used twice.'),
      '#weight' => 1,
      '#default_value' => $component['extra']['unique'],
      '#parents' => array('extra', 'unique'),
  );
  $form['validation']['maxlength'] = array(
      '#type' => 'textfield',
      '#title' => t('Maxlength'),
      '#default_value' => $component['extra']['maxlength'],
      '#description' => t('Maximum length of the textfield value.'),
      '#size' => 5,
      '#maxlength' => 10,
      '#weight' => 2,
      '#parents' => array('extra', 'maxlength'),
  );
  $form['linking'] = array(
      '#type' => 'fieldset',
      '#title' => 'Data Linking',
      '#collapsible' => TRUE,
      '#weight' => 20,
      'accela_id' => array(
          '#type' => 'textfield',
          '#title' => t('AccelaWorks ID'),
          '#default_value' => isset($component['extra']['accela_id'])?$component['extra']['accela_id']:'',
          '#description' => t('ID of corresponding AccelaWorks field.'),
          '#size' => 20,
          '#maxlength' => 255,
          '#weight' => 0,
          '#parents' => array('extra', 'accela_id'),
      ),
      'leadworks_id' => array(
          '#type' => 'textfield',
          '#title' => t('Leadworks ID'),
          '#default_value' => isset($component['extra']['leadworks_id'])?$component['extra']['leadworks_id']:'',
          '#description' => t('ID of corresponding Leadworks field.'),
          '#size' => 20,
          '#maxlength' => 255,
          '#weight' => 0,
          '#parents' => array('extra', 'leadworks_id'),
      ),
      'silverpop_field' => array(
          '#type' => 'textfield',
          '#title' => t('Silverpop field'),
          '#default_value' => isset($component['extra']['silverpop_field'])?$component['extra']['silverpop_field']:'',
          '#description' => t('Name of corresponding Silverpop field. Case sensitive.'),
          '#size' => 40,
          '#maxlength' => 255,
          '#weight' => 1,
          '#parents' => array('extra', 'silverpop_field'),
      ),
      'prepopulate' => array(
          '#type' => 'checkbox',
          '#title' => t('Prepopulate'),
          '#default_value' => isset($component['extra']['prepopulate'])?$component['extra']['prepopulate']:TRUE,
          '#description' => t('Check if you want this field to prepopulate with user data from this Silverpop field.'),
          '#weight' => 2,
          '#parents' => array('extra', 'prepopulate'),
      ),
      'prepend' => array(
          '#type' => 'checkbox',
          '#title' => t('Prepend Question'),
          '#default_value' => isset($component['extra']['prepend'])?$component['extra']['prepend']:FALSE,
          '#description' => t('Check if you want the question to be prepended to the answer.'),
          '#weight' => 3,
          '#parents' => array('extra', 'prepend'),
      ),
      'salesforce_field' => array(
          '#type' => 'textfield',
          '#title' => t('Salesforce field'),
          '#default_value' => isset($component['extra']['salesforce_field'])?$component['extra']['salesforce_field']:'',
          '#description' => t('Name of corresponding Salesforce field. Case sensitive.'),
          '#size' => 40,
          '#maxlength' => 255,
          '#weight' => 3,
          '#parents' => array('extra', 'salesforce_field'),
      ),
      'gotowebinar_field' => array(
        '#type' => 'textfield',
        '#title' => t('GotoWebinar field'),
        '#default_value' => isset($component['extra']['gotowebinar_field'])?$component['extra']['gotowebinar_field']:'',
        '#description' => t('Name of corresponding GotoWebinar field. Case sensitive.'),
        '#size' => 40,
        '#maxlength' => 255,
        '#weight' => 4,
        '#parents' => array('extra', 'gotowebinar_field'),
      ),
      'db_label' => array(
          '#type' => 'textfield',
          '#title' => t('Database label'),
          '#default_value' => isset($component['extra']['db_label'])?$component['extra']['db_label']:'',
          '#description' => t('Label to use for this field when concatenating multiple fields in external databases'),
          '#size' => 20,
          '#maxlength' => 255,
          '#weight' => 5,
          '#parents' => array('extra', 'db_label'),
      ),
  );
  $form['extra']['dependency'] = array(
      '#type' => 'textfield',
      '#title' => t('Dependency'),
      '#default_value' => isset($component['extra']['dependency']) ? $component['extra']['dependency'] : '',
      '#description' => t(DEPENDENCY_TEXT),
      '#weight' => 20,
      '#size' => 120,
      '#maxlength' => DEPENDENCY_LIMIT,
      '#required' => FALSE,
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_smg_password($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'fieldset',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : NULL,
    '#default_value' => $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : ($filter ? _webform_filter_xss($component['extra']['field_prefix']) : $component['extra']['field_prefix']),
    '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : ($filter ? _webform_filter_xss($component['extra']['field_suffix']) : $component['extra']['field_suffix']),
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#collapsible' => $component['extra']['collapsible'],
    '#collapsed' => $component['extra']['collapsed'],
    '#attributes' => array('class' => array('webform-component-fieldset', 'password_fieldset', 'webform-component', 'webform-component-smg_password'), 'id' => 'webform-component-' . $component['form_key']),
    '#pre_render' => array('webform_fieldset_prerender', 'webform_element_title_display'),
    '#translatable' => array('title', 'description'),
  );
  $element['smg_password_hidden_textfield'] = array(
    '#type' => 'textfield',
    '#attributes' => array('class'=>array('smg_password_hidden_textfield'), 'style'=>'display:none;','autocomplete'=>'off'),
  );
  $element['password_primary'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => true,
    '#webform_component' => $component,
    '#attributes' => array('class' => array('password'), 'autocomplete'=>'off'),
  );
  $element['verify_password'] = array(
    '#type' => 'password',
    '#title' => t('Verify Password'),
    '#required' => true,
    '#webform_component' => $component,
    '#attributes' => array('class' => array('password-validate'), 'autocomplete' =>'off'),
  );
  return $element;
}

/**
 * Pre-render function to set a fieldset ID.
 */
function webform_smg_password_prerender($element) {
  $element['#attributes']['id'] = 'webform-component-' . str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_smg_password($component, $value, $format = 'html') {
  return array(
    //'#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_smg_password',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#field_prefix' => $component['extra']['field_prefix'],
    '#field_suffix' => $component['extra']['field_suffix'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
  );

  return $element;
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_smg_password($variables) {
    $element = $variables['element'];
    $prefix = $element['#format'] == 'html' ? '' : $element['#field_prefix'];
    $suffix = $element['#format'] == 'html' ? '' : $element['#field_suffix'];
    $value = $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
    return $value !== '' ? ($prefix . $value . $suffix) : ' ';
}