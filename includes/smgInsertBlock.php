<?php
class SmgInsertBlock
{
  private $_body;
  private $_wordCount;
  private $_explodedBody;
  private $_paragraphCounts;
  private $_insertedBody;
  private $_explodeTag;
  private $_inserts;
  
  function __construct($node, $field_name, $explodeTag = '</p>') {
    $this->_explodeTag = $explodeTag;
    $this->_body = $this->getBodyFromNode($node, $field_name);
    $this->_wordCount = $this->findWordCount($this->_body);
    $this->_explodedBody = explode($explodeTag, $this->_body);
    if($this->_explodedBody) {
      foreach($this->_explodedBody as $paragraph) {
        if($paragraph) {
          $this->_paragraphCounts[] = $this->findWordCount($paragraph);
        }
      }
    }
  }
  
  /**
   * Insert string after a certain word count
   *
   * @param int $wordCount
   *  the word count
   * @param string $insertString
   *  the string you want to insert
   */
  public function insertAfterWordCount($wordCount, $insertString) {
    $totalWordCount = 0;
    $inserted = FALSE;
    foreach($this->_explodedBody as $key => $exploded) {
      $totalWordCount = $totalWordCount + $this->_paragraphCounts[$key];
            
      if($totalWordCount >= $wordCount && !$inserted) {
        $this->_inserts[$key] = $insertString;
        $inserted = TRUE;
      }
    }
  }
  
  /**
   * Insert string after a paragraph count
   *
   * @param int $paragraphAfter
   *  the paragraph you want to insert after
   * @param string $insertString
   *  the string you want to insert
   */
  public function insertAfterParagraph($paragraphAfter, $insertString) {
    
    for($i = 0; $i < count($this->_explodedBody); $i++) {
      if($i === $paragraphAfter) {
        $this->_insertedBody .= $insertString;
      }
      
      $this->_insertedBody .= $this->_explodedBody[$i] . $this->_explodeTag;
    }
  }
  
  /**
   * Returns the body from a node
   *
   * @param object $node
   *  the drupal node object
   * @param string $field name
   *  the name of the field. Ex. 'body'
   *
   * @return string
   *  the body string
   */
  public function getBodyFromNode($node, $field_name) {
    $body = field_get_items('node', $node, $field_name);

    if (isset($body[0]['safe_value'])) {
      return $body[0]['safe_value'];
    }
    else {
      if ($body[0]['format'] == 'plain_text') {
        return check_plain($body[0]['value']);
      }
      else {
        return check_markup($body[0]['value'], $body[0]['format']);
      } 
    }
  }
  
  /**
   * Get word count
   *
   * @param string $string
   *  the string you want word count
   * @param boolean $removeHtml
   *  set to true if you want to remove html
   *
   * @return int
   *  the word count
   */
  public function findWordCount($string, $removeHtml = TRUE) {
    if($removeHtml) {
      $string = strip_tags($string);
    }
    
    return str_word_count($string);
  }
  
  public function addInsertBlocks() {
    $new_body = '';
    foreach($this->_explodedBody as $key => $paragraph) {
      if(isset($this->_inserts[$key])) {
        $new_body .= $paragraph . $this->_explodeTag . $this->_inserts[$key];
      }
      else {
        $new_body .= $paragraph . $this->_explodeTag;
      }
    }
    
    $this->_insertedBody = $new_body;
  }
  
  /**
   * Get the body string
   *
   * @return string
   *  body string
   */
  public function getBody() {
    $this->addInsertBlocks();

    return $this->_insertedBody;
  }
  
  /**
   * Get the word count
   *
   * @return int
   *  word count
   */
  public function getWordCount() {
    return $this->_wordCount;
  }
}