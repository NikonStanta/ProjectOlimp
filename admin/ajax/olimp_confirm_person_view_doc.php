<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');

//header('Content-Type: application/json');

if ($role<3) die('Нет доступа');

if (empty($_REQUEST['IdPerson'])) die('Запрошенный документ не найден E1');
$IdPerson=(int)$_REQUEST['IdPerson'];
if (empty($_REQUEST['docType'])) die('Запрошенный документ не найден  E2');
if (!in_array($_REQUEST['docType'],array('application','schooldoc','identdoc','identphoto','reculc_application'))) die('Запрошенный документ не найден  E3');
$docType=$_REQUEST['docType'];
$docOther=!empty($_REQUEST['docOther']) ? $_REQUEST['docOther'] : '';
$examCode = !empty($_REQUEST['exam_code']) ? $_REQUEST['exam_code'] : '';

$_doc_folder='../../docs/applications/';
function mime_type2ext($tp)
{    switch ($tp)
    {
    case 'application/pdf':        return '.pdf';
    case 'application/zip':        return '.zip';
    case 'application/gzip':    return '.gzip';
    case 'application/x-rar':    return '.rar';
    case 'image/jpeg':             
    case 'image/pjpeg':            return '.jpeg';
    case 'image/png':             return '.png';
    }
    return '.unknown';
}
/*
$lock_edit_id=$_REQUEST['lock_edit_id']+0;
$lock_edit_dt=$mysqli->real_escape_string($_REQUEST['lock_edit_dt']);
$mysqli->query('UPDATE olimp_persons SET lock_edit_id='.$lock_edit_id.', lock_edit_dt="'.$lock_edit_dt.'" WHERE lock_edit_id=0 AND id='.$IdPerson);
*/

$doc_filename=$docType.$IdPerson.($examCode ? '_'.$examCode : '').($docOther ? '_'.$docOther : '');
$doc_fullname=$_doc_folder.$doc_filename;    
echo $doc_fullname;

if (file_exists($doc_fullname))
{    $mime=mime_content_type($doc_fullname);
    header('Content-Type: '.$mime);
    #header('Content-disposition: attachment; filename='.$doc_filename.mime_type2ext($mime));
    header('Content-Length: ' . filesize($doc_fullname));
    if (ob_get_level()) 
    {
        ob_end_clean();
    }
    readfile($doc_fullname);
}
else
{    die('Запрошенный документ не найден  E4');
}

?>