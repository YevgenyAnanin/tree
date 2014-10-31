/**
 * Brand specific configuration settings for the video widget
 *
 * @author Yevgeny Ananin <yananin@summitmediagroup.com>
 *
 */

var videoWidgetConfig = angular.module('videoWidgetConfig', []);

videoWidgetConfig.factory('getConfig', function () {
  return {
    videosToShow: 3,
    extraCssClasses: {
      aw: true
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
      else {
        // Keep default
      }
      return {
        'mappingVideos': mappingVideos,
        'mappingClass': mappingClass
      }
    }
  };
});

