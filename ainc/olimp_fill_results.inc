<?php

// Ввод результатов по шифрам
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli,$error;

    if (($role & 128) == 0) {
        return $tpl['noaccess'];
    }

    $user_places = getUserPlaces($uid);

    if (empty($user_places['Access'])) {
        return $tpl['noaccess'];
    }


    if (isset($_REQUEST['cmd'])) {
        $cmd = $_REQUEST['cmd'];
    } else {
        $cmd = '';
    }

    if (isset($_REQUEST['btnList'])) {
        $cmd = 'List';
    }


// Фильтр по дате
    if (isset($_POST['filter_date'])) {
        $filter_date = trim($_POST['filter_date']);
        if (empty($filter_date)) {
            $filter_date = -1;
        }
        $_SESSION['filter_date'] = $filter_date;
    } elseif (isset($_SESSION['filter_date'])) {
        $filter_date = $_SESSION['filter_date'];
    } else {
        $filter_date = -1;
    }

// Фильтр по площадкам
    if (isset($_POST['filter_place'])) {
        $filter_place = $_POST['filter_place'] + 0;
        if (empty($filter_place)) {
            $filter_place = -1;
        }
        $_SESSION['filter_place'] = $filter_place;
    } elseif (isset($_SESSION['filter_place'])) {
        $filter_place = $_SESSION['filter_place'];
    } else {
        $filter_place = -1;
    }

// Фильтр по предметам
    if (isset($_POST['filter_exam'])) {
        $filter_exam = $_POST['filter_exam'] + 0;
        if (empty($filter_exam)) {
            $filter_exam = -1;
        }
        $_SESSION['filter_exam'] = $filter_exam;
    } elseif (isset($_SESSION['filter_exam'])) {
        $filter_exam = $_SESSION['filter_exam'];
    } else {
        $filter_exam = -1;
    }

// Фильтр по типам этапов
    if (isset($_POST['filter_stage'])) {
        $filter_stage = $_POST['filter_stage'] + 0;
        $_SESSION['filter_stage'] = $filter_stage;
    } elseif (isset($_SESSION['filter_stage'])) {
        $filter_stage = $_SESSION['filter_stage'];
    } else {
        $filter_stage = -1;
    }

// Фильтр по формам этапов
    if (isset($_POST['filter_form'])) {
        $filter_form = $_POST['filter_form'] + 0;
        if (empty($filter_form)) {
            $filter_form = -1;
        }
        $_SESSION['filter_form'] = $filter_form;
    } elseif (isset($_SESSION['filter_form'])) {
        $filter_form = $_SESSION['filter_form'];
    } else {
        $filter_form = -1;
    }


    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $stage = 0;
        $_SESSION['stage'] = $stage;
    }
    if (isset($_REQUEST['btnFilter'])) {
        $cmd = '';
        $stage = 0;
        $_SESSION['stage'] = $stage;
    }


    if ($cmd == 'Return') {
        $cmd = '';
        $page = 0;
        $stage = 0;
    }


    if (isset($_REQUEST['btnUpdateUnPresence'])) {
        $cmd = 'UpdateUnPresence';
    }
    if (isset($_REQUEST['btnUpdatePresence'])) {
        $cmd = 'UpdatePresence';
    }
    if (isset($_REQUEST['btnClearResults'])) {
        $cmd = 'ClearResults';
    }
// Выполнение команд

