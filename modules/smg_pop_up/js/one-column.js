
/**
 * This JS file will be added to the Pop-Up Webform Layout configuration page, it implements
 * the jQuery UI draggable and resizable interactions
 */

(function ($) {
  
  // After every reorder or resize, store the new order in the #hidden-order hidden input element
  function getOrder(){
    // The order var stores the order of the li elements in an array
    var order = $('#column-1').sortable('toArray');
    // Now we iterate through the order array and determine the x and y position of each li element.
    // We do this to check if any li elements are adjacent to each other (i.e. - have the same y-axis position).
    // The newOrder object will be keyed by the y-axis position(s) of the li elements.  Each y-axis key has a 
    // value that is an array, this array contains one or more objects, each of which represents the li elements
    // for that y-axis position.
    var newOrder = {}; 
    newOrder.column1 = {};
    // We also want to determine the width of the li element with respect to the width of the containing list.
    // This way we know that if two li elements are adjacent, what their respective widths should be.  For example
    // we may want the city field to be ~70%, while an adjacent zip code field is ~30%.
    var containerWidth = $('#column-1').width();
    
    $.each(order, function(index, value){
      var yPosition = $('li#' + value).position().top; // y axis position of li element
      var liWidth = $('li#' + value).outerWidth(); // width of li element
      var liWidthRatio = Math.round(liWidth/containerWidth * 100); // width as a % of container width
      var liObj = {'id':value, 'width':liWidthRatio};
      if (yPosition in newOrder['column1']) {
        newOrder['column1'][yPosition].push(liObj);
      }
      else {
        newOrder['column1'][yPosition] = new Array();
        newOrder['column1'][yPosition].push(liObj);
      }
    });
    //console.log(newOrder);
    newOrderString = JSON.stringify(newOrder);
    console.log(newOrderString);
    $('#hidden-order').val(newOrderString);
  }
  
  $(window).load(function(){
    $('#field-list, #column-1').sortable({
      connectWith: '#field-list, #column-1',
      tolerance: function(currentItem) {
        return 'pointer';
      },
      items: 'li',
      opacity: 0.6,
      update: function(event, ui) {
        getOrder();
      },
    });
    $('#field-list, #column-1').disableSelection();
    
    // Get the pixel width value of the columns.  This will be used to calculate the width of
    // each li as a % of the width of the column (after resizing the li).  Subtract 3 because that
    // is the left/right margin on the li elements.
    var columnWidth = $('#field-list').width();
    
    $('.items').children('li').each(function(){
      $(this).resizable({
        resize: function(event, ui) {
          ui.element.css('height', 20);
        },
        stop: function(event, ui) {
          getOrder();
        },
        minHeight: 20,
        maxHeight: 20,
        maxWidth: columnWidth,
        minWidth: 10,
        grid: [1,1],
        handles: 'e',
      }).disableSelection();
    });
  
  });
})(jQuery)