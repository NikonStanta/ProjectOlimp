<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');

//header('Content-Type: application/json');

if ($role<3) die('{"status":"noaccess"}');
    
$IdPerson=$_REQUEST['IdPerson']+0;
$lock_edit_id=$_REQUEST['lock_edit_id']+0;
$lock_edit_dt=$mysqli->real_escape_string($_REQUEST['lock_edit_dt']);
$apply_data='';
if (!empty($_REQUEST['confirmed_person']))
{    if (in_array($_REQUEST['confirmed_person'],array('Not','Loaded','Updated','Rejected','Accepted')))
    {    $apply_data.=', confirmed_person="'.$_REQUEST['confirmed_person'].'"';    }
}
if (!empty($_REQUEST['confirmed_school']))
{    if (in_array($_REQUEST['confirmed_school'],array('Not','Loaded','Updated','Rejected','Accepted')))
    {    $apply_data.=', confirmed_school="'.$_REQUEST['confirmed_school'].'"';    }
}
if (!empty($_REQUEST['printed_z']))
{    if (in_array($_REQUEST['printed_z'],array('N','L','Y','R','A')))
    {    $apply_data.=', printed_z="'.$_REQUEST['printed_z'].'"';    }
}
if (!empty($_REQUEST['SchoolDoc']))
{    if (in_array($_REQUEST['SchoolDoc'],array('Not','Loaded','Updated','Rejected','Accepted')))
    {    $apply_data.=', SchoolDoc="'.$_REQUEST['SchoolDoc'].'"';    }
}
if (!empty($_REQUEST['IdentDoc']))
{    if (in_array($_REQUEST['IdentDoc'],array('Not','Loaded','Updated','Rejected','Accepted')))
    {    $apply_data.=', IdentDoc="'.$_REQUEST['IdentDoc'].'"';    }
}
if (!empty($_REQUEST['IdentPhoto']))
{    if (in_array($_REQUEST['IdentPhoto'],array('Not','Loaded','Updated','Rejected','Accepted')))
    {    $apply_data.=', IdentPhoto="'.$_REQUEST['IdentPhoto'].'"';    }
}

if (empty($apply_data)) echo '{"status":"ok"}';

$mysqli->query('UPDATE olimp_persons SET lock_edit_id=0'.$apply_data.' WHERE lock_edit_id='.$lock_edit_id.' AND lock_edit_dt="'.$lock_edit_dt.'" AND id='.$IdPerson);
if ($mysqli->affected_rows<1)
{    // Участник заблокирован другим, не заблокирован или отсутствует
    $res = $mysqli->query('SELECT lock_edit_id, lock_edit_dt, confirmed_person, confirmed_school, printed_z, SchoolDoc, IdentDoc, IdentPhoto FROM olimp_persons WHERE id='.$IdPerson);
    if ($row=$res->fetch_assoc())
    {    die('{"status":"error", "lock_edit_id":"'.$row['lock_edit_id'].'", "lock_edit_dt":"'.$row['lock_edit_dt'].'", "confirmed_person":"'.$row['confirmed_person'].'", "confirmed_school":"'.$row['confirmed_school'].'", "printed_z":"'.$row['printed_z'].'", "IdentDoc":"'.$row['IdentDoc'].'", "IdentPhoto":"'.$row['IdentPhoto'].'", "SchoolDoc":"'.$row['SchoolDoc'].'"}');    }
    else
    {    die('{"status":"missing"}');    }
}

echo '{"status":"ok"}';
