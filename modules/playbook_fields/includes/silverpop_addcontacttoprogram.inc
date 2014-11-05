<?php

class AddContactToProgramRequestType {
  public function __construct($contact_id, $program_id) {
    $this->CONTACT_ID = new SoapVar($contact_id, XSD_INTEGER, '', '', 'CONTACT_ID', SP_XSD);
    $this->PROGRAM_ID = new SoapVar($program_id, XSD_INTEGER, '', '', 'PROGRAM_ID', SP_XSD);
  }
}
