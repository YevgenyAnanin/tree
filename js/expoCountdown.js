var expoCountdown = angular.module('expoCountdown', []);

expoCountdown.controller('expoCountdownCtrl', ['$scope', '$attrs', '$interval', function ($scope, $attrs, $interval) {

  var settings = Drupal.settings.expoCountdown;

  $scope.expoName = settings.expoName;

  $scope.registerUrl = settings.registerUrl;

  $scope.logoPath = settings.logo_path;

  $scope.day = parseInt(settings.utcDay);

  $scope.hour = parseInt(settings.utcHour);

  $scope.month = parseInt(settings.utcMonth);

  $scope.year = parseInt(settings.utcYear);

  $scope.min = parseInt(settings.utcMin);

  $scope.date = new Date(Date.UTC($scope.year, ($scope.month - 1), $scope.day, $scope.hour, $scope.min, 0, 0));

  $scope.currentDate = new Date();

  /**
   * Given the current date/time, determine how many days/minutes/hours until the
   * expo
   * @param currentDate
   */
  $scope.determineCountdownTimes = function (currentDate) {
    $scope.dayCountdown = parseInt( ($scope.date - $scope.currentDate) / (24*3600*1000) );

    $scope.hourCountdown = parseInt( (( $scope.date - $scope.currentDate ) / (3600*1000)) - ( $scope.dayCountdown * 24) );

    $scope.minCountdown =  parseInt( (( $scope.date - $scope.currentDate ) / ( 60*1000 )) - (( $scope.hourCountdown * ( 60 )) + ( $scope.dayCountdown * (24*60) ) ) );
  };

  // Run the determineCountdownTimes function intially
  $scope.determineCountdownTimes($scope.currentDate);

  // Set a boolean value do specify whether or not to show days remaining, or hours, or minutes
  // Days will always show, but hours/mins only when we get close to a show (exact time TBD).
  $scope.showDays = true;
  $scope.showHours = settings.showHour;
  $scope.showMins = settings.showMin;

  $scope.digitsQuantity = function (countdownType) {
    var digNum = $scope[countdownType].toString().split("").length;
    var digNumClass = 'digits-quantity-' + digNum;
    var classObj = {};
    classObj[digNumClass] = true;
    return classObj;
  };

  /**
   * For use in ng-class.  Returns a 1, 2, or 3 depending on whether or not we're showing the
   * hours and/or minute countdowns
   */
  $scope.countdownQuantity = function () {
    var quantity = 1;
    if ( $scope.showHours ) quantity++;
    if ( $scope.showMins ) quantity++;
    var quantityClass = "countdown-quantity-" + quantity;
    var classObj = {};
    classObj[quantityClass] = true;
    return classObj;
  }

  $interval(function () {
    // Change the current date every 60 seconds
    $scope.currentDate = new Date();
    // Rerun the determineCountdownTimes function
    $scope.determineCountdownTimes($scope.currentDate);
  }, 60000);

}]);

/**
 * Directive for the expo countdown widget
 */
