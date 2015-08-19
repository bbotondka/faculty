	<div id="container">

		<header>
			<?php if ($site_name || $logo): ?>
				<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><?php print $site_name; ?></a>
			<?php endif; ?>

			<?php if ($site_slogan): ?>
				<?php print $site_slogan; ?>
			<?php endif; ?>

			<?php print render($page['header']); ?>
		</header>

		<?php if ($breadcrumb): ?>
		<div id="breadcrumb"><?php print $breadcrumb; ?></div>
		<?php endif; ?>

    	<?php print $messages; ?>

		<section id="main">

			<div id="content">
				<?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
				<a id="main-content"></a>
				<?php print render($title_prefix); ?>
				<?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
				<?php print render($title_suffix); ?>
				<?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
				<?php print render($page['help']); ?>
				<?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

				<?php if ($page['top_content']): ?>
					<?php print render($page['top_content']); ?>
				<?php endif; ?>

				<?php print render($page['content']); ?>

				<?php if ($page['middle_grey_content']): ?>
					<?php print render($page['middle_grey_content']); ?>
				<?php endif; ?>

				<?php if ($page['middle_white_content']): ?>
					<?php print render($page['middle_white_content']); ?>
				<?php endif; ?>

				<?php if ($page['bottom_grey_content']): ?>
					<?php print render($page['bottom_grey_content']); ?>
				<?php endif; ?>

				<?php if ($page['bottom_white_content']): ?>
					<?php print render($page['bottom_white_content']); ?>
				<?php endif; ?>

				<?php //print $feed_icons; ?>
			</div>

			<?php if ($page['sidebar_first']): ?>
				<aside id="sidebar-first">
					<?php print render($page['sidebar_first']); ?>
				</aside>
			<?php endif; ?>

			<?php if ($page['sidebar_second']): ?>
				<aside id="sidebar-second">
					<?php print render($page['sidebar_second']); ?>
				</aside>
			<?php endif; ?>

		</section> <!-- /#main -->

		<footer>

			<?php if ($page['top_footer']): ?>
				<?php print render($page['top_footer']); ?>
			<?php endif; ?>

			<?php print render($page['footer']); ?>

			<?php if ($page['bottom_footer']): ?>
				<?php print render($page['bottom_footer']); ?>
			<?php endif; ?>

			<?php if ($page['mention']): ?>
				<?php print render($page['mention']); ?>
			<?php endif; ?>
		</footer>

	</div> <!-- /#container -->
