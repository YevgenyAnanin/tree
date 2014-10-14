/**
 * Adds functionality that allows us to wrap the google analytics event
 * syntax in a function, so that we can put it into an ng-click directive
 *
 * @author Yevgeny Ananin <yananin@summitmediagroup.com>
 *
 */

var smgGaEvent = angular.module('smgGaEvent', []);

smgGaEvent.factory('gaEventWrapper', ['$timeout', function ($timeout) {
  return {
    gaEvent: function (link, category, action, label) {
      _gaq.push('_trackEvent', category, action, label);
      // Add prefix if necessary
      var prefix = 'http://'
      if (link.substr(0,1) !== '/' && link.substr(0, prefix.length) !== prefix) {
        link = prefix + link;
      }
      $timeout( function () {
        window.location.href = link;
      }, 100);
    }
  };
}]);