<?php
/**
 * Form Validation
 */
function load_update_form_validate($form, &$form_state) { 
  
  // VALID BROKER COMPANY EMAIL
  $bcemail = $form_state['values']['bcemail'];
  if(!empty($bcemail) && !valid_email_address($bcemail)) {
    form_set_error('bcemail', 'Please, insert a valid email address.');
  } 

  // VALID BROKER AGENT EMAIL
  $baemail = $form_state['values']['baemail'];
  if(!empty($baemail) && !valid_email_address($baemail)) { 
  	form_set_error('baemail', 'Please, insert a valid email address.'); 
  }
  
  // GROSS & SOLD PAYMENT
  $gross = $form_state['values']['grospay'];
  if(empty($gross)){ form_set_error('grospay', 'Please, define a gross payment.'); } 
  if(!is_numeric($gross)){ form_set_error('grospay', 'Please, define a numeric value.'); } 

  $sold = $form_state['values']['soldpay'];
  if(empty($sold)){ form_set_error('soldpay', 'Please, define a sold payment.'); } 
  if(!is_numeric($sold)){ form_set_error('grospay', 'Please, define a numeric value.'); } 
  
  if($sold > $gross){ form_set_error('grospay', 'Gross payment has to be greater than sold payment.'); } 

  // AUDIT ONLY COMPLETE LOADS
  $status = $form_state['values']['status'];
  $audit = $form_state['values']['audit'];
  if($audit && $status != 'Completed'){
    form_set_error('audit', 'Only completed loads can be audited.');
  }
}