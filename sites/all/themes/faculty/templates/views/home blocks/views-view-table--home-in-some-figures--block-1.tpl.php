<?php
/**
 * @file
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $caption: The caption for this table. May be empty.
 * - $header_classes: An array of header classes keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $classes: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * - $field_classes: An array of classes to apply to each field, indexed by
 *   field id, then row number. This matches the index in $rows.
 * @ingroup views_templates
 */
?>
<div class="container">
    <?php foreach ($rows as $row_count => $row): ?>
        <div class="article-block article-block-<?php print $row_count; ?>">
            <?php print $row['field_image']; ?>
            <div class="details">
                <div class="article-tags"><?php print $row['field_tags']; ?></div>
                <div class="article-title"><?php print $row['title']; ?></div>
            </div>
        </div>
    <?php endforeach; ?>
</div>
