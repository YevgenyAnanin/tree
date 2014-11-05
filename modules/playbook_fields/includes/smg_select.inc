<?php
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
/**
  * @file
  * Playbook Fields module multiple select component.
  */

/**
  * Implements _webform_defaults_component().
  */
function _webform_defaults_smg_select() {
    return array(
        'name' => '',
        'form_key' => NULL,
        'mandatory' => 0,
        'pid' => 0,
        'weight' => 0,
        'value' => '',
        'extra' => array(
            'items' => '',
            'multiple' => NULL,
            'aslist' => NULL,
            'optrand' => 0,
            'other_option' => NULL,
            'other_text' => t('Other...'),
            'title_display' => 0,
            'description' => '',
            'custom_keys' => FALSE,
            'options_source' => '',
            'private' => FALSE,
        ),
    );
}

/**
  * Implements _webform_theme_component().
  */
function _webform_theme_smg_select() {
    return array(
        'webform_display_smg_select' => array(
            'render element' => 'element',
            'file' => 'includes/smg_select.inc',
            'path' => drupal_get_path('module', 'playbook_fields'),
        ),
    );
}

/**
  * Implements _webform_edit_component().
  */
function _webform_edit_smg_select($component) {
    $form = array(
        '#attached' => array(
            'js' => array(
                drupal_get_path('module', 'webform') . '/js/select-admin.js' => array('preprocess' => FALSE),
                array('data' => array('webform' => array('selectOptionsUrl' => url('webform/ajax/options/' . $component['nid']))), 'type' => 'setting'),
            ),
        ),
    );

    $other = array();
    if ($info = _webform_smg_select_options_info()) {
        $options = array('' => t('None'));
        foreach ($info as $name => $source) {
            $options[$name] = $source['title'];
        }

        $other['options_source'] = array(
            '#title' => t('Load a pre-built option list'),
            '#type' => 'select',
            '#options' => $options,
            '#default_value' => $component['extra']['options_source'],
            '#weight' => 1,
            '#description' => t('Use a pre-built list of options rather than entering options manually. Options will not be editable if using pre-built list.'),
            '#parents' => array('extra', 'options_source'),
            '#weight' => 5,
        );
    }

    if (module_exists('select_or_other')) {
        $other['other_option'] = array(
            '#type' => 'checkbox',
            '#title' => t('Allow "Other..." option'),
            '#default_value' => $component['extra']['other_option'],
            '#description' => t('Check this option if you want to allow users to enter an option not on the list.'),
            '#parents' => array('extra', 'other_option'),
            '#weight' => 2,
        );
        $other['other_text'] = array(
            '#type' => 'textfield',
            '#title' => t('Text for "Other..." option'),
            '#default_value' => $component['extra']['other_text'],
            '#description' => t('If allowing other options, enter text to be used for other-enabling option.'),
            '#parents' => array('extra', 'other_text'),
            '#weight' => 3,
        );
    }

    if (module_exists('options_element')) {
        $options = _webform_smg_select_options($component, FALSE, FALSE);

        $form['items'] = array(
            '#type' => 'fieldset',
            '#title' => t('Options'),
            '#collapsible' => TRUE,
            '#attributes' => array('class' => array('webform-options-element')),
            '#element_validate' => array('_webform_edit_validate_smg_options'),
            '#weight' => 2,
        );

        $form['items']['options'] = array(
            '#type' => 'options',
            '#limit' => 500,
            '#optgroups' => $component['extra']['aslist'],
            '#multiple' => $component['extra']['multiple'],
            '#multiple_toggle' => t('Multiple'),
            '#default_value' => $component['value'],
            '#options' => $options,
            '#options_readonly' => !empty($component['extra']['options_source']),
            '#key_type' => 'mixed',
            '#key_type_toggle' => t('Customize keys (Advanced)'),
            '#key_type_toggled' => $component['extra']['custom_keys'],
            '#default_value_pattern' => '^%.+\[.+\]$',
            '#weight' => 1,
        );

        $form['items']['options']['option_settings'] = $other;
    }
    else {
        $form['extra']['items'] = array(
            '#type' => 'textarea',
            '#title' => t('Options'),
            '#default_value' => $component['extra']['items'],
            //'#description' => t('<strong>Key-value pairs MUST be specified as "<span style=\'font-family:Courier\'>safe_key|option</span>"</strong>. For multiple-select options, an AccelaWorks ID must be specified for each option with the format "<span style=\'font-family:Courier\'>safe_key|option|accela_id</span>". Use of only alphanumeric characters and underscores is recommended in keys. One option per line. Option groups may be specified with &lt;Group Name&gt;. &lt;&gt; can be used to insert items at the root of the menu after specifying a group.') . theme('webform_token_help'),
            '#description' => t('Each line may be specified in one of three ways: "submit value|display value|accelaworks id", "submit value|display value", or simply "display value". If this is a multi-select option, an AccelaWorks ID must be specified for each option. One option per line. Option groups may be specified with &lt;Group Name&gt;. &lt;&gt; can be used to insert items at the root of the menu after specifying a group.') . theme('webform_token_help'),
            '#cols' => 60,
            '#rows' => 5,
            '#weight' => 0,
            '#required' => TRUE,
            '#wysiwyg' => FALSE,
            '#element_validate' => array('_webform_edit_validate_smg_select'),
        );

        if (!empty($component['extra']['options_source'])) {
            $form['extra']['items']['#attributes'] = array('readonly' => 'readonly');
        }

        $form['extra'] = array_merge($form['extra'], $other);
        $form['value'] = array(
            '#type' => 'textfield',
            '#title' => t('Default value'),
            '#default_value' => $component['value'],
            '#description' => t('The default value of the field identified by its key. For multiple selects use commas to separate multiple defaults.') . theme('webform_token_help'),
            '#size' => 60,
            '#maxlength' => 1024,
            '#weight' => 0,
        );
        $form['extra']['multiple'] = array(
            '#type' => 'checkbox',
            '#title' => t('Multiple'),
            '#default_value' => $component['extra']['multiple'],
            '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
            '#weight' => 0,
        );
    }
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

    $form['display']['aslist'] = array(
        '#type' => 'checkbox',
        '#title' => t('Listbox'),
        '#default_value' => isset($component['extra']['aslist']) ? $component['extra']['aslist'] : 1,
        '#description' => t('Check this option if you want the select component to be displayed as a select list box instead of radio buttons or checkboxes. Option groups (nested options) are only supported with listbox components.'),
        '#parents' => array('extra', 'aslist'),
    );
    $form['display']['optrand'] = array(
        '#type' => 'checkbox',
        '#title' => t('Randomize options'),
        '#default_value' => $component['extra']['optrand'],
        '#description' => t('Randomizes the order of the options when they are displayed in the form.'),
        '#parents' => array('extra', 'optrand'),
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
            '#description' => t('ID of corresponding AccelaWorks field. LEAVE BLANK for Accela \'checkbox\' type (multiselect).'),
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
        '#description' => t(DEPENDENCY_TEXT),
        '#weight' => 20,
        '#size' => 120,
        '#maxlength' => DEPENDENCY_LIMIT,
        '#required' => FALSE,
    );

    return $form;
}

/**
  * Element validation callback. Ensure keys are not duplicated.
  */
