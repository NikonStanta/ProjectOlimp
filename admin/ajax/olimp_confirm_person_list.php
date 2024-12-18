<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');
if ($role<3) die('{"status":"noaccess"}');
	
$where='';
if (empty($_SESSION['confirmed_person_filter_type'])) $_SESSION['confirmed_person_filter_type']='Started'; 

switch ($_SESSION['confirmed_person_filter_type'])
{	case 'All':
		break;
	case 'Unconfirmed':
		$where='(p.confirmed_person="N" OR p.confirmed_school="N")';
		break;
	case 'Rejected':
		$where='(p.IdentDoc="Rejected" OR p.IdentPhoto="Rejected" OR p.SchoolDoc="Rejected")';
		break;		
	case 'Accepted':
		$where='(p.IdentDoc="Accepted" AND p.IdentPhoto="Accepted" AND p.SchoolDoc="Accepted")';
		break;
	case 'Confirmed':
		$where='(p.confirmed_person="Y" AND p.confirmed_school="Y")';
		break;
	case 'Loaded':	
		$where='(p.IdentDoc="Loaded" OR p.IdentPhoto="Loaded" OR p.SchoolDoc="Loaded" OR p.IdentDoc="Updated" OR p.IdentPhoto="Updated" OR p.SchoolDoc="Updated")';
	default:
		$where='(p.IdentDoc<>"Not" OR p.IdentPhoto<>"Not" OR p.SchoolDoc<>"Not")';
		break;
}

if (!empty($_SESSION['confirmed_person_filter_surename'])) $where.=($where ? ' AND ' : ' ').'(p.surename LIKE "'.$mysqli->real_escape_string($_SESSION['confirmed_person_filter_surename']).'%")';
if (!empty($_SESSION['confirmed_person_filter_name'])) $where.=($where ? ' AND ' : ' ').'(p.name LIKE "'.$mysqli->real_escape_string($_SESSION['confirmed_person_filter_name']).'%")';
if (!empty($_SESSION['confirmed_person_filter_patronymic'])) $where.=($where ? ' AND ' : ' ').'(p.patronymic LIKE "'.$mysqli->real_escape_string($_SESSION['confirmed_person_filter_patronymic']).'%")';

if (empty($where)) die('{"status":"error"}');

$list='{"status":"ok","persons":[';
$notfirst=false;

//echo 'SELECT p.id, CONCAT_WS(" ",p.surename,p.name,p.patronymic) as fio, p.confirmed_person, p.confirmed_school, p.IdentDoc, p.IdentPhoto, p.SchoolDoc, p.olymp_year, p.lock_edit_hash, p.lock_edit_dt FROM olimp_persons as p WHERE '.$where.' ORDER BY p.olymp_year DESC, fio<hr>';


$res = $mysqli->query('SELECT p.id, CONCAT_WS(" ",p.surename,p.name,p.patronymic) as fio, GROUP_CONCAT(" ",a.presence) as presence, p.confirmed_person, p.confirmed_school, p.printed_z, p.IdentDoc, p.IdentPhoto, p.SchoolDoc, p.olymp_year, p.lock_edit_id, p.lock_edit_dt FROM olimp_persons as p LEFT JOIN olimp_actions as a ON p.id = a.person_id  LEFT JOIN olimp_stages as s ON a.stage_id = s.id WHERE (p.olymp_year=2025) GROUP BY p.id ORDER BY p.olymp_year DESC, FIELD(p.printed_z,  "L", "Y", "R", "A"), fio '); //'.$where.' and         s.year=2022 and s.stage=2 and (a.rank in ("1","2", "3", "4")) and s.exam in(1,2,3,4)    and (s.year=2023)  
while ($row=$res->fetch_assoc())
{	$list.=($notfirst ? ', ' : '').'{"pid":"'.$row['id'].'", "fio":"'.$row['fio'].'", "presence":"'.$row['presence'].'","confirmed_person":"'.$row['confirmed_person'].'", "confirmed_school":"'.$row['confirmed_school'].'", "printed_z":"'.$row['printed_z'].'", "IdentDoc":"'.$row['IdentDoc'].'", "IdentPhoto":"'.$row['IdentPhoto'].'", "SchoolDoc":"'.$row['SchoolDoc'].'", "olymp_year":"'.$row['olymp_year'].'", "lock_edit_id":"'.$row['lock_edit_id'].'", "lock_edit_dt":"'.$row['lock_edit_dt'].'"}';
	$notfirst=true;
}
$list.=']}';

//header('Content-Type: application/json');

echo $list;
?>