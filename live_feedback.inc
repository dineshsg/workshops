<?php
	function view_live_feedback($id = NULL){
//		$path = drupal_get_path('module', 'workshops');
//		drupal_add_css($path . '/workshops.css', 'module');
		if($id){
			$query = "select * from live_workshop_participants where id=".$id;
			db_set_active('workshop_info');
			$result = db_query($query);
			db_set_active('default');
			$rating = array(1 => 'Very Bad', 2 => 'Bad', 3 => 'Satisfactory', 4 => 'Good', 5 => 'Excellent');
			$output = '<table class="feedback-message" width="100%" cellpadding="6">';
 			while($fd = db_fetch_object($result)){
 				
				$output .= '<tr><td>Workshop Code</td>'.'<td>: '.$fd->workshop_code.'</td></tr>';
				$output .= '<tr><td>Name</td>'.'<td>: '.$fd->pname.'</td></tr>';
				$output .= '<tr><td>Branch and Year of Study/Area of work</td>'.'<td>: '.$fd->branch.'</td></tr>';
				$output .= '<tr><td>Name of Institution/Organisation</td>'.'<td>: '.$fd->institution.'</td></tr>';
				
				$output .= '<tr><td>Workshop overall Rating</td>'.'<td>:'.$rating[$fd->rate_workshop].'</td></tr>';
				$output .= "<tr><td><p style='color:gray'><b>Please rate this workshop on the following items:</b></p></td></tr>";

				$output .= "<table class='feedback-tick-mark' border='1'><tr><th class='big-column'></th><th>Very Bad</th><th>Bad</th><th>Fair</th><th>Good</th><th>Very Good</th></tr>";
				$output .= "<tr><td class='big-column'>Content</td>".'<td>'.(($fd->content == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->content == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->content == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->content == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->content == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Sequence of tutorials</td>".'<td>'.(($fd->logical_arrangement == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->logical_arrangement == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->logical_arrangement == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->logical_arrangement == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->logical_arrangement == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Clarity of explanation</td>".'<td>'.(($fd->clarity == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->clarity == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->clarity == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->clarity == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->clarity == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Interesting</td>".'<td>'.(($fd->understandable == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->understandable == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->understandable == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->understandable == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->understandable == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Use of appropriate examples</td>".'<td>'.(($fd->included_examples == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->included_examples == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->included_examples == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->included_examples == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->included_examples == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Instruction Sheet </td>".'<td>'.(($fd->instruction_sheet == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->instruction_sheet == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->instruction_sheet == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->instruction_sheet == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->instruction_sheet == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Assignments </td>".'<td>'.(($fd->assignments == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->assignments == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->assignments == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->assignments == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->assignments == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				
				
				$output .= "<br><table class='feedback-tick-mark'  border=1><tr><th></th><th>Slow</th><th>Appropriate</th><th>Fast</th></tr>";
				
				$output .= "<tr><td class='big-column'>Pace of the tutorial</td>".'<td>'.(($fd->pace_tutorial == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->pace_tutorial == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->pace_tutorial == "3") ? '&#10004;' : '')."</td></tr></table>";

				$output .= '<br><div id="qustion-and-answer"><span class="question">What was the most useful thing you learnt  in this workshop&nbsp;</span>'.'<span>:-&nbsp;'.$fd->useful_thing.'</span></div>';


				

				$output .= '<div id="qustion-and-answer"><p class="question"> What were the weaknesses of this workshop :-</p><ol><li="answer">'.($fd->weakness_duration == "0" ? '' : '<li class="answer">Duration of the workshop is less</li>').'  '.($fd->weakness_understand == "0" ? '' : '<li class="answer">Had to listen more than two times to understand the commands</li>').($fd->weakness_narration == "0" ? '' : '<li class="answer">Pace of the narration in the tutorials was very fast</li>').'</ol></div>';
				
				if($fd->other_weakness != ''){
					$output .= '<br><div id="qustion-and-answer"><span class="question">Any other weakness&nbsp;</span><span>:-&nbsp;'.$fd->other_weakness.'</span></div>';
				}
				$output .= '<br><div id="qustion-and-answer"><span class="question">In which language did you watch the tutorials&nbsp;</span><span>:-&nbsp;'.$fd->workshop_language.'</span></div>';
				
				$output .= "<h5><b>To what extent do you think you can apply the information you received today to your work/study?</b></h5>";
				
				$output .= "<table class='feedback-tick-mark' border='1'><tr><th>Not at all</th><th>A little bit</th><th>Somewhat</th><th>Quite a bit</th><th>A lot</th></tr>";
				
				$output .= '<tr><td>'.(($fd->info_received == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->info_received == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->info_received == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->info_received == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->info_received == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				
				//$output .= "<table class='feedback-message'><tr><td>Do you  plan to use the information from the workshop?</td><td>".(($fd->info_use_plan == '0' )? ': No' : ': Yes')."</td></tr></table>";
				//if($fd->info_use_plan == '1'){
					$output .= '<br><div id="qustion-and-answer"><span class="question">If so, how do you plan to use the information&nbsp;</span><span>:-&nbsp;'.$fd->if_yes.'</span></div>';
				//}
				$output .= "<h5>How would you rate the workshop on the following? </h5>";
				$output .= "<table class='feedback-tick-mark' border='1'><tr><th></th><th>Very low</th><th>Below average</th><th>Average</th><th>Above average</th><th>Very high</th></tr>";
				
				$output .= "<tr><td class='big-column'>Setup for learning</td><td>".(($fd->comfortable_learning == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->comfortable_learning == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->comfortable_learning == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->comfortable_learning == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->comfortable_learning == "5") ? '&#10004;' : '')."</td>"."</tr>";
								
				$output .= "<tr><td class='big-column'>Computers in the lab</td><td>".(($fd->working_computers == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->working_computers == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->working_computers == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->working_computers == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->working_computers == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Audio quality</td><td>".(($fd->audio_quality == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->audio_quality == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->audio_quality == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->audio_quality == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->audio_quality == "5") ? '&#10004;' : '')."</td>"."</tr>";
				$output .= "<tr><td class='big-column'>Video quality</td><td>".(($fd->video_quality == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->video_quality == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->video_quality == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->video_quality == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->video_quality == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				$output .= "<h5>How would you rate the Workshop Organiser from your college, in terms of the following? </h5>";
				$output .= "<table class='feedback-tick-mark' border='1'><tr><th></th><th>Very Bad</th><th>Bad</th><th>Fair</th><th>Good</th><th>Very Good</th></tr>";
				
				$output .= "<tr><td class='big-column'>Organisation of the workshop </td><td>".(($fd->orgn_wkshop == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->orgn_wkshop == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->orgn_wkshop == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->orgn_wkshop == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->orgn_wkshop == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Ability to facilitate learning </td><td>".(($fd->facil_learning == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->facil_learning == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->facil_learning == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->facil_learning == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->facil_learning == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Ability to motivate the learners </td><td>".(($fd->motiv_learning == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->motiv_learning == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->motiv_learning == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->motiv_learning == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->motiv_learning == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Time management </td><td>".(($fd->time_mgmt == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->time_mgmt == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->time_mgmt == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->time_mgmt == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->time_mgmt == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				
				$output .= "<h5>How would you rate the IITB Resource person, in terms of the following? </h5>";
				
				$output .= "<table class='feedback-tick-mark' border='1'><tr><th></th><th>Strongly Disagree</th><th>Disagree</th><th>Neutral</th><th>Agree</th><th>Strongly Agree</th></tr>";
				
				$output .= "<tr><td class='big-column'>Sufficient knowledge about the software  </td><td>".(($fd->soft_klg == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->soft_klg == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->soft_klg == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->soft_klg == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->soft_klg == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Provided clear explanations </td><td>".(($fd->prov_expn == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->prov_expn == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->prov_expn == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->prov_expn == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->prov_expn == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Answered questions clearly </td><td>".(($fd->ans_cln == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->ans_cln == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->ans_cln == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->ans_cln == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->ans_cln == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Was interested in helping me learn </td><td>".(($fd->help_lern == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->help_lern == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->help_lern == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->help_lern == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->help_lern == "5") ? '&#10004;' : '')."</td>"."</tr>";
				
				$output .= "<tr><td class='big-column'>Executed the workshop efficientlyt </td><td>".(($fd->exec_effly == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->exec_effly == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->exec_effly == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->exec_effly == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->exec_effly == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				
				//$output .= '<div id="qustion-and-answer"><p class="question">What was most useful about this workshop?</p><p class="answer">'.$fd->most_use.'<p></div>';
				//$output .= '<div id="qustion-and-answer"><p class="question">What was least useful about this workshop?</p><p class="answer">'.$fd->least_use.'<p></div>';
				$output .= '<br><div id="qustion-and-answer"><span class="question">How can the workshop be improved&nbsp;</span><span class="answer">:-&nbsp;'.$fd->ws_improved.'<span></div>';
 				
 				$output .= "<h5>Would you recommend this workshop, Spoken Tutorial Project and its activities to others?</h5>";
 				
				$output .= "<table class='feedback-tick-mark' border='1'><tr><th>Not at all</th><th>Maybe</th><th>Likely</th><th>Quite likely</th><th>Definitely</th></tr>";
				
				$output .= '<tr><td>'.(($fd->recomm_wkshop == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->recomm_wkshop == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->recomm_wkshop == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->recomm_wkshop == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->recomm_wkshop == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				
				$output .= '<br><div id="qustion-and-answer"><span class="question">Reason why&nbsp;</span>:-&nbsp;<span>'.$fd->reason_why.'</span></div>';
				//$output .= "<tr><td class='big-column'>Setup for learning</td><td>".(($fd->comfortable_learning == "1") ? '&#10004;' : '')."</td>".'<td>'.(($fd->comfortable_learning == "2") ? '&#10004;' : '')."</td>"."<td>".(($fd->comfortable_learning == "3") ? '&#10004;' : '')."</td>"."<td>".(($fd->comfortable_learning == "4") ? '&#10004;' : '')."</td>"."<td>".(($fd->comfortable_learning == "5") ? '&#10004;' : '')."</td>"."</tr></table>";
				
				
						
				//$output .= '<div id="qustion-and-answer"><p class="question"> How could this workshop be improved ?</p><p class="answer">'.$fd->like_improved.'<p></div>';
				//$output .= '<div id="qustion-and-answer"><p class="question">Would you recommend the FOSS, Spoken Tutorial Project and its activities to others?</p><p class="answer">'.$fd->foss_recommend.'<p></div>';
				//$output .= '<div id="qustion-and-answer"><p class="question"> Why/why not </p><p class="answer">'.$fd->why_or_whynot.'<p></div>';
 				
 			}
 			//$output .= '</table>';
		}else{
			$output = "<p>There is no data</p>";
		}
		return $output;

	}



?>
