<?php

/**
 * @file
 * Class definition of WaywireProcessor.
 */

/**
 * Creates waywire videos from feed items.
 */
class WaywireProcessor extends FeedsProcessor {
  /**
   * @var integer - the company nid
   */
  public $company_nid = NULL;
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'waywire_video';
  }

    /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Waywire Videos');
    return $info;
  }

  /**
   * Creates a waywire_video in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $values = array(
      'type' => $this->bundle(),
      'author_uid' => $this->config['author'],
    );
    return waywire_video_create($values);
  }

  /**
   * Loads an existing waywire video.
   */
  protected function entityLoad(FeedsSource $source, $id) {
    return waywire_video_load($id);
  }

  /**
   * Save a waywire video.
   */
  public function entitySave($entity) {
    return waywire_video_save($entity);
  }

  /**
   * Delete a series of waywire videos.
   */
  protected function entityDeleteMultiple($ids) {
    waywire_video_delete_multiple($ids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $return = array(
      'author' => 0,
    ) + parent::configDefaults();

    return $return;
  }

  /**
   * Override parent:configForm() to add custom config FAPI elements.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    unset($form['input_format']);

    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the organizations to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ?  'anonymous' : check_plain($author->name),
    );

    return $form;
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    if ($author = user_load_by_name($values['author'])) {
      $values['author'] = $author->uid;
    }
    else {
      $values['author'] = 0;
    }
    parent::configFormSubmit($values);
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = array(
      'name' => array(
        'name' => t('Title'),
        'description' => t('The Waywire video title.'),
      ),
      'magnify_externalid' => array(
        'name' => t('Magnify External ID'),
        'description' => t('The Waywire External ID (used to be called Magnify).'),
      ),
      'updated' => array(
        'name' => t('Updated'),
        'description' => t('The date the video was updated.'),
      ),
      'author_id' => array(
        'name' => t('Author ID'),
        'description' => t('The Waywire author ID.'),
      ),
      'author_name' => array(
        'name' => t('Author Name'),
        'description' => t('The Waywire author name.'),
      ),
      'author_uri' => array(
        'name' => t('Author URI'),
        'description' => t('The Waywire author uri.'),
      ),
      'content' => array(
        'name' => t('Content'),
        'description' => t('The Waywire content.'),
      ),
      'thumbnail' => array(
        'name' => t('Thumbnail'),
        'description' => t('The Waywire thumbnail.'),
      ),
      'published' => array(
        'name' => t('Published'),
        'description' => t('The Waywire published date.'),
      ),
      'media_width' => array(
        'name' => t('Media Width'),
        'description' => t('Video Width'),
      ),
      'media_medium' => array(
        'name' => t('Media Medium'),
        'description' => t('Video Medium'),
      ),
      'media_url' => array(
        'name' => t('Media URL'),
        'description' => t('Video URL'),
      ),
      'media_type' => array(
        'name' => t('Media Type'),
        'description' => t('Video type'),
      ),
      'media_height' => array(
        'name' => t('Media Height'),
        'description' => t('Video type'),
      ),
      'media_duration' => array(
        'name' => t('Media Duration'),
        'description' => t('Video Duration'),
      ),
      'link_self' => array(
        'name' => t('Link Self'),
        'description' => t('Link self'),
      ),
      'link_alternative' => array(
        'name' => t('Link Alternative'),
        'description' => t('Link alternative'),
      ),
      'link_author' => array(
        'name' => t('Link Author'),
        'description' => t('Link author'),
      ),
      'categories' => array(
        'name' => t('Categories'),
        'description' => t('Tags'),
      ),
      'guid' => array(
        'name' => t('Magnify ID'),
        'description' => t('The external GUID of the comment. E. g. the feed item GUID in the case of a syndication feed. May be unique.'),
        'optional_unique' => TRUE,
      ),
    );

    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->bundle();
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Override setTargetElement to operate on a target item.
   */
  public function setTargetElement(FeedsSource $source, $target_entity, $target_element, $value) {
    switch ($target_element) {
      case 'name':
      case 'node_id':
      case 'created':
      case 'changed':
        parent::setTargetElement($source, $target_entity, $target_element, $value);
        break;
      case 'guid':
        // Setting GUID to feeds table and also the waywire video table
        // (magnify_id)
        parent::setTargetElement($source, $target_entity, $target_element, $value);
        $target_entity->{$target_element} = $value;
        // When setting the GUID I will also set the company node id
        if($this->company_nid) {
          $target_entity->company_nid = $this->company_nid;
        }
        break;
      default:
        $target_entity->data[$target_element] = $value;
        break;
    }
  }
}
