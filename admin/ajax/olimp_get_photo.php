<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');

//header('Content-Type: application/json');
if (($role & 64)==0)
{	return die('{"status":"noaccess"}');
}

$_works_folder='../../docs/applications/';

function mime_type2ext($tp)
{	switch ($tp)
	{
	case 'application/pdf':		return '.pdf';
	case 'application/zip':		return '.zip';
	case 'application/gzip':	return '.gzip';
	case 'application/x-rar':	return '.rar';
	case 'image/jpeg': 			
	case 'image/pjpeg':			return '.jpeg';
	case 'image/png': 			return '.png';
	}
	return '.unknown';
}
$IdPerson=$_REQUEST['IdPerson']+0;

$filename=$_works_folder.'identphoto'.$IdPerson;

if (file_exists($filename))
{	$mime=mime_content_type($filename);
	header('Content-Type: '.$mime);
	//header('Content-disposition: attachment; filename='.$work_filename.mime_type2ext($mime));
	header('Content-Length: ' . filesize($filename));
	if (ob_get_level()) 
	{
		ob_end_clean();
	}
	readfile($filename);
	
}
else
{	$filename=$_works_folder.'identphoto_no';
	$mime=mime_content_type($filename);
	header('Content-Type: '.$mime);
	//header('Content-disposition: attachment; filename='.$work_filename.mime_type2ext($mime));
	header('Content-Length: ' . filesize($filename));
	if (ob_get_level()) 
	{
		ob_end_clean();
	}
	readfile($filename);
	
}

?>