<?php
/**
 * RETURN AN ASSOCIATIVE ARRAY WITH THE US STATES
 */
function getUsStatesOptions(){
  return array(
       0 => ' - Select State - ',
    'AL' => 'Alabama',
    'AK' => 'Alaska',
    'AZ' => 'Arizona',
    'AR' => 'Arkansas',
    'CA' => 'California',
    'CO' => 'Colorado',
    'CT' => 'Connecticut',
    'DE' => 'Delaware',
    'DC' => 'District of Columbia',
    'FL' => 'Florida',
    'GA' => 'Georgia',
    'HI' => 'Hawaii',
    'ID' => 'Idaho',
    'IL' => 'Illinois',
    'IN' => 'Indiana',
    'IA' => 'Iowa',
    'KS' => 'Kansas',
    'KY' => 'Kentucky',
    'LA' => 'Louisiana',
    'ME' => 'Maine',
    'MD' => 'Maryland',
    'MA' => 'Massachusetts',
    'MI' => 'Michigan',
    'MN' => 'Minnesota',
    'MS' => 'Mississippi',
    'MO' => 'Missouri',
    'MT' => 'Montana',
    'NE' => 'Nebraska',
    'NV' => 'Nevada',
    'NH' => 'New Hampshire',
    'NJ' => 'New Jersey',
    'NM' => 'New Mexico',
    'NY' => 'New York',
    'NC' => 'North Carolina',
    'ND' => 'North Dakota',
    'OH' => 'Ohio',
    'OK' => 'Oklahoma',
    'OR' => 'Oregon',
    'PA' => 'Pennsylvania',
    'RI' => 'Rhode Island',
    'SC' => 'South Carolina',
    'SD' => 'South Dakota',
    'TN' => 'Tennessee',
    'TX' => 'Texas',
    'UT' => 'Utah',
    'VT' => 'Vermont',
    'VA' => 'Virginia',
    'WA' => 'Washington',
    'WV' => 'West Virginia',
    'WI' => 'Wisconsin',
    'WY' => 'Wyoming');
}


/**
 * RETURN A BROKER COMPANY ARRAY
 */ 
function getBrokerCompanyList(){
  $result = db_select('node', 'n')
    ->fields('n', array('title', 'nid'))
    ->condition('status', '1', '=')
    ->condition('type', 'broker_company', '=')
    ->orderBy('title', 'ASC')
    ->execute();
  $bcompany_op = array(0 => 'New Company');
  while($record = $result->fetchAssoc()) {
    $bcompany_op[$record['nid']] = $record['title'];
  }
  return $bcompany_op;
}


/**
 * RETURN A BROKER AGENT ARRAY
 */ 
function getBrokerAgentList($cid = NULL){
  $query = db_select('node', 'n')
    ->fields('n', array('title', 'nid'))
    ->condition('n.status', '1', '=')
    ->condition('n.type', 'broker_agent', '=')
    ->orderBy('n.title', 'ASC');

  if(!is_null($cid)) {
    $query->join('field_data_field_broker_company', 'bc', 'bc.entity_id = n.nid');
    $query->condition('bc.field_broker_company_target_id', $cid, '=');  
  }

  $result = $query->execute();
  $bagent_op = array(0 => 'New Agent');
  while($record = $result->fetchAssoc()) {
    $bagent_op[$record['nid']] = $record['title'];
  }
  return $bagent_op;
}

/**
 * Callback function for add event
 */
function add_event($form, &$form_state) {
  if ($form_state['values']['addevent']){
    $form_state['storage']['events']++;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback function for remove event
 */
function remove_event($form, &$form_state) {
  if ($form_state['values']['removeevent'] && $form_state['storage']['events'] > 0){
    $form_state['storage']['events']--;
  }
  $form_state['rebuild'] = TRUE;
}


/**
 * Ajax callback for add event
 */
function add_event_js($form, &$form_state) {
  return $form['events']['extraevents'];
}

/**
 * Clean String
 */
function clean($string) {
   return preg_replace('/[^A-Za-z0-9\ ]/', '', $string); // Removes special chars.
}

/**
 * Clean String To Numbers
 */
function cleanStrToNumbers($str) {
    return preg_replace("/[^0-9.]/", '', $str);
}

/**
 * Clean String To Letters
 */
function cleanStrToLetters($str) {
    $z = strtolower($str);
    $z = preg_replace('/[^a-z -]+/', '', $z);
    $z = str_replace(' ', '', $z);
    return trim($z, '-');
}