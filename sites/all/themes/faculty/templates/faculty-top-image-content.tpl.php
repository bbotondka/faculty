<div style="background-image: url('<?php print $field_image_url; ?>');" data-mobile="<?php print $field_mobile_image_url; ?>">
	<?php print $title_prefix; ?>
	<?php
	if (!$page): ?>
		<h1<?php print $title_attributes; ?>><?php print $title; ?></h1>
	<?php else : ?>
		<h3<?php print $title_attributes; ?>><?php print $title; ?></h3>
	<?php endif; ?>

	<?php print $title_suffix; ?>
</div>
