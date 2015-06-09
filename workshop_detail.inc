<?php
	
	function workshop_detail(){
		global $user;
		/*if((in_array('resource_person', array_values($user->roles)))&&($user->uid !== '5517')){
		$output = drupal_set_message("You cannot fill the details! as you are resource person");
		}
		else
		{*/
		$output = '';//<div class="form-item" id="workshop-details"></div>';
		drupal_set_message('<p>Take the assessment test within 4 weeks of the Workshop. Exceptions in special cases can be considered.</p>');
        drupal_set_message('<p>For instructions how to request the test <a href="http://process.spoken-tutorial.org/images/a/aa/Test_Request.pdf" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
        drupal_set_message('<p>Inform the Invigilator to create an account before making Test Request</p>');
		$output .= drupal_get_form('workshop_detail_form');
		//}
		return $output;
	}
	function workshop_detail_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['workshop_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Workshop Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'detail_workshop'),
		);
		$form['no_of_participants'] = array(
			'#type' => 'textfield',
			'#title' => t('No of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'no_of_participants'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function workshop_detail_form_validate($form, &$form_state){
     if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
            form_set_error('no_of_participants', t('Invalid participants count'));
        }
        if ($form_state['values']['workshop_code'] != ''){
            $query = "select id, cfm_wkshop_date from workshop_requests where workshop_code='".$form_state['values']['workshop_code']."'";
                        db_set_active('workshop_info');
                        $result = db_query($query);
                        db_set_active('default');
                        $row = db_fetch_object($result);
                        if(!$row){
                            form_set_error('workshop_code', t('Invalid Workshop Code!'));
                            return;
                        }
                        if($row->cfm_wkshop_date > date("Y-m-d")){
                            
                            form_set_error('workshop_code', t('You are trying to enter the details of workshop, which has been schedule on '.date("d M Y", strtotime($row->cfm_wkshop_date)).'!'));
                            return;
                        }
                        
            $query = "select id from workshop_details where workshop_code='".$form_state['values']['workshop_code']."'";
            db_set_active('workshop_info');
            $result = db_query($query);
            db_set_active('default');
            if($row = db_fetch_object($result)){
                form_set_error('workshop_code', t('You have already entered workshop detail'));
            }
        }
        
          return;

    }
	function workshop_detail_form_submit($form, &$form_state){
		
		
		$query = "update workshop_requests set status=2 where workshop_code='".trim(strtoupper($form_state['values']['workshop_code']))."'";
		db_set_active('workshop_info');
		db_query($query);
		$query = "insert into  workshop_details (workshop_code, no_of_participants) values('".$form_state['values']['workshop_code']."', '".$form_state['values']['no_of_participants']."')";	
		if(db_query($query)){
		
			db_set_active('default');
		
			// mail to Organiser
			//$query = "select email, organiser_name, workshop_requests.foss_category from organiser left join workshop_requests on organiser.username = workshop_requests.organiser_id where workshop_requests.workshop_code='".$form_state['values']['workshop_code']."'";
			
			$query_wc = "select organiser_id,foss_category from workshop_requests where workshop_code='".$form_state['values']['workshop_code']."'";
			db_set_active('workshop_info');
			$result_wc = db_query($query_wc);
			$row_wc = db_fetch_object($result_wc);
			
			$query_org = "select organiser_name from organiser where organiser_id='".$row_wc->organiser_id."'";
			$result_org = db_query($query_org);
			$row_org = db_fetch_object($result_org);

			$query_mail = "select name, mail from users where uid='".$row_wc->organiser_id."'";
			db_set_active('default');
			$result_mail = db_query($query_mail);
			$row_mail = db_fetch_object($result_mail);
			$mailto = $row->mail;
			
			//var_dump($query);
			//die;
			db_set_active('workshop_info');
			$result = db_query($query);
			$row = db_fetch_object($result);
			db_set_active('default');

			//$mailfrom = variable_get('site_mail');
			$mailto = $row->email;
			$subject = 'Workshop Detail';
			
			//$message = "<p>Dear ".$row_org->organiser_name.",</p><p>Workshop ".$form_state['values']['workshop_code']." details has been updated. The details are </p><p>No of Participants : ".$form_state['values']['no_of_participants']."</p><p>Name of Invigilator : ".$form_state['values']['name_of_invigilator']."</p>";
			$message1 = "<p>Dear ".$row_org->organiser_name.",</p>
			            <p>You have successfully completed the workshop ".$form_state['values']['workshop_code']." on ".$row_wc->foss_category." at your institute. Please see the following instructions for the future activities.</p>
			            <p><strong>Bulk Workshops</strong><br>It is necessary for all students in the Institute to get the opportunity to take Spoken Tutorial based software training. For this please spread the awareness about the workshops in other departments amongst the Faculty. You can also request the Principal to send out a circular to all. For your department prepare a calendar/time-table to organise workshops for all the student batches in a systematic way. Send us the confirmed schedule soon for the upcoming workshops. To view sample calenders Click here ( Provide the link for sample calenders). To know which software is relevant for which department Click here  (Provide the link for e- brochure).</p>
			            <p><strong>Online Assessment test</strong><br>After the workshop, the participants need to complete listening to all the tutorials and practice well including solving the assignment questions. This revision needs to be done at least 2 times so that the students are ready to take the online test. The organiser needs to keep a watch that proper revision is being done. Total time taken can be anywhere from 2 weeks to 1 month or even more. <strong>Fix the online test date after confirming that all students have practiced well.</strong></p>
			            <p>Please note that an invigilator is required for the online test. For this, identify a faculty member who can invigilate the test and help him/her register on the spoken-tutorial.org website before you make a test request.</p>
						<p>Click on the link below for the instructions for invigilator:<br>
						<a href='process.spoken-tutorial.org/images/0/09/Instructions_for_Invigilator.pdf' target='blank'>process.spoken-tutorial.org/images/0/09/Instructions_for_Invigilator.pdf</a>
						<p>To make an online Test Request please <a href='process.spoken-tutorial.org/images/a/aa/Test_Request.pdf' target='blank'>Click here</a> for the instructions on how to request the test</p>";
			//var_dump($message1);
			//die;
			//workshops_notify($mailto, $subject, $message);
			
			drupal_set_message('Workshop Details Added Successfully');
			//drupal_set_message('<p>Take the assessment test within 4 weeks of the Workshop. Exceptions in special cases can be considered.</p>');
			//drupal_set_message('<p>For instructions how to request the test <a href="http://process.spoken-tutorial.org/images/a/aa/Test_Request.pdf" style="font-size: 16px;"><b><i>Click Here</i></b></a></p>');
			//drupal_set_message('<p>Inform the Invigilator to create an account before making Test Request</p>');
			drupal_goto($path = 'workshops/workshop_detail', $query = NULL, $fragment = NULL, $http_response_code = 302);
		}else {
			db_set_active('default');
			drupal_set_message('Error Adding Workshop Details');
		}
	}
?>
