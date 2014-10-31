<?php
/**
 * @file
 * Functions related to a leadership user.
 */

/**
 * Function for directing the user back to the Leadership page.
 */
function leadership_user_redirect($user) {
  drupal_goto('leaders/user/' . $user->uid);
}

/**
 * User Dashboard page that allows the user to view the status of all their
 * profiles.
 *
 * @param integer $uid
 *   The user id of the user.
 *
 * @return string $content
 *  the html content
 */
function leadership_user_dashboard($uid) {
  drupal_add_css(drupal_get_path('module', 'leadership') . '/css/leadership_user_dashboard.css');
  $site_id = variable_get('smg_global_site');
  switch ($site_id) {
    case 'pw':
      $title = 'Leaders in Packaging';
      $widget_title = 'Download your Leaders Widget!';
      break;
    case 'aw':
      $title = 'Leaders in Automation';
      $widget_title = 'Download your Leaders Widget!';
      break;
    case 'hcp':
      $title = 'Premier Suppliers';
      $widget_title = 'Download your Premier Suppliers Widget!';
      break;
  }

  $user = user_load($uid);

  // *** Based on the company associated with the user find the 3 profile nodes.
  $mappings = leadership_site_mappings($site_id);
  $company_nid = $user->{$mappings['company_field']}['und'][0]['nid'];
  $company = node_load($company_nid);

  $output = '<h1>' . $title . '</h1>';
  $output .= '<h2 class="company-name">' . $company->title . '</h2>';

  $header = array(
    'Click each form to get started',
    'Status',
    'Due',
  );

  $rows[] = leadership_user_dashboard_get_profile_row($company_nid, 'leadership_print_profile', '1) ', variable_get('leadership_print_due_date'), $site_id);
  $rows[] = leadership_user_dashboard_get_profile_row($company_nid, 'leadership_data_card', '2) ', variable_get('leadership_datacard_due_date'), $site_id);
  $rows[] = leadership_user_dashboard_get_profile_row($company_nid, 'leadership_online_profile', '3) ', variable_get('leadership_online_due_date'), $site_id);

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  $output .= '<a class="widget-button icon chat" href="/leaders/widget/' . $company_nid . '/' . $uid . '"><span>' . $widget_title . '</span></a>';

  return $output;
}

/**
 * Returns an array that contains the profile links, precent complete, and the
 * due date.
 *
 * @param integer $company_nid
 *  The company node id.
 * @param string $content_type
 *  The content type. Valid content types include: leadership_data_card,
 *  leadership_online_profile, leadership_print_profile
 * @param string $count
 *  The number that will be in the row.
 * @param string $due_date
 *  The date string to show when the profile is due
 * @param string $site_id
 *  A string identifier that identifies the site.
 *
 * @return array $row
 *  The html row string.
 */
function leadership_user_dashboard_get_profile_row($company_nid, $content_type, $count = '', $due_date, $site_id) {
  $mappings = leadership_site_mappings($site_id);
  $query = db_select($mappings['company_field_table'], 'companies')
        ->fields('companies', array('entity_id', $mappings['company_field_nid'], 'bundle'))
        ->condition('bundle', $content_type)
        ->condition($mappings['company_field_nid'], $company_nid);

  $result = $query->execute();

  while($record = $result->fetchAssoc()) {
    $nid = $record['entity_id'];
  }

  $node = node_load($nid);
  // Get node type names.
  $types = node_type_get_types();

  // Node Path
  $path =  l($types[$content_type]->name,'node/' . $node->nid . '/edit');

  $percent_complete = content_complete_get_data($node);

  $percent_complete = $percent_complete['percent'];

  // Check if published or not
  if($node->status == 0) {
    $status = $percent_complete . '% complete / ' . 'Unapproved';
  }
  else {
    $status = 'Approved';
  }

  $row = array(
    $count . $path,
    $status,
    $due_date,
  );

  return $row;
}

/**
 * Page that contains a companies Leadership Widget
 *
 * @param integer $nid
 *  Node id.
 * @param integer $uid
 *  User id.
 *
 * @return string $message
 *  Html message.
 */
function leadership_widget_page($nid, $uid = FALSE) {
  $site_id = variable_get('smg_global_site');
  // Set the dashboard back link
  if(!$uid) {
    // If no uid its probably an admin so sending back to admin dashboard
    $dashboard_link = '/admin/leadership';
  }
  else {
    $dashboard_link = '/leaders/user/' . $uid;
  }

  // Path to the company page
  $company_page_path = url("node/$nid", array('absolute' => TRUE));

  $data_card_nid = leadership_get_corresponding_nid($nid, 'leadership_data_card', $site_id);

  $message = '
  <div style="margin-bottom: 10px;">
    <h1>Leadership Widget Instructions</h1>
    <a class="back-link" href="' . $dashboard_link . '">« Back to Dashboard</a>
    <div style="font-size:18px; width="90%;"">
      <br />
      <br />
      <p>There are 3 sizes: small, medium, and large.  The resolutions are listed below the Widget.</p>
      <br />
      <p>The widget will use the logo you uploaded in your <a style="color:blue" href="/node/' . $data_card_nid . '/edit">data card</a>. Copy the ENTIRE Embed Code and paste it into your html page.</p>
      <p>If you update your logo, please copy embed code again.</p>
    </div>
  </div>';

  // array of setting for the widget
  $image_styles = array('small' => '120x111 pixels', 'medium' => '210x195 pixels', 'large' => '300x278 pixels');

  $data_card = node_load(leadership_get_corresponding_nid($nid, 'leadership_data_card', $site_id));

  if(!isset($data_card->field_ld_logo['und'][0]['uri'])) {
    $message .= "<strong>Please update your data card with a logo.</strong>";
  }
  else {
    foreach($image_styles as $image_style_suffix => $resolution) {
      $image = '<img src="' . image_style_url("leadership-widget-$image_style_suffix", $data_card->field_ld_logo['und'][0]['uri']) . '" />';
      $html = '<a href="' . $company_page_path . '" >' . $image . '</a>';

      $message .= '
        <div style="float: left; width: 310px; height: 530px; margin: 5px; padding: 10px; border: 1px dotted #999;"><br />
          ' . $image . '<br />
          <h4>' . $image_style_suffix . '</h4><br />
          <strong>Size: </strong>' . $resolution . '<br /><br />
          <strong>Embed Code: </strong><br /><textarea rows="10" style="width: 290px; border:1px solid #000;">' . htmlentities($html) . '</textarea><br />
        </div>
      ';
    }
  }

  return $message;
}
