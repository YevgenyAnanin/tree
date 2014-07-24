(function($){
  
  // Set date variable
  var date = new Date();
  date.setTime(date.getTime() + (1000 * 60 * 60 * 24 * 365));    
  
  $(document).ready(function(){
    
    documentDomain = document.domain;
    if(documentDomain.indexOf(".www") !== -1){
      documentDomain = documentDomain.substring(1);
    }
    
    var appAlertCookie = $.cookie("smg_mobile_app_alert");
    
    if(!appAlertCookie){
      $("#mobile-app-alert-block").show();
      $("#mobile-app-alert-block a.#app-alert-close").on("click", function(){
        $.cookie('smg_mobile_app_alert', true, {expires: date, path: '/', domain: documentDomain });        
        $(this).parent().hide(400);
      });
      $("#mobile-app-alert-apps-link").on("click", function(event){
        event.preventDefault();
        event.stopPropagation();
        event.stopImmediatePropagation();        
        var url = $(this).attr("href");
        _gaq.push(['_trackEvent', 'App Enticement', 'Clicked', '', 0, true]);
        setTimeout(function(){
          document.location.href = url;
        }, 110);        
      });
    }
  
  });
  
})(jQuery);