var videoWidget = angular.module('videoWidget', ['videoWidgetConfig'/*, 'ngAnimate'*/]);

// Bootstrap angularjs manually
/*
jQuery(document).ready(function () {
  angular.bootstrap(document, ['videoWidget'] );
});
*/

/**
 * The main controller for each video widget
 */
videoWidget.controller('VideoListCtrl',['$scope', 'getConfig', '$attrs', '$http', '$element', '$window', function ($scope, getConfig, $attrs, $http, $element, $window) {

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
      $scope.companyName = waywireData.companyName;
      $scope.videos = waywireData.videos;
    }
  }

  $scope.config = getConfig;

  $scope.currentCount = 1; // Initialize the count

  // Boolean value that specifies if this video widget belongs to a datacard
  $scope.isDataCard = ($attrs.hasOwnProperty("datacard") && $attrs.datacard);

  // An object that stores the count of how many videos to show in the widget
  $scope.videoWidgetParams = {
    videosToShow: ($scope.isDataCard) ? 3 : $scope.config.videosToShow, // How many videos to show at once
    oldVideosToShow: ($scope.isDataCard) ? 3 : $scope.config.videosToShow // Will store the value of videosToShow from previous draw()
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

  $scope.isLastVideoBlock = false;

  // Helper function for determining size of object
  // @see http://stackoverflow.com/questions/5223/length-of-javascript-object-ie-associative-array
  $scope.sizeOfObj = function (obj) {
    var size = 0, key;
    for (key in obj) {
      if (obj.hasOwnProperty(key)) size++;
    }
    return size;
  }

  /**
   * Determine if the video widget is currently showing the last video block.  For example, if there
   * are 9 videos, and we're showing 3 at a time, then when the widget is showing videos 6-9 then
   * we're on the last video block
   */
  $scope.isLastVideoBlockCalculate = function () {
    return ( ($scope.videoBlockCount() - $scope.currentCount === 0) && !($scope.sizeOfObj($scope.videos) < $scope.totalCount) );
  }

  // Iterates the counter to the next position
  $scope.videosNext = function () {

    var videoBlockCount = $scope.videoBlockCount();
    if($scope.currentCount < videoBlockCount) {

      // If we're close to the end, then use ajax to get more videos
      if( videoBlockCount - $scope.currentCount === 1 ) {

        if ( $scope.sizeOfObj($scope.videos) < $scope.totalCount ) {
          var waywireJSONPath = '/waywire_leadership/get/json/' + $scope.nid + '/' + (++$scope.page);
          $http({method: 'GET', url: waywireJSONPath}).
            success(function (data, status, headers, config) {
              for (i = 0; i < data.videos.length; i++) {
                $scope.videos.push( data.videos[i] );

              }
            });
        }
        else {
          $scope.isLastVideoBlock = true;
        }
      }

      $scope.currentCount++;
    }
    else {
      //$scope.currentCount = 1;
    }

  };

  $scope.videosPrev = function () {
    if($scope.currentCount > 1) {
      $scope.currentCount--;
      $scope.isLastVideoBlock = false;
    }
  };

  // We show/hide videos in the counter by changing their visibility
  $scope.draw = function () {
    var startIndex = ($scope.currentCount - 1) * $scope.videoWidgetParams.videosToShow;
    var endIndex = ($scope.currentCount * $scope.videoWidgetParams.videosToShow);

    // Check if we're on the last video block
    $scope.isLastVideoBlock = $scope.isLastVideoBlockCalculate();
    // Add to extraCssClasses
    $scope.extraCssClasses["lastVideoBlock"] = $scope.isLastVideoBlock;

    $scope.videos.forEach(function(video, index) {
      if (!(index >= startIndex && index < endIndex)) {
        video.visible = false;
        if (index < startIndex) {
          video.videoCss = 'hideLeft';
        }
        else {
          video.videoCss = 'hideRight';
        }
      }
      else {
        video.visible = true;
        video.videoCss = (endIndex - index == 1) ? {noHide:true, lastNoHide:true} : {noHide:true};
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

  // The passed in config fct may have an extraResponsiveClasses obj, if so
  // then merge w/ values in scope's responsiveCssClasses obj
  if ( $scope.config.hasOwnProperty('extraResponsiveClasses') && !$scope.isDataCard ) {
    for ( var cssClass in $scope.config.extraResponsiveClasses ) {
      $scope.responsiveCssClasses[cssClass] = $scope.config.extraResponsiveClasses[cssClass];
    }
  }

  // This object contains extra classes that will be added to the templates.
  // It will look to see if there are any classes added globally in the site
  // specific video widget module (videoWidgetConfig).
  $scope.extraCssClasses = ($scope.config.extraCssClasses !== undefined) ? angular.copy($scope.config.extraCssClasses) : {};

  // This function simply merges responsiveCssClasses with extraCssClasses
  $scope.mergeCssClassesObjs = function () {
    for (var attrname in $scope.responsiveCssClasses) $scope.extraCssClasses[attrname] = $scope.responsiveCssClasses[attrname];
  }

  // Call the mergeCssClassesObjs function initially
  $scope.mergeCssClassesObjs();


}]);

videoWidget.directive('videoWidget', ['$window', function ($window) {

  var videoWidgetTpl = Drupal.settings.smgAngularTemplates.videoWidget;

  return {
    restrict: 'E',
    replace: true,
    scope: true,
    controller: 'VideoListCtrl',
    link: function (scope, elem, attrs) {
      attrs.$observe('nid', function (newValue) {
        scope.nid = newValue;
      });
    },
    templateUrl: videoWidgetTpl
  };
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
    templateUrl: videoTpl
  };
});

/**
 * This directive is applied to the videoWidget-wrapper div.  It watches
 * the size of the browser window and redraws the video widget if
 * necessary
 */
videoWidget.directive('resizable', ['$window', 'getConfig', function ($window, getConfig) {
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

        // Call the videosToShowMapping function that we expect each site to have implemented
        videosToShowMap = getConfig.videosToShowMapping(newValue);
        if ( !scope.isDataCard )
          resizeRedraw(videosToShowMap.mappingVideos, videosToShowMap.mappingClass);

        /*
        if(newValue.w < 800 && newValue.w > 479) {
          resizeRedraw(2, 'twoVideo');
        }
        else if (newValue.w < 480) {
          resizeRedraw(2, 'max480');
        }
        else {
          resizeRedraw(3, 'full');
        }*/
      }, true);

      w.bind('resize', function () {
        scope.$apply();
      });
    }
  };
}]);

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

      // Make the whole video div clickable
      var videoDiv = element.parents(".videoWidget-video");
      videoDiv.on("click", function () {
        element[0].click();
      });
    }
  };
}]);

