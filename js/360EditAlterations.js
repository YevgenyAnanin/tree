/**
 * @file
 * - A javascript file that is added to the node edit page for 360 nodes, to tweak the 
 *   bulk upload and field collection fields.
 *
 */

(function($){
  $(document).ready(function(){
    $('#field-360-images-values thead th.field-label label').append('<span id="360-images-label-append"> (+ Click to expand +)</span>');
    $('#field-360-images-values tbody').slideToggle(500);
    $('#field-360-images-values thead th.field-label label').addClass('collapsed').hover(function(){
      $('#field-360-images-values thead th.field-label label').css({'cursor':'pointer'});
    });
    $('#field-360-images-values thead th.field-label label').on('click', function(){
      if($(this).hasClass('collapsed')){
        $('#field-360-images-values tbody').slideToggle();
        $(this).removeClass('collapsed').addClass('expanded');        
        $('#field-360-images-values thead th.field-label label #360-images-label-append').text(' (- Click to collapse -)');
      }
      else if($(this).hasClass('expanded')) {
        $('#field-360-images-values tbody').slideToggle();
        $(this).removeClass('expanded').addClass('collapsed');
        $('#field-360-images-values thead th.field-label label #360-images-label-append').text(' (+ Click to expand +)');
      }
    });
  });  
})(jQuery)