<?php

function gsb_feature_program_app_finder_preprocess_gsb_feature_program_app_finder_program_list(&$vars) {
  $programs = array();
  foreach($vars as $key => $program) {
    if (is_numeric($key)) {
      $markup =
        '<span ' .
          'data-program-nid="' . $program->nid . '"' .
          'data-program-title="' . $program->title . '"' .
          'data-sample-file_fid="' . $program->field_sample_application_file_fid . '"' .
        '></span>';
      $programs['program-'. $program->nid] = $markup;
    }
  }
  $vars['program_list'] = theme('item_list', array('items' => $programs));
}
