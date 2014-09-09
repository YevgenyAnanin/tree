<?php

/**
 * Hooks provided by SMG Angular module
 */

/*
 * Tell SMG Angular the location (directory/url) of an AngularJS template.
 * SMG Angular will then add it to Drupal.settings.smgAngularTemplates, where
 * it can be accessed from an Angular app.
 *
 * @return An array where the keys are readable names of templates, and the values
 * are the location
 */
function hook_smg_angular_add_template() {
  // Example
  $templates = array(
    'template1' => '/sites/default/files/template1.html',
    'template2' => '/sites/default/modules/myModule/template2.html',
  );

  return $templates;
}