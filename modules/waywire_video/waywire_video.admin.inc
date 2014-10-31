<?php

/**
 * @file
 * Waywire Video editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring
 * instead to use  views. That offers more flexibility to change a UI that will,
 * more often than not, be end-user facing.
 */

/**
 * UI controller.
 */
class WaywireVideoUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Waywire Videos',
      'description' => 'Add edit and update waywire videos.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of waywire videos.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a waywire video',
      'description' => 'Add a new waywire video',
      'page callback'  => 'waywire_video_add_page',
      'access callback'  => 'waywire_video_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'waywire_video.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    // Add menu items to add each different type of entity.
    foreach (waywire_video_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'waywire_video_form_wrapper',
        'page arguments' => array(waywire_video_create(array('type' => $type->type))),
        'access callback' => 'waywire_video_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'waywire_video.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing waywire video entities
    $items[$this->path . '/waywire_video/' . $wildcard] = array(
      'page callback' => 'waywire_video_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'waywire_video_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'waywire_video.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/waywire_video/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/waywire_video/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'waywire_video_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'waywire_video_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'waywire_video.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing waywire videos
    $items['waywire_video/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'waywire_video_page_title',
      'title arguments' => array(1),
      'page callback' => 'waywire_video_page_view',
      'page arguments' => array(1),
      'access callback' => 'waywire_video_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }


  /**
   * Create the markup for the add Waywire Video Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('waywire_video_add_list', array('content' => $content));
  }

}

/**
 * Form callback wrapper: create or edit a waywire video.
 *
 * @param object $waywire_video
 *   The waywire video object being edited by this form.
 *
 * @see waywire_video_edit_form()
 */
function waywire_video_form_wrapper($waywire_video) {
  // Add the breadcrumb for the form's location.
  waywire_video_set_breadcrumb();
  return drupal_get_form('waywire_video_edit_form', $waywire_video);
}

/**
 * Form callback wrapper: delete a waywire video.
 *
 * @param object $waywire_video
 *   The waywire viceo object being edited by this form.
 *
 * @see waywire_video_edit_form()
 */
function waywire_video_delete_form_wrapper($waywire_video) {
  // Add the breadcrumb for the form's location.
  return drupal_get_form('waywire_video_delete_form', $waywire_video);
}

/**
 * Form callback: create or edit a waywire video.
 *
 * @param object $waywire_video
 *   The waywire_video object to edit or for a create form an empty
 *   waywire_video object with only a waywire video type defined.
 */
function waywire_video_edit_form($form, &$form_state, $waywire_video) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($waywire_video->name) ? $waywire_video->name : '',
    '#size' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['node_id'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Company'),
    '#default_value' => isset($title) ? $title . ' [nid:' . $waywire_video->node_id . ']' : '',
    '#autocomplete_path' => 'node_reference/autocomplete/user/user/field_ld_company',
    '#required' => FALSE,
  );

  $form['guid'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Magnify ID'),
    '#default_value' => isset($waywire_video->guid) ? $waywire_video->guid : '',
    '#required' => FALSE,
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['magnify_externalid'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Magnify External ID'),
    '#default_value' => isset($waywire_video->data['magnify_externalid']) ? $waywire_video->data['magnify_externalid'] : '',
    '#required' => FALSE,
  );
  $form['data']['updated'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Updated'),
    '#default_value' => isset($waywire_video->data['updated']) ? $waywire_video->data['updated'] : '',
    '#required' => FALSE,
  );
  $form['data']['author_id'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Author ID'),
    '#default_value' => isset($waywire_video->data['author_id']) ? $waywire_video->data['author_id'] : '',
    '#required' => FALSE,
  );
  $form['data']['author_name'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Author Name'),
    '#default_value' => isset($waywire_video->data['author_name']) ? $waywire_video->data['author_name'] : '',
    '#required' => FALSE,
  );
  $form['data']['author_uri'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Author URI'),
    '#default_value' => isset($waywire_video->data['author_uri']) ? $waywire_video->data['author_uri'] : '',
    '#required' => FALSE,
  );
  $form['data']['content'] = array(
    '#type' => 'textarea',
    '#title' => t('Waywire Content'),
    '#default_value' => isset($waywire_video->data['content']) ? $waywire_video->data['content'] : '',
    '#required' => FALSE,
  );
  $form['data']['thumbnail'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Thumbnail'),
    '#default_value' => isset($waywire_video->data['thumbnail']) ? $waywire_video->data['thumbnail'] : '',
    '#required' => FALSE,
  );
  $form['data']['published'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Published'),
    '#default_value' => isset($waywire_video->data['published']) ? $waywire_video->data['published'] : '',
    '#required' => FALSE,
  );
  $form['data']['media_width'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Media Width'),
    '#default_value' => isset($waywire_video->data['media_width']) ? $waywire_video->data['media_width'] : '',
    '#required' => FALSE,
  );
  $form['data']['media_medium'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Media Medium'),
    '#default_value' => isset($waywire_video->data['media_medium']) ? $waywire_video->data['media_medium'] : '',
    '#required' => FALSE,
  );
  $form['data']['media_url'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Media URL'),
    '#default_value' => isset($waywire_video->data['media_url']) ? $waywire_video->data['media_url'] : '',
    '#required' => FALSE,
  );
  $form['data']['media_duration'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Media Duration'),
    '#default_value' => isset($waywire_video->data['media_duration']) ? $waywire_video->data['media_duration'] : '',
    '#required' => FALSE,
  );
  $form['data']['media_type'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Media Type'),
    '#default_value' => isset($waywire_video->data['media_type']) ? $waywire_video->data['media_type'] : '',
    '#required' => FALSE,
  );
  $form['data']['media_height'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Media Height'),
    '#default_value' => isset($waywire_video->data['media_height']) ? $waywire_video->data['media_height'] : '',
    '#required' => FALSE,
  );
  $form['data']['link_self'] = array(
    '#type' => 'textarea',
    '#title' => t('Waywire Links'),
    '#default_value' => isset($waywire_video->data['link_self']) ? $waywire_video->data['link_self'] : '',
    '#required' => FALSE,
  );
  $form['data']['link_alternative'] = array(
    '#type' => 'textarea',
    '#title' => t('Waywire Link Alternative'),
    '#default_value' => isset($waywire_video->data['link_alternative']) ? $waywire_video->data['link_alternative'] : '',
    '#required' => FALSE,
  );
  $form['data']['link_author'] = array(
    '#type' => 'textarea',
    '#title' => t('Waywire Link Author'),
    '#default_value' => isset($waywire_video->data['link_author']) ? $waywire_video->data['link_author'] : '',
    '#required' => FALSE,
  );
  $form['data']['categories'] = array(
    '#type' => 'textarea',
    '#title' => t('Waywire Categories'),
    '#default_value' => isset($waywire_video->data['categories']) ? $waywire_video->data['categories'] : '',
    '#required' => FALSE,
  );

  // Add the field related form elements.
  $form_state['waywire_video'] = $waywire_video;
  field_attach_form('waywire_video', $waywire_video, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save waywire video'),
    '#submit' => $submit + array('waywire_video_edit_form_submit'),
  );

  if (!empty($waywire_video->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete waywire video'),
      '#suffix' => l(t('Cancel'), 'admin/content/waywire_videos'),
      '#submit' => $submit + array('waywire_video_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'waywire_video_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the waywire video form.
 */
function waywire_video_edit_form_validate(&$form, &$form_state) {
  $waywire_video = $form_state['waywire_video'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('waywire_video', $waywire_video, $form, $form_state);
}

/**
 * Form API submit callback for the waywire video form.
 *
 * @todo remove hard-coded link
 */
function waywire_video_edit_form_submit(&$form, &$form_state) {

  $waywire_video = entity_ui_controller('waywire_video')->entityFormSubmitBuildEntity($form, $form_state);

  // Get the nid fromt the automcomplete text
  preg_match('/.*?\[nid:(\d+)?]/', $form['node_id']['#value'], $matches);

  if (isset($matches[1])) {
    $waywire_video->node_id = $matches[1];
  }
  // Save the waywire video and go back to the list of waywire videos

  // Add in created and changed times.
  if ($waywire_video->is_new = isset($waywire_video->is_new) ? $waywire_video->is_new : 0){
    $waywire_video->created = time();
  }

  $waywire_video->changed = time();
  $waywire_video->save();
  $form_state['redirect'] = 'admin/content/waywire_videos';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function waywire_video_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/waywire_videos/waywire_video/' . $form_state['waywire_video']->id. '/delete';
}

/**
 * Form callback: confirmation form for deleting a waywire video.
 *
 * @param object $waywire_video
 *   The waywire_video to delete
 *
 * @see confirm_form()
 */
function waywire_video_delete_form($form, &$form_state, $waywire_video) {
  $form_state['waywire_video'] = $waywire_video;

  $form['#submit'][] = 'waywire_video_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete waywire_video %name?', array('%name' => $waywire_video->name)),
    'admin/content/waywire_videos/waywire_video',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for waywire_video_delete_form
 */
function waywire_video_delete_form_submit($form, &$form_state) {
  $waywire_video = $form_state['waywire_video'];

  waywire_video_delete($waywire_video);

  drupal_set_message(t('The waywire video %name has been deleted.', array('%name' => $waywire_video->name)));
  watchdog('waywire_video', 'Deleted waywire video %name.', array('%name' => $waywire_video->name));

  $form_state['redirect'] = 'admin/content/waywire_videos';
}

/**
 * Page to add Waywire Video Entities.
 *
 * @todo Pass this through a proper theme function
 */
function waywire_video_add_page() {
  $controller = entity_ui_controller('waywire_video');
  return $controller->addPage();
}

/**
 * Displays the list of available waywire video types for waywire video
 * creation.
 *
 * @ingroup themeable
 */
function theme_waywire_video_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="waywire-video-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer waywire video types')) {
      $output = '<p>' . t('Waywire Video Entities cannot be added because you have not created any waywire video types yet. Go to the <a href="@create-waywire-video-type">waywire video type creation page</a> to add a new waywire video type.', array('@create-waywire-video-type' => url('admin/structure/waywire_video_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No waywire video types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Sets the breadcrumb for administrative waywire video pages.
 */
function waywire_video_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Models'), 'admin/content/waywire_videos'),
  );

  drupal_set_breadcrumb($breadcrumb);
}
