<?php
/**
  * Form-builder for Forwarding form.
  *
  * @param $form_state
  *  The current state of the form as per FAPI documentation.
  * @param $playbook
  *  A Playbook configuration array.
  *
  * @return
  *  A FAPI array, ready to be processed.
  */
function playbook_forward_form($form, $form_state, $playbook_name, $playbook_alias) {
    // Pull any existing data from the cookie for pre-population of form
    //list($rid, $first_name, $last_name, $company, $email) = explode('::', $_COOKIE['PWOMEDA']);

    $form = array();

    $form['playbook_name'] = array(
        '#type'   => 'value',
        '#value'  => $playbook_name,
    );
    $form['playbook_url'] = array(
        '#type' => 'value',
        '#value' => url($playbook_alias, array(
            'absolute' => TRUE,
        )),
    );
    $form['greeting'] = array(
        '#type' => 'markup',
        '#markup' => 'Forward <em>' . $playbook_name . '</em> to your friends and colleagues!',
        '#prefix' => '<h1>',
        '#suffix' => '</h1>',
    );
    $form['sender_name'] = array(
        '#type'           => 'textfield',
        '#title'          => t('Your name'),
        '#default_value'  => !empty($first_name) || !empty($last_name) ? $first_name . ' ' . $last_name : '',
        '#size'           => 40,
        '#maxlength'      => 64,
        '#required'       => TRUE,
    );
    $form['sender_email'] = array(
        '#type'           => 'textfield',
        '#title'          => t('Your email address'),
        '#default_value'  => $email,
        '#size'           => 40,
        '#maxlength'      => 64,
        '#required'       => TRUE,
    );
    $form['recipients'] = array(
        '#type'         => 'textarea',
        '#title'        => t("Recipients' email addresses"),
        '#description'  => t('Separate multiple addresses with a comma (e.g. "john@example.com, jane@example.com").'),
        '#cols'         => 40,
        '#rows'         => 3,
        '#required'     => TRUE,
    );
    $form['sender_message'] = array(
        '#type'         => 'textarea',
        '#title'        => t('Your personal message to these recipients'),
        '#cols'         => 40,
        '#rows'         => 5,
        '#required'     => FALSE,
    );
    $name_to_theme = array(
      'Packaging World' => 'pw960',
      'Automation World' => 'aw960',
      'Healthcare Packaging' => 'hcp',
    );
    $theme = $name_to_theme[variable_get('site_name')];
    $form['submit'] = array(
        '#type'   => 'submit',
        '#value'  => t('Forward Playbook'),
        '#attributes' => array(
            'style' => "float:none; width:271px; height:28px; background:#F0F0F0 0 0 no-repeat scroll url(/sites/default/themes/$theme/css/images/bigbluebtn.png); color:#FFFFFF; font-size:18px;",
        ),
    );

    return $form;
}



/**
  * Form validation handler for Playbooks Forwarding form.
  */
function playbook_forward_form_validate($form, &$form_state) {
    // Ensure a valid "From" address.
    if (!valid_email_address($form_state['values']['sender_email'])) {
        form_error($form['sender_email'], t('Please check your email address, it appears to be invalid'));
    }

    // Simplistic validation of "To" address(es) and comma-separated format for multiple addresses
    foreach (explode(',', $form_state['values']['recipients']) as $mail) {
        if (!valid_email_address(trim($mail))) {
            form_error($form['recipients'], t("Please check the recipients' addresses.  It appears that one or more is invalid.  Remember to separate multiple addresses with a comma."));
            break;
        }
    }
}



/**
  * Form submission handler for Playbooks Forwarding form.
  *
  * Sends an email to one or more recipients based on the "Foward Playbook"
  * functionality.
  */
function playbook_forward_form_submit($form, &$form_state) {
    $playbook_name = $form_state['values']['playbook_name'];
    $playbook_url = $form_state['values']['playbook_url'];
    // Send the user to the forwading relay so as to avoid breaking the link when
    // wrapping the email text.
    //$playbook_url = url('playbooks/' . $playbook_name . '/forwarded', array('absolute' => TRUE));

    // Build the various message parts based on the message template and the
    // contents of the submitted form.
    $to = $form_state['values']['recipients'];
    $from = $form_state['values']['sender_email'];
    $subject = $form_state['values']['sender_name'] . ' has sent you a Playbook!';
    $body = array(
        $form_state['values']['sender_name'] . " has recommended that you download " . $playbook_name . ".  Below, you will find the download link:\n",
        $playbook_url . "\n",
        "Download this free, extensive PDF playbook jam-packed with strategies for success, best practices, and pitfalls to avoid.  For this playbook, " . variable_get('site_name', 'Summit Media Group') . " interviewed leading packaging professionals at CPG companies, consultants, suppliers, and design experts.  The playbook is written in multiple sections for the entire cross-functional team.",
        (!empty($form_state['values']['sender_message']) ?
            "\n\n" . "A message from " . $form_state['values']['sender_name'] . ":\n\n" . $form_state['values']['sender_message']
            : ''
        ),
    );
    $headers = array(
        'From'      => $form_state['values']['sender_name'] . " <$from>",
        'Reply-To'  => $from,
    );

    $params = array(
        'to'      => $to,
        'from'    => $from,
        'subject' => $subject,
        'body'    => $body,
        'headers' => $headers,
    );

    $message = drupal_mail('playbook_fields', 'playbook_forward', $to, language_default(), $params, $from);

    if ($message) {
        // Add to the total recpient tally for this Playbook.
        //$recipient_count = count(explode(',', $to));
        //variable_set('playbooks_forwarded_' . $playbook['name'], variable_get('playbooks_forwarded_' . $playbook['name'], 0) + $recipient_count);

        drupal_set_message(t('Thanks for sharing this Playbook!  The Playbook has been forwarded to the recipients you specified.'));
    }
    else {
        //watchdog('playbooks', smg_prewatch(__FILE__, __LINE__, -6, __FUNCTION__) . 'Forwarding email failed to send successfully.  To: %to, From: %from', array('%to' => $to, '%from' => $form_state['values']['sender_name'] . ' <' . $form_state['values']['sender_email'] . '>'), WATCHDOG_ERROR);
        watchdog('playbooks', 'Forwarding email failed to send successfully.  To: %to, From: %from', array('%to' => $to, '%from' => $form_state['values']['sender_name'] . ' <' . $form_state['values']['sender_email'] . '>'), WATCHDOG_ERROR);
        drupal_set_message(t('There was a problem forwarding the Playbook.  Please try again or contact the administrator if you encounter this problem repeatedly.'), 'error');
    }
}