// echo 'cmd='.$cmd;

    switch ($cmd) {
        case 'UpdateResults':
        {
            $results = $_POST['r'];
            $checked = array_key_exists('c', $_POST) ? $_POST['c'] : '';
            $group_id = $_POST['group_id'];

            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                $res = $mysqli->query('select person_id,task_results from olimp_actions where (group_id=' . $group_id . ') AND (person_id in (' . $person_ids . '))');
                while ($row = $res->fetch_row()) {
                    $old_result[$row[0]] = $row[1];
                }

                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if ($task_result != $old_result[$person_id]) {
                        $mysqli->query('update olimp_actions set ' . ($task_result != '' ? 'presence="Y", ' : '') . ' task_results="' . $task_result . '", `rank`="0" where (group_id=' . $group_id . ') AND (person_id=' . $person_id . ')');

                    //$mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
                    }
                }
            }
            $cmd = '';
            break;
        }

        case 'UpdateUnPresence':
        {    $results = $_POST['r'];
            $checked = $_POST['c'];
            $group_id = $_REQUEST['group_id'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    $task_result = preg_replace('/^0|,0|\ /', '', $task_result);
                    echo $task_result;
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if ($task_result == '') {
                        $mysqli->query('update olimp_actions set presence="N", result=0, `rank`="0" where (group_id=' . $group_id . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
                    }
                }
            }
            $cmd = 'fill';
            break;
        }
        case 'UpdateTechUnPresence':
        {    $results = $_POST['r'];
            $checked = $_POST['c'];
            $group_id = $_REQUEST['group_id'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    $task_result = preg_replace('/^0|,0|\ /', '', $task_result);
                    echo $task_result;
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if ($task_result == '') {
                        $mysqli->query('update olimp_actions set presence="P", result=0, `rank`="0" where (group_id=' . $group_id . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
                    }
                }
            }
            $cmd = 'fill';
            break;
        }

        case 'UpdatePresence':
        {    $results = $_POST['r'];
            $checked = $_POST['c'];
            $group_id = $_REQUEST['group_id'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if ($task_result == '') {
                        $mysqli->query('update olimp_actions set presence="Y", `rank`="0" where (group_id=' . $group_id . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
                    }
                }
            }
            $cmd = 'fill';
            break;
        }
        case 'ClearResults':
        {    $results = $_POST['r'];
            $checked = $_POST['c'];
            $group_id = $_REQUEST['group_id'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $mysqli->query('update olimp_actions set presence="R",task_results="", result=0, `rank`="0" where (group_id=' . $group_id . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
                }
            }
            $cmd = 'fill';
            break;
        }
    }


