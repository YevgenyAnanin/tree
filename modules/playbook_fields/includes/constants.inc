<?php
define('DEPENDENCY_LIMIT', 10000);

/// SilverPop Soap API
define('SP_USERNAME', 'playbooksubmissions@summitmediagroup.com');
define('SP_PASSWORD', 'P!ayb00k10');
define('SP_WSDL', 'http://api5.silverpop.com/SoapApi?wsdl');
define('SP_XSD', 'SilverpopApi:EngageService.ListMgmt.RecipientActions');
define('SP_OPT_IN', 2); // Code for an opt-in
define('DATABASE_ID', '860100'); // Summit Media Group Audience Database
//define('DATABASE_ID', '853684'); // Test database
define('SP_SCRIPT_TIME_LIMIT', 840); ///limiting to 14 minutes

/// Silverpop XML API
define('SP_XAU', 'http://api5.silverpop.com/XMLAPI');

/// Account-specific values w/ AccelWorks.
//define('PLAYBOOKS_ACCELA_BILLCODE', 48);
//define('PLAYBOOKS_ACCELA_BILLCODE', 67);

// GotoWebinars
define('GOTO_WEBINAR_API_KEY', '2d597ff25327bb9a6d4ce4ba33a27979');
define('REDIRECT_URL_AFTER_AUTHENTICATION', 'http://dev.hcp.255elmp02.blackmesh.com/oauth/oauth.php');
define('GOTO_WEBINAR_AUTH_TOKEN', 'bb497a03c1ab0d4e2f55521d2199fb51');
define('GOTO_WEBINAR_ORGANIZER_KEY', '300000000000371046');

// Leadworks
define('LEADWORKS_BASE_URL', 'http://leadworks.summitmediagroup.com/api/reg');
define('LEADWORKS_FIELDS_QUERY', 'http://leadworks.summitmediagroup.com/api/getFieldValues');
define('LEADWORKS_APP_USER_POST', 'http://leadworks.summitmediagroup.com/api/appUserPost');
define('LEADWORKS_APP_USER_GET', 'http://leadworks.summitmediagroup.com/api/appUserGet');

define('DEPENDENCY_TEXT', 'Enter a dependency rule to optionally enable this field. Rules take the form \'field_key\' = \'value\'. \'=\': equals. \'!=\': does not equal. \'^=\': begins with. \'!^=\': does not begin with. \'value\' is the submit value, if given, or the display value otherwise, lowercase, with all spaces and punctuation replaced with dashes: \'United States (of America)\' -> \'united-states-of-america\'. Rules can be combined using \'and\' or \'or\'. For example: (\'field_1\' = \'foo\') and (\'field_2\' != \'bar\'). Rules can be nested: ((\'field_1\' = \'united-states\') or (\'field_2\' != \'chicago\')) and (\'field_3\' ^= \'yes\'). All field keys and values are case-sensitive. <br /><br /> Radio Button Dependencies: For a radio button dependency you input \'field_key\' checked \'radio button number\' Ex:\'display-namefield\' checked \'1\'');