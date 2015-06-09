<?php
require_once('common.inc');
function workshops_add_resource_person_form($form_state) {

	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['rp_fname'] = array(
		'#type' => 'textfield',
		'#title' => t('Full Name'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
	);
 	$form['uid'] = array(
 		'#type' => 'hidden',
 		''
 	);
	$form['email'] = array(
		'#type' => 'textfield',
		'#title' => t('E-mail'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('class' => 'rp_user_email'),
	);

	$form['username'] = array(
		'#type' => 'textfield',
		'#title' => t('User Name'),
		'#size' => 30,
		'#maxlength' => 20,
		'#attributes' => array('class' => 'rp_user_name', 'readonly' => 'readonly'),
	);
	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone'),
		'#size' => 30,
		'#required' => TRUE
	);

	$form['states'] = array(
		'#type' => 'select',
		'#multiple' => TRUE,
		'#title' => t('States<br />(Press ctrl & click to select multiple states)'),
		'#required' => TRUE,
		'#attributes' => array('id' => 'resource_person_state'),
		'#options' => get_states(0),
		'#prefix' => '<div class="rp-states">',
		'#suffix' => '<div id="rp-sel-states" class="form-item"><label><h1 class="title">Selected States:</h1></label></div></div>'
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function workshops_add_resource_person_form_validate($form, &$form_state){
	if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
		form_set_error('phone', t('Invalid phone number'));
	}
	if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
		form_set_error('email', t('Invalid Email'));
	}else{
		$query = "select user_uid from resource_person where user_uid='".$form_state['values']['uid']."'";
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		$row = db_fetch_object($result);
		if($row){
			form_set_error('email', t('The e-mail address '.$form_state['values']['email'].' is already registered as Resource Person.'));
		}
	}if($form_state['values']['uid'] == ''){
		form_set_error('email', t('This email have not register yet'));
	}
  return;
}

function workshops_add_resource_person_form_submit($form, &$form_state){
	$selected_states = '';
	foreach($form_state['values']['states'] as $sel_state){
		if($selected_states != '')
			$selected_states .= ",";
		$selected_states .= $sel_state;
	}
	$query = "insert into resource_person (user_uid,rp_fname,phone,states) values('".$form_state['values']['uid']."', '".trim($form_state['values']['rp_fname'])."', '".trim($form_state['values']['phone'])."', '".$selected_states."')";
	db_set_active('workshop_info');
	if(db_query($query)){
		db_set_active('default');
	    // mail to resource person
	    $mailto = $form_state['values']['email'];
	    $subject = 'Account notification';
	    $message = "<p>Dear ".$form_state['values']['rp_name'].",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>. Your application for an account is currently pending approval. Once it has been approved, you will receive another e-mail containing information about how to log in, set your password, and other details.";
		workshops_notify($mailto, $subject, $message);
		
		// send email to administrator
		
		$mailto = variable_get('site_mail', '');
	    //$mailfrom = $form_state['values']['email'];
	    $subject = 'Request for Resource Person in spoken-tutorial.org (pending admin approval)';
	    $message = "<p>Dear Administrator,</p><p> ".$form_state['values']['username']." registred as resource person. Please 
assign role as resouce person</p>";
		workshops_notify($mailto, $subject, $message);
		drupal_set_message('Resource Person Login Details Added Successfully, User Name: '.$form_state['values']['username']);
	}else {
		drupal_set_message('Error Adding Resource Person Login Details');
		db_set_active('default');
	}
	db_set_active('default');
	drupal_goto($path = 'workshops/college', $query = NULL, $fragment = NULL, $http_response_code = 302);
	//echo '<br />'.$query;
	//exit;
}
	
