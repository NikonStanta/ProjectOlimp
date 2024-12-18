<?php

// $strRespond = [];
// $strRespond['error'] = 1;
// $strRespond['error_text'] = 'page type: ' . $_POST['page'];
include_once('init_mobile.php');
include_once('build_page' . $PAGE['type'] . ".php");

// $strRespond['error_text'] .= 'page_id = ' . $_POST['stage_id'];
// $strRespond['error_text'] .= 'user pid = ' . $USER_INFO['pid'];
// $strRespond['page_type'] = $PAGE['type'];


$strRespond = build_page(0, 0);
if(is_array($strRespond)){
  $strRespond['page_type'] = $PAGE['type'];
}


// $strRespond['Listeners']['count'] = 1;
// $strRespond['Listeners'][0]['type'] = 'registration';
// $strRespond['Listeners'][0]['elem_id'] = 'btnStageAction3102';


echo json_encode($strRespond);