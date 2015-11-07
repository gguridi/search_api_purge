<?php
/**
 * @file
 * Search API Purge batch handlers.
 */

/**
 * Form submission handler for search_api_admin_index_status_form().
 *
 * @see search_api_purge_form_alter()
 */
function search_api_purge_batch(array $form, array &$form_state) {
  if (isset($form_state['input'][SearchApiPurgeForm::PURGE_BUTTON])) {
    $index = $form_state['index'];
    $batch = $form_state['input'][SearchApiPurgeForm::PURGE_BATCH];
    search_api_purge_batch_create($index, $batch);
    batch_process('admin/config/search/search_api/index/' . $index->id);
    $form_state['submitted'] = TRUE;
  }
}

/**
 * Batch 'creation' callback.
 *
 * @param SearchApiIndex $index
 *   Index to use.
 * @param int $batch_size
 *   Batch size.
 */
function search_api_purge_batch_create(SearchApiIndex $index, $batch_size) {
  $batch = array(
    'title' => t('Purging items'),
    'operations' => array(
      array('search_api_purge_batch_process', array($index, $batch_size)),
    ),
    'progress_message' => t('Completed about @percentage% of the purging operation.'),
    'finished' => 'search_api_purge_batch_finished',
    'file' => drupal_get_path('module', 'search_api_purge') . '/search_api_purge.batch.inc',
  );
  batch_set($batch);
}

/**
 * Batch Operation Callback.
 *
 * @param SearchApiIndex $index
 *   Index to use.
 * @param int $batch_size
 *   Batch size.
 * @param array $context
 *   Context set by the batch process.
 */
function search_api_purge_batch_process(SearchApiIndex $index, $batch_size, &$context) {
  try {
    if (!isset($context['sandbox']['progress'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['batch_size'] = $batch_size;
    }

    // Persistent data for results.
    if (!isset($context['results']['purged'])) {
      $context['results']['purged'] = 0;
      $context['results']['not purged'] = 0;
      $context['results']['last_checked'] = 0;
    }

    $batch = SearchApiPurgeBatch::getInstance();
    $batch->setBatch($batch_size);
    $items = $batch->getItems($index, $context['results']['last_checked']);
    $context['results']['purged'] += $batch->purge($index, $items);
    $context['results']['not purged'] += count($items) - $context['results']['purged'];
    $context['results']['last_checked'] = end($items)['id'];

    if (!isset($context['results']['total'])) {
      $context['results']['total'] = $batch->getTotal();
    }

    $context['sandbox']['progress'] += count($items);
    $context['message'] = t('Checked %items items of %total. Purged %purged items.', array(
      '%items' => $context['sandbox']['progress'],
      '%total' => $context['results']['total'],
      '%purged' => $context['results']['purged'],
    ));
    $context['finished'] = $context['sandbox']['progress'] / $context['results']['total'];
  }
  catch (Exception $exception) {
    watchdog('search_api', 'Error purging items [%message]', array('%message' => $exception->getMessage()));
    $context['finished'] = 1;
  }
}

/**
 * Batch 'finished' callback.
 */
function search_api_purge_batch_finished($success, $results, $operations) {
  drupal_set_message(t('@count items successfully purged.', array('@count' => count($results['purged']))));
}