function _webform_edit_validate_smg_select($element, &$form_state) {
    // Check for duplicate key values to prevent unexpected data loss. Require
    // all options to include a safe_key.
    if (!empty($element['#value'])) {
        $lines = explode("\n", trim($element['#value']));
        $existing_keys = array();
        $duplicate_keys = array();
        $missing_keys = array();
        $long_keys = array();
        $group = '';
        foreach ($lines as $line) {
            $matches = array();
            $line = trim($line);
            if (preg_match('/^\<([^>]*)\>$/', $line, $matches)) {
                $group = $matches[1];
                $key = NULL; // No need to store group names.
            }
            elseif (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
                $key = $matches[1];
                if (strlen($key) > 128) {
                    $long_keys[] = $key;
                }
            }
            else {
                $missing_keys[] = $line;
            }

            if (isset($key)) {
                if (isset($existing_keys[$group][$key])) {
                    $duplicate_keys[$key] = $key;
                }
                else {
                    $existing_keys[$group][$key] = $key;
                }
            }
        }

        if (!empty($missing_keys)) {
            // We set default keys, so these aren't needed
            //form_error($element, t('Every option must have a key specified. Specify each option as "safe_key|Some readable option".'));
        }

        if (!empty($long_keys)) {
            //form_error($element, t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys));
        }

        if (!empty($duplicate_keys)) {
            //form_error($element, t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', array('items' => $duplicate_keys)));
        }

        // Set the listbox option if needed.
        if (empty($missing_keys) && empty($long_keys) && empty($duplicate_keys)) {
            $options = _webform_smg_select_options_from_text($element['#value']);
            _webform_edit_validate_smg_set_aslist($options, $form_state);
        }
    }

    return TRUE;
}

/**
  * Set the appropriate webform values when using the options element module.
  */
function _webform_edit_validate_smg_options($element, &$form_state) {
    $key = end($element['#parents']);
    $element_options = $form_state['values'][$key]['options'];
    unset($form_state['values'][$key]);

    $form_state['values']['extra'][$key] = form_options_to_text($element_options['options'], 'custom');

    // Options saved for select components.
    if ($key == 'items') {
        $form_state['values']['extra']['multiple'] = $element_options['multiple'];
        $form_state['values']['extra']['custom_keys'] = $element_options['custom_keys'];
        $form_state['values']['value'] = is_array($element_options['default_value']) ? implode(', ', $element_options['default_value']) : $element_options['default_value'];

        // Set the listbox option if needed.
        _webform_edit_validate_smg_set_aslist($element_options['options'], $form_state);
    }
    // Options saved for grid components.
    else {
        $form_state['values']['extra']['custom_' . rtrim($key, 's') . '_keys'] = $element_options['custom_keys'];
    }
}

/**
  * Ensure "aslist" is used for option groups. Called from options validations.
  */
function _webform_edit_validate_smg_set_aslist($options, &$form_state) {
    if (empty($form_state['values']['extra']['aslist']) && !empty($options)) {
        foreach ($options as $option) {
            if (is_array($option)) {
                $form_state['values']['extra']['aslist'] = 1;
                drupal_set_message(t('The component %name has automatically been set to display as a listbox in order to support option groups.', array('%name' => $form_state['values']['name'])), 'warning');
                break;
            }
        }
    }
}

/**
  * Implements _webform_render_component().
  */
function _webform_render_smg_select($component, $value = NULL, $filter = TRUE) {
    $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

    $element = array(
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#required' => $component['mandatory'],
        '#weight' => $component['weight'],
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
        '#theme_wrappers' => array('webform_element'),
        '#pre_render' => array(), // Needed to disable double-wrapping of radios and checkboxes.
        '#translatable' => array('title', 'description', 'options'),
    );

    // Convert the user-entered options list into an array.
    $default_value = $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'];
    $options = _webform_smg_select_options($component, !$component['extra']['aslist'], $filter);

    if ($component['extra']['optrand']) {
        _webform_shuffle_smg_options($options);
    }

    // Add default options if using a select list with no default. This triggers
    // Drupal 7's adding of the option for us. See @form_process_select().
    if ($component['extra']['aslist'] && !$component['extra']['multiple'] && $default_value === '') {
        $element['#empty_value'] = '';
    }

    // Set the component options.
    $element['#options'] = $options;

    // Set the default value.
    if (isset($value)) {
        if ($component['extra']['multiple']) {
            // Set the value as an array.
            $element['#default_value'] = array();
            foreach ((array) $value as $key => $option_value) {
                $element['#default_value'][] = $option_value;
            }
        }
        else {
            // Set the value as a single string.
            $element['#default_value'] = '';
            foreach ((array) $value as $option_value) {
                $element['#default_value'] = $option_value;
            }
        }
    }
    elseif ($default_value !== '') {
        // Convert default value to a list if necessary.
        if ($component['extra']['multiple']) {
            $varray = explode(',', $default_value);
            foreach ($varray as $key => $v) {
                $v = trim($v);
                if ($v !== '') {
                    $element['#default_value'][] = $v;
                }
            }
        }
        else {
            $element['#default_value'] = $default_value;
        }
    }
    elseif ($component['extra']['multiple']) {
        $element['#default_value'] = array();
    }

    if ($component['extra']['other_option'] && module_exists('select_or_other')) {
        // Set display as a select_or_other element:
        $element['#type'] = 'select_or_other';
        $element['#other'] = !empty($component['extra']['other_text']) ? check_plain($component['extra']['other_text']) : t('Other...');
        $element['#other_title'] = $element['#title'] . ' ' . $element['#other'];
        $element['#other_title_display'] = 'invisible';
        $element['#other_unknown_defaults'] = 'other';
        $element['#other_delimiter'] = ', ';
        // Merge in Webform's #process function for Select or other.
        $element['#process'] = array_merge(element_info_property('select_or_other', '#process'), array('webform_expand_smg_select_or_other'));

        if ($component['extra']['multiple']) {
            $element['#multiple'] = TRUE;
            $element['#select_type'] = 'checkboxes';
        }
        else {
            $element['#multiple'] = FALSE;
            $element['#select_type'] = 'radios';
        }
        if ($component['extra']['aslist']) {
            $element['#select_type'] = 'select';
        }
    }
    elseif ($component['extra']['aslist']) {
        // Set display as a select list:
        $element['#type'] = 'select';
        if ($component['extra']['multiple']) {
            $element['#size'] = 4;
            $element['#multiple'] = TRUE;
        }
    }
    else {
        if ($component['extra']['multiple']) {
            // Set display as a checkbox set.
            $element['#type'] = 'checkboxes';
            $element['#theme_wrappers'] = array_merge(array('checkboxes'), $element['#theme_wrappers']);
            $element['#process'] = array_merge(element_info_property('checkboxes', '#process'), array('webform_expand_smg_select_ids'));

            // Entirely replace the normal expand checkboxes with our custom version.
            // This helps render checkboxes in multipage forms.
            $process_key = array_search('form_process_checkboxes', $element['#process']);
            $element['#process'][$process_key] = 'webform_expand_smg_checkboxes';
        }
        else {
            // Set display as a radio set.
            $element['#type'] = 'radios';
            $element['#theme_wrappers'] = array_merge(array('radios'), $element['#theme_wrappers']);
            $element['#process'] = array_merge(element_info_property('radios', '#process'), array('webform_expand_smg_select_ids'));
        }
    }

    return $element;
}

