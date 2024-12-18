<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');

//header('Content-Type: application/json');

if ($role<3) die('{"status":"noaccess"}');
	
$IdPerson=$_REQUEST['IdPerson']+0;
$lock_edit_id=$_REQUEST['lock_edit_id']+0;
$lock_edit_dt=$mysqli->real_escape_string($_REQUEST['lock_edit_dt']);

$mysqli->query('UPDATE olimp_persons SET lock_edit_id=0 WHERE lock_edit_id='.$lock_edit_id.' AND lock_edit_dt="'.$lock_edit_dt.'" AND id='.$IdPerson);
if ($mysqli->affected_rows<1)
{	// Участник заблокирован другим, не заблокирован или отсутствует
	$res = $mysqli->query('SELECT lock_edit_id, lock_edit_dt FROM olimp_persons WHERE id='.$IdPerson);
	if ($row=$res->fetch_assoc())
	{	die('{"status":"error", "lock_edit_id":"'.$row['lock_edit_id'].'", "lock_edit_dt":"'.$row['lock_edit_dt'].'"}');	}
	else
	{	die('{"status":"missing"}');	}
}

echo '{"status":"ok"}';
?>