function isIE () {
  var myNav = navigator.userAgent.toLowerCase();
  return (myNav.indexOf('msie') != -1) ? parseInt(myNav.split('msie')[1]) : false;
}

/**
 * This directive is applied to each videoWidget-videos div
 */
videoWidget.directive('videoContainerStyle',['$window', '$timeout', function ($window, $timeout) {
  return {
    require: 'videoContainerStyle',
    restrict: 'A',
    scope: true,
    controller: function ($scope, $element) {
      this.changeVideoContainerHeight = function () {

        var videosContainer = $element;
        var window = jQuery($window);

        var isIE = function () {
          var myNav = navigator.userAgent.toLowerCase();
          return (myNav.indexOf('msie') != -1) ? parseInt(myNav.split('msie')[1]) : false;
        }

        var isIE = isIE();
        var isIE8orLess = (isIE && jQuery.inArray(isIE, ["6", "7", "8"]) );

        if (window.innerWidth() > 480 || isIE) {

          // Set the width of the videoWidget-videos div to the closest number
          // divisible by the "videos to show" number
          var videosToShow = $scope.videoWidgetParams.videosToShow;

          //var contWidth = Math.floor(videosContainer[0].getBoundingClientRect().width);
          var videoWidgetWrapperWidth = $element.parents(".videoWidget-wrapper").width();

          var videosMaxWidth = .8;
          if ( $element.css("max-width") != "none" ) {
            var mWidth = $element.css("max-width");

            if ( mWidth.search("%") > -1) {
              videosMaxWidth = parseFloat(mWidth) / 100;
            }
          }
          var contWidth = videosMaxWidth * videoWidgetWrapperWidth;

          if (contWidth % videosToShow != 0) {
            var newContainerWidth = Math.floor(contWidth / videosToShow) * videosToShow;
            $element.css({"width":newContainerWidth+"px"});
          }

          $element.css({"height":""});
        }
        else {
          // Calculate the height of the video container
          var newHeight = 0;
          videosContainer.find(".videoWidget-video.noHide").each(function (i,e) {
            newHeight += jQuery(e).outerHeight(true);
          });

          // Subtract the margin-bottom value
          newHeight -= 2 * parseInt(videosContainer.find(".videoWidget-video.lastNoHide").css("margin-bottom"));
          //$element[0].style.height = newHeight + 'px';
          $element.css({"height":newHeight+"px"});

          $element.css({"width":""});

        }
      };

    },
    link: function (scope, element, attrs, currentCtrl) {

      //$timeout(currentCtrl.changeVideoContainerHeight,0);

      scope.$watch(scope.getWindowDimensions(), function (newValue, oldValue) {
        currentCtrl.changeVideoContainerHeight();
      });

      angular.element($window).bind('resize', function () {
        currentCtrl.changeVideoContainerHeight();
        scope.$apply();
      });

      scope.$on('leadershipHoverBegin', function () {
        currentCtrl.changeVideoContainerHeight();

        scope.$broadcast('videoContainerVisible');
      });

    }
  };
}]);

