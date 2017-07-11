<?php

/**
 * @file
 * Migration File for AAA Harvest.
 */

/**
 * Migration class to harvest csv.
 */
class AaaCsvMigration extends HarvestMigration {

  public $itemUrl;

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->itemUrl = drupal_realpath($this->dkanHarvestSource->getCacheDir()) .
      '/:id';

    $this->source = new HarvestMigrateSourceList(
      new HarvestList($this->dkanHarvestSource->getCacheDir()),
      new MigrateItemJSON($this->itemUrl),
      array(),
      $this->sourceListOptions
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setFieldMappings() {
    parent::setFieldMappings();

    $this->addFieldMapping('title', 'Title');
    $this->addFieldMapping('field_guid', 'UniqueID');
    $this->addFieldMapping('body', 'Description');
    $this->addFieldMapping('field_tags', 'Tags');
    $this->addFieldMapping('field_topic', 'Category');
    $this->addFieldMapping('field_frequency', 'Frequency');
    $this->addFieldMapping('field_landing_page', 'HomepageURL');
  }

  /**
   * Implements prepareRow.
   */
  public function prepareRow($row)
  {
    parent::prepareRow($row);

    // Convert comma separated tags into array.
    $tags = $row->Tags;
    $row->Tags = array_map('trim', explode(',', $tags));

    // Convert comma separated frequency into array of frequency keys.
    $frequency_field = field_info_field('field_frequency');
    $allowed_frequency = array_flip(list_allowed_values($frequency_field));

    $frequencies = $row->Frequency;
    $frequencies = array_map('trim', explode(',', $frequencies));
    $frequencies = array_flip($frequencies);
    $frequencies = array_intersect_key($allowed_frequency, $frequencies);

    $row->Frequency = array_values($frequencies);
  }
}