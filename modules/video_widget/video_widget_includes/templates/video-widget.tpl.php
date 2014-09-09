<div class="videoWidget" ng-controller="VideoListCtrl" >
  <div class="videoWidget-wrapper" resizable>
    <div ng-click="videosPrev()" class="videoWidget-count-selector videoWidget-left-selection clearfix" ng-class="moverCssClasses">
      <div class="videoWidget-triangle-wrapper videoWidget-left-triangle-wrapper" ng-class="moverCssClasses">
        <div class="videoWidget-triangle videoWidget-triangle-left"></div>
      </div>
    </div>
    <video-slider videos="videos" /></video-slider>
    <div ng-click="videosNext()" class="videoWidget-count-selector videoWidget-right-selection clearfix" ng-class="moverCssClasses">
      <div class="videoWidget-triangle-wrapper videoWidget-right-triangle-wrapper" ng-class="moverCssClasses">
        <div class="videoWidget-triangle videoWidget-triangle-right"></div>
      </div>
    </div>
  </div>
</div>