<?php

function gsb_feature_program_app_finder_preprocess_gsb_feature_program_app_finder_program_instance_list(&$vars) {
  global $base_url;
  $apower_base_application_url = variable_get('apower_base_application_url', '');
  $program_instances = array();
  foreach($vars as $key => $pi_array) {
    if (is_numeric($key)) {
      foreach($pi_array as $pi_nid) {
        $program_instances[] = $pi_nid;
      }
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
    $program_wrapper = entity_metadata_wrapper('node', $target_id);
    $sample_app_form_url = $program_wrapper->field_sample_application_file->value();
    if ($sample_app_form_url && !empty($sample_app_form_url['uri'])) {
      $sample_app_form_url = $sample_app_form_url['uri'];
      $sample_app_form_url = str_replace('public://', $base_url . '/sites/gsb/files/', $sample_app_form_url);
    }
    $apower_id = $instance_wrapper->field_apower_id->value();
    $application_deadlines = $instance_wrapper->field_application_deadlines->value();
    $application_url = $instance_wrapper->field_application_url->value();
    if ($application_url) {
      $application_url = $application_url['url'];
    }
    else if ($apower_id) {
      $application_url = $apower_base_application_url . $apower_id;
    }

    // Application deadlines for program
    $application_deadline_date = null;
    $application_deadline_text = '';
    if (count($application_deadlines) > 0) {
      if (count($application_deadlines) == 1) {
        $last_deadline = array_pop($application_deadlines);
        if (!empty($last_deadline->field_date)) {
          $application_deadline_date = new DateTime($last_deadline->field_date['und'][0]['value']);
          $application_deadline_date = $application_deadline_date->format('d M Y');
        }
        $application_deadline_text = !empty($last_deadline->field_deadline_text['und'][0]['value']) ? $last_deadline->field_deadline_text['und'][0]['value'] : '';
        $application_deadline_full_text = $application_deadline_date;
        if ($application_deadline_text != '') {
          $application_deadline_full_text = $application_deadline_text;
          if ($application_deadline_date) {
            $application_deadline_full_text .= ' | ' . $application_deadline_date;
          }
        }
        $application_deadline_full_text = t('<b>Deadline</b> ^') . $application_deadline_full_text;
      }
      else {
        $application_deadlines = array_reverse($application_deadlines);
        $application_deadline_full_text = t('<b>Deadlines</b>');
        foreach ($application_deadlines as $key => $application_deadline) {
          if (!empty($application_deadline->field_date)) {
            $application_deadline_date = new DateTime($application_deadline->field_date['und'][0]['value']);
            $application_deadline_date = $application_deadline_date->format('d M Y');
          }
          $application_deadline_text = !empty($application_deadline->field_deadline_text['und'][0]['value']) ? 
                            $application_deadline->field_deadline_text['und'][0]['value'] : '';
          $application_deadline_full_text .= '^'. $application_deadline_text;
          if ($application_deadline_date) {
            $application_deadline_full_text .= ' | ' . $application_deadline_date;
          }
        }
      }
    }
    $markup =
      '<span ' .
        'data-program-instance-nid="' . $pi_nid . '"' .
        'data-program-instance-target-id="' . $target_id . '"' .
        'data-program-instance-full-title="' . $instance_full_title . '"' .
        'data-program-instance-apower-id="' . $apower_id . '"' .
        'data-program-instance-deadline="' . $application_deadline_full_text . '"' .
        'data-program-instance-sample-app-form-url="' . $sample_app_form_url . '"' .
        'data-program-instance-application-url="' . $application_url . '"' .
      '></span>';
    $pi_list['program-'. $pi_nid] = $markup;
  }
  $vars['program_instance_list'] = theme('item_list', array('items' => $pi_list));
}
