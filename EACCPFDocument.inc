<?php

/**
 * @file
 *
 * Abstracts EAC-CPF xml document into a PHP class where the maintence history is maintained automatically by the class.
 * 
 */
module_load_include('inc', 'islandora_metadata', 'XMLDocument');

/**
 * Enumerated Types.
 */
enum('EACCPFAgent', array(
  'human' => 'human',
  'machine' => 'machine')
);
enum('EACCPFMaintenceEvent', array(
  'created' => 'created',
  'revised' => 'revised',
  'deleted' => 'deleted',
  'cancelled' => 'cancelled',
  'derived' => 'cancelled',
  'updated' => 'cancelled')
);

/**
 * EACCPF Class.
 */
class EACCPFDocument extends _XMLDocument {

  /**
   * Creates an EACCPFDocument from a valid template.
   */
  public static function fromTemplate() {
    $filename = drupal_get_path('module', 'islandora_metadata') . '/templates/eac-cpf.xml';
    $doc = new EACCPFDocument();
    $doc->load($filename);
    return $doc;
  }

  /**
   * Creates an EACCPF instance.
   * 
   * @throws 
   * @param DOMDocument $document
   *   An optional valid EAC-CPF document
   */
  public function __construct() {
    $validator = new XMLDocumentValidator(XMLSchemaFormat::XSD(), drupal_get_path('module', 'islandora_metadata') . '/xsd/cpf.xsd');
    $namespaces = new XMLDocumentNamepaces('urn:isbn:1-931666-33-4', array(
          'xsi' => 'http://www.w3.org/2001/XMLSchema-instance',
          'xlink' => 'http://www.w3.org/1999/xlink'));
    parent::__construct($namespaces, $validator);

    $root_attributes = array('xsi:schemaLocation' => 'urn:isbn:1-931666-33-4 http://eac.staatsbibliothek-berlin.de/schema/cpf.xsd');
  }

  /**
   * Removed function.
   */
  public function saveHTML() {
    throw new Exception(__FUNCTION__ . ' is not supported.');
  }

  /**
   * Adds a Maintence Event.
   * 
   * @param EACCPFMaintenceEventType $event_type
   *   The event type.
   * @param EACCPFAgentType $agent_type
   *   The agent type.
   * @param string $agent
   *   The current user performing the maintence event.
   * @param string $description 
   *   A description for the event.
   * 
   * @return boolean
   *   TRUE on success FALSE otherwise.
   */
  public function addMaintenanceEvent(EACCPFMaintenceEventType $event_type, EACCPFAgentType $agent_type, $agent, $description, $date = NULL) {
    $date = isset($data) ? $date : date("Y-m-d");
    $standard_date_time = new DateTime($date);
    $standard_date_time = $standard_date_time->format("Y-m-d");
    $results = $this->xpath->query('/default:eac-cpf]/default:control/default:maintainenceHistory');
    if ($results->length == 1) {
      $default_uri = $this->namespaces->getDefaultURI();
      $history = $results->item(0);
      $event = $this->createElementNS('maintenanceEvent', $default_uri);
      $history->appendChild($event);
      $event->appendChild($this->createElementNS('eventType', $default_uri, (string) $event_type));
      $date = $this->createElementNS('eventDateTime', $default_uri, $date);
      $date->setAttribute('standardDateTime', $standard_date_time);
      $event->appendChild($date);
      $event->appendChild($this->createElementNS('agentType', $default_uri, (string) $agent_type));
      $event->appendChild($this->createElementNS('agent', $default_uri, $agent));
      $event->appendChild($this->createElementNS('eventDescription', $default_uri, $description));
      return TRUE;
    }
    return FALSE;
  }

}