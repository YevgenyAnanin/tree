<span id="fields-header">Field List</span></br>
<div class="fields-header-description" >Drag the following fields into the "Column" box, in any order, to define the layout of this pop-up form</div>
<ul id="field-list" class="items">
  <li id="01" class="list">Item 1</li>
  <li id="02" class="list">Item 2</li>
  <li id="03" class="list">Item 3</li>
  <li id="04" class="list">Item 4</li>
  <li id="05" class="list">Item 5</li>
</ul>
<br>
<span class="column-header">Column</span>
<ul id="column-1" class="items">
</ul>
<br>
<div id="submit-form">
  <?php $reorder_form = drupal_get_form('smg_pop_up_layout_form', 'one_column', $node); ?>
  <?php print drupal_render($reorder_form); ?>
</div>