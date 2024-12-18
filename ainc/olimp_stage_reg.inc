<?php

// Подтверждение данных пользователя
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli,$error,$message;

    if (($role & 16) == 0) {
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

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
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

// Номер этапа
    if (isset($_REQUEST['stage'])) {
        $stage = $_REQUEST['stage'];
        $_SESSION['stage'] = $stage;
    } elseif (isset($_SESSION['stage'])) {
        $stage = $_SESSION['stage'];
    } else {
        $stage = 0;
    }
// Класс
    if (isset($_REQUEST['sclass']) && is_int($_REQUEST['sclass'])) {
        $sclass = $_REQUEST['sclass'] + 0;
        $_SESSION['sclass'] = $sclass;
    } elseif (isset($_SESSION['sclass'])) {
        $sclass = $_SESSION['sclass'];
    } else {
        $sclass = '';
    }

// Выполнение команд

    $res = $mysqli->query('select t.tp,t.name,t.txt from site_templates as t where t.name in ("cabinet","common","cabinet_stages")');
    while ($row = $res->fetch_assoc()) {
        $w1 = explode('::TEMPLATE::', $row['txt']);
        foreach ($w1 as $v) {
            $w2 = explode('::', $v);
            $w[$w2[0]] = $w2[1];
        }
        $tpl[$row['name']] = $w;
    }

    $olimp_exams = array();
    $res = $mysqli->query('select * from olimp_exams');
    while ($row = $res->fetch_assoc()) {
        $olimp_exams[$row['id']] = $row;
    }
    $olimp_forms = array();
    $res = $mysqli->query('select * from olimp_forms');
    while ($row = $res->fetch_assoc()) {
        $olimp_forms[$row['id']] = $row;
    }
    $olimp_stage_types = array();
    $res = $mysqli->query('select * from olimp_stage_types');
    while ($row = $res->fetch_assoc()) {
        $olimp_stage_types[$row['id']] = $row;
    }
    $olimp_places = array();

    $res = $mysqli->query('select * from olimp_places where id in (' . ($user_places['Access'] ? $user_places['Access'] : '0') . ') order by id');

//$res=$mysqli->query('select * from olimp_places');
    while ($row = $res->fetch_assoc()) {
        $olimp_places[$row['id']] = $row;
    }

    switch ($cmd) {
        case 'reg':
        {    $person = (int)$_REQUEST['person'];
            $res = $mysqli->query('select concat_ws(" ",p.surename, p.name, p.patronymic) as fio from olimp_persons as p where p.id=' . $person);
            if ($row = $res->fetch_assoc()) {
                $fio = $row['fio'];
                $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
                $mysqli->query('CALL addToGroup(' . $person . ', ' . $stage . ', @G)');
                $mysqli->query('UNLOCK TABLES');
                $res = $mysqli->query('select @G');
                if ($row = $res->fetch_row()) {
                    // Выдаем вариант
                    $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $stage);
                    $row2 = $res2->fetch_assoc();
                    if ($row2['form'] == 2 || $row2['form'] == 4 || $row2['form'] == 6) {
                        if (!empty($row2['var_prefix'])) {
                            $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $person . ') AND (stage_id=' . $stage . ')');
                            $row3 = $res3->fetch_row();
                            if ($row3[0] != 'L') {
                                $vars = array();
                                for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                    $vars[$i] = $i;
                                }
                                shuffle($vars);
                                $varnum = $vars[0];
                                $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $person . ') AND (stage_id=' . $stage . ')');
                            }
                        }
                    }

                    $update = '';
                    $update_txt = '';
                    if (isset($_REQUEST['result'])) {
                        $result = trim($_REQUEST['result']);
                        $update = 'task_results="' . $result . '"';
                        $update_txt = '<p>Введен результат: ' . $result;
                    }
                    if (isset($_REQUEST['presence'])) {
                        $presence = trim($_REQUEST['presence']);
                        if (empty($presence)) {
                            $presence = 'R';
                        }
                        $update .= ($update ? ', ' : '') . 'presence="' . $presence . '"';
                        $update_txt .= '<br>Состояние изменено на:' . (($presence == 'Y') ? 'Явка' : (($presence == 'N') ? 'Неявка' : (($presence == 'К') ? 'Удален' : 'Исходное')));
                    }
                    if ($update) {
                        $mysqli->query('update olimp_actions set ' . $update . ' where (stage_id=' . $stage . ') AND (person_id=' . $person . ')');
                    }

                    if ($row2['form'] == 1) {
                        $message = str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($stage,'Участник зарегистрирован в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%','%fio%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0],$fio),
                                $tpl['cabinet_stages']['action_text1ra']
                            ) . $update_txt
                            ),
                            $tpl['cabinet_stages']['modal_message']
                        );
                    }

                    if ($row2['form'] == 2 || $row2['form'] == 4 || $row2['form'] == 6) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($stage,'Участник зарегистрирован в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%','%fio%','%varnum%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0],$fio,$varnum),
                                $tpl['cabinet_stages']['action_text1rza']
                            ) . $update_txt
                            ),
                            $tpl['cabinet_stages']['modal_message']
                        );
                    }
                }
            }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    $ranks = array(1 => 'участник',2 => 'призер 3',3 => 'призер 2',4 => 'победитель');

    switch ($cmd) {
        case 'step2':
        {    $sname = trim($_REQUEST['sname']);
            $name = trim($_REQUEST['name']);
            $pname = trim($_REQUEST['pname']);

            //echo $filter_place;

            if (strlen($sname . $name . $pname) > 0) {
                $whereS = '';
                if ($user_places['Access']) {
                    $whereS = '(p.id in (' . $user_places['Access'] . '))';
                } else {
                    $whereS = '(false)';
                }

                if ($filter_place > 0) {
                    $whereS .= ($whereS ? ' AND ' : '') . '(p.id=' . $filter_place . ')';
                }
                //if ($filter_exam>0) $whereS.=($whereS ? ' AND ' : '').'(e.id='.$filter_exam.')';
                if ($filter_date > -1) {
                    $whereS .= ($whereS ? ' AND ' : '') . '(s.date_exam BETWEEN 
"' . $filter_date . ' 00:00" AND "' . $filter_date . ' 23:59")';
                }

                if ($whereS) {
                    $whereS = ' WHERE ' . $whereS;
                }


                $stages = '';
                $res = $mysqli->query('select s.id as id,e.name as exam,s.classes as classes,p.name as place,st.name as stage,f.name as form,DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_stage_types as st on (st.id=s.stage) left join olimp_forms as f on (f.id=s.form) left join olimp_places as p on (p.id=s.place) ' . $whereS . ' order by s.stage, s.date_exam, s.exam');
                while ($row = $res->fetch_assoc()) {
                    $stages .= str_replace(
                        array('%id%','%exam%','%stage_name%','%classes%','%place%','%form%','%dt%','%selected%'),
                        array($row['id'],    $row['exam'],$row['stage'],$row['classes'],$row['place'],$row['form'],$row['dt'],$row['id'] == $stage ? ' selected' : ''),
                        $tpl['stage_result']['stage_line']
                    );
                }
                $persons = '';
                $where = '(p.school_class>-1)';
                if (strlen($sname) > 0) {
                    $where .= ' AND (p.surename like "' . $sname . '%")';
                }
                if (strlen($name) > 0) {
                    $where .= ' AND (p.name like "' . $name . '%")';
                }
                if (strlen($pname) > 0) {
                    $where .= ' AND (p.patronymic like "' . $pname . '%")';
                }
                if ($sclass > 0) {
                    $where .= ' AND (p.school_class=' . $sclass . ')';
                }

                $res = $mysqli->query('select p.id as id, concat_ws(" ",p.surename, p.name, p.patronymic) as fio, p.school_class as class,p.birthday as birthday, u.id as uid, u.nic as nic from olimp_persons as p left join olimp_users as u on (u.id=p.uid) where ' . $where);
                while ($row = $res->fetch_assoc()) {
                    $res2 = $mysqli->query('select e.id as eid,e.name as exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt, p.name as place,  a.presence, a.task_results, a.result, a.`rank`,s.stage,s.classes from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_exams as e on (e.id=s.exam) left join olimp_places as p on (p.id=s.place) where ((a.presence="Y") or (a.presence="R")) AND (a.person_id=' . $row['id'] . ')  ');
                    $regs[1] = '';
                    $regs[2] = '';
                    $regs[3] = '';
                    $regs[4] = '';
                    while ($row2 = $res2->fetch_assoc()) {
                        $row2['rank'] += 0;
                        $row2['result'] += 0;
                        $regs[$row2['eid']] .= $row2['exam'] . '(' . $row2['stage'] . '; ' . $row2['classes'] . ') ' . $row2['dt'] . ' ' . $row2['place'] . ' ' . $row2['presence'] . ' ' . ($row2['rank'] > 0 ? $ranks[$row2['rank']] : ($row2['result'] > 0 ? $row2['result'] : $row2['task_results'] ) ) . '&nbsp;&nbsp;&nbsp;&nbsp;';
                    }
                    $w = $regs[1] . ($regs[1] ? '<br>' : '') . $regs[2] . ($regs[2] ? '<br>' : '') . $regs[3] . ($regs[3] ? '<br>' : '') . $regs[4];
                    $regs_all = ($w ? '<br>' : '') . $w;
                    $persons .= str_replace(array('%id%','%fio%','%class%','%regs%','%birthday%','%uid%','%nic%'), array($row['id'],$row['fio'],$row['class'],$regs_all,$row['birthday'],$row['uid'],$row['nic']), $tpl['stage_result']['person_line']);
                }


                $body = str_replace(
                    array('%stages%','%persons%'),
                    array($stages,$persons),
                    $tpl['stage_result']['step2']
                );
                break;
            }

        }
        default:
        {    // Первый шаг

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

            $body = str_replace(
                array('%sclass%','%filter_places%','%filter_dates%'),
                array($sclass,$filter_places,$filter_dates),
                $tpl['stage_result']['step1']
            );
        }
    }
    if (!empty($error)) {
        $error = preg_replace('/%error%/', preg_replace(array('/%title%/','/%text%/'), array('Ошибка!',$error), $tpl['common']['error']), $tpl['cabinet']['error']);
    }
    return str_replace(array('%error%','%message%'), array($error,$message), $body);
}

