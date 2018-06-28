<?php

/**
 * @file
 * Admin form functions for Islandora DOI Block.
 */

/**
 * Admin form builder.
 */
function islandora_doi_block_admin_form($form, $form_settings) {
  module_load_include('inc', 'islandora', 'includes/utilities');

  $all_cmodels = islandora_get_content_models();
  $already_chosen = variable_get('islandora_doi_block_selected_cmodels', array('ir:citationCModel', 'ir:thesisCModel'));
  $selected_rows = drupal_map_assoc($already_chosen);

  $form['islandora_doi_block_source_dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('DSID for the datastream that contains the DOI'),
    '#default_value' => variable_get('islandora_doi_block_source_dsid', 'MODS'),
  );

  $form['islandora_doi_block_namespace_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace prefix to use in the XPath expression'),
    '#default_value' => variable_get('islandora_doi_block_namespace_prefix', 'mods'),
  );

  $form['islandora_doi_block_namespace_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace URI associated with the above namespace prefix'),
    '#default_value' => variable_get('islandora_doi_block_namespace_uri', 'http://www.loc.gov/mods/v3'),
  );

  $form['islandora_doi_block_doi_xpath'] = array(
    '#type' => 'textfield',
    '#title' => t('DOI XPath'),
    '#description' => t('XPath expression for retrieving the DOI.'),
    '#default_value' => variable_get('islandora_doi_block_doi_xpath', '/mods:mods/mods:identifier[@type="doi"]'),
  );

  $form['content_model_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure which content models will display the DOI block.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['content_model_wrapper']['doi_block_table'] = array(
    '#type' => 'tableselect',
    '#title' => t('Enable custom pathauto settings for these content models'),
    '#tree' => TRUE,
    '#multiple' => TRUE,
    '#header' => array(
      'label' => array('data' => t('Content-Model')),
    ),
    '#default_value' => $selected_rows,
  );

  $options = array();
  foreach ($all_cmodels as $cmodel) {
    $pid = $cmodel['pid'];
    $label = $cmodel['label'];
    $options[$pid] = array('label' => array('data' => $label));
  }
  $form['content_model_wrapper']['doi_block_table']['#options'] = $options;

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 0,
  );

  return $form;
}

/**
 * Form validation handler.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_doi_block_admin_form_validate(array $form, array &$form_state) {
  if (!isset($form_state['values']['islandora_doi_block_doi_xpath']) ||
    empty($form_state['values']['islandora_doi_block_doi_xpath'])) {
    form_set_error(
      'islandora_doi_block_doi_xpath',
      t('You must set the XPath to find the DOI identifier.')
    );
  }
  if (!array_keys(array_filter($form_state['values']['doi_block_table']))) {
    form_set_error(
      'doi_block_table',
      t("Please select at least one content model to display the DOI block on.")
    );
  }
}

/**
 * Submit handler for admin form.
 */
function islandora_doi_block_admin_form_submit(array $form, array &$form_state) {
  $enabled = array_keys(array_filter($form_state['values']['doi_block_table']));
  variable_set('islandora_doi_block_selected_cmodels', $enabled);
  variable_set('islandora_doi_block_source_dsid', $form_state['values']['islandora_doi_block_source_dsid']);
  variable_set('islandora_doi_block_namespace_prefix', $form_state['values']['islandora_doi_block_namespace_prefix']);
  variable_set('islandora_doi_block_namespace_uri', $form_state['values']['islandora_doi_block_namespace_uri']);
  variable_set('islandora_doi_block_doi_xpath', $form_state['values']['islandora_doi_block_doi_xpath']);
  drupal_set_message(t("Configuration saved."));
}