/**
  * Process function to ensure select_or_other elements validate properly.
  */
function webform_expand_smg_select_or_other($element) {
    // Disable validation for back-button and save draft.
    $element['select']['#validated'] = TRUE;
    $element['select']['#webform_validated'] = FALSE;

    $element['other']['#validated'] = TRUE;
    $element['other']['#webform_validated'] = FALSE;

    // The Drupal FAPI does not support #title_display inline so we need to move
    // to a supported value here to be compatible with select_or_other.
    $element['select']['#title_display'] = $element['#title_display'] === 'inline' ? 'before' : $element['#title_display'];

    // If the default value contains "select_or_other" (the key of the select
    // element for the "other..." choice), discard it and set the "other" value.
    if (is_array($element['#default_value']) && in_array('select_or_other', $element['#default_value'])) {
        $key = array_search('select_or_other', $element['#default_value']);
        unset($element['#default_value'][$key]);
        $element['#default_value'] = array_values($element['#default_value']);
        $element['other']['#default_value'] = implode(', ', $element['#default_value']);
    }

    // Sanitize the options in Select or other check boxes and radio buttons.
    if ($element['#select_type'] == 'checkboxes' || $element['#select_type'] == 'radios') {
        $element['select']['#process'] = array_merge(element_info_property($element['#select_type'], '#process'), array('webform_expand_smg_select_ids'));
    }

    return $element;
}

/**
  * Drupal 6 hack that properly *renders* checkboxes in multistep forms. This is
  * different than the value hack needed in Drupal 5, which is no longer needed.
  */
function webform_expand_smg_checkboxes($element) {
    // Elements that have a value set are already in the form structure cause
    // them not to be written when the expand_checkboxes function is called.
    $default_value = array();
    foreach (element_children($element) as $key) {
        if (isset($element[$key]['#default_value'])) {
            $default_value[$key] = $element[$key]['#default_value'];
            unset($element[$key]);
        }
    }

    $element = form_process_checkboxes($element);

    // Escape the values of checkboxes.
    foreach (element_children($element) as $key) {
        $element[$key]['#return_value'] = check_plain($element[$key]['#return_value']);
        $element[$key]['#name'] = $element['#name'] . '[' . $element[$key]['#return_value'] . ']';
    }

    foreach ($default_value as $key => $val) {
        $element[$key]['#default_value'] = $val;
    }
    return $element;
}

/**
  * FAPI process function to rename IDs attached to checkboxes and radios.
  */
function webform_expand_smg_select_ids($element) {
    $id = $element['#id'] = str_replace('_', '-', _webform_safe_name(strip_tags($element['#id'])));
    $delta = 0;
    foreach (element_children($element) as $key) {
        $delta++;
        // Convert the #id for each child to a safe name, regardless of key.
        $element[$key]['#id'] = $id . '-' . $delta;

        // Prevent scripts or CSS in the labels for each checkbox or radio.
        $element[$key]['#title'] = _webform_filter_xss($element[$key]['#title']);
    }
    return $element;
}

/**
  * Implements _webform_display_component().
  */
function _webform_display_smg_select($component, $value, $format = 'html') {
    return array(
        '#title' => $component['name'],
        '#weight' => $component['weight'],
        '#theme' => 'webform_display_smg_select',
        '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
        '#format' => $format,
        '#options' => _webform_smg_select_options($component, !$component['extra']['aslist']),
        '#value' => (array) $value,
        '#translatable' => array('title', 'options'),
    );
}

/**
  * Implements _webform_submit_component().
  *
  * Convert FAPI 0/1 values into something saveable.
  */
function _webform_submit_smg_select($component, $value) {
    // Build a list of all valid keys expected to be submitted.
    $options = _webform_smg_select_options($component, TRUE, TRUE);

    $return = NULL;
    if (is_array($value)) {
        $return = array();
        foreach ($value as $key => $option_value) {
            // Handle options that are specified options.
            if ($option_value !== '' && isset($options[$option_value])) {
                // Checkboxes submit an integer value of 0 when unchecked. A checkbox
                // with a value of '0' is valid, so we can't use empty() here.
                if ($option_value === 0 && !$component['extra']['aslist'] && $component['extra']['multiple']) {
                    //unset($value[$option_value]);
                }
                else {
                    $return[] = $option_value;
                }
            }
            // Handle options that are added through the "other" field. Specifically
            // exclude the "select_or_other" value, which is added by the select list.
            elseif ($component['extra']['other_option'] && module_exists('select_or_other') && $option_value != 'select_or_other') {
                $return[] = $option_value;
            }
        }
    }
    elseif (is_string($value)) {
        $return = $value;
    }

    return $return;
}

/**
  * Format the text output for this component.
  */
function theme_webform_display_smg_select($variables) {
    $element = $variables['element'];
    $component = $element['#webform_component'];

    // Flatten the list of options so we can get values easily. These options
    // may be translated by hook_webform_display_component_alter().
    $options = array();
    foreach ($element['#options'] as $key => $value) {
        if (is_array($value)) {
            foreach ($value as $subkey => $subvalue) {
                $options[$subkey] = $subvalue;
            }
        }
        else {
            $options[$key] = $value;
        }
    }

    $items = array();
    if ($component['extra']['multiple']) {
        foreach ((array) $element['#value'] as $option_value) {
            if ($option_value !== '') {
                // Administer provided values.
                if (isset($options[$option_value])) {
                    $items[] = $element['#format'] == 'html' ? _webform_filter_xss($options[$option_value]) : $options[$option_value];
                }
                // User-specified in the "other" field.
                else {
                    $items[] = $element['#format'] == 'html' ? check_plain($option_value) : $option_value;
                }
            }
        }
    }
    else {
        if (isset($element['#value'][0]) && $element['#value'][0] !== '') {
            // Administer provided values.
            if (isset($options[$element['#value'][0]])) {
                $items[] = $element['#format'] == 'html' ? _webform_filter_xss($options[$element['#value'][0]]) : $options[$element['#value'][0]];
            }
            // User-specified in the "other" field.
            else {
                $items[] = $element['#format'] == 'html' ? check_plain($element['#value'][0]) : $element['#value'][0];
            }
        }
    }

    if ($element['#format'] == 'html') {
        $output = count($items) > 1 ? theme('item_list', array('items' => $items)) : (isset($items[0]) ? $items[0] : ' ');
    }
    else {
        if (count($items) > 1) {
            foreach ($items as $key => $item) {
                $items[$key] = ' - ' . $item;
            }
            $output = implode("\n", $items);
        }
        else {
            $output = isset($items[0]) ? $items[0] : ' ';
        }
    }

    return $output;
}

/**
  * Implements _webform_analysis_component().
  */
