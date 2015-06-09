<?php
require_once('common.inc');
function academic_details(){
	$params = substr($_REQUEST['q'], strripos($_REQUEST['q'], '/'));
	$ac = explode('=', $params);
	$query = "select id, academic_code, institution_name, city from academic_center order by academic_code, institution_name ASC";
	$sql_count = "select count(id) from academic_center";
	$def_state = 'all';
	if(isset($ac[1]) && $ac[1] != 'all'){
		$query = "select id, academic_code, institution_name, city from academic_center where state_code='".$ac[1]."'";
		$sql_count = "select count(id) from academic_center where state_code='".$ac[1]."'";
		$def_state = $ac[1];
	}
	
	$output = drupal_get_form('academic_details_form', $def_state);
	// $header = array('Academic Code', 'Institution Name', 'City', '');
	$header = array(
		array('data'=>'Academic Code', 'field'=>'academic_code','sort'=>'asc'), // Name column will be sort by default
		array('data'=>'Institution', 'field'=>'institution_name'),
		array('data'=>'City', 'field'=>'city'),
		array('data'=>'')
	);
	$query .= tablesort_sql($header);
	db_set_active('workshop_info');
	$result = pager_query($query, 30, 0, $sql_count);
	db_set_active('default');
	$row = array();
	$count = 0;
	while ($item = db_fetch_object($result)) {
		$row[] = array($item->academic_code, $item->institution_name, $item->city, l(t('View'), 'workshops/college/view_college/'. $item->id , array('attributes' => array('class' => 'view-record', 'title' => 'View'))));
		$count++;
	}
	if($count == 0) {
		$output .= "<div class='academic-data'>";
			$output .= "<p>There is no College for this State     </p>";
		$output .= "</div>";
	}else{
		$output .= theme_table($header,$row);
		$output .= theme_pager(null, 30);
	}

	return $output;
}

function academic_details_form($form_state, $def_state) {
	$path = drupal_get_path('module', 'workshops');
	drupal_add_js($path . '/workshops.js', 'module');
	drupal_add_css($path . '/workshops.css', 'module');
	$form['#prefix'] = '<div class="workshop-form">';
	$form['#suffix'] = '</div>';
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['states'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#required' => TRUE,
		'#default_value' => $def_state,
		'#attributes' => array('id' => 'resource_person_state'),
		'#options' => get_states(2),
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	
	return $form;
}

function academic_details_form_validate($form, &$form_state){
  return;
}

function academic_details_form_submit($form, &$form_state){
	$redirect = 'workshops/academic_details/?ac='.$form_state['values']['states'];
	drupal_goto($path = $redirect, $query=NULL, $fragment = NULL, $http_response_code = 302);
}
	
