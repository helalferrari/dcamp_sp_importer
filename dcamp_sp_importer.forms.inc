<?php

/**
 * dcamp_sp_settings_importer_form function create a form
 * @param  [array] $form
 * @param  [array] $form_state
 * @return [array] $form
 */
function dcamp_sp_settings_importer_form($form, $form_state){

  $form['amount_articles'] = array(
    '#type' => 'textfield',
    '#title' => t("Articles's amount to import"),
    '#required' => TRUE,
    '#size' => 4,
    '#maxlength' => 4,
    '#default_value' => 100,
    '#element_validate' => array('element_validate_number'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * dcamp_sp_settings_importer_form_submit form submit
 * @param  [array] $form       [description]
 * @param  [array] $form_state [description]
 */
function dcamp_sp_settings_importer_form_submit(&$form, &$form_state){

  $article_number = variable_get(DCAMP_SP_IMPORTER_ARTICLE_NUMBER);

  // Here I verify if is first time to pass here
  if ($article_number == NULL) {

    variable_set(DCAMP_SP_IMPORTER_ARTICLE_NUMBER, 1);
  }

  // Batch's definitions
  $batch = array(
    'title' => t('Importing'),
    'operations' => array(
      array('dcamp_sp_importer_importing', array(
          $form_state['values']['amount_articles'],
          $article_number,
        )
      ),
    ),
    'finished' => 'dcamp_sp_importer_importing_finished',
    'title' => t('Processing Importer DCAMP'),
    'init_message' => t('Importer DCAMP is starting.'),
    'error_message' => t('Importer DCAMP has encountered an error.'),
    'progress_message' => t('Article importing'),
    'file' => drupal_get_path('module', 'dcamp_sp_importer') . '/dcamp_sp_importer.batchs.inc'
  );

  // I do not set a batch_process because here is a form submiting
  // For more details access: http://drupal.org/node/180528
  batch_set($batch);
}
