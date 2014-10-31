(function ($, Drupal, window, document, undefined){

  // We set several cookies in the code below.  I want to create a Date object set to a 15 minutes
  // from when the page is loaded, as an expire time for the cookie.
  // @see http://stackoverflow.com/questions/1830246/how-to-expire-a-cookie-in-30-minutes-using-jquery
  var date = new Date();
  date.setTime(date.getTime() + (15 * 60 * 1000));

  var header = '';
  var layoutType = 'two-column-layout';
  var spFullName = false;
  var overrideOn = false;
  var popType = false;
  // If popType is video_click, we use the viddlerVerified variable to track when the user presses play.
  // Once we've tracked the lead (i.e. they fill out the form or we auto-submit), we set the value to true.
  var viddlerVerified = false;
  // If popType is video_click, the following variables will be instantiated (which one
  // depends on whether or not the browser supports flash player)
  var player = false;
  var htmlVideo = false; // This will hold the jQuery object representing the HTML5 video
  var htmlVideoElement = false; // This will hold the actual DOM element contained within the above object

  var htmlWidth;
  var documentDomain;
  var jqxhr;

  // Utility function, helps determine if a function is empty
  var hasOwnProperty = Object.prototype.hasOwnProperty; // @see http://stackoverflow.com/questions/4994201/is-object-empty
  function isEmpty(obj) {
    for (var key in obj) {
      if (hasOwnProperty.call(obj,key)) return false;
    }

    return true;
  }

  // Create a global PURF object, that will contain global properties
  function PURFGlobal() {}
  // Now add the properties to the prototype property, so that all PURFs
  // that extend PURFGlobal will share (and can override)
  PURFGlobal.prototype.queryParameters = '';
  PURFGlobal.prototype.header = '';
  PURFGlobal.prototype.headerNew = '';
  PURFGlobal.prototype.headerExist = '';
  PURFGlobal.prototype.inlineLinkInfo = {};
  PURFGlobal.prototype.twoColumn = {};
  PURFGlobal.prototype.oneColumn = {};
  PURFGlobal.prototype.userID = false;
  PURFGlobal.prototype.disclaimer = false;

  // Create an object that will manage PURF variables and details
  function PURFManager() {
    this.purfNID = false;
    this.customQuestions = [];
    this.extraQuestions = {};
    this.queryURL = '';
    this.purfSettings = {
      "referer_nid":"",
      "leadworks_id":"",
    };
    this.viddlerId = false;
    this.popType = 'page_load';
    this.isExtra = false;
  }

  // Create a PURFGlobal object for PURFManager
  PURFManager.prototype = new PURFGlobal();

  // Function to add parameters to PURFManager object
  PURFManager.prototype.addParameters = function(purfNID, customQuestions) {
    this.purfNID = typeof purfNID !== 'undefined' ? purfNID : 'FALSE';
    this.customQuestions = typeof customQuestions !== 'undefined' ? customQuestions : this.customQuestions;
  }

  PURFManager.prototype.addQueryParameters = function() {}

  // Add function to purfManager object that will generate query URLs
  PURFManager.prototype.createQueryURL = function() {
    var custom = '';
    if(this.customQuestions.length) {
      custom = 'custQ[]=' + this.customQuestions.join("&custQ[]=");
    }
    var extra = '';
    if(this.extraQuestions.hasOwnProperty("ext")) {
      var extParams = decodeURIComponent($.param(this.extraQuestions));
      extra = (custom.length) ? '&' + extParams : extParams;
    }
    if(custom.length || extra.length){
      this.queryParameters = '?' + custom + extra;
    }
    this.queryURL = '/smg-pop-up/' + this.purfNID + '/user-status' + this.queryParameters;
  }

  // This function calls the triggerAction function
  PURFManager.prototype.manageActions = function(data, extra) {
    var extra = (typeof extra !== 'undefined') ? extra : false;

    var userData = {};
    for (var key in data) {
      if(key != "custom" && key != "extra" && key != "missing_required" && key != "missing_required_custom") {
        userData[key] = data[key];
      }
    }

    if(extra && data.hasOwnProperty("extra")) {
      var dataMainPURF = data;
      data = data.extra[this.key];
      for (var key in userData) {
        data[key] = userData[key];
      }
    }

    // Check if the reader_token is set
    if( !data["reader_token"] ) {
      this.header = this.headerNew;
      if(!extra)
        PURFManager.prototype.header = this.headerNew;
      // Is the Silverpop user id provided as a URL parameter?
      if(this.userID){
        // Get the user's full name, will be used in the layout defined at the bottom of this document
        spFullName = data["sp_full_name"];
        // Are there any missing "required" questions (i.e. standard lead gen questions)?  If so, the data.missing_required property will be non-empty.
        // This includes dependencee fields that are "visible".
        if(typeof data["missing_required"] != 'undefined'){
          // The smg_pop_up_missing_required cookie is used on the server in smg_pop_up_form_alter
          $.cookie('smg_pop_up_missing_required', 1, {expires: date, path: '/', domain: documentDomain});
          this.triggerAction(this.twoColumn, this.popType, false, false);
        }
        else {
          // Auto-submit IF there are no unanswered Custom questions
          if($(data.custom.unanswered).length == 0){
            $.cookie('spUserIDTrue', 1, {expires: date, path: '/', domain: documentDomain}); // #TODO possibly delete
            this.triggerAction('silverpopSubmit', this.popType, this.userID, false);
          }
          else {
            // #TODO combine the conditionals below
            if($(data.custom.unanswered).length > 5){
              //triggerAction(twoColumn, popType);
              this.triggerAction(this.oneColumn, this.popType, false, false);
            }
            else {
              layoutType = 'one-column-layout'; // used in the Drupal.theme definition below (added to modal's css classes)
             this.triggerAction(this.oneColumn, this.popType, false, false);
            }
          }
        }
      }
      // reader_token not set, and there is no spuid in the url
      else {
        this.triggerAction(this.twoColumn, this.popType, false, false);
      }
    }
    // The else if below is true if the reader_token IS SET AND (EITHER there are unanswered custom question OR unanswered standard-lead gen questions)
    //else if ( data["reader_token"] && ( (!data.custom.complete && $(data.custom.unanswered).length != 0) || (typeof data["missing_required"] != 'undefined') ) ){
    else if (data["reader_token"] && (data.hasOwnProperty('missing_required') || data.hasOwnProperty('missing_required_custom'))){
      this.header = this.headerExist;
      if(!extra)
        PURFManager.prototype.header = this.header;
      if(data.hasOwnProperty('missing_required')){  // If one or more standard lead-gen questions are missing, we always show two column form
        layoutType = 'two-column-layout';
        this.triggerAction(this.twoColumn, this.popType, false, false);
      }
      else if (data.hasOwnProperty('missing_required_custom')) {
        //layoutType = 'one-column-layout'; // used in the Drupal.theme definition below (added to modal's css classes)
        this.triggerAction(this.oneColumn, this.popType, false, false);
      }
    }
    else {
      var token = data["reader_token_value"];
      this.triggerAction('readerTokenSubmit', this.popType, token, false);
    }
  }

  /**
   * This function implements the appropriate action based on the result of user-status query
   *
   * @param mixed actionType
   *  - If we are auto-submitting, then this will either be readerTokenSubmit (when the reader_token is set) or silverpopSubmit (when spuid is in url)
   *  - If we are triggering a form pop-up, this will be either the the twoColumn jQuery object or the oneColumn jQuery object
   * @param string popType
   *  - The value should be either page_load, link_click or video_click.  In most cases, you can just pass in the popType variable that is set
   *    above.
   * @param string token
   *  - If we are auto submitting, then we should be passing in either the reader_token or the silverpop user Id.  Either ignore or set to false if not
   *    auto submitting
   * @param boolean override
   *  - Used to override the logic in hook_form_alter, to force the full form
   * @param Array extraPURFs
   *  - Either false if there is only one PURF on the page.  Or it contains an array of PURFManager objects
   */
  PURFManager.prototype.triggerAction = function(actionType, popType, token, override){

    // Set defaults for some parameters in case they weren't passed in, and other variables
    var token = typeof token != 'undefined' ? token : false;
    var override = typeof override != 'undefined' ? override : false;
    var submitType = (actionType == 'readerTokenSubmit' || actionType == 'silverpopSubmit') ? actionType : false;
    var columnType = false;
    var _this = this;
    // Check to see if actionType is the two/one column jQuery object.  If so, set the columnType
    // var to a string value.
    if(actionType instanceof jQuery){
      if(actionType.hasClass('ctools-modal-smg-pop-up-style-two-column') || actionType.hasClass('ctools-modal-smg-pop-up-640') || actionType.children().hasClass('ctools-modal-smg-pop-up-style-two-column') || actionType.children().hasClass('ctools-modal-smg-pop-up-640')){
        columnType = 'twoColumn';
        layoutType = 'two-column-layout';
      }
      else {
        columnType = 'oneColumn';
        layoutType = 'one-column-layout';
      }
    }

    if(popType == 'page_load'){
      if(submitType !== false){
        _this.autoSubmit(submitType, token);
      }
      else {
        actionType.trigger('click');
      }
    }
    else if (popType == 'link_click') {

      $.each(_this.purfSettings["pop_data"].inlineLinks, function(index, value){

        var uniqueId = index;
        var uniqueClass = '.purf-unique-' + uniqueId;
        // get the ad id
        var adId = value.adId;
        var valueLink = value;
        var uniqueURL = (value.url) ? value.protocol + value.url : false;
        var layoutTypeClass = 'ctools-modal-smg-pop-up-style-two-column';
        var layoutTypeClassRemove = 'ctools-modal-smg-pop-up-style-one-column';
        if(columnType){
          if(columnType == 'twoColumn'){
            if(actionType.parent("#two-column-640").length)
              layoutTypeClass = 'ctools-modal-smg-pop-up-640';
          }
          else if (columnType == 'oneColumn') {
            if(actionType.parent("#one-column-640").length){
              layoutTypeClass = 'ctools-modal-smg-pop-up-one-640';
              layoutTypeClassRemove = 'ctools-modal-smg-pop-up-640';
            } else {
              layoutTypeClass = 'ctools-modal-smg-pop-up-style-one-column';
              if(!overrideOn)
                layoutTypeClassRemove = 'ctools-modal-smg-pop-up-style-two-column';
            }
          }
        }
        $(uniqueClass)
          .addClass('ctools-use-modal-processed')
          .addClass('ctools-use-modal')
          .addClass(layoutTypeClass)
          .removeClass(layoutTypeClassRemove)
          .each(function(){

            // In case uniqueURL is false, create it using the href attribute of the PURF
            // enabled link.  uniqueURL usually won't be false, but sometimes it will, like
            // for OAS links - since we don't know the URL of the link until the page is rendered.
            if(!uniqueURL){
              uniqueURL = $(this).attr("href");
              valueLink.url = uniqueURL; // So we know where to redirect
            }

            if(submitType == false){

              // #2317 Remove the href attribute from inline link PURFs, so users can't
              // bypass the PURF.  The value of href is stored in
              // Drupal.settings["smg-pop-up-settings"]["pop_data"].inlineLinks[purfLinkUniqueId].url
              // Save the value of href before anything is done
              var storedHref = $(this).attr('href');
              $(this).removeAttr("href").hover(function(){
                $(this).css({'cursor':'pointer'});
              });
              var element_settings = {};
              // The href attribute of actionType is the link to the modal build callback
              var modalCallback = '/smg-register/nojs/' + _this.purfSettings.webform_nid + '/' + _this.purfSettings.leadworks_id + '/' + _this.purfSettings.referer_nid;
              element_settings.url = modalCallback + '/' + adId + '/' + uniqueId;
              // Add additional parameters to URL
              var callbackParams = {};
              if(_this.isExtra){
                callbackParams["extra"] = true;
                callbackParams["key"] = uniqueId;
              }
              if(override){
                callbackParams["override"] = true;
                $(this).off('click').on('click', function(){
                  overrideOn = true;
                  // Set the value as the inlineLinkInfo global param
                  jqxhr.abort();
                  PURFManager.prototype.inlineLinkInfo = {
                    "uniqueId" : index,
                    "value" : valueLink,
                  };
                  PURFGlobal.prototype.header = _this.header;
                });
              }
              else {
                $(this).off('click').on('click', function(){
                  if(layoutType == 'one-column-layout'){
                    $(this).removeClass('ctools-modal-smg-pop-up-style-two-column');
                  }
                  overrideOn = false;
                  // Set the value as the inlineLinkInfo global param
                  jqxhr.abort();
                  PURFManager.prototype.inlineLinkInfo = {
                    "uniqueId" : index,
                    "value" : valueLink,
                  };
                  PURFGlobal.prototype.header = _this.header;
                  if(_this.purfSettings.hasOwnProperty("disclaimer")){
                    PURFGlobal.prototype.disclaimer = _this.purfSettings.disclaimer;
                  }
                });
              }
              if(!isEmpty(callbackParams)){
                element_settings.url += '?' + decodeURIComponent( $.param(callbackParams) );
              }
              element_settings.progress = {type:"throbber"};
              element_settings.event = 'click';

              // Now we need to specify the base.  This should be an ID.  If one isn't
              // set for the current href, then we use the purf unique id
              if(this.id) {
                base = $(this).attr("id");
              }
              else {
                $(this).attr("id", uniqueId);
                base = uniqueId;
              }

              Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
            }
            else {
              var base = $(this).attr('href');
              if(Drupal.ajax.hasOwnProperty(base)){
                delete Drupal.ajax[base];
              }
              $(this).off('click').on('click', function(e){
                // Use all of jQuery's event override functions to be safe
                e.preventDefault();
                e.stopPropagation();
                e.stopImmediatePropagation();
                //autoSubmit(submitType, token, adId, uniqueURL);
                _this.autoSubmit(submitType, token, adId).success(function(){
                  window.location.href = uniqueURL;
                });
              });
            }
          });
      });
    }
    else if (popType == 'video_click'){
      var viddlerId = this.viddlerId;
      if(viddlerId){
        player = Viddler('viddlerOuter-' + viddlerId);

        // This will take care of HTML5 Fallback Flash player, which works differently from Iframe
        var playerStateTimer = setInterval(function(){
          if(typeof player.getPlayerState === 'function'){
            var playerState = player.getPlayerState();
            if(playerState == 3 || playerState == 1){
              if(!viddlerVerified){
                // If submitType is false, then we're not auto-submitting.  Therefore we prevent the video
                // from playing, and then trigger the appropriate form.
                if(submitType === false){
                  player.pauseVideo();
                  actionType.trigger('click');
                }
                else {
                  _this.autoSubmit(submitType, token).success(function(){
                    viddlerVerified = true;
                  });
                }
                clearInterval(playerStateTimer);
              }
            }
          }
        }, 500);

        // This takes care of when we're using HTML5 video player
        htmlVideo = $('.field-name-field-viddler-id video');
        htmlVideo.on('play', function(event){
          htmlVideoElement = htmlVideo.get(0);
          if(!viddlerVerified){
            // If submitType is false, then we're not auto-submitting.  Therefore we prevent the video
            // from playing, and then trigger the appropriate form.
            if(submitType === false){
              htmlVideoElement.pause();
              actionType.trigger('click');
            }
            else {
              _this.autoSubmit(submitType, token).success(function(){
                viddlerVerified = true;
              });
            }
          }
          else {
            return;
          }
        });

        // Add onStateChange event @see http://developers.viddler.com/documentation/chromeless/#toc-onstatechange
        // This will take care of Flash player when/if using an Iframe
        player.onStateChange(function(data)
        {
          if(data == 3 || data == 1){
            // Check if we've verified the lead already on this page
            if(!viddlerVerified){
              // If submitType is false, then we're not auto-submitting.  Therefore we prevent the video
              // from playing, and then trigger the appropriate form.
              if(submitType === false){
                player.pauseVideo();
                actionType.trigger('click');
              }
              else {
                _this.autoSubmit(submitType, token).success(function(){
                  viddlerVerified = true;
                });
              }
            }
            else {
              return;
            }
          }
        });
      }
    }

  }

  PURFManager.prototype.autoSubmit = function(submitType, token, adId) {
    // Set a cookie that indicates we're autosubmitting.  We check to see if this cookie is set
    // in the playbook_fields.module before we call the playbook_fields_set_cookie function
    $.cookie('smg_pop_up_auto_submit', true, {expires: date, path: '/', domain: documentDomain});

    var submitUrlString = (submitType == 'readerTokenSubmit') ? 'reader_token' : 'silverpop';
    var nid = this.purfNID;
    // Get the nid of the referer node
    var refererNID = this.purfSettings["referer_nid"];
    var leadworks = this.purfSettings["leadworks_id"];
    var submitURL = '/smg-pop-up/auto-submit/' + token + '/' + nid + '/' + leadworks + '/' + refererNID + '/' + submitUrlString;

    // If popType is link_click, then don't automatically submit.  Just return the submitURL so we can attach it to an onclick event
    var params = {};
    if(this.popType == 'link_click' && typeof adId != 'undefined'){
      //return submitURL;
      //submitURL = submitURL + '?adId=' + adId;
      params["adId"] = adId;
    }
    if(this.isExtra){
      params["extra"] = true;
      params["key"] = PURFManager.prototype.inlineLinkInfo.uniqueId;
    }
    if(Object.getOwnPropertyNames(params).length !== 0){
      submitURL = submitURL + '?' + decodeURIComponent( $.param( params ) );
    }
    return $.ajax({
      url: submitURL,
    });
  }

  var purfObj = new PURFManager();

  $(document).ready(function(){

    documentDomain = document.domain;
    if(documentDomain.indexOf('.www') != -1){
      documentDomain = documentDomain.substring(1); // Will use this when setting cookies
    }

    // Delete the spUserID cookie right away
    $.cookie('spUserID', null, {path: '/', domain: documentDomain});
    $.cookie('smg_pop_up_missing_required', null, {path: '/', domain: documentDomain});
    $.cookie('smg_pop_up_auto_submit', null, {path: '/', domain: documentDomain});

    var mainContentWidth = $('body #page #main').width(); // Get the window width
    htmlWidth = $('html').width();
    if(htmlWidth < mainContentWidth){
      htmlWidth = mainContentWidth;
    }

    // Check to see if the 'smg-pop-up-settings' key is present in Drupal.settings.  If
    // it is, set some default values.
    // If it isn't then this page ONLY contains a separately added (or "extra") PURF.
    if(typeof Drupal.settings["smg-pop-up-settings"] !== 'undefined'){
      purfObj.addParameters(Drupal.settings["smg-pop-up-settings"].webform_nid, Drupal.settings["smg-pop-up-settings"].custom_questions);
      purfObj.purfSettings = Drupal.settings["smg-pop-up-settings"];

      // Get the jQuery objects for the two-column and one-column ctools-modal links
      var twoColumnFull = $('#smg-pop-up-hidden-links #two-column a.ctools-use-modal');
      var oneColumnFull = $('#smg-pop-up-hidden-links #one-column a.ctools-use-modal');
      var twoColumn640 = $('#smg-pop-up-hidden-links #two-column-640 a.ctools-use-modal');
      var oneColumn640 = $('#smg-pop-up-hidden-links #one-column-640 a.ctools-use-modal');

      popType = Drupal.settings["smg-pop-up-settings"].pop_type;
      purfObj.popType = popType;

      // Check for Viddler Id in the settings
      purfObj.viddlerId = (typeof Drupal.settings["smg-pop-up-settings"]["viddler_id"] !== 'undefined') ? Drupal.settings["smg-pop-up-settings"]["viddler_id"] : false;

      if(!Drupal.settings["smg-pop-up-settings"].referredUser){
        purfObj.purfSettings.referredUser = false;
      }

      PURFGlobal.prototype.header = Drupal.settings["smg-pop-up-settings"]["header_new_user"]; // Header New
      PURFGlobal.prototype.headerNew = Drupal.settings["smg-pop-up-settings"]["header_new_user"]; // Header New
      PURFGlobal.prototype.headerExist = Drupal.settings["smg-pop-up-settings"]["header_exist_user"];

      if(!Drupal.settings["smg-pop-up-settings"].disclaimer) {
        PURFGlobal.prototype.disclaimer = Drupal.settings["smg-pop-up-settings"].disclaimer;
      }
    }
    else {
      var ctoolsModalLinkFull = $('<a></a>').addClass('ctools-use-modal').addClass('ctools-modal-smg-pop-up-style-two-column');
      var ctoolsModalLinkFullOne = $('<a></a>').addClass('ctools-use-modal').addClass('ctools-modal-smg-pop-up-640');
      var ctoolsModalLink = $('<a></a>').addClass('ctools-use-modal');
      var twoColumnFull = $('<div></div>').attr('id','two-column');
      var oneColumnFull = $('<div></div>').attr('id','one-column');
      var twoColumn640 = $('<div></div>').attr('id','two-column-640').append(ctoolsModalLink);
      var oneColumn640 = $('<div></div>').attr('id','one-column-640').append(ctoolsModalLink);
      twoColumnFull.append(ctoolsModalLinkFull);
      oneColumnFull.append(ctoolsModalLinkFullOne);
      twoColumn640.append(ctoolsModalLink);
      oneColumn640.append(ctoolsModalLink);
      popType = 'link_click';

      purfObj.purfSettings = {
        'referredUser': false,
      };
    }

    var twoColumn = twoColumnFull;
    var oneColumn = oneColumnFull;

    // If device width less than 640, change viewport so that minimum scale is 1.  Also, modify the ctools-modal link
    // objects declared above to point to the 640 width links
    var deviceWidth = $(window).width();
    if(deviceWidth < 640){
      viewport = document.querySelector("meta[name=viewport]");
      viewport.setAttribute('content', 'initial-scale=1.0, width=device-width, minimum-scale=1.0, maximum-scale=1.0, user-scalable=yes');
      twoColumn = twoColumn640;
      oneColumn = oneColumn640;
    }
    else if (deviceWidth < 770) {
      Drupal.settings["smg-pop-up-style-two-column"].modalSize.width = 0.9;
    }

    // Store twoColumn and oneColumn in global object
    PURFGlobal.prototype.twoColumn = twoColumn;
    PURFGlobal.prototype.oneColumn = oneColumn;

    // Look for the reader_token cookie.  If it isn't set, look for the silvepop user ID in the url
    var readerTokenCookie = $.cookie('reader_token');
    if(!readerTokenCookie){
      var query_string = {};
      var query = window.location.search.substring(1);
      var vars = query.split("&");
      for (var i=0; i < vars.length; i++) {
        var pair = vars[i].split("=");
        query_string[pair[0]] = pair[1];
      }
      if("spUserID" in query_string && !purfObj.purfSettings.referredUser){
        var userID = query_string["spUserID"];
        PURFManager.prototype.userID = userID;
        $.cookie('spUserID', userID, {expires: date, path: '/', domain: documentDomain});
      }
    }

    // #2345 Look for PURF links that point to a second (or third, fourth, ...) PURF
    var extraPURFs = false;
    if(Drupal.settings.hasOwnProperty("smg-pop-up-settings-extra")){
      var extraPurf = {"ext":{}};
      for (var key in Drupal.settings["smg-pop-up-settings-extra"]) {
        var obj = Drupal.settings["smg-pop-up-settings-extra"][key];
        extraPurf["ext"][obj.webform_nid] = {
          "key" : key,
          "nid" : obj.webform_nid,
          "cust" : obj.custom_questions,
        };
      }

      purfObj.extraQuestions = extraPurf;

      extraPURFs = [];
      $.each(Drupal.settings["smg-pop-up-settings-extra"], function(index, value){
        extraPURFs[index] = new PURFManager();
        extraPURFs[index].header = value["header_new_user"];
        extraPURFs[index].headerNew = value["header_new_user"];
        extraPURFs[index].headerExist = value["header_exist_user"];
        extraPURFs[index].purfSettings = value;
        extraPURFs[index].purfNID = value.webform_nid;
        extraPURFs[index].popType = 'link_click';
        extraPURFs[index].key = index;
        extraPURFs[index].isExtra = true;
      });
    }

    purfObj.createQueryURL();

    var showPURF = true;

    // Below, we account for situations where the user clicks a PURF link BEFORE the
    // user-status AJAX call has returned.  So we show the full form by default, unless
    // the user-status call returns and overrides.
    if(popType == 'link_click'){
      if( !$('.ctools-use-modal').length ){
        showPURF = false;
      }
      else {
        header = purfObj.header;
        purfObj.triggerAction(purfObj.twoColumn, purfObj.popType, false, true);
      }
    }
    if(extraPURFs) {
      for (var key in extraPURFs) {
        extraPURFs[key].triggerAction(extraPURFs[key].twoColumn, extraPURFs[key].popType, false, true);
      }
    }

    if ( showPURF ) {
      jqxhr = $.getJSON( purfObj.queryURL, function( data ) {

        Drupal.settings.purfUserStatus = data;

        if(Drupal.settings.hasOwnProperty("smg-pop-up-settings")) {
          purfObj.manageActions(data);
        }

        if(extraPURFs) {
          for (var key in extraPURFs) {
            if (extraPURFs.hasOwnProperty(key)) {
              extraPURFs[key].manageActions(data, true);
            }
          }
        }
      });
    }
  });

  // All of the Drupal.ajax.prototype.commands.*** functions below are called in smg_register_modal_build in smg_pop_up.module

  /**
   * The runValidationDependencyJS function will be added to the Drupal.ajax namespace.  It will be used as an ajax callback that runs the javascript
   * validation and dependency code.  This code must be supplied as a string (it will be attached to the response object), and it must be a set
   * of executable javascript/jquery expressions i.e. - $('#id').doSomething(); $('#id2').doSomethingElse();
   * To understand why I am running the validation/dependency javascript code in an ajax callback function, please read:
   * http:*www.jaypan.com/tutorial/calling-function-after-ajax-event-drupal-7.
   * To sum it up: normally on webforms on our sites, we implement the jQuery UI Multiselect code and our custom validation javascript by
   * adding it to the page with jQuery(document).ready().  This won't work for modal displays because they're rendered with an ajax callback,
   * hence after the document loads.  So I need to execute our javascript after the modal renders, by adding the function below as an ajax
   * callback, which I'm doing in the smg_register_modal_build function in smg_pop_up.module
   */
  Drupal.ajax.prototype.commands.runValidationDependencyJS = function(ajax, response, status) {
    var js = response.js;
    // Since the javascript expressions are provided as a string, I need to use special syntax to make them executable.  Please refer to:
    // http://stackoverflow.com/questions/1271516/executing-anonymous-functions-created-using-javascript-eval
    // http://stackoverflow.com/questions/939326/execute-javascript-code-stored-as-a-string
    var func = new Function('$', js);
    func(jQuery);
  }

  // When the modal closes, IF the modal was generated by a link-click (as opossed to on page load), we redirect to the original href destination
  Drupal.ajax.prototype.commands.smgPopRedirect = function(ajax, response, status){

    //var purfLinkUniqueId = response.purfLinkUniqueId;
    var purfLinkUniqueId = PURFManager.prototype.inlineLinkInfo.index
    //var linkObject = Drupal.settings["smg-pop-up-settings"]["pop_data"].inlineLinks[purfLinkUniqueId];
    var linkObject = PURFManager.prototype.inlineLinkInfo.value;
    var protocol = (linkObject.hasOwnProperty('protocol') && linkObject.protocol.length > 1) ? linkObject.protocol : '';
    var linkObjURL = (linkObject.url.indexOf('summitmediagroup.com') >= 0 && linkObject.url.indexOf('www') === -1) ? ('www.' + linkObject.url) : linkObject.url;
    var originalLink = protocol + linkObjURL;
    window.location.href = originalLink;
  }

  // When the modal loads, look for the jQuery UI Multiselect widgets and move them into the ctools modal.  Also, we dynamically modify the width
  // of the dropdown, and it's x,y pos
  Drupal.ajax.prototype.commands.moveMultiWidgets = function(ajax, response, status) {
    $('body > .ui-multiselect-menu').each(function(){
      $(this).appendTo('#modalContent #modal-content');
    });
  }

  // When the modal closes, this function cleans up cookies, and if popType is video_click, it plays the viddler video
  Drupal.ajax.prototype.commands.smgCloseModal = function(ajax, response, status) {
    $.cookie('spUserID', null, {path: '/', domain: documentDomain});
    $.cookie('smg_pop_up_missing_required', null, {path: '/', domain: documentDomain});
    if(popType == 'video_click'){
      var videoOffset = ($('div.field-name-field-viddler-id').length) ? $('div.field-name-field-viddler-id').offset().top : 40;
      $("html, body").animate({ scrollTop: (videoOffset - 150) }, 800);
      if(player){
        try{
          player.playVideo();
        }
        catch(err) {

        }
      }
      if(htmlVideo){
        if(typeof htmlVideoElement.play === 'function'){
          htmlVideoElement.play();
        }
      }
      viddlerVerified = true;
    }
  }

  Drupal.ajax.prototype.commands.modifyModalDimensions = function(ajax, response, status){

    // Perform some manipulations to the font-size to try to get a consistent styling between sites
    var bodyFontSize = $('body').css("font-size");
    if(bodyFontSize.indexOf("px") != -1){
      var bodyFontSizeInt = Number(bodyFontSize.substring(0, bodyFontSize.indexOf("px")));
      if(bodyFontSizeInt == 15){
        $('body').css({'font-size':'100%'});
        $('#modalContent').css({'font-size':'75%'});
      }
    }

    var formHeight = $('#modal-content .webform-client-form').height();
    $('#modal-content').css({'min-height':formHeight + 28});
    var headerHeight = $('.modal-header').height();
    if(formHeight < ( $(window).height() * .72 ) && layoutType != 'two-column-layout'){
      //$('div.ctools-modal-content .modal-content').css({'height' : formHeight + 20 + 'px'});
      //$('div.ctools-modal-content').css({'height' : formHeight + headerHeight + 60 + 'px'});
      $('div.ctools-modal-content .modal-content').css({'height' : 'auto'});
      $('div.ctools-modal-content').css({'height' : 'auto', 'padding-bottom' : '13px'});
    }

    // Get the offset position of the modal
    var modalTop = $('#modalContent').offset().top;
    $(window).on('resize', function(e){
      $('#modalContent').css({'top':modalTop + 'px'});
    });

    $('.node-360-package-spin-rotate').each(function(){
      //$(this).('iframe #myFlash').css({'z-index':'-1'});
    });

    if(popType == 'video_click' || popType == 'link_click'){

      $("html, body").animate({ scrollTop: 0 }, 100);
      //$('#modalContent').css({'position':'fixed', 'top':'40px'});
      $('#modalContent').animate({top:'40px'}, 100);
    }

    if(htmlWidth < 1030 && htmlWidth > 640){
      var resizeModalNewWidth = 0.9 * htmlWidth;
      $('#modalContent').css({'font-size':'100%', 'left':(htmlWidth * 0.05)});
      setTimeout(function(){
        $('.ctools-modal-content').animate({ width:resizeModalNewWidth}, 1200);
      },1000);
    }

    // Set the modalBackdrop height to 100% of the page
    var docHeight = $(document).height();
    setTimeout(function(){
      $('#modalBackdrop').css({'height':docHeight});
    }, 900);
    setTimeout(function(){
      var timeoutDocHeight = docHeight = $(document).height();
      $('#modalBackdrop').css({'height':timeoutDocHeight});
    }, 1900);

  }

  // When the modal loads we dynamically modify the width of the multiselect widget dropdown, as well as its x,y position
  Drupal.behaviors.modifyModalElementDimensions = {
    attach: function(context, settings) {

      var multiSelectIsOpen = false;

      $('#modalContent').on('load', function(){
        if(popType == 'video_click'){
          $("html, body").animate({ scrollTop: 0 }, "slow");
          $('#modalContent').css({'top':'40px'});
        }
      });

      var windowWidth = $(window).width(); // Get the window width
      var htmlWidth = $('html').width();
      if(htmlWidth < windowWidth){
        htmlWidth.css({'width':windowWidth});
      }
      var moveRequired = function(){
        // Move the required-field text above the submit button (i.e. "* Indicates a required field").  This either has
        // done in javascript, or at some point in the form rendering process (or in the form template).  I'm doing it
        // in js now b/c it's easier and quicker
        var requiredText = $('#smg-pop-up-required-note');
        //$('.webform-client-form .form-actions').before($('#smg-pop-up-required-note'));
        if(layoutType == 'two-column-layout' && windowWidth > 640){
          $('#smg-pop-up-required-note').css({'display':'inline', 'position':'absolute'});
          if(($('.webform-client-form .form-actions').find('#smg-pop-up-required-note').length)){
            //$('.webform-client-form .form-actions #smg-pop-up-required-note').remove();
            $('.webform-client-form .form-actions').prepend($('#smg-pop-up-required-note'));
            $('div.ctools-modal-content .modal-content #smg-pop-up-required-note').css({'display':'inline', 'position':'absolute'});
            $('.webform-client-form > div').not('.form-actions').children('#smg-pop-up-required-note').remove();
          } else {
            $('.webform-client-form .form-actions').prepend($('#smg-pop-up-required-note'));
            $('div.ctools-modal-content .modal-content #smg-pop-up-required-note').css({'display':'inline', 'position':'absolute'});
            $('.webform-client-form > #smg-pop-up-required-note').remove();
          }
        }
      }
      moveRequired();

      $(document).ready(function(){
        setTimeout(moveRequired,500);
      });

      var modalHeaderHeight, modalScrollHeight;

      var modifyModalHeight = function(addHeight){
        var additionalHeight = (typeof addHeight != "undefined") ? addHeight : 0;

        // If one-column-layout make sure it's at least as long as its contents
        if(layoutType == 'one-column-layout'){
          // One more check to make sure everything fits
          var innerContentsHeight = $('.modal-header').outerHeight() + $('.modal-scroll').outerHeight() + $('#modalContent .disclaimer').outerHeight();
          var newModalHeight = $('#modalContent').height();
          if(innerContentsHeight > newModalHeight){
            $('#modalContent, .ctools-modal-content').css({'height':innerContentsHeight + 5 + 'px'});
          }
        }

        var modalHeight = $('.ctools-modal-content').height();
        var formHeight = $('.webform-client-form').height();
        $('#modal-content').css({'min-height':formHeight + 28});
        if(windowWidth > 640 && layoutType != 'one-column-layout'){
          $('.ctools-modal-content').css({'height':'auto','padding-bottom':'15px'});
          $('#modal-content').css({'height':'auto','padding-bottom':'15px'});
        }

        if(windowWidth > 640){
          if(layoutType != 'one-column-layout'){
            if($('.ctools-modal-content > .clear').length == 0){
              $('.ctools-modal-content').append('<div class="clear"></div>');
              $('.ctools-modal-content .clear').css({'clear':'both'});
            }
            if($('#modal-content > .clear').length == 0){
              $('#modal-content').append('<div class="clear"></div>');
              $('#modal-content .clear').css({'clear':'both'});
            }
            if($('.popups-container > .clear').length == 0){
              $('.popups-container').append('<div class="clear"></div>');
              $('.popups-container .clear').css({'clear':'both'});
            }
            $('.ctools-modal-content, #modal-content').css({'overflow':'visible', 'overflow-y':'visible'});
          }
        }


        if( ($('#modalContent').height() - 150) < $(document).height()){
          var newBodyHeight = $('#modalContent').height() + 300;
          $('body, #modalBackdrop').css({'height': newBodyHeight + 'px'});
        }

      }
      setTimeout(modifyModalHeight,1000);

      $(window).on('resize', function(){
        modifyModalHeight();
      });

      // Function for determining if browser is IE and if so what version, used below in
      // msieChangeWidth to make some css adjustments
      function msieversion(){
        var ua = window.navigator.userAgent;
         var msie = ua.indexOf("MSIE ");
         if (msie > 0)      // If Internet Explorer, return version number
            return parseInt(ua.substring(msie + 5, ua.indexOf(".", msie)));
         else                 // If another browser, return 0
         return false;
      }

      msie = msieversion();
      msie9 = false;
      msie8 = false;
      if(msie && msie == 9){
        msie9 = true;
      } else if (msie == 8) {
        msie8 = true;
      }

      // If the webform returns a validation error, call the modifyModalHeight function again because
      // that adds to the height of the form.  If msie = 8, add extra height
      if($('#modal-content').find('.messages.error').length){
        if(!msie8){
          modifyModalHeight(45);
        } else {
          var ie8FormHeight = $('#modalContent .webform-client-form').height();
          $('#modalContent .webform-client-form').css({'height':ie8FormHeight + 45 + 'px'});
          setTimeout(function(){
            modifyModalHeight(80);
          },2000);
        }
      }

      var msieChangeWidth = function () {
        if(msie9 || msie8){
          if(windowWidth > 640 && layoutType == 'two-column-layout'){
            var newWidth = windowWidth * .64;
            newWidth = Math.round(newWidth);
            $('#modalContent').css({'width': newWidth + 'px'});
            $('#modalContent .smg-pop-up-column').css({'width':'50%'});
            var newLeft = (windowWidth - newWidth) / 2;
            newLeft = Math.round(newLeft);
            $('#modalContent').css({'position':'absolute','left': newLeft + 'px'});
          } else if (windowWidth > 640 && layoutType == 'one-column-layout'){
            var newWidth = windowWidth * .38;
            newWidth = Math.round(newWidth);
            $('#modalContent').css({'width': newWidth + 'px'});

            var newLeft = (windowWidth - newWidth) / 2;
            newLeft = Math.round(newLeft);
            $('#modalContent').css({'position':'absolute','left': newLeft + 'px'});
          }
        }
      }

      msieChangeWidth();

      var modifyWidgets = function(){
        /*var countryButton = $('.smg-pop-up-element.select #webform-component-country select').multiselect('widget');
        var buttonUL = countryButton.find('ul.ui-multiselect-checkboxes');
        countryButton.css({'overflow':'hidden','max-height':'200px'});
        buttonUL.css({'overflow':'scroll','max-height':'200px'});*/

        $('.smg-pop-up-element.select select', context).each(function () {
          // Get the multiselect button for this select element using multiselect method (http://www.erichynds.com/blog/jquery-ui-multiselect-widget)
          var button = $(this).multiselect('getButton');

          // Below, we modify the position of the menu relative to the button.  The jQuery UI Multiselect library
          // has its own function that I use directly below (ie multiselect("option","position", {...})).  However, I
          // also repeat (redo) the operation using jQuery's native position() function.  On a desktop browser, the Multiselect
          // function is sufficient.  However, to get it to work on mobile, I needed to use both the multiselect fct and
          // jQuery's position fct (not sure why, but it doesn't seem to affect performance in any way)
          $(this).multiselect("option","position", {my:"left top", at:"left bottom", of:button, collision:"none"});
          var widget = $(this).multiselect('widget');
          var htmlOverflowDefault = $('html').css('overflow');
          // Set some variables @todo maybe delete
          var scrollTop = $(window).scrollTop(),
              elementOffset = ($('#modalContent').length) ? $('#modalContent').offset().top : 0,
              distance = (elementOffset - scrollTop);

          widget.position({my:"left top", at:"left bottom", of:button, collision:"none"});
          var buttonWidth = button.width();
          // set the width of the widget
          $(this).multiselect('widget').width(buttonWidth);

          widget.on("multiselectbeforeopen", function(event, ui){
            // Even though we set the width of the multiselect popup above, that didn't account
            // for questions that are shown conditionally (i.e. the buttons have 0 width when the
            // document intially loads) so we set the width again just before the multiselect popup
            // opens.
            var buttonBeforeOpenWidth = $(this).multiselect('getButton').width();
            $(this).multiselect('widget').width(buttonBeforeOpenWidth);
          });

          widget.on("multiselectopen", function(event, ui){
            scrollTop = $(window).scrollTop(),
            elementOffset = ($('#modalContent').length) ? $('#modalContent').offset().top : 0,
            distance = (elementOffset - scrollTop);
            currentWidget = $(this);
            if(!$(this).hasClass('ui-multiselect-menu')){
              try{
                currentWidget = $(this).multiselect('widget');
                currentWidgetUl = currentWidget.children("ul");
              }
              catch (err){

              }
            }
            var widgetOffset = currentWidget.offset().top;
            var widgetDistanceFromTop = (widgetOffset - scrollTop);
            var bottomOfVisibleWindow = $(window).height();
            var widgetDistanceToBottom = (bottomOfVisibleWindow - widgetDistanceFromTop);
            currentWidgetUl.css({'max-height':widgetDistanceToBottom - 15});
            multiSelectIsOpen = true;
            $('#modalContent').css({'position':'fixed', 'top':distance});

            $('body').css({'overflow':'hidden'});
            $('html').css({'overflow':'hidden'});
          });
          widget.on("multiselectclose", function(event, ui){
            multiSelectIsOpen = false;
            $('#modalContent').css({'position':'absolute', 'top':'40px'});
            $('body').css({'overflow':'visible'});
            $('html').css({'overflow':htmlOverflowDefault});
          });
        });
      }
      modifyWidgets();
      setTimeout(modifyWidgets,500);
      setTimeout(modifyWidgets,1000);

      var absoluteModalPosY = function(){
        $('#modalContent').css({'position':'absolute','top':'40px'});
      }

      var absoluteModalPosX = function(){
        var windowWidth = $(window).width();
        var modalContentWidth = $('#modalContent').width();
        var leftPos = ((windowWidth - modalContentWidth) / 2);
        $('#modalContent').css({'left':leftPos});
      }

      $(window).load(function(){

        modifyWidgets();

        // To be safe, I call the modifyWidgets fct several times
        setTimeout(modifyWidgets, 2000);
        setTimeout(modifyWidgets, 5000);
        setTimeout(modifyWidgets, 15000);
        setTimeout(modifyWidgets, 25000);
        setTimeout(modifyWidgets, 45000);

        var labelSize = function(){
          if($(window).width() < 640){
            $('div.ctools-modal-content .modal-content .form-item.webform-component > label, div.ctools-modal-content .modal-content .form-item.webform-component > label .form-required').each(function(){
              $(this, context).css('font-size','12px');
              $(this, context).css('-webkit-text-size-adjust','12px');
            });
          }
        }

        labelSize();
        setTimeout(labelSize, 3000);
        setTimeout(labelSize, 8000);
        setTimeout(labelSize, 11000);

        $(window).on("orientationchange", function(event){

          var deviceWidth = $(window).width();
          if(deviceWidth < 640){
            viewport = document.querySelector("meta[name=viewport]");
            viewport.setAttribute('content', 'initial-scale=1.0, width=device-width, minimum-scale=1.0, maximum-scale=1.0, user-scalable=yes');
          }

          modifyWidgets();

          labelSize();

          if(window.orientation == 90){
            setTimeout(function(){
              var modalHeight = $('#modal-content').outerHeight();
              if($('#modal-content').height() != modalHeight + 25){
                $('#modal-content').css({'height':modalHeight + 25 + 'px'});
              }
            }, 500);
          } else {
            $(window).scrollLeft(0);
          }
        });

        var setAbsoluteModalPosY;
        var setAbsoluteModalPosX;
        $(window).on('resize', function(e){
          // Make sure the modal is in absolute pos at 40px
          $('#modalContent').css({'position':'absolute','top':'40px'});
          clearTimeout(setAbsoluteModalPosY);
          clearTimeout(setAbsoluteModalPosX);
          setAbsoluteModalPosY = setTimeout(absoluteModalPosY, 5);
          //setAbsoluteModalPosX = setTimeout(absoluteModalPosX, 100);

          modifyWidgets();
          labelSize();
          msieChangeWidth();

          if(window.orientation == 90){
            setTimeout(function(){
              var modalHeight = $('#modal-content').outerHeight();
              if($('#modal-content').height() != modalHeight + 20){
                $('#modal-content').css({'height':modalHeight + 20 + 'px'});
              }
            }, 500);
          }
          var modalOuterWidth = $('.ctools-modal-content').outerWidth();
          if(modalOuterWidth < 800 && modalOuterWidth > 500){
            var modalHeightResize = $('#modal-content').outerHeight();
            $('#modal-content').css({'height':modalHeightResize - 5 + 'px'});
          }
        });

      }); // end $(window).load

    },
  };

  Drupal.behaviors.modifyModalDimensions = {
    attach: function(context, settings) {
      $(window).load(function(){
        setTimeout(function(){
          //var formHeight = $('#modal-content .webform-client-form').height();
          //console.log(formHeight);
          //$('#modal-content').height(formHeight + 28);
        }, 50);
      });
    },
  };

  /**
   * Provide the HTML to create the modal dialog
   */
  Drupal.theme.prototype.smg_pop_up_modal = function() {

    if(PURFGlobal.prototype.header){
      headerHTML = '<span class="modal-header-text">' + PURFGlobal.prototype.header + '</span>';
    }

    var disclaimer = PURFGlobal.prototype.disclaimer;
    var disclaimerHTML = '';
    if(disclaimer){
      disclaimerHTML = '<div class="disclaimer"><span class="disclaimer-text">This content is sponsored by the supplier.  Your contact information may be shared with this sponsor, as detailed in our Privacy Policy.  Your contact information will not be shared with a sponsor whose content you have not reviewed.</span></div>';
    }

    if(overrideOn){
      layoutType = 'two-column-layout';
    }

    var fullNameHTML = '';
    if(spFullName){

      $(document.body).on('click', 'a#not-sp-user', function(e) {
        e.preventDefault();
        Drupal.CTools.Modal.modal_dismiss();
        // get cookie value
        $.cookie('spUserID', null, {path: '/', domain: documentDomain});
        var url = location.protocol + '//' + location.host + location.pathname;
        window.location = url;
      });

      var link = $("<a />", {
        id : "not-sp-user",
        name : "not-sp-user",
        href : "#",
        text : "Click here.",
      });
      linkString = link.wrapAll('<div>').parent().html();
      fullNameHTML = '<div id="sp-full-name-wrapper"><span id="sp-full-name">Not ' + spFullName +'?  ' + linkString + '</span></div>';

    }

    var windowWidth = $(window).width();
    if(windowWidth >= 640){
      windowWidthClass = 'min-640';
    } else {
      windowWidthClass = 'max-640';
    }

    var html = '';
    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div class="ctools-modal-content ctools-sample-modal-content ' + layoutType + ' ' + windowWidthClass + '">';
    html += '    <div class="popups-container">';
    html += '      <div class="modal-header popups-title">';
    html += '        <span id="modal-title" class="modal-title"></span>' + headerHTML + fullNameHTML;
    html += '      </div>';
    html += '      <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
    html += '    </div>';
    html += disclaimerHTML;
    html += '  </div>';
    return html;
  }

})(jQuery, Drupal, this, this.document);
