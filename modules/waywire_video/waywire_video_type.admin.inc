<?php

/**
 * @file
 * Waywire Video type editing UI.
 */

/**
 * UI controller.
 */
class WaywireVideoTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage waywire entity types, including adding
    and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the wawywire video type editing form.
 */
function waywire_video_type_form($form, &$form_state, $waywire_video_type, $op = 'edit') {

  if ($op == 'clone') {
    $waywire_video_type->label .= ' (cloned)';
    $waywire_video->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $waywire_video_type->label,
    '#description' => t('The human-readable name of this waywire video type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($waywire_video_type->type) ? $waywire_video_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'waywire_video_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this waywire video type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save waywire video type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function waywire_video_type_form_submit(&$form, &$form_state) {
  $model_type = entity_ui_form_submit_build_entity($form, $form_state);
  $model_type->save();
  $form_state['redirect'] = 'admin/structure/waywire_video_types';
}

/**
 * Form API submit callback for the delete button.
 */
function waywire_video_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/waywire_video_types/manage/' . $form_state['waywire_video_type']->type . '/delete';
}
