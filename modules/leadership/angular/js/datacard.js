var dataCard = angular.module('dataCard', ['ngSanitize', 'smgGaEvent']);

dataCard.controller('DataCardCtrl', ['$scope', '$attrs', '$timeout', 'gaEventWrapper', function ($scope, $attrs, $timeout, gaEventWrapper) {

  $scope.nid = $attrs.nid; // Datacard NID
  $scope.tid = $attrs.tid; // Taxonomy ID
  $scope.site = $attrs.site; // Site ID

  $scope.photos = [];

  // Set to true when user hovers over the company link (i.e. the <li> element
  // that this datacard is associated with).
  $scope.parentHover = false;

  // The following variable is set to true if the user is hovering the cursor over the
  // datacard
  $scope.dataCardHover = false;

  // If the information for this data card was added to Drupal.settings, then it should
  // be in Drupal.settings.leadershipDatacard[tid]
  if ( Drupal.settings.hasOwnProperty("leadershipDatacard") ) {
    if ( Drupal.settings.leadershipDatacard.hasOwnProperty($scope.tid) ) {
      for (var compNid in Drupal.settings.leadershipDatacard[$scope.tid] ) {
        if ( Drupal.settings.leadershipDatacard[$scope.tid][compNid]["datacard_nid"] == $scope.nid ) {
          var companyInfo = Drupal.settings.leadershipDatacard[$scope.tid][compNid];
          var dataCardInfo = companyInfo["data_card_processed_for_angular"];

          // Set some properties
          $scope.contactPhotoPath = (dataCardInfo.hasOwnProperty("contact_photo_path")) ? dataCardInfo["contact_photo_path"] : false;

          $scope.contactName = (dataCardInfo.hasOwnProperty("contact_name")) ? dataCardInfo["contact_name"] : "";

          $scope.contactTitle = (dataCardInfo.hasOwnProperty("contact_title")) ? dataCardInfo["contact_title"] : "";

          $scope.companyLogoPath = (dataCardInfo.hasOwnProperty("company_logo_path")) ? dataCardInfo["company_logo_path"] : false;

          $scope.companyWebsite = (companyInfo.hasOwnProperty("company_website")) ? companyInfo["company_website"] : "";

          $scope.productSummary = (dataCardInfo.hasOwnProperty("product_summary")) ? dataCardInfo["product_summary"] : "";

          $scope.companyName = (companyInfo.hasOwnProperty("title")) ? companyInfo.title : "";

          $scope.teaser = (dataCardInfo.hasOwnProperty("teaser")) ? dataCardInfo["teaser"] : "";

          if ( dataCardInfo.hasOwnProperty("product_photos") && (dataCardInfo["product_photos"].length > 0) ) {
            for (j = 0; j < dataCardInfo["product_photos"].length; j++) {

              var photoData = dataCardInfo["product_photos"][j];

              photoObj = {
                url: (photoData.hasOwnProperty("product_photo")) ? photoData["product_photo"] : "",
                link: (photoData.hasOwnProperty("product_link")) ? photoData["product_link"] : "",
                title: (photoData.hasOwnProperty("product_title")) ? photoData["product_title"] : ""
              };

              $scope.photos.push(photoObj);
            }
          }

          $scope.printProfileNid = (companyInfo.hasOwnProperty("print_profile_nid")) ? companyInfo["print_profile_nid"] : "";

          $scope.companyNid = compNid;

          // Check if the company has any waywire videos
          $scope.companyHasWaywireVideos = false;
          if ( companyInfo.waywire_videos.hasOwnProperty($scope.companyNid) && companyInfo.waywire_videos[$scope.companyNid].hasOwnProperty("videos") ) {
            $scope.companyHasWaywireVideos = true
          }

          break;
        }
      }
    }
  }

  $scope.gaEvent = function (link, category, action, label) {
    return gaEventWrapper.gaEvent(link, category, action, label);
  };

  /**
   * Handles the "leadershipHoverBegin" broadcast that is sent from the
   * leadershipLinkCtrl.  It sets the parentHover property to true so the
   * datacard knows that the user is hovering over the leadership link
   *
   */
  $scope.$on('leadershipHoverBegin', function (newVal, oldVal) {
    $scope.$apply(function () {
      $scope.parentHover = true;
    });
  });

  /**
   * Handles the "leadershipHoverEnd" broadcast that is sent from the
   * leadershipLinkCtrl.  It sets the parentHover property to false so the
   * datacard knows that the user is not hovering over the leadership link
   *
   */
  $scope.$on('leadershipHoverEnd', function (newVal, oldVal) {
    $scope.$apply(function () {
      $scope.parentHover = false;
    });
  });

  /**
   * Shows a comma if there is a name for the company contact, and
   * a title
   */
  $scope.contactShowComma = function () {
    return ($scope.contactName.length > 0 && $scope.contactTitle.length > 0) ? "," : "";
  };

  /**
   * Sets the parentHover value to false, called from link function
   * Useful for touch screens.
   */
  $scope.setParentHoverFalse = function () {
    $scope.parentHover = false;
  }

}]);

