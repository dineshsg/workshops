<?php
	function get_states($empty){
		$query = "select code, name from states order by name ASC";

		db_set_active('workshop_info');
		$result = db_query($query);
		db_set_active('default');

		$states_array = array();
		if($empty){
			$states_array[''] = "-- select --";
			if($empty == 2){
				$states_array['all'] = "All";
			}
		}
		while($row = db_fetch_object($result)){
			$states_array[$row->code] = $row->name;
		}
		return $states_array;
	}
?>