// Формирование кода страницы

    $where = '';
    if ($user_places['Access']) {
        $where = '(pl.id in (' . $user_places['Access'] . '))';
    } else {
        $where = '(false)';
    }
    if ($filter_place > 0) {
        $where .= ($where ? ' AND ' : '') . '(pl.id=' . $filter_place . ')';
    }
    if ($filter_exam > 0) {
        $where .= ($where ? ' AND ' : '') . '(e.id=' . $filter_exam . ')';
    }
    if ($filter_form > 0) {
        $where .= ($where ? ' AND ' : '') . '(s.form=' . $filter_form . ')';
    }
    if ($filter_stage > -1) {
        $where .= ($where ? ' AND ' : '') . '(s.stage=' . $filter_stage . ')';
    }
    if ($filter_date > -1) {
        $where .= ($where ? ' AND ' : '') . '(s.date_exam BETWEEN 
"' . $filter_date . ' 00:00" AND "' . $filter_date . ' 23:59")';
    }

    if ($where) {
        $where = ' WHERE ' . $where;
    }


    switch ($cmd) {
        case 'fill':
        {    $group_id = $_REQUEST['group_id'] + 0;
            $res = $mysqli->query('select s.cipher,g.cipher,s.task_count from olimp_groups as g left join olimp_stages as s on (s.id=g.stage) where (s.cipher<>"") AND (g.cipher>0) AND (g.id=' . $group_id . ')');
            if ($row = $res->fetch_row()) {
                $tasks = '';
                $cipher = $row[0] . $row[1];
                $task_count = $row[2];
                for ($i = 1; $i <= $task_count; $i++) {
                    $tasks .= '<td style="text-align:center;">' . $i . '</td>';
                }
                $lines = '';
                $even = false;
                $res = $mysqli->query('select a.cipher as cipher,a.task_results as results,a.presence as presence,a.person_id as person_id from olimp_actions as a where a.group_id=' . $group_id . ' order by cipher');
                while ($row = $res->fetch_assoc()) {
                    $r = explode(',', $row['results']);
                    $task_results = '';
                    for ($i = 1; $i <= $task_count; $i++) {
                        if ($i > count($r)) {
                                        $task_results .= str_replace(
                                            array('%person_id%','%num%','%value%'),
                                            array($row['person_id'],$i,''),
                                            $tpl['res']['task_cell']
                                        );
                        } else {
                            $task_results .= str_replace(
                                array('%person_id%','%num%','%value%'),
                                array($row['person_id'],$i,$r[$i - 1]),
                                $tpl['res']['task_cell']
                            );
                        }
                    }
                    switch ($row['presence']) {
                        case 'Y': {    $presence = 'явка';
                            break;
                        }
                        case 'N': {    $presence = 'неявка';
                            break;
                        }
                        case 'D': {    $presence = 'отказ';
                            break;
                        }
                        case 'W': {    $presence = 'ожидание';
                            break;
                        }
                        case 'P': {    $presence = 'тех.неявка';
                            break;
                        }
                        case 'A': {    $presence = 'аннулир.';
                            break;
                        }
                        default: {    $presence = '-';
                        }
                    }
                    $lines .= str_replace(
                        array('%cipher%','%presence%','%results%','%person%'),
                        array($cipher . '-' . $row['cipher'],$presence,$task_results,$row['person_id']),
                        $tpl['res']['fill_line' . ($even ? 1 : 2)]
                    );
                    $even = !$even;
                }
                $body = str_replace(
                    array('%cipher%','%lines%','%tasks%','%group_id%'),
                    array($cipher, $lines, $tasks, $group_id),
                    $tpl['res']['fill']
                );
            } else {
                $body = $tpl['res']['wrong_group'];
            }
            break;
        }

        case 'list1':
        case 'list2':
        {    $lvl = $_REQUEST['lvl'] + 0;
            $group_ids = array();
            $w = '';

            switch ($lvl) {
                case 1:
                {    $dt = preg_replace('/([0-9]{1,2})\.([0-9]{1,2})\.([0-9]{2,4})/', '$3-$2-$1', trim($_REQUEST['dt']));
                    $w = '(s.date_exam BETWEEN "' . $dt . ' 00:00" AND "' . $dt . ' 23:59")';
                    break;
                }
                case 2:
                {    $dt = preg_replace('/([0-9]{1,2})\.([0-9]{1,2})\.([0-9]{2,4})/', '$3-$2-$1', trim($_REQUEST['dt']));
                    $place_id = $_REQUEST['place_id'] + 0;
                    $w = '(s.date_exam BETWEEN "' . $dt . ' 00:00" AND "' . $dt . ' 23:59") AND (s.place=' . $place_id . ')';
                    break;
                }
                case 3:
                {    $dt = preg_replace('/([0-9]{1,2})\.([0-9]{1,2})\.([0-9]{2,4})/', '$3-$2-$1', trim($_REQUEST['dt']));
                    $place_id = $_REQUEST['place_id'] + 0;
                    $exam_id = $_REQUEST['exam_id'] + 0;
                    $w = '(s.date_exam BETWEEN "' . $dt . ' 00:00" AND "' . $dt . ' 23:59") AND (s.place=' . $place_id . ') AND (s.exam=' . $exam_id . ')';
                    break;
                }
                case 4:
                {    $group_id = $_REQUEST['group_id'] + 0;
                    $w = '(g.id=' . $group_id . ')';
                    break;
                }
            }

            if (empty($w)) {
                $cmd = '';
                break;
            }

            //echo 'select g.id from olimp_stages as s left join olimp_groups as g on (g.stage=s.id) where '.$w.' order by s.cipher,g.cipher';

            $res = $mysqli->query('select g.id from olimp_stages as s left join olimp_groups as g on (g.stage=s.id) where ' . $w . ' order by s.cipher,g.cipher');

            while ($row = $res->fetch_row()) {
                $group_ids[$row[0]] = $row[0];
            }

            $body = '';
            foreach ($group_ids as $gid) {
                if ($gid < 1) {
                    continue;
                }
                $res = $mysqli->query('select s.cipher,g.cipher,s.task_count from olimp_groups as g left join olimp_stages as s on (s.id=g.stage) where (s.cipher<>"") AND (g.cipher>0) AND (g.id=' . $gid . ')');
                if ($row = $res->fetch_row()) {
                    $cipher = $row[0] . $row[1];
                    $task_count = $row[2];
                    $tasks = '';
                    for ($i = 1; $i <= $task_count; $i++) {
                        $tasks .= str_replace('%num%', $i, $tpl['res']['task_cell_head']);
                    }
                    $lines = '';

                    $res = $mysqli->query('select a.cipher as cipher,a.task_results as results,a.presence as presence,a.person_id as person_id from olimp_actions as a where a.group_id=' . $gid . ' order by cipher');
                    while ($row = $res->fetch_assoc()) {
                        $r = explode(',', $row['results']);
                        $task_results = '';
                        for ($i = 1; $i <= $task_count; $i++) {
                            $task_results .= str_replace(
                                array('%person_id%','%num%','%value%'),
                                array($row['person_id'],$i,$r[$i - 1]),
                                $tpl['res'][$cmd . '_task_cell']
                            );
                        }
                        switch ($row['presence']) {
                            case 'Y': {    $presence = 'явка';
                                break;
                            }
                            case 'N': {    $presence = 'неявка';
                                break;
                            }
                            case 'D': {    $presence = 'отказ';
                                break;
                            }
                            case 'W': {    $presence = 'ожидание';
                                break;
                            }
                            case 'A': {    $presence = 'аннулир.';
                                break;
                            }
                            default: {    $presence = '-';
                            }
                        }
                        $lines .= str_replace(
                            array('%cipher%','%presence%','%results%','%person%'),
                            array($cipher . '-' . $row['cipher'],$presence,$task_results,$row['person_id']),
                            $tpl['res'][$cmd . '_line']
                        );
                    }
                    $body .= str_replace(
                        array('%cipher%','%lines%','%tasks%','%group_id%'),
                        array($cipher, $lines, $tasks, $group_id),
                        $tpl['res'][$cmd]
                    );
                }
            }
            echo $tpl['res'][$cmd . '_header'] . $body;
            die();
        break;
        }
        default:
        {    // Выбор группы

            $filter_places = '<option value=-1 ' . ($filter_place == -1 ? ' selected' : '') . '>Все площадки</option>';
            $res = $mysqli->query('select id,name from olimp_places where id in (' . ($user_places['Access'] ? $user_places['Access'] : '0') . ') order by id');
            while ($row = $res->fetch_row()) {
                $filter_places .= '<option value=' . $row[0] . ' ' . ($filter_place == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            $filter_dates = '<option value=-1 ' . ($filter_date == -1 ? ' selected' : '') . '>Все даты</option>';
            $res = $mysqli->query('select DISTINCT DATE_FORMAT(date_exam,"%Y-%m-%d") as d1, DATE_FORMAT(date_exam,"%d.%m.%Y")  from olimp_stages order by date_exam DESC');
            while ($row = $res->fetch_row()) {
                $filter_dates .= '<option value="' . $row[0] . '" ' . ($filter_date == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            $filter_exams = '<option value=-1 ' . ($filter_exam == -1 ? ' selected' : '') . '>Все предметы</option>';
            $res = $mysqli->query('select id,name from olimp_exams order by id');
            while ($row = $res->fetch_row()) {
                $filter_exams .= '<option value=' . $row[0] . ' ' . ($filter_exam == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }
            $filter_stages = '<option value=-1 ' . ($filter_stage == -1 ? ' selected' : '') . '>Все этапы</option>';
            $res = $mysqli->query('select id,name from olimp_stage_types order by id');
            while ($row = $res->fetch_row()) {
                $filter_stages .= '<option value=' . $row[0] . ' ' . ($filter_stage == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }
            $filter_forms = '<option value=-1 ' . ($filter_form == -1 ? ' selected' : '') . '>Все формы</option>';
            $res = $mysqli->query('select id,name from olimp_forms order by id');
            while ($row = $res->fetch_row()) {
                $filter_forms .= '<option value=' . $row[0] . ' ' . ($filter_form == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            $list = '';

            $stage_dt = '';
            $stage_place = '';
            $stage_exam = '';

            if (empty($where)) {
                $where = ' WHERE (s.cipher<>"") AND s.year=2023';
            } else {
                $where .= ' AND (s.cipher<>"")  AND s.year=2023';
            }

            $res = $mysqli->query('select e.name as exam,pl.name as place, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt, s.cipher as scipher,s.id as sid, pl.id as place_id, e.id as exam_id from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_forms as f on (f.id=s.form) left join olimp_places as pl on (pl.id=s.place) ' . $where . ' order by dt,place,exam');
            while ($row = $res->fetch_assoc()) {
                if ($stage_dt != $row['dt']) {
                    $stage_dt = $row['dt'];
                    $stage_place = '';
                    $stage_exam = '';
                    $list .= str_replace('%dt%', $stage_dt, $tpl['res']['date_line']);
                }
                if ($stage_place != $row['place']) {
                    $stage_place = $row['place'];
                    $stage_exam = '';
                    $list .= str_replace(
                        array('%place%','%place_id%','%dt%'),
                        array($stage_place,$row['place_id'],$stage_dt),
                        $tpl['res']['place_line']
                    );
                }
                if ($stage_exam != $row['exam']) {
                    $stage_exam = $row['exam'];
                    $list .= str_replace(
                        array('%exam%','%exam_id%','%place_id%','%dt%'),
                        array($stage_exam,$row['exam_id'],$row['place_id'],$stage_dt),
                        $tpl['res']['exam_line']
                    );
                }

                $res2 = $mysqli->query('select g.id as gid, g.cipher as gcipher from olimp_groups as g where (g.stage=' . $row['sid'] . ') and (g.cipher>0) order by gcipher');
                while ($row2 = $res2->fetch_assoc()) {
                    $total = 0;
                    $absent = 0;
                    $entered = 0;
                    $res3 = $mysqli->query('select a.presence as presence, a.task_results as res from olimp_actions as a where a.group_id=' . $row2['gid']);
                    while ($row3 = $res3->fetch_assoc()) {
                        $total++;
                        //if (($row3['presence']=='N')||($row3['presence']=='A')||($row3['presence']=='W')) $absent++;
                        if (($row3['presence'] != 'Y') && ($row3['presence'] != 'L')) {
                            $absent++;
                        }
                        if (strlen($row3['res']) > 0) {
                            $entered++;
                        }
                    }
                    $list .= str_replace(
                        array('%group_id%','%total%','%absent%','%entered%','%cipher%','%ttl%'),
                        array($row2['gid'],$total,$absent,$entered,$row['scipher'] . $row2['gcipher'],$total - $absent),
                        $tpl['res']['group_line']
                    );
                }
            }


            $body = str_replace(
                array('%list%','%filter_places%','%filter_exams%','%filter_stages%','%filter_forms%','%filter_dates%'),
                array($list,$filter_places,$filter_exams,$filter_stages,$filter_forms,$filter_dates),
                $tpl['res']['list']
            );

        }
    }

    return str_replace('%error%', $error, $body);
}



$tpl['res']['list'] =
'%error%
<style>
table#stages {border-top: 1px solid blue;border-left: 1px solid blue; border-collapse:collapse;}
#stages td {border-bottom: 1px solid blue;border-right: 1px solid blue; padding:4px 8px;}
.date {font-size:24px; font-weight:bold; }
.place {font-size:13px; font-weight:bold; color:blue;}
.exam {font-size:13px; font-weight:bold; color:#333;}
</style>
<form action="?" method=post>
<table>
<tr><td>Площадка:&nbsp;</td><td><select name="filter_place" size=0>%filter_places%</select></td>
<td> Форма:&nbsp;</td><td><select name="filter_form" size=0>%filter_forms%</select></td>
<td rowspan=3><button type=submit name="btnFilter"><br>Применить фильтр<br><br></button></td></tr>

<tr><td>Предмет:&nbsp;</td><td><select name="filter_exam" size=0>%filter_exams%</select></td>
<td> Этап:&nbsp;</td><td><select name="filter_stage" size=0>%filter_stages%</select></td></tr>

<tr><td>Дата:&nbsp;</td><td><select name="filter_date" size=0>%filter_dates%</select></td></tr>
</table>
<br>
<table cellpadding=2 id="stages">
%list%
</table>
<br>
</form>
<br><br>


<br><br>
';


$tpl['res']['date_line'] =
'<tr><td class="date">%dt%</td><td><a href="?cmd=list1&lvl=1&dt=%dt%" target="lists">[Проверочные ведомости]</a>&nbsp;<a href="?cmd=list2&lvl=1&dt=%dt%" target="lists">[Cверочные ведомости]</a></td></tr>
';
$tpl['res']['place_line'] =
'<tr><td class="place">Площадка %place%</td><td><a href="?cmd=list1&lvl=2&dt=%dt%&place_id=%place_id%" target="lists">[Проверочные ведомости]</a>&nbsp;<a href="?cmd=list2&lvl=2&dt=%dt%&place_id=%place_id%" target="lists">[Cверочные ведомости]</a></td></tr>
';
$tpl['res']['exam_line'] =
'<tr><td class="exam">%exam%</td><td><a href="?cmd=list1&lvl=3&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%" target="lists">[Проверочные ведомости]</a>&nbsp;<a href="?cmd=list2&lvl=3&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%" target="lists">[Cверочные ведомости]</a></td></tr>
';
$tpl['res']['group_line'] =
'<tr><td><a href="?cmd=fill&group_id=%group_id%">%cipher%&nbsp;&nbsp;&nbsp;&nbsp;(введено %entered% из %ttl%)</a> неявок, удаленных и т.п. %absent%</td><td><a href="?cmd=list1&lvl=4&group_id=%group_id%" target="lists">[Проверочные ведомости]</a>&nbsp;<a href="?cmd=list2&lvl=4&group_id=%group_id%" target="lists">[Cверочные ведомости]</a></td></tr>
';






$tpl['res']['fill'] = '<h2>Группа %cipher%</h2>


<script>

$(function() {


$( ".ResultRow" ).hover(

function () {
    $(this).addClass("HoverRow");
  },
  function () {
    $(this).removeClass("HoverRow");
  }
);


$( ".ResultRow" ).focusin(

function () {
    $(this).addClass("TaskFocus");
 }
);

$( ".ResultRow" ).focusout(

function () {
    $(this).removeClass("TaskFocus");
  }
);


$( ".ResultRow" ).keypress(

function () {
    var cbid="c"+this.id; 
if (this.id!=cbid)
    $("#"+cbid).attr("checked", "checked");
    
  }
);



}); 

    
</script>

<style>
.EvenRow
{  background:#E0FFE0;
}
.HoverRow, .HoverRow input
{  background:#FFE0E0;
}

.TaskFocus
{  background:#99FF99;
    font-weight:bold;
}
.TaskFocus input
{  background:#FFFFFF;
}

</style>

<a href="?cmd=Return">&laquo;&laquo; Вернуться к списку групп</a>

<form action="?" method=post>
<input type="hidden" name="cmd" value="UpdateResults">
<input type="hidden" name="group_id" value="%group_id%">
<table cellspacin=1 cellpadding=2 border=0>
<tr bgcolor="#CCCCff"><td style="width:150">Шифр</td><td style="width:100">Явка</td>%tasks%</tr>
%lines%
</table>
<br><br>
<div style="text-align:left;"><input type=submit name="btnUpdateResults" value="  Внести изменения" style="color:green; font-weight:bold;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=submit name="btnUpdateUnPresence" value="  Ввести неявки ">&nbsp;&nbsp;<input type=submit name="btnUpdatePresence" value="  Ввести явки ! ">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=submit name="btnClearResults" value="  Очистить выбранные " style="color:red; font-weight:bold;"></div>
</form>
<a href="?cmd=Return">&laquo;&laquo; Вернуться к списку групп</a><div style="clear:both;">&nbsp;</div>
';


$tpl['res']['fill_line1'] = '<tr class="ResultRow EvenRow"  id="r%person%"><td>%cipher%</td><td>%presence%</td>%results%<td><input type="checkbox" name="c[%person%]" id="cr%person%"></td></tr>';
$tpl['res']['fill_line2'] = '<tr class="ResultRow" id="r%person%"><td>%cipher%</td><td>%presence%</td>%results%<td><input type="checkbox" name="c[%person%]" id="cr%person%"></td></tr>';
$tpl['res']['task_cell'] = '<td><input type="text" name="r[%person_id%][%num%]" value="%value%" size=3 class="task_cell"></td>';


$tpl['res']['wrong_group'] = '<h2 style="color:red;">Несуществующая или незашифрованная группа.</h2>';




$tpl['res']['list1_header'] = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
<title>Олимпиада школьников "Надежда энергетики". Проверочные ведомоти.</title>
<style>
table 
{    border-collapse:collapse;
    border-top: 1px solid black;
    border-left: 1px solid black;
}

td 
{    border-bottom: 1px solid black;
    border-right: 1px solid black;
    padding:4px;
    text-align:center;
}
td.first 
{    text-align:left !important;
}
</style>

';


$tpl['res']['list2_header'] = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
<title>Олимпиада школьников "Надежда энергетики". Сверочные ведомоти.</title>
<style>
table 
{    border-collapse:collapse;
    border-top: 1px solid black;
    border-left: 1px solid black;
}

td 
{    border-bottom: 1px solid black;
    border-right: 1px solid black;
    padding:4px;
    text-align:center;
}
td.first 
{    text-align:left !important;
}
</style>

';



$tpl['res']['list1'] = '<h1>Проверочная ведомость</h1><h2>Группа %cipher%</h2>
<table cellspacin=1 cellpadding=2 border=0>
<tr bgcolor="#CCCCff"><td style="width:5cm"  class="first">Шифр</td><td style="width:3cm">Явка</td>%tasks%</tr>
%lines%
</table>
<div style="page-break-after: always;"></div>
';

$tpl['res']['list2'] = '<h1>Сверочная ведомость</h1><h2>Группа %cipher%</h2>
<table cellspacin=1 cellpadding=2 border=0>
<tr bgcolor="#CCCCff"><td style="width:5cm" class="first">Шифр</td><td style="width:3cm">Явка</td>%tasks%</tr>
%lines%
</table>
<div style="page-break-after: always;"></div>
';


$tpl['res']['list1_line'] = '<tr><td class="first">%cipher%</td><td>%presence%</td>%results%</tr>';
$tpl['res']['list2_line'] = '<tr><td class="first">%cipher%</td><td>%presence%</td>%results%</tr>';
$tpl['res']['list1_task_cell'] = '<td>&nbsp;</td>';
$tpl['res']['list2_task_cell'] = '<td>%value%</td>';
$tpl['res']['task_cell_head'] = '<td style="width:1.5cm">%num%</td>';



$body = build_page();