/**
 * This directive is applied to each video inside of a video slider
 */
videoWidget.directive('videoStyle', ['$window', '$timeout', function ($window, $timeout) {
  return {
    restrict: 'A',
    require: '^videoContainerStyle',
    scope: true,
    link: function (scope, element, attrs, vidContCtrl) {

      scope.changeVideoStyles = function () {

        if ($window.innerWidth < 480) {

          // Make sure all videos are equal height when in mobile view
          // so we make them all the height of the first video
          var heightFirst = element.siblings().first().height();
          element.siblings().height(heightFirst + "px");

          element.css("margin-left","");
          if ( scope.$first ) {
            $timeout(function () {
              element.css("width","");
              element.siblings().css("width","");
            }, 200);
          }

          if (scope.$last) {
            $timeout(vidContCtrl.changeVideoContainerHeight,1000);
            $timeout(vidContCtrl.changeVideoContainerHeight,4000);
          }


          if (scope.$first && (scope.currentCount > 1)) {
            $timeout(function () {
              var elementMargin = parseInt(element.css("margin-bottom"));
              newMargin = (-1 * (scope.currentCount - 1))
              * ((element.outerHeight() * scope.videoWidgetParams.videosToShow) + (elementMargin * (scope.videoWidgetParams.videosToShow)));

              newMargin += elementMargin;
              element.css({"margin-top":newMargin + "px"});
            }, 300);
          }
        }
        else {

          // If the index is 0, then we're on the first video.  The margin left prop
          // of this video determines how far the entire video widget is scrolled
          if (scope.$index === 0) {

            $timeout(function () {
              var videosContainer = element.parents(".videoWidget-videos");
              var correctVideoWidth = Math.floor(videosContainer.width() / scope.videoWidgetParams.videosToShow);
              element.css({"width":correctVideoWidth});
              element.siblings(".videoWidget-video").css({"width":correctVideoWidth});
            }, 100);

            var marginLeft = -100 * (scope.currentCount -1);
            element.css({"margin-left":marginLeft + '%'});
          }

          // Remove any specific height
          element.css({"height":""});
        }
      };

      scope.$watch('currentCount', function () {
        scope.changeVideoStyles();
      });

      angular.element($window).bind('resize', function () {
        if ( element.is(":visible") ) {
          scope.changeVideoStyles();
          //scope.$apply();
        }
      });

      scope.$on('videoContainerVisible', function () {
        element.css({"transition":"none"});
        scope.changeVideoStyles();
        $timeout(function () {
          element.css({"transition":""});
        }, 100);
      });

    }
  };
}]);

// will be loaded in smg_angular module
window.smgAngularDependencies.push('videoWidget');
