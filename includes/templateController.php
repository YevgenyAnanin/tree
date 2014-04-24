<?php
  
class SmgGlobalTemplateController {
  protected $vars = array();
  public function render($template_file){
    if(file_exists($template_file)) {
      ob_start();
      include $template_file;
      return ob_get_clean();
    } else {
      throw new Exception('no template file called ' . $template_file . ' found');
    }
  }
  public function __set($name, $value) {
    $this->vars[$name] = $value;
  }
  public function __get($name) {
    return $this->vars[$name];
  }
}
  
?>