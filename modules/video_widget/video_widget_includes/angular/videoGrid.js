var videoGrid = angular.module('videoGrid', []);

videoGrid.controller('videoGridCtrl', ['$scope', '$attrs', '$element', function ($scope, $attrs, $element) {

  // Get all of the videos
  $scope.videos = Drupal.settings.smgVideoGrid.videos;

  $scope.gridWidth = 4;
  $scope.videosInGroup = 12;
  $scope.gridWidthClass = "grid-width-" + $scope.gridWidth;

  // Initialize the counter value that tracks which group of videos in the
  // grid the user is viewing
  $scope.currentCount = 1;

  $scope.totalVideos = Drupal.settings.smgVideoGrid.total_count;

  // Divide the videos into groups
  $scope.videoGroups = {};
  if ( $scope.videos !== 'undefined' ) {
    for ( i=1; i <= Math.ceil($scope.videos.length / $scope.videosInGroup); i++ ) {
      var groupRangeStart = (i - 1) * $scope.videosInGroup;
      var groupRangeEnd = ((i) * $scope.videosInGroup);
      $scope.videoGroups[i] = $scope.videos.slice(groupRangeStart, groupRangeEnd);
    }
  }
  // Function for ng-show
  $scope.gridGroupShow = function (group) {
    return (group == $scope.currentCount)
  }

  // Initialize some other properties
  $scope.hasMoreVideos = ($scope.totalVideos > $scope.videosInGroup);
  $scope.hasPreviousVideos = false;

  // Function that determines how many videos are left in the scroll
  $scope.countRemaining = function () {
    return $scope.videoGroups[$scope.currentCount + 1].length;
  }

  $scope.countPrevious = function () {
    return $scope.videoGroups[$scope.currentCount - 1].length;
  }

  // Store the first video group
  $scope.firstVideoGroup = false;

  $scope.changeFirstGroupsMargin = function () {
    // Change the left-margin value of the first video group
    var newMargin = ($scope.currentCount - 1) * -100 + "%";
    // Store the first video group
    if ( !$scope.firstVideoGroup )
      $scope.firstVideoGroup = $element.find('.smgVideoGrid-video-group.first');
    //var firstVideoGroup = $element.find('.smgVideoGrid-video-group.first');
    $scope.firstVideoGroup.css({"margin-left":newMargin});
  }

  // Increments the counter
  $scope.incrementCounter = function () {
    $scope.currentCount++;
    if ( !$scope.videoGroups.hasOwnProperty($scope.currentCount + 1) || ($scope.videoGroups[$scope.currentCount + 1].length == 0) ) {
      $scope.hasMoreVideos = false;
    }
    $scope.hasPreviousVideos = true;
    $scope.changeFirstGroupsMargin();
  }

  // Decrements the counter
  $scope.decrementCounter = function () {
    $scope.currentCount--;
    $scope.hasMoreVideos = true;
    if ( $scope.currentCount == 1)
      $scope.hasPreviousVideos = false;
    $scope.changeFirstGroupsMargin();
  }

  // Function for determining if video is first in grid row
  $scope.isVideoFirst = function (index) {
    var videoPos = index + 1;
    if (videoPos == 1 || (videoPos % $scope.gridWidth == 1))
      return "is-first-video";
    else
      return "not-first-video";
  }

}]);

videoGrid.directive('videoGrid', [function () {

  videoGridTpl = Drupal.settings.smgAngularTemplates.videoGrid;

  return {
    restrict: 'E',
    scope: true,
    replace: true,
    controller: 'videoGridCtrl',
    templateUrl: videoGridTpl
  };

}]);

/**
 * This directive is applied to each <a> tag that links to a Waywire video.
 * It calls the createLinkPlayer function from the lightbox.js file, which
 * creates a lightbox object and attaches the appropriate onclick events to
 * the link.
 */
videoGrid.directive('smgGridLightbox',['$timeout', function ($timeout) {
  return {
    restrict: 'A',
    scope: true,
    link: function (scope, element) {
      $timeout(function () {
        createLinkPlayer(element[0]);
      }, 0);

      // Make the whole video div clickable
      var videoDiv = element.parents(".smgVideoGrid-video");
      videoDiv.on("click", function () {
        element[0].click();
      });
    }
  };
}]);

/**
 * Custom filter to shorten sentences
 */
videoGrid.filter('cut', function () {
  return function (value, wordwise, max, tail) {
    if (!value) return '';

    max = parseInt(max, 10);
    if (!max) return value;
    if (value.length <= max) return value;

    value = value.substr(0, max);
    if (wordwise) {
      var lastspace = value.lastIndexOf(' ');
      if (lastspace != -1) {
        value = value.substr(0, lastspace);
      }
    }

    return value + (tail || ' ...');
  };
});

// will be loaded in smg_angular module
window.smgAngularDependencies.push('videoGrid');