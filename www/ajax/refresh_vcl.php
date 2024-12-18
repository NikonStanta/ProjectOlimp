<?php
$_connection_charset='utf8';
include_once('../../inc/init.inc');
include_once('../../inc/page.inc');
include_once('../../inc/cabinet.inc');
include_once('../../inc/logonout.inc');
include_once('../../inc/cabinet2.inc');
$stage_id=$_REQUEST['stageid']+0;
if ($stage_id<1) die('{"status":"error"}');
$vclink='';
$varpwd='';
check_vcl_param($stage_id,$vclink,$varpwd,$delay);

header('Content-Type: application/json');
echo '{"status":"ok", "vclink":"'.$vclink.'", "varpwd":"'.$varpwd.'", "delay":"'.$delay.'"}';
?>