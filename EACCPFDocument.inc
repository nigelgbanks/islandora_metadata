<?php

/**
 * @file
 *
 * Abstracts EAC-CPF xml document into a PHP class where the maintence history is maintained automatically by the class.
 * 
 */

/**
 * EACCPF Class.
 */
class EACCPFDocument extends XMLDocument {

  /**
   * Creates an EACCPF instance.
   * 
   * @throws 
   * @param DOMDocument $document
   *   An optional valid EAC-CPF document
   */
  public function __construct() {
    parent::__construct(new XMLDocumentValidator(XMLValidatorType::XSD, drupal_get_path('module', 'islandora_metadata') . '/xsd/cpf.xsd'));
    
  }
  
}