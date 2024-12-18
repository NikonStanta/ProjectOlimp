<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');
if ($role<3) die('{"status":"noaccess"}');

$where='';
if (empty($_SESSION['confirmed_person_filter_type'])) $_SESSION['confirmed_person_filter_type']='Started';

switch ($_SESSION['confirmed_person_filter_type'])
{    case 'All':
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
$llist = array();
$llist['status'] = 'ok';
$llist['persons'] = array();

//echo 'SELECT p.id, CONCAT_WS(" ",p.surename,p.name,p.patronymic) as fio, p.confirmed_person, p.confirmed_school, p.IdentDoc, p.IdentPhoto, p.SchoolDoc, p.olymp_year, p.lock_edit_hash, p.lock_edit_dt FROM olimp_persons as p WHERE '.$where.' ORDER BY p.olymp_year DESC, fio<hr>';


$res = $mysqli->query('SELECT r.id as rec_id, p.id, CONCAT_WS(" ",p.surename,p.name,p.patronymic) as fio, r.score as score, r.stage_id as old_stage, 
       r.status as status, e.name as exam, pr.name as partner, r.lock_edit_id, r.refuse_message as msg, r.exam as exam_code
FROM olimp_persons as p RIGHT JOIN olimp_reculc as r ON p.id = r.person_id LEFT JOIN olimp_partners as pr on r.partner_id = pr.id LEFT JOIN olimp_exams as e on e.id = r.exam
WHERE (p.olymp_year=2025) ORDER BY p.olymp_year DESC, p.id, fio '); //'.$where.' and         s.year=2022 and s.stage=2 and (a.rank in ("1","2", "3", "4")) and s.exam in(1,2,3,4)    and (s.year=2023)

while ($row=$res->fetch_assoc()) {
    if ($row['status'] != 'D') {
        switch ($row['status']) {
            case 'R':
            {
                $row['status'] = 'Заявка подана';
                break;
            }
            case 'N':
            {
                $row['status'] = 'Заявка отклонена';
                break;
            }
            case 'Y':
            {
                $row['status'] = 'Заявка принята';
                break;
            }
        }
        if ($row['msg'] == null) $row['msg'] = '';
        $llist['persons'][] = $row;
    }
}

//header('Content-Type: application/json');
echo json_encode($llist);
