<?php
	require_once('common.inc');

	function workshops_list_live_workshop_completed(){
		global $user;
		
		
		$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.status=2 AND ac.academic_code=wr.academic_code AND ac.academic_code='MAH-00029'";
		$count_query = "select count(*) from workshop_requests wr, academic_center ac where wr.status=2 and ac.academic_code=wr.academic_code AND ac.academic_code='MAH-00029'";
		
		if(in_array('organiser', array_values($user->roles))) {
			$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=2 AND ac.academic_code=wr.academic_code AND ac.academic_code='MAH-00029'";
			$count_query = "select count(*) from workshop_requests wr, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=2 AND ac.academic_code=wr.academic_code AND ac.academic_code='MAH-00029'";
		}
		
		if(in_array('resource_person', array_values($user->roles))) {
			$query = "select states from resource_person where user_uid=".$user->uid;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$state_code_arr = explode(',', $row->states);
		
			/*$tmp_query_param = "";
			foreach($state_code_arr as $st_code){
				if($tmp_query_param != ""){
					$tmp_query_param .= " or ";
				}
				$tmp_query_param .=	"wr.academic_code like '".$st_code."%'";
			}*/
			$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.status=2 AND ac.academic_code=wr.academic_code AND ac.academic_code='MAH-00029'";
			//echo $query."<br /><br />";
			$count_query = "select count(*) from workshop_requests wr, academic_center ac where wr.status=2 AND ac.academic_code='MAH-00029' AND ac.academic_code=wr.academic_code";
			//echo $count_query."<br />"; die;
		}
		$header = array(
			array('data'=>'Workshop Code', 'field'=>'wr.workshop_code', 'sort'=>'asc'),
			// array('data'=>'FOSS', 'field'=>'wr.foss_category'),
			// array('data'=>'Language', 'field'=>'wr.pref_language'),
			array('data'=>'Institution', 'field'=>'ac.institution_name'),
			array('data'=>'State', 'field'=>'ac.state_code'),
			array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'desc')
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		db_set_active('default');
		$row = array();
		$sno = 1;
		$states_array = get_states(0);
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->workshop_code, /*human_text($item->foss_category), $item->pref_language,*/ $item->institution_name, $states_array[$item->state_code], $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/view_live_workshop/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
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

	function workshops_view_live_workshop($id = NULL){
		
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
	 			$query = "select id ,pname, institution from live_workshop_participants where workshop_code='".$item->workshop_code."'";

	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$output .= '<table width="100%" cellpadding="6"><tr><th>User name</th><th>Institution</th><th></th></tr>';
	 			while($fd = db_fetch_object($result)){
		 		$output .= '<tr><td>'.$fd->pname."</td><td>".$fd->institution."</td><td>".l('View Feedback', 'workshops/live_feedback/'.$fd->id)."</td></tr>";
	 				
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

?>
