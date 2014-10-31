<?php
/**
 * Handles configurations needed in Leadership
 */

/**
 * Implements hook_drush_command().
 */
function leadership_drush_command() {
  $items = array();
  $items['leadership-unpublish-print-profiles'] = array(
    'description' => 'Unpublishes all print profiles',
    'drupal dependencies' => array('leadership'),
    'examples' => array(
      'unpublish nodes' => 'drush leadership-unpublish-print-profiles',
    ),
  );
  $items['leadership-add-year'] = array(
    'description' => 'Add a year (which is a taxonomy term of Leadership Session Vocabulary) to all active Leadership companies',
    'drupal dependencies' => array('leadership'),
    'arguments' => array(
      'tid1' => t('The taxonomy id of the previous year'),
      'tid2' => t('The taxonomy id of the year that needs to be added '),
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'Add 2014' => 'drush leadership-add-year 6511 6512',
      'Add 2015' => 'drush leadership-add-year 6512 6513',
    ),
  );
  $items['leadership-check-approval'] = array(
    'description' => 'Looks for data-cards & online-profiles that are unapproved, then sends email to the sales rep for that company',
    'drupal dependencies' => array('leadership'),
    'examples' => array(
      'check for unapproved' => 'drush leadership-check-approval',
    ),
  );
  
  return $items;
}

/**
 * Unpublishes all print profiles
 */
function drush_leadership_unpublish_print_profiles() {
  $query = db_select('node', 'n');
  $query->fields('n', array('nid '));
  $query->condition('n.type', 'leadership_print_profile', '=');
  $print_profiles = $query->execute();
  
  if($print_profiles) {
    foreach($print_profiles as $print_profile) {
      $node = node_load($print_profile->nid);
      $node->status = 0;
      node_save($node);
      echo "Updated {$node->title}\n";
    }
  }
}

/**
 * Finds all the companies that have $tid1 and adds $tid2 to it
 * 
 * @param int $tid1
 *  taxonomy id of a Vocab of Leadership Session @see http://www.packworld.com/admin/structure/taxonomy/leadership_session
 * @param int $tid2
 *  taxonomy id of a Vocab of Leadership Session @see http://www.packworld.com/admin/structure/taxonomy/leadership_session
 */
function drush_leadership_add_year($tid1, $tid2) {
  $query = db_select('node', 'n');
  $query->fields('n', array('nid '));
  $query->join('field_data_field_ld_session', 'f', 'n.nid = f.entity_id AND n.vid = f.revision_id');
  $query->condition('f.field_ld_session_tid', $tid1, '=');
  $companies = $query->execute();
  
  if($companies) {
    foreach($companies as $company) {      
      $node = node_load($company->nid);
      $taxonomies = $node->field_ld_session['und'];
      if(is_array($taxonomies)) {
        foreach($taxonomies as $term) {
          // If it already has the term then skip
          if($term['tid'] == $tid2){
            continue 2;
          }
        }
      }
      
      $node->field_ld_session['und'][]['tid'] = $tid2;
      // Please note that you may get this error: Invalid argument supplied for foreach() reltw.field.inc:548
      // I believe that there isn't an issue here.  @TODO figure out what is causing the error. I think
      // the problem is on the reltw side.
      node_save($node);
      echo "Updated {$node->title}\n";
    } 
  }
}

/**
 * Performs a query for all Data Cards and Online Profiles that are not approved.  Then, if the associated company has
 * a sales-rep, we email the sales rep to remind him that the company hasn't finished one of their profiles
 */
