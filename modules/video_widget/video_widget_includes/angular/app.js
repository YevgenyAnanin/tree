var videoWidget = angular.module('videoWidget', ['videoWidgetConfig']);

// Bootstrap angularjs manually
jQuery(document).ready(function () {
  angular.bootstrap(document, ['videoWidget'] );
});

/**
 * The main controller for each video widget
 */
videoWidget.controller('VideoListCtrl',['$scope', 'getConfig', '$attrs', '$http', function ($scope, getConfig, $attrs, $http) {

  /*
  $scope.videos = [
    {'name':'Test Video 1',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/H06L9B3P3T7T5YH1.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/Moxa-IP-Video-Showcase-Overview'},
    {'name': 'Test Video 2',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/C448BH34GP9VYXN0.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/The-Monthly-Rundown-for-Moxa-On'},
    {'name': 'Test Video 3',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/6VV9DG0JB3JV1FKQ.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/VPort-36-Intelligent-Video-Anal'},
    {'name':'Test Video 4',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/T682GX1KXKC21NG3.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/VPort-36-De-mist-Function-Demo'},
    {'name': 'Test Video 5',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/9YMDDD32L57GZJ81.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/VPort-36-Footage-Low-light-and'},
    {'name': 'Test Video 6',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/BJFSXR3DX0PH61XZ.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/VPort-36-De-mist-Function-Dem-2'},
    {'name': 'Test Video 7',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/75GQFF2X9LV148SG.jpg',
     'href': 'http://summitmediagroup.magnify.net/video/Moxa-VPort-36-Footage-Vision-Te'}
    {'name': 'Test Video 8',
     'img_url': 'http://s3.amazonaws.com/magnifythumbs/GMN7042X1Z6VJH82.jpg',
     'href': 'http://summitmediagroup.magnify.net/embed/content/MFJLR62GCV3YWPCN'}
  ];*/

  $scope.nid = $attrs.nid;

  // Populate the videos property
  $scope.videos = {};
  if ( Drupal.settings.hasOwnProperty('waywire_leadership') ) {
    if ( Drupal.settings.waywire_leadership.hasOwnProperty($scope.nid) ) {
      var waywireData = Drupal.settings.waywire_leadership[$scope.nid];
      // Set some properties on the scope
      $scope.page = waywireData.page;
      $scope.pageCount = waywireData.page_count;
      $scope.totalCount = waywireData.total_count;
      $scope.videos = waywireData.videos;
    }
  }

  $scope.config = getConfig;

  $scope.currentCount = 1; // Initialize the count

  // An object that stores the count of how many videos to show in the widget
  $scope.videoWidgetParams = {
    videosToShow: $scope.config.videosToShow, // How many videos to show at once
    oldVideosToShow: $scope.config.videosToShow // Will store the value of videosToShow from previous draw()
  };

  $scope.updateVideosToShow = function (newNumber) {
    $scope.videoWidgetParams.oldVideosToShow = $scope.videoWidgetParams.videosToShow;
    $scope.videoWidgetParams.videosToShow = newNumber;

    $scope.normalizeCount();
  }

  // Determine how many video blocks there are (i.e. if there's
  // 12 possible videos and you're showing 3 at a time, then
  // there's 4 video blocks
  $scope.videoBlockCount = function () {
    return Math.ceil($scope.videos.length / $scope.videoWidgetParams.videosToShow);
  };

  // Helper function for determining size of object
  // @see http://stackoverflow.com/questions/5223/length-of-javascript-object-ie-associative-array
  $scope.sizeOfObj = function (obj) {
    var size = 0, key;
    for (key in obj) {
      if (obj.hasOwnProperty(key)) size++;
    }
    return size;
  }

  // Iterates the counter to the next position, or to position 1
  // if we've reached the end
  $scope.videosNext = function () {

    var videoBlockCount = $scope.videoBlockCount();

    if($scope.currentCount < videoBlockCount) {

      // If we're close to the end, then use ajax to get more videos
      if( (videoBlockCount - $scope.currentCount === 1) && ($scope.sizeOfObj($scope.videos) < $scope.totalCount) ) {
        var waywireJSONPath = '/waywire_leadership/get/json/' + $scope.nid + '/' + (++$scope.page);
        $http({method: 'GET', url: waywireJSONPath}).
          success(function (data, status, headers, config) {
            for (i = 0; i < data.videos.length; i++) {
              $scope.videos.push( data.videos[i] );
            }
          });
      }

      $scope.currentCount++;
    }
    else {
      $scope.currentCount = 1;
    }

  };

  $scope.videosPrev = function () {
    if($scope.currentCount > 1) {
      $scope.currentCount--;
    }
  };

  // We show/hide videos in the counter by changing their visibility
  $scope.draw = function () {
    var startIndex = ($scope.currentCount - 1) * $scope.videoWidgetParams.videosToShow;
    var endIndex = ($scope.currentCount * $scope.videoWidgetParams.videosToShow);

    $scope.videos.forEach(function(video, index) {
      if (!(index >= startIndex && index < endIndex)) {
        video.visible = false;
      }
      else {
        video.visible = true;
      }
    });
  };

  /**
   *
   * Normalize the currentCount parameter.  Since the currentCount param
   * increments every time the user clicks next, we have a problem if the
   * user resizes the window.  Example:
   *  - We have 6 videos, and on a small browser window we show one at a time,
   *     therefore you can increment the counter 6 times.
   *  - On a larger window, we show three at a time, therefore you can
   *    only increment 2 times: increment 1 = (videos 1 - 3)
   *    and  increment 2 = (videos 4 - 6).
   *  - If you're viewing on a small window, and you're on increment 4 (video 4),
   *    and then you transition to a larger page, we have to redraw the video
   *    widget as though it were on increment 2, since that corresponds to
   *    video 4.
   *
   */
  $scope.normalizeCount = function () {


    var firstVideo = ($scope.currentCount * $scope.videoWidgetParams.oldVideosToShow) - ($scope.videoWidgetParams.oldVideosToShow - 1);
    var show = $scope.videoWidgetParams.videosToShow;
    if (firstVideo < show) {
      $scope.currentCount = 1;
    }
    else if (!(firstVideo % show)) {
      $scope.currentCount = firstVideo / show;
    }
    else {
      $scope.currentCount = (Math.floor(firstVideo / show)) + 1
    }
  };

  // This object contains extra classes that will be added throughout the
  // templates for responsive design (using ng-class).  Only one of these
  // values should be true.
  $scope.responsiveCssClasses = {
    full: true,
    twoVideo: false,
    oneVideo: false,
    max480: false,
  };

  // This object contains extra classes that will be added to the templates.
  // It will look to see if there are any classes added globally in the site
  // specific video widget module (videoWidgetConfig).
  $scope.extraCssClasses = ($scope.config.extraCssClasses !== undefined) ? $scope.config.extraCssClasses : {};

  // This function simply merges responsiveCssClasses with extraCssClasses
  $scope.mergeCssClassesObjs = function () {
    for (var attrname in $scope.responsiveCssClasses) $scope.extraCssClasses[attrname] = $scope.responsiveCssClasses[attrname];
  }

  // Call the mergeCssClassesObjs function initially
  $scope.mergeCssClassesObjs();

}]);

