<div id="container">

    <header>
        <div class="container">
            <div class="mobile-menu">
                <a class="nav-trigger" href="#">
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </a><!--
                --><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo-mobile"><img src="<?php print base_path() . path_to_theme(); ?>/build/images/logo-mobile.png" /></a><!--
                --><a class="extra-trigger" href="#">

                </a>
            </div>
            <div class="col-1-4 no-lr-padding">
                <?php if ($site_name || $logo): ?>
                    <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print base_path() . path_to_theme(); ?>/build/images/logo.jpg" /></a>
                <?php endif; ?>
            </div>
            <div class="col-1-2 site-slogan">
                <?php if ($site_slogan): ?>
                    <?php print $site_slogan; ?>
                <?php endif; ?>
            </div>


            <div class="no-lr-padding side-menu">
              <div class="side-menu-content">
                <?php print render($page['header']); ?>
              </div>
            </div>
            <!--div class="nav-container">
                <?php if ($main_menu): ?>
                    <nav>
                        <?php
                        print theme('links__system_main_menu', array(
                            'links' => $main_menu,
                            'attributes' => array(
                                'id' => 'main-menu',
                                'class' => array('links', 'inline', 'clearfix')
                            ),
                                        //'heading' => t('Main menu')
                        ));
                        ?>
                    </nav>
                <?php endif; ?>
            </div-->
        </div>
    </header>
    <div>
        <?php if ($breadcrumb): ?>
            <div id="breadcrumb"><?php print $breadcrumb; ?></div>
        <?php endif; ?>

        <?php print $messages; ?>

        <section id="main">

            <div id="content">
                <div >
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

                    <?php //print $feed_icons;  ?>
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
    </div>

    <footer>

        <?php if ($page['top_footer']): ?>
            <?php print render($page['top_footer']); ?>
        <?php endif; ?>
        <div class="custom-region-map">
            <?php print render($page['footer']); ?>
            <ul class="custom-mobile-actions">
                <li><a href="#"><img src="<?php print base_path() . path_to_theme(); ?>/build/images/icon-phone.png" /><p><?php print t('Je souhaite etre contacte'); ?></p></a></li>
                <li><a href="#"><img src="<?php print base_path() . path_to_theme(); ?>/build/images/icon-email.png" /><p><?php print t('Je souhaite vous ecrire'); ?></p></a></li>
                <li><a href="#"><img src="<?php print base_path() . path_to_theme(); ?>/build/images/icon-info.png" /><p><?php print t('Informations & acces'); ?></p></a></li>
            </ul>
        </div>
        <?php if ($page['bottom_footer']): ?>
            <?php print render($page['bottom_footer']); ?>
        <?php endif; ?>

        <?php if ($page['mention']): ?>
            <?php print render($page['mention']); ?>
        <?php endif; ?>
    </footer>

</div> <!-- /#container -->