function drush_leadership_check_approval(){
  $notifications_state = variable_get('profile_notifications_state', 1);
  $default_email = variable_get('profile_default_email', 'nelson@packworld.com');
  
  if($notifications_state){
    // Get the mappings for the site field names
    $site_id = variable_get('smg_global_site');
    switch ($site_id) {
      case 'aw':
        $site_name = 'Automation World';
        $site_web = 'AutomationWorld.com';
        $leaders_type = 'LIA';
        break;
      case 'pw':
        $site_name = 'Packaging World';
        $site_web = 'Packworld.com';
        $leaders_type = 'LIP';
        break;
      case 'hcp':
        $site_name = 'Healthcare Packaging';
        $site_web = 'HealthcarePackaging.com';
        $leaders_type = 'Premier Supplier';
        break;
    }
    $mappings = leadership_site_mappings($site_id);

    // Get all the companies in the current session
    $query = db_select('node', 'node')
          ->fields('node', array('title', 'nid'))
          ->condition('node.type', 'company')
          ->condition('node.status', '1')
          ->orderBy('node.title', 'asc');
    $query->leftJoin('field_data_field_ld_session', 'session', 'node.nid = session.entity_id');
    $query->condition('session.field_ld_session_tid', variable_get('leadership_session_term'));
    $query->addField('session', 'field_ld_session_tid');
    $query->leftJoin($mappings['company_field_table'], 'companies', "companies.{$mappings['company_field_nid']} = node.nid");
    $query->condition('companies.bundle', 'user');
    $query->addField('companies', 'entity_id', 'user_id');
    $query->leftJoin($mappings['company_field_table'], 'print', "print.{$mappings['company_field_nid']} = node.nid");
    $query->condition('print.bundle', 'leadership_print_profile');
    $query->addField('print', 'entity_id', 'print_node_id');
    $query->leftJoin($mappings['company_field_table'], 'datacard', "datacard.{$mappings['company_field_nid']} = node.nid");
    $query->condition('datacard.bundle', 'leadership_data_card');
    $query->addField('datacard', 'entity_id', 'datacard_node_id');
    $query->leftJoin('node', 'datacard_node', 'datacard_node.nid = datacard.entity_id');
    $query->addField('datacard_node', 'status', 'datacard_status');
    $query->leftJoin($mappings['company_field_table'], 'online', "online.{$mappings['company_field_nid']} = node.nid");
    $query->condition('online.bundle', 'leadership_online_profile');
    $query->addField('online', 'entity_id', 'online_node_id');
    $query->leftJoin('node', 'online_node', 'online_node.nid = online.entity_id');
    $query->addField('online_node', 'status', 'online_status');
    $result = $query->execute();
  
    while($record = $result->fetchAssoc()) {
      // Load the company
      $company = node_load($record['nid']);
    
      // Look for "Sales Reps"
      $reps_string = FALSE;
      if(isset($company->field_sales_reps['und'])){
        $reps_array = array();
        foreach($company->field_sales_reps['und'] as $rep){
          $user = user_load($rep['uid']);
          $reps_array[] = $user->mail;
        }
        // Join the elements of the $reps_array into a string, to use in the to field 
        // of the email
        $reps_string = implode(", ", $reps_array);
      }

      /*if(!($record['datacard_status'] == '1') && !($record['online_status'] == '1')){
        $params['company'] = $company->title;
        if($reps_string){
          $to = $reps_string;
          //$to = 'yevgeny.ananin@gmail.com';    // uncomment for testing
          $params['message'] = 'Hello, our records show that both the Data Card and the Online Profile for ' . $company->title . ' are unapproved on packworld.com.  If possible, please encourage the company to complete its profiles, so that they can be approved.  This message will be sent out monthly.  Thank You!';
        }
        else {
          $to = $default_email;
          //$to = 'yevgeny.ananin@gmail.com';    // uncomment for testing
          $params['message'] = '(Note: this company does not have any sales reps.  Please edit the company and add sales reps, then forward this notice to them)  Hello, our records show that both the Data Card and the Online Profile for ' . $company->title . ' are unapproved on packworld.com.  If possible, please encourage the company to complete its profiles, so that they can be approved.  This message will be sent out monthly.  Thank You!';        
        }
        drush_log(($company->title . ': Datacard & Online unapproved. -------- Email sent to ' . $to), 'ok');
        drupal_mail('leadership', 'both', $to, $language, $params);
      }
      elseif(!($record['online_status']) || !($record['datacard_status'])){
        $params['company'] = $company->title;
        $unapproved = ($record['datacard_status'] != '1') ? 'Data Card' : 'Online Profile';
        $params['unapproved'] = $unapproved;
        if($reps_string){
          $to = $reps_string;
          //$to = 'yevgeny.ananin@gmail.com';        //uncomment for testing
          $params["message"] = "Hello, our records show that the " . $unapproved . " for " . $company->title . " is unapproved on packworld.com. \r\n If possible blah blah blah, please encourage the company to complete \r\n its profiles, so that they can be approved.  This message \r\n will be sent out monthly.  Thank You!";
        }
        else {
          $to = $default_email;
          //$to = 'yevgeny.ananin@gmail.com';       //uncomment for testing
          //$params['message'] = '(Note: this company does not have any sales reps.  Please edit the company and add sales reps, then forward this notice to them)  Hello, our records show that the ' . $unapproved . ' for ' . $company->title . ' is unapproved on packworld.com.  If possible, please encourage the company to complete its profiles, so that they can be approved.  This message will be sent out monthly.  Thank You!';
          $params["message"] = "Hello, our records show that the " . $unapproved . " for " . $company->title . " is unapproved on packworld.com. \r\n If possible blah blah blah, please encourage the company to complete \r\n its profiles, so that they can be approved.  This message \r\n will be sent out monthly.  Thank You!";
        }
        drush_log(($company->title . ': ' . $unapproved . ' unapproved. -------- Email sent to ' . $to), 'ok');
        drupal_mail('leadership', 'single', $to, $language, $params);
      }*/
      
      if(!($record['datacard_status'])){
        $params['company'] = $company->title;
        $params['headers'] = array(
          'Bcc' => 'yananin@summitmediagroup.com',
        );
        $params['leaders_type'] = $leaders_type;
        $params['message'] = "Hello, \r\n" . $company->title . "'s data card is not showing on " . $site_web . " because they still have not filled out their data card.  This means: \r\n\r\n- they are not receiving ANY exposure within their chosen categories \r\n- they are not receiving ANY clicks on their data card \r\n- they are not receiving any traffic to their online profile from the data card";
        if(!($record['online_status'])){
          $params['message'] .= "\r\n\r\nFurther, they have not filled out their online profile, which means they are losing a valuable opportunity to introduce their company to prospects who drill all the way down to their company on " . $site_web . ".";
        }
        if($reps_string){
          $to = $reps_string;
          //$to = 'yananin@summitmediagroup.com, ajankowski@summitmediagroup.com';
        }
        else {
          $to = $default_email;
          $params['message'] .= "\r\n\r\nTHERE IS NO REP ASSIGNED TO THIS COMPANY, PLEASE ADD ASAP";
        }

        drush_log('Company: ' . $company->title . ' ----- Email sent to ' . $to, 'ok');
        drupal_mail('leadership', 'profile_unapproved', $to, $language, $params);
      }
    }
  }
}

/**
 * Implements hook_mail()
 */
function leadership_mail($key, &$message, $params){
  switch($key){
    case 'both':
      $langcode = $message['language']->language;
      $message['subject'] = $params['company'] . ': Data Card & Online Profile still unapproved';
      $message['body'][] = $params['message'];
      $message['headers'] += $params['headers'];
      break;
    case 'single':
      $langcode = $message['language']->language;
      $message['subject'] = $params['company'] . ': ' . $params['unapproved'] . ' still unapproved';
      $message['body'][] = $params['message'];
      $message['headers'] += $params['headers'];
      break;
    case 'profile_unapproved':
      $langcode = $message['language']->language;
      $message['subject'] = $params['leaders_type'] . ' Alert: ' . $params['company'] . ' data card not showing up';
      $message['body'][] = $params['message'];
      $message['headers'] += $params['headers'];
      break;
  }
}