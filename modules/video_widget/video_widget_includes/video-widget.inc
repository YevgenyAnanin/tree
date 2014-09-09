<?php

/**
 *
 * @file
 * Contains functions for the video widget and angularjs integration
 *
 */

/*
 * This function returns a resource, such as an html file, without any
 * other drupal code.  For example, this will be used as a router for
 * AngularJS apps that need to load a template file
 *
 * @param string $type
 *  The type of resource (i.e. angular template)
 * @param string $resource
 *  Argument specifying a resource to load
 */
function video_widget_resource($type, $resource) {

  // Initialize $path as the path to the video_widget_includes dir
  $path = drupal_get_path('module','smg_global').'/video_widget_includes';

  switch ($type) {
    case 'angular-template':
      $path .= '/angular/templates/';
      break;
  }

  $path .= $resource; // Full resource path
  $resource_string = file_get_contents($path); // Load resource as string

  print $resource_string;
  return NULL;

}