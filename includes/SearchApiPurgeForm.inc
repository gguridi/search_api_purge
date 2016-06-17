<?php
/**
 * @file
 * Form handlers.
 */

/**
 * Class SearchApiPurgeForm implements the admin form alter handler.
 */
class SearchApiPurgeForm {

  // Field names to use in the admin form.
  const FIELDSET_PURGE = 'fieldset-purge';
  const PURGE_BATCH = 'search-api-purge-batch';
  const PURGE_BUTTON = 'search-api-purge-button';

  /**
   * Admin form array.
   *
   * @var array
   *   Form array.
   */
  protected $form;

  /**
   * Admin form_state array.
   *
   * @var array
   *   Form state array.
   */
  protected $formState;

  /**
   * Constructor of the class.
   *
   * @param array $form
   *   Form array.
   * @param array $form_state
   *   Form state array.
   */
  public function __construct(array $form, array &$form_state) {
    $this->form = $form;
    $this->formState = &$form_state;
  }

  /**
   * Processes the form.
   *
   * @return array
   *   Form array processed.
   */
  public function process() {
    form_load_include($this->formState, 'batch.inc', 'search_api_purge');
    array_unshift($this->form['#submit'], 'search_api_purge_batch');
    $this->addFieldset();
    $this->addInfo();
    $this->addButton();
    return $this->form;
  }

  /**
   * Adds the fieldset to contain the purge action.
   */
  public function addFieldset() {
    $this->form[self::FIELDSET_PURGE] = array(
      '#type' => 'fieldset',
      '#tree' => FALSE,
      '#title' => t('Purge now'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array('class' => array('container-inline')),
    );
  }

  /**
   * Adds the action elements to the fieldset.
   */
  public function addInfo() {
    $this->form[self::FIELDSET_PURGE]['action'] = array(
      '#markup' => t('Purge all items in batches of !batch_size items.', array(
        '!batch_size' => $this->getBatchElement(),
      )),
    );
  }

  /**
   * Gets the batch form element rendered.
   *
   * @return string
   *   Rendered element.
   */
  protected function getBatchElement() {
    // Adding a type value so we can recover the value easily.
    $this->form[self::FIELDSET_PURGE][self::PURGE_BATCH] = array(
      '#type' => 'value',
    );
    $element = array(
      '#type' => 'textfield',
      '#name' => self::PURGE_BATCH,
      '#size' => 3,
      '#value' => SearchApiPurgeBatch::getInstance()->getBatch(),
    );
    return drupal_render($element);
  }

  /**
   * Adds the submit button.
   */
  public function addButton() {
    $this->form[self::FIELDSET_PURGE][self::PURGE_BUTTON] = array(
      '#type' => 'submit',
      '#value' => $this->getButtonId(),
    );
  }

  /**
   * Gets the id expected for the button submit.
   *
   * @return string
   *   Button id.
   */
  public function getButtonId() {
    return t('Purge now');
  }

  /**
   * Gets the batch value submitted in the form.
   *
   * @return int
   *   Batch value.
   */
  public function getBatchValue() {
    if (isset($this->formState['values'][self::PURGE_BATCH])) {
      return check_plain($this->formState['values'][self::PURGE_BATCH]);
    }
    return SearchApiPurgeBatch::getInstance()->getBatch();
  }

  /**
   * Checks if user has clicked the submit button of the purge.
   *
   * @return bool
   *   True if the purge button has been triggered, false otherwise.
   */
  public function isPurgeSubmit() {
    return ($this->formState['values']['op'] == $this->getButtonId());
  }

}