function _webform_analysis_smg_select($component, $sids = array(), $single = FALSE) {
    $options = _webform_smg_select_options($component, TRUE);
    $show_other_results = $single;

    $sid_placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
    $sid_filter = count($sids) ? " AND sid IN (" . implode(",", $sid_placeholders) . ")" : "";

    $option_operator = $show_other_results ? 'NOT IN' : 'IN';
    $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
        ->fields('wsd', array('data'))
        ->condition('nid', $component['nid'])
        ->condition('cid', $component['cid'])
        ->condition('data', '', '<>')
        ->condition('data', array_keys($options), $option_operator)
        ->groupBy('data');
    $query->addExpression('COUNT(data)', 'datacount');

    if (count($sids)) {
        $query->condition('sid', $sids, 'IN');
    }

    $count_query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
        ->condition('nid', $component['nid'])
        ->condition('cid', $component['cid'])
        ->condition('data', '', '<>');
    $count_query->addExpression('COUNT(*)', 'datacount');
    if (count($sids)) {
        $count_query->condition('sid', $sids, 'IN');
    }

    $result = $query->execute();
    $rows = array();
    $normal_count = 0;
    foreach ($result as $data) {
        $display_option = $single ? $data['data'] : $options[$data['data']];
        $rows[$data['data']] = array(_webform_filter_xss($display_option), $data['datacount']);
        $normal_count += $data['datacount'];
    }

    if (!$show_other_results) {
        // Order the results according to the normal options array.
        $ordered_rows = array();
        foreach (array_intersect_key($options, $rows) as $key => $label) {
            $ordered_rows[] = $rows[$key];
        }

        // Add a row for any unknown or user-entered values.
        if ($component['extra']['other_option']) {
            $full_count = $count_query->execute()->fetchField();
            $other_count = $full_count - $normal_count;
            $display_option = !empty($component['extra']['other_text']) ? check_plain($component['extra']['other_text']) : t('Other...');
            $other_text = $other_count ? $other_count . ' (' . l(t('view'), 'node/' . $component['nid'] . '/webform-results/analysis/' . $component['cid']) . ')' : $other_count;
            $ordered_rows[] = array($display_option, $other_text);
        }

        $rows = $ordered_rows;
    }

    return $rows;
}

/**
  * Implements _webform_table_component().
  */
function _webform_table_smg_select($component, $value) {
    // Convert submitted 'safe' values to un-edited, original form.
    $options = _webform_smg_select_options($component, TRUE);

    $value = (array) $value;
    $items = array();
    // Set the value as a single string.
    foreach ($value as $option_value) {
        if ($option_value !== '') {
            if (isset($options[$option_value])) {
                $items[] = _webform_filter_xss($options[$option_value]);
            }
            else {
                $items[] = check_plain($option_value);
            }
        }
    }

    return implode('<br />', $items);
}

/**
  * Implements _webform_csv_headers_component().
  */
function _webform_csv_headers_smg_select($component, $export_options) {
    $headers = array(
        0 => array(),
        1 => array(),
        2 => array(),
    );

    if ($component['extra']['multiple'] && $export_options['select_format'] == 'separate') {
        $headers[0][] = '';
        $headers[1][] = $component['name'];
        $items = _webform_smg_select_options($component, TRUE, FALSE);
        if ($component['extra']['other_option']) {
            $other_label = !empty($component['extra']['other_text']) ? check_plain($component['extra']['other_text']) : t('Other...');
            $items[$other_label] = $other_label;
        }
        $count = 0;
        foreach ($items as $key => $item) {
            // Empty column per sub-field in main header.
            if ($count != 0) {
                $headers[0][] = '';
                $headers[1][] = '';
            }
            if ($export_options['select_keys']) {
                $headers[2][] = $key;
            }
            else {
                $headers[2][] = $item;
            }
            $count++;
        }
    }
    else {
        $headers[0][] = '';
        $headers[1][] = '';
        $headers[2][] = $component['name'];
    }
    return $headers;
}

/**
  * Implements _webform_csv_data_component().
  */
function _webform_csv_data_smg_select($component, $export_options, $value) {
    $options = _webform_smg_select_options($component, TRUE, FALSE);
    $return = array();

    if ($component['extra']['multiple']) {
        foreach ($options as $key => $item) {
            $index = array_search($key, (array) $value);
            if ($index !== FALSE) {
                if ($export_options['select_format'] == 'separate') {
                    $return[] = 'X';
                }
                else {
                    $return[] = $export_options['select_keys'] ? $key : $item;
                }
                unset($value[$index]);
            }
            elseif ($export_options['select_format'] == 'separate') {
                $return[] = '';
            }
        }

        // Any remaining items in the $value array will be user-added options.
        if ($component['extra']['other_option']) {
            $return[] = count($value) ? implode(',', $value) : '';
        }
    }
    else {
        $key = $value[0];
        if ($export_options['select_keys']) {
            $return = $key;
        }
        else {
            $return = isset($options[$key]) ? $options[$key] : $key;
        }
    }

    if ($component['extra']['multiple'] && $export_options['select_format'] == 'compact') {
        $return = implode(',', (array) $return);
    }

    return $return;
}

/**
  * Menu callback; Return a predefined list of select options as JSON.
  */
/*function webform_select_options_ajax($source_name = '') {
    $info = _webform_smg_select_options_info();

    $component['extra']['options_source'] = $source_name;
    if ($source_name && isset($info[$source_name])) {
        $options = _webform_smg_select_options_to_text(_webform_smg_select_options($component, !$component['extra']['aslist'], FALSE));
    }
    else {
        $options = '';
    }

    $return = array(
        'elementId' => module_exists('options_element') ? 'edit-items-options-options-field-widget' : 'edit-extra-items',
        'options' => $options,
    );

    drupal_json_output($return);
}*/

/**
  * Generate a list of options for a select list.
  */
function _webform_smg_select_options($component, $flat = FALSE, $filter = TRUE, &$slugs = array(), &$accela_ids = array()) {
    if ($component['extra']['options_source']) {
        $options = _webform_smg_select_options_callback($component['extra']['options_source'], $component, $flat, $filter);
    }
    else {
        $options = _webform_smg_select_options_from_text($component['extra']['items'], $flat, $filter, $slugs, $accela_ids);
    }

    return isset($options) ? $options : array();
}

/**
  * Load Webform select option info from 3rd party modules.
  */
function _webform_smg_select_options_info() {
    static $info;
    if (!isset($info)) {
        $info = array();

        foreach (module_implements('webform_select_options_info') as $module) {
            $additions = module_invoke($module, 'webform_select_options_info');
            foreach ($additions as $key => $addition) {
                $additions[$key]['module'] = $module;
            }
            $info = array_merge($info, $additions);
        }
        drupal_alter('webform_select_options_info', $info);
    }
    return $info;
}

/**
  * Create automatic machine names values given human-readable names
  */
function _webform_smg_select_machine_name($human_name, $returned) {
    $return = strtolower($human_name);
    //$return = preg_replace('/\([^()]*\)/', '', $return);    // delete everything in parentheses
    //$return = preg_replace('/&/', 'and', $return);
    //$return = preg_replace('/\+/', 'plus', $return);
    //$return = preg_replace('/@/', 'at', $return);
    //$return = preg_replace('/^\d/', '-', $return);          // change initial digit to '-'
    $return = preg_replace('/[^\w-]/', '-', $return);       // change illegal characters to dashes
    $return = preg_replace('/-{2,}/', '-', $return);        // collapse all multiple dashes
    $return = trim($return, " \t\n\r\0\x0B-");             // trim bs ws
    while (in_array($return, $returned)) {
        $return = "-$return";                               //prepend dashes until we have a unique index
    }

    return $return;
}

/**
  * Execute a select option callback.
  *
  * @param $name
  *   The name of the options group.
  * @param $component
  *   The full Webform component.
  * @param $flat
  *   Whether the information returned should exclude any nested groups.
  * @param $filter
  *   Whether information returned should be sanitized. Defaults to TRUE.
  */
