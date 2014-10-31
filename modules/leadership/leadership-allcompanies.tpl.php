<div class="leadership-all-companies-list">
  <div class="ld-browse-text">Browse these leading suppliers:</div>
  <?php foreach ($taxonomies as $category): ?>
    <div class="ld-cateogry-name"><?php print $category['title'] ?></div>
    <ul ng-controller="AllCompaniesBlockTermListCtrl" site="<?php print $site_id; ?>">
      <li ng-repeat="taxonomy in taxonomies" ng-if="taxonomy.parents[0] == <?php print $category['tid'] ?>">
        <taxonomy-list ></taxonomy-list>
      </li>
    </ul>
  <?php endforeach; ?>

  <div class="link-more" style="border-top:1px dotted #999;">
    <a href="/leaders">See all companies »</a>
  </div>
</div>
