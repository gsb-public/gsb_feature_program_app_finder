<?php

function gsb_feature_program_app_finder_preprocess_gsb_feature_program_app_finder_program_instance_list(&$vars) {
  $program_instances = array();
  foreach($vars as $key => $program_instance) {
    if (is_numeric($key)) {
      $program_instances[$key] = $program_instance;
    }
  }
  $pi_list = array();
  foreach(array_values($program_instances) as $pi_nid) {
    $instance_wrapper = entity_metadata_wrapper('node', $pi_nid);
    $instance_text = $instance_wrapper->field_instance_text->value();
    $instance_label = $instance_wrapper->field_instance_label->value();
    $instance_date = $instance_wrapper->field_instance_date->value();
    $start_date = new DateTime($instance_date['value']);
    $formatted_date = $start_date->format('d M Y');
    if (!empty($instance_date['value2'])) {
      $end_date = new DateTime($instance_date['value2']);
      $formatted_date = $formatted_date . ' – ' . $end_date->format('d M Y');
    }
    $instance_full_title = $formatted_date;
    if ($instance_label) {
      $instance_full_title = $instance_label . ' | ' . $formatted_date;
    }
    $program = $instance_wrapper->field_program->value();
    $target_id = $program->nid;
    $apower_id = $instance_wrapper->field_apower_id->value();
    $application_deadlines = $instance_wrapper->field_application_deadlines->value();
    $application_deadline_date = null;
    $application_deadline_text = '';
    if (count($application_deadlines) > 0) {
      $last_deadline = array_pop($application_deadlines);
      $application_deadline_date = new DateTime($last_deadline->field_date['und'][0]['value']);
      $application_deadline_date = $application_deadline_date->format('d M Y');
      $application_deadline_text = !empty($last_deadline->field_deadline_text['und'][0]['value']) ? $last_deadline->field_deadline_text['und'][0]['value'] : '';
    }
    $markup =
      '<span ' .
        'data-program-instance-nid="' . $pi_nid . '"' .
        'data-program-instance-target-id="' . $target_id . '"' .
        'data-program-instance-full-title="' . $instance_full_title . '"' .
        'data-program-instance-apower-id="' . $apower_id . '"' .
      '></span>';
    $pi_list['program-'. $pi_nid] = $markup;
  }
  $vars['program_instance_list'] = theme('item_list', array('items' => $pi_list));
}
