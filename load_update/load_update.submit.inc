<?php
function load_update_form_submit($form, &$form_state) {
  /*************************
   ******* NODE LOAD *******
   *************************/ 
  global $user;
  $author_full = user_load($user->uid)->field_full_name['und'][0]['value'];
  $author_name = $user->name; 
 
  $event_log = '';
  $raw_node = node_load($form_state['storage']['load']['nid']);
  $node = entity_metadata_wrapper('node', $raw_node);
  
  /*******************************************
   ******* GENERAL INFORMATION (TAB 2) *******
   *******************************************/ 
  // Broker Load Number
  $loadnum = clean($form_state['values']['loadnumber']);
  if($node->field_broker_load_number->value() != $loadnum){
    $node->field_broker_load_number->set(check_plain($loadnum));
    $event_log .= empty($event_log) ? 'load number' : ', load number';
  }  
  // Audited
  $audited = $form_state['values']['audit'];
  if($node->field_audited->value() != $audited){
    $node->field_audited->set($audited);
    $event_log .= empty($event_log) ? 'audited' : ', audited';
  }
  // Paid
  $paid = $form_state['values']['paid'];
  if($node->field_paid->value() != $paid){
    $node->field_paid->set($paid);
    $event_log .= empty($event_log) ? 'paid' : ', paid';
  } 
  // Status
  $status = $form_state['values']['status'];
  if($node->field_csctatus->value() != $status){
    $node->field_csctatus->set($status);
    $event_log .= empty($event_log) ? 'status to ' : ', status to ';
    $event_log .= $status;
  } 
  // Gross
  $gross = cleanStrToNumbers($form_state['values']['grospay']);
  if($node->field_gross_payment->value() != $gross){
    $node->field_gross_payment->set($gross);
    $event_log .= empty($event_log) ? 'gross' : ', gross';
  } 
  // Sold
  $sold = cleanStrToNumbers($form_state['values']['soldpay']);
  if($node->field_sold_payment->value() != $sold){
    $node->field_sold_payment->set($sold);
    $event_log .= empty($event_log) ? 'sold' : ', sold';
  } 
  // Rate
  $rate = round($sold / $form_state['values']['totalmi'], 2);
  if($node->field_rate_x_miles->value() != $rate){
    $node->field_rate_x_miles->set($rate);
    $event_log .= empty($event_log) ? 'rate' : ', rate';
  } 
  // Rate confirmation
  if($form_state['values']['rate'] != $form_state['storage']['load']['fid']){
    $rateconfirm = file_load($form_state['values']['rate']);
    if ($rateconfirm) {
      $rateconfirm->status = FILE_STATUS_PERMANENT;
      file_save($rateconfirm);
      $node->field_rate_confirmation = array(
        'fid' => $rateconfirm->fid,
        'display' => TRUE,
        'description' => $rateconfirm->filename,
      );
    }
    if(!is_null($form_state['storage']['load']['fid'])){
      $oldrate = file_load($form_state['storage']['load']['fid']);
      file_delete($oldrate, TRUE);
    }
    $event_log .= empty($event_log) ? 'rate confirmation' : ', rate confirmation';
  } 

  /*******************************************
   ******* ITINERARY & PAYMENTS (TAB 3) ******
   *******************************************/ 
  // Initial Point
  $address = clean($form_state['values']['address']);
  $city = clean($form_state['values']['city']);
  $state = $form_state['values']['state'];
  $zip = $form_state['values']['zip'];
  if(clean($node->field_initial_point->thoroughfare->value()) != $address){
    $event_log .= empty($event_log) ? 'initial address' : ', initial address';
  }
  // Initial city
  if(clean($node->field_initial_point->locality->value()) != $city){
    $event_log .= empty($event_log) ? 'initial city' : ', initial city';
  }
  // Initial state
  if($node->field_initial_point->administrative_area->value() != $state){
    $event_log .= empty($event_log) ? 'initial state' : ', initial state';
  }
  // Initial zip code
  if($node->field_initial_point->postal_code->value() != $zip){
    $event_log .= empty($event_log) ? 'initial zip' : ', initial zip';
  }
  $node->field_initial_point->set(
    array(
      "country" => "US",
      "thoroughfare" => check_plain($address),
      "locality" => check_plain($city),
      "administrative_area" => $state,
      "postal_code" => $zip
    )
  );
  
  foreach ($node->field_loadpoint->value() as $key => $value) {
    // index
    $point_index = $key + 1;
    
    // point address
    $loadpoint = entity_metadata_wrapper('field_collection_item', $value);
    $address = clean($form_state['values']['address'.$key]);
    $city = clean($form_state['values']['city'.$key]);
    $state = $form_state['values']['state'.$key];
    $zip = $form_state['values']['zip'.$key];

    if(clean($loadpoint->field_address->thoroughfare->value()) != $address){
      $event_log .= empty($event_log) ? 'point '.$point_index.' address' : ', point '.$point_index.' address';
    }
    
    // point city
    if(clean($loadpoint->field_address->locality->value()) != $city){
      $event_log .= empty($event_log) ? 'point '.$point_index.' city' : ', point '.$point_index.' city';
    }
    
    // point state
    if($loadpoint->field_address->administrative_area->value() != $state){
      $event_log .= empty($event_log) ? 'point '.$point_index.' state' : ', point '.$point_index.' state';
    }
    
    // point zip code
    if($loadpoint->field_address->postal_code->value() != $zip){
      $event_log .= empty($event_log) ? 'point '.$point_index.' zip' : ', point '.$point_index.' zip';
    }
    $loadpoint->field_address->set(
      array(
        "country" => "US",
        "thoroughfare" => check_plain($address),
        "locality" => check_plain($city),
        "administrative_area" => $state,
        "postal_code" => $zip
      )
    );
    
    // point type
    $point_type = $form_state['values']['type'.$key];
    if($loadpoint->field_type->value() != $point_type){
     $loadpoint->field_type->set($point_type);
     $event_log .= empty($event_log) ? 'point '.$point_index.' type ('.$point_type.')' 
                                     : ', point '.$point_index.' type ('.$point_type.')';
    }
    
    // point warehouse
    $point_ware = clean($form_state['values']['ware'.$key]);
    if(clean($loadpoint->field_warehouse->value()) != $point_ware){
      $loadpoint->field_warehouse->set(check_plain($point_ware));
      $event_log .= empty($event_log) ? 'point '.$point_index.' warehouse' : ', point '.$point_index.' warehouse';
    }
    
    // point date
    $date = !empty($form_state['values']['date'.$key]) ? $form_state['values']['date'.$key] : NULL;
    $time = !empty($form_state['values']['time'.$key]) ? $form_state['values']['time'.$key] : '00:00:00';
    if($time != '00:00:00'){
      $time = date('h:i:s A', strtotime($time));
    }
    $final = !is_null($date) ? strtotime($date.' '.$time) : NULL;
    if($loadpoint->field_date_time->value() != $final){
      $loadpoint->field_date_time->set($final);
      $event_log .= empty($event_log) ? 'point '.$point_index.' date' : ', point '.$point_index.' date';
    }

    // save loadpoint
    $loadpoint->save();
  }
  
  /*****************************
   ******* EVENTS (TAB 4) ******
   *****************************/ 
  if(isset($form_state['storage']['events']) && $form_state['storage']['events'] > 0) { 
    for($i = 0; $i < $form_state['storage']['events']; $i++){
      $event_date = $form_state['values']['edate'.$i];
      $event_time = $form_state['values']['etime'.$i];
      $event_desc = $form_state['values']['edesc'.$i];
      if(!empty($event_date) && !empty($event_time) && !empty($event_desc)){
        $event = entity_create('field_collection_item', array('field_name' => 'field_events'));
        $event->setHostEntity('node', $node->value());
        $wrap_event = entity_metadata_wrapper('field_collection_item', $event);
        $wrap_event->field_description->set(clean($event_desc));
        $event_final = strtotime($event_date .' '. date('h:i:s A', strtotime($event_time)));
        $wrap_event->field_date->set($event_final);
      }
    }
  }  
  
  /*****************************
   ******* NOTES (TAB 5) *******
   *****************************/ 
  $node->body->value->set(check_plain(clean($form_state['values']['notes_desc'])));
  
  /************************************
   ******* BROKER COMPANY (TAB 6) *****
   ************************************/ 
  if($form_state['values']['bcompopt'] != 0) {
    $node->field_broker_company->set($form_state['values']['bcompopt']);
  } 
  elseif (!empty($form_state['values']['bcname'])) {
    $bcomp_entity = entity_create('node', array('type' => 'broker_company'));
    $bcomp_entity->uid = $user->uid;
    $bcomp = entity_metadata_wrapper('node', $bcomp_entity);
    $bcomp->title->set(check_plain(clean($form_state['values']['bcname'])));
    $bcomp->field_mctext->set(check_plain($form_state['values']['bcmc'])); 
    $bcomp->field_dot->set(check_plain($form_state['values']['bcdot'])); 
    $bcomp->field_email->set(check_plain($form_state['values']['bcemail'])); 
    $bcomp->field_phone->set(check_plain($form_state['values']['bcphone'])); 
    $bcomp->field_ext1->set(check_plain(cleanStrToNumbers($form_state['values']['bcext1']))); 
    $bcomp->field_ext2->set(check_plain(cleanStrToNumbers($form_state['values']['bcext2']))); 
    $bcomp->field_address->set(array(
      "country" => "US",
      "thoroughfare" => check_plain(clean($form_state['values']['bcaddress'])),
      "locality" => check_plain(clean($form_state['values']['bccity'])),
      "administrative_area" => $form_state['values']['bcstate'],
      "postal_code" => $form_state['values']['bcpostal'])
    );
    $bcomp->save();
    $node->field_broker_company->set($bcomp_entity->nid);
  }
  
  /***********************************
   ******* BROKER AGENT (TAB 6) ******
   ***********************************/ 
  if ($form_state['values']['bagentopt'] != 0){ 
    $node->field_broker_agent->set($form_state['values']['bagentopt']);
  } 
  elseif (!empty($form_state['values']['baname'])) {
    $bagent_entity = entity_create('node', array('type' => 'broker_agent'));
    $bagent_entity->uid = $user->uid;
    $bagent = entity_metadata_wrapper('node', $bagent_entity);
    $bagent->title->set(check_plain(clean($form_state['values']['baname'])));
    $bagent->field_email->set($form_state['values']['baemail']); 
    $bagent->field_phone->set($form_state['values']['baphone']); 
    $bagent->field_ext1->set(check_plain(cleanStrToNumbers($form_state['values']['baext1']))); 
    $bagent->field_ext2->set(check_plain(cleanStrToNumbers($form_state['values']['baext2']))); 
    $bagent->field_broker_company->set($node->field_broker_company->value()->nid);
    $bagent->save();  
    $node->field_broker_agent->set($bagent_entity->nid);
  }

  /***************************
   ******* UPDATE EVENT ******
   ***************************/
  if(!empty($event_log)){
    $event = entity_create('field_collection_item', array('field_name' => 'field_events'));
    $event->setHostEntity('node', $node->value());
    $wrap_event = entity_metadata_wrapper('field_collection_item', $event);
    $wrap_event->field_description->set('Updated by '.$author_full.' ('.$author_name.') Changes: ' . $event_log);
    $wrap_event->field_date->set(strtotime(date('M d Y h:i:s A')));
  }
  
  // SAVING
  $node->save();
  drupal_set_message('Load updated successfully.', 'status');
  
  // send email (canceled loads) 
  if($status == 'Canceled' && $node->field_single_carrier->field_rateauto->value()) {
    $subject = 'This load has been CANCELED !!! LOAD ' . $node->title->value();   
    $body  = '<h1 style="background-color: red;"> CANCELED </h1>';
    $body .= '<strong> Dispatcher Name: </strong> ' . $node->author->field_full_name->value() . '<br>';
    $body .= '<strong> Owner Operator:  </strong> ' . $node->field_customer->value()->title . '<br>';
    $body .= '<strong> Driver: </strong> ' . $node->field_driver[0]->value()->title . '<br>';
    $body .= '<strong> Empty Miles: </strong> ' . $node->field_empty_miles->value() . '<br>';
    $body .= '<strong> Total Miles: </strong> ' . $node->field_total_miles->value() . '<br>';
    $body .= '<strong> Load Price: </strong> USD ' . $node->field_gross_payment->value() . '<br>';
    if($node->field_sold_payment->value() < $node->field_gross_payment->value()) {
      $body .= '<strong> VRate: </strong> USD ' . $node->field_sold_payment->value() . '<br>';
    }    
    $body .= '<strong> Rate x Miles: </strong> ' . $node->field_rate_x_miles->value() . ' USD/MILE';
    $body .= '<br><br><br>';
    $body .= '--- The email was sent using 4Global Dispatch Software ---';      
    
    $attachment_object = file_load($node->field_rate_confirmation->value()['fid']);  
    $attachment = array(
      'filecontent' => file_get_contents($attachment_object->uri),
      'filemime' => $attachment_object->filemime
    );

    $language = language_default();
    $to = $node->field_single_carrier->field_email->value();
    $params['subject'] = $subject;
    $params['body'] = drupal_wrap_mail($body);
    $params['cc'] = $user->mail;
    $params['attachment'] = $attachment;  
    $result = drupal_mail('load_notification', 'rate', $to, $language, $params);
   
    if($result['result'] && $result['send']){
      // Email Event
      $event = entity_create('field_collection_item', array('field_name' => 'field_events'));
      $event->setHostEntity('node', $node->value());
      $node_event = entity_metadata_wrapper('field_collection_item', $event);
      $node_event->field_description->set('Rate confirmation sent to: '.$to);
      $node_event->field_date->set(strtotime(date('M d Y h:i:s A')));
      drupal_set_message('The cancelation email has been sent successfully.', 'status');   
    }
    else {
      drupal_set_message('There was an error sending the rate confirmation. Please, contact developmet team.', 'error');
    }
  }
 
  if($form_state['values']['op'] == 'Save & Exit'){
    $form_state['redirect'] = 'dispatch';
  } 
   else {
    $form_state['redirect'] = 'load/edit/'.$form_state['storage']['load']['nid']; 
  }  
}


