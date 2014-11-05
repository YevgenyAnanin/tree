<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Webform module hidden component.
  */

/**
  * Implements _webform_defaults_component().
  */
function _webform_defaults_smg_hidden() {
    return array(
        'name' => '',
        'form_key' => NULL,
        'pid' => 0,
        'weight' => 0,
        'value' => '',
        'extra' => array(
            'private' => FALSE,
            'hidden_type' => 'value',
        ),
    );
}

/**
  * Implements _webform_theme_component().
  */
function _webform_theme_smg_hidden() {
    return array(
        'webform_display_smg_hidden' => array(
            'render element' => 'element',
            'file' => 'includes/smg_hidden.inc',
            'path' => drupal_get_path('module', 'playbook_fields'),
        ),
    );
}

/**
  * Implements _webform_edit_component().
  */
function _webform_edit_smg_hidden($component) {
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

    $form['display']['hidden_type'] = array(
        '#type' => 'radios',
        '#options' => array(
            'value' => t('Secure value (allows use of all tokens)'),
            'hidden' => t('Hidden element (less secure, changeable via JavaScript)'),
        ),
        '#title' => t('Hidden type'),
        '#description' => t('Both types of hidden fields are not shown to end-users. Using a <em>Secure value</em> allows the use of <em>all tokens</em>, even for anonymous users.'),
        '#default_value' => $component['extra']['hidden_type'],
        '#parents' => array('extra', 'hidden_type'),
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
        'salesforce_field' => array(
            '#type' => 'textfield',
            '#title' => t('Salesforce field'),
            '#default_value' => isset($component['extra']['salesforce_field'])?$component['extra']['salesforce_field']:'',
            '#description' => t('Name of corresponding Salesforce field. Case sensitive.'),
            '#size' => 40,
            '#maxlength' => 255,
            '#weight' => 2,
            '#parents' => array('extra', 'salesforce_field'),
        ),
        'gotowebinar_field' => array(
          '#type' => 'textfield',
          '#title' => t('GotoWebinar field'),
          '#default_value' => isset($component['extra']['gotowebinar_field'])?$component['extra']['gotowebinar_field']:'',
          '#description' => t('Name of corresponding GotoWebinar field. Case sensitive.'),
          '#size' => 40,
          '#maxlength' => 255,
          '#weight' => 3,
          '#parents' => array('extra', 'gotowebinar_field'),
        ),
        'db_label' => array(
            '#type' => 'textfield',
            '#title' => t('Database label'),
            '#default_value' => isset($component['extra']['db_label'])?$component['extra']['db_label']:'',
            '#description' => t('Label to use for this field when concatenating multiple fields in external databases'),
            '#size' => 20,
            '#maxlength' => 255,
            '#weight' => 4,
            '#parents' => array('extra', 'db_label'),
        ),
    );

    return $form;
}

/**
  * Implements _webform_render_component().
  */
function _webform_render_smg_hidden($component, $value = NULL, $filter = TRUE) {
    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

    // Set filtering options for "value" types, which are not displayed to the
    // end user so they do not need to be sanitized.
    $strict = $component['extra']['hidden_type'] != 'value';
    $allow_anonymous = $component['extra']['hidden_type'] == 'value';
    $default_value = $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, $strict, $allow_anonymous) : $component['value'];
    if (isset($value[0])) {
        $default_value = $value[0];
    }

    $element = array(
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#weight' => $component['weight'],
        '#translatable' => array('title'),
    );

    if ($component['extra']['hidden_type'] == 'value') {
        $element['#type'] = 'value';
        $element['#value'] = $default_value;
    }
    else {
        $element['#type'] = 'hidden';
        $element['#default_value'] = $default_value;
    }

    return $element;
}

/**
  * Implements _webform_display_component().
  */
function _webform_display_smg_hidden($component, $value, $format = 'html') {
    $element = array(
        '#title' => $component['name'],
        '#markup' => isset($value[0]) ? $value[0] : NULL,
        '#weight' => $component['weight'],
        '#format' => $format,
        '#theme' => 'webform_display_smg_hidden',
        '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
        '#translatable' => array('title'),
    );

    return $element;
}

function theme_webform_display_smg_hidden($variables) {
    $element = $variables['element'];

    return $element['#format'] == 'html' ? check_plain($element['#markup']) : $element['#markup'];
}

/**
  * Implements _webform_analysis_component().
  */
function _webform_analysis_smg_hidden($component, $sids = array()) {
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
        if (strlen(trim($data['data'])) > 0) {
            $nonblanks++;
            $wordcount += str_word_count(trim($data['data']));
        }
        $submissions++;
    }

    $rows[0] = array( t('Empty'), ($submissions - $nonblanks));
    $rows[1] = array( t('Non-empty'), $nonblanks);
    $rows[2] = array( t('Average submission length in words (ex blanks)'),
                                        ($nonblanks !=0 ? number_format($wordcount/$nonblanks, 2) : '0'));
    return $rows;
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_table_smg_hidden($component, $value) {
    return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_csv_headers_smg_hidden($component, $export_options) {
    $header = array();
    $header[0] = '';
    $header[1] = '';
    $header[2] = $component['name'];
    return $header;
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_csv_data_smg_hidden($component, $export_options, $value) {
    return isset($value[0]) ? $value[0] : '';
}
