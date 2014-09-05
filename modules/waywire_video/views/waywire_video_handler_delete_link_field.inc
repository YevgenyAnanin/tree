<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */

class waywire_video_handler_delete_link_field extends waywire_video_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy waywire video to check access against
    $dummy_waywire_video = (object) array('type' => $type);
    if (!waywire_video_access('edit', $dummy_waywire_video)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $id = $values->{$this->aliases['id']};

    return l($text, 'admin/content/waywire_videos/waywire_video/' . $id . '/delete');
  }
}
