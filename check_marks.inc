<?php
	require_once('common.inc');
	function check_marks() {
		$output = '';
		if(isset($_GET['code'])){
			$test_code = trim(base64_decode($_GET['code']));
			$query = "select tr.foss_category, tr.cfm_test_date, iv.invigilator_name, og.organiser_name, ac.institution_name, ac.city, ac.state_code from test_requests tr, invigilator iv, organiser og, academic_center ac where tr.test_code='".$test_code."' and tr.status=4 and iv.invigilator_id=tr.invigilator_id and og.organiser_id=tr.organiser_id and ac.academic_code=tr.academic_code";
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$row = db_fetch_object($result);
			$states = get_states(0);
			$foss_categories["PHP-and-MySQL"] = 8;
			$foss_categories["Linux"] = 9;
			$foss_categories["C-and-C-Plus-Plus"] = 11;
			$foss_categories["Linux-Ubuntu"] = 9;
			$foss_categories["Java"] = 20;
			$foss_categories["Java1"] = 30;
			$foss_categories["Java2"] = 47;
			$foss_categories["Scilab"] = 4;
			$foss_categories["Python"] = 10;
			$foss_categories["LaTeX"] = 5;
			$foss_categories["OpenFOAM"] = 16;
			$foss_categories["C"] = 18;
			$foss_categories["C1"] = 28;
			$foss_categories["C2"] = 33;
			$foss_categories["C3"] = 38;
			$foss_categories["C3"] = 42;
			$foss_categories["C-Plus-Plus"] = 22;
			$foss_categories["C-Plus-Plus1"] = 29;
			$foss_categories["C-Plus-Plus2"] = 31;
			$foss_categories["C-Plus-Plus3"] = 46;
			$foss_categories["CheckList"] = 6;
			if($row) {
				$output .= '<table>';
				$output .= '<tr><td><b>Test Code: </b>'.$test_code.'</td>';
				$output .= '<td><b>FOSS: </b>'.$row->foss_category.'</td></tr>';
				$output .= '<tr><td><b>Organiser: </b>'.$row->organiser_name.'</td>';
				$output .= '<td><b>Invigilator: </b>'.$row->invigilator_name.'</td></tr>';
				$output .= '<tr><td><b>Institution: </b>'.$row->institution_name.'</td>';
				$output .= '<td><b>City: </b>'.$row->city.'</td></tr>';
				$output .= '<tr><td><b>State: </b>'.$states[$row->state_code].'</td><td></td></tr>';
				$output .= '</table><br />';
				$tmp_query = "mg.quiz=".$foss_categories[$row->foss_category];
				$tmp_query1 = "";
				if(isset($foss_categories[$row->foss_category."1"]))
					$tmp_query1 .= " or mg.quiz=".$foss_categories[$row->foss_category."1"];
				if(isset($foss_categories[$row->foss_category."2"]))
					$tmp_query1 .= " or mg.quiz=".$foss_categories[$row->foss_category."2"];
				if(isset($foss_categories[$row->foss_category."3"]))
					$tmp_query1 .= " or mg.quiz=".$foss_categories[$row->foss_category."3"];
				if($tmp_query != "")
					$tmp_query = "(".$tmp_query.$tmp_query1.")";
				// $query = "select ar.moodle_uid, mu.firstname, mu.lastname, mu.gender, mg.grade from workshop_info.attendance_register ar, OTC.mdl_user mu, OTC.mdl_quiz_grades mg where ar.test_code='".$test_code."' and status > 0 and mu.id=ar.moodle_uid and mg.userid=ar.moodle_uid and mg.quiz=".$foss_categories[$row->foss_category];
			    $query = "select distinct moodle_uid from attendance_register where test_code='".$test_code."'";
			    db_set_active('workshop_info');
			    $uid_result = db_query($query);
			    db_set_active('default');
			    $tmp = '';
			    $count = 0;
			    while($uid_row = db_fetch_object($uid_result)) {
				$query = "select mu.firstname, mu.lastname, mu.gender, mg.grade from mdl_user mu, mdl_quiz_grades mg where mu.id=".$uid_row->moodle_uid." and mg.userid=mu.id and ".$tmp_query." order by mg.id DESC LIMIT 1";
				//var_dump($query); die;
				db_set_active('moodle');
				$result = db_query($query);
				db_set_active('default');
				while($rec = db_fetch_object($result)) {
					$count++;
					$tmp .= '<tr><td><iframe style="width:55px; height:55px;" src="http://onlinetest.spoken-tutorial.org/get_profile_picture.php?id='.$uid_row->moodle_uid.'"></iframe></td><td>'.$rec->firstname.' '.$rec->lastname.'</td><td>'.$rec->gender.'</td><td>'.$rec->grade.'</td><td>'.(((float)($rec->grade) >= 40)?'Pass':'Fail').'</td>';
				}
			    }
				if($count) {
					$output .= '<table><tr style="font-weight: bold;"><td></td><td>Name</td><td>Gender</td><td>Grade</td><td>Result</td></tr>'.$tmp.'</table>';
				}else {
					$output .= "<p>Users information not available.</p>";
				}
			}else {
				$output = '<p>Invalid Test Code!</p>';
			}
		}
		
		return drupal_get_form('check_marks_form').'<div class="output">'.$output.'</div>';
	}
	function check_marks_form($form_state) {
		
		$form['#attributes'] = array('enctype' => "multipart/form-data", 'class' => 'check_marks');
		$form['test_code'] = array(
			'#type' => 'textfield',
			'#title' => t(''),
			'#prefix' => '<table ><tr><td>Test Code: </b></td><td>',
			'#suffix' => '</td></tr>',
			'#required' => true,
			'#attributes' => array('class' => 'test_code')
		);
		$form['save'] = array(
			'#type' => 'submit',
			'#value' => 'Submit',
			'#prefix' => '<tr><td></td><td>',
			'#suffix' => '</td></tr></table>',
			'#attributes' => array('class' => 'locate')
		);
		
		return $form;
	}
	
	function check_marks_form_submit($form, &$form_state) {
		drupal_goto('onlinetest/check_marks', array('code' => base64_encode($_POST['test_code'])), $fragment = NULL, $http_response_code = 302);
	}
?>
