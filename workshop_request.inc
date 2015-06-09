<?php
	require_once('common.inc');
	function workshops_workshop_request(){
  $path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-pref-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		$output = '<div class="form-item" id="academic-details"></div>';
		drupal_set_message('<p>Please check if your machine is ready.<br>For the Machine Readiness document <a href="http://process.spoken-tutorial.org/images/5/58/Machine-Readiness.pdf" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
                drupal_set_message('<p>Please fill the <span style="font-size:18px;font-weight:bold;">Workshop Details</span> after the workshop.<br>For the instructions <a href="http://process.spoken-tutorial.org/images/e/eb/Workshop_Details.pdf" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
                drupal_set_message('<p>You will receive a mail that the workshop has been approved.</p>');

		$output .= drupal_get_form('workshops_workshop_request_form');
		return $output;
	}
	function workshops_workshop_request_form(){
			
		$hour = array('' => 'Hour');
		$minite = array('' => 'Minute');
		$foss_category = array('' => '-- Select --');
		$standard = array("5"=>"Fifth Standard","6"=>"Sixth Standard","7"=>"Seventh Standard","8"=>"Eighth Standard","9"=>"Ninth Standard","10"=>"Tenth Standard","11"=>"Eleventh Standard","12"=>"Twelfth Standard");
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}

		$query = "select name from foss_categories where workshop=1 order by name";
		$foss_categories = db_query($query);

		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->name] = $row->name;
		}

		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		drupal_add_css($path . '/workshops.css', 'module');
		$query = "select distinct name from departments order by name ASC";
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		$departments = array();
		$departments[''] = '-- select --';
		while($row = db_fetch_object($result)){
			$departments[$row->name] = $row->name;
		}
		$departments['others'] = '...others...';

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
		$form['department'] = array(
			'#type' => 'select',
			'#title' => t('Department of Participants'),
			'#required' => TRUE,
			'#options' => $departments,
			'#attributes' => array('class' => 'add_reqs_department'),
		);
		$form['standard'] = array(
			'#type' => 'select',
			'#title' => t('School Standard'),
			'#options' => $standard,
			'#attributes' => array('class' => 'add_school_standard'),
		);
		$form['pref_wkshop_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Preferred Workshop Date'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'pref_wkshop_date', 'readonly' => 'readonly')
		);
		$form['pref_wkshop_time'] = array(
			'#type' => 'select',
			'#options' => $hour,
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-wkshop-time'>Preferred Workshop Time: <span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['pref_wkshop_minute'] = array(
			'#type' => 'select',
			'#options' => $minite,
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['foss_category'] = array(
			'#type' => 'select',
			'#title' => t('Foss Category'),
			'#options' => $foss_category,
			'#required' => TRUE,
			'#attributes' => array('class' => 'foss_category'),
			'#prefix' => '<div class="ws-foss-lang"><div class="ws-foss">',
			'#suffix' => '</div>'
		);
		$form['pref_language'] = array(
			'#type' => 'select',
			'#title' => t('Preferred Language'),
			'#options' => array('' => '-- Select --'),
			'#required' => TRUE,
			'#validated' => TRUE,
			'#attributes' => array('class' => 'pref_language'),
			'#prefix' => '<div class="ws-lang">',
			'#suffix' => '</div></div>'
		);
		$form['skype_request'] = array(
			'#type' => 'radios',
			'#title' => t('Skype Support'),
			'#options' => array(1 => 'Yes', 0 => 'No'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'skype_request'),
			'#prefix' => '<div class="skype-request">',
			'#suffix' => '</div>'
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshops_workshop_request_form_validate($form, &$form_state){
		if($form_state['values']['academic_code'] == ""){
        	form_set_error('academic_code', t('Please select Institution Name'));
        }
		if (!preg_match('/^[a-zA-Z ]+$/', $form_state['values']['department'])){
			form_set_error('department', t('Invalid Department'));
		}
		if($form_state['values']['pref_language'] == ""){
                        form_set_error('pref_language', t('Please select language'));
        }
        if($form_state['values']['pref_wkshop_time'] == ""){
        	form_set_error('pref_wkshop_time', t('Please select Hour'));
        }
        if($form_state['values']['pref_wkshop_minute'] == ""){
        	form_set_error('pref_wkshop_minute', t('Please select Minute'));
        }

	  return;
	}
	function workshops_workshop_request_form_submit($form, &$form_state){
		global $user;
		global $base_url;
		$pref_wkshop_date = $form_state['values']['pref_wkshop_date'];
		$pref_wkshop_time = $form_state['values']['pref_wkshop_time'].':'.$form_state['values']['pref_wkshop_minute'];
		if($form_state['values']['department'] == 'School'){
		$query = "insert into  workshop_requests(academic_code, organiser_id, department, pref_wkshop_date, pref_wkshop_time, foss_category, pref_language, skype_request, created_at,school_standard) values('".$form_state['values']['academic_code']."', '".$user->uid."', '".$form_state['values']['department']."', '".$pref_wkshop_date."', '".$pref_wkshop_time."', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."','".$form_state['values']['skype_request']."', '".date('Y-m-d H:i:s')."','".$form_state['values']['standard']."')";
		}
		else{
		$query = "insert into  workshop_requests(academic_code, organiser_id, department, pref_wkshop_date, pref_wkshop_time, foss_category, pref_language, skype_request, created_at,school_standard) values('".$form_state['values']['academic_code']."', '".$user->uid."', '".$form_state['values']['department']."', '".$pref_wkshop_date."', '".$pref_wkshop_time."', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."','".$form_state['values']['skype_request']."', '".date('Y-m-d H:i:s')."',0)";
		}
		//$query = "insert into  workshop_requests(academic_code, organiser_id, department, pref_wkshop_date, pref_wkshop_time, foss_category, pref_language, skype_request, created_at) values('".$form_state['values']['academic_code']."', '".$user->uid."', '".$form_state['values']['department']."', '".$pref_wkshop_date."', '".$pref_wkshop_time."', '".$form_state['values']['foss_category']."', '".$form_state['values']['pref_language']."','".$form_state['values']['skype_request']."', '".date('Y-m-d H:i:s')."')";
		db_set_active('workshop_info');
		if(db_query($query)){
			db_set_active('default');
			//var_dump("tester"); die;
			// mail to Organiser
			//$mailfrom = $custom_email;
			$mailto = $user->mail;
			$subject = 'Workshop Request Notification';
			$message = "<p>Dear ".$user->name.",</p><p>Thank you for registering at <a href='http://spoken-tutorial.org'>spoken-tutorial.org</a>.Your workshop request is waiting for approval. Once it has been approved, you will receive another e-mail containing information about workshop confirmation details.</p>";
			//workshops_notify($mailto, $subject, $message);
			
			
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
			$subject = 'Workshop Request Notification';
			$message = "<p>Dear Resource-person,</p><p>".$user->name." made a new workshop request.</p><p><b>Workshop Details</b></p><p>Foss category : ".$form_state['values']['foss_category']."</p><p>Preferred Workshop Date : ".$form_state['values']['pref_wkshop_date']."</p><p>Preferred Workshop Time : ".$pref_wkshop_time."</p>";
			//workshops_notify($mailto, $subject, $message);
			drupal_set_message('Workshop Request Added Successfully');
			//drupal_set_message('<p>Please check if your machine is ready.<br>For the Machine Readiness document <a href="http://process.spoken-tutorial.org/images/5/58/Machine-Readiness.pdf" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
			//drupal_set_message('<p>Please fill the workshop details after the workshop.<br>For the instructions <a href="http://process.spoken-tutorial.org/images/e/eb/Workshop_Details.pdf" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
			//drupal_set_message('<p>You will receive a mail that the workshop has been approved.</p>');
			drupal_goto($path = 'workshops/list_workshop_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Request Details');
		}
		db_set_active('default');
		drupal_goto($path = 'workshops/list_workshop_request', $query = NULL, $fragment = NULL, $http_response_code = 302);
	}

	function workshop_request_delete($id = NULL){
		if($id){
			$query = "update workshop_requests set status=3 where id=".$id;
			db_set_active('workshop_info');
			db_query($query);
			db_set_active('default');
			
			drupal_set_message('Workshop request removed successfully!');
			drupal_goto($_SERVER['HTTP_REFERER']);
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_list_workshop_request(){
		global $user;
		
		$query = "";
		$count_query = "";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select wr.*, ac.institution_name, o.organiser_name from workshop_requests wr, organiser o, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=0 and wr.organiser_id=o.organiser_id and ac.academic_code=wr.academic_code";
			$count_query = "select count(*) from workshop_requests wr, organiser o, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=0 and wr.organiser_id=o.organiser_id and ac.academic_code=wr.academic_code";
		}

		if(in_array('resource_person', array_values($user->roles))) {
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
			$tmp_query_param .=	"wr.academic_code like '".$st_code."%'";
			}
			$query = "select wr.*, ac.institution_name, o.organiser_name from workshop_requests wr, organiser o, academic_center ac where (".$tmp_query_param.") and wr.status=0 and wr.organiser_id=o.organiser_id and ac.academic_code=wr.academic_code";
		//echo $query."<br /><br />";
			$count_query = "select count(*) from workshop_requests wr, organiser o, academic_center ac where (".$tmp_query_param.") and wr.status=0 and wr.organiser_id=o.organiser_id and ac.academic_code=wr.academic_code";
		//echo $count_query."<br />"; die;
		}
		// echo $query; die;
		if(in_array('organiser', array_values($user->roles))){
			$header = array(
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				//array('data'=>'Academic Center Code', 'field'=>'wr.academic_code'),
				array('data'=>'Institution Name', 'field'=>'ac.institution_name'),
				array('data'=>'Organiser', 'field'=>'o.organiser_name'),
				array('data'=>'Workshop Date', 'field'=>'wr.pref_wkshop_date', 'sort'=>'desc'),
			);
		}
		if(in_array('resource_person', array_values($user->roles))){
			$header = array(
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				//array('data'=>'Academic Center Code', 'field'=>'wr.academic_code'),
				array('data'=>'Institution Name', 'field'=>'ac.institution_name'),
				array('data'=>'Organiser', 'field'=>'o.organiser_name'),
				array('data'=>'Workshop Date', 'field'=>'wr.pref_wkshop_date', 'sort'=>'desc'),
				//array('data'=>'')
			);
		}
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query, 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			// $row_tmp = db_fetch_object($res1);
			if(in_array('resource_person', array_values($user->roles))){
				$row[] = array(human_text($item->foss_category), $item->institution_name, $item->organiser_name, $item->pref_wkshop_date, l(t('Confirm'), 'workshops/workshop_confirm/'. $item->id, array('attributes' => array('class' => 'confirm-record', 'title' => 'Confirm'))).l('Reject', 'workshops/workshop_delete/'. $item->id, array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
			}else {
				$row[] = array(human_text($item->foss_category), $item->academic_code, $item->organiser_name, $item->pref_wkshop_date);
			}
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
function workshop_filter_form($form_state, $selected_val) {
		global $user;
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		drupal_add_css($path . '/workshops.css', 'module');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js("$(document).ready(function(){ new datepickr('edit-from-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		drupal_add_js("$(document).ready(function(){ new datepickr('edit-to-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$fquery = "select name from foss_categories where workshop=1 order by name ASC";
		$foss_categories = db_query($fquery);
		$lquery = "select pref_language from workshop_requests where status=2 group by pref_language";
		$squery = "select states from resource_person where user_uid='" . $user->uid."'";
		db_set_active('workshop_info');
		$lresult = db_query($lquery);
		$sresult = db_query($squery);
		db_set_active('default');
		$foss_category[''] = 'All';
		while($row = db_fetch_object($foss_categories)) {
			$foss_category[$row->name] = human_text($row->name);
		}
		
		$languages[''] = 'All';
		while($row = db_fetch_object($lresult)) {
			$languages[$row->pref_language] = human_text($row->pref_language);
		}
		/* select states */
		$states_array = get_states(0);
		# get current resource person name
		
		$row = db_fetch_object($sresult);
		$state_codes = explode(",", $row->states);
	
		$states = array();
		$states[''] = "All";
		foreach($state_codes as $st_code){
			$states[$st_code] = $states_array[$st_code];
		}
		
		/* check collapse */
		$attr = 1;
		if(count($selected_val) > 0)
			$attr = 0;
		$form['filter'] = array(
		 	'#type' => 'fieldset',
		    '#title' => t('Filter'),
		    '#weight' => 5,
		    '#collapsible' => FALSE,
		    '#collapsed' => $attr,
		);
		$form['filter']['foss'] = array(
			'#type' => 'select',
			'#title' => t('FOSS Category'),
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => $foss_category,
			'#default_value' => $selected_val['foss'],

		);
		$form['filter']['language'] = array(
			'#type' => 'select',
			'#title' => t('Language'),
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => $languages,
			'#default_value' => $selected_val['language'],

		);
		$form['filter']['state'] = array(
			'#type' => 'select',
			'#title' => t('State'),
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => $states,
			'#default_value' => $selected_val['state'],

		);
		$form['filter']['type'] = array(
			'#type' => 'select',
			'#title' => t('Type'),
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => array("school" => "School","college" => "College",'polytechnic' => "Polytechnic","iti" => "ITI"),
			'#default_value' => $selected_val['type'],

		);
		
		if($selected_val['from_date'])
			$attr = array('class' => 'from_date clear-data');
		else
			$attr = array('class' => 'from_date clear-data', 'readonly' => 'readonly');
		
		$form['filter']['from_date'] = array(
			'#type' => 'textfield',
			'#title' => t('From'),
			'#attributes' => $attr,
			'#default_value' => $selected_val['from_date'],
		);
		
		if($selected_val['to_date'])
			$attr = array('class' => 'to_date clear-data');
		else
			$attr = array('class' => 'to_date clear-data', 'readonly' => 'readonly');
		
		$form['filter']['to_date'] = array(
			'#type' => 'textfield',
			'#title' => t('To'),
			'#attributes' => $attr,
			'#default_value' => $selected_val['to_date'],
		);
		$form['filter']['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
	
		return $form;
	}
	
	function workshop_filter_form_validate($form, &$form_state){
		if ($form_state['values']['from_date'] != '' && $form_state['values']['to_date'] != '' && ($form_state['values']['from_date'] > $form_state['values']['to_date']))
			form_set_error('academic_code', t('To date should not less that from date '.$form_state['values']['from_date']));
  		return;
	}
	function workshop_filter_form_submit($form, &$form_state){
	
		// filter
		$params = array('foss' => $form_state['values']['foss'], 'language' => $form_state['values']['language'],  'state' => $form_state['values']['state'],'type' => $form_state['values']['type'], 'from_date' => $form_state['values']['from_date'], 'to_date' => $form_state['values']['to_date']);
		$filter =  array('filter' => base64_encode(serialize($params)));
		$redirect = $_REQUEST['q'];
		drupal_goto($path = $redirect, $query = $filter, $fragment = NULL, $http_response_code = 302);
	}
	
 /*	function workshops_list_workshop_approval(){
		global $user;
		
		$query = "";
		$count_query = "";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select wr.*, o.organiser_name from workshop_requests wr, organiser o where wr.organiser_id=".$user->uid." and wr.status=1 and wr.organiser_id=o.organiser_id";
			$count_query = "select count(*) from workshop_requests wr, organiser o where wr.organiser_id=".$user->uid." and wr.status=1 and wr.organiser_id=o.organiser_id";
		}

		if(in_array('resource_person', array_values($user->roles))) {
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
				$tmp_query_param .=	"wr.academic_code like '".$st_code."%'";
			}
			$query = "select wr.*, o.organiser_name from workshop_requests wr, organiser o where (".$tmp_query_param.") and wr.status=1 and wr.organiser_id=o.organiser_id";
			//echo $query."<br /><br />";
			$count_query = "select count(*) from workshop_requests wr, organiser o where (".$tmp_query_param.") and wr.status=1 and wr.organiser_id=o.organiser_id";
			//echo $count_query."<br />"; die;
		}
		if(in_array('organiser', array_values($user->roles))){
			$header = array(
				array('data'=>'Workshop Code', 'field'=>'wr.workshop_code'),
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				array('data'=>'Academic Center Code', 'field'=>'wr.academic_code'),
				array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'desc'),
				// array('data'=>'Department', 'field'=>'wr.department'),
				array('data'=>'Organiser', 'field'=>'o.organiser_name')
			);
		}
		if(in_array('resource_person', array_values($user->roles))){
			$header = array(
				array('data'=>'Workshop Code', 'field'=>'wr.workshop_code'),
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				array('data'=>'Academic Center Code', 'field'=>'wr.academic_code'),
				array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'desc'),
				// array('data'=>'Department', 'field'=>'wr.department'),
				array('data'=>'Organiser', 'field'=>'o.organiser_name')
			);
		}
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			if(in_array('resource_person', array_values($user->roles))){
				$row[] = array($item->workshop_code, human_text($item->foss_category), $item->academic_code, $item->cfm_wkshop_date.' '.$item->cfm_wkshop_time, /*$item->department,*/ /*$item->organiser_name, l('Edit', 'workshops/workshop_edit/'. $item->id, array('attributes' => array('class' => 'edit-record', 'title' => 'Edit'))).' '.l('Reject', 'workshops/workshop_delete/'. $item->id, array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
			}else {
				$row[] = array($item->workshop_code, human_text($item->foss_category), $item->academic_code, $item->cfm_wkshop_date.' '.$item->cfm_wkshop_time, /*$item->department,*/ /*$item->organiser_name);
			}
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}
		return $output;
	}*/


function workshops_list_workshop_approval(){
		global $user;
		$filter = unserialize(base64_decode($_REQUEST['filter']));
		$filter_query = '';
		
		$selected_val = array();
		
		if($filter['foss'] != ''){
			$filter_query .= " and wr.foss_category = '".$filter['foss']."'";
			$selected_val['foss'] = $filter['foss'];
		}
		if($filter['state'] != ''){
			$filter_query .= " and wr.academic_code like '".$filter['state']."%'";
			$selected_val['state'] = $filter['state'];
		}
		if($filter['language'] != ''){
			$filter_query .= " and wr.pref_language = '".$filter['language']."'";
			$selected_val['language'] = $filter['language'];
		}
		if($filter['type'] != ''){
			if($filter['type'] == 'school'){
				$filter_query .= " and ac.school_college = 0";
				$selected_val['type'] = $filter['type'];
			}
			else if($filter['type'] == 'college'){
				$filter_query .= " and ac.school_college = 1 and ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE 'iti%'";
				//$filter_query .= " and ac.school_college = 1";
				$selected_val['type'] = $filter['type'];
			}
			else if($filter['type'] == 'polytechnic'){
				$filter_query .= " and ac.school_college = 1 and ac.institution_name LIKE '%".$filter['type']."%'";
				$selected_val['type'] = $filter['type'];
			}
			else if($filter['type'] == 'iti'){
				$filter_query .= " and ac.school_college = 1 and ac.institution_name LIKE '".$filter['type']."%'";
				$selected_val['type'] = $filter['type'];
			}
			else{
				//$filter_query .= " and ac.institution_name like '%".$filter['type']."%'";
				$selected_val['type'] = $filter['type'];
			}
		}
		if($filter['from_date'] != ''){
			$filter_query .= " and wr.cfm_wkshop_date >= '".$filter['from_date']."'";
			$selected_val['from_date'] = $filter['from_date'];
		}
		if($filter['to_date'] != ''){
			$filter_query .= " and wr.cfm_wkshop_date <= '".$filter['to_date']."'";
			$selected_val['to_date'] = $filter['to_date'];
		}
		
		$query = "";
		$count_query = "";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select wr.*, o.organiser_name, ac.institution_name from workshop_requests wr, organiser o, academic_center ac where ac.academic_code = wr.academic_code and wr.organiser_id=".$user->uid." and wr.status=1 and wr.organiser_id=o.organiser_id ".$filter_query;
			$count_query = "select count(*) from workshop_requests wr, organiser o where wr.organiser_id=".$user->uid." and wr.status=1 and wr.organiser_id=o.organiser_id ".$filter_query;
		}
		
		if(in_array('resource_person', array_values($user->roles))) {
			$query = "select states from resource_person where user_uid=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$state_code_arr = explode(',', $row->states);
		    
			$tmp_query_param = "";
			if(!isset($selected_val['state'])){
				foreach($state_code_arr as $st_code){
					if($tmp_query_param != ""){
						$tmp_query_param .= " or ";
					}
					$tmp_query_param .=	"wr.academic_code like '".$st_code."%'";
				}
				if($tmp_query_param){
					$tmp_query_param = ' and ('.$tmp_query_param.')';
				}
			}
			$query = "select wr.*, o.organiser_name,ac.institution_name from workshop_requests wr, organiser o, academic_center ac where ac.academic_code = wr.academic_code and wr.status=1 ".$tmp_query_param.$filter_query." and wr.organiser_id=o.organiser_id";
			$count_query = "select count(*) as count from workshop_requests wr, organiser o, academic_center ac where wr.status=1 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code and wr.organiser_id=o.organiser_id";
		}
		if(in_array('organiser', array_values($user->roles))){
			$header = array(
				array('data'=>'Workshop Code', 'field'=>'wr.workshop_code'),
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				//array('data'=>'Academic Center Code', 'field'=>'wr.academic_code'),
				array('data'=>'Institution Name', 'field'=>'ac.institution_name', 'sort'=>'asc'),
				array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'asc'),
				// array('data'=>'Department', 'field'=>'wr.department'),
				array('data'=>'Organiser', 'field'=>'o.organiser_name')
			);
		}
		if(in_array('resource_person', array_values($user->roles))){
			$header = array(
				array('data'=>'Workshop Code', 'field'=>'wr.workshop_code'),
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				//array('data'=>'Academic Center Code', 'field'=>'wr.academic_code'),
				array('data'=>'Institution Name', 'field'=>'ac.institution_name', 'sort'=>'asc'),
				array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'asc'),
				// array('data'=>'Department', 'field'=>'wr.department'),
				array('data'=>'Organiser', 'field'=>'o.organiser_name')
			);
		}
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		//var_dump($query);
		$result = pager_query($query , 50, 0, $count_query);
		$count = db_query($count_query);
		$count = db_fetch_object($count);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			if(in_array('resource_person', array_values($user->roles))){
				$row[] = array($item->workshop_code, human_text($item->foss_category), $item->institution_name, $item->cfm_wkshop_date.' '.$item->cfm_wkshop_time, /*$item->department,*/ $item->organiser_name, l('Edit', 'workshops/workshop_edit/'. $item->id, array('attributes' => array('class' => 'edit-record', 'title' => 'Edit'))).' '.l('Reject', 'workshops/workshop_delete/'. $item->id, array('attributes' => array('class' => 'delete-record', 'title' => 'Reject', 'onclick' => 'return confirm("Are you sure?")'))));
			}else {
				$row[] = array($item->workshop_code, human_text($item->foss_category), $item->academic_code, $item->cfm_wkshop_date.' '.$item->cfm_wkshop_time, /*$item->department,*/$item->organiser_name);
			}
		}
		if(in_array('resource_person', array_values($user->roles))) {
		$output = "<p><b>Total no of workshops : ".$count->count."</p></b>";
		$output .= drupal_get_form('workshop_filter_form', $selected_val);
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




function workshops_list_workshop_completed(){
		global $user;
		$filter = unserialize(base64_decode($_REQUEST['filter']));
		$filter_query = '';
		
		$selected_val = array();
		
		if($filter['foss'] != ''){
			$filter_query .= " and wr.foss_category = '".$filter['foss']."'";
			$selected_val['foss'] = $filter['foss'];
		}
		if($filter['language'] != ''){
			$filter_query .= " and wr.pref_language = '".$filter['language']."'";
			$selected_val['language'] = $filter['language'];
		}
		if($filter['state'] != ''){
			$filter_query .= " and wr.academic_code like '".$filter['state']."%'";
			$selected_val['state'] = $filter['state'];
		}
		if($filter['type'] != ''){
			if($filter['type'] == 'school'){
				$filter_query .= " and ac.school_college = 0";
				$selected_val['type'] = $filter['type'];
			}
			else if($filter['type'] == 'college'){
				$filter_query .= " and ac.school_college = 1 and ac.institution_name NOT LIKE '%polytechnic%' AND ac.institution_name NOT LIKE 'iti%'";
				$selected_val['type'] = $filter['type'];
			}
			else if($filter['type'] == 'polytechnic'){
				$filter_query .= " and ac.school_college = 1 and ac.institution_name LIKE '%".$filter['type']."%'";
				$selected_val['type'] = $filter['type'];
			}
			else if($filter['type'] == 'iti'){
				$filter_query .= " and ac.school_college = 1 and ac.institution_name LIKE '".$filter['type']."%'";
				$selected_val['type'] = $filter['type'];
			}
			else{
				//$filter_query .= " and ac.institution_name like '%".$filter['type']."%'";
				$selected_val['type'] = $filter['type'];
			}
			
		}
		if($filter['from_date'] != ''){
			$filter_query .= " and wr.cfm_wkshop_date >= '".$filter['from_date']."'";
			$selected_val['from_date'] = $filter['from_date'];
		}
		if($filter['to_date'] != ''){
			$filter_query .= " and wr.cfm_wkshop_date <= '".$filter['to_date']."'";
			$selected_val['to_date'] = $filter['to_date'];
		}
		$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.status=2 and ac.academic_code=wr.academic_code and ac.academic_code != 'MAH-00029'";
		$count_query = "select count(*) as count from workshop_requests wr, academic_center ac where wr.status=2 and ac.academic_code=wr.academic_code  and ac.academic_code != 'MAH-00029'";
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=2".$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code != 'MAH-00029'";
			$count_query = "select count(*) as count from workshop_requests wr, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=2".$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code != 'MAH-00029'";

		}
		
		if((in_array('resource_person', array_values($user->roles)))||($user->uid=='0')) {
			if($user->id=='0')
			{
				$st_code='';
			}
			else
			{
			$query = "select states from resource_person where user_uid=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$state_code_arr = explode(',', $row->states);
			}
			
		
			$tmp_query_param = "";
			if(!isset($selected_val['state'])){
				foreach($state_code_arr as $st_code){
					if($tmp_query_param != ""){
						$tmp_query_param .= " or ";
					}
					$tmp_query_param .=	"wr.academic_code like '".$st_code."%'";
				}
				$tmp_query_param = ' and ('.$tmp_query_param.')';
			}
			$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.status=2 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code!='MAH-00029'";
			//echo $query."<br /><br />";
			$count_query = "select count(*) as count from workshop_requests wr, academic_center ac where wr.status=2 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code!='MAH-00029'";
			//echo $count_query."<br />"; die;
			$participant_count_query = "select sum(wd.no_of_participants) as count from workshop_requests wr, academic_center ac, workshop_details wd where wr.status=2 ".$tmp_query_param.$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code!='MAH-00029' and wd.workshop_code=wr.workshop_code";
			
		}
		//var_dump($query);
		//var_dump($count_query);
		$header = array(
			array('data'=>'Workshop Code', 'field'=>'wr.id', 'sort'=>'desc'),
			// array('data'=>'FOSS', 'field'=>'wr.foss_category'),
			// array('data'=>'Language', 'field'=>'wr.pref_language'),
			array('data'=>'Institution', 'field'=>'ac.institution_name'),
			array('data'=>'State', 'field'=>'ac.state_code'),
			array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'asc')
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 50, 0, $count_query);
		$count = db_query($count_query);
		$count = db_fetch_object($count);
		$participant_count = db_query($participant_count_query);
		$participant_count = db_fetch_object($participant_count);
		db_set_active('default');
		$row = array();
		$sno = 1;
		$states_array = get_states(0);
		//var_dump($states_array);
		//die;
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->workshop_code, $item->institution_name, $states_array[$item->state_code], $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/view_workshop/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))), l('Download Certificate', 'attendance-register/list/'. $item->id, array('attributes' => array('target' => '_blank', 'title' => 'Download Certificate'))));
		}
		if((in_array('resource_person', array_values($user->roles)))||($user->uid=='0')) {
		$output = "<table><tr><th><b>Total no of workshops : ".$count->count."</b></th><th><b>Total no of participant : ".$participant_count->count."</b></th></tr></table></b>";
		$output .= drupal_get_form('workshop_filter_form', $selected_val);
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


/*
*/

	function workshops_workshop_confirm($id = NULL){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
  drupal_add_js("$(document).ready(function(){ new datepickr('edit-cfm-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		if($id){
			$query = "select * from {workshop_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);
			db_set_active('default');
			//$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			//$result = db_query($query);
			//$academic_item = db_fetch_object($result);
			$output = '<div class="form-item" id="academic-details"><table width="100%" cellpadding="6">';
			$output .= '<tr><td>Academic Center Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->pref_wkshop_date.' '.$item->pref_wkshop_time.'</td>
			</tr>';
			$output .= '<tr><td>Skype Request</td><td>: '.(($item->skype_request == 1) ? 'Yes': 'No').'</td></tr>';
			$output .= '</table></div>';
			$output .= drupal_get_form('workshops_workshop_confirm_form', $id, $item->pref_wkshop_date, $item->pref_wkshop_time, $item->organiser_id);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_workshop_confirm_form($form_state, $id, $date_val, $time_val, $oid){
	
		$time_arr = explode(':', $time_val);
		$hour = array();
		$minite = array();
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['id'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Workshop Date'),
			'#value' => $id,
		);
		$form['oid'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Workshop Date'),
			'#value' => $oid,
		);
		$form['cfm_wkshop_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirm Workshop Date'),
			'#required' => TRUE,
			'#size' => 30,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_wkshop_date', 'readonly' => 'readonly'),
		);

		$form['cfm_wkshop_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-wkshop-time'>Confirm Workshop Time:<span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['cfm_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	function workshops_workshop_confirm_form_validate($form, &$form_state){
		// if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['phone'])){
		// 	form_set_error('phone number', t('Invalid Phone number'));
		// }
		// if (!preg_match('/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/', $form_state['values']['email'])){
		// 	form_set_error('email', t('Invalid Email'));
		// }
  		return;

	}
	function workshops_workshop_confirm_form_submit($form, &$form_state){
	
		global $user;
		global $base_url;
		$cfm_wkshop_date = $_POST['cfm_wkshop_date'];
		$cfm_wkshop_time = $_POST['cfm_wkshop_time'].':'.$_POST['cfm_wkshop_minute'].':00';
		$workshop_code = 'WC-'.$form_state['values']['id'];
		$query = "update {workshop_requests} set workshop_code='".$workshop_code."', cfm_wkshop_date='".$cfm_wkshop_date."', cfm_wkshop_time='".$cfm_wkshop_time."', status=1 where id=".$form_state['values']['id'];
		db_set_active('workshop_info');
		if(db_query($query)){
			// mail to Resorce Person

			//$mailfrom = variable_get('site_mail');
			$mailto = $user->mail;
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$user->name.",</p><p>workshop ".$workshop_code." has been approved.</p>";
			//workshops_notify($mailto, $subject, $message);
			
			
			// send email to Organiser
			$query = "select name, mail from users where uid='".$_POST['oid']."'";
			db_set_active('default');
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
		
			$query1 = "select foss_category from workshop_requests where organiser_id='".$_POST['oid']."'";
			db_set_active('workshop_info');
			$result1 = db_query($query1);
			$row1 = db_fetch_object($result1);
			
			//var_dump($row1->foss_category);
			//die;
			
			//$mailfrom = variable_get('site_mail');
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$row->name.",</p><p> Your new workshop request has has been confirmed.</p><p><b>Workshop Details</b></p><p>Workshop code : ".$workshop_code."</p><p>Confirmed Workshop Date : ".$cfm_wkshop_date."</p><p>Confirmed Workshop Time : ".$cfm_wkshop_time."</p>";
			$message1 = "Dear ".$row->name.",</p>
						<p>Your workshop on ".$row1->foss_category.", requested on ".$cfm_wkshop_date." at ".$cfm_wkshop_time." has been approved. Please find the workshop code ".$workshop_code.". This code is very important, please preserve it for your future reference.</p>
						<p>If you have clicked YES for Skype support then you can send us a request on our Skype ID namely st-wshop and st-iitb. Any day before the workshop please call us on Skype to have a Skype testing session and interact with Spoken Tutorial, IIT Bombay team.</p>
						<p>You also need to download the specified software, for that Click below.<br><a href='http://process.spoken-tutorial.org/images/1/1b/Download-Tutorials.pdf' target='_blank'>http://process.spoken-tutorial.org/images/1/1b/Download-Tutorials.pdf</a>
						<p>For getting the lab and systems ready for the workshop Click below<br><a href='http://process.spoken-tutorial.org/images/5/58/Machine-Readiness.pdf' target='_blank'>http://process.spoken-tutorial.org/images/5/58/Machine-Readiness.pdf</a>";
			var_dump($message1);
			die;
			//workshops_notify($mailto, $subject, $message);
			drupal_set_message('Workshop Confirmation Added Successfully');
			drupal_goto($path = 'workshops/list_workshop_approval', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Confirmation Details');
		}
		db_set_active('default');
	}
	function workshops_workshop_edit($id = NULL){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/datepickr.js');
		drupal_add_css($path . '/datepickr.css');
		drupal_add_js($path . '/workshops.js');
		drupal_add_css($path . '/workshops.css');
		drupal_add_js("$(document).ready(function(){ new datepickr('edit-cfm-wkshop-date', { dateFormat: 'Y-m-d' }); });", 'inline');
		if($id){
			$query = "select * from {workshop_requests} WHERE id='". $id."'";
			db_set_active('workshop_info');
			$result = db_query($query);
			$item = db_fetch_object($result);
			db_set_active('default');
			//$query = "select * from {academic_center} WHERE academic_code='". $item->academic_code."'";

			//$result = db_query($query);
			//$academic_item = db_fetch_object($result);
			$output = '<div class="form-item" id="academic-details"><table width="100%" cellpadding="6">';
			$output .= '<tr><td>Academic Center Code</td><td>: '.$item->academic_code.'</td></tr>';
			$output .= '<tr><td>Language</td><td>: '.$item->pref_language.'</td></tr>';
			$output .= '<tr><td>FOSS Category</td><td>: '.$item->foss_category.'</td></tr>';
			$output .= '<tr><td>Preferred Date & Time</td><td>: '.$item->cfm_wkshop_date.' '.$item->cfm_wkshop_time.'</td></tr>';
			$output .= '<tr><td>Skype request</td><td>: '.($item->skype_request ? "Yes" : "No").'</td></tr>';
			$output .= '</table></div>';
	
			$output .= drupal_get_form('workshops_workshop_edit_form', $id, $item->cfm_wkshop_date, $item->cfm_wkshop_time, $item->organiser_id);
			return $output;
		}else{
			return "<p>Sorry, no data found.</p>";
		}
	}

	function workshops_workshop_edit_form($form_state, $id, $date_val, $time_val, $oid){
		$time_arr = explode(':', $time_val);
		$hour = array();
		$minite = array();
		$i=0;
		while($i <= 23){
			if($i < 10){
				$hour['0'.$i] = '0'.$i;
			}else{
				$hour[$i] = $i;
			}
			$i++;
		}
		$i=0;
		while($i <= 59){
			if($i < 10){
				$minite['0'.$i] = '0'.$i;
			}else{
				$minite[$i] = $i;
			}
			$i++;
		}
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['id'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Workshop Date'),
			'#value' => $id,
		);
		$form['oid'] = array(
			'#type' => 'hidden',
			'#title' => t('Preferred Workshop Date'),
			'#value' => $oid,
		);
		$form['cfm_wkshop_date'] = array(
			'#type' => 'textfield',
			'#title' => t('Confirm Workshop Date'),
			'#required' => TRUE,
			'#size' => 30,
			'#value' => $date_val,
			'#attributes' => array('class' => 'cfm_wkshop_date', 'readonly' => 'readonly')
		);
		$form['cfm_wkshop_time'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $hour,
			'#default_value' => $time_arr[0],
			'#attributes' => array('class' => 'pref_wkshop_time'),
			'#prefix' => "<div class='date-and-time'><label for='edit-pref-wkshop-time'>Confirm Workshop Time:<span class='form-required' title='This field is required.'>*</span></label>",
		);
		$form['cfm_wkshop_minute'] = array(
			'#type' => 'select',
			'#required' => TRUE,
			'#options' => $minite,
			'#default_value' => $time_arr[1],
			'#attributes' => array('class' => 'pref_wkshop_minute'),
			'#suffix' => "</div>"
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		return $form;
	}
	
	function workshops_workshop_edit_form_submit($form, &$form_state){
		global $user;
		$cfm_wkshop_date = $_POST['cfm_wkshop_date'];
		$cfm_wkshop_time = $_POST['cfm_wkshop_time'].':'.$_POST['cfm_wkshop_minute'].':00';
		$query = "update {workshop_requests} set cfm_wkshop_date='".$cfm_wkshop_date."', cfm_wkshop_time='".$cfm_wkshop_time."', status=1 where id=".$form_state['values']['id'];
		db_set_active('workshop_info');
		if(db_query($query)){
		
		// mail to Resorce Person

			//$mailfrom = variable_get('site_mail');
			$mailto = $user->mail;
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$user->name.",</p><p>Approved workshop WC-".$_POST['id']." has been Updated.</p>";
			//workshops_notify($mailto, $subject, $message);
			// send email to Organiser
			$query = "select name, mail from users where uid='".$_POST['oid']."'";
			db_set_active('default');
			$result = db_query($query);
			$row = db_fetch_object($result);
			$mailto = $row->mail;
			//$mailfrom = variable_get('site_mail');
			$subject = 'Workshop confirmation Notification';
			$message = "<p>Dear ".$row->name.",</p><p> Your workshop WC-".$_POST['id']." has has been postponded.</p><p><b>Workshop Details</b></p><p>Confirmed Workshop Date : ".$cfm_wkshop_date."</p><p>Confirmed Workshop Time : ".$cfm_wkshop_time."</p>";
			//workshops_notify($mailto, $subject, $message);
			drupal_set_message('Workshop Confirmation Added Successfully');

			drupal_goto($path = 'workshops/list_workshop_approval', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			drupal_set_message('Error Adding Workshop Confirmation Details');
		}
		db_set_active('default');
	}
	function workshops_view_workshop($id = NULL){
	 	if($id){
	 		$query = "select * from {workshop_requests} where id=".$id;
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
	 		if($result){
	 			$item = db_fetch_object($result);
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>Workshop Code</b></td><td><b>'.$item->workshop_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Foss Category</b></td><td><b>'.human_text($item->foss_category).'</b></td></tr>';
	 			$output .= '<tr><td><b>Language</b></td><td><b>'.$item->pref_language.'</b></td></tr>';
	 			$output .= '<tr><td><b>Date & Time</b></td><td><b>'.$item->cfm_wkshop_date.' &nbsp;'.$item->cfm_wkshop_time.'</b></td></tr>';

	 			$query = "select * from  {workshop_details} where workshop_code='".$item->workshop_code."'";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$ws_detail = db_fetch_object($result);

	 			$output .= '<tr><td><b>Participants Count</b></td><td><b>'.$ws_detail->no_of_participants.'</b></td></tr>';
	 		//	$output .= '<tr><td><b>Invigilator</b></td><td><b>'.$ws_detail->name_of_invigilator.'</b></td></tr>';
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';

	 			$query = "select * from {academic_center} where academic_code='".$item->academic_code."'";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$coll_detail = db_fetch_object($result);

	 			$output .= '<tr><td><b>State Code</b></td><td><b>'.$coll_detail->state_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$coll_detail->academic_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$coll_detail->institution_name.'</b></td></tr>';
	 			$output .= '<tr><td><b>Department</b></td><td><b>'.$item->department.'</b></td></tr>';
	 			$output .= '<tr><td><b>Street</b></td><td><b>'.$coll_detail->street.'</b></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td><b>'.$coll_detail->city.'</b></td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td><b>'.$coll_detail->pincode.'</b></td></tr>';
	 			$output .= '</table>';
	 			
	 			$output .= '<h2>Participants Details</h2>';
	 			//$query = "select id, user_id from {workshop_feedback} where workshop_code='".$item->workshop_code."'";
	 			$query = "select u.username as user_name, wf.user_id as uid from workshop_info.workshop_feedback wf, OTC.mdl_user u where wf.workshop_code='".$item->workshop_code."' and u.id=wf.user_id";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$output .= '<table width="100%" cellpadding="6"><tr><th>User name</th><th>Action</th></tr>';
	 			while($fd = db_fetch_object($result)){
	 				$output .= '<tr><td>'.$fd->user_name."</td><td>".l('View Feedback', 'workshops/feedback/'.$item->workshop_code.'/'.$fd->user_name.'/'.$fd->uid)."</td></tr>";
	 				
	 			}
	 			$output .= '</table>';
	 			//exit;

	 			
	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
	 	}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}

	function workshops_admin_workshop_completed(){
		global $user;
		$query = "select * from {workshop_requests} where status=2 ORDER BY workshop_code ASC";
		// $query = "SELECT * FROM {workshop_requests} WHERE status=2 ORDER BY foss_category, pref_language ASC";
		$header = array('Sno','Workshop Code', 'FOSS', 'Language', 'Academic Center Code', 'Department', 'Date & Time', 'Action');
		$tablesort = tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query . $tablesort, 30, 0, NULL);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($sno++, $item->workshop_code, $item->foss_category, $item->pref_language, $item->academic_code, $item->department, $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/view_workshop/'. $item->id));
		}
		if (count($row) == 0) {
			$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
		} else {
			drupal_set_message($message);
			$output = theme('table', $header, $row);
			$output .= theme('pager');
		}
		return $output;
	}
?>
