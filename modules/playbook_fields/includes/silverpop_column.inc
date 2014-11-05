<?php

class SilverpopColumn {
    public function __construct($name, $value) {
        $this->NAME = new SoapVar($name, XSD_STRING, '', '', 'NAME', SP_XSD);
        $this->VALUE = new SoapVar($value, XSD_STRING, '', '', 'VALUE', SP_XSD);
    }
}
