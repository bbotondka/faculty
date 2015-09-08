<?php foreach($rows as $row): ?>
  <div>
    <h3><?php print $row['field_section_title']; ?></h3>
    <p><?php print $row['field_section_desc']; ?></p>

    <?php print (!empty($row['field_section_option']['accordion'])) ? 'Accordion mode' : 'No accordion mode'; ?>
    <?php print (!empty($row['field_section_option']['grey_bg'])) ? 'grey background' : 'white background'; ?>

    <?php foreach($row['field_bloc'] as $bloc): ?>
      <div class="col <?php print (!empty($bloc['field_size'])) ? 'col-' . $bloc['field_size'] : ''; ?>">
        <?php print $bloc['field_content']['value']; ?>
      </div>
    <?php endforeach; ?>

  </div>
<?php endforeach; ?>
