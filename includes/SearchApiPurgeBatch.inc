<?php
/**
 * @file
 * Batch handlers.
 */

/**
 * Class SearchApiPurgeBatch implements a batch handler for the module.
 */
class SearchApiPurgeBatch {

  /**
   * Contains the batch size to use.
   *
   * @var int
   */
  protected $batch;

  /**
   * Contains the total items to be processed.
   *
   * @var int
   */
  protected $total;

  /**
   * Constructor of the class.
   */
  protected function __construct() {
  }

  /**
   * Gets the instance of the batch processor.
   *
   * @return $this
   *   Instance of the class.
   */
  public static function getInstance() {
    static $instance;
    if (!$instance) {
      $instance = new static();
    }
    return $instance;
  }

  /**
   * Returns the batch to use.
   *
   * @return int
   *   Batch size.
   */
  public function getBatch() {
    return $this->batch ?: variable_get('search_api_purge_batch', 100);
  }

  /**
   * Sets the batch to use.
   *
   * @param int $batch
   *   Batch size.
   */
  public function setBatch($batch) {
    $this->batch = $batch;
  }

  /**
   * Gets the total items to be processed.
   *
   * @return int
   *   Total items number.
   */
  public function getTotal() {
    return $this->total;
  }

  /**
   * Sets the total items to be processed.
   *
   * @param int $total
   *   Total items number.
   */
  public function setTotal($total) {
    $this->total = $total;
  }

  /**
   * Loads the items to be processed in this batch.
   *
   * @param SearchApiIndex $index
   *   Search API index to use.
   * @param int $last_item
   *   Last identifier checked to continue from.
   *
   * @return array
   *   List of items to check.
   *
   * @throws SearchApiException
   *   In case something went wrong.
   */
  public function getItems(SearchApiIndex $index, $last_item) {
    $query = search_api_query($index->id, array(
      'parse mode' => 'direct',
      'highlight_data' => '',
      'retrieve_data' => 'item_id',
    ));
    $key_info = $index->datasource()->getIdFieldInfo();
    $query->condition($key_info['key'], $last_item, '>');
    $query->range(0, $this->getBatch());
    $query->sort($key_info['key'], 'ASC');
    $results = $query->execute();
    $this->setTotal($results['result count']);
    return array_keys($results['results']);
  }

  /**
   * Purge the items if necessary.
   *
   * @param SearchApiIndex $index
   *   Search API index to use.
   * @param array $items
   *   Items loaded to check.
   *
   * @return int
   *   Number of items purged, 0 if nothing done.
   */
  public function purge(SearchApiIndex $index, array $items) {
    $unknown = array();
    $loaded = $index->loadItems($items);
    // If some items couldn't be loaded, remove them from tracking.
    if (count($loaded) != count($items)) {
      $items = array_flip($items);
      $unknown = array_keys(array_diff_key($items, $loaded));
      $index->server()->deleteItems($unknown, $index);
    }
    return count($unknown);
  }

}