// Dispatch Address
function load_dispatch_form_submit($form, &$form_state){
  $raw_node = node_load($form_state['storage']['load']['nid']);
  $node = entity_metadata_wrapper('node', $raw_node);
  $dispatchall = $form_state['values']['dispatchall'];
  $phone = cleanStrToNumbers($form_state['storage']['drphone']);
  
  foreach ($node->field_loadpoint->value() as $key => $value) {
    if($form_state['values']['dispatchall']) { 
      $loadpoint = entity_metadata_wrapper('field_collection_item', $value);
      $loadpoint->field_dispatched->set(1); 
      $loadpoint->save();
      // add event mark as dispatched
    }
    elseif($form_state['values']['lp'.$key]) {
      // message
      $message = mb_strtoupper($form_state['storage']['dispatch'][$key]['type']) . ' INFO --- ';
      $message .= 'Warehouse: ' . $form_state['storage']['dispatch'][$key]['ware'] . ' --- ';
      $message .= 'Address: ' . $form_state['storage']['dispatch'][$key]['address'] . ' --- ';
      $message .= 'Date: ' . $form_state['storage']['dispatch'][$key]['date'];
      $sent = twilio_send($phone, $message, 1, NULL);
      // set point as dispatched
      if($sent) { 
        $loadpoint = entity_metadata_wrapper('field_collection_item', $value);
        $loadpoint->field_dispatched->set(1); 
        $loadpoint->save();
        drupal_set_message('A text message has been sent to driver ('.$phone.').', 'status');  
       // add event mark as dispatched and including message        
      }
    }
  }
  $form_state['rebuuild'] = TRUE;
}