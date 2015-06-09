<?php
	require_once('common.inc');
	function workshops_add_invigilator(){
		$output = '<div class="form-item" id="academic-details"></div>';
		$output .= drupal_get_form('workshops_add_invigilator_form');
                drupal_set_message('<p>Please note that Invigilator has to confirm test acceptance to invigilate by clicking on <b>"Assessment Test Invigilator Confirmation"</b>.</p>');
		
		return $output;
	}
	function workshops_add_invigilator_form(){
	
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		
		$form['state'] = array(
			'#type' => 'select',
			'#title' => t('State'),
			'#required' => TRUE,
			'#options' => get_states(1),
			'#attributes' => array('class' => 'add_reqs_state')
		);
		$form['academic_code'] = array(
			'#type' => 'select',
			'#title' => t('Institution Name'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#options' => array('' => '-- select --'),
			'#attributes' => array('class' => 'add_reqs_academic_code'),
			'#prefix' => '<div class="reqs_academic_code_div">',
			'#suffix' => '</div>'
		);
		$form['invigilator_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Invigilator Name'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'invigilator_name')
		);
		$form['permanemt_address'] = array(
			'#type' => 'textarea',
			'#title' => t('Permanent Address'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'permanemt_address')
		);
		$form['city'] = array(
			'#type' => 'textfield',
			'#title' => t('City'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'city')
		);
		$form['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'phone')
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshops_add_invigilator_form_validate($form, &$form_state){
		if ($form_state['values']['academic_code'] == ''){
			form_set_error('academic_code', t('Please select Institution Name'));
		}
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone', t('Invalid Phone number'));
		}
		
  		return;

	}
	function workshops_add_invigilator_form_submit($form, &$form_state){
		global $user;
		
		$query = "select id from invigilator where invigilator_id='".$user->uid."'";
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		if($row = db_fetch_object($result)){
			if($row->status == 0){
				drupal_set_message('You have already sent the Invigilator request, your request is waiting for Organiser\'s approval');
			}else{
				drupal_set_message('You already have an Invigilator permissions');
			}
		}else{
			$query = "insert into invigilator (academic_code,invigilator_name,address,city,phone,invigilator_id,status, created_at) values('".$form_state['values']['academic_code']."','".$form_state['values']['invigilator_name']."', '".$form_state['values']['permanemt_address']."', '".$form_state['values']['city']."', '".$form_state['values']['phone']."', '".$user->uid."', 0, '".date('Y-m-d H:i:s')."')";	
			db_set_active('workshop_info');
			if(db_query($query)){
			    db_set_active('default');
			    // send email to Resource person
			    $tem = explode('-', $form_state['values']['academic_code']);
			    $query = "select rp.rp_fname, u.mail from workshop_info.resource_person rp, CDEEP.users u where FIND_IN_SET('".$tem[0]."', rp.states) > 0 and rp.user_uid=u.uid";
			    db_set_active('workshop_info');
			    $result = db_query($query);
			    $mailto = '';
			    while($row = db_fetch_object($result)){
				    $mailto .= $row->mail.',';
			    }
			    db_set_active('default');
			    //$mailfrom = variable_get('site_mail');
			    $subject = 'Invigilator Request';
			    $message = "<p>Dear Resource-person,</p><p>".$user->name." made a new Invigilator request from ".academic_center_name($form_state['values']['academic_code']).". To accept his request click <a href='http://spoken-tutorial.org/workshops/pending/list_pending_invigilators'>here<a></p>";
			    //echo $mailto."<br><br>".$subject."<br><br>".$message; die;
				workshops_notify($mailto, $subject, $message);
				drupal_set_message('Invigilator Details Added Successfully');
				//drupal_set_message('<p>Please note that Invigilator has to confirm acceptance to invigilate by clicking on <b>"Assessment Test Invigilator Confirmation"</b>.</p>');
			}else {
			    db_set_active('default');
				drupal_set_message('Error Adding Invigilator Details');
			}
		}
	}
	function workshops_edit_invigilator($id = NULL) {
		if($id) {
			return drupal_get_form('workshops_edit_invigilator_form', $id);
		}else {
			return "<center>Access Denied!</center>";
		}
	}
	function workshops_edit_invigilator_form($form_state, $id){
	
		$states_array = get_states(0);
		$query = "select inv.academic_code, inv.invigilator_name, inv.address, inv.city, inv.phone, ac.state_code, ac.institution_name, ac.street acstreet, ac.city as accity, ac.pincode as acpincode from invigilator inv, academic_center ac where inv.id=".$id." and ac.academic_code=inv.academic_code";
		db_set_active('workshop_info');
		$result = db_query($query);
		$row = db_fetch_object($result);
		db_set_active('default');
		$output = '';
		if($row) {
			$output = '<table>';
			$output .= '<tr><td><b>Academic Code</b></td><td>'.$row->academic_code.'</td></tr>';
			$output .= '<tr><td><b>Institution Name</b></td><td>'.$row->institution_name.'</td></tr>';
			$output .= '<tr><td><b>Address</b></td><td><p>'.$row->acstreet.'<br />'.$row->accity.'<br />'.$states_array[$row->state_code].' - '.$row->acpincode.'</p></td></tr>';
			$output .= '</table>';
		}
	
		$form['#prefix'] = '<div class="workshop-form">'.$output;
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		
		$form['invigilator_name'] = array(
			'#type' => 'textfield',
			'#title' => t('Invigilator Name'),
			'#required' => TRUE,
			'#value' => $row->invigilator_name,
			'#attributes' => array('class' => 'invigilator_name')
		);
		$form['permanemt_address'] = array(
			'#type' => 'textarea',
			'#title' => t('Permanent Address'),
			'#required' => TRUE,
			'#value' => $row->address,
			'#attributes' => array('class' => 'permanemt_address')
		);
		$form['city'] = array(
			'#type' => 'textfield',
			'#title' => t('City'),
			'#required' => TRUE,
			'#value' => $row->city,
			'#attributes' => array('class' => 'city')
		);
		$form['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone'),
			'#required' => TRUE,
			'#value' => $row->phone,
			'#attributes' => array('class' => 'phone')
		);
		$form['id'] = array(
			'#type' => 'hidden',
			'#value' => $id
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshops_edit_invigilator_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
			form_set_error('phone', t('Invalid Phone number'));
		}
		
  		return;

	}
	function workshops_edit_invigilator_form_submit($form, &$form_state){
		global $user;
		
		$query = "update invigilator set invigilator_name='".$_POST['invigilator_name']."', address='".$_POST['permanemt_address']."', city='".$_POST['city']."', phone='".$_POST['phone']."' where id=".$_POST['id'];
		db_set_active('workshop_info');
		if(db_query($query)){
			drupal_set_message('Invigilator Details Updated Successfully');
			drupal_goto('workshops/members/list_invigilators');
		}else {
			drupal_set_message('Error Updating Invigilator Details');
		}
		db_set_active('default');
	}
	
	function accept_invigilator($id = NULL){
		if($id){
			//echo "id: ".$id; die;
			$query = "select o.invigilator_name, o.invigilator_id, ac.institution_name from invigilator o, academic_center ac where o.id=".$id." and ac.academic_code=o.academic_code";
			//echo $query; die;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$query = "SELECT rid FROM `role` WHERE name = 'invigilator'";
			$result = db_query($query);
			$role_id = db_fetch_object($result);
			// insert into role_user
			$query = "INSERT INTO users_roles (uid, rid) VALUES ('".$row->invigilator_id."', '".$role_id->rid."')";	
			db_query($query);
			$query = "select mail from users where uid=".$row->invigilator_id;
			$result = db_query($query);
			$invigilator_email = db_fetch_object($result);
			// change organiser status to active
			$query = "update invigilator set status=1 where id=".$id;
			// echo $query; die;
			db_set_active('workshop_info');
			if(db_query($query)){
				db_set_active('default');
				// mail to Organiser
				// $mailfrom = variable_get('site_mail');
				$mailto = $invigilator_email->mail;
				$subject = 'Account notification';
				$message = "<p>Dear ".$row->invigilator_name.",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>. You are added as an invigilator for '".$row->institution_name."'.";

				// echo $message; die;
				// echo $mailto."<br><br>".$subject."<br><br>".$message; die;
				workshops_notify($mailto, $subject, $message);
				global $base_url;
				drupal_set_message('Invigilator is added successfully.');
				drupal_goto($path = $base_url.'/workshops/college/list_pending_invigilators', $query = NULL, $fragment = NULL, $http_response_code = 302);
			}
			db_set_active('default');
		}else{
			drupal_set_message('Invalid request!');
			drupal_goto($path = $base_url.'/workshops/college/list_pending_invigilators', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}
	}
	
	function workshops_view_invigilator($id = NULL) {
		if($id) {
			$query = "select inv.academic_code, inv.invigilator_name, inv.address, inv.city, inv.phone, inv.invigilator_id, ac.state_code, ac.institution_name, ac.street acstreet, ac.city as accity, ac.pincode as acpincode from invigilator inv, academic_center ac where inv.id=".$id." and ac.academic_code=inv.academic_code";
			db_set_active('workshop_info');
			$result = db_query($query);
			$row = db_fetch_object($result);
			db_set_active('default');
			$states_array = get_states(0);
	 		if($row){
				$query1 = "select mail from users where uid=".$row->invigilator_id;
				$res = db_query($query1);
				$value = db_fetch_object($res);
	 			$output = '<table>';
				$output .= '<tr><td><b>Academic Code</b></td><td>'.$row->academic_code.'</td></tr>';
				$output .= '<tr><td><b>Institution Name</b></td><td>'.$row->institution_name.'</td></tr>';
				$output .= '<tr><td><b>Location</b></td><td><p>'.$row->accity.'<br />'.$states_array[$row->state_code].' - '.$row->acpincode.'</p></td></tr>';
				$output .= '<tr><td><b>Invigilator Name</b></td><td>'.$row->invigilator_name.'</td></tr>';
				$output .= '<tr><td><b>Permanent Address</b></td><td>'.str_replace("\n", "<br />", $row->address).'</td></tr>';
				$output .= '<tr><td><b>Phone number</b></td><td>'.$row->phone.'</td></tr>';
				$output .= '<tr><td><b>Email ID</b></td><td>'.$value->mail.'</td></tr>';
				$output .= '</table>';

	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
		}else {
			return "<center>Something went wrong, please try again later.</center>";
		}
	}
	
	function reject_invigilator($id = NULL){
		if($id){
			$query = "select u.mail, inv.invigilator_name from workshop_info.invigilator inv, CDEEP.users u where u.uid=inv.invigilator_id and inv.id=".$id;
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $invigilator_email->mail;
			$subject = 'Registration reject notification';
			$message = "<p>Dear ".$row->invigilator_name.",</p><p> Your Invigilator request has been rejected by Resource Person</p>";
			workshops_notify($mailto, $subject, $message);

			$query = "delete from invigilator where id=".$id;
			db_set_active('workshop_info');
			if(db_query($query)){
				db_set_active('default');
				// mail to Organiser
				$query = "select mail from users where uid=".$row->invigilator_id;
			    $result = db_query($query);
			    $invigilator_email = db_fetch_object($result);
				$mailto = $invigilator_email->mail;
				$subject = 'Invigilator Request Reject Notification';
				$message = "<p>Dear ".$row->invigilator_name.",</p><p>Your invigilator request has been rejected by Resource persons.</p>";
				//echo $mailto."<br><br>".$subject."<br><br>".$message; die;
				workshops_notify($mailto, $subject, $message);
				
				drupal_set_message("Invigilator request has been removed from list.");
				drupal_goto($path = $base_url.'/workshops/college/list_pending_invigilators', $query = NULL, $fragment = NULL, $http_response_code = 302);
			}
			db_set_active('default');
		}else{
			drupal_set_message('Invalid request!');
			drupal_goto($path = $base_url.'/workshops/college/list_pending_invigilators', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}
	}
	function workshops_list_invigilator_requests() {
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
			$tmp_query_param .= "o.academic_code like '".$st_code."%'";
		}
		//$query = "select id, academic_code, invigilator_name, city, phone from invigilator where (".$tmp_query_param.") and status=0";
		$query = "select o.id, o.academic_code, ac.institution_name, o.invigilator_name, o.city, o.phone from invigilator o,academic_center ac where o.academic_code = ac.academic_code and (".$tmp_query_param.") and status=0";
		$count_query = "select count(*) from organiser where (".$tmp_query_param.") and status=0";
		// $header = array('', 'Academic Center Code', 'Name', 'City', 'Phone', 'Action');
		$header = array(
			//array('data'=>'Academic Center Code', 'field'=>'academic_code', 'sort'=>'asc'),
			array('data'=>'Institution Name', 'field'=>'ac.institution_name', 'sort'=>'asc'),
			array('data'=>'Name', 'field'=>'invigilator_name'),
			array('data'=>'City', 'field'=>'city'),
			array('data'=>'Phone', 'field'=>'phone'),
			array('data'=>'Action')
		);

		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query, 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->institution_name, $item->invigilator_name, $item->city, $item->phone, l(t('Accept'), 'workshops/accept_invigilator/'. $item->id , array('attributes' => array('class' => 'confirm-record', 'title' => 'Accept')))." ".l(t('Reject'), 'workshops/reject_invigilator/'. $item->id , array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
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
	
	function workshops_invigilate_test_request(){
		global $user;
		
		$query = "";
		$count_query = "";
		drupal_set_message('<p>Invigilator has to fill the attendance of the participants<br> on the day of the test by clicking on <b>"Fill / Edit participants list"</b>.');
		$query = "select tr.id, tr.test_code, tr.academic_code, tr.department, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category from test_requests tr where invigilator_id=".$user->uid." and status=1";
		$count_query = "select count(id) from test_requests where invigilator_id=".$user->uid." and status=1";
		
		// $header = array('Test Code', 'Academic Code', 'Department', 'FOSS Category', 'Date & Time', '');
		$header = array(
			array('data'=>'Test Code', 'field'=>'tr.id', 'sort'=>'asc'),
			array('data'=>'Academic Code', 'field'=>'tr.academic_code'),
			array('data'=>'Department', 'field'=>'tr.department'),
			array('data'=>'FOSS Category', 'field'=>'tr.foss_category'),
			array('data'=>'Date & Time', 'field'=>'tr.cfm_test_date'),
			array('data'=>'')
		);

		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');

		$row = array();
		$sno = 1;
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->test_code, $item->academic_code, $item->department, $item->foss_category, $item->cfm_test_date." ".$item->cfm_test_time, l('Confirm', 'workshops/invigilate_test_confirm/'. $item->id, array('attributes' => array('class' => 'confirm-record', 'title' => 'Confirm'))).l('Reject', 'workshops/test_delete/'. $item->id, array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
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
	
	function invigilate_test_confirm($id = NULL) {
		if($id){
			$query = "update test_requests set status=2 where id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			if($result) {
				drupal_set_message('Test request confirmed.');
				
				//email to organiser
				$query = "select test_code, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category, tr.organiser_id, tr.invigilator_id from academic_center ac, test_requests tr where tr.academic_code=ac.academic_code and tr.id=".$id;
		        db_set_active('workshop_info');
		
		        db_set_active('default');
			    drupal_set_message('Test Confirm Details Added Successfully');
			    $data = db_fetch_object($result);

			    // send email to Organiser
			    $query = "select name, mail from users where uid='".$data->organiser_id."'";
			    $result = db_query($query);
			    $row = db_fetch_object($result);
			    $mailto = $row->mail;
			    //$mailfrom = variable_get('site_mail');
			    $subject = 'Test confirmation notification';
			    $message = "<p>Dear ".$row->name.",</p><p> Your new Test request has been approved</p>";
			    $message .= "<table><tr><td>Institution Name</td><td>: ".$data->institution_name."</td></tr>";
			    $message .= "<tr><td>Department</td><td>: ".$data->department."</td></tr>";
			    $message .= "<tr><td>Foss Category </td><td>: ".$data->foss_category ."</td></tr>";
			    $message .= "<tr><td>Test Code </td><td>: ".$data->test_code ."</td></tr>";
			    $message .= "<tr><td>Test date & time</td><td>: ".$data->cfm_test_date."  ".$data->cfm_test_time."</td></tr></table>";
			    $message .=  "Please go through the Test Instructions for the participants so that you can guide them properly on the day of the test";
			    //echo $mailto."<br><br>".$subject."<br><br>".$message;
			    workshops_notify( $mailto, $subject, $message);

			}
			drupal_goto('workshops/invigilate_pending_test_requests');
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}
	function workshops_invigilate_test_attendance(){
		global $user;
		
		$query = "";
		$count_query = "";
		
		$query = "select tr.id, tr.test_code,"./*" tr.department,".*/" tr.cfm_test_date, tr.cfm_test_time, tr.foss_category, tr.status, ac.institution_name from test_requests tr, academic_center ac where tr.invigilator_id=".$user->uid." and tr.status>1 and tr.status<4 and tr.cfm_test_date='".date('Y-m-d')."' and ac.academic_code=tr.academic_code";
		$count_query = "select count(id) from test_requests tr where tr.invigilator_id=".$user->uid." and tr.status>1 and tr.status<4 and tr.cfm_test_date='".date('Y-m-d')."' and ac.academic_code=tr.academic_code";
		
		// $header = array('Test Code', 'Academic Code', 'Department', 'FOSS Category', 'Date & Time', '');
		$header = array(
			array('data'=>'Test Code', 'field'=>'tr.id', 'sort'=>'asc'),
			array('data'=>'Institution', 'field'=>'tr.institution_name'),
			// array('data'=>'Department', 'field'=>'tr.department'),
			array('data'=>'FOSS Category', 'field'=>'tr.foss_category'),
			array('data'=>'Date & Time', 'field'=>'tr.cfm_test_date'),
			array('data'=>'')
		);

		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');

		$row = array();
		$sno = 1;
		while ($item = db_fetch_object($result)) {
			$caption_string = 'Attendance';
			$caption_description = 'Fill the attendance form';
			if($item->status == 3){
				$caption_string = 'Edit';
				$caption_description = 'Edit the attendance form';
			}
			$row[] = array($item->test_code, $item->institution_name, /*$item->department, */$item->foss_category, $item->cfm_test_date." ".$item->cfm_test_time, l($caption_string, 'workshops/invigilate_fill_test_attendance/'. $item->id, array('attributes' => array('title' => $caption_description))));
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
	function invigilate_fill_test_attendance($id = NULL) {
		if($id){
			$path = drupal_get_path('module', 'workshops');
			drupal_add_js($path . '/workshops.js');
			$query = "select test_code, academic_code, department, foss_category from test_requests where id='". $id."' and status!=4 and status!=1 and status!=0";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$output = '';
			if($row = db_fetch_object($result)){
				$output = '<form method="post" action="'.base_path().'workshops/save_attendance">
								<table border="0" cellpadding ="30" cellspacing="30" align="left" style="width: 100%; margin-bottom: 20px; margin-top: 10px; margin-left: 20px;">';
				$output .= '<tr><td><b>Test code</b> &nbsp;&nbsp;&nbsp;&nbsp; <b>:</b> </td><td>'.$row->test_code.'</td><td><b>Academic code</b>&nbsp;<b>:</b> </td><td>'.$row->academic_code.'</td></tr>';
				$output .= '<tr><td><b>FOSS category</b>&nbsp;<b>:</b> </td><td>'.$row->foss_category.'</td></tr>';
				$output .= '</table><br><br>';
				
				$query = "select ar.id, ar.status, mu.id, mu.username, mu.picture, mu.firstname, mu.lastname, mu.institution from workshop_info.attendance_register ar, OTC.mdl_user mu where ar.test_code='".$row->test_code."' and mu.id=ar.moodle_uid";
				$count_query = "select count(ar.id) from workshop_info.attendance_register ar, OTC.mdl_user mu where ar.test_code='".$row->test_code."' and mu.id=ar.moodle_uid";
				$header = array('<input type="checkbox" name="selectall" id="selectall" class="select-all">', 'Username', 'Name', 'Institution');
				db_set_active('workshop_info');
				$result = pager_query($query , 30000, 0, $count_query);
				db_set_active('default');
				$rows = array();
				$count = 0;
				while ($item = db_fetch_object($result)) {
					$count++;
					$checked_value = '';
					if($item->status == 1){
						$checked_value = 'checked';
					}
					if($item->status == 2){
						$rows[] = array('<input type="checkbox" name="user'.$count.'" value="'.$item->id.'" checked disabled=disabled>', '<iframe style="width:55px; height:55px;" src="http://onlinetest.spoken-tutorial.org/get_profile_picture.php?id='.$item->id.'"></iframe>', $item->username, $item->firstname.' '.$item->lastname, $item->institution);
					}else{
						$rows[] = array('<input type="checkbox" name="user'.$count.'" value="'.$item->id.'" '.$checked_value.'>', '<iframe style="width:55px; height:55px;" src="http://onlinetest.spoken-tutorial.org/get_profile_picture.php?id='.$item->id.'"></iframe>', $item->username, $item->firstname.' '.$item->lastname, $item->institution);
					}
				}
				if ($count == 0) {
					$output .= '<div style="color:red;text-align:center;">List is empty.</div></form>';
				} else {
					drupal_set_message($message);
					$output .= theme('table', $header, $rows);
					$output .= theme('pager');
					$output .= '<input type="hidden" name="count" value="'.$count.'"><input type="hidden" name="trid" value="'.$id.'"><input type="hidden" name="tc" value="'.$row->test_code.'"><input type="submit" value="Submit" /></form>';
				}
				
				return $output;
			}else{
				return 'Something went wrong, please try again later.';
			}
			// drupal_goto('workshops/invigilate_pending_test_requests');
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}
	
	function invigilate_save_attendance(){
		if(isset($_POST['trid']) && isset($_POST['tc']) && isset($_POST['count']) && $_POST['count'] != 0){
			$query = "update attendance_register set status=0 where test_code='".$_POST['tc']."' and status != 2";
			db_set_active('workshop_info');
			db_query($query);
			db_set_active('default');
			$flag = 1;
			for($i = 1; $i <= $_POST['count']; $i++) {
				if(isset($_POST['user'.$i])){
					$query = "update attendance_register set status=1 where status != 2 and moodle_uid=".$_POST['user'.$i];
					db_set_active('workshop_info');
					db_query($query);
					db_set_active('default');
					$flag = 0;
				}
			}
			if($flag){
				drupal_set_message('Please tick the students who are all attending the test, before pressing submit button');
				drupal_goto('workshops/invigilate_fill_test_attendance/'.$_POST['trid']);
			}else{
				$query = "update test_requests set status=3 where id=".$_POST['trid'];
				db_set_active('workshop_info');
				db_query($query);
				db_set_active('default');
				drupal_set_message('Attendance details saved successfully. Now students can proceed with the online test.');
				drupal_goto('workshops/invigilate_attendance_test_request');
			}
		}
	}
	
	function workshops_invigilate_ongoing_tests(){
		global $user;

                $query = "";
                $count_query = "";
                if(in_array('resource_person', array_values($user->roles))) {
                    $query = "select tr.id, tr.test_code, tr.academic_code, tr.department, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category from test_requests tr where tr.status=3 and tr.cfm_test_date<='".date('Y-m-d')."'";
                $count_query = "select count(id) from test_requests where status=3 and tr.cfm_test_date<='".date('Y-m-d')."'";
                }elseif(in_array('organiser', array_values($user->roles))){
                    $query = "select tr.id, tr.test_code, tr.academic_code, tr.department, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category from test_requests tr where tr.organiser_id=".$user->uid." and tr.status=3 and tr.cfm_test_date<='".date('Y-m-d')."'";
                $count_query = "select count(id) from test_requests where invigilator_id=".$user->uid." and status=3 and tr.cfm_test_date<='".date('Y-m-d')."'";
                }
		if(in_array('invigilator', array_values($user->roles))){
                    $query = "select tr.id, tr.test_code, tr.academic_code, tr.department, tr.cfm_test_date, tr.cfm_test_time, tr.foss_category from test_requests tr where tr.invigilator_id=".$user->uid." and tr.status=3 and tr.cfm_test_date<='".date('Y-m-d')."'";
                $count_query = "select count(id) from test_requests where invigilator_id=".$user->uid." and status=3 and tr.cfm_test_date<='".date('Y-m-d')."'";
                }
                // $header = array('Test Code', 'Academic Code', 'Department', 'FOSS Category', 'Date & Time', '');
                $header = array(
                        array('data'=>'Test Code', 'field'=>'tr.id', 'sort'=>'asc'),
                        array('data'=>'Academic Code', 'field'=>'tr.academic_code'),
                        array('data'=>'Department', 'field'=>'tr.department'),
                        array('data'=>'FOSS Category', 'field'=>'tr.foss_category'),
                        array('data'=>'Date & Time', 'field'=>'tr.cfm_test_date'),
                );

                $query .= tablesort_sql($header);
                db_set_active('workshop_info');
                $result = pager_query($query , 30, 0, $count_query);
                db_set_active('default');

                $row = array();
                $sno = 1;
                while ($item = db_fetch_object($result)) {
                    if(in_array('invigilator', array_values($user->roles)))
                        $row[] = array($item->test_code, $item->academic_code, $item->department, $item->foss_category, $item->cfm_test_date." ".$item->cfm_test_time, l('Close', 'workshops/invigilate_close_test/'. $item->id, array('attributes' => array('title' => 'Close the online test'))));
                    else
                        $row[] = array($item->test_code, $item->academic_code, $item->department, $item->foss_category, $item->cfm_test_date." ".$item->cfm_test_time, '');
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
	
	function invigilate_close_test($id = NULL){
		global $user;
		if($id){
			$query = "select tr.test_code, count(ar.id) as pcount, inv.invigilator_name from test_requests tr, attendance_register ar, invigilator inv where tr.id=".$id." and ar.test_code=tr.test_code and ar.status=2 and inv.invigilator_id=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			if($row = db_fetch_object($result)){
				$query = "insert into test_details (test_code, no_of_participants, invigilator) values('".$row->test_code."', ".$row->pcount.", '".$row->invigilator_name."')";
				db_set_active('workshop_info');
				db_query($query);
				db_set_active('default');
				$query = "update test_requests set status=4 where id=".$id;
				db_set_active('workshop_info');
				db_query($query);
				db_set_active('default');
				drupal_set_message('Online test has been closed successfully. Now participants can get their certificates.');
			}
			drupal_goto('workshops/invigilate_ongoing_tests');
		}else{
			drupal_goto('workshops/invigilate_ongoing_tests');
		}
	}
?>
