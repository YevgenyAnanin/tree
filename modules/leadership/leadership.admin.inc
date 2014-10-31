<?php
/**
 * @file
 *  Handles Leadership admin items
 */

/**
 * The dashboard page that lists all the companies in the current session.
 */
function leadership_dashboard() {
  $rows = array();
  $mapping = leadership_site_mappings(variable_get('smg_global_site'));

  $header = array(
    'Company Name',
    'User Page',
    'Print Profile',
    'Data Card',
    'Online Profile',
  );

  // Get all company profiles in current session
  $query = db_select('node', 'node')
        ->fields('node', array('title', 'nid'))
        ->condition('type', 'company')
        ->condition('status', '1')
        ->orderBy('title', 'asc');
  $query->leftJoin('field_data_field_ld_session', 'session', 'node.nid = session.entity_id');
  $query->condition('session.field_ld_session_tid', variable_get('leadership_session_term'));
  $query->addField('session', 'field_ld_session_tid');
  $query->leftJoin($mapping['company_field_table'], 'companies', "companies.{$mapping['company_field_nid']} = node.nid");
  $query->condition('companies.bundle', 'user');
  $query->addField('companies', 'entity_id', 'user_id');
  $query->leftJoin($mapping['company_field_table'], 'print', "print.{$mapping['company_field_nid']} = node.nid");
  $query->condition('print.bundle', 'leadership_print_profile');
  $query->addField('print', 'entity_id', 'print_node_id');
  $query->leftJoin($mapping['company_field_table'], 'datacard', "datacard.{$mapping['company_field_nid']} = node.nid");
  $query->condition('datacard.bundle', 'leadership_data_card');
  $query->addField('datacard', 'entity_id', 'datacard_node_id');
  $query->leftJoin($mapping['company_field_table'], 'online', "online.{$mapping['company_field_nid']} = node.nid");
  $query->condition('online.bundle', 'leadership_online_profile');
  $query->addField('online', 'entity_id', 'online_node_id');
  $result = $query->execute();

  while ($record = $result->fetchAssoc()) {
    // Get User info
    $user = user_load($record['user_id']);

    // Print Profile Info
	  $print_profile = node_load($record['print_node_id']);
	  // Only show this if it is approved
	  if ($print_profile->status) {
	    $print_menu = l('XML', 'admin/leadership/xml/' . $record['print_node_id']);
	    if(isset($print_profile->field_ld_print_logo['und'][0]['uri'])) {
	      $print_menu .= ' | ' . l('Logo', file_create_url($print_profile->field_ld_print_logo['und'][0]['uri']));
	    }
	    if(isset($print_profile->field_ld_print_photo_1['und'][0]['uri'])) {
	      $print_menu .= ' | ' . l('Photo 1', file_create_url($print_profile->field_ld_print_photo_1['und'][0]['uri']));
      }
      if(isset($print_profile->field_ld_print_photo_2['und'][0]['uri'])) {
	      $print_menu .= ' | ' . l('Photo 2', file_create_url($print_profile->field_ld_print_photo_2['und'][0]['uri']));
	    }
	    $print_menu .= ' ';
	  }
	  else {
	    $print_menu = '';
	  }
	  $print_profile_percent = content_complete_get_data($print_profile);

	  // Datacard Profile Info
  	$datacard_profile = node_load($record['datacard_node_id']);
  	$datacard_percent = content_complete_get_data($datacard_profile);

    // Online Profile Info
	  $online_profile = node_load($record['online_node_id']);
	  $online_profile_percent = content_complete_get_data($online_profile);

    $rows[] = array(
      l($record['title'], 'node/' . $record['nid'], array('attributes' => array('target' => '_blank'))),
      l($user->name . ' Profile', 'leaders/user/' . $record['user_id'], array('attributes' => array('target' => '_blank'))),
      $print_menu .  $print_profile_percent['percent'] . '% Complete - ' . leadership_node_status_string($print_profile->status) .  ' ' . l('Edit', 'node/' . ($record['print_node_id']) . '/edit', array('attributes' => array('target' => '_blank'))),
  	  $datacard_percent['percent'] . '% Complete - ' . leadership_node_status_string($datacard_profile->status) .  ' ' . l('Edit', 'node/' . ($record['datacard_node_id']) . '/edit', array('attributes' => array('target' => '_blank'))) .  ' ' .  l('View', 'node/' . ($record['datacard_node_id']), array('attributes' => array('target' => '_blank'))),
  	  $online_profile_percent['percent'] . '% Complete - ' . leadership_node_status_string($online_profile->status) .  ' ' . l('Edit', 'node/' . ($record['online_node_id']) . '/edit', array('attributes' => array('target' => '_blank'))) .  ' ' . l('View', 'node/' . ($record['online_node_id']), array('attributes' => array('target' => '_blank')))
    );
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}

/**
 * Lists all the companies and also ability to add a company
 */
function leadership_companies() {
  $rows = array();

  $output = '<h1 class="ld-form-title">Add Companies To Current Session</h1>';
  $output .= drupal_render(drupal_get_form('leadership_admin_company_form'));

  $header = array(
    'Company Name',
    'Remove From Session',
  );

  $companies = leadership_get_all_companies_in_current_session();

  foreach ($companies as $company) {
    $rows[] = array(
      $company->title,
      l('Remove From Current Session', 'leaders/' . $company->nid . '/remove/session'),
    );
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}

/**
 * Form to add a company to leadership
 */
function leadership_admin_company_form() {
  if (variable_get('smg_global_site') == 'aw') {
    $company_field_name = 'field_ld_company';
  }
  else {
    $company_field_name = 'field_companies';
  }

  $form['company'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Company'),
    '#autocomplete_path' => "node_reference/autocomplete/user/user/$company_field_name",
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Company To Current Session'),
  );

  return $form;
}

/**
 * Submit handler for the admin company form
 */
function leadership_admin_company_form_submit($form, &$form_state) {
  $term_already_set = FALSE;

  // Get the nid fromt the automcomplete text
  preg_match('/.*?\[nid:(\d+)?]/', $form['company']['#value'], $matches);

  if (isset($matches[1])) {
    $node = node_load($matches[1]);
    if (isset($node->field_ld_session['und'])) {
      foreach ($node->field_ld_session['und'] as $terms) {
        if ($terms['tid'] == variable_get('leadership_session_term')) {
          $term_already_set = TRUE;
        }
      }
    }

    // If it hasn't been set then set it
    if (!$term_already_set) {
      $node->field_ld_session['und'][] = array('tid' => variable_get('leadership_session_term'));
      node_save($node);
      // Create all three nodes
      leadership_create_node($node->nid, $node->title, 'leadership_print_profile');
      leadership_create_node($node->nid, $node->title, 'leadership_online_profile');
      leadership_create_node($node->nid, $node->title, 'leadership_data_card');
    }

    // If PW got to add main categories form
    if (variable_get('smg_global_site') == 'pw') {
      drupal_goto('admin/leadership/add/main-categories/' . $matches[1]);
    }
  }
}

/**
 * Callback function to remove the current session taxonomy term from the company
 *
 * @param $nid
 *  the company node id
 *
 * @return
 *  after it saves the node it goes back to the Company Page
 */
function leadership_companies_remove_from_current_session($nid) {

  $node = node_load($nid);

  foreach ($node->field_ld_session['und'] as $key => $terms) {
    if ($terms['tid'] == variable_get('leadership_session_term')) {
      unset($node->field_ld_session['und'][$key]);
      node_save($node);
    }
  }

  drupal_goto('admin/leadership/companies');
}

/**
 * Lists all the users that are associated to a company
 */
function leadership_user_config() {
  $rows = array();

  $output = drupal_render(drupal_get_form('leadership_user_config_form'));

  $header = array(
    'Username',
    'Email',
    'Company',
  );

  $users = leadership_get_current_users(variable_get('leadership_role_name'));
  if ($users) {
    foreach ($users as $user) {
      $rows[] = array(
        l($user['name'], 'user/' . $user['uid'] . '/edit'),
        $user['email'],
        $user['company']
      );
    }
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}

/**
 * Function for adding a node and set the the company node reference
 *
 * @param $nid
 *  the node nid
 * @param $title
 *  the node title
 * @param $content_type
 *  the node content type
 */
function leadership_create_node($nid, $title, $content_type) {
  $mapping = leadership_site_mappings(variable_get('smg_global_site'));

  // Check to see if a node exists already
  $query = db_select($mapping['company_field_table'], 'companies')
        ->fields('companies', array('entity_id'))
        ->condition('companies.bundle', $content_type)
        ->condition('companies.' . $mapping['company_field_nid'], $nid);
  $result = $query->execute();

  while ($record = $result->fetchAssoc()) {
	  $nodes[] = $record;
	}

	// Only add a node if there isn't a node for this already
	if (!isset($nodes)) {

	  $node = new StdClass();
	  $node->type = $content_type;
	  $node->status = 0;
	  // Setting the title to the company name by default
	  $node->title = $title;
	  $node->{$mapping['company_field']}['und'][0]['nid'] = $nid;
	  $node->language = 'und';
	  $node->body = '';

	  // Setting these to an empty array to prevent warnings
	  if ($content_type == 'leadership_print_profile') {
	    $node->field_ld_address_1 = array();;
	    $node->field_ld_address_2 = array();
	    $node->field_ld_address_city = array();
	    $node->field_ld_categories_technologies = array();
	    $node->field_ld_categories_topics = array();
	    $node->field_ld_country = array();
	    $node->field_ld_email = array();
	    $node->field_ld_fax = array();
	    $node->field_ld_phone = array();
	    $node->field_ld_print_logo = array();
	    $node->field_ld_print_photo_1 = array();
	    $node->field_ld_print_photo_2 = array();
	    $node->field_ld_state = array();
	    $node->field_ld_website = array();
	    $node->field_ld_zip_postal_code = array();
	  }
	  if ($content_type == 'leadership_online_profile') {
	    $node->field_ld_employees = array();
	    $node->field_ld_facebook = array();
	    $node->field_ld_geo_distrib = array();
	    $node->field_ld_google_plus = array();
	    $node->field_ld_linkedin = array();
	    $node->field_ld_logo = array();
	    $node->field_ld_pinterest = array();
	    $node->field_ld_sales = array();
	    $node->field_ld_services = array();
	    $node->field_ld_training = array();
	    $node->field_ld_twitter = array();
	    $node->field_ld_years = array();
	    $node->field_ld_youtube = array();
	  }
	  node_object_prepare($node);
	  node_save($node);
	}
}

/**
 * Form that handles Adding/editing Users
 */
function leadership_user_config_form() {
  if (variable_get('smg_global_site') == 'aw') {
    $company_field_name = 'field_ld_company';
  }
  else {
    $company_field_name = 'field_companies';
  }

  $form['name'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Username'),
    '#size'     => 60,
    '#required' => TRUE,
  );
  $form['full_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Name'),
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#size' => 60,
    '#required' => TRUE,
  );
  $form['company'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Company'),
    //@TODO - use custom auto complete that only allows current companies
    '#autocomplete_path' => "node_reference/autocomplete/user/user/$company_field_name",
    '#description' => t('Only choose companies from current session'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add User'),
  );

  return $form;
}

/**
 *  Implements hook_form_validate().
 */
function leadership_user_config_form_validate($form, &$form_state) {
  // Validate new or changing username.
  if (isset($form_state['values']['name'])) {
    if ($error = user_validate_name($form_state['values']['name'])) {
      form_set_error('name', $error);
    }
    elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('name', db_like($form_state['values']['name']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
      form_set_error('name', t('The name %name is already taken.', array('%name' => $form_state['values']['name'])));
    }
  }

  // Validate the e-mail address, and check if it is taken by an existing user.
  if ($error = user_validate_mail($form_state['values']['email'])) {
    form_set_error('mail', $error);
  }
  elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('mail', db_like($form_state['values']['email']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
    // Format error message dependent on whether the user is logged in or not.
    if ($GLOBALS['user']->uid) {
      form_set_error('email', t('The e-mail address %email is already taken.', array('%email' => $form_state['values']['email'])));
    }
    else {
      form_set_error('mail', t('The e-mail address %email is already registered. <a href="@password">Have you forgotten your password?</a>', array('%email' => $form_state['values']['mail'], '@password' => url('user/password'))));
    }
  }
}

/**
 * Implements hook_form_submit()
 */
function leadership_user_config_form_submit($form, &$form_state) {
  $mapping = leadership_site_mappings(variable_get('smg_global_site'));

  // Get the nid fromt the automcomplete text
  preg_match('/.*?\[nid:(\d+)?]/', $form['company']['#value'], $matches);

  $user_roles = user_roles();

  if (isset($matches[1])) {
    $edit = array(
      'name' => $form['name']['#value'],
      'pass' => $form['pass']['#value'],
      'mail' => $form['email']['#value'],
      'status' => 1,
      'language' => 'en',
      'roles' => array(variable_get('leadership_role_name') => $user_roles[variable_get('leadership_role_name')]),
      $mapping['company_field'] => array(
        'und' => array(
          0 => array(
            'nid' => $matches[1]
          ),
        ),
      ),
      'field_full_name' => array(
        'und' => array(
          0 => array(
            'value' => $form['full_name']['#value']
           ),
         ),
       ),
    );

    $user = user_save(FALSE, $edit);

    if (!$user) {
      drupal_set_message(t('Failed to save user!'), 'status');
    }
    else {
      drupal_set_message(t('@user has been added!', array('@user' => $form['name']['#value'])), 'status');

      // Associate the user with the three profiles associated with that company
      $query = db_select('node', 'node')
            ->fields('node', array('nid', 'type', 'title'))
            ->condition('status', '1');
      $query->join($mapping['company_field_table'], 'companies', 'node.nid = companies.' . $mapping['company_field_nid']);
      $query->condition('companies.bundle',  array('leadership_data_card', 'leadership_online_profile', 'leadership_print_profile'),'IN');
      $query->addField('companies', $mapping['company_field_nid']);
      $query->addField('companies', 'bundle');
      $query->addField('companies', 'entity_id');
      $query->condition('companies.' . $mapping['company_field_nid'], $matches[1]);
      $result = $query->execute();

      while ($record = $result->fetchAssoc()) {
        // Update node with the new user
        $node = node_load($record['entity_id']);
        $node->uid = $user->uid;
        node_save($node);
        watchdog('action', 'Changed owner of @type %title to uid %name.', array('@type' => node_type_get_name($node), '%title' => $node->title, '%name' => $user->name));
    	}
    }
  }
  else {
    drupal_set_message(t('Problem with Company!'), 'status');
  }
}

/**
 * Configuration of leadership
 */
function leadership_configure_form() {
  $site_id = variable_get('smg_global_site');

  $vocabs_raw = taxonomy_get_vocabularies();
  foreach ($vocabs_raw as $vid => $vocab) {
    $vocabs[$vid] = check_plain($vocab->name);
  }

  // *** Admin User
  $form['admin_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Admin User'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['admin_user']['leadership_admin_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin User Full Name'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#default_value' => variable_get('leadership_admin_user_name'),
  );
  $form['admin_user']['leadership_admin_user_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin User Email Address'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#default_value' => variable_get('leadership_admin_user_email'),
  );
  // *** Display
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['display']['leadership_block_logo'] = array(
    '#type' => 'textfield',
    '#title' => t('All Companies Block Image'),
    '#description' => t('Path to the Image of the Leadership block'),
    '#default_value' => variable_get('leadership_block_logo'),
  );

  // *** Sessions
  $form['sessions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sessions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['sessions']['leadership_session_vocab'] = array(
    '#type' => 'select',
    '#title' => t('Session vocabulary'),
    '#description'    => t('Choose which vocabulary should be used as the Sessions.'),
    '#options'        => $vocabs,
    '#default_value'  => variable_get('leadership_session_vocab'),
  );
  // Only display the taxonomy terms if Session was selected
  if (variable_get('leadership_session_vocab')) {
    $taxonomies = taxonomy_get_tree(variable_get('leadership_session_vocab'));
    foreach($taxonomies as $term) {
      $terms[$term->tid] = $term->name;
    }

    $form['sessions']['leadership_session_term'] = array(
      '#type'           => 'select',
      '#title'          => t('Session Year'),
      '#description'    => t('Choose which year is the current session.'),
      '#options'        => $terms,
      '#default_value'  => variable_get('leadership_session_term'),
    );
  }
  $form['sessions']['leadership_print_due_date'] = array(
    '#type' =>'textfield',
    '#title' => t('Date that Leadership Print Profile is Due'),
    '#description' => t('Use this format MM/DD/YYYY EX: 12/30/2014'),
    '#default_value' => variable_get('leadership_print_due_date'),
  );
  $form['sessions']['leadership_datacard_due_date'] = array(
    '#type' =>'textfield',
    '#title' => t('Date that Leadership Data Card Profile is Due'),
    '#description' => t('Use this format MM/DD/YYYY EX: 12/30/2014'),
    '#default_value' => variable_get('leadership_datacard_due_date'),
  );
  $form['sessions']['leadership_online_due_date'] = array(
    '#type' =>'textfield',
    '#title' => t('Date that Leadership Online Profile is Due'),
    '#description' => t('Use this format MM/DD/YYYY EX: 12/30/2014'),
    '#default_value' => variable_get('leadership_online_due_date'),
  );

  if ($site_id == 'pw') {
    $form['admin_categories'] = array(
      '#type'         => 'fieldset',
      '#title'        => t('Admin Categories'),
      '#collapsible'  => TRUE,
      '#collapsed'    => FALSE,
    );
    // Which vocab will admin configure
    $form['admin_categories']['leadership_admin_category_vocab'] = array(
      '#type'           => 'select',
      '#title'          => t('Admin Leadership Category'),
      '#description'    => t('Choose which vocabulary will the leadership admin need to configure. This will be only used by admins.'),
      '#options'        => $vocabs,
      '#default_value'  => variable_get('leadership_admin_category_vocab'),
    );
    if(variable_get('leadership_admin_category_vocab')) {

      $taxonomies = taxonomy_get_tree(variable_get('leadership_admin_category_vocab'));
      foreach($taxonomies as $term) {
        $admin_category_terms[$term->tid] = $term->name;
      }

      $form['admin_categories']['leadership_admin_category_machines'] = array(
        '#type'           => 'select',
        '#title'          => t('Machines Term'),
        '#description'    => t('Choose which term fits the Machines category.'),
        '#options'        => $admin_category_terms,
        '#default_value'  => variable_get('leadership_admin_category_machines'),
      );
      $form['admin_categories']['leadership_admin_category_materials'] = array(
        '#type'           => 'select',
        '#title'          => t('Materials Term'),
        '#description'    => t('Choose which term fits the Materials category.'),
        '#options'        => $admin_category_terms,
        '#default_value'  => variable_get('leadership_admin_category_materials'),
      );
      $form['admin_categories']['leadership_admin_category_services'] = array(
        '#type'           => 'select',
        '#title'          => t('Services Term'),
        '#description'    => t('Choose which term fits the Service category.'),
        '#options'        => $admin_category_terms,
        '#default_value'  => variable_get('leadership_admin_category_services'),
      );
    }
  }

  // Which vocab will user use
  $form['user_categories'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('User Categories'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );
  $form['user_categories']['leadership_user_category_vocab'] = array(
    '#type'           => 'select',
    '#title'          => t('User Leadership Category'),
    '#description'    => t('Choose which vocabulary will the leadership user use.'),
    '#options'        => $vocabs,
    '#default_value'  => variable_get('leadership_user_category_vocab'),
    '#multiple'       => $site_id == 'aw' ? TRUE : FALSE,
  );
  if(variable_get('leadership_user_category_vocab') && $site_id == 'pw') {

    $taxonomies = taxonomy_get_tree(variable_get('leadership_user_category_vocab'));
    foreach($taxonomies as $term) {
      $user_category_terms[$term->tid] = $term->name;
    }

    $form['user_categories']['leadership_user_machinery'] = array(
      '#type'           => 'select',
      '#title'          => t('Machinery Term'),
      '#description'    => t('Choose which term fits the Machines category.'),
      '#options'        => $user_category_terms,
      '#default_value'  => variable_get('leadership_user_machinery'),
    );
    $form['user_categories']['leadership_user_materials'] = array(
      '#type'           => 'select',
      '#title'          => t('Materials Term'),
      '#description'    => t('Choose which term fits the Materials category.'),
      '#options'        => $user_category_terms,
      '#default_value'  => variable_get('leadership_user_materials'),
    );
    $form['user_categories']['leadership_user_service'] = array(
      '#type'           => 'select',
      '#title'          => t('Services Term'),
      '#description'    => t('Choose which term fits the Service category.'),
      '#options'        => $user_category_terms,
      '#default_value'  => variable_get('leadership_user_service'),
    );
  }

  // *** Roles
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['roles']['leadership_role_name'] = array(
    '#type' => 'select',
    '#title' => t('User Role'),
    '#description' => t('Choose which Role will the user be.'),
    '#options'        => user_roles(),
    '#default_value'  => variable_get('leadership_role_name'),
  );

  // *** Notifications
  $form['profile_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Profile Notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['profile_notifications']['profile_notifications_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Notifications'),
    '#description' => t('If this setting is enabled, then on the first of the month, the system will look for companies that have an unapproved data-card and/or an unapproved online-profile.  An email notification will be sent to any sales representative that is associated with that company.'),
    '#default_value' => variable_get('profile_notifications_state'),
  );
  $form['profile_notifications']['profile_default_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Email'),
    '#description' => t('Please provide an email address to be used in the cases where a company doesn\'t have any associated sales reps.'),
    '#default_value' => variable_get('profile_default_email'),
  );

  // *** Youtube Amplify
  $form['youtube_amplify'] = array(
    '#type' => 'fieldset',
    '#title' => t('Youtube Amplify'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['youtube_amplify']['youtube_amplify_admin_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Youtube Amplify Admin Email'),
    '#description' => t('Please provide an email address to the person that will be configuring videos in Waywire.'),
    '#default_value' => variable_get('youtube_amplify_admin_email'),
  );

  return system_settings_form($form);
}

/**
 * Gets all the companies in the current session
 *
 * @return array
 *  companies in session
 */
function leadership_get_all_companies_in_current_session() {
  // Get all company names in current session
  $query = db_select('node', 'node')
        ->fields('node', array('title', 'nid'))
        ->condition('type', 'company')
        ->condition('status', '1')
        ->orderBy('title', 'asc');
  $query->leftJoin('field_data_field_ld_session', 'session', 'node.nid = session.entity_id');
  $query->condition('session.field_ld_session_tid', variable_get('leadership_session_term'));
  $query->addField('session', 'field_ld_session_tid');
  $query->distinct();

  return $query->execute()->fetchAll();
}

/**
 * Export a company profile in xml format
 *
 * @param $nid
 *  the node id
 *
 * @return $xml
 *  sends an XML document directly, no HTML output
 */
function leadership_get_xml($nid) {
  $tids = array();
  $terms = array();

  $profile = node_load($nid);

  // Get Taxonomy tids and names
  if (isset($profile->field_ld_categories_technologies['und'][0])) {
    foreach ($profile->field_ld_categories_technologies['und'] as $value) {
      $tids[] = $value['tid'];
    }
  }
  if (isset($profile->field_ld_categories_topics['und'][0])) {
    foreach ($profile->field_ld_categories_topics['und'] as $value) {
      $tids[] = $value['tid'];
    }
  }
  foreach ($tids as $tid) {
    $terms[] = taxonomy_term_load($tid)->name;
  }

  // Prepare all the data for export
  // NOTE: All variable names are carefully crafted to match -- INCLUDING
  //  CASE -- the specific name of the tags in XML file.
  $Sub_Category = check_plain(implode("\n", $terms));
  $category_tids = implode(', ', $tids);
  $company_nid = $profile->field_ld_company['und'][0]['nid'];

  $Company = check_plain($profile->title);
  $Address_1 = check_plain($profile->field_ld_address_1['und'][0]['safe_value']);
  $Address_2 = check_plain($profile->field_ld_address_2['und'][0]['safe_value']);
  $City = check_plain($profile->field_ld_address_city['und'][0]['safe_value']);
  $State = check_plain($profile->field_ld_state['und'][0]['value']);
  $Postal_Zipcode = check_plain($profile->field_ld_zip_postal_code['und'][0]['safe_value']);
  $Country = check_plain($profile->field_ld_country['und'][0]['value']);

  // IF USA than just making it blank
  $Country = ($Country == 'United States') ? '' : $Country;

  $Phone = leadership_phone_number_output($profile->field_ld_phone['und'][0]['safe_value']);
  $Fax = leadership_phone_number_output($profile->field_ld_fax['und'][0]['safe_value']);
  $Email = $profile->field_ld_email['und'][0]['safe_value'];
  $Company_Profile = check_plain($profile->body['und'][0]['safe_value']);

  $Website = $profile->field_ld_website['und'][0]['value'];
  // If there's no protocol portion, $Website is unmodified, otherwise the
  // protocol portion is stripped from URLs
  $Website = ($pos = strpos($Website, '//')) !== FALSE ? substr($Website, $pos + 2) : $Website;

  // Get corresponding company path since that is where the leadership profile will live
  $company = node_load($company_nid);
  $Supplier_Link = url('node/' . $company->nid, array('absolute' => TRUE));
  $Supplier_Link = ($pos = strpos($Supplier_Link, '//')) !== FALSE ? substr($Supplier_Link, $pos + 2) : $Supplier_Link;
  $session_tid = variable_get('leadership_session_term');

  // Build the actual file string
  $xml = <<<EOF
<?xml version="1.0" encoding="UTF-8" ?>
<Root>
  <Ad>
    <Category>$Sub_Category</Category>
    <Sub_Category></Sub_Category>
    <Company>$Company</Company>
    <Address_1>$Address_1</Address_1>
    <Address_2>$Address_2</Address_2>
    <City>$City</City>
    <State_Province>$State</State_Province>
    <Postal_Zipcode>$Postal_Zipcode</Postal_Zipcode>
    <Country>$Country</Country>
    <Phone>$Phone</Phone>
    <Fax>$Fax</Fax>
    <Email>$Email</Email>
    <Website>$Website</Website>
    <Company_Profile>$Company_Profile</Company_Profile>
    <Company2>$Company</Company2>
    <Supplier_Link>$Supplier_Link</Supplier_Link>

    <profile_nid>$profile->nid</profile_nid>
    <category_tids>$category_tids</category_tids>
    <session_tid>$session_tid</session_tid>
    <company_nid>$company_nid</company_nid>
  </Ad>
</Root>
EOF;

  $file = file_save_data($xml);
  if ($file) {
    file_transfer($file->uri, array('Content-Type' => 'text/xml', 'Content-Disposition' => 'attachment', 'filename' => $Company . '.xml'));
  }
}

/**
 * Function for basically taking the status of a node and returning string Approve or Unapproved
 *
 * @param $status
 *   the status of a node
 *
 * @return $status
 *   the string status of a node. Approved or Unapproved
 */
function leadership_node_status_string($status) {

  if ($status == 0) {
    $status = 'Unapproved';
  }
  if ($status == 1) {
    $status = 'Approved';
  }

  return $status;
}

/**
 * Function for returning the leadership formatted phone number
 */
function leadership_phone_number_output($phone_number) {

	$numbers_only = preg_replace('/[^\d]+/', '', $phone_number);

	$string_length = strlen($numbers_only);

	//Only US/Canada numbers will have this styling
	if($string_length == 10) {

		$areacode = substr($numbers_only, 0, 3);
		$prefix   = substr($numbers_only, 3, 3);
		$number   = substr($numbers_only, 6, 4);

		$phone_number = $areacode . '/' . $prefix  . '-' . $number;

	}
	if($string_length == 11 && substr($numbers_only, 0, 1) == 1) {

	  $areacode = substr($numbers_only, 1, 3);
		$prefix   = substr($numbers_only, 4, 3);
		$number   = substr($numbers_only, 7, 4);

	  $phone_number = $areacode . '/' . $prefix  . '-' . $number;

	}

	return $phone_number;
}

/**
 * Function for getting the current user with a certain role
 *
 * @param $role
 *  the role id
 *
 * @return $users
 *  array of user data
 *  - name - the users name
 *  - email - the users email
 *  - company - the company name that the user is associated with
 *  - uid - the user id
 */
function leadership_get_current_users($role = FALSE) {
  $mapping = leadership_site_mappings(variable_get('smg_global_site'));
  $users = array();

  // Get all the users with administer_leadership
  $query = db_select('users', 'user')
    ->fields('user', array('mail', 'name', 'uid'))
    ->condition('status', 1);
  $query->leftJoin($mapping['company_field_table'], 'company', 'user.uid = company.entity_id');
  $query->addField('company', $mapping['company_field_nid']);
  $query->condition('company.bundle', 'user');
  $query->leftJoin('users_roles ', 'role', 'role.uid = user.uid');
  if ($role) {
    $query->condition('role.rid', $role);
  }

  $result = $query->execute();

  while ($record = $result->fetchAssoc()) {
    $user['name'] = $record['name'];
	  $user['email'] = $record['mail'];
	  $user['company'] = node_load($record[$mapping['company_field_nid']])->title;
	  $user['uid'] = $record['uid'];
	  $users[] = $user;
	}

  return $users;
}

/**
 * Render the form to add categories to one company.
 *
 * @param integer $company_nid
 *   The Company node id.
 * @return string
 *   Rendered form.
 */
function leadership_add_categories_to_one_company($company_nid) {
  return drupal_render(drupal_get_form('leadership_add_categories_to_one_company_form', $company_nid));
}

/**
 * Form to add a main categories to the company.
 *
 * @param object $form
 *   the form object.
 * @param object $form_state
 *   the from state object.
 * @param integer $company_nid
 *   The node id of the company.
 */
function leadership_add_categories_to_one_company_form($form, &$form_state, $company_nid) {
  $taxonomies = taxonomy_get_tree(variable_get('leadership_admin_category_vocab'));
  foreach($taxonomies as $term) {
    $admin_category_terms[$term->tid] = $term->name;
  }

  $form['categories'] = array(
    '#type'           => 'checkboxes',
    '#title'          => t('Main Leadership Categories'),
    '#description'    => t('Choose which categories this company belongs to. You can do it later but please make sure it must be set before the customer starts filling out the form.'),
    '#options'        => $admin_category_terms,
    '#multiple'       => TRUE,
  );
  $form['nid'] = array(
    '#type' => 'hidden',
    '#default_value' => $company_nid,
  );
  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Submit'),
  );

  return $form;
}

/**
 * Leadership one company categories submit handler.
 *
 * @param object $form
 *   the form object.
 * @param object $form_state
 *   the from state object.
 */
function leadership_add_categories_to_one_company_form_submit($form, &$form_state) {
  if ($form_state['values']['nid']) {
    $node = node_load($form_state['values']['nid']);
    // Just in case there are some terms already set.
    if (isset($node->field_ld_main_categories['und'])) {
      unset($node->field_ld_main_categories['und']);
    }
    // Add taxonomy terms.
    foreach ($form_state['values']['categories'] as $tid) {
      if ($tid) {
        $node->field_ld_main_categories['und'][] = array('tid' => $tid);
      }
    }

    node_save($node);
    drupal_set_message(t('Cagetories for %company_name have been added. Now add user to the company!', array('%company_name' => $node->title)));
    drupal_goto('admin/leadership/user');
  }
}

/**
 * Function that return the form for adding companies to one of the main
 * categories.
 *
 * @param integer $category
 *   The taxonomy term id of the main leadership category.
 */
function leadership_all_companies_category_form($form, &$form_state, $category) {
  // Get all company names in current session
  $query = db_select('node', 'node')
        ->fields('node', array('title', 'nid'))
        ->condition('type', 'company')
        ->condition('status', '1')
        ->orderBy('title', 'asc');
  $query->leftJoin('field_data_field_ld_session', 'session', 'node.nid = session.entity_id');
  $query->condition('session.field_ld_session_tid', variable_get('leadership_session_term'));
  $query->addField('session', 'field_ld_session_tid');
  $query->leftJoin('field_data_field_ld_main_categories', 'main_categories', 'main_categories.entity_id = node.nid');
  $query->addField('main_categories', 'field_ld_main_categories_tid');
  $query->addField('main_categories', 'entity_id');
  $query->distinct();
  $result = $query->execute();

  while ($record = $result->fetchAssoc()) {
    $company_options[$record['nid']] = array(
      'company_name' => l($record['title'], '/node/' . $record['nid'], array('attributes' => array('target' => '_blank'))),
    );

    // Check to make there aren't any companies already set to this year.
    // Default check these companies
    if ($record['field_ld_main_categories_tid'] == $category) {
      $default_checked[$record['nid']] = 1;
    }
  }

  $header = array(
   'company_name' => t('Company Name'),
  );

  $form['companies'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $company_options,
    '#js_select' => FALSE,
    '#default_value' => $default_checked,
  );
  $form['category_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $category,
  );

  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Submit'),
  );

  return $form;
}

/**
 * Submition handler for the leadership_all_companies_category_form.
 */
function leadership_all_companies_category_form_submit($form, &$form_state) {
  $category_tid = $form_state['values']['category_id'];
  foreach ($form_state['values']['companies'] as $key => $value) {
    $term_already_set = FALSE;
    $node = node_load($key);

    // Going through all checked values. If checked and not already saved then
    // resave node. If already saved than do not save node.
    if ($value) {
      // Check to see if the term is set
      if (!isset($node->field_ld_main_categories['und'])) {
        foreach ($node->field_ld_main_categories['und'] as $terms) {
          if ($terms['tid'] == $category_tid) {
            $term_already_set = TRUE;
          }
        }
      }

      // If it hasn't been set than set it.
      if (!$term_already_set) {
        $node->field_ld_main_categories['und'][] = array('tid' => $category_tid);
        node_save($node);
      }
    }

    // Going through all the not checked values. Just in case the user unchecked
    //  a company.
    if (!$value && isset($node->field_ld_main_categories['und'])) {
      foreach ($node->field_ld_main_categories['und'] as $key => $terms) {
        if ($terms['tid'] == $category_tid) {
          unset($node->field_ld_main_categories['und'][$key]);
          node_save($node);
        }
      }
    }
  }

  drupal_set_message(t('Category has been updated'));
  drupal_goto('admin/leadership');
}
