<?php

/**
 * @file
 *
 * Tempory dump for classes and functions.
 */
module_load_include('inc', 'islandora_metadata', 'Enum');


enum('XMLSchemaFormat', array('DTD', 'XSD', 'relaxNG'));

/**
 * Validates DOMDocuments.
 */
class XMLDocumentValidator {

  /**
   * The type of validation.
   * 
   * @var XMLValidatorType
   */
  private $type;

  /**
   * The filename of the validition document.
   * 
   * @var string
   */
  private $filename;

  /**
   *
   * @param XMLValidatorType $type
   * @param string $filename 
   */
  public function __construct(XMLSchemaFormat $type, $filename = NULL) {
    $this->type = $type;
    if ($filename === NULL && $type !== XMLSchemaFormat::DTD()) {
      throw new InvalidArgumentException('No file provided.');
    }
  }

  /**
   *
   * @param DOMDocument $document 
   * @return boolean
   */
  public function isValid(DOMDocument $document) {
    switch ($this->type) {
      case XMLSchemaFormat::DTD():
        $document->validate();
        break;
      case XMLSchemaFormat::XSD():
        $document->schemaValidate($this->filename);
        break;
      case XMLSchemaFormat::RNG():
        break;
    }
  }

}

/**
 * Handles generic XML concepts and data.
 */
class XMLDocument extends DOMDocument {

  /**
   *
   * @var XMLValidator 
   */
  protected $validator;

  /**
   *
   * @param DOMDocument $document
   * @param XMLValidator $validator 
   */
  public function __construct(XMLDocumentValidator $validator = NULL) {
    $this->validator = $validator;
  }

  /**
   * Checks if this XML document is valid.
   * 
   * If no method of validation is provided its always assumed to be valid.
   * 
   * @return boolean
   *   TRUE if valid, FALSE otherwise.
   */
  public function isValid() {
    return isset($this->validator) ? $this->validator->isValid($this) : TRUE;
  }

}