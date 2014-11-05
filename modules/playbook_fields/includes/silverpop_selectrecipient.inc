<?php

class SelectRecipientRequestType {
  public function __construct($list_id, $email) {
    $this->LIST_ID = new SoapVar($list_id, XSD_INTEGER, '', '', 'LIST_ID', SP_XSD);
    $this->EMAIL = new SoapVar($email, XSD_STRING, '', '', 'EMAIL', SP_XSD);
  }
}
