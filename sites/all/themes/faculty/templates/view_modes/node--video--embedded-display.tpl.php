<div class="video-wrapper">
    <h1><?php print render($title); ?></h1>
    <div class="video-subtitle">
        <?php print render($content['field_subtitle']); ?>
    </div>
    <div class="video-thumb">
        <?php print render($content['field_image']); ?>
        <?php print render($content['field_mobile_image']); ?>
        <button type="button" class="btn btn-primary btn-lg" data-toggle="modal" data-target="#myModal"></button>
    </div>
    <div class="video-more">
        <?php print render($content['field_link']); ?>
    </div>
    <!-- Modal -->
    <div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-body">
                    <?php print render($content['field_youtube']); ?>
                </div>
            </div>
        </div>
    </div>
</div>