dataCard.controller('leadershipLinkCtrl', ['$scope', '$rootScope', function ($scope, $rootScope) {

  $scope.hoverOn = false;

  $scope.broadcastHoverBegin = function () {
    $scope.$broadcast('leadershipHoverBegin',{});
    $scope.hoverOn = true;
  };

  $scope.broadcastHoverEnd = function () {
    $scope.$broadcast('leadershipHoverEnd', {});
    $scope.hoverOn = false;
  };

}]);

dataCard.directive('leadershipLink', [function () {
  return {
    restrict: 'A',
    scope: true,
    controller: 'leadershipLinkCtrl',
    link: function (scope, elem, attrs) {
      var leadershipLink = elem.find(".leadership-link");
      var leadershipLinkRightOffset = leadershipLink.position().left + leadershipLink.width();
      elem.on({
        mouseover: function (e) {
          // Get the x offset @see http://stackoverflow.com/questions/12704686/html5-with-jquery-e-offsetx-is-undefined-in-firefox
          xOffset = (e.offsetX == undefined) ? (e.pageX - elem.offset().left) : e.offsetX;
          if ( xOffset < (leadershipLinkRightOffset + 20)) {
            scope.broadcastHoverBegin();
          }
        },
        mouseleave: function () {
          scope.broadcastHoverEnd();
        }
      });

      elem.find('a.leadership-link').each(function (i,el) {

        var leaderLink = jQuery(el);
        if ( !leaderLink.hasClass('touchstart-off') )
          leaderLink.addClass('touchstart-off');

        leaderLink.on('touchstart', function (e) {
          if ( leaderLink.hasClass('touchstart-off') ) {
            e.preventDefault();
            e.stopPropagation();
            e.stopImmediatePropagation();
            leaderLink.addClass('touchstart-begin').removeClass('touchstart-off');
            scope.broadcastHoverBegin();
          }
          else {
            window.location.href = leaderLink.attr("href");
          }
        });

      });
    }
  };
}]);

/**
 * Directive that defines the pop-up data card
 */
dataCard.directive('leadershipDataCard', ['$window', '$http', '$compile', function ($window, $http, $compile) {

  var dataCardTpl = Drupal.settings.smgAngularTemplates.dataCard;

  return {
    restrict: 'E',
    replace: true,
    scope: true,
    controller: 'DataCardCtrl',
    link: function (scope, elem, attrs) {

      scope.currentDataCardHover = false;

      elem.on({
        mouseenter: function () {
          scope.dataCardHover = true;
        },
        mouseleave: function () {
          scope.dataCardHover = false;
        }
      });

      // Utility function for getting the distance to the bottom of the page
      // from a given element
      scope.getDistanceToBottom = function (element) {
        var scrollTop = angular.element($window).scrollTop(),
            liOffset = element.offset().top,
            distance = liOffset - scrollTop;
        var bottomOfVisibleWindow = angular.element($window).height();
        var distanceToBottomOfPage = bottomOfVisibleWindow - distance;

        return distanceToBottomOfPage;
      };

      scope.checkTouchEvent = function (e) {
        var rect = elem[0].getBoundingClientRect();

        if ( jQuery(e.target).hasClass('touchstart-begin') )
          return false;

        var yOffset = e.pageY - jQuery(window).scrollTop();
        if ((yOffset <= rect.bottom && yOffset >= rect.top) && (e.pageX <= rect.right && e.pageX >= rect.left)) {
          scope.currentDataCardHover = true;
        }
        else {
          scope.currentDataCardHover = false;
          scope.$apply();
        }

      }

      scope.$watch('parentHover', function () {
        if ( scope.parentHover ) {

          // Now we show the datacard, but first must calculate the top position and width
          var windowWidth = angular.element($window).width();
          var dataCardWidth = (windowWidth > 500) ? "500px" : "90%";
          // The top absolute position of the datacard, which we might modify below
          var topPosition = '-50%';

          // Set the width of the datacard FIRST, that way when we calculate the height below,
          // it will be accurate
          elem.css({"visibility":"hidden"});
          elem.css({"width":dataCardWidth, "display":"block"});

          // Get the datacard height
          var dataCardHeight = elem.outerHeight();
          // Below, we check to see if there's a video widget in this datacard.  If there is,
          // and the videoWidget-video has a width of zero, then the widget has rendered yet, so
          // we can't trust the full height of the datacard that we calculated above.  Therefore
          // we just add 225px.
          if ( elem.find(".videoWidget").length ) {
            var videoWidget = elem.find(".videoWidget");
            if ( !videoWidget.find(".videoWidget-video").width() ) {
              dataCardHeight = elem.find(".dataCard-top").outerHeight() + 225;
            }
          }

          // Get distance of parent li link from top of browser window
          var distanceToBottom = scope.getDistanceToBottom(elem.parent());

          /**
           * This function is used to calculate where to put the arrow
           * @returns {number}
           *  Pixels that represent the distance from the top of the datacard
           *  to the half-way point of the data card's parent li
           */
          var calculatePosArrow = function () {
            // Get the height of the parent li
            var heightOfParentLi = elem.parent().outerHeight(true);
            // Get the distance of the data card to the bottom of window
            var dataCardDistance = scope.getDistanceToBottom(elem);
            // Get the distance b/w datacard and parent li
            var distanceDataCardLi = dataCardDistance - distanceToBottom;

            return distanceDataCardLi + (heightOfParentLi * 0.5);
          };

          // If the datacard height is bigger than the distance to the bottom of
          // the page then it will go below the fold of the page, so we recalculate
          // its top position
          if ((dataCardHeight - 20) > distanceToBottom) {
            topPosition = (dataCardHeight - distanceToBottom + 20) * -1;
            topPosition+="px";
          }

          elem.css({top:topPosition, visibility:"visible"});

          // Set the top position of the right arrow
          var distanceArrow = calculatePosArrow();
          var dataCardArrow = elem.find(".dataCard-after-arrow");
          if ( parseInt(distanceArrow) > (dataCardHeight - 20) ) {
            dataCardArrow.css({"display":"none"});
          }
          else {
            dataCardArrow.css({"display":"block"});
            scope.currentDataCardHover = true;

            document.addEventListener('touchstart', scope.checkTouchEvent, true);
          }

          dataCardArrow.css({top:distanceArrow});

        }
        else {
          if ( !scope.dataCardHover ){
            elem.css({display:""}); // Revert to default css style
            scope.currentDataCardHover = false;
          }
        }

        scope.$watch('currentDataCardHover', function () {
          if ( !scope.currentDataCardHover ) {
            document.removeEventListener('touchstart', scope.checkTouchEvent, true);
            elem.css({display:""});

            // Set the data card's parent hover value to false
            scope.setParentHoverFalse();

            elem.siblings('.touchstart-begin').each(function (i,e) {
              jQuery(e).removeClass('touchstart-begin').addClass('touchstart-off');
            });
            elem.siblings('.link-section').each(function (i,e) {
              jQuery(e).find('a.leadership-link').removeClass('touchstart-begin').addClass('touchstart-off');
            });
          }
        });


      });
    },
    templateUrl: dataCardTpl
  };
}]);

