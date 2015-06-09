<?php
	
	function test_detail(){
		$output = '<div class="form-item" id="test-details"></div>';
		$output .= drupal_get_form('test_detail_form');
		return $output;
	}
	function test_detail_form(){
		$path = drupal_get_path('module', 'workshops');
		drupal_add_js($path . '/workshops.js', 'module');
		$form['#prefix'] = '<div class="workshop-form">';
		$form['#suffix'] = '</div>';
		$form['#attributes'] = array('enctype' => "multipart/form-data");

		$form['test_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Test Code'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'detail_test_code'),
		);
		
		$form['batch_no'] = array(
			'#type' => 'hidden',
			'#required' => TRUE,
			'#value' => 1,
		);
		
		$form['no_of_participants'] = array(
			'#type' => 'textfield',
			'#title' => t('No of Participants'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'no_of_participants'),
		);
		$form['invigilator'] = array(
			'#type' => 'textfield',
			'#title' => t('Name of invigilator'),
			'#required' => TRUE,
			'#attributes' => array('class' => 'invigilator'),
		);
		
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
		
		return $form;
	}
	function test_detail_form_validate($form, &$form_state){
		if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['no_of_participants'])){
			form_set_error('no_of_participants', t('Invalid participants count'));
		}
  		return;

	}
	function test_detail_form_submit($form, &$form_state){

		$query = "select id from test_details where test_code='".$form_state['values']['test_code']."' and batch_no=".$form_state['values']['batch_no'];
		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');
		if($row = db_fetch_object($result)){
			drupal_set_message('You already filled these details.');
		} else{
			$query = "update test_requests set status=2 where test_code='".$form_state['values']['test_code']."'";
			db_set_active('workshop_info');
			
			if(db_query($query)){
			
				$query = "insert into  test_details values('', '".$form_state['values']['test_code']."', ".$form_state['values']['batch_no'].", ".$form_state['values']['no_of_participants'].", '".$form_state['values']['invigilator']."')";
				if(db_query($query)){
					// mail to Organiser
					/*$query = "select email, organiser_name from organiser left join workshop_requests on organiser.username = workshop_requests.organiser_id where workshop_requests.workshop_code='".$form_state['values']['workshop_code']."'";
			
					db_set_active('workshop_info');
					$result = db_query($query);
					$row = db_fetch_object($result);
					db_set_active('default');

					//$mailfrom = variable_get('site_mail');
					$mailto = $row->email;
					$subject = 'Workshop Detail';
			
					$message = "<p>Dear ".$row->organiser_name.",</p><p>Workshop ".$form_state['values']['workshop_code']." details has been updated. The details are </p><p>No of Participants : ".$form_state['values']['no_of_participants']."</p><p>Name of Invigilator : ".$form_state['values']['name_of_invigilator']."</p>";
					workshops_notify($mailto, $subject, $message); */
					db_set_active('default');
						
					drupal_set_message('Test admin details added successfully');
					drupal_goto($path = 'workshops/test_detail', $query = NULL, $fragment = NULL, $http_response_code = 302);
				}else {
					db_set_active('default');
					drupal_set_message('Error adding test details');
				}
			} else{
				db_set_active('default');
				drupal_set_message('Error adding test details');
			}
		}
	}
?>
