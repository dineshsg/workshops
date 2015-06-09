<?php

require_once('common.inc');

function workshops_add_department_form($form_state) {

	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	drupal_add_css($path . '/workshops.css', 'module');
	global $user;
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data", 'name' => 'add_college_form');
	$form['department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department'),
		'#required' => TRUE,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}

function workshops_add_department_form_validate($form, &$form_state){
	if (!preg_match('/^[a-zA-Z ]+$/', $form_state['values']['department'])){
		form_set_error('department', t('Invalid Department'));
	}

}

function workshops_add_department_form_submit($form, &$form_state){
	$query ="insert into departments (name) values('".$form_state['values']['department']."')";
	db_set_active('workshop_info');
	if(db_query($query)){
		drupal_set_message('Department Added Successfully');
	}else {
		drupal_set_message('Department already exist');
	}
	db_set_active('default');
	drupal_goto($path = 'workshops/department', $query = NULL, $fragment = NULL, $http_response_code = 302);

}

function workshops_add_college_form($form_state) {

	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	global $user;
	$states_array = get_states(0);
	
	# get current resource person name
	$query = "select states from resource_person where user_uid='" . $user->uid."'";
	db_set_active('workshop_info');
	$result = db_query($query);
	db_set_active('default');
	$row = db_fetch_object($result);
	$state_codes = explode(",", $row->states);
	
	$states = array();
	$states[''] = "-- select --";
	foreach($state_codes as $st_code){
		$states[$st_code] = $states_array[$st_code];
	}
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#required' => TRUE,
		'#options' => $states,
		'#attributes' => array('class' => 'college_state')
	);

	$form['academic_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Academic Center Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_academic_code'),
		'#prefix' => '<div class="coll_academic_code_div">',
		'#suffix' => '</div>'
	);
	
	$form['school_college'] = array(
		'#type' => 'radios',
		'#title' => t('College / school'),
		'#options' => array(1 => 'College', 0 => 'School'),
		'#required' => TRUE,
		'#default_value' => 1,
		'#prefix' => '<div class="skype-request">',
		'#suffix' => '</div>'
	);
	
	$form['college_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Institution Name'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array()
	);

	$form['street'] = array(
		'#type' => 'textarea',
		'#title' => t('Address'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array( 'rows' => '4', 'cols' => '30')
	);

	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array()
	);

	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pin Code'),
		'#size' => 30,
		'#required' => TRUE,
	);
	
	$form['resource_center'] = array(
		'#type' => 'radios',
		'#title' => t('Resource center'),
		'#options' => array(1 => 'Yes', 0 => 'No'),
		'#required' => TRUE,
		'#default_value' => 1,
		'#attributes' => array('class' => 'resource_center'),
		'#prefix' => '<div class="skype-request">',
		'#suffix' => '</div>'
	);
	
	$form['star_rating'] = array(
		'#type' => 'select',
		'#title' => t('Star Rating'),
		'#required' => TRUE,
		'#options' => array(0 => '0', 1 => '1', 2 => '2', 3 => '3', 4 => '4', 5 => '5'),
		'#attributes' => array('class' => 'college_state'),
		'#prefix' => '<div class="resource_center_details">'
	);
	
	$form['contact_details'] = array(
		'#type' => 'textarea',
		'#title' => t('Contact details'),
		'#size' => 30,
		'#attributes' => array( 'rows' => '4', 'cols' => '30')
	);
	
	$form['remarks'] = array(
		'#type' => 'textarea',
		'#title' => t('Activities'),
		'#size' => 30,
		'#attributes' => array( 'rows' => '4', 'cols' => '30'),
		'#suffix' => '</div>'
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function workshops_add_college_form_submit($form, &$form_state){
	global $user;
	$query = "insert into academic_center (state_code, academic_code, school_college, institution_name, street, city, pincode, resource_center, star_rating, contact_details, remarks, created_at) values('".$form_state['values']['state']."', '".$form_state['values']['academic_code']."', ".$form_state['values']['school_college'].", '".db_escape_string(trim($form_state['values']['college_name']))."', '".db_escape_string(trim($form_state['values']['street']))."', '".db_escape_string(trim($form_state['values']['city']))."', '".trim($form_state['values']['pincode'])."', ".$form_state['values']['resource_center'].", ".$form_state['values']['star_rating'].", '".$form_state['values']['contact_details']."', '".$form_state['values']['remarks']."', '".date('Y-m-d H:i:s')."')";
	db_set_active('workshop_info');
	if(db_query($query)){
		drupal_set_message('College Details Added Successfully');
	}else {
		drupal_set_message('Error Adding College Details');
	}
	db_set_active('default');
	drupal_goto($path = 'workshops/college', $query = NULL, $fragment = NULL, $http_response_code = 302);
	// echo $query;
	// exit;
}
function workshops_add_college_form_validate($form, &$form_state){
	if (!preg_match('/^[0-9\ \+]{0,6}$/', $form_state['values']['pincode'])){
		form_set_error('pincode', t('Invalid Pincode number'));
	}
	if($form_state['values']['resource_center'] == 1) {
		if(trim($form_state['values']['contact_details']) == '') {
			form_set_error('contact_details', t('Please fill contact details'));
		}
		if(trim($form_state['values']['remarks']) == '') {
                        form_set_error('remarks', t('Please fill Activities field'));
                }
	}
  return;
}

function workshops_list_college_details(){
	global $user;
	$filter = unserialize(base64_decode($_REQUEST['filter']));
	$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0";
	$count_query = "SELECT count(ac.id) FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0";
	if(isset($filter['state']) && $filter['state'] != ''){
		if($filter['type']=='polytechnic')
		{
			$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '%".$filter['type']."%'";
			$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '%".$filter['type']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		else if($filter['type']=='iti')
		{
			$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '".$filter['type']."%'";
			$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '".$filter['type']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		else if($filter['type']=='school')
		{
			$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=0 and rp.user_uid='".$user->uid."' and ac.academic_code LIKE '".$filter['state']."%'";
			$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=0 and rp.user_uid='".$user->uid."' and ac.academic_code LIKE '".$filter['state']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		else
		{
		$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE '%iti%'";
		$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE '%iti%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		
	}
	$header = array(
		array('data'=>'Academic Center Code', 'field'=>'ac.academic_code', 'sort'=>'asc'),
		array('data'=>'Institution', 'field'=>'ac.institution_name'),
		array('data'=>'City', 'field'=>'ac.city'),
		array('data'=>'Action')
	);
	$query .= tablesort_sql($header);
	db_set_active('workshop_info');
	$result = pager_query($query , 30, 0, $count_query);
	$count = db_query($count_query);
	$count = db_fetch_object($count);
	db_set_active('default');
	$row = array();
	
	while ($item = db_fetch_object($result)) {
		$row[] = array($item->academic_code, $item->institution_name, $item->city, l(t('Edit'), 'workshops/college/edit_college/'. $item->id , array('attributes' => array('class' => 'edit-record', 'title' => 'Edit')))." ".l(t('View'), 'workshops/college/view_college/'. $item->id , array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
	}
		$output .= "<p><b>Total no of Colleges : ".$count->count."</b></p>";
	$output .= drupal_get_form('state_filter_form', $selected_val);
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	} else {
		drupal_set_message($message);
		$output .= theme_table($header,$row);
		$output .= theme_pager(null, 30);
	}
	return $output;
}

function workshops_list_school_details(){
	global $user;
	
	$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=0 and rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0";
	$count_query = "SELECT count(ac.id) FROM `academic_center` ac, resource_person rp WHERE ac.school_college=0 rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0";
	if(isset($filter['state']) && $filter['state'] != ''){
		if($filter['type']=='polytechnic')
		{
			$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '%".$filter['type']."%'";
			$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '%".$filter['type']."%'";
		}
		else if($filter['type']=='iti')
		{
			$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '".$filter['type']."%'";
			$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' and ac.institution_name LIKE '".$filter['type']."%'";
		}
		else if($filter['type']=='school')
		{
			$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=0 and rp.user_uid='".$user->uid."' and ac.academic_code LIKE '".$filter['state']."%'";
			$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=0 and rp.user_uid='".$user->uid."' and ac.academic_code LIKE '".$filter['state']."%'";
		}
		else
		{
		$query = "SELECT ac.id, ac.academic_code, ac.institution_name, ac.city, rp.states FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE 'iti%'";
		$count_query = "SELECT count(ac.id) as count FROM `academic_center` ac, resource_person rp WHERE ac.school_college=1 and rp.user_uid='".$user->uid."' and ac.state_code='".$filter['state']."' AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE 'iti%'";
		}
		$selected_val = $filter['state'];
	}
	$header = array(
		array('data'=>'Academic Center Code', 'field'=>'ac.academic_code', 'sort'=>'asc'),
		array('data'=>'Institution', 'field'=>'ac.institution_name'),
		array('data'=>'City', 'field'=>'ac.city'),
		array('data'=>'Action')
	);

	$query .= tablesort_sql($header);
	db_set_active('workshop_info');
	$result = pager_query($query , 30, 0, $count_query);
	db_set_active('default');
	$row = array();
	
	while ($item = db_fetch_object($result)) {
		$row[] = array($item->academic_code, $item->institution_name, $item->city, l(t('Edit'), 'workshops/college/edit_college/'. $item->id , array('attributes' => array('class' => 'edit-record', 'title' => 'Edit')))." ".l(t('View'), 'workshops/college/view_college/'. $item->id , array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
	}
	#$output .= "<p><b>Total no of Colleges : ".$count->count."</b></p>";
	#$output .= drupal_get_form('state_filter_form', $selected_val);

	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	} else {
		drupal_set_message($message);
		$output .= theme_table($header,$row);
		$output .= theme_pager(null, 30);
	}
	return $output;
}

function workshops_list_organisers_detail(){
	global $user;
	global $base_url;
	$filter = unserialize(base64_decode($_REQUEST['filter']));
	$query = "SELECT o.id, o.academic_code, o.organiser_name, ac.institution_name,o.city, o.phone FROM organiser o,academic_center ac WHERE o.status=1 and ac.academic_code = o.academic_code and o.academic_code IN (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0)";
	$count_query = "SELECT count(o.id) FROM organiser o WHERE o.status=1 and o.academic_code IN (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0)";
	$selected_val = '';
	if(isset($filter['state']) && $filter['state'] != ''){
		if($filter['type']=='polytechnic'){
		$query = "SELECT o.id, o.academic_code, o.organiser_name, ac.institution_name, ac.institution_name, o.city, o.phone FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code and ac.institution_name LIKE '%".$filter['type']."%'";
		$count_query = "SELECT count(o.id) as count FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code and ac.institution_name LIKE '%".$filter['type']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		else if($filter['type']=='iti'){
		$query = "SELECT o.id, o.academic_code, o.organiser_name, ac.institution_name, o.city, o.phone FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code and ac.institution_name LIKE '%".$filter['type']."%'";
		$count_query = "SELECT count(o.id) as count FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code and ac.institution_name LIKE '%".$filter['type']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		else if($filter['type']=='school'){
		$query = "SELECT o.id, o.academic_code, o.organiser_name, o.city, ac.institution_name, o.phone FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code and ac.school_college=0";
		$count_query = "SELECT count(o.id) as count FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code and ac.school_college=0";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
		else
		{
		$query = "SELECT o.id, o.academic_code, o.organiser_name, ac.institution_name, o.city, o.phone FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE '%iti%'";
		$count_query = "SELECT count(o.id) as count FROM organiser o, academic_center ac WHERE o.status=1 and o.academic_code like '".$filter['state']."%' and ac.academic_code = o.academic_code AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE '%iti%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
		}
	}
	// $header = array('', 'Academic Center Code', 'Name', 'City', 'Phone', 'Action');
	$header =array(
		//array('data'=>'Academic Center Code', 'field'=>'o.academic_code', 'sort'=>'asc'),
		array('data'=>'Institution Name', 'field'=>'ac.institution_name', 'sort'=>'asc'),
		array('data'=>'Name', 'field'=>'o.organiser_name'),
		array('data'=>'City', 'field'=>'o.city'),
		array('data'=>'Phone', 'field'=>'o.phone'),
		array('data'=>'Action'),
	);
	$query .= tablesort_sql($header);
	db_set_active('workshop_info');
	$result = pager_query($query, 30, 0, $count_query);
	$count = db_query($count_query);
	$count = db_fetch_object($count);
	db_set_active('default');
	$row = array();
	
	while ($item = db_fetch_object($result)) {
		$row[] = array($item->institution_name, $item->organiser_name, $item->city, $item->phone, l(t('Edit'), 'workshops/edit_organiser/'. $item->id , array('attributes' => array('class' => 'edit-record', 'title' => 'Edit')))." ".l(t('View'), 'workshops/view_organiser/'. $item->id , array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
	}
	$output .= "<p><b>Total no of Organisers : ".$count->count."</b></p>";
	$output .= drupal_get_form('state_filter_form', $selected_val);
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	} else {
		drupal_set_message($message);
		$output .= theme_table($header,$row);
		$output .= theme_pager(null, 30);
	}
	return $output;
}

function workshops_list_invigilators_detail(){
	global $user;
	global $base_url;
	$filter = unserialize(base64_decode($_REQUEST['filter']));
	$query = "SELECT o.id, o.academic_code, o.invigilator_name, ac.institution_name, o.address, o.city, o.phone FROM invigilator o, academic_center ac WHERE ac.academic_code = o.academic_code and o.academic_code IN (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0) and o.status=1";
	$count_query = "SELECT count(*) FROM invigilator o WHERE o.academic_code IN (SELECT ac.`academic_code` FROM `academic_center` ac, resource_person rp WHERE rp.user_uid='".$user->uid."' and FIND_IN_SET(ac.state_code, rp.states) > 0) and o.status=1 ORDER BY o.academic_code ASC";
	// $header = array('', 'Academic Center Code', 'Name', 'Address', 'City', 'Phone');
	
	$selected_val = '';
	if(isset($filter['state']) && $filter['state'] != ''){
	if($filter['type']=='polytechnic'){
			$query = "SELECT o.id, o.academic_code, o.invigilator_name, ac.institution_name,o.address, o.city, o.phone FROM invigilator o, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 and o.academic_code = ac.academic_code and ac.institution_name LIKE '%".$filter['type']."%'";
		$count_query = "SELECT count(*) as count FROM invigilator o, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 and o.academic_code = ac.academic_code and ac.institution_name LIKE '%".$filter['type']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
	}
	else if($filter['type']=='iti'){
			$query = "SELECT o.id, o.academic_code, o.invigilator_name, ac.institution_name, o.address, o.city, o.phone FROM invigilator o,, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 and o.academic_code = ac.academic_code and ac.institution_name LIKE '".$filter['type']."%'";
		$count_query = "SELECT count(*) as count FROM invigilator o,academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 and o.academic_code = ac.academic_code and ac.institution_name LIKE '".$filter['type']."%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
	}
	else if($filter['type']=='school'){
			$query = "SELECT o.id, o.academic_code, o.invigilator_name, ac.institution_name, o.address, o.city, o.phone FROM invigilator o, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 ac.academic_code = o.academic_code and ac.school_college=0";
		$count_query = "SELECT count(*) as count FROM invigilator o, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 ac.academic_code = o.academic_code and ac.school_college=0";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
	}
	else
	{
		$query = "SELECT o.id, o.academic_code, o.invigilator_name, ac.institution_name, o.address, o.city, o.phone FROM invigilator o, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 and ac.academic_code = o.academic_code AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE 'iti%'";
		$count_query = "SELECT count(*) as count FROM invigilator o, academic_center ac WHERE o.academic_code like '".$filter['state']."%' and o.status=1 and ac.academic_code = o.academic_code AND ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE 'iti%'";
		$selected_val[] = $filter['state'];
		$selected_val[] = $filter['type'];
	}
	}
	$header =array(
//		array('data'=>'Academic Center Code', 'field'=>'o.academic_code', 'sort'=>'asc'),
		array('data'=>'Institution Name', 'field'=>'ac.institution_name', 'sort'=>'asc'),
		array('data'=>'Name', 'field'=>'o.invigilator_name'),
		array('data'=>'City', 'field'=>'o.city'),
		array('data'=>'Phone', 'field'=>'o.phone'),
		array('data'=>'Action'),
	);
	$query .= tablesort_sql($header);
	db_set_active('workshop_info');
	$result = pager_query($query, 30, 0, $count_query);
	$count = db_query($count_query);
	$count = db_fetch_object($count);
	//var_dump($count_query);
	//var_dump($count);
	db_set_active('default');
	$row = array();
	while ($item = db_fetch_object($result)) {
		$row[] = array($item->institution_name, $item->invigilator_name, $item->city, $item->phone, l(t('Edit'), 'workshops/edit_invigilator/'. $item->id , array('attributes' => array('class' => 'edit-record', 'title' => 'Edit')))." ".l(t('View'), 'workshops/view_invigilator/'. $item->id , array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
	}
	$output .= "<p><b>Total no of Invigilates : ".$count->count."</b></p>";
	$output .= drupal_get_form('state_filter_form', $selected_val);
	if (count($row) == 0) {
		$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
	} else {
		drupal_set_message($message);
		$output .= theme_table($header,$row);
		$output .= theme_pager(null, 30);
	}
	return $output;
}

function workshops_edit_college_form($form_state, $id) {

	// $path = drupal_get_path('module', 'workshops');
	// drupal_add_js($path . '/workshops.js', 'module');

	global $user;
	
	# get current resource person name

	$query = "select * from {academic_center} WHERE id='". $id."'";

	db_set_active('workshop_info');
	$result = db_query($query);
	db_set_active('default');
	
	$item = db_fetch_object($result);
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data");

	$form['state'] = array(
		'#type' => 'textfield',
		'#title' => t('State'),
		'#required' => TRUE,
		'#size' => 30,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_state'),
		'#value' => $item->state_code,
	);

	$form['academic_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Academic Center Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array('readonly' => 'readonly', 'class' => 'college_academic_code'),
		'#value' => $item->academic_code,
	);
	
	$form['school_college'] = array(
		'#type' => 'radios',
		'#title' => t('College / school'),
		'#options' => array(1 => 'College', 0 => 'School'),
		'#required' => TRUE,
		'#default_value' => $item->school_college,
		'#prefix' => '<div class="skype-request">',
		'#suffix' => '</div>'
	);
	
	$form['college_name'] = array(
		'#type' => 'textfield',
		'#title' => t('College Name'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array(),
		'#value' => $item->institution_name,
	);

	$form['street'] = array(
		'#type' => 'textarea',
		'#title' => t('Address'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array(),
		'#value' => $item->street,
	);

	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#size' => 30,
		'#required' => TRUE,
		'#attributes' => array(),
		'#value' => $item->city,
	);

	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pin Code'),
		'#size' => 30,
		'#required' => TRUE,
		'#value' => $item->pincode,
	);

	$form['resource_center'] = array(
		'#type' => 'radios',
		'#title' => t('Resource center'),
		'#options' => array(1 => 'Yes', 0 => 'No'),
		'#required' => TRUE,
		'#default_value' => $item->resource_center,
		'#attributes' => array('class' => 'resource_center'),
		'#prefix' => '<div class="skype-request">',
		'#suffix' => '</div>'
	);
	
	$form['star_rating'] = array(
		'#type' => 'select',
		'#title' => t('Star Rating'),
		'#required' => TRUE,
		'#options' => array(0 => '0', 1 => '1', 2 => '2', 3 => '3', 4 => '4', 5 => '5'),
		'#default_value' => $item->star_rating,
		'#attributes' => array('class' => 'college_state'),
		'#prefix' => '<div class="resource_center_details">'
	);
	
	$form['contact_details'] = array(
		'#type' => 'textarea',
		'#title' => t('Contact details'),
		'#size' => 30,
		'#value' => $item->contact_details,
		'#attributes' => array( 'rows' => '4', 'cols' => '30')
	);
	
	$form['remarks'] = array(
		'#type' => 'textarea',
		'#title' => t('Activities'),
		'#size' => 30,
		'#value' => $item->remarks,
		'#attributes' => array( 'rows' => '4', 'cols' => '30'),
		'#suffix' => '</div>'
	);

	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $item->id,
	);
	
	$form['referer'] = array(
		'#type' => 'hidden',
		'#value' => $_SERVER['HTTP_REFERER'],
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}
function workshops_edit_college_form_submit($form, &$form_state){
	$query = "update academic_center set state_code='".$_POST['state']."', institution_name='".db_escape_string($_POST['college_name']);
	$query .= "', street='".db_escape_string($_POST['street'])."', city='".db_escape_string($_POST['city'])."', pincode='".$_POST['pincode']."', school_college=".db_escape_string($_POST['school_college']).", resource_center=".db_escape_string($_POST['resource_center']).", star_rating=".db_escape_string($_POST['star_rating']).", contact_details='".db_escape_string($_POST['contact_details'])."', remarks='".db_escape_string($_POST['remarks'])."'";
	$query .= " where id=".$_POST['id'];
	db_set_active('workshop_info');
	if(db_query($query)){
		drupal_set_message('College Details Added Successfully');
		drupal_goto($_POST['referer'], $query = NULL, $fragment = NULL, $http_response_code = 302);
	}else {
		drupal_set_message('Error Adding College Details');
	}
	db_set_active('default');

	// echo $query;
	// exit;
}

function workshops_edit_form_validate($form, &$form_state) {
	if($form_state['values']['resource_center'] == 1) {
                if(trim($form_state['values']['contact_details']) == '') {
                        form_set_error('resource_center', t('Please fill contact details'));
                }
                if(trim($form_state['values']['remarks']) == '') {
                        form_set_error('remarks', t('Please fill Activities field'));
                }
        }
}
function workshops_view_college_details($id = NULL){
	if($id){
		$states_array = get_states(0);
 		$query = "select * from {academic_center} where id=".$id;
 		
 		db_set_active('workshop_info');
 		$result = db_query($query);
 		db_set_active('default');

		
		db_set_active('workshop_info');
		$result_organiser = db_query($query_organizer);
 		if($result){
 			$item = db_fetch_object($result);
 			$query_organizer = "select organiser_name from organiser where academic_code = '$item->academic_code'";
 		  //var_dump($query_organizer);
 			//die;
 			//$organiser = db_fetch_object($result_organiser);
 			$output = '<table width="100%" cellpadding="6">';
 			$output .= '<tr><td><b>State Code</b></td><td>'.$item->state_code.'</td></tr>';
 			$output .= '<tr><td><b>Academic Center Code</b></td><td>'.$item->academic_code.'</td></tr>';
 			$temp = ($item->school_college)?"College":"School";
 			$output .= '<tr><td><b>College / School</b></td><td>'.$temp.'</td></tr>';
 			$output .= '<tr><td><b>Institution Name</b></td><td>'.$item->institution_name.'</td></tr>';
 			$output .= '<tr><td><b>Address</b></td><td><p>'.$item->street.'<br />'.$item->city.'<br />'.$states_array[$item->state_code].' - '.$item->pincode.'</td></tr>';
 			$output .= '<tr><td colspan="2"><hr /></td></tr>';
 			$output .= '<tr><td><b>Organiser Name</b></td>';
 			while($organiser = db_fetch_object($result_organiser))
 			{
 			$output .= '<td>'.$organiser->organiser_name.'</td></tr>';
 			}
 			$output .= '</table>';

 			return $output;
 		}else {
 			drupal_set_message('Error accessing database workshop_info');
 		}
 	}else{
 		return "<p>Sorry, no data found.</p>";
 	}
}

function edit_organiser($id=NULL){
	if($id){
		$output = drupal_get_form('edit_organiser_form', $id);
	}else{
		$output = "<p>Something went worong, Please try again. </p>";
	}
	return $output;
}
function edit_organiser_form($form_state, $id) {
	$query = "select organiser_name, address, city, phone from organiser where id='".$id."'";
	db_set_active('workshop_info');
	$result = db_query($query);
	db_set_active('default');
	$row = db_fetch_object($result);

	$form['#attributes'] = array('enctype' => "multipart/form-data");
	
	$form['organiser_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Organiser Name'),
		'#required' => TRUE,
		'#attributes' => array('class' => 'organiser_name'),
		'#value' => $row->organiser_name,
	);
	$form['address'] = array(
		'#type' => 'textarea',
		'#title' => t('Address'),
		'#required' => TRUE,
		'#value' => $row->address,
	);
	$form['city'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#required' => TRUE,
		'#value' => $row->city,
	);
	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#required' => TRUE,
		'#value' => $row->phone,
	);
	$form['id'] = array(
		'#type' => 'hidden',
		'#value' => $id,
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}
function edit_organiser_form_validate($form, &$form_state){
	if($_POST['phone'] == ''){
		form_set_error('phone', t('Please fill your phone number'));
		if(strlen($_POST['phone']) == 10){
			form_set_error('phone', t('phone number should be 10 digits'));
		}
	}
	if($_POST['address'] == ''){
		form_set_error('address', t('Please fill your Address'));
	}if($_POST['city'] == ''){
		form_set_error('city', t('Please fill your City'));
	}
	return;
}
function edit_organiser_form_submit($form, &$form_state){
	$query = "update organiser set organiser_name='".$_POST['organiser_name']."', address='".db_escape_string($_POST['address'])."', city='".$_POST['city']."', phone='".$_POST['phone']."' where id='".$_POST['id']."'";

	db_set_active('workshop_info');
	if(db_query($query)){
		db_set_active('default');
		drupal_set_message('organiser Details updated Successfully');
		drupal_goto($path = 'workshops/members/list_organisers', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}else {
		drupal_set_message('Error updating organiser Details');
	}
	db_set_active('default');

	// echo $query;
	// exit;
}
	function view_organiser($id = NULL){
		if($id){
	 		$query = "select academic_code, organiser_name, address, city, phone, organiser_id from {organiser} where id=".$id;
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');

	 		if($result){
	 			$item = db_fetch_object($result);
	 			$query = "select mail from users where uid=".$item->organiser_id;
				$res = db_query($query);
				$value = db_fetch_object($res);
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td>'.$item->academic_code.'</td></tr>';
	 			$output .= '<tr><td><b>Organiser Name</b></td><td>'.$item->organiser_name.'</td></tr>';
	 			$output .= '<tr><td><b>Address</b></td><td><p>'.str_replace("\n", '<br />', $item->address).'</p></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td>'.$item->city.'</td></tr>';
	 			$output .= '<tr><td><b>Phone</b></td><td>'.$item->phone.'</td></tr>';
	 			$output .= '<tr><td><b>Email</b></td><td>'.$value->mail.'</td></tr>';
	 			
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';
	 			$output .= '</table>';

	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
	 	}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}
	
	function workshops_list_organiser_requests(){
		global $user;
		global $base_url;
		
		$query = "select states from resource_person where user_uid=".$user->uid;
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		$row = db_fetch_object($result);
		$state_code_arr = explode(',', $row->states);
		
		$tmp_query_param = "";
		foreach($state_code_arr as $st_code){
			if($tmp_query_param != ""){
				$tmp_query_param .= " or ";
			}
			$tmp_query_param .=	"o.academic_code like '".$st_code."%'";
		}
		//$query = "select id, academic_code, organiser_name, city, phone from organiser where (".$tmp_query_param.") and status=0";
		$query = "select o.id, ac.institution_name,o.academic_code, o.organiser_name, o.city, o.phone from organiser o, academic_center ac where o.academic_code = ac.academic_code and (".$tmp_query_param.") and status=0";
		//echo $query."<br /><br />";
		$count_query = "select count(id) from organiser where (".$tmp_query_param.") and status=0";
		//echo $count_query."<br />"; die;
		
		// $header = array('Academic Center Code', 'Name', 'City', 'Phone', 'Action');
		$header =array(
		//	array('data'=>'Academic Center Code', 'field'=>'academic_code', 'sort'=>'asc'),
			array('data'=>'Institution Name', 'field'=>'ac.institution_name', 'sort'=>'asc'),
			array('data'=>'Name', 'field'=>'organiser_name'),
			array('data'=>'City', 'field'=>'city'),
			array('data'=>'Phone', 'field'=>'phone'),
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query, 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->institution_name, $item->organiser_name, $item->city, $item->phone, l(t('Edit'), 'workshops/accept_organiser/'. $item->id , array('attributes' => array('class' => 'confirm-record', 'title' => 'Accept')))." ".l(t('View'), 'workshops/reject_organiser/'. $item->id , array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
		return $output;
	}
?>