$tpl['stage_result']['step1'] =
'<h1>Введите часть ФИО для поиска участника</h1>%error%%message%
<form action="?" method=post>
<input type="hidden" name="cmd" value="step2">
<table cellspacin=1 cellpadding=2 border=0 width=100%>
<tr><td>Фамилия:</td><td><input type=text name="sname"></td></tr>
<tr><td>Имя:</td><td><input type=text name="name"></td></tr>
<tr><td>Отчество:</td><td><input type=text name="pname"></td></tr>
<tr><td>Класс:</td><td><input type=text name="sclass" value="%sclass%"></td></tr>
<tr><td>Дата:</td><td><select name="filter_date" size=0>%filter_dates%</select></td></tr>
<tr><td>Площадка:</td><td><select name="filter_place" size=0>%filter_places%</select></td></tr>
</table>
<br>
<div style="text-align:center"><input type=submit name="btnSelectPerson" value=" Найти участника "></div>
</form>
<br><br><br><br>
';


$tpl['stage_result']['stage_line'] =
'<option value="%id%"%selected%>%exam%, %stage_name% %dt%,  %form% форма, %classes% классы, площадка %place%</option>
';


$tpl['stage_result']['step2'] =
'<h1>Выберите участника, поток и введите результат</h1>%error%
<form action="?" method=post>
<input type="hidden" name="cmd" value="reg">
<table cellspacin=1 cellpadding=2 border=0 width=100%>
<tr><td>Поток:</td><td><select name="stage" size=0>%stages%</select></td></tr>
<tr><td>Результат:</td><td><input name="result" size=30>&nbsp;&nbsp;Состояние: <select name="presence" size=0><option value="R">Исходное</option><option value="Y">Явка</option><option value="N">Неявка</option><option value="K">Удален</option><option value="W">Ожидание</option></select></td></tr>
%persons%
</table>
<br>
<div style="text-align:center"><input type=submit name="btnSelectPerson" value="  Выбрать участника ">&nbsp;<input type=submit name="btnCancel" value="  Отменить "></div>
</form>
<br><br><br><br>
';


$tpl['stage_result']['person_line'] = '
<tr><td align=center><input type="radio" name="person" value="%id%" id="p%id%"></td><td width=100%><label for="p%id%"><span style="font-weight:bold">%fio% (%birthday%) </span>%class% класс </label>&nbsp;&nbsp;&nbsp; <span>%nic% [%id%/%uid%]</span><span style="font-size:smaller;">%regs%</span></td></tr>
';

$body = build_page();
