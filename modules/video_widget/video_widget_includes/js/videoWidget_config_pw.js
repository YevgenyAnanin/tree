/**
 * Brand specific configuration settings for the video widget
 *
 * @author Yevgeny Ananin <yananin@summitmediagroup.com>
 *
 */

var videoWidgetConfig = angular.module('videoWidgetConfig', []);

videoWidgetConfig.factory('getConfig', function () {
  return {
    videosToShow: 4,
    extraCssClasses: {
      pw: true
    },
    extraResponsiveClasses: {
      full_pw: true,
      full: false // overrides the default defined in app.js
    },
    videosToShowMapping: function (windowWidth) {
      var mappingVideos = 3,
          mappingClass = 'full';
      if(windowWidth.w < 800 && windowWidth.w > 479) {
        mappingVideos = 2;
        mappingClass = 'twoVideo';
      }
      else if (windowWidth.w < 480) {
        mappingVideos = 2;
        mappingClass = 'max480';
      }
      else if (windowWidth.w > 799 && windowWidth.w < 1000){
        // Keep default
      }
      else {
        mappingVideos = 4;
        mappingClass = 'full_pw';
      }
      return {
        'mappingVideos': mappingVideos,
        'mappingClass': mappingClass
      };
    }
  };
});

