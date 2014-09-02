<div id="videoWidget" ng-controller="VideoListCtrl" video-height>
  <div id="videoWidget-wrapper" resizable>
    <div id="videoWidget-left-selection" ng-click="videosPrev()" class="videoWidget-count-selector clearfix" ng-class="moverCssClasses">
      <div id="videoWidget-left-triangle-wrapper" class="videoWidget-triangle-wrapper" ng-class="moverCssClasses">
        <div id="videoWidget-triangle-left" class="videoWidget-triangle"></div>
      </div>
    </div>
    <video-slider videos="videos" /></video-slider>
    <div id="videoWidget-right-selection" ng-click="videosNext()" class="videoWidget-count-selector clearfix" ng-class="moverCssClasses">
      <div id="videoWidget-right-triangle-wrapper" class="videoWidget-triangle-wrapper" ng-class="moverCssClasses">
        <div id="videoWidget-triangle-right" class="videoWidget-triangle"></div>
      </div>
    </div>
  </div>
</div>