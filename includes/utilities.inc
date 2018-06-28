<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Find a DOI in the specified datastream and return it.
 *
 * @param \AbstractObject $object
 *   The Fedora object.
 *
 * @return string
 *   The DOI or FALSE
 */
function islandora_doi_block_get_doi(AbstractObject $object) {
  $dsid = variable_get('islandora_doi_block_source_dsid', 'MODS');
  $namespace_prefix = variable_get('islandora_doi_block_namespace_prefix', 'mods');
  $namespace_uri = variable_get('islandora_doi_block_namespace_uri', 'http://www.loc.gov/mods/v3');
  // Get DOI.
  if (!isset($object[$dsid])) {
    return FALSE;
  }
  $doc = new DOMDocument();
  $doc->loadXML($object[$dsid]->content);
  $xpath = new DOMXPath($doc);
  $xpath->registerNamespace($namespace_prefix, $namespace_uri);
  $xpath_results = $xpath->query(
    variable_get('islandora_doi_block_doi_xpath', '/mods:mods/mods:identifier[@type="doi"]')
  );
  if ($xpath_results->length == 0) {
    return FALSE;
  }
  $doi = $xpath_results->item(0)->nodeValue;
  if (!$doi) {
    return FALSE;
  }
  return $doi;
}

/**
 * Determine if the object has a Content model we want to show the block on.
 *
 * @param \AbstractObject $object
 *   The object we are viewing.
 *
 * @return bool
 *   Whether $object has a content model to show the block for.
 */
function islandora_doi_block_show_for_cmodel(AbstractObject $object) {
  $show_cmodels = variable_get('islandora_doi_block_selected_cmodels', array('ir:citationCModel', 'ir:thesisCModel'));
  return (count(array_intersect($object->models, $show_cmodels)) > 0);
}
