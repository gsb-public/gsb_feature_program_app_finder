<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Exec Ed: Application Finder'),
  'description' => t('Shows a program application finder.'),
  'category' => t('Custom blocks'),
  'edit form' => 'gsb_feature_program_app_finder_edit_form',
  'render callback' => 'gsb_feature_program_app_finder_render',
  //'admin info' => 'gsb_feature_program_app_finder_admin_info',
  //'admin title' => 'gsb_feature_program_app_finder_admin_title',
  'defaults' => array('viewmodes' => '')
);

/**
 * The edit form the "Exec Ed: Application Finder" Content Panes
 */
function gsb_feature_program_app_finder_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Run-time rendering of the body of the Program Application Finder
 * See ctools_plugin_examples for more advanced info
 */
function gsb_feature_program_app_finder_render($subtype, $conf, $panel_args, $context = NULL) {

  $programs = array();

  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'status', 'type', 'created', 'title'));

  // Find all Programs that are 'Exec Ed Program'
  $category_ids = array('34911');

  if (!empty($category_ids)) {
    $query->innerJoin('field_data_field_program_category', 'pc', "n.nid = pc.entity_id AND pc.entity_type = 'node'");
    $query->fields('pc', array('field_program_category_tid'));
    $query->condition('pc.field_program_category_tid', $category_ids, 'IN');
  }

  $query->leftJoin('field_data_field_sample_application_file', 'saf', "n.nid = saf.entity_id AND saf.entity_type = 'node'");
  $query->fields('saf', array('field_sample_application_file_fid'));

  $query->condition('n.type', 'program');
  $query->condition('n.status', '1');

  $results = $query->execute();
  foreach ($results as $record) {
    $programs[$record->nid] = $record;
  }

  // Next look-up the most current program instances related to each of these programs
  $program_nids = array_keys($programs);
  $today = date('Y-m-d H:i:s', mktime(0,0,0));

  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'status', 'type'));
  $query->join('field_data_field_program', 'fp', "n.nid = fp.entity_id AND fp.entity_type = 'node' AND fp.deleted = '0'");
  $query->fields('fp', array('entity_id', 'entity_type', 'deleted', 'field_program_target_id'));
  $query->join('node', 'nodefp', 'fp.field_program_target_id = nodefp.nid');
  $query->fields('nodefp', array('nid'));
  $query->join('field_data_field_instance_date', 'fid', "n.nid = fid.entity_id AND fid.entity_type = 'node' AND fid.deleted = '0'");
  $query->fields('fid', array('field_instance_date_value'));
  $query->leftJoin('field_data_field_display_on_finder', 'df', 'n.nid = df.entity_id AND df.deleted = 0');
  $query->condition('nodefp.nid', $program_nids, 'IN');
  $query->condition('n.status', '1');
  $query->condition('n.type', 'program_instance');
  $query->condition('fid.field_instance_date_value', $today, '>=');
  $query->condition('df.field_display_on_finder_value', 1);
  $query->orderBy('fid.field_instance_date_value', 'ASC');
  $results = $query->execute();

  $program_instances = array();
  foreach($results as $instance) {
    if (empty($program_instances[$instance->field_program_target_id])) {
      $program_instances[$instance->field_program_target_id] = array();
    }
    $program_instances[$instance->field_program_target_id][] = $instance->nid;
  }

  $program_instance_wrapper = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('program-instance-wrapper'),
    ),
    'content' => array('#markup' => theme('gsb_feature_program_app_finder_program_instance_list', $program_instances))
  );

  $program_options = array();
  foreach($programs as $key => $program) {
    if (!empty($program_instances[$key])) {
      $program_options[$key] = str_replace("\xE2\x80\x8B", "", $program->title);
    }
  }

  asort($program_options);

  $states = array(
    'programs' => array(
      'options' => $program_options
    )
  );

  $form_state = array('method' => 'get', $states, 'redirect' => FALSE);
  $form = drupal_build_form('program_app_finder', $form_state);
  unset($form['form_build_id']);
  $app_finder_form = drupal_render($form);

  $form_wrapper = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('app-finder-form-wrapper'),
    ),
    'content' => array('#markup' => $app_finder_form)
  );

  return (object) array(
    'content' => array($form_wrapper, $program_instance_wrapper),
  );
}
