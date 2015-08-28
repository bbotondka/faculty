<article id="node-<?php print $node->nid; ?>" class="TEASER DISPLAY TEST CLASS<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

    <?php
    // @Todo: Create custom template to render header of page.
    ?>
    <?php
    /**
     * Render faculty_top_image_content.
     */
    print theme('faculty_top_image_content', array(
        // Get image url (mobile or desktop version).
        'field_image_url' => render($content['field_image']),
        'field_mobile_image_url' => render($content['field_mobile_image']),
        'title_prefix' => render($title_prefix),
        'title_attributes' => $title_attributes,
        'title' => $title,
        'title_suffix' => render($title_suffix),
        'page' => $page,
        'node' => $node
    ));
    ?>

    <?php print $user_picture; ?>

    <?php if ($display_submitted): ?>
        <div class="submitted">
            <?php print $submitted; ?>
        </div>
    <?php endif; ?>

    <div class="container"<?php print $content_attributes; ?>>
        <?php
        // We hide the comments and links now so that we can render them later.
        hide($content['comments']);
        hide($content['links']);
        hide($content['field_image']);
        hide($content['field_mobile_image']);
        print render($content);
        ?>


        <?php print render($content['links']); ?>

        <?php print render($content['comments']); ?>
    </div>
</article>
