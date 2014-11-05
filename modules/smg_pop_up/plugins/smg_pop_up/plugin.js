(function($){

  $(document).ready(function(){
    // See if the purfData is already stored in the value attribute of the data field
    var purfDataJson = $('.field-name-field-smg-pop-data input').val();
    if(typeof purfDataJson != 'undefined' && purfDataJson.length){
      var purfData = JSON.parse(purfDataJson);
      if('inlineLinks' in purfData){
        Drupal.settings.purfData = purfData;
      } else {
        Drupal.settings.purfData = {'inlineLinks':{}};
      }
    } else {
      Drupal.settings.purfData = {'inlineLinks':{}};
    }
  });

  var makeRandomString = function(){
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for( var i=0; i < 5; i++ )
      text += possible.charAt(Math.floor(Math.random() * possible.length));
    return text;
  }

  // Modify our object that holds data about the inline links
  var modifyPurfInlineLinks = function(data, adId, uniqueId ,oldAdId){
    var uniqueId = typeof uniqueId !== 'undefined' ? uniqueId : false;
    var oldAdId = typeof oldAdId !== 'undefined' ? oldAdId : false;
    if(adId.length == 0 && oldAdId != false) {
      // If this condition is true, then the ad id field was deleted
      delete Drupal.settings.purfData.inlineLinks[uniqueId];
    } else if (adId.length != 0) {

      if(oldAdId != false){
        if(typeof Drupal.settings.purfData.inlineLinks[uniqueId] != 'undefined')
          delete Drupal.settings.purfData.inlineLinks[uniqueId];
      }

      var linksObject = {
        adId: adId,
        protocol: data.url.protocol,
        url: data.url.url,
      };
      Drupal.settings.purfData.inlineLinks[uniqueId] = linksObject;
    }
    var jsonPurf = JSON.stringify(Drupal.settings.purfData);
    //jsonPurf = jsonPurf.replace(/"/g, "'");
    $('.field-name-field-smg-pop-data input').val(jsonPurf);
  }

  CKEDITOR.plugins.add('smgPopUpPlugin', {
    init: function(editor, pluginPath) {
      CKEDITOR.on('dialogDefinition', function(e){
        if((e.editor != editor) || (e.data.name != 'link')) return;
        // Overrides definition.
        var definition = e.data.definition;

        definition.onOk = CKEDITOR.tools.override(definition.onOk, function(original){
          return function(){
            original.call(this);
          }
        });

        adTabDefinition = {
          id: 'purf_ad',
          title: 'PURF Ad',
          label: 'PURF Ad',
          elements: [{
            type: 'vbox',
            //widths: ['50%', '50%'],

            children: [/*{
              type: 'checkbox',
              id: 'is_purf',
              label: 'Would you like to apply a Pop Up Registration Form to this link?',
            }, */{
              type: 'text',
              id: 'purf_ad_id',
              label: 'Would you like to apply a PURF to this link?  If so, please specify the Ad ID.  To remove the PURF, just delete the Ad ID.',
              controlStyle: 'width: 52px',
              setup: function(data) {
                if(typeof data.adv != 'undefined'){
                  var advSetup = 'adv';
                }
                else if (typeof data.advanced != 'undefined') {
                  var advSetup = 'advanced';
                }
                if(typeof data[advSetup] != 'undefined' && typeof data[advSetup].advCSSClasses != 'undefined'){
                  var cssClasses = data[advSetup].advCSSClasses;
                  if(cssClasses.indexOf('purf-ad-') != -1){
                    var pattern = /purf-ad-[0-9]*/g;
                    var searchResult = cssClasses.match(pattern)[0];
                    var adID = searchResult.substring(8);
                    this.setValue(adID);
                  }
                }
              },
              commit: function(data) {
              	var purfClass = 'purf-ad-' + this.getValue();
                var dialog = this.getDialog();
                if(typeof data.adv != 'undefined'){
                  var advanced = 'adv';
                }
                else if (typeof data.advanced != 'undefined') {
                  var advanced = 'advanced';
                }
                if(typeof data[advanced] != 'undefined' && typeof data[advanced].advCSSClasses != 'undefined'){
                  var cssClasses = data[advanced].advCSSClasses;
                } else {
                  data[advanced].advCSSClasses = "";
                  var cssClasses = data[advanced].advCSSClasses;
                }
                // If purf class exists, but for different ad id, then overwrite
                if((classIndex = cssClasses.indexOf('purf-ad-')) != -1){

                  // search for the purf-ad class
                  var pattern = /purf-ad-[0-9]*/g;
                  var searchResult = cssClasses.match(pattern)[0];
                  var oldAdID = searchResult.substring(8);

                  // search for the unique-id
                  var uniquePattern = /purf-unique-[0-9A-Za-z]*/g;
                  var uniqueSearchResult = cssClasses.match(uniquePattern)[0];
                  var uniqueId = uniqueSearchResult.substring(12);

                  if(oldAdID == this.getValue()){
                    // In case unique ID wasn't found, create one and add the class
                    if(uniqueId == null && this.getValue().length != 0){
                      uniqueId = makeRandomString();
                      var uniqueClass = 'purf-unique-' + uniqueId;
                      updatedCssClasses.concat(" " + uniqueClass);
                    }
                    modifyPurfInlineLinks(data, this.getValue(), uniqueId, oldAdID);
                  }
                  if(oldAdID != this.getValue()){
                    // below, if this ad-id is empty, remove the purf-ad-* and perf-unique-* classes
                    var updatedCssClasses = (this.getValue().length == 0) ? cssClasses.replace(searchResult, "").replace(uniqueSearchResult, "") : cssClasses.replace(searchResult, purfClass);
                    updatedCssClasses = updatedCssClasses.replace(/\s+/g, ' ').trim();
                    // In case unique ID wasn't found, create one and add the class
                    if(uniqueId == null && this.getValue().length != 0){
                      uniqueId = makeRandomString();
                      var uniqueClass = 'purf-unique-' + uniqueId;
                      updatedCssClasses.concat(" " + uniqueClass);
                    }
                    data[advanced].advCSSClasses = updatedCssClasses;
                    modifyPurfInlineLinks(data, this.getValue(), uniqueId ,oldAdID);
                    return;
                  }
                } else {
                  // the purf class doesn't exist yet, and it isn't being set this time either, so we return
                  if(this.getValue().length == 0) {
                    return;
                  // we're creating the purf for the first time
                  } else {
                    // generate a random string to be used for the unique id
                    var uniqueId = makeRandomString();
                    var uniqueClass = 'purf-unique-' + uniqueId;
                    // add the uniqueClass to the purfClass string
                    purfClass += ' ' + uniqueClass;
                    var updatedCssClasses = (cssClasses.length > 1) ? cssClasses.concat(" " + purfClass) : cssClasses.concat(purfClass);
                    data[advanced].advCSSClasses = updatedCssClasses;
                    modifyPurfInlineLinks(data, this.getValue(), uniqueId);
                  }
                }
              },
            }]
          }]
        };
        definition.addContents(adTabDefinition);
        adTab = definition.getContents('purf_ad');
        Drupal.settings.def = definition;

      });
    }
  });
})(jQuery);