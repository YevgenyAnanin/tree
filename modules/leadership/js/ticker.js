/**
 * Slides the Leadership company list
 */
jQuery(document).ready(function($) {
  function tick() {
    $("#leadership-companies li:lt(3)").slideUp( function() {
      $(this).appendTo($('#leadership-companies')).slideDown();
    });
  }

  var interval = setInterval(function() {
    tick();
  }, 4000);
  
  // If hover over the companies than stop the ticker and turn into scroll
   $("#leadership-companies").hover( function() {
     clearInterval(interval);
   });
});