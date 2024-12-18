<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');

//header('Content-Type: application/json');

if ($role<3) die('{"status":"noaccess"}');
    
$IdPerson=(int)$_REQUEST['IdPerson'];
$lock_edit_id=(int)$_REQUEST['lock_edit_id'];
$exam = (int)$_REQUEST['IdExam'];
$id = (int)$_REQUEST['IdReculc'];
$action = $_REQUEST['Action'] == 'accept';
$lock_edit_dt=$mysqli->real_escape_string($_REQUEST['lock_edit_dt']);
$stage_old = (int)$_REQUEST['StageOld'];
$apply_data='';

if ($action) {
    $stage = (int)$_REQUEST['Stage'];
    $rank = (int)$_REQUEST['Rank'];
    $score = (int)$_REQUEST['Score'];
    $apply_data .= 'status="Y", `refuse_message`=NULL, score=' . $score . ', stage_id=' . $stage;
} else {
    if (!empty($_REQUEST['comment'])) {
        $apply_data.='refuse_message="'.$_REQUEST['comment'].'", ';
    }
    $apply_data .= 'status="N"';
}

$res=$mysqli->query('SELECT * FROM olimp_reculc WHERE lock_edit_id='.$lock_edit_id.' AND lock_edit_dt="'.$lock_edit_dt.'" AND id=' . $id);
if (!$res) {
    // Участник заблокирован другим, не заблокирован или отсутствует
    $res = $mysqli->query('SELECT lock_edit_id, lock_edit_dt FROM olimp_reculc WHERE id=' . $id);
    if ($row=$res->fetch_assoc())
    {    die('{"status":"error", "lock_edit_id":"'.$row['lock_edit_id'].'", "lock_edit_dt":"'.$row['lock_edit_dt'].'"}');    }
    else
    {    die('{"status":"missing"}');    }
}

$mysqli->query('UPDATE olimp_reculc SET '.$apply_data.' WHERE lock_edit_id='.$lock_edit_id.' AND lock_edit_dt="'.$lock_edit_dt.'" AND id=' . $id);

// Процедура добавления в группы и потоки

if ($action) {
    if ($stage == $stage_old) {
        $res = $mysqli->query("SELECT presence FROM olimp_actions WHERE person_id=$IdPerson AND stage_id=$stage");
        if ($res && $row=$res->fetch_row())
        {
            $apply_data='';
            if ($row[0] != 'Y') {
                $apply_data .= "presence='Y', ";
            }
            $mysqli->query("UPDATE olimp_actions SET $apply_data result=$score, stage_id=$stage, `rank`=$rank WHERE person_id=$IdPerson AND stage_id=$stage");
        }
    } else {
        $mysqli->query("UPDATE olimp_actions SET presence='D' WHERE person_id=$IdPerson AND stage_id=$stage_old");
        $res = $mysqli->query('select concat_ws(" ",p.surename, p.name, p.patronymic) as fio from olimp_persons as p where p.id=' . $IdPerson);
        if ($res && $row = $res->fetch_assoc()) {
            $fio = $row['fio'];
            $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
            $mysqli->query('CALL addToGroup(' . $IdPerson . ', ' . $stage . ', @G)');
            $mysqli->query('UNLOCK TABLES');
            $res = $mysqli->query('select @G');
            if ($res && $row = $res->fetch_row()) {
                // Выдаем вариант
                $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $stage);
                $row2 = $res2->fetch_assoc();
                if ($row2['form'] == 2 || $row2['form'] == 4 || $row2['form'] == 6) {
                    if (!empty($row2['var_prefix'])) {
                        $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $IdPerson . ') AND (stage_id=' . $stage . ')');
                        $row3 = $res3->fetch_row();
                        if ($row3[0] != 'L') {
                            $vars = array();
                            for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                $vars[$i] = $i;
                            }
                            shuffle($vars);
                            $varnum = $vars[0];
                            $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $IdPerson . ') AND (stage_id=' . $stage . ')');
                        }
                    }
                }
                $mysqli->query('UPDATE olimp_actions SET `rank`=' . $rank . ', `result`=' . $score . ', presence="Y" WHERE (stage_id=' . $stage . ') AND (person_id=' . $IdPerson . ')');   //
            }
        }
    }
} else {
    $mysqli->query('UPDATE olimp_reculc SET '.$apply_data.' WHERE lock_edit_id='.$lock_edit_id.' AND lock_edit_dt="'.$lock_edit_dt.'" AND id=' . $id);
    $mysqli->query("UPDATE olimp_actions SET presence='N' WHERE person_id=$IdPerson AND stage_id=$stage_old");
}
$mysqli->query('UPDATE olimp_reculc SET lock_edit_id=0 WHERE lock_edit_id='.$lock_edit_id.' AND lock_edit_dt="'.$lock_edit_dt.'" AND id=' . $id);

echo json_encode([
    'status' => 'ok',
    'type' => $action ? 'accepted' : 'rejected',
    'score' => isset($score) ? $score : null,
    'stage' => isset($stage) ? $stage : $stage_old,
]) ;
