<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Webform module markup component.
  */

/**
  * Implements _webform_defaults_component().
  */
function _webform_defaults_smg_markup() {
    return array(
        'name' => '',
        'form_key' => NULL,
        'pid' => 0,
        'weight' => 0,
        'value' => '',
        'extra' => array(
            'format' => NULL,
            'private' => FALSE,
        ),
    );
}

/**
  * Implements _webform_edit_component().
  */
function _webform_edit_smg_markup($component) {
    $form = array();
    $form['value'] = array(
        '#type' => 'text_format',
        '#title' => t('Value'),
        '#default_value' => $component['value'],
        '#description' => t('Markup allows you to enter custom HTML or PHP logic into your form.') . theme('webform_token_help'),
        '#weight' => -1,
        '#format' => $component['extra']['format'],
        '#element_validate' => array('_webform_edit_smg_markup_validate'),
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
    $form['dependency'] = array(
        '#type' => 'textfield',
        '#title' => t('Dependency'),
        '#default_value' => isset($component['extra']['dependency']) ? $component['extra']['dependency'] : '',
        '#description' => t(DEPENDENCY_TEXT),
        '#weight' => 20,
        '#size' => 120,
        '#maxlength' => DEPENDENCY_LIMIT,
        '#required' => FALSE,
        '#parents' => array('extra', 'dependency'),
    );

    return $form;
}

/**
  * Element validate handler; Set the text format value.
  */
function _webform_edit_smg_markup_validate($form, &$form_state) {
    if (is_array($form_state['values']['value'])) {
        $form_state['values']['extra']['format'] = $form_state['values']['value']['format'];
        $form_state['values']['value'] = $form_state['values']['value']['value'];
    }
}

/**
  * Implements _webform_render_component().
  */
function _webform_render_smg_markup($component, $value = NULL, $filter = TRUE) {
    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

    $element = array(
        '#type' => 'markup',
        '#title' => $filter ? NULL : $component['name'],
        '#weight' => $component['weight'],
        '#markup' => $filter ? _webform_filter_values(check_markup($component['value'], $component['extra']['format'], '', TRUE), $node, NULL, NULL, FALSE) : $component['value'],
        '#format' => $component['extra']['format'],
        '#theme_wrappers' => array('webform_element'),
        '#translatable' => array('title', 'markup'),
    );

    // TODO: Remove when #markup becomes available in D7.
    $element['#value'] = $element['#markup'];

    return $element;
}

/**
  * Implements _webform_display_component().
  */
function _webform_display_smg_markup($component, $value, $format = 'html') {
    return array();
}
