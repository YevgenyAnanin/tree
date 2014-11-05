<?php

class AddRecipientRequestType {
    public function __construct($list_id, $created_from, $columns, $send_autoreply = FALSE, $update_if_found = TRUE, $allow_html = TRUE) {
        $this->LIST_ID = new SoapVar($list_id, XSD_INTEGER, '', '', 'LIST_ID', SP_XSD);
        $this->CREATED_FROM = new SoapVar($created_from, XSD_INTEGER, '', '', 'CREATED_FROM', SP_XSD);
        $this->SEND_AUTOREPLY = new SoapVar($send_autoreply, XSD_BOOLEAN, '', '', 'SEND_AUTOREPLY', SP_XSD);
        $this->UPDATE_IF_FOUND = new SoapVar($update_if_found, XSD_BOOLEAN, '', '', 'UPDATE_IF_FOUND', SP_XSD);
        $this->ALLOW_HTML = new SoapVar($allow_html, XSD_BOOLEAN, '', '', 'ALLOW_HTML', SP_XSD);
        $this->COLUMN = $columns;
    }
}
