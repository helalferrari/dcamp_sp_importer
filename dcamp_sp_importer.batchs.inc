<?php

/**
 * dcamp_sp_importer_importing
 * @param  [int] $amount_articles
 * @param  [int] $article_number
 * @param  [array] $context
 */
function dcamp_sp_importer_importing($amount_articles, $article_number, &$context){

  if (empty($context['sandbox'])) {

    // Starting variables
    $context['sandbox']['progress'] = 1;
    $context['sandbox']['processed'] = 0;
    $context['sandbox']['max'] = $amount_articles;
    $context['sandbox']['current_pos'] = 0;
    $context['sandbox']['article_number'] = $article_number;
  }

  // Define the start and end to the For
  $start = $context['sandbox']['current_pos'];
  $end = $context['sandbox']['current_pos'] + floor($context['sandbox']['max'] / 100);

  for ($i=$start; $i < $end; $i++) {

    // Prepare a node object
    $node = new stdClass();
    $node->is_new = TRUE;
    $node->type = 'article';
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->title = t('Article # @number', array(
        '@number' => $i,
      )
    );
    $node->body[$node->language][0]['value'] = t('Oppa gangnam style!, Oppa gangnam style!, Oppa gangnam style!');
    $node->uid = 1;

    node_save($node);

    if (!empty($node->nid)) {

      $context['sandbox']['processed']++;
      $context['sandbox']['article_number']++;
      $context['results'][] = $node->nid;
    }

    // Import Status
    $context['message'] =
        t("
          Articles: <br>
          - Progress: @progress <br>
          - Processed: @processed <br>
          - Total: @max", array(
          '@progress' => $context['sandbox']['progress'],
          '@max' => $context['sandbox']['max'],
          '@processed' => $context['sandbox']['processed'],
        )
      );

    $context['sandbox']['progress']++;
    $context['sandbox']['current_pos']++;
  }

  // Here is verify when need to stop importing
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {

    variable_set(DCAMP_SP_IMPORTER_ARTICLE_NUMBER, $context['sandbox']['article_number']);
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * dcamp_sp_importer_importing_finished
 * @param  [boolean] $success
 * @param  [array] $results
 * @param  [type] $operations
 */
function dcamp_sp_importer_importing_finished($success, $results, $operations){

  if ($success) {

    $message = format_plural(count($results), 'One article processed.', '@count articles processed.');
    watchdog('dcamp_sp_importer', $message, array(), WATCHDOG_NOTICE, NULL);
  }
  else {

    $message = t('Finished with an error.');
    watchdog('dcamp_sp_importer', $message, array(), WATCHDOG_ALERT, NULL);
  }

  drupal_set_message($message);
  drupal_goto(DCAMP_SP_IMPORTER_PATH);

}
