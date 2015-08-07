<?php

/**
 * Custom setting page to manage footer blocks.
 *
 * @return mixed
 *   We render form.
 */
function _faculty_general_admin_footer() {
	$form = array();

	// Logo mobile.
	$form['logo_mobile'] = array(
		'#weight' => -50,
		'#type' => 'managed_file',
		'#title' => t('Logo mobile'),
		'#description' => t('Allowed extensions: gif png jpg jpeg'),
		'#default_value' => variable_get('logo_mobile', ''),
		'#upload_location' => 'public://images',
		'#upload_validators' => array(
			'file_validate_extensions' => array('gif png jpg jpeg'),
			// Pass the maximum file size in bytes
			//'file_validate_size' => array(MAX_FILE_SIZE*1024*1024),
		)
	);

	// If there is already an uploaded image display the image here.
	if ($image_fid = variable_get('logo_mobile', FALSE)) {
		$image_load = file_load($image_fid);
		$form['logo_mobile_thumbnail'] = array(
			'#weight' => -40,
			'#markup' => theme('image_style', array(
				'path' => $image_load->uri,
				'style_name' => 'thumbnail'
			)),
		);
	}


	// Logo desktop.
	$form['logo_desktop'] = array(
		'#weight' => -30,
		'#type' => 'managed_file',
		'#title' => t('Logo desktop'),
		'#description' => t('Allowed extensions: gif png jpg jpeg'),
		'#default_value' => variable_get('logo_desktop', ''),
		'#upload_location' => 'public://images',
		'#upload_validators' => array(
			'file_validate_extensions' => array('gif png jpg jpeg'),
			// Pass the maximum file size in bytes
			//'file_validate_size' => array(MAX_FILE_SIZE*1024*1024),
		)
	);

	// If there is already an uploaded image display the image here.
	if ($image_fid = variable_get('logo_desktop', FALSE)) {
		$image_load = file_load($image_fid);
		$form['logo_logo_desktop_thumbnail'] = array(
			'#weight' => -20,
			'#markup' => theme('image_style', array(
				'path' => $image_load->uri,
				'style_name' => 'thumbnail'
			)),
		);
	}

	// Get enabled languages.
	$enabled_languages = _faculty_general_get_enabled_languages();
	// For each enabled languages.
	foreach($enabled_languages as $langcode => $language) {

		// One fieldset by language.
		$form[$langcode] = array(
			'#type' => 'fieldset',
			'#title' => t('Language: !langname', array(
				'!langname' => $language->name,
			)),
			'#weight' => -5,
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

			// One fieldset by block.
			$form[$langcode]['virtual_visit'] = array(
				'#type' => 'fieldset',
				'#title' => t('Virtual visit'),
				'#weight' => -5,
				'#collapsible' => TRUE,
				'#collapsed' => TRUE,
			);

				// Virtual visit label.
				$form[$langcode]['virtual_visit']['virtual_visit_label_' . $langcode] = array(
					'#type' => 'textfield',
					'#title' => t('Virtual visit label'),
					'#default_value' => variable_get('virtual_visit_label_' . $langcode, ''),
				);

				// Virtual visit link.
				$form[$langcode]['virtual_visit']['virtual_visit_link_' . $langcode] = array(
					'#type' => 'textfield',
					'#title' => t('Virtual visit link'),
					'#default_value' => variable_get('virtual_visit_link_' . $langcode, ''),
					'#element_validate' => array('_faculty_general_url_validate'),
				);

	} // End foreach - fieldset by language.

	// Add submit function to save permanently all files.
	$form['#submit'][] = '_faculty_general_save_file_permanently';

	return system_settings_form($form);
}

/**
 * #element_validate used by Virtual visit link field.
 *
 * @param $element
 * @param $form_state
 * @param $form
 */
function _faculty_general_url_validate($element, &$form_state, $form) {
	if (empty($element['#value'])) {
		form_error($element, t('This field is required.'));
	}

	if (!drupal_valid_path($element['#value'])) {
		form_error($element, t('Error with link.'));
	}
}

/**
 * Save images permanently.
 *
 * @param $form
 * @param $form_state
 */
function _faculty_general_save_file_permanently($form, &$form_state) {
	// Declare here field image to save permanently.
	$fields_image = array(
		'logo_mobile',
		'logo_desktop',
	);
	global $user;

	// For each field image to save.
	foreach ($fields_image as $field_img) {
		// When using the #managed_file form element the file is automatically
		// uploaded an saved to the {file} table. The value of the corresponding
		// form element is set to the {file}.fid of the new file.
		//
		// If fid is not 0 we have a valid file.
		if ($form_state['values'][$field_img] != 0) {
			// The new file's status is set to 0 or temporary and in order to ensure
			// that the file is not removed after 6 hours we need to change it's status
			// to 1. Save the ID of the uploaded image for later use.
			$file = file_load($form_state['values'][$field_img]);
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);

			// When a module is managing a file, it must manage the usage count.
			// Here we increment the usage count with file_usage_add().
			file_usage_add($file, 'faculty_migrate', 'sample_image', $user->uid);

			// Save the fid of the file so that the module can reference it later.
			variable_set($field_img, $file->fid);
			drupal_set_message(t('The image @image_name was uploaded and saved with an ID of @fid and will be displayed.', array(
				'@image_name' => $file->filename,
				'@fid' => $file->fid
			)));
		}
		// If the file was removed we need to remove the module's reference to the
		// removed file's fid, and remove the file.
		elseif ($form_state['values'][$field_img] == 0) {
			// Retrieve the old file's id.
			$fid = variable_get($field_img, FALSE);
			$file = $fid ? file_load($fid) : FALSE;
			if ($file) {
				// When a module is managing a file, it must manage the usage count.
				// Here we decrement the usage count with file_usage_delete().
				file_usage_delete($file, 'faculty_migrate', 'sample_image', $user->uid);

				// The file_delete() function takes a file object and checks to see if
				// the file is being used by any other modules. If it is the delete
				// operation is cancelled, otherwise the file is deleted.
				file_delete($file);
			}

			// Either way the module needs to update it's reference since even if the
			// file is in use by another module and not deleted we no longer want to
			// use it.
			variable_set($field_img, FALSE);
			if (!empty($file->filename)) {
				drupal_set_message(t('The image @image_name was removed.', array('@image_name' => $file->filename)));
			}
		}

	}

}