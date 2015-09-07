<?php
/**
 * @file
 * faculty_image_styles.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function faculty_image_styles_image_default_styles() {
  $styles = array();

  // Exported image style: desktop_320x215.
  $styles['desktop_320x215'] = array(
    'label' => 'desktop 320x215',
    'effects' => array(
      3 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 320,
          'height' => 215,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: desktop_730x495.
  $styles['desktop_730x495'] = array(
    'label' => 'desktop 730x495',
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 730,
          'height' => 495,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: mobile_350x235.
  $styles['mobile_350x235'] = array(
    'label' => 'mobile 350x235',
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 350,
          'height' => 235,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
