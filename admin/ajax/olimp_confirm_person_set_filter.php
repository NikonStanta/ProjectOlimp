<?php
$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');
if ($role<3) die('{"status":"noaccess"}');
	

if (empty($_REQUEST['confirmed_person_filter_type']))	
{	$_SESSION['confirmed_person_filter_type']='Loaded';	}
else
{	$_SESSION['confirmed_person_filter_type']=$_REQUEST['confirmed_person_filter_type'];	}


if (empty($_REQUEST['confirmed_person_filter_surename'])) 
{	$_SESSION['confirmed_person_filter_surename']='';	}
else
{	$_SESSION['confirmed_person_filter_surename']=$_REQUEST['confirmed_person_filter_surename']; }

if (empty($_REQUEST['confirmed_person_filter_name'])) 
{	$_SESSION['confirmed_person_filter_name']='';	}
else
{	$_SESSION['confirmed_person_filter_name']=$_REQUEST['confirmed_person_filter_name']; }

if (empty($_REQUEST['confirmed_person_filter_patronymic'])) 
{	$_SESSION['confirmed_person_filter_patronymic']='';	}
else
{	$_SESSION['confirmed_person_filter_patronymic']=$_REQUEST['confirmed_person_filter_patronymic']; }
?>