var all_companies_block = angular.module('all_companies_block', [/*'ngAnimate'*/]);

/**
 * The controller of the all companies block.
 */
all_companies_block.controller('AllCompaniesBlockTermListCtrl', ['$scope', '$http', '$attrs', function($scope, $http, $attrs) {
  // Defaults.
  $scope.taxonomies = {};
  $scope.taxonomies.companies = {};

  $scope.site = $attrs.site;

  // Get taxonomies from Drupal object.
  if (Drupal.settings.hasOwnProperty('leadership_all_companies_block')) {
    if (Drupal.settings.leadership_all_companies_block) {
      $scope.taxonomies = Drupal.settings.leadership_all_companies_block;
    }
  }

  // Expand category.
  $scope.categoryOpen = function(taxonomy) {
    // Get the companies only if it never been expanded
    if (taxonomy.show == null) {
      $scope.getCompanies(taxonomy);
    }

    // Show/Hide Category
    taxonomy.show = !taxonomy.show;
  };

  /**
   * Get all the companies for the taxonomy.
   *
   * @param object taxonomy
   *   Taxonomy object.
   */
  $scope.getCompanies = function(taxonomy) {
    var path = '/leaders/get-company-list/json/' + taxonomy.tid + '/1';
    $http({method: 'GET', url: path}).
      success(function (data, status, headers, config) {
        taxonomy.companies = data;

        // If there aren't any videos then instantiate the variable. (FYI videos
        // can come from other places. Like the video widget on node pages.)
        if (!Drupal.settings.hasOwnProperty('waywire_leadership')) {
          Drupal.settings.waywire_leadership = {};
        }

        // Add the datacard info to the Drupal object
        if ( !Drupal.settings.hasOwnProperty('leadershipDatacard') ) {
          Drupal.settings.leadershipDatacard = {};
        }
        if ( !Drupal.settings.leadershipDatacard.hasOwnProperty(taxonomy.tid) ) {
          Drupal.settings.leadershipDatacard[taxonomy.tid] = data;
        }

        // Add the videos to the Drupal object.
        /*
        for (i = 0; i < data.length; i++) {
          // Only add data if the company has videos.
          if (data[i].waywire_videos[data[i].nid] !== 'undefined' && data[i].waywire_videos[data[i].nid].length !== 0) {
            Drupal.settings.waywire_leadership[data[i].nid] = data[i].waywire_videos[data[i].nid];
          }
        }*/
      }
    );
  };
}]);

/**
 * Animate the slidedown and up of the all companies block.
 */
all_companies_block.animation('.slideDown', function() {
  return {
    addClass: function(element, className, done) {
      jQuery(element).slideDown(done);
    },
    removeClass: function(element, className, done) {
      jQuery(element).slideUp(done);
    }
  }
});

/**
 * Lists all the companies.
 */
all_companies_block.directive('companyList', function () {
  return {
    restrict: 'E',
    replace: true,
    template: '<li ng-repeat="company in taxonomy.companies" leadership-link>' +
                '<a class="all-companies-block-text company-name leadership-link" href="{{company.alias}}">{{company.title}}</a>' +
                '<span class="youtube-amplified" ng-show="company.field_youtube_amplified_value == 1">' +
                  '<span class="circle">' +
                    '<span class="triangle"></span>' +
                  '</span>' +
                '</span>' +
                '<compile-data-card tid="{{company.tid}}" nid="{{company.datacard_nid}}" site="{{site}}" hover-state="hoverState"></compile-data-card>' +
              '</li>'
  }
});

/**
 *  Lists all the taxonomies.
 */
all_companies_block.directive('taxonomyList',['$timeout', function ($timeout) {
  return {
    restrict: 'E',
    link: function (scope, elem, attrs) {
      scope.$watch(
        function () {
          return scope.taxonomy.show;
        },
        function (newValue, oldValue) {
          var ulMenu = elem.find(".leadership-menu");
          if (newValue) {
            if ( ulMenu.hasClass("slideDown") ) {
              if ( ulMenu.hasClass("hide") ) {
                ulMenu.removeClass("hide");
                ulMenu.addClass("invisible");
                ulMenu.css({"max-height":""});
                var ulMenuHeight = ulMenu.height();
                var timeoutHeightCapture = setInterval(function () {
                  var timerHeight = ulMenu.height();
                  if ( ulMenuHeight != 0 && (timerHeight == ulMenuHeight) ) {
                    clearInterval(timeoutHeightCapture);
                    ulMenu.css("max-height",ulMenuHeight);
                    ulMenu.removeClass("invisible");
                    ulMenu.addClass("hide");
                    ulMenu.slideDown("fast", function () {
                      ulMenu.removeClass("hide");
                    });
                  }
                  else {
                    ulMenuHeight = timerHeight;
                  }
                }, 15);
              }
            }
          }
          else if (!newValue && newValue != null && newValue != undefined) {
            if ( scope.taxonomy.companies ) {
              if ( ulMenu.hasClass("slideUp") ) {
                ulMenu.slideUp("slow", function () {
                  ulMenu.addClass("hide");
                });
              }
            }
          }
        }
      );

    },
    template: '<div class="taxonomy-tid-{{taxonomy.tid}} taxonomy-tid">' +
                '<span class="brackets" ng-click="categoryOpen(taxonomy)">' +
                  '[' +
                  '<span class="plus-minus">' +
                    '<span ng-show="!taxonomy.show">+</span><span ng-show="taxonomy.show">–</span>' +
                  '</span>' +
                  ']' +
                '</span>' +
                '&nbsp;' +
                '<span class="taxonomy-name all-companies-block-text" ng-click="categoryOpen(taxonomy)">{{taxonomy.name}}</span>' +
                '<ul class="company-list leadership-menu hide" ng-class="{slideDown: taxonomy.show, slideUp: !taxonomy.show}">' +
                  '<company-list></company-list>' +
                '</ul>' +
              '</div>'
  };
}]);

// will be loaded in smg_angular module
window.smgAngularDependencies.push('all_companies_block');
