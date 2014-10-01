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

/**
 * Register custom elements.  So if any AngularJS code, anywhere on the site uses
 * any custom elements, they need to register.  This will be used to fix IE8.
 * @see https://docs.angularjs.org/guide/ie
 *
 * @param array $elements
 *  An array where the values are custom elements
 */
function hook_smg_angular_custom_elements() {
  // Example
  $custom_elements = array(
    'custom1',
    'custom2',
    'custom3',
  );
  return $custom_elements;
}
