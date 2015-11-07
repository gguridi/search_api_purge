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
   */
  protected $form;

  /**
   * Admin form_state array.
   *
   * @var array
   */
  protected $form_state;

  /**
   * Constructor of the class.
   *
   * @param array $form
   *   Form array.
   * @param array $form_state
   *   Form state array.
   */
  public function __construct($form, &$form_state) {
    $this->form = $form;
    $this->form_state = &$form_state;
  }

  /**
   * Processes the form.
   *
   * @return array
   *   Form array processed.
   */
  public function process() {
    form_load_include($this->form_state, 'batch.inc', 'search_api_purge');
    array_unshift($this->form['#submit'], 'search_api_purge_batch');
    $this->addFieldset();
    $this->addAction();
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
  public function addAction() {
    $sentence = t('Purge all items in batches of !batch_size items !action');
    $this->form[self::FIELDSET_PURGE]['action'] = array(
      '#markup' => t($sentence, array(
        '!batch_size' => $this->getBatchElement(),
        '!action' => $this->getButtonElement(),
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
    $element = array(
      '#type' => 'textfield',
      '#name' => self::PURGE_BATCH,
      '#size' => 3,
      '#value' => SearchApiPurgeBatch::getInstance()->getBatch(),
    );
    return drupal_render($element);
  }

  /**
   * Gets the submit button form element rendered.
   *
   * @return string
   *   Rendered element.
   */
  public function getButtonElement() {
    $element = array(
      '#type' => 'submit',
      '#name' => self::PURGE_BUTTON,
      '#value' => t('Purge now'),
    );
    return drupal_render($element);
  }
}