function _webform_smg_select_options_callback($name, $component, $flat = FALSE, $filter = TRUE) {
    $info = _webform_smg_select_options_info();

    // Include any necessary files.
    if (isset($info[$name]['file'])) {
        $pathinfo = pathinfo($info[$name]['file']);
        $path = ($pathinfo['dirname'] ? $pathinfo['dirname'] . '/' : '') . basename($pathinfo['basename'], '.' . $pathinfo['extension']);
        module_load_include($pathinfo['extension'], $info[$name]['module'], $path);
    }

    // Execute the callback function.
    if (isset($info[$name]['options callback']) && function_exists($info[$name]['options callback'])) {
        $function = $info[$name]['options callback'];

        $arguments = array();
        if (isset($info[$name]['options arguments'])) {
            $arguments = $info[$name]['options arguments'];
        }

        return $function($component, $flat, $filter, $arguments);
    }
}

/**
  * Utility function to split user-entered values from new-line seperated
  * text into an array of options.
  *
  * @param $text
  *   Text to be converted into a select option array.
  * @param $flat
  *   Optional. If specified, return the option array and exclude any optgroups.
  * @param $filter
  *   Optional. Whether or not to filter returned values.
  * @param slugs
  *   Optional. Filled with slug values (usually shorter values to be inserted to the db).
  * @param accela_ids
  *   Optional. Filled with AccelaWorks ID values, if given.
  */
function _webform_smg_select_options_from_text($text, $flat = FALSE, $filter = TRUE, &$slugs = array(), &$accela_ids = array(), &$leadworks_ids = array()) {
    static $cache = array();

    // Keep each processed option block in an array indexed by the MD5 hash of
    // the option text and the value of the $flat variable.
    $md5 = md5($text);

    // Check if this option block has been previously processed.
    if (!isset($cache[$flat][$md5])) {
        $options = array();
        $rows = array_filter(explode("\n", trim($text)));
        $group = NULL;
        $indices = array();
        foreach ($rows as $option) {
            $option = trim($option);
            /**
              * If the Key of the option is within < >, treat as an optgroup
              *
              * <Group 1>
              *   creates an optgroup with the label "Group 1"
              *
              * <>
              *   Unsets the current group, allowing items to be inserted at the root element.
              */
            if (preg_match('/^\<([^>]*)\>$/', $option, $matches)) {
                if (empty($matches[1])) {
                    unset($group);
                }
                elseif (!$flat) {
                    $group = $filter ? _webform_filter_values($matches[1], NULL, NULL, NULL, FALSE) : $matches[1];
                }
            }
            /// case: "submit value"|"display value"|"accelaworks id"|"leadworks id"
            elseif (preg_match('/^([^|]*)\|([^|]+)\|([^|]+)\|([^|]+)$/', $option, $matches)) {
                $slug = trim($matches[1]); // "submit value"
                $value = $filter ? _webform_filter_values($matches[2], NULL, NULL, NULL, FALSE) : $matches[2];
                $accela_id = $matches[3];
                $leadworks_id = $matches[4];
                $index = $slug?$slug:$value; ///< preindex is the starting point for generating an array index for this option
                //$index = _webform_smg_select_machine_name($preindex, $indices); ///< automatically generate component index
                //$indices[] = $index;
                $slugs[$index] = $slug;
                $accela_ids[$index] = $accela_id;
                $leadworks_ids[$index] = $leadworks_id;
                isset($group) ? $options[$group][$index] = $value : $options[$index] = $value;
            }
            /// case: "submit value"|"display value"|"accelaworks id"
            elseif (preg_match('/^([^|]*)\|([^|]+)\|([^|]+)$/', $option, $matches)) {
                $slug = trim($matches[1]); // "submit value"
                $value = $filter ? _webform_filter_values($matches[2], NULL, NULL, NULL, FALSE) : $matches[2];
                $accela_id = $matches[3];
                $index = $slug?$slug:$value; ///< preindex is the starting point for generating an array index for this option
                //$index = _webform_smg_select_machine_name($preindex, $indices); ///< automatically generate component index
                //$indices[] = $index;
                $slugs[$index] = $slug;
                $accela_ids[$index] = $accela_id;
                isset($group) ? $options[$group][$index] = $value : $options[$index] = $value;
            }
            /// case: "submit value"|"display value"
            elseif (preg_match('/^([^|]+)\|([^|]+)$/', $option, $matches)) {
                $slug = trim($matches[1]);
                $value = $filter ? _webform_filter_values($matches[2], NULL, NULL, NULL, FALSE) : $matches[2];
                $index = $slug?$slug:$value;
                //$index = _webform_smg_select_machine_name($preindex, $indices);
                //$indices[] = $index;
                $slugs[$index] = $slug;
                isset($group) ? $options[$group][$index] = $value : $options[$index] = $value;
            }
            /// case: "submit and display value"
            else {
                $value = $filter ? _webform_filter_values($option, NULL, NULL, NULL, FALSE) : $option;
                $index = $value;
                //$index = _webform_smg_select_machine_name($value, $indices);
                //$indices[] = $index;
                isset($group) ? $options[$group][$index] = $value : $options[$index] = $value;
            }
        }

        $cache[$flat][$md5][0] = $options;
        $cache[$flat][$md5][1] = $accela_ids;
        $cache[$flat][$md5][2] = $slugs;
        $cache[$flat][$md5][3] = $leadworks_ids;
    }

    // Recover cached values
    $options = $cache[$flat][$md5][0];
    $accela_ids = $cache[$flat][$md5][1];
    $slugs = $cache[$flat][$md5][2];
    $leadworks_ids = $cache[$flat][$md5][3];

    // Return our options from the cache array.
    return $options;
}

/**
  * Convert an array of options into text.
  */
function _webform_smg_select_options_to_text($options) {
    $output = '';
    $previous_key = FALSE;

    foreach ($options as $key => $value) {
        // Convert groups.
        if (is_array($value)) {
            $output .= '<' . $key . '>' . "\n";
            foreach ($value as $subkey => $subvalue) {
                $output .= $subkey . '|' . $subvalue . "\n";
            }
            $previous_key = $key;
        }
        // Typical key|value pairs.
        else {
            // Exit out of any groups.
            if (isset($options[$previous_key]) && is_array($options[$previous_key])) {
                $output .= "<>\n";
            }
            // Skip empty rows.
            if ($options[$key] !== '') {
                $output .= $key . '|' . $value . "\n";
            }
            $previous_key = $key;
        }
    }

    return $output;
}

/**
  * Utility function to shuffle an array while preserving key-value pairs.
  */
function _webform_shuffle_smg_options(&$array) {
    // First shuffle the array keys, then use them as the basis for ordering
    // the options.
    $aux = array();
    $keys = array_keys($array);
    shuffle($keys);
    foreach ($keys as $key) {
        $aux[$key] = $array[$key];
    }
    $array = $aux;
}

/**
 * Defines a list of countries suitable for passing to Silverpop
 */
