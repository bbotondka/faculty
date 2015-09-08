<?php foreach($rows as $row): ?>
  <div>
    <h3><?php print $row['field_section_title']; ?></h3>
    <p><?php print $row['field_section_desc']; ?></p>

    <?php //print_r($row['field_section_option']); ?>

    <?php print isset($row['field_section_option']['accordion']) ? 'Accordion mode' : ''; ?>
    <?php print isset($row['field_section_option']['grey_bg']) ? 'grey background' : ''; ?>

    <?php foreach($row['field_bloc'] as $bloc): ?>
      <?php print $bloc['field_size']; ?>
      <?php print $bloc['field_content']; ?>
    <?php endforeach; ?>

  </div>
<?php endforeach; ?>
