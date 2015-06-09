<?php
	require_once('common.inc');
	function resource_center_details(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_css($path . '/rc_star_rating.css', 'module');
		$params = substr($_REQUEST['q'], strripos($_REQUEST['q'], '/'));
		$ac = explode('=', $params);
		if($ac[1])
			$output = drupal_get_form('resource_center_details_form', $ac[1]);
		else
			$output = drupal_get_form('resource_center_details_form', 'all');
	 	//$output .= '<br /><ul><li>The RESOURCE Centres partner with us and get other colleges in their district to offer the training to their students and Faculty.They do this by organising awareness events and communicating with their contacts. Depending on the number of confirmed colleges that are obtained, they get a star rating. We expect that our esteemed RESOURCE Centres will expand their list of already impressive activities. We will guide and support through and through !<br /><br /></li><li>1 star - if a college is in its first year but became an RC. \'Becoming an RC\' means ONLY if scan sign copy is there and team members are identified and have expressed interest to be active.<br /><br /></li> <li>2 stars - if a college came back in the second year for workshops AND became an RC (as defined above).<br /><br /></li> <li>3 stars - will be given as Diwali gift based on having completed 12 events / new institutions / pilots etc. by end October.</li></ul><br />';
		$output .= '<br /><ul><li><p style="text-align:justify; line-height:1.6">A college or a university can become a RESOURCE Centre (RC) after a few training workshops have been done. RESOURCE Centres ensure that there are multiple organisers from the Institution who are registered on the Spoken Tutorial website and the workshops are being organised to cover all the students in the college through calendars. Additionally, they become a partner with us to spread awareness on the Spoken Tutorial project. This they do by organising Faculty Development and Training programs and Awareness events for colleges in the district. Mention of the Spoken Tutorial workshops can also be made in the class notes or syllabus so that students easily come forth for the training. Depending on the number of confirmed colleges that are obtained, they get a star rating. We expect that our esteemed RESOURCE Centres will expand their list of already impressive activities. We will guide and support through and through!</p></li><li><p style="text-align:justify; line-height:1.6">1 star - if a college is in its first year but became an RC. "Becoming an RC" means ONLY if scan sign copy is there and team members are identified and have expressed interest to be active.</p></li><li><p style="text-align:justify; line-height:1.6">2 stars - if a college has completed multiple organiser registration into the Spoken Tutorial site and workshop calendars are prepared to train all systematically. Have done large number of awareness events.</p></li><li><p style="text-align:justify; line-height:1.6">3 stars - will be given as New Year gift based on having completed 12 events / new institutions / pilot trainings/ fests etc. by end December.</p></li></ul>';
		if(isset($ac[1]) && $ac[1] != 'all'){
			$query = "select id, academic_code, institution_name, city, contact_details, remarks, star_rating from academic_center where state_code='".$ac[1]."' and resource_center=1";
			$sql_count = "select count(id) from academic_center where state_code='".$ac[1]."' and resource_center=1";
		}else {
			$query = "select id, academic_code, institution_name, city, contact_details, remarks, star_rating from academic_center where resource_center=1";
			$sql_count = "select count(id) from academic_center where resource_center=1";
		}

		// $header = array('Academic Code', 'Institution Name', 'City', '');
		$header = array(
			array('data' => 'Sr.no'),
			array('data'=>'Institution', 'field'=>'institution_name', 'sort_order'=>'asc'),
			array('data'=>'City', 'field'=>'city'),
			array('data'=>'Rating', 'field'=>'star_rating'),
			array('data'=>'Action')
		);
		$query .= tablesort_sql($header);
		db_set_active('workshop_info');
		$result = pager_query($query, 30, 0, $sql_count);
		db_set_active('default');
		$row = array();
		$count = 0;
		
		while ($item = db_fetch_object($result)) {
			$stars = '';
			$star_count = (int)($item->star_rating);
			$path = "star2.gif";
			for($i = 0; $i < $star_count; $i++) {
				$stars .= '<div class="rc-star-rating"></div>';
			}
			$row[] = array(++$count, $item->institution_name, $item->city, $stars, l(t('View Activities'), 'workshops/resource_center_view_college/'. $item->id));
		}
		if($count == 0) {
			$output .= "<div class='academic-data'>";
				$output .= "<p>There is no Resource Center for this State     </p>";
			$output .= "</div>";
		}else{
			$output .= theme_table($header,$row);
			$output .= theme_pager(null, 30);
		}

		return $output;
	}

	function resource_center_details_form($form_state, $ac) {
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		// drupal_add_css($path . '/workshops.css', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");
		$form['states'] = array(
			'#type' => 'select',
			'#title' => t('State'),
			'#required' => TRUE,
			'#attributes' => array('id' => 'resource_person_state'),
			'#options' => get_states(2),
			'#default_value' => $ac
		);
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
	
		return $form;
	}

	function resource_center_details_form_validate($form, &$form_state){
	  return;
	}

	function resource_center_details_form_submit($form, &$form_state){
		$redirect = 'workshops/resource_center_details/?ac='.$form_state['values']['states'];
		drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
	}
	
	function resource_center_view_college_details($id = NULL){
		if($id){
			$states_array = get_states(0);
	 		$query = "select * from {academic_center} where id=".$id;
	 		$path = drupal_get_path('module', 'workshops');
			drupal_add_css($path . '/rc_star_rating.css', 'module');
	 		db_set_active('workshop_info');
	 		$result = db_query($query);
	 		db_set_active('default');
			$states_array = get_states(0);
	 		if($result){
	 			$item = db_fetch_object($result);
	 			$stars = '';
				$star_count = (int)($item->star_rating);
				$path = "star2.gif";
				for($i = 0; $i < $star_count; $i++) {
					$stars .= '<div class="rc-star-rating"></div>';
				}
	 			$output = '<table width="100%" cellpadding="6">';
	 			$output .= '<tr><td><b>State</b></td><td>'.$states_array[$item->state_code].'</td></tr>';
	 			//$output .= '<tr><td><b>Academic Center Code</b></td><td>'.$item->academic_code.'</td></tr>';
	 			$tmp = ($item->school_college)? "College" : "School";
	 			$output .= '<tr><td><b>College / School</b></td><td>'.$tmp.'</td></tr>';
	 			$output .= '<tr><td><b>Institution Name</b></td><td>'.$item->institution_name.'</td></tr>';
	 			$output .= '<tr><td><b>Address</b></td><td>'.$item->street.'</td></tr>';
	 			$output .= '<tr><td><b>City</b></td><td>'.$item->city.'</td></tr>';
	 			$output .= '<tr><td><b>State</b></td><td>'.$states_array[$item->state_code].'</td></tr>';
	 			$output .= '<tr><td><b>Pin Code</b></td><td>'.$item->pincode.'</td></tr>';
	 			$output .= '<tr><td><b>Rating</b></td><td>'.$stars.'</td></tr>';
	 			//$output .= '<tr><td><b>Contact Details</b></td><td><p>'.str_replace("\n", "<br />", $item->contact_details).'</p></td></tr>';
	 			$output .= '<tr><td><b>Activities</b></td><td><p>'.str_replace("\n", "<br />", $item->remarks).'</p></td></tr>';
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
