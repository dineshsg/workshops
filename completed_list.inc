<?php
	require_once('common.inc');
	function completed_workshops_list($code = NULL){
		global $user;
		$states_array = get_states(0);
		
		if($code != '' && isset($states_array[$code])){
			$query = "select wr.id, wr.workshop_code, wr.cfm_wkshop_date, wr.foss_category, ac.institution_name, ac.city, wd.no_of_participants from workshop_requests wr, academic_center ac, workshop_details wd where wr.status=2 and wr.academic_code like '".$code."-%' and ac.academic_code=wr.academic_code and wd.workshop_code=wr.workshop_code";
			$count_query = "select count(wr.id) from workshop_requests wr, academic_center ac, workshop_details wd where wr.status=2 and wr.academic_code like '".$code."-%' and ac.academic_code=wr.academic_code and wd.workshop_code=wr.workshop_code";
		
			// $header = array('Institution', 'City', 'FOSS', 'Workshop Date', '');
			$header = array(
				array('data'=>'Institution', 'field'=>'ac.institution_name'),
				array('data'=>'City', 'field'=>'ac.city'),
				array('data'=>'FOSS', 'field'=>'wr.foss_category'),
				array('data'=>'Workshop_Date', 'field'=>'wr.cfm_wkshop_date', 'sort'=>'desc'),
				array('data'=>'No. of students', 'field'=>'wd.no_of_participants'),
			);
			$query .= tablesort_sql($header);
			db_set_active('workshop_info');
			$result = pager_query($query , 30, 0, $count_query);
			db_set_active('default');
			$row = array();
			$sno = 1;
		
			while ($item = db_fetch_object($result)) {
				$row[] = array($item->institution_name, $item->city, human_text($item->foss_category), $item->cfm_wkshop_date, $item->no_of_participants, l('View', 'view_completed_workshop/'. $item->id, array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
			}
			if (count($row) == 0) {
				$output .= '<div style="color:red;text-align:center;">List is empty.</div>';
			} else {
				drupal_set_message($message);
				$output .= theme('table',$header, $row);
				//$output .= str_replace('<table>', '<table itemscope itemtype="http://schema.org/Organisation">', $output);
				$output .= theme_pager(null, 30);
			}
			
			return $output;
		}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}
	
	function view_completed_workshop($id = NULL){
	 	if($id){
	 		$query = "select wr.workshop_code, wr.academic_code, wr.foss_category, wr.cfm_wkshop_date, wr.cfm_wkshop_time, o.organiser_name from workshop_requests wr, organiser o where wr.id=".$id." and o.organiser_id=wr.organiser_id";
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
	 		if($result){

				$item = db_fetch_object($result);

				$query = "select * from  {workshop_details} where workshop_code='".$item->workshop_code."'";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$ws_detail = db_fetch_object($result);
	 		
	 			$query = "select * from academic_center where academic_code='".$item->academic_code."'";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$coll_detail = db_fetch_object($result);
	 			$states_array = get_states(0);
	 			
	 			$output = '<a href="'.base_path().'completed_workshops_list/'.$coll_detail->state_code.'" style="float: right;">Back to list</a>';
	 			$output .= '<table width="100%" cellpadding="6" itemscope itemtype="http://schema.org/Organisation">';
	 			$output .= '<tr><td><b>Workshop Code</b></td><td><b>'.$item->workshop_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Foss Category</b></td><td><b>'.$item->foss_category.'</b></td></tr>';
	 			$output .= '<tr><td><b>Date & Time</b></td><td><b>'.$item->cfm_wkshop_date.' &nbsp;'.$item->cfm_wkshop_time.'</b></td></tr>';
	 			$output .= '<tr><td><b>No.of Participants </b></td><td><b>'.$ws_detail->no_of_participants.'</b></td></tr>';
				$output .= '<tr><td><b>Organiser </b></td><td><b>'.$item->organiser_name.'</b></td></tr>';
	 			$output .= '<tr><td colspan="2"><hr /></td></tr>';
	 			
	 			$output .= '<tr><td><b>Academic Center Code</b></td><td><b>'.$coll_detail->academic_code.'</b></td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td><b>'.$coll_detail->institution_name.'</b></td></tr>';
	 			$output .= '<tr><td><b>Street</b></td><td itemprop="location"><b>'.$coll_detail->street.'</b></td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td><b>'.$coll_detail->city.'</b></td></tr>';
	 			$output .= '<tr><td><b>State</b></td><td><b>'.$states_array[$coll_detail->state_code].'</b></td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td><b>'.$coll_detail->pincode.'</b></td></tr>';
	 			$output .= '</table>';
	 			
	 			/* $output .= '<h2>Participants Details</h2>';
	 			//$query = "select id, user_id from {workshop_feedback} where workshop_code='".$item->workshop_code."'";
	 			$query = "select u.username as user_name, wf.user_id as uid from workshop_info.workshop_feedback wf, OTC.mdl_user u where wf.workshop_code='".$item->workshop_code."' and u.id=wf.user_id";
	 			db_set_active('workshop_info');
	 			$result = db_query($query);
	 			db_set_active('default');
	 			$output .= '<table width="100%" cellpadding="6"><tr><th>User name</th><th>Action</th></tr>';
	 			while($fd = db_fetch_object($result)){
	 				$output .= '<tr><td>'.$fd->user_name."</td><td>".l('View Feedback', 'workshops/feedback/'.$item->workshop_code.'/'.$fd->user_name.'/'.$fd->uid)."</td></tr>";
	 				
	 			}
	 			$output .= '</table>';*/
	 			
	 			return $output;
	 		}else {
	 			drupal_set_message('Error accessing database workshop_info');
	 		}
	 	}else{
	 		return "<p>Sorry, no data found.</p>";
	 	}
	}
?>