function playbook_fields_options_countries() {
  $countries = array(
    'UNITED STATES' => 'United States',
    'CANADA' => 'Canada',
    'AFGHANISTAN' => 'Afghanistan',
    'ALAND ISLANDS' => 'Aland Islands',
    'ALBANIA' => 'Albania',
    'ALGERIA' => 'Algeria',
    'AMERICAN SAMOA' => 'American Samoa',
    'ANDORRA' => 'Andorra',
    'ANGOLA' => 'Angola',
    'ANGUILLA' => 'Anguilla',
    'ANTARCTICA' => 'Antarctica',
    'ANTIGUA AND BARBUDA' => 'Antigua and Barbuda',
    'ARGENTINA' => 'Argentina',
    'ARMENIA' => 'Armenia',
    'ARUBA' => 'Aruba',
    'AUSTRALIA' => 'Australia',
    'AUSTRIA' => 'Austria',
    'AZERBAIJAN' => 'Azerbaijan',
    'BAHAMAS' => 'Bahamas',
    'BAHRAIN' => 'Bahrain',
    'BANGLADESH' => 'Bangladesh',
    'BARBADOS' => 'Barbados',
    'BELARUS' => 'Belarus',
    'BELGIUM' => 'Belgium',
    'BELIZE' => 'Belize',
    'BENIN' => 'Benin',
    'BERMUDA' => 'Bermuda',
    'BHUTAN' => 'Bhutan',
    'BOLIVIA' => 'Bolivia',
    'BOSNIA AND HERZEGOVINA' => 'Bosnia and Herzegovina',
    'BOTSWANA' => 'Botswana',
    'BOUVET ISLAND' => 'Bouvet Island',
    'BRAZIL' => 'Brazil',
    'BRITISH INDIAN OCEAN TERRITORY' => 'British Indian Ocean Territory',
    'BRITISH VIRGIN ISLANDS' => 'British Virgin Islands',
    'BRUNEI' => 'Brunei',
    'BULGARIA' => 'Bulgaria',
    'BURKINA FASO' => 'Burkina Faso',
    'BURUNDI' => 'Burundi',
    'CAMBODIA' => 'Cambodia',
    'CAMEROON' => 'Cameroon',
    'CAPE VERDE' => 'Cape Verde',
    'CAYMAN ISLANDS' => 'Cayman Islands',
    'CENTRAL AFRICAN REPUBLIC' => 'Central African Republic',
    'CHAD' => 'Chad',
    'CHILE' => 'Chile',
    'CHINA' => 'China',
    'CHRISTMAS ISLAND' => 'Christmas Island',
    'COCOS (KEELING) ISLANDS' => 'Cocos (Keeling) Islands',
    'COLOMBIA' => 'Colombia',
    'COMOROS' => 'Comoros',
    'CONGO (BRAZZAVILLE)' => 'Congo (Brazzaville)',
    'CONGO (KINSHASA)' => 'Congo (Kinshasa)',
    'COOK ISLANDS' => 'Cook Islands',
    'COSTA RICA' => 'Costa Rica',
    'CROATIA' => 'Croatia',
    'CUBA' => 'Cuba',
    'CURAÇAO' => 'Curaçao',
    'CYPRUS' => 'Cyprus',
    'CZECH REPUBLIC' => 'Czech Republic',
    'DENMARK' => 'Denmark',
    'DJIBOUTI' => 'Djibouti',
    'DOMINICA' => 'Dominica',
    'DOMINICAN REPUBLIC' => 'Dominican Republic',
    'ECUADOR' => 'Ecuador',
    'EGYPT' => 'Egypt',
    'EL SALVADOR' => 'El Salvador',
    'EQUATORIAL GUINEA' => 'Equatorial Guinea',
    'ERITREA' => 'Eritrea',
    'ESTONIA' => 'Estonia',
    'ETHIOPIA' => 'Ethiopia',
    'FALKLAND ISLANDS' => 'Falkland Islands',
    'FAROE ISLANDS' => 'Faroe Islands',
    'FIJI' => 'Fiji',
    'FINLAND' => 'Finland',
    'FRANCE' => 'France',
    'FRENCH GUIANA' => 'French Guiana',
    'FRENCH POLYNESIA' => 'French Polynesia',
    'FRENCH SOUTHERN TERRITORIES' => 'French Southern Territories',
    'GABON' => 'Gabon',
    'GAMBIA' => 'Gambia',
    'GEORGIA' => 'Georgia',
    'GERMANY' => 'Germany',
    'GHANA' => 'Ghana',
    'GIBRALTAR' => 'Gibraltar',
    'GREECE' => 'Greece',
    'GREENLAND' => 'Greenland',
    'GRENADA' => 'Grenada',
    'GUADELOUPE' => 'Guadeloupe',
    'GUAM' => 'Guam',
    'GUATEMALA' => 'Guatemala',
    'GUERNSEY' => 'Guernsey',
    'GUINEA' => 'Guinea',
    'GUINEA-BISSAU' => 'Guinea-Bissau',
    'GUYANA' => 'Guyana',
    'HAITI' => 'Haiti',
    'HEARD ISLAND AND MCDONALD ISLANDS' => 'Heard Island and McDonald Islands',
    'HONDURAS' => 'Honduras',
    'HONG KONG S.A.R., CHINA' => 'Hong Kong S.A.R., China',
    'HUNGARY' => 'Hungary',
    'ICELAND' => 'Iceland',
    'INDIA' => 'India',
    'INDONESIA' => 'Indonesia',
    'IRAN' => 'Iran',
    'IRAQ' => 'Iraq',
    'IRELAND' => 'Ireland',
    'ISLE OF MAN' => 'Isle of Man',
    'ISRAEL' => 'Israel',
    'ITALY' => 'Italy',
    'IVORY COAST' => 'Ivory Coast',
    'JAMAICA' => 'Jamaica',
    'JAPAN' => 'Japan',
    'JERSEY' => 'Jersey',
    'JORDAN' => 'Jordan',
    'KAZAKHSTAN' => 'Kazakhstan',
    'KENYA' => 'Kenya',
    'KIRIBATI' => 'Kiribati',
    'KUWAIT' => 'Kuwait',
    'KYRGYZSTAN' => 'Kyrgyzstan',
    'LAOS' => 'Laos',
    'LATVIA' => 'Latvia',
    'LEBANON' => 'Lebanon',
    'LESOTHO' => 'Lesotho',
    'LIBERIA' => 'Liberia',
    'LIBYA' => 'Libya',
    'LIECHTENSTEIN' => 'Liechtenstein',
    'LITHUANIA' => 'Lithuania',
    'LUXEMBOURG' => 'Luxembourg',
    'MACAO S.A.R., CHINA' => 'Macao S.A.R., China',
    'MACEDONIA' => 'Macedonia',
    'MADAGASCAR' => 'Madagascar',
    'MALAWI' => 'Malawi',
    'MALAYSIA' => 'Malaysia',
    'MALDIVES' => 'Maldives',
    'MALI' => 'Mali',
    'MALTA' => 'Malta',
    'MARSHALL ISLANDS' => 'Marshall Islands',
    'MARTINIQUE' => 'Martinique',
    'MAURITANIA' => 'Mauritania',
    'MAURITIUS' => 'Mauritius',
    'MAYOTTE' => 'Mayotte',
    'MEXICO' => 'Mexico',
    'MICRONESIA' => 'Micronesia',
    'MOLDOVA' => 'Moldova',
    'MONACO' => 'Monaco',
    'MONGOLIA' => 'Mongolia',
    'MONTENEGRO' => 'Montenegro',
    'MONTSERRAT' => 'Montserrat',
    'MOROCCO' => 'Morocco',
    'MOZAMBIQUE' => 'Mozambique',
    'MYANMAR' => 'Myanmar',
    'NAMIBIA' => 'Namibia',
    'NAURU' => 'Nauru',
    'NEPAL' => 'Nepal',
    'NETHERLANDS' => 'Netherlands',
    'NETHERLANDS ANTILLES' => 'Netherlands Antilles',
    'NEW CALEDONIA' => 'New Caledonia',
    'NEW ZEALAND' => 'New Zealand',
    'NICARAGUA' => 'Nicaragua',
    'NIGER' => 'Niger',
    'NIGERIA' => 'Nigeria',
    'NIUE' => 'Niue',
    'NORFOLK ISLAND' => 'Norfolk Island',
    'NORTHERN MARIANA ISLANDS' => 'Northern Mariana Islands',
    'NORTH KOREA' => 'North Korea',
    'NORWAY' => 'Norway',
    'OMAN' => 'Oman',
    'PAKISTAN' => 'Pakistan',
    'PALAU' => 'Palau',
    'PALESTINIAN TERRITORY' => 'Palestinian Territory',
    'PANAMA' => 'Panama',
    'PAPUA NEW GUINEA' => 'Papua New Guinea',
    'PARAGUAY' => 'Paraguay',
    'PERU' => 'Peru',
    'PHILIPPINES' => 'Philippines',
    'PITCAIRN' => 'Pitcairn',
    'POLAND' => 'Poland',
    'PORTUGAL' => 'Portugal',
    'PUERTO RICO' => 'Puerto Rico',
    'QATAR' => 'Qatar',
    'REUNION' => 'Reunion',
    'ROMANIA' => 'Romania',
    'RUSSIA' => 'Russia',
    'RWANDA' => 'Rwanda',
    'SAINT BARTHÉLEMY' => 'Saint Barthélemy',
    'SAINT HELENA' => 'Saint Helena',
    'SAINT KITTS AND NEVIS' => 'Saint Kitts and Nevis',
    'SAINT LUCIA' => 'Saint Lucia',
    'SAINT MARTIN (FRENCH PART)' => 'Saint Martin (French part)',
    'SAINT PIERRE AND MIQUELON' => 'Saint Pierre and Miquelon',
    'SAINT VINCENT AND THE GRENADINES' => 'Saint Vincent and the Grenadines',
    'SAMOA' => 'Samoa',
    'SAN MARINO' => 'San Marino',
    'SAO TOME AND PRINCIPE' => 'Sao Tome and Principe',
    'SAUDI ARABIA' => 'Saudi Arabia',
    'SENEGAL' => 'Senegal',
    'SERBIA' => 'Serbia',
    'SEYCHELLES' => 'Seychelles',
    'SIERRA LEONE' => 'Sierra Leone',
    'SINGAPORE' => 'Singapore',
    'SLOVAKIA' => 'Slovakia',
    'SLOVENIA' => 'Slovenia',
    'SOLOMON ISLANDS' => 'Solomon Islands',
    'SOMALIA' => 'Somalia',
    'SOUTH AFRICA' => 'South Africa',
    'SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS' => 'South Georgia and the South Sandwich Islands',
    'SOUTH KOREA' => 'South Korea',
    'SPAIN' => 'Spain',
    'SRI LANKA' => 'Sri Lanka',
    'SUDAN' => 'Sudan',
    'SURINAME' => 'Suriname',
    'SVALBARD AND JAN MAYEN' => 'Svalbard and Jan Mayen',
    'SWAZILAND' => 'Swaziland',
    'SWEDEN' => 'Sweden',
    'SWITZERLAND' => 'Switzerland',
    'SYRIA' => 'Syria',
    'TAIWAN' => 'Taiwan',
    'TAJIKISTAN' => 'Tajikistan',
    'TANZANIA' => 'Tanzania',
    'THAILAND' => 'Thailand',
    'TIMOR-LESTE' => 'Timor-Leste',
    'TOGO' => 'Togo',
    'TOKELAU' => 'Tokelau',
    'TONGA' => 'Tonga',
    'TRINIDAD AND TOBAGO' => 'Trinidad and Tobago',
    'TUNISIA' => 'Tunisia',
    'TURKEY' => 'Turkey',
    'TURKMENISTAN' => 'Turkmenistan',
    'TURKS AND CAICOS ISLANDS' => 'Turks and Caicos Islands',
    'TUVALU' => 'Tuvalu',
    'U.S. VIRGIN ISLANDS' => 'U.S. Virgin Islands',
    'UGANDA' => 'Uganda',
    'UKRAINE' => 'Ukraine',
    'UNITED ARAB EMIRATES' => 'United Arab Emirates',
    'UNITED KINGDOM' => 'United Kingdom',
    'UNITED STATES MINOR OUTLYING ISLANDS' => 'United States Minor Outlying Islands',
    'URUGUAY' => 'Uruguay',
    'UZBEKISTAN' => 'Uzbekistan',
    'VANUATU' => 'Vanuatu',
    'VATICAN' => 'Vatican',
    'VENEZUELA' => 'Venezuela',
    'VIETNAM' => 'Vietnam',
    'WALLIS AND FUTUNA' => 'Wallis and Futuna',
    'WESTERN SAHARA' => 'Western Sahara',
    'YEMEN' => 'Yemen',
    'ZAMBIA' => 'Zambia',
    'ZIMBABWE' => 'Zimbabwe'
  );

  return $countries;
}