dataCard.directive('compileDataCard', ['$compile', function ($compile) {
  return {
    restrict: 'E',
    scope: {
      nid: '@',
      tid: '@',
      site: '@'
    },
    replace: true,
    link: function (scope, elem, attrs) {

      // Create an object.  Upon the interpolation of the nid and tid values (determined
      // by the $observe fcts below), the values of the interpValues obj will be changed.
      // Subsequently the $watchCollection fct will determine when both values are set.
      scope.interpValues = {
        interpNid: false,
        interpTid: false,
        interpSite: false
      };

      attrs.$observe('nid', function (newValue) {
        scope.interpValues.interpNid = newValue;
      });

      attrs.$observe('tid', function (newValue) {
        scope.interpValues.interpTid = newValue;
      });

      attrs.$observe('site', function (newValue) {
        scope.interpValues.interpSite = newValue;
      });

      scope.$watchCollection('interpValues', function (newValues, oldValues) {
        if ( newValues.interpTid && newValues.interpTid && newValues.interpSite ) {
          var dataCardTplWrap = angular.element('<leadership-data-card nid="' + scope.nid + '" tid="' + scope.tid + '" site="' + scope.site + '"></leadership-data-card>');
          var compiledDataCardTpl = $compile(dataCardTplWrap)(scope);
          elem.replaceWith(compiledDataCardTpl);
        }
      });

    }
  };
}]);

dataCard.directive('compileVideoWidget', ['$compile', '$window', function ($compile, $window) {
  return {
    restrict: 'E',
    scope: {
      nid: '@',
      tid: '@'
    },
    replace: true,
    link: function (scope, elem, attrs) {

      var w = angular.element($window);

      // Create an object.  Upon the interpolation of the nid and tid values (determined
      // by the $observe fcts below), the values of the interpDatacardValues obj will be changed.
      // Subsequently the $watchCollection fct will determine when both values are set.
      scope.interpDataCardValues = {
        interpNid: false,
        interpTid: false
      };

      attrs.$observe('nid', function (newValue) {
        scope.interpDataCardValues.interpNid = newValue;
      });

      attrs.$observe('tid', function (newValue) {
        scope.interpDataCardValues.interpTid = newValue;
      });

      scope.$watchCollection('interpDataCardValues', function (newValues, oldValues) {
        if ( newValues.interpNid && newValues.interpTid ) {
          //scope.nid = newValues.interpNid;
          //scope.tid = newValues.interpTid;
          var videoWidgetTpl = angular.element('<video-widget datacard="true" nid="' + scope.nid + '" tid="' + scope.tid + '" ></video-widget>');
          if (w.width() > 480) {
            var compiledWidget = $compile(videoWidgetTpl)(scope);
            elem.replaceWith(compiledWidget);
          }
        }
      });

    }
  }
}]);

dataCard.directive('smgAnchorGaEvent',[function () {
  return {
    restrict: 'A',
    link: function (scope, element, attrs) {
      angular.element(element).click(function (event) {
        event.preventDefault();
        event.stopPropagation();
      });
    }
  };
}]);

// will be loaded in smg_angular module
window.smgAngularDependencies.push('dataCard');