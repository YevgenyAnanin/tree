<?php
/**
 * @file
 *   Template file for the datacard block.
 */?>

<div class="ld-header">
  <div class="ld-header-top-text">Looking for leading suppliers?</div>
  <div class="ld-header-bottom-text">Mouse over to see these companies:</div>
</div>

<?php foreach($category_content as $taxonomy_tid => $data): ?>
  <script type="text/javascript">_gaq.push(['_trackEvent', 'Leadership - <?php print $taxonomy_name[$taxonomy_tid]; ?>', 'Viewed', 0, true]);</script>
  <h4><?php print $taxonomy_name[$taxonomy_tid]; ?></h4>
  <ul class="leadership-menu">
    <?php foreach($data as $content): ?>
      <?php if ($content->data_card_processed_for_angular['node']->status == 1): ?>
        <li ng-controller="leadershipLinkCtrl" leadership-link>
          <div class="icon-section">
            <img class="icon" src="<?php print $company_link_icon; ?>" width="16" height="13" />
          </div>
          <div class="link-section">
            <a onclick="_gaq.push(['_trackEvent', 'Leadership action', 'Profile click', ' <?php print  $content->data_card_processed_for_angular['node']->title; ?> ']);" class="leadership-link" href="<?php print $content->alias; ?>" style="background-color:#FFF;"><?php print $content->title; ?></a>
            <?php if ($content->field_youtube_amplified_value == 1): ?>
              <span class="youtube-amplified">
                <span class="circle">
                  <span class="triangle"></span>
                </span>
              </span>
            <?php endif; ?>
          </div>
          <leadership-data-card tid="<?php print $taxonomy_tid; ?>" nid="<?php print $content->datacard_nid; ?>" site="<?php print $site_id; ?>" hover-state="hoverState"></leadership-data-card>
        </li>
      <?php endif; ?>
    <?php endforeach; ?>
  </ul>
<?php endforeach; ?>
