<?php
/**
 * @file
 * Drush commands implementation.
 */

/**
 * Implements hook_drush_command().
 */
function search_api_purge_drush_command() {
  $commands = array();
  $commands['search-api-purge'] = array(
    'description' => dt('Purges solr content from indexes.'),
    'arguments' => array(
      'element' => dt('The index to purge. If not provided, all indexes will be purged.'),
    ),
    'options' => array(
      'batch' => 'Specifies the batch to use in the process',
    ),
    'examples' => array(
      dt('standard example') => 'drush search-api-purge my_index',
    ),
  );
  return $commands;
}

/**
 * Implements search_api_purge drush command.
 *
 * @param mixed $index_id
 *   Index id or machine name of the index to load.
 */
function drush_search_api_purge($index_id) {
  try {
    module_load_include('batch.inc', 'search_api_purge');
    $indexes = search_api_drush_get_index($index_id);
    if (empty($indexes)) {
      return;
    }

    $batch = drush_get_option('batch', SearchApiPurgeBatch::getInstance()->getBatch());
    foreach ($indexes as $index) {
      search_api_purge_batch_create($index, $batch);
      drush_backend_batch_process();
    }
  }
  catch (SearchApiException $exception) {
    drush_log($exception->getMessage(), 'error');
  }
}
