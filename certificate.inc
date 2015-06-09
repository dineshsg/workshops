<?php

	function learner_certificate()
	{
		global $user;
		$query = "select wr.*, ac.institution_name, ac.state_code from workshop_requests wr, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=2".$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code != 'MAH-00029'";
		$count_query = "select count(*) as count from workshop_requests wr, academic_center ac where wr.organiser_id=".$user->uid." and wr.status=2".$filter_query." and ac.academic_code=wr.academic_code  and ac.academic_code != 'MAH-00029'";
		$header = array(
			array('data'=>'Workshop Code', 'field'=>'wr.id', 'sort'=>'desc'),
			array('data'=>'Institution', 'field'=>'ac.institution_name'),
			array('data'=>'State', 'field'=>'ac.state_code'),
			array('data'=>'Date & Time', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'asc')
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query , 30, 0, $count_query);
		$count = db_query($count_query);
		$count = db_fetch_object($count);
		$participant_count = db_query($participant_count_query);
		$participant_count = db_fetch_object($participant_count);
		db_set_active('default');
		$row = array();
		$sno = 1;
		
		while ($item = db_fetch_object($result)) {
			$row[] = array($item->workshop_code, $item->institution_name, $states_array[$item->state_code], $item->cfm_wkshop_date." ".$item->cfm_wkshop_time, l('View', 'workshops/download/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
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
	
	function student_details_form()
	{
		
	}
	
	function download($id = NULL)
	{
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
?>
