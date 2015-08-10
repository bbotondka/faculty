<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

	<?php
		// @Todo: Create custom template to render header of page.
	?>
	<div style="background-image: url('<?php print render($content['field_image']); ?>');">
		<?php print render($title_prefix); ?>
		<?php if (!$page): ?>
			<h1<?php print $title_attributes; ?>><?php print $title; ?></h1>
		<?php endif; ?>

		<?php print render($title_suffix); ?>
	</div>

	<?php print $user_picture; ?>

	<?php if ($display_submitted): ?>
		<div class="submitted">
			<?php print $submitted; ?>
		</div>
	<?php endif; ?>

	<div class="content"<?php print $content_attributes; ?>>
		<?php
		// We hide the comments and links now so that we can render them later.
		hide($content['comments']);
		hide($content['links']);
		hide($content['field_image']);
		print render($content);
		?>
	</div>

	<?php print render($content['links']); ?>

	<?php print render($content['comments']); ?>

</article>