expoCountdown.directive('expoCountdown', ['$timeout', '$window', function ($timeout, $window) {

  var expoCountdownTpl = Drupal.settings.smgAngularTemplates.expoCountdown;

  return {
    restrict: 'E',
    replace: true,
    scope: true,
    controller: 'expoCountdownCtrl',
    link: function (scope, elem, attrs) {

      var window = jQuery($window);
      
      var centerRegButton = function () {
        var widgetHeight = elem.outerHeight(true);
        var regButton = elem.find(".expo-countdown-register");
        var regButtonHeight = regButton.outerHeight(true);
        var newMarginTop = ((widgetHeight / 2) - regButtonHeight);
        if (newMarginTop < 20 && attrs.hasOwnProperty("site") && attrs.site == 'pw') {
          newMarginTop = 20;
        }
        else if (attrs.hasOwnProperty("site") && attrs.site == "hcp") {
          newMarginTop+= 3;
        }
        regButton.css({"margin-top":newMarginTop});
      }

      $timeout(function () {
        if (window.innerWidth() > 480){
          centerRegButton();
        }
      }, 800);

      // Fix widget to page on PW
      if (window.innerWidth() > 480 && attrs.hasOwnProperty("site") && attrs.site == "pw" && !jQuery("body").hasClass("logged-in")) {

        $timeout(function () {

          var elementPos = elem.offset();
          var containerWidth = jQuery("#container").width();

          var expoWidgetIsFixed = false;

          window.bind("scroll", function () {
            if ( window.scrollTop() > elementPos.top ) {
              elem.css({"position":"fixed", "top": "0", "max-width":containerWidth, "border-bottom":"1px solid #ccc"});
              expoWidgetIsFixed = true;
            }
            else {
              elem.css({"position":"static","border-bottom":"none"});
              expoWidgetIsFixed = false;
            }
          });

          window.bind("resize", function () {
            if (expoWidgetIsFixed) {
              var newContainerWidth = jQuery("#container").width();
              elem.css({"max-width":newContainerWidth});
            }
          });

        }, 100);

      }

      // Fix widget to page on HCP
      if (window.innerWidth() > 480 && attrs.hasOwnProperty("site") && attrs.site == "hcp" && !jQuery("body").hasClass("logged-in")) {

        $timeout(function () {

          var elementPos = elem.offset();
          var containerWidth = jQuery(".wrapper-middle").width();

          window.bind("scroll", function () {
            if ( window.scrollTop() > elementPos.top ) {
              elem.css({"position":"fixed", "top": "0", "max-width":containerWidth});
            }
            else {
              elem.css({"position":"static"});
            }
          });

        }, 100);

      }

    },
    templateUrl: expoCountdownTpl
  }
}]);

/**
 * Directive for the flip card widget
 */
expoCountdown.directive('countdownFlipWidget', ['$interval', function ($interval) {

  var flipWidgetTpl = Drupal.settings.smgAngularTemplates.flipWidget;

  return {
    restrict: 'E',
    replace: true,
    scope: true,
    link: function (scope, elem, attrs) {

      // Need to split the countdown into individual digits.  Store in an array.
      scope.countdownDigits = attrs.countdown.toString().split("");

      // Keeps track of changing time
      scope.currentDigit = attrs.countdown;

      scope.countdownDigitsStruct = {};

      scope.generateDigitMap = function (digits) {

        var size = 0, key;
        for (key in scope.countdownDigitsStruct) {
          if (scope.countdownDigitsStruct.hasOwnProperty(key)) size++;
        }

        if (size > digits.length) {
          var diff = size - digits.length;
          //scope.countdownDigitsStruct.splice((scope.countdownDigitsStruct.length - diff), diff);
          for (var digitKey in scope.countdownDigitsStruct) {
            if (digitKey >= digits.length) delete scope.countdownDigitsStruct[digitKey]
          }
        }
        for (var i = 0; i < digits.length; i++) {
          var digit = parseInt(digits[i]);
          if ( !scope.countdownDigitsStruct[i] )
            scope.countdownDigitsStruct[i] = {};
          for (var j = 0; j < 10; j++) {
            if ( (digit - j === 1) ) {
              if (scope.countdownDigitsStruct[i][j] !== 'flip-clock-before')
                scope.countdownDigitsStruct[i][j] = 'flip-clock-before';
            }
            else if ( (digit - j === 0) ) {
              scope.countdownDigitsStruct[i][j] = 'flip-clock-active'
            }
            else {
              scope.countdownDigitsStruct[i][j] = 'hidden';
            }
          }
        }
      }

      // Call the map generator initially
      scope.generateDigitMap(scope.countdownDigits);

      // The countdown flip widget element should have an attribute called
      // type.  Watch for this value on the parent scope.
      scope.$watch(attrs.type, function (newValue, oldValue) {
        if (newValue !== oldValue) {
          var digits = newValue.toString().split("");
          scope.generateDigitMap(digits);
        }
      });

    },
    templateUrl: flipWidgetTpl
  }

}]);

window.smgAngularDependencies.push('expoCountdown');