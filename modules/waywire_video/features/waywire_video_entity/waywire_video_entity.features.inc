<?php
/**
 * @file
 * waywire_video_entity.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function waywire_video_entity_image_default_styles() {
  $styles = array();

  // Exported image style: waywire_video_widget_thumbnail.
  $styles['waywire_video_widget_thumbnail'] = array(
    'name' => 'waywire_video_widget_thumbnail',
    'label' => 'waywire_video_widget_thumbnail',
    'effects' => array(
      34 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '122',
          'height' => '69',
        ),
        'weight' => '1',
      ),
      35 => array(
        'label' => 'Overlay (watermark)',
        'help' => 'Choose the file image you wish to use as an overlay, and position it in a layer on top of the canvas.',
        'effect callback' => 'canvasactions_file2canvas_image',
        'dimensions passthrough' => TRUE,
        'form callback' => 'canvasactions_file2canvas_form',
        'summary theme' => 'canvasactions_file2canvas_summary',
        'module' => 'imagecache_canvasactions',
        'name' => 'canvasactions_file2canvas',
        'data' => array(
          'xpos' => 'center',
          'ypos' => 'center',
          'alpha' => '100',
          'path' => 'sites/default/themes/aw960/css/images/video_overlay_50.png',
        ),
        'weight' => '2',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_default_waywire_video_type().
 */
function waywire_video_entity_default_waywire_video_type() {
  $items = array();
  $items['leadership_video'] = entity_import('waywire_video_type', '{
    "type" : "leadership_video",
    "label" : "Leadership Video",
    "weight" : "0",
    "data" : "",
    "rdf_mapping" : []
  }');
  return $items;
}
