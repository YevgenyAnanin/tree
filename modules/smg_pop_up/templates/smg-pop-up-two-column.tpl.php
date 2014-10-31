<div class="column-wrapper column-1">
  <span class="column-header">Column One</span>
  <ul id="column-1" class="items">
    <?php foreach($components['column1'] as $col1_row): ?>
      <?php foreach($col1_row as $element): ?>
        <?php 
          $component_name = $components['component_names'][$element['id']]['name']; 
          $component_name = ($component_name != 'Other') ? $component_name : $component_name . ' (' . $element['id'] . ')';
        ?>
        <li id="<?php echo $element['id']; ?>" class="list" style="width:<?php echo $element['width']; ?>%"><?php echo $component_name; ?></li>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </ul>
</div>
<div class="column-wrapper column-2">
  <span class="column-header">Column Two</span>
  <ul id="column-2" class="items">
    <?php foreach($components['column2'] as $col2_row): ?>
      <?php foreach($col2_row as $element): ?>
        <?php 
          $component_name = $components['component_names'][$element['id']]['name']; 
          $component_name = ($component_name != 'Other') ? $component_name : $component_name . ' (' . $element['id'] . ')';
        ?>        
        <li id="<?php echo $element['id']; ?>" class="list" style="width:<?php echo $element['width']; ?>%"><?php echo $component_name; ?></li>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </ul>
</div>
<br>
<span id="fields-header">Field List</span></br>
<div class="fields-header-description" >Drag the following fields into the "Column" box, in any order, to define the layout of this pop-up form.  Then click the "Generate Layout" button below.</div>
<ul id="field-list" class="items">
  <?php foreach($components['fields'] as $field): ?>
    <?php $component_name = ($field['name'] != 'Other') ? $field['name'] : $field['name'] . ' (' . $field['form_key'] . ')'; ?>
    <?php if($field['type'] != 'smg_hidden'): ?>
      <li id="<?php echo $field['form_key']; ?>" class="list"><?php echo $component_name; ?></li>
    <?php endif; ?>
  <?php endforeach; ?>
</ul>
<div id="submit-form">
  <?php $reorder_form = drupal_get_form('smg_pop_up_layout_form', 'two_column', $node); ?>
  <?php print drupal_render($reorder_form); ?>
</div>