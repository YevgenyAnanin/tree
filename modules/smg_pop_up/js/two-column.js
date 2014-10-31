
/**
 * This JS file will be added to the Pop-Up Webform Layout configuration page, it implements
 * the jQuery UI draggable and resizable interactions
 */

(function ($) {
  
  // After every reorder or resize, store the new order in the #hidden-order hidden input element
  function getOrder(){
    // The orderOne var stores the order of the li elements from column 1 in an array
    var orderOne = $('#column-1').sortable('toArray');
    // The orderTwo var stores the order of the li elements from column 2 in an array
    var orderTwo = $('#column-2').sortable('toArray');
    // Now we iterate through the order array and determine the x and y position of each li element.
    // We do this to check if any li elements are adjacent to each other (i.e. - have the same y-axis position).
    // The newOrder object will be keyed by the y-axis position(s) of the li elements.  Each y-axis key has a 
    // value that is an array, this array contains one or more objects, each of which represents the li elements
    // for that y-axis position.
    var newOrder = {}; 
    newOrder.column1 = {};
    newOrder.column2 = {};
    // We also want to determine the width of the li element with respect to the width of the containing list.
    // This way we know that if two li elements are adjacent, what their respective widths should be.  For example
    // we may want the city field to be ~70%, while an adjacent zip code field is ~30%.
    var containerOneWidth = $('#column-1').width();
    var containerTwoWidth = $('#column-2').width();
    
    $.each(orderOne, function(index, value){
      var yPosition1 = $('li#' + value).position().top; // y axis position of li element
      var liWidth1 = $('li#' + value).outerWidth(); // width of li element
      var liWidthRatio1 = Math.round(liWidth1/containerOneWidth * 100); // width as a % of container width
      var liObj1 = {'id':value, 'width':liWidthRatio1};
      if (yPosition1 in newOrder['column1']) {
        newOrder['column1'][yPosition1].push(liObj1);
      }
      else {
        newOrder['column1'][yPosition1] = new Array();
        newOrder['column1'][yPosition1].push(liObj1);
      }
    });

    $.each(orderTwo, function(index, value){
      var yPosition2 = $('li#' + value).position().top; // y axis position of li element
      var liWidth2 = $('li#' + value).outerWidth(); // width of li element
      var liWidthRatio2 = Math.round(liWidth2/containerTwoWidth * 100); // width as a % of container width
      var liObj2 = {'id':value, 'width':liWidthRatio2};
      if (yPosition2 in newOrder['column2']) {
        newOrder['column2'][yPosition2].push(liObj2);
      }
      else {
        newOrder['column2'][yPosition2] = new Array();
        newOrder['column2'][yPosition2].push(liObj2);
      }
    });

    //console.log(newOrder);
    newOrderString = JSON.stringify(newOrder);
    console.log(newOrderString);
    $('#hidden-order').val(newOrderString);
  }
  
  
  
  $(window).load(function(){
    $('#field-list, #column-1, #column-2').sortable({
      connectWith: '#field-list, #column-1, #column-2',
      tolerance: function(currentItem) {
        return 'pointer';
      },
      items: 'li',
      opacity: 0.6,
      update: function(event, ui) {
        getOrder();
      },
    });
    $('#field-list, #column-1, #column-2').disableSelection();
    
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