/**
 * This directive defines the videoWidget-videos div that contains the videos,
 * and it is responsible for watching the value of the counter, and then
 * redrawing the widget when the counter changes.
 */
videoWidget.directive('videoSlider', function () {

  // Get the location of the video.html template
  var videoTpl = Drupal.settings.smgAngularTemplates.video;

  return {
    restrict: 'AE',
    replace: true,
    scope: true,
    link: function (scope, element, attrs) {

      // Initialize the visibility of the videos in the video
      // widget
      scope.draw();

      // Watch changes to the parent scope's currentCount property
      scope.$watch('currentCount', function () {
        scope.draw();
      });

    },
    //templateUrl: '/video-widget/resource/angular-template/video.html'
    templateUrl: videoTpl
  };
});

/**
 * This directive is applied to the videoWidget-wrapper div.  It watches
 * the size of the browser window and redraws the video widget if
 * necessary
 */
videoWidget.directive('resizable', function ($window) {
  return {
    restrict: 'AE',
    scope: true,
    link: function (scope, element) {
      var w = angular.element($window);
      scope.getWindowDimensions = function () {
        return {
          'w': w.width(),
          'h': w.height()
        };
      };
      scope.$watch(scope.getWindowDimensions, function (newValue, oldValue) {

        // This function checks the "videosToShow" parameter in the parent
        // scope, and if necessary it changes the parameter and calls the
        // draw() function.  It also calls the changeClass function.
        var resizeRedraw = function (videosToShow, newClass) {
          if ( !scope.responsiveCssClasses[newClass] ) {
            scope.updateVideosToShow(videosToShow);
            changeClass(newClass);
            scope.draw();
          }
        }

        // This function changes the css class on the move button, by manipulating
        // the moverCssClasses array
        var changeClass = function (newClass) {
          if (!scope.responsiveCssClasses[newClass]) {
            for (var key in scope.responsiveCssClasses) {
              if (scope.responsiveCssClasses.hasOwnProperty(key)) {
                if (key == newClass) {
                  scope.responsiveCssClasses[key] = true;
                }
                else {
                  scope.responsiveCssClasses[key] = false;
                }
              }
            }
            scope.mergeCssClassesObjs();
          }
        }

        if(newValue.w < 800 && newValue.w > 479) {
          resizeRedraw(2, 'twoVideo');
        }
        else if (newValue.w < 480) {
          resizeRedraw(2, 'max480');
        }
        else {
          resizeRedraw(3, 'full');
        }
      }, true);

      w.bind('resize', function () {
        scope.$apply();
      });
    }
  };
});

/**
 * This directive is applied to each <a> tag that links to a Waywire video.
 * It calls the createLinkPlayer function from the lightbox.js file, which
 * creates a lightbox object and attaches the appropriate onclick events to
 * the link.
 */
videoWidget.directive('smgLightbox',['$timeout', function ($timeout) {
  return {
    restrict: 'A',
    scope: true,
    link: function (scope, element) {
      $timeout(function () {
        createLinkPlayer(element[0]);
      }, 0);
    }
  };
}]);