/**
 * Defines a list of countries suitable for passing to Salesforce
 */
function playbook_fields_options_force_countries() {
  $countries = array(
    'United States',
    'Afghanistan',
    'Aland Islands',
    'Albania',
    'Algeria',
    'Andorra',
    'Angola',
    'Anguilla',
    'Antarctica',
    'Antigua and Barbuda',
    'Argentina',
    'Armenia',
    'Aruba',
    'Australia',
    'Austria',
    'Azerbaijan',
    'Bahamas',
    'Bahrain',
    'Bangladesh',
    'Barbados',
    'Belarus',
    'Belgium',
    'Belize',
    'Benin',
    'Bermuda',
    'Bhutan',
    'Bolivia, Plurinational State of',
    'Bonaire, Sint Eustatius and Saba',
    'Bosnia and Herzegovina',
    'Botswana',
    'Bouvet Island',
    'Brazil',
    'British Indian Ocean Territory',
    'Brunei Darussalam',
    'Bulgaria',
    'Burkina Faso',
    'Burundi',
    'Cambodia',
    'Cameroon',
    'Canada',
    'Cape Verde',
    'Cayman Islands',
    'Central African Republic',
    'Chad',
    'Chile',
    'China',
    'Chinese Taipei',
    'Christmas Island',
    'Cocos (Keeling) Islands',
    'Colombia',
    'Comoros',
    'Congo',
    'Congo, the Democratic Republic of the',
    'Cook Islands',
    'Costa Rica',
    'Cote d\'Ivoire',
    'Croatia',
    'Cuba',
    'Curaçao',
    'Cyprus',
    'Czech Republic',
    'Denmark',
    'Djibouti',
    'Dominica',
    'Dominican Republic',
    'Ecuador',
    'Egypt',
    'El Salvador',
    'Equatorial Guinea',
    'Eritrea',
    'Estonia',
    'Ethiopia',
    'Falkland Islands (Malvinas)',
    'Faroe Islands',
    'Fiji',
    'Finland',
    'France',
    'French Guiana',
    'French Polynesia',
    'French Southern Territories',
    'Gabon',
    'Gambia',
    'Georgia',
    'Germany',
    'Ghana',
    'Gibraltar',
    'Greece',
    'Greenland',
    'Grenada',
    'Guadeloupe',
    'Guatemala',
    'Guernsey',
    'Guinea',
    'Guinea-Bissau',
    'Guyana',
    'Haiti',
    'Heard Island and McDonald Islands',
    'Holy See (Vatican City State)',
    'Honduras',
    'Hungary',
    'Iceland',
    'India',
    'Indonesia',
    'Iran, Islamic Republic of',
    'Iraq',
    'Ireland',
    'Isle of Man',
    'Israel',
    'Italy',
    'Jamaica',
    'Japan',
    'Jersey',
    'Jordan',
    'Kazakhstan',
    'Kenya',
    'Kiribati',
    'Korea, Democratic People\'s Republic of',
    'Korea, Republic of',
    'Kuwait',
    'Kyrgyzstan',
    'Lao People\'s Democratic Republic',
    'Latvia',
    'Lebanon',
    'Lesotho',
    'Liberia',
    'Libyan Arab Jamahiriya',
    'Liechtenstein',
    'Lithuania',
    'Luxembourg',
    'Macao',
    'Macedonia, the former Yugoslav Republic of',
    'Madagascar',
    'Malawi',
    'Malaysia',
    'Maldives',
    'Mali',
    'Malta',
    'Martinique',
    'Mauritania',
    'Mauritius',
    'Mayotte',
    'Mexico',
    'Moldova, Republic of',
    'Monaco',
    'Mongolia',
    'Montenegro',
    'Montserrat',
    'Morocco',
    'Mozambique',
    'Myanmar',
    'Namibia',
    'Nauru',
    'Nepal',
    'Netherlands',
    'New Caledonia',
    'New Zealand',
    'Nicaragua',
    'Niger',
    'Nigeria',
    'Niue',
    'Norfolk Island',
    'Norway',
    'Oman',
    'Pakistan',
    'Palestinian Territory, Occupied',
    'Panama',
    'Papua New Guinea',
    'Paraguay',
    'Peru',
    'Philippines',
    'Pitcairn',
    'Poland',
    'Portugal',
    'Qatar',
    'Reunion',
    'Romania',
    'Russian Federation',
    'Rwanda',
    'Saint Barthélemy',
    'Saint Helena, Ascension and Tristan da Cunha',
    'Saint Kitts and Nevis',
    'Saint Lucia',
    'Saint Martin (French part)',
    'Saint Pierre and Miquelon',
    'Saint Vincent and the Grenadines',
    'Samoa',
    'San Marino',
    'Sao Tome and Principe',
    'Saudi Arabia',
    'Senegal',
    'Serbia',
    'Seychelles',
    'Sierra Leone',
    'Singapore',
    'Sint Maarten (Dutch part)',
    'Slovakia',
    'Slovenia',
    'Solomon Islands',
    'Somalia',
    'South Africa',
    'South Georgia and the South Sandwich Islands',
    'South Sudan',
    'Spain',
    'Sri Lanka',
    'Sudan',
    'Suriname',
    'Svalbard and Jan Mayen',
    'Swaziland',
    'Sweden',
    'Switzerland',
    'Syrian Arab Republic',
    'Tajikistan',
    'Tanzania, United Republic of',
    'Thailand',
    'Timor-Leste',
    'Togo',
    'Tokelau',
    'Tonga',
    'Trinidad and Tobago',
    'Tunisia',
    'Turkey',
    'Turkmenistan',
    'Turks and Caicos Islands',
    'Tuvalu',
    'Uganda',
    'Ukraine',
    'United Arab Emirates',
    'United Kingdom',
    'Uruguay',
    'Uzbekistan',
    'Vanuatu',
    'Venezuela, Bolivarian Republic of',
    'Viet Nam',
    'Virgin Islands, British',
    'Wallis and Futuna',
    'Western Sahara',
    'Yemen',
    'Zambia',
    'Zimbabwe',
  );

  return $countries;
}

