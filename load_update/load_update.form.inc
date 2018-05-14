<?php
/**
 * Implements hook_form().
 */
function load_update_form($form, &$form_state) {
  // NODE ID (Otherwise we will face some problems with the ajax request)
  if(!isset($form_state['storage']['load']['nid'])){
    $form_state['storage']['load']['nid'] = arg(2);
  }
  
  // LOAD NODE
  $raw_node = node_load($form_state['storage']['load']['nid']);
  $node = entity_metadata_wrapper('node', $raw_node);

  // RATE CONFIRMATION FID
  if(!isset($form_state['storage']['load']['fid'])){
    $form_state['storage']['load']['fid'] = NULL;
    if(!empty($node->field_rate_confirmation->value())){
      $form_state['storage']['load']['fid'] = $node->field_rate_confirmation->value()['fid'];
    }
  } 
  
  // FORM
  $form = array(
    'loadtabs' => array(
      '#type' => 'horizontal_tabs'
    )
  );
  
  /*****************************
   * TAB 1 - INVOLVED ENTITIES *
   *****************************/  
  $author = $node->author->field_full_name->value();
  if(empty($author)){ $author = $node->author->name->value(); }

  $dispatcher = 'Unknown';
  if(!empty($node->field_dispatcher->value())) { 
    $dispatcher = $node->field_dispatcher->field_full_name->value(); 
    if(empty($dispatcher)) { 
      $dispatcher = $node->field_dispatcher->name->value(); 
    }
  }  

  $customer = $node->field_customer->title->value();
  $carrier = $node->field_single_carrier->title->value();
  $truck = $node->field_truck->title->value();
  $driver = '';
  $form_state['storage']['drphone'] = $node->field_driver[0]->field_phone->value();


  foreach ($node->field_driver->value() as $key => $value) {
    if(!empty($driver)){ $driver .= ' / '; }
    $driver .= $value->title;
  }
  
  $form['entities'] = array(
    '#type' => 'fieldset',
    '#title' => 'Involved Entities',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    'author' => array(
      '#type' => 'textfield',
      '#title' => 'Author:',
      '#default_value' => $author,
      '#disabled' => TRUE,
      '#prefix' => '<div class="row"><div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'dispatcher' => array(
      '#type' => 'textfield',
      '#title' => 'Dispatcher:',
      '#default_value' => $dispatcher,
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'customer' => array(
      '#type' => 'textfield',
      '#title' => 'Customer:',
      '#default_value' => $customer,
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'carrier' => array(
      '#type' => 'textfield',
      '#title' => 'Carrier:',
      '#default_value' => $carrier,
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div></div>',
    ),
    'created' => array(
      '#type' => 'textfield',
      '#title' => 'Date:',
      '#default_value' => date('M / d / Y', $node->created->value()),
      '#disabled' => TRUE,
      '#prefix' => '<div class="row" style="margin-top:25px;"><div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'truck' => array(
      '#type' => 'textfield',
      '#title' => 'Truck:',
      '#default_value' => $truck,
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'drivers' => array(
      '#type' => 'textfield',
      '#title' => 'Driver(s):',
      '#default_value' => $driver,
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-4">',
      '#suffix' => '</div></div>',
    ),
  );

  /*******************************
   * TAB 2 - GENERAL INFORMATION *
   *******************************/
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => 'General Information',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '4gid' => array(
      '#type' => 'textfield',
      '#title' => '4G ID:',
      '#default_value' => $node->title->value(),
      '#disabled' => TRUE,
      '#prefix' => '<div class="row"><div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'loadnumber' => array(
      '#type' => 'textfield',
      '#title' => 'Load Number:',
      '#default_value' => $node->field_broker_load_number->value(),
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>'
    ),
    'audit' => array(
      '#type' => 'select',
      '#title' => 'Audited:',
      '#options' => list_allowed_values(field_info_field('field_audited')),
      '#default_value' =>  $node->field_audited->value(),
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'paid' => array(
      '#type' => 'select',
      '#title' => 'Paid:',
      '#options' => list_allowed_values(field_info_field('field_paid')),
      '#default_value' =>  $node->field_paid->value(),
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'status' => array(
      '#type' => 'select',
      '#title' => 'Status:',
      '#options' => list_allowed_values(field_info_field('field_csctatus')),
      '#default_value' =>  $node->field_csctatus->value(),
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'ratexmi' => array(
      '#type' => 'textfield',
      '#title' => 'Rate x Miles:',
      '#default_value' => $node->field_rate_x_miles->value(),
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'totalmi' => array(
      '#type' => 'textfield',
      '#title' => 'Total Miles:',
      '#default_value' => $node->field_total_miles->value(),
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'emptymi' => array(
      '#type' => 'textfield',
      '#title' => 'Empty Miles:',
      '#default_value' => $node->field_empty_miles->value(),
      '#disabled' => TRUE,
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'grospay' => array(
      '#type' => 'textfield',
      '#title' => 'Gross:',
      '#default_value' => $node->field_gross_payment->value(),
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'soldpay' => array(
      '#type' => 'textfield',
      '#title' => 'Sold:',
      '#default_value' => $node->field_sold_payment->value(),
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div></div>',
    ),
    'rate_container' => array(
      '#type' => 'container',
      '#prefix' => '<div class="row"><div class="col-sm-12 col-md-4" style="margin-top:30px;">',
      '#suffix' => '</div></div>',
      'rate' => array(
        '#title' => 'Rate Confirmation:',
        '#type' => 'managed_file',
        '#upload_validators' => array('file_validate_extensions' => array('pdf jpg jpeg')),
        '#upload_location' => 'public://rateconfirmation/',
        '#default_value' => $form_state['storage']['load']['fid'],
        '#description' => t('Allowed extensions: pdf jpg jpeg.'),
        '#required' => TRUE,
        //'#disabled' => TRUE,
      ),    
    ),    
  );
  
  /***********************************************************
  * TAB 3 - ITINERARY  (ADD REAL TIME OF PICKUP OR DELIVERY) *
  ************************************************************/
  $form['itinerary'] = array(
    '#type' => 'fieldset',
    '#title' => 'Itinerary',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    'initial' => array(
      '#type' => 'fieldset',
      '#title' => 'Initial Point',
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      'address' => array(
        '#type' => 'textfield',
        '#title' => 'Address:',
        '#default_value' => $node->field_initial_point->thoroughfare->value(),
        '#prefix' => '<div class="row"><div class="col-sm-12 col-md-4">',
        '#suffix' => '</div>',
      ),
      'city' => array(
        '#type' => 'textfield',
        '#title' => 'City:',
        '#default_value' => $node->field_initial_point->locality->value(),
        '#prefix' => '<div class="col-sm-12 col-md-3">',
        '#suffix' => '</div>',
      ),
      'state' => array(
        '#type' => 'select',
        '#title' => 'State:',
        '#options' => getUsStatesOptions(),
        '#default_value' => $node->field_initial_point->administrative_area->value(),
        '#prefix' => '<div class="col-sm-12 col-md-2">',
        '#suffix' => '</div>',
      ), 
      'zip' => array(
        '#type' => 'masked_input',
        '#mask' => '99999',
        '#title' => 'Zip Code:',
        '#default_value' => $node->field_initial_point->postal_code->value(),
        '#prefix' => '<div class="col-sm-12 col-md-2">',
        '#suffix' => '</div></div>',
      ),
    ),    
  );

  foreach ($node->field_loadpoint->value() as $key => $value) {
    $interm_date = null;
    if(!empty($node->field_loadpoint[$key]->field_date_time->value())){
      $interm_date = date('Y-m-d H:i:s', $node->field_loadpoint[$key]->field_date_time->value());
    }

    $form['itinerary']['interm'.$key] = array(
      '#type' => 'fieldset',
      '#title' => $key + 1 . ' ' . ucwords($node->field_loadpoint[$key]->field_type->value()) . ' Point',
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      'address' . $key => array(
        '#type' => 'textfield',
        '#title' => 'Address:',
        '#default_value' => $node->field_loadpoint[$key]->field_address->thoroughfare->value(),
        '#prefix' => '<div class="row"><div class="col-sm-12 col-md-4">',
        '#suffix' => '</div>',
      ),
      'city' . $key => array(
        '#type' => 'textfield',
        '#title' => 'City:',
        '#default_value' => $node->field_loadpoint[$key]->field_address->locality->value(),
        '#prefix' => '<div class="col-sm-12 col-md-3">',
        '#suffix' => '</div>',
      ),
      'state' . $key => array(
        '#type' => 'select',
        '#title' => 'State:',
        '#options' => getUsStatesOptions(),
        '#default_value' => $node->field_loadpoint[$key]->field_address->administrative_area->value(),
        '#prefix' => '<div class="col-sm-12 col-md-2">',
        '#suffix' => '</div>',
      ),
      'zip' . $key => array(
        '#type' => 'masked_input',
        '#mask' => '99999',
        '#title' => 'Zip Code:',
        '#default_value' => $node->field_loadpoint[$key]->field_address->postal_code->value(),
        '#prefix' => '<div class="col-sm-12 col-md-2">',
        '#suffix' => '</div></div>',
      ),
      'type' . $key => array(
        '#type' => 'select',
        '#title' => 'Action:',
        '#options' => list_allowed_values(field_info_field('field_type')),
        '#default_value' => $node->field_loadpoint[$key]->field_type->value(),
        '#prefix' => '<div class="row" style="margin-top:30px;"><div class="col-sm-12 col-md-2">',
        '#suffix' => '</div>',
      ),
      'ware' . $key => array(
        '#type' => 'textfield',
        '#title' => 'Warehouse:',
        '#default_value' => $node->field_loadpoint[$key]->field_warehouse->value(),
        '#prefix' => '<div class="col-sm-12 col-md-2">',
        '#suffix' => '</div>',
      ),
      'date' . $key => array(
        '#type' => 'date_popup',
        '#date_format' => 'M / d / Y',
        '#date_label_position' => 'above',
        '#title' => 'Date:',
        '#date_year_range' => '0:+1',
        '#default_value' => $interm_date,  
        '#prefix' => '<div class="col-sm-12 col-md-1">',
        '#suffix' => '</div>'    
      ),
      'time' . $key => array(
        '#type' => 'date_popup',
        '#date_format' => 'H:i:s',
        '#date_label_position' => 'above',
        '#title' => 'Time:',
        '#date_increment' => 30,
        '#default_value' => $interm_date,
        '#prefix' => '<div class="col-sm-12 col-md-1">',
        '#suffix' => '</div></div>',
      ), 
    );
  }
  
 /******************
  * TAB 4 - EVENTS *
  ******************/
  $markup = '<h2> Event Log: </h2>'; 
  foreach ($node->field_events->value() as $key => $value) {
    $markup .= '<div><p>';
    $markup .= date('M/d/Y H:i', $node->field_events[$key]->field_date->value()) . ' - ';
    $markup .= '<strong>' . $node->field_events[$key]->field_description->value() . '</strong>';
    $markup .= '</p></div>';
  }
  $form['events'] = array(
    '#type' => 'fieldset',
    '#title' => 'Events',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE, 
    'eventlist' => array(
      '#type' => 'item', 
      '#markup' => $markup,
    ),
    'addevent' => array(
      '#type' => 'submit',
      '#value' => 'Add Event',
      '#submit' =>  array('add_event'),
      '#prefix' => '<div class="row"><div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',  
      '#ajax' => array(
        'callback' => 'add_event_js',
        'wrapper' => 'event-wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    ),
    'removeevent' => array(
      '#type' => 'submit',
      '#value' => 'Remove Event',
      '#submit' =>  array('remove_event'),
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div></div>',  
      '#ajax' => array(
        'callback' => 'add_event_js',
        'wrapper' => 'event-wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    ),
   'extraevents' => array(
     '#type' => 'container',
     '#prefix' => '<div id="event-wrapper">',
     '#suffix' => '</div>'
    ),
  );  
  if(!isset($form_state['storage']['events'])){
    $form_state['storage']['events'] = 0;
  }
  if($form_state['storage']['events'] > 0) { 
    for($i = 0; $i < $form_state['storage']['events']; $i++){
      $form['events']['extraevents']['edate'.$i] = array(
        '#type' => 'date_popup',
        '#date_format' => 'M / d / Y',
        '#date_label_position' => 'above',
        '#title' => 'Date:',
        '#date_year_range' => '0:+1',
        '#prefix' => '<div class="row" style="margin-top:30px;"><div class="col-sm-12 col-md-1">',
        '#suffix' => '</div>',   
      ); 
      $form['events']['extraevents']['etime'.$i] = array(
        '#type' => 'date_popup',
        '#date_format' => 'H:i:s',
        '#date_label_position' => 'above',
        '#title' => 'Time:',
        '#date_increment' => 30,
        '#prefix' => '<div class="col-sm-12 col-md-1">',
        '#suffix' => '</div>',
      ); 
      $form['events']['extraevents']['edesc'.$i] = array(
        '#type' => 'textfield',
        '#title' => 'Description:',
        '#prefix' => '<div class="col-sm-12 col-md-2">',
        '#suffix' => '</div></div>',
      );
    }    
  }
 
 /******************
  * TAB 5 - NOTES *
  ******************/ 
  $form['notes'] = array(
    '#type' => 'fieldset',
    '#title' => 'Notes',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    'notes_desc' => array(
      '#title' => 'Description:',
      '#type' => 'textarea',
      '#prefix' => '<div class="row"><div class="col-sm-12">',
      '#suffix' => '</div></div>',  
      '#default_value' => !empty($node->body->value()) ? $node->body->value->value() : NULL,
    ),  
  );

 /******************
  * TAB 6 - BROKER *
  ******************/
  $bcompopt = getBrokerCompanyList();
  $form['broker'] = array (
    '#type' => 'fieldset',
    '#title' => 'Broker',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );

  $form['broker']['bcompopt'] = array(
    '#type' => 'select',
    '#title' => 'Broker Company',
    '#options' => $bcompopt,
    '#default_value' => !empty($node->field_broker_company->value()) ? $node->field_broker_company->value()->nid : 0,
    '#prefix' => '<div class="row"><div class="col-sm-12 col-md-3">',
    '#suffix' => '</div></div>',
    '#ajax' => array(
      'callback' => 'refresh_bragent',
      'wrapper' => 'agents-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['broker']['bcform'] = array (
    '#type' => 'container',
    '#states' => array('visible' => array('select[name="bcompopt"]' => array(array('value' => '0')))),
    'bcname' => array(
      '#type' => 'textfield',
      '#title' => 'Company Name',
      '#prefix' => '<div class="row" style="margin-top:10px;"><div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>' 
    ),
    'bcemail' => array(
      '#type' => 'textfield',
      '#title' => 'Email Address',
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>' 
    ),
    'bcphone' => array(
      '#type' => 'masked_input',
      '#mask' => '(999) 999 9999',
      '#title' => 'Phone Number',
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>' 
    ),
    'bcext1' => array(
      '#type' => 'textfield',
      '#title' => 'Ext 1',
      '#maxlength' => 5, 
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>' 
    ),
    'bcext2' => array(
      '#type' => 'textfield',
      '#title' => 'Ext 2',
      '#maxlength' => 5, 
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>' 
    ),
    'bcmc' => array(
      '#type' => 'masked_input',
      '#mask' => '999999',
      '#title' => 'MC',
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>' 
    ),    
    'bcdot' => array(
      '#type' => 'masked_input',
      '#mask' => '9999999',
      '#title' => 'DOT',
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div></div>' 
    ),
    'bcaddress' => array(
      '#type' => 'textfield',
      '#title' => 'Address',
      '#prefix' => '<div class="row" style="margin-top:10px;"><div class="col-sm-12 col-md-4">',
      '#suffix' => '</div>' 
    ),
    'bccity' => array(
      '#type' => 'textfield',
      '#title' => 'City',
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>' 
    ),
    'bcstate' => array(
      '#type' => 'select',
      '#title' => 'State',
      '#options' => getUsStatesOptions(),
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>' 
    ),
    'bcpostal' => array(
      '#type' => 'masked_input',
      '#mask' => '99999',
      '#title' => 'Zip Code',
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div></div>' 
    ),
  );
  
  if(isset($form_state['values']['bcompopt'])){ 
    $bagentopt = getBrokerAgentList($form_state['values']['bcompopt']); 
  } 
  elseif (!empty($node->field_broker_company->value())) { 
    $bagentopt = getBrokerAgentList($node->field_broker_company->value()->nid); 
  }
  else { 
    $bagentopt = getBrokerAgentList(); 
  } 

  $form['broker']['bacontainer'] = array (
    '#type' => 'container',
    '#prefix' => '<div id="agents-wrapper">',
    '#suffix' => '</div>',     
    'bagentopt' => array (
      '#type' => 'select',
      '#title' => 'Broker Agent',
      '#options' => $bagentopt,
      '#default_value' => !empty($node->field_broker_agent->value()) ? $node->field_broker_agent->value()->nid : 0,
      '#prefix' => '<div class="row" style="margin-top:20px;"><div class="col-sm-12 col-md-3">',
      '#suffix' => '</div></div>',  
    ),
  );  

  $form['broker']['baform'] = array (
    '#type' => 'container',
    '#states' => array('visible' => array('select[name="bagentopt"]' => array(array('value' => '0')))),
    'baname' => array(
      '#type' => 'textfield',
      '#title' => 'Agent Name',
      '#prefix' => '<div class="row" style="margin-top:10px;"><div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>'
    ),
    'baemail' => array(
      '#type' => 'textfield',
      '#title' => 'Email Address',
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>'
    ),
    'baphone' => array(
      '#type' => 'masked_input',
      '#mask' => '(999) 999 9999',
      '#title' => 'Phone Number',
      '#prefix' => '<div class="col-sm-12 col-md-2">',
      '#suffix' => '</div>',
    ),
    'baext1' => array(
      '#type' => 'textfield',
      '#title' => 'Ext 1',
      '#maxlength' => 5, 
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div>',
    ),
    'baext2' => array(
      '#type' => 'textfield',
      '#title' => 'Ext 2',
      '#maxlength' => 5, 
      '#prefix' => '<div class="col-sm-12 col-md-1">',
      '#suffix' => '</div></div>',
    ),
  );

 /********************
  * TAB 7 - DISPATCH *
  ********************/
  $form['dispatch'] = array(
    '#type' => 'fieldset',
    '#title' => 'Dispatch',
    '#group' => 'loadtabs',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    'dispinfo' => array (
      '#type' => 'item',
      '#markup' => $driver.' '.$form_state['storage']['drphone'],
    ), 
    'dispatchall' => array (
      '#type' => 'checkbox',  
      '#title' => '<strong>Mark all as sent</strong>',
      '#default_value' => 0,
    )
  );

  $dispatchdisable = true;

  foreach ($node->field_loadpoint->value() as $key => $value) {
    // default values
    $lpextra = '';
    $lpdisabled = 0;
    $lpchecked = 1;
    $lpvalue = '';    
    $lptype = $node->field_loadpoint[$key]->field_type->value();
    $lpware = $node->field_loadpoint[$key]->field_warehouse->value();
    $lpdate = null;
    if(!empty($node->field_loadpoint[$key]->field_date_time->value())){
      $lpdate = date('M/d/y H:i', $node->field_loadpoint[$key]->field_date_time->value());
    }

    // address(disabled, unchecked)
    $lpaddre = $node->field_loadpoint[$key]->field_address->thoroughfare->value(); 
    if(empty($lpaddre)) { 
      $lpdisabled = 1; 
      $lpchecked = 0; 
      if (empty($lpextra)) { $lpextra .= 'Missing '; }
      else { $lpextra .= ', '; }
      $lpextra .= 'address'; 
    }
    else { $lpvalue .= $lpaddre . ', '; }

    // city (disabled, unchecked)
    $lpcity = $node->field_loadpoint[$key]->field_address->locality->value(); 
    if(empty($lpcity)) { 
      $lpdisabled = 1;
      $lpchecked = 0; 
      if (empty($lpextra)) { $lpextra .= 'Missing '; }
      else { $lpextra .= ', '; }
      $lpextra .= 'city'; 
    }
    else { $lpvalue .= $lpcity . ', '; }

    // state (disabled, unchecked)
    $lpstate = $node->field_loadpoint[$key]->field_address->administrative_area->value(); 
    if(empty($lpstate)) { 
      $lpdisabled = 1; 
      $lpchecked = 0; 
      if (empty($lpextra)) { $lpextra .= 'Missing '; }
      else { $lpextra .= ', '; }
      $lpextra .= 'state'; 
    }
    else { $lpvalue .= $lpstate . ' '; }
    
    // zip (disabled, unchecked)
    $lpzcode = $node->field_loadpoint[$key]->field_address->postal_code->value(); 
    if(empty($lpzcode)) { 
      $lpdisabled = 1; 
      $lpchecked = 0; 
      if (empty($lpextra)) { $lpextra .= 'Missing '; }
      else { $lpextra .= ', '; }
      $lpextra .= 'zip code'; 
    }
    else { $lpvalue .= $lpzcode; }
    
    // phone validation
    if(empty($form_state['storage']['drphone'])){
      $lpdisabled = 1; 
      $lpchecked = 0; 
      if (empty($lpextra)) { $lpextra .= 'Missing '; }
      else { $lpextra .= ', '; }
      $lpextra .= 'phone number'; 
    }
    
    // end extra
    if (!empty($lpextra)) { $lpextra = '<label class="label label-danger">'.$lpextra.'</label>'; }
    
    // dispatched already (unchecked)
    $lpdispa = $node->field_loadpoint[$key]->field_dispatched->value(); 
    if($lpdispa == 1) { 
      $lpextra = '<label class="label label-success"> dispatched </label> &nbsp;' . $lpextra;
      $lpchecked = 0; 
    }    

    $form_state['storage']['dispatch'][$key] = array ('address' => $lpvalue, 'type' => $lptype, 'date' => $lpdate, 'ware' => $lpware);

    $form['dispatch']['lp'.$key] = array (
      '#type' => 'checkbox',  
      '#title' => $lpvalue,
      '#disabled' => $lpdisabled,
      '#default_value' => $lpchecked,
      '#prefix' => $lpextra,
    );
    
    if($lpdisabled == 0){ $dispatchdisable = false;}

  }

  $form['dispatch']['send'] = array(
    '#type' => 'submit',
    '#value' => 'Dispatch',
    '#submit' =>  array('load_dispatch_form_submit'),
    '#disabled' => $dispatchdisable,
  );

 /*****************************
  **** UPDATE AND CONTINUE ****
  *****************************/
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => 'Update',
    '#submit' =>  array('load_update_form_submit'),
  );

 /*************************
  **** UPDATE AND EXIT ****
  *************************/
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save & Exit',
    '#submit' =>  array('load_update_form_submit'),
  );

  return $form;   
}

/**
 * Callback function for refresh broker agent
 */
function refresh_bragent($form, $form_state) {
  return $form['broker']['bacontainer'];
}