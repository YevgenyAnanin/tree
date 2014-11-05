<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Playbook Fields module textarea component.
  */


/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_smg_textarea() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'cols' => '',
      'rows' => '',
      'title_display' => 0,
      'resizable' => 1,
      'disabled' => 0,
      'description' => '',
      'attributes' => array(),
      'private' => FALSE,
    ),
  );
}


/**
 * Implements _webform_theme_component().
 */
function _webform_theme_smg_textarea() {
  return array(
    'webform_display_smg_textarea' => array(
      'render element' => 'element',
      'file' => 'includes/smg_textarea.inc',
      'path' => drupal_get_path('module', 'playbook_fields'),
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_smg_textarea($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => 0,
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
  $form['display']['cols'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['cols'],
    '#description' => t('Width of the textarea.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#parents' => array('extra', 'cols'),
  );
  $form['display']['rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $component['extra']['rows'],
    '#description' => t('Height of the textarea.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#parents' => array('extra', 'rows'),
  );
  $form['display']['resizable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Resizable'),
    '#description' => t('Make this field resizable by the user.'),
    '#weight' => 2,
    '#default_value' => $component['extra']['resizable'],
    '#parents' => array('extra', 'resizable'),
  );
  $form['display']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 11,
    '#default_value' => $component['extra']['disabled'],
    '#parents' => array('extra', 'disabled'),
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
      '#description' => t('Enter a dependency rule to optionally enable this field. Rules take the form \'field_key\' = \'value\'. \'=\': equals. \'!=\': does not equal. \'^=\': begins with. \'!^=\': does not begin with. \'value\' is the submit value, if given, or the display value otherwise, lowercase, with all spaces and punctuation replaced with dashes: \'United States (of America)\' -> \'united-states-of-america\'. Rules can be combined using \'and\' or \'or\'. For example: (\'field_1\' = \'foo\') and (\'field_2\' != \'bar\'). Rules can be nested: ((\'field_1\' = \'united-states\') or (\'field_2\' != \'chicago\')) and (\'field_3\' ^= \'yes\'). All field keys and values are case-sensitive.'),
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
function _webform_render_smg_textarea($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'textarea',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? _webform_filter_values($component['value'], $node) : $component['value'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#rows' => !empty($component['extra']['rows']) ? $component['extra']['rows'] : 5,
    '#cols' => !empty($component['extra']['cols']) ? $component['extra']['cols'] : 60,
    '#attributes' => $component['extra']['attributes'],
    '#resizable' => (bool) $component['extra']['resizable'], // MUST be FALSE to disable.
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  if ($component['extra']['disabled']) {
    if ($filter) {
      $element['#attributes']['readonly'] = 'readonly';
    }
    else {
      $element['#disabled'] = TRUE;
    }
  }

  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_smg_textarea($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_smg_textarea',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title'),
  );
}

/**
 * Format the output of data for this component.
 */
function theme_webform_display_smg_textarea($variables) {
  $element = $variables['element'];
  $output = $element['#format'] == 'html' ? nl2br(check_plain($element['#value'])) : $element['#value'];
  if (drupal_strlen($output) > 80) {
    $output = ($element['#format'] == 'html') ? '<div class="webform-long-answer">' . $output . '</div>' : $output;
  }
  return $output !== '' ? $output : ' ';
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_smg_textarea($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = $query->execute();
  foreach ($result as $data) {
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  $rows[2] = array(t('Average submission length in words (ex blanks)'), ($nonblanks != 0 ? number_format($wordcount/$nonblanks, 2) : '0'));
  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_smg_textarea($component, $value) {
  return empty($value[0]) ? '' : check_plain($value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_smg_textarea($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_smg_textarea($component, $export_options, $value) {
  return empty($value[0]) ? '' : $value[0];
}
