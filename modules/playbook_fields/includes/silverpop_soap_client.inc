<?php

require_once(drupal_get_path('module', 'playbook_fields') . '/includes/constants.inc');
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/silverpop_column.inc');
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/silverpop_addrecipient.inc');
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/silverpop_selectrecipient.inc');
require_once(drupal_get_path('module', 'playbook_fields') . '/includes/silverpop_addcontacttoprogram.inc');

class SilverpopSoapClient {
  private $client = NULL;
  private $sessionid = NULL;
  private $file_path = NULL;
  private $job_id = NULL;

  public function __construct() {
    //Login right when the object is created
    $this->login();
  }

  /**
    * Function for logging into SilverPop API
    */
  private function login() {
    $this->client = new SoapClient(SP_WSDL, array(
      "trace" => TRUE,
      'cache_wsdl' => WSDL_CACHE_NONE,
      'soap_version' => SOAP_1_1,
    ));

    try {
      $result = $this->client->Login(array('USERNAME'=> SP_USERNAME, 'PASSWORD'=> SP_PASSWORD));
      if($result->SUCCESS == FALSE) {
        watchdog('playbook_fields', 'Silverpop Client login failed. login(USERNAME:%username PASSWORD:%password) FaultString: %faultstring ErrorID: %faultcode', array('%username' => SP_USERNAME, '%password' => SP_PASSWORD, '%faultstring' => $result->Fault->FaultString, '%faultcode' => $result->Fault->FaultCode), WATCHDOG_ERROR);
      } else {
        $this->sessionid = $result->SESSIONID;
      }
    } catch (SoapFault $fault) {
      // *** Log Error
      //$this->log->log(array('login()', 'Failed Soap Call',$file_path));
      //TODO watchdog
    } 
  }
  
  /**
    * Function for logging out of Silverpop API
    */   
  public function logOut() {
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
  }

  /**
    * Add a contact to Summit Media Group Audience Database
    */
  public function addContact($contact) {
    $columns = array();

    foreach ($contact as $name => $value) {
      $columns[] = new SilverpopColumn($name, $value);
    }

    //$visitor_key = isset($_COOKIE['com.silverpop.iMAWebCookie']) ? $_COOKIE['com.silverpop.iMAWebCookie'] : '';
    $visitor_key = '';
    $recipient = new AddRecipientRequestType(DATABASE_ID, SP_OPT_IN, $columns, $visitor_key);

    try {
      $headerbody = array('sessionid' => $this->sessionid);
      $header = new SOAPHeader('SilverpopApi:EngageService.SessionHeader', 'sessionHeader', $headerbody);
      $this->client->__setSoapHeaders($header);
      $response = $this->client->AddRecipient($recipient);
      //mail('jsalisbury@summitmediagroup.com','testytest',$this->client->__getLastRequest());
      return $response;
    } catch (SoapFault $fault) {
      //TODO watchdog
      throw $fault;
    }
  }

  /**
    * Fetch a contact from Summit Media Group Audience Database
    */
  public function getContact($key) {
    $contact = array();

    $select = new SelectRecipientRequestType(DATABASE_ID, $key);

    try {
      $headerbody = array('sessionid' => $this->sessionid);
      $header = new SOAPHeader('SilverpopApi:EngageService.SessionHeader', 'sessionHeader', $headerbody);
      $this->client->__setSoapHeaders($header);
      $response = $this->client->SelectRecipientData($select);
    } catch (SoapFault $fault) {
      //TODO watchdog
      throw $fault;
    }

    if ($response->SUCCESS) {
      foreach($response->COLUMNS->COLUMN as $column) {
        $contact[$column->NAME] = $column->VALUE;
      }
      $contact['Email'] = $response->EMAIL;
      $contact['Recipient ID'] = $response->RecipientId;
    }

    return $contact;
  }

  /**
    * Add a contact to a program
    */
  public function addContactToProgram($contact_id, $program_id) {
    $request = new AddContactToProgramRequestType($contact_id, $program_id);

    try {
      $headerbody = array('sessionid' => $this->sessionid);
      $header = new SOAPHeader('SilverpopApi:EngageService.SessionHeader', 'sessionHeader', $headerbody);
      $this->client->__setSoapHeaders($header);
      $response = $this->client->AddContactToProgram($request);
    } catch (SoapFault $fault) {
      //TODO watchdog
      throw $fault;
    }
  }

  /**
    *  Function for running loops to verify that the job has been complete
    *
    * @param $jobid
    *  string - Job ID of the current job
    */
  public function checkStatus($jobid) {
    //Getting time to prevent infinite loop
    $time_limit = time() + SP_SCRIPT_TIME_LIMIT;

    $status = $this->getJobStatus($jobid);

    //If Waiting than keep looping
    while($status->JOB_STATUS == 'WAITING' && time() < $time_limit) {
      $status = $this->getJobStatus($jobid);
    }
    
    //If Running than keep looping
    while($status->JOB_STATUS == 'RUNNING' && time() < $time_limit) {
      $status = $this->getJobStatus($jobid);
    }

    
    if($time <= $time_limit && $status->JOB_STATUS == 'COMPLETE') {
      //Done!!!!
    } else {
      // *** Log Error
      //$this->log->log(array('checkStatus(' . $job_id . ')', 'Failed during status check. JOB_STATUS=' . $status->JOB_STATUS . ' $time=' . date("H:i:s", $time) . ' $time_limit=' . date("H:i:s", $time_limit),$file_path));
      //TODO watchdog
    }
  }
      
  /**
    * Function for getting job status
    *
    * @param $job_id
    *  string - id provided when performing a job
    */
  public function getJobStatus($job_id) {
    try {
      $result = $this->client->GetJobStatus(array('JOB_ID' => $job_id));
    } catch (SoapFault $fault) {
      // *** Log Error
      //$this->log->log(array('GetJobStatus(' . $job_id . ')', 'Failed Soap Call',$file_path));
      //TODO watchdog
    }
    return $result;
  }

  /**
   * Function for showing debug information on the browser for a good api call
   */
  public function debugPass() {
    echo "REQUEST:\n";
    echo htmlentities($this->client->__getLastRequest());
    echo "<br />";
    echo "RESPONSE:\n";
    echo htmlentities($this->client->__getLastResponse());
  }


  /**
   * Function for showing debug information on the browser for a failed api call
   */
  public function debugFail() {
    print('<pre>');
    echo "GENERAL EXCEPTION:<br />".$fault->faultcode."-".$fault->faultstring."\n<br />";

    echo "EXCEPTION:\n";
    print_r($fault);

    echo "REQUEST:\n";
    echo htmlentities($this->client->__getLastRequest());
    echo "<br />";

    echo "REQUEST HEADERS:\n";
    echo $this->client->__getLastRequestHeaders();

    echo "RESPONSE HEADERS:\n";
    echo $this->client->__getLastResponseHeaders();

    echo "RESPONSE:\n";
    echo htmlentities($this->client->__getLastResponse());

    print('</pre>');
  }
  
  public function __destruct()
  {
    $this->logOut();
  }
}