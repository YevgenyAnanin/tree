<?php

require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');

class SilverpopXMLClient {
  private $sessionid = NULL;

  public function __construct() {
    //Login right when the object is created
    $this->login();
  }

  /**
    * Function for logging into SilverPop API
    */
  private function login() {
    $login_xml = 'xml=' . rawurlencode('<Envelope><Body><Login><USERNAME>' . SP_USERNAME . '</USERNAME><PASSWORD>' . SP_PASSWORD . '</PASSWORD></Login></Body></Envelope>');
    $curl = curl_init(SP_XAU);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($curl, CURLOPT_POST, TRUE);
    curl_setopt($curl, CURLOPT_POSTFIELDS, $login_xml);
    $parser = xml_parser_create();

    try {
      $result = curl_exec($curl);
      if (xml_parse_into_struct($parser, $result, $values, $index)) {
        $success = $values[$index['SUCCESS'][0]]['value'];
      } else {
        $success = FALSE;
      }
    } catch (SoapFault $fault) {
      //TODO watchdog
    } 

    if ($success == 'true') {
      $this->sessionid = $values[$index['SESSIONID'][0]]['value'];
    }

    xml_parser_free($parser);
    curl_close($curl);
  }
  
  /**
    * Function for logging out of Silverpop API
    */   
  /*public function logOut() {
    try {
      $headerbody = array('sessionid' => $this->sessionid);
      $header = new SOAPHeader('SilverpopApi:EngageService.SessionHeader', 'sessionHeader', $headerbody);
      $this->client->__setSoapHeaders($header);

      $result = $this->client->Logout();
    } catch (SoapFault $fault) {
      // *** Log Error
      //$this->log->log(array('logOut()', 'Failed Soap Call',$file_path));
      //TODO watchdog
    }   
  }*/

  /**
    * Fetch a contact from Summit Media Group Audience Database
    */
  public function getContact($key, $column = 'EMAIL') {
    $contact = array();
    $query_xml = 'xml=' . rawurlencode("<Envelope><Body><SelectRecipientData><LIST_ID>860100</LIST_ID><$column>$key</$column></SelectRecipientData></Body></Envelope>");
    $curl = curl_init(SP_XAU . ';jsessionid=' . $this->sessionid);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($curl, CURLOPT_POST, TRUE);
    curl_setopt($curl, CURLOPT_POSTFIELDS, $query_xml);
    $parser = xml_parser_create();

    try {
      $result = curl_exec($curl);
      if (xml_parse_into_struct($parser, $result, $values, $index)) {
        $success = $values[$index['SUCCESS'][0]]['value'];
      } else {
        $success = FALSE;
      }
    } catch (SoapFault $fault) {
      //TODO watchdog
    } 

    if ($success == 'TRUE') {
      preg_match_all('#<COLUMN>[^<>]*<NAME>([^<>]*)</NAME>[^<>]*<VALUE>([^<>]*)</VALUE>[^<>]*</COLUMN>#', $result, $matches, PREG_SET_ORDER);
      foreach ($matches as $m) {
        $contact[$m[1]] = $m[2];
      }
      $contact['Email'] = $values[$index['EMAIL'][0]]['value'];
    }

    xml_parser_free($parser);
    curl_close($curl);
    return $contact;
  }
  
  /**
   * Logout a user
   */
  public function logout()
  {    
     $logout_xml = 'xml=' . rawurlencode('<Envelope><Body><Logout/></Body></Envelope>');
     $curl = curl_init(SP_XAU . ';jsessionid=' . $this->sessionid);
     curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
     curl_setopt($curl, CURLOPT_POST, TRUE);
     curl_setopt($curl, CURLOPT_POSTFIELDS, $logout_xml);
     try {
       $result = curl_exec($curl);
     } catch (SoapFault $fault) {
       //TODO watchdog
     } 
     curl_close($curl);
  }
  
  public function __destruct()
  {
    $this->logout();
  }
}
