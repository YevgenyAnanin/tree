<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Playbook Fields module textfield component.
  */

/**
  * Implements _webform_defaults_component().
  */
function _webform_defaults_smg_textfield() {
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
  * Implements _webform_theme_component().
  */
function _webform_theme_smg_textfield() {
    return array(
        'webform_display_smg_textfield' => array(
            'render element' => 'element',
            'file' => 'includes/smg_textfield.inc',
            'path' => drupal_get_path('module', 'playbook_fields'),
        ),
    );
}

/**
  * Implements _webform_edit_component().
  */
function _webform_edit_smg_textfield($component) {
    $form = array();
    $form['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Default value'),
        '#default_value' => $component['value'],
        '#description' => t('The default value of the field.') . theme('webform_token_help'),
        '#size' => 60,
        '#maxlength' => 1024,
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
    $form['display']['disabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Disabled'),
        '#return_value' => 1,
        '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
        '#weight' => 11,
        '#default_value' => $component['extra']['disabled'],
        '#parents' => array('extra', 'disabled'),
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
    /*$form['extra']['style'] = array(
      '#type' => 'textfield',
      '#title' => t('CSS inline style'),
      '#default_value' => isset($component['extra']['style']) ? $component['extra']['style'] : '',
      '#weight' => 20,
      '#size' => 120,
      '#maxlength' => 512,
      '#required' => FALSE,
    );*/
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
function _webform_render_smg_textfield($component, $value = NULL, $filter = TRUE) {
    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
 
    $element = array(
        '#type' => 'textfield',
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#default_value' => $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'],
        '#required' => $component['mandatory'],
        '#weight' => $component['weight'],
        '#field_prefix' => empty($component['extra']['field_prefix']) ? NULL : ($filter ? _webform_filter_xss($component['extra']['field_prefix']) : $component['extra']['field_prefix']),
        '#field_suffix' => empty($component['extra']['field_suffix']) ? NULL : ($filter ? _webform_filter_xss($component['extra']['field_suffix']) : $component['extra']['field_suffix']),
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
        '#attributes' => $component['extra']['attributes'],
        '#theme_wrappers' => array('webform_element'),
        '#translatable' => array('title', 'description', 'field_prefix', 'field_suffix'),
    );

    if ($component['extra']['disabled']) {
        if ($filter) {
            $element['#attributes']['readonly'] = 'readonly';
        }
        else {
            $element['#disabled'] = TRUE;
        }
    }

    // Enforce uniqueness.
    if ($component['extra']['unique']) {
        $element['#element_validate'][] = 'webform_validate_unique';
    }

    // Change the 'width' option to the correct 'size' option.
    if ($component['extra']['width'] > 0) {
        $element['#size'] = $component['extra']['width'];
    }
    if ($component['extra']['maxlength'] > 0) {
        $element['#maxlength'] = $component['extra']['maxlength'];
    }

    if (isset($value)) {
        $element['#default_value'] = $value[0];
    }

    return $element;
}

/**
  * Implements _webform_display_component().
  */
function _webform_display_smg_textfield($component, $value, $format = 'html') {
    return array(
        '#title' => $component['name'],
        '#weight' => $component['weight'],
        '#theme' => 'webform_display_smg_textfield',
        '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
        '#field_prefix' => $component['extra']['field_prefix'],
        '#field_suffix' => $component['extra']['field_suffix'],
        '#format' => $format,
        '#value' => isset($value[0]) ? $value[0] : '',
        '#translatable' => array('title', 'field_prefix', 'field_suffix'),
    );
}

/**
  * Format the output of data for this component.
  */
function theme_webform_display_smg_textfield($variables) {
    $element = $variables['element'];
    $prefix = $element['#format'] == 'html' ? '' : $element['#field_prefix'];
    $suffix = $element['#format'] == 'html' ? '' : $element['#field_suffix'];
    $value = $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
    return $value !== '' ? ($prefix . $value . $suffix) : ' ';
}

/**
  * Implements _webform_analysis_component().
  */
function _webform_analysis_smg_textfield($component, $sids = array()) {
    $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
        ->fields('wsd', array('data'))
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
function _webform_table_smg_textfield($component, $value) {
    return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
  * Implements _webform_csv_headers_component().
  */
function _webform_csv_headers_smg_textfield($component, $export_options) {
    $header = array();
    $header[0] = '';
    $header[1] = '';
    $header[2] = $component['name'];
    return $header;
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_csv_data_smg_textfield($component, $export_options, $value) {
    return !isset($value[0]) ? '' : $value[0];
}
