<div <?php print $attributes;?> class="<?php print $classes;?>">
  <?php if($section == 'frontend') :?>
    <div class="nav-collapse always-show">
  <?php endif;?>
  <?php print $content;?>
  <?php if($section == 'frontend') :?>
    </div>
  <?php endif;?>
</div>
