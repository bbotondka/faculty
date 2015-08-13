<?php
/**
 * @file
 * faculty_user.features.inc
 */

/**
 * Implements hook_default_profile2_type().
 */
function faculty_user_default_profile2_type() {
  $items = array();
  $items['professor'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "professor",
    "label" : "Professor",
    "weight" : "0",
    "data" : { "registration" : 1 },
    "rdf_mapping" : []
  }');
  $items['professor_detail'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "professor_detail",
    "label" : "Professor detail",
    "weight" : "0",
    "data" : { "registration" : 1 },
    "rdf_mapping" : []
  }');
  return $items;
}
