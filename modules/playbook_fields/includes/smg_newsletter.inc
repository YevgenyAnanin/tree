<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Webform module hidden component.
  */

/**
  * Implements _webform_defaults_component().
  */
function _webform_defaults_smg_newsletter() {
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
function _webform_theme_smg_newsletter() {
    return array(
        'webform_display_smg_newsletter' => array(
            'render element' => 'element',
            'file' => 'includes/smg_newsletter.inc',
            'path' => drupal_get_path('module', 'playbook_fields'),
        ),
    );
}

/**
  * Implements _webform_edit_component().
  */
function _webform_edit_smg_newsletter($component) {
    $form = array();
    $form['value'] = array(
      '#type' => 'hidden',
      '#value' => 'Yes',
    );
    $form['display'] = array('#type'=>'hidden', '#value'=>'');
    $form['newsletter_description'] = array(
      '#type' => 'markup',
      '#markup' => '<div style="max-width:800px; font-weight:bold;">This field will attach all of the hidden newsletter fields.  The user submitting the form will be automatically signed up to each newsletter.</div>',
      '#weight' => -20,
    );

    return $form;
}

/**
  * Implements _webform_render_component().
  */
function _webform_render_smg_newsletter($component, $value = NULL, $filter = TRUE) {
    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
    
    $default_value = $component['value'];

    $element = array(
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#weight' => $component['weight'],
        '#translatable' => array('title'),
    );
    $element['#type'] = 'hidden';
    $element['#default_value'] = $default_value;


    return $element;
}

/**
  * Implements _webform_display_component().
  */
function _webform_display_smg_newsletter($component, $value, $format = 'html') {
    $element = array(
        '#title' => $component['name'],
        '#markup' => isset($value[0]) ? $value[0] : NULL,
        '#weight' => $component['weight'],
        '#format' => $format,
        '#theme' => 'webform_display_smg_newsletter',
        '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
        '#translatable' => array('title'),
    );

    return $element;
}

function theme_webform_display_smg_newsletter($variables) {
    $element = $variables['element'];

    return $element['#format'] == 'html' ? check_plain($element['#markup']) : $element['#markup'];
}

/**
  * Implements _webform_analysis_component().
  */
function _webform_analysis_smg_newsletter($component, $sids = array()) {
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
function _webform_table_smg_newsletter($component, $value) {
    return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_csv_headers_smg_newsletter($component, $export_options) {
    $header = array();
    $header[0] = '';
    $header[1] = '';
    $header[2] = $component['name'];
    return $header;
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_csv_data_smg_newsletter($component, $export_options, $value) {
    return isset($value[0]) ? $value[0] : '';
}
