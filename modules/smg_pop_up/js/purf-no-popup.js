/**
 *
 * @file
 * Functionality for registering an ajax callback for PURF no-popup links
 * See http://redmine.summitmediagroup.com/issues/57
 */

(function ($) {
  $(document).ready(function () {
    if( $('.purf-no-popup').length ) {
      
      // Look for the reader_token cookie.  If it isn't set, then we will
      // not override the functionality of the link in any way
      var noPopupReaderToken = $.cookie('reader_token');
      if ( noPopupReaderToken ){

        // Get the standard lead gen PURF
        var standardPURFNid = Drupal.settings.standard_purf;
        
        var autoSubmit = function(readerToken, adId, campaignId) {
          // Create GET parameters
          var params = {
            "adId": adId,
            "noPopup": true,
          };

          submitUrl = '/smg-pop-up/auto-submit/' + noPopupReaderToken + '/' + standardPURFNid + '/' + campaignId + '/false/reader_token'; 
          submitUrl+= '?' + decodeURIComponent( $.param( params ) );
          
          return $.ajax({
            url: submitUrl
          });
        }
        
        $('.purf-no-popup').each(function () {

          // Initialize some variables needed by the autoSubmit fct
          var redirectUrl = $(this).attr("href");          
          // Get the classes for this element, so I can extract the ad id
          var classes = $(this).attr("class").split(/\s+/);
          for ( var i = 0; i < classes.length; i++ ) {
            if ( classes[i].indexOf('purf-ad-') > -1 ) {
              var adId = classes[i].split("-")[2];
            }
          }
          // Get the campaign id
          var cId = $(this).attr("data-cid");          

          // Add an onclick event to each purf-no-popup link
          $(this).off('click').on('click', function (e) {
            e.preventDefault();
            e.stopPropagation();
            e.stopImmediatePropagation();
            autoSubmit(noPopupReaderToken, adId, cId).success(function () {
              window.location.href = redirectUrl;
            });
          });
          
        });

      }
    }
  });
})(jQuery);