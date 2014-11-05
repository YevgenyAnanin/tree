<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Webform module grid component.
  */

// Grid depends on functions provided by select.
webform_component_include('smg_select');

/**
  * Implements _webform_defaults_component().
  */
function _webform_defaults_smg_grid() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'grid_type' => 'radio',
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'title_display' => 0,
      'custom_option_keys' => 0,
      'custom_question_keys' => 0,
      'description' => '',
      'private' => FALSE,
    ),
  );
}


/**
  * Implements _webform_theme_component().
  */
function _webform_theme_smg_grid() {
  return array(
    'webform_smg_grid' => array(
      'render element' => 'element',
      'file' => 'includes/smg_grid.inc',
      'path' => drupal_get_path('module', 'playbook_fields'),
    ),
    'webform_display_smg_grid' => array(
      'render element' => 'element',
      'file' => 'includes/smg_grid.inc',
      'path' => drupal_get_path('module', 'playbook_fields'),
    ),
  );
}

/**
  * Implements _webform_edit_component().
  */
function _webform_edit_smg_grid($component) {
  $form = array();
  
  $form['extra']['grid_type'] = array(
    '#type' => 'select',
    '#title' => t('Grid Type'),
    '#default_value' => $component['extra']['grid_type'] ? $component['extra']['grid_type'] : 'radio',
    '#description' => t('Sets if grid will use checkboxes or radio buttons'),
    '#options' => array(
      'radio' => 'radio',
      'checkbox' => 'checkbox',
    ),
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
  $form['extra']['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $component['extra']['options'],
    '#description' => t('List of options across the top of the grid. One option per line. These will be submitted as answers to the questions defined below. To define slugs (short submitted answers), use the form "slug|display answer".') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -3,
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#element_validate' => array('_webform_edit_validate_smg_select'),
  );
  $form['extra']['questions'] = array(
    '#type' => 'textarea',
    '#title' => t('Questions'),
    '#default_value' => $component['extra']['questions'],
    '#description' => t('List of questions down the side of the grid. One question per line. Each question creates a radio button set on the form. Specify corresponding AccelaWorks IDs using this format: (note the leading \'|\') "|question name|accela_id". When defining fields which are dependent on this field, individual radio buttons must be tested. Use this dependency rule format: "\'[key]-[name]-[num]\' = \'checked\'" where [key] is this Field Key, [name] is the question name from this textarea, and [num] is the ordinal of the radio button.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -2,
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#element_validate' => array('_webform_edit_validate_smg_select'),
  );

  $form['display']['optrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Options'),
    '#default_value' => $component['extra']['optrand'],
    '#description' => t('Randomizes the order of options on the top when they are displayed in the form.'),
    '#parents' => array('extra', 'optrand')
  );
  $form['display']['qrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Questions'),
    '#default_value' => $component['extra']['qrand'],
    '#description' => t('Randomize the order of the questions on the side when they are displayed in the form.'),
    '#parents' => array('extra', 'qrand')
  );
  $form['linking'] = array(
    '#type' => 'fieldset',
    '#title' => 'Data Linking',
    '#collapsible' => TRUE,
    '#weight' => 20,
    'silverpop_field' => array(
      '#type' => 'textfield',
      '#title' => t('Silverpop field'),
      '#default_value' => isset($component['extra']['silverpop_field'])?$component['extra']['silverpop_field']:'',
      '#description' => t('Name of corresponding Silverpop field. Case sensitive.'),
      '#size' => 40,
      '#maxlength' => 255,
      '#weight' => 0,
      '#parents' => array('extra', 'silverpop_field'),
    ),
    'prepopulate' => array(
        '#type' => 'checkbox',
        '#disabled' => TRUE,
        '#title' => t('Prepopulate'),
        '#default_value' => isset($component['extra']['prepopulate'])?$component['extra']['prepopulate']:FALSE,
        '#description' => t('Check if you want this field to prepopulate with user data from this Silverpop field.'),
        '#weight' => 1,
        '#parents' => array('extra', 'prepopulate'),
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
function _webform_render_smg_grid($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'webform_grid',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#grid_questions' => _webform_smg_select_options_from_text($component['extra']['questions'], TRUE),
    '#grid_options' => _webform_smg_select_options_from_text($component['extra']['options'], TRUE),
    '#optrand' => $component['extra']['optrand'],
    '#qrand' => $component['extra']['qrand'],
    '#grid_type' => $component['extra']['grid_type'],
    '#theme' => 'webform_smg_grid',
    '#theme_wrappers' => array('webform_element'),
    '#process' => array('webform_expand_smg_grid'),
    '#translatable' => array('title', 'description', 'grid_options', 'grid_questions'),
  );

  if ($value) {
    $element['#default_value'] = $value;
  }

  return $element;
}

/**
 * A Form API #process function for Webform grid fields.
 */
function webform_expand_smg_grid($element) {
  $options = $element['#grid_options'];
  $questions = $element['#grid_questions'];

  if (!empty($element['#optrand'])) {
    _webform_shuffle_options($options);
  }

  if (!empty($element['#qrand'])) {
    _webform_shuffle_options($questions);
  }
  
  if($element['#grid_type'] == 'checkbox') {
    $type = 'checkboxes';
    $process = array('form_process_checkboxes', 'webform_expand_select_ids');
  }
  else {
    $type = 'radios';
    $process = array('form_process_radios', 'webform_expand_select_ids');
  }
  
  foreach ($questions as $key => $question) {
    if ($question != '') {
      $element[$key] = array(
        '#title' => $question,
        '#required' => $element['#required'],
        '#options' => $options,
        '#type' => $type,
        '#process' => $process,

        // Webform handles validation manually.
        '#validated' => TRUE,
        '#webform_validated' => FALSE,
        '#translatable' => array('title'),
      );
    }
  }

  $value = isset($element['#default_value']) ? $element['#default_value'] : array();
  foreach (element_children($element) as $key) {
    if (isset($value[$key])) {
      $element[$key]['#default_value'] = ($value[$key] !== '') ? $value[$key] : FALSE;
    }
    else {
      if($type == 'checkboxes') {
        $element[$key]['#default_value'] = array();
      }
      else {
        $element[$key]['#default_value'] = FALSE; 
      }
    }
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_smg_grid($component, $value, $format = 'html') {
  $questions = _webform_smg_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_smg_select_options_from_text($component['extra']['options'], TRUE);

  $element = array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#grid_questions' => $questions,
    '#grid_options' => $options,
    '#theme' => 'webform_display_smg_grid',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#sorted' => TRUE,
    '#translatable' => array('#title', '#grid_questions', '#grid_options'),
  );

  foreach ($questions as $key => $question) {
    if ($question !== '') {
      $element[$key] = array(
        '#title' => $question,
        '#value' => isset($value[$key]) ? $value[$key] : NULL,
        '#translatable' => array('#title', '#value'),
      );
    }
  }

  return $element;
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_smg_grid($variables) {
  $element = $variables['element'];

  $component = $element['#webform_component'];
  $format = $element['#format'];

  if ($format == 'html') {
    $rows = array();
    $header = array(array('data' => '', 'class' => array('webform-grid-question')));
    foreach ($element['#grid_options'] as $option) {
      $header[] = array('data' => _webform_filter_xss($option), 'class' => array('checkbox', 'webform-grid-option'));
    }
    foreach ($element['#grid_questions'] as $question_key => $question) {
      $row = array();
      $row[] = array('data' => _webform_filter_xss($question), 'class' => array('webform-grid-question'));
      foreach ($element['#grid_options'] as $option_value => $option_label) {
        if (strcmp($element[$question_key]['#value'], $option_value) == 0) {
          $row[] = array('data' => '<strong>X</strong>', 'class' => array('checkbox', 'webform-grid-option'));
        }
        else {
          $row[] = array('data' => '&nbsp;', 'class' => array('checkbox', 'webform-grid-option'));
        }
      }
      $rows[] = $row;
    }

    $option_count = count($header) - 1;
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('webform-grid', 'webform-grid-' . $option_count))));
  }
  else {
    $items = array();
    foreach (element_children($element) as $key) {
      $items[] = ' - ' . $element[$key]['#title'] . ': ' . (isset($element['#grid_options'][$element[$key]['#value']]) ? $element['#grid_options'][$element[$key]['#value']] : '');
    }
    $output = implode("\n", $items);
  }

  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_smg_grid($component, $sids = array()) {
  // Generate the list of options and questions.
  $options = _webform_smg_select_options_from_text($component['extra']['options'], TRUE);
  $questions = _webform_smg_select_options_from_text($component['extra']['questions'], TRUE);

  // Generate a lookup table of results.
  $query = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('no', 'data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->groupBy('no')
    ->groupBy('data');
  $query->addExpression('COUNT(sid)', 'datacount');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $result = $query->execute();
  $counts = array();
  foreach ($result as $data) {
    $counts[$data->no][$data->data] = $data->datacount;
  }

  // Create an entire table to be put into the returned row.
  $rows = array();
  $header = array('');

  // Add options as a header row.
  foreach ($options as $option) {
    $header[] = _webform_filter_xss($option);
  }

  // Add questions as each row.
  foreach ($questions as $qkey => $question) {
    $row = array(_webform_filter_xss($question));
    foreach ($options as $okey => $option) {
      $row[] = !empty($counts[$qkey][$okey]) ? $counts[$qkey][$okey] : 0;
    }
    $rows[] = $row;
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('webform-grid'))));


  return array(array(array('data' => $output, 'colspan' => 2)));
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_smg_grid($component, $value) {
  $questions = _webform_smg_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_smg_select_options_from_text($component['extra']['options'], TRUE);

  $output = '';
  // Set the value as a single string.
  foreach ($questions as $key => $label) {
    if (isset($value[$key]) && isset($options[$value[$key]])) {
      $output .= _webform_filter_xss($label) . ': ' . _webform_filter_xss($options[$value[$key]]) . '<br />';
    }
  }

  return $output;
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_smg_grid($component, $export_options) {
  $header = array();
  $header[0] = array('');
  $header[1] = array($component['name']);
  $items = _webform_smg_select_options_from_text($component['extra']['questions'], TRUE);
  $count = 0;
  foreach ($items as $key => $item) {
    // Empty column per sub-field in main header.
    if ($count != 0) {
      $header[0][] = '';
      $header[1][] = '';
    }
    // The value for this option.
    $header[2][] = $item;
    $count++;
  }

  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_smg_grid($component, $export_options, $value) {
  $questions = _webform_smg_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_smg_select_options_from_text($component['extra']['options'], TRUE);
  $return = array();
  foreach ($questions as $key => $question) {
    if (isset($value[$key]) && isset($options[$value[$key]])) {
      $return[] = $export_options['select_keys'] ? $value[$key] : $options[$value[$key]];
    }
    else {
      $return[] = '';
    }
  }
  return $return;
}

function theme_webform_smg_grid($variables) {
  $element = $variables['element'];

  $rows = array();
  $header = array(array('data' => '', 'class' => array('webform-grid-question')));
  // Set the header for the table.
  foreach ($element['#grid_options'] as $option) {
    $header[] = array('data' => _webform_filter_xss($option), 'class' => array('checkbox', 'webform-grid-option'));
  }

  foreach (element_children($element) as $key) {
    $question_element = $element[$key];

    // Create a row with the question title.
    $row = array(array('data' => _webform_filter_xss($question_element['#title']), 'class' => array('webform-grid-question')));

    // Render each radio button in the row.
    $radios = form_process_radios($question_element);
    foreach (element_children($radios) as $key) {
      $radios[$key]['#title_display'] = 'invisible';
      $row[] = array('data' => drupal_render($radios[$key]), 'class' => array('checkbox', 'webform-grid-option'));
    }
    $rows[] = $row;
  }

  $option_count = count($header) - 1;
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('webform-grid', 'webform-grid-' . $option_count))));
}