/**
 * Options list containing United States states and territories.
 */
function playbook_fields_options_states($component, $flat, $filter, $arguments) {
  $states = array(
    'AL' => t('Alabama'),
    'AK' => t('Alaska'),
    'AS' => t('American Samoa'),
    'AZ' => t('Arizona'),
    'AR' => t('Arkansas'),
    'CA' => t('California'),
    'CO' => t('Colorado'),
    'CT' => t('Connecticut'),
    'DE' => t('Delaware'),
    'DC' => t('District of Columbia'),
    'FL' => t('Florida'),
    'GA' => t('Georgia'),
    'GU' => t('Guam'),
    'HI' => t('Hawaii'),
    'ID' => t('Idaho'),
    'IL' => t('Illinois'),
    'IN' => t('Indiana'),
    'IA' => t('Iowa'),
    'KS' => t('Kansas'),
    'KY' => t('Kentucky'),
    'LA' => t('Louisiana'),
    'ME' => t('Maine'),
    'MH' => t('Marshall Islands'),
    'MD' => t('Maryland'),
    'MA' => t('Massachusetts'),
    'MI' => t('Michigan'),
    'MN' => t('Minnesota'),
    'MS' => t('Mississippi'),
    'MO' => t('Missouri'),
    'MT' => t('Montana'),
    'NE' => t('Nebraska'),
    'NV' => t('Nevada'),
    'NH' => t('New Hampshire'),
    'NJ' => t('New Jersey'),
    'NM' => t('New Mexico'),
    'NY' => t('New York'),
    'NC' => t('North Carolina'),
    'ND' => t('North Dakota'),
    'MP' => t('Northern Marianas Islands'),
    'OH' => t('Ohio'),
    'OK' => t('Oklahoma'),
    'OR' => t('Oregon'),
    'PW' => t('Palau'),
    'PA' => t('Pennsylvania'),
    'PR' => t('Puerto Rico'),
    'RI' => t('Rhode Island'),
    'SC' => t('South Carolina'),
    'SD' => t('South Dakota'),
    'TN' => t('Tennessee'),
    'TX' => t('Texas'),
    'UT' => t('Utah'),
    'VT' => t('Vermont'),
    'VI' => t('Virgin Islands'),
    'VA' => t('Virginia'),
    'WA' => t('Washington'),
    'WV' => t('West Virginia'),
    'WI' => t('Wisconsin'),
    'WY' => t('Wyoming'),
    'ALBERTA' => t('Alberta'),
    'BRITISH COLUMBIA' => t('British Columbia'),
    'MANITOBA' => t('Manitoba'),
    'NEW BRUNSWICK' => t('New Brunswick'),
    'NEWFOUNDLAND AND LABRADOR' => t('Newfoundland and Labrador'),
    'NORTHWEST TERRITORIES' => t('Northwest Territories'),
    'NOVA SCOTIA' => t('Nova Scotia'),
    'NUNAVUT' => t('Nunavut'),
    'ONTARIO' => t('Ontario'),
    'PRINCE EDWARD ISLAND' => t('Prince Edward Island'),
    'QUEBEC' => t('Quebec'),
    'SASKATCHEWAN' => t('Saskatchewan'),
    'YUKON' => t('Yukon'),
  );

  return $states;
}
