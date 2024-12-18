<?php

// Подтверждение данных пользователя
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli,$error;

    if (($role & 256) == 0) {
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

    if (isset($_REQUEST['btnUpdatePresence'])) {
        $cmd = 'UpdatePresence';
    }
    if (isset($_REQUEST['btnUpdateUnPresence'])) {
        $cmd = 'UpdateUnPresence';
    }
    if (isset($_REQUEST['btnUpdateTechUnPresence'])) {
        $cmd = 'UpdateTechUnPresence';
    }

    if (isset($_REQUEST['btnAnnulate'])) {
        $cmd = 'Annulate';
    }

// # Страница
    if (isset($_REQUEST['page'])) {
        $page = $_REQUEST['page'];
        $_SESSION['page'] = $page;
    } elseif (isset($_SESSION['page'])) {
        $page = $_SESSION['page'];
    } else {
        $page = 1;
    }


// # Участников на странице
    if (isset($_POST['pppage'])) {
        $page = 1;
        $_SESSION['page'] = $page;
        $pppage = $_POST['pppage'];
        $_SESSION['pppage'] = $pppage;
    } elseif (isset($_SESSION['pppage'])) {
        $pppage = $_SESSION['pppage'];
    } else {
        $pppage = 25;
        $_SESSION['pppage'] = $pppage;
    }

// 50
// Номер этапа
    if (isset($_REQUEST['stage'])) {
        $stage = $_REQUEST['stage'];
        $_SESSION['stage'] = $stage;
        $page = 1;
        $_SESSION['page'] = $page;
    } elseif (isset($_SESSION['stage'])) {
        $stage = $_SESSION['stage'];
    } else {
        $stage = 0;
        $page = 1;
        $_SESSION['page'] = $page;
    }

// Способ группировки/сортировки
    if (isset($_POST['sort_group'])) {
        $sort_group = $_POST['sort_group'];
        $_SESSION['sort_group'] = $sort_group;
        $page = 1;
        $_SESSION['page'] = $page;
    } elseif (isset($_SESSION['sort_group'])) {
        $sort_group = $_SESSION['sort_group'];
    } else {
        $sort_group = 0;
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

// Рабочий год
    $year = 0;
    if (isset($_POST['year'])) {
        $year = (int)$_POST['year'];
    } elseif (isset($_SESSION['year'])) {
        $year = $_SESSION['year'];
    }

    if (empty($year)) {
        $res = $mysqli->query('select value from vars_string where name="current_year"');
        list($year) = $res->fetch_row();
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

    if (empty($cmd) && ($stage > 0)) {
        $cmd = 'EditResults';
    }

    if (empty($stage) || ($cmd == 'Return')) {
        $cmd = '';
        $page = 0;
        $stage = 0;
    }

// Выполнение команд

    switch ($cmd) {
        case 'UpdateResults':
        {
            $results = $_POST['r'];
            $checked = $_POST['c'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                $res = $mysqli->query('select person_id,task_results from olimp_actions where (stage_id=' . $stage . ') AND (person_id in (' . $person_ids . '))');
                while ($row = $res->fetch_row()) {
                    $old_result[$row[0]] = $row[1];
                }
                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if ($task_result != $old_result[$person_id]) {
                        $mysqli->query('update olimp_actions set ' . ($task_result != '' ? 'presence="Y", ' : '') . ' task_results="' . $task_result . '" where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
                        $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values (' . $_SESSION['uid'] . ',1, "(person_id=' . $person_id . ') AND (stage_id=' . $stage . ')", "' . $old_result[$person_id] . '", "' . $task_result . '")');
                    }
                }
            }
            $cmd = 'EditResults';
            break;
        }

        case 'UpdatePresence':
        {    $results = $_POST['r'];
            $checked = $_POST['c'];
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
                        $mysqli->query('update olimp_actions set presence="Y" where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
                    }
                }
            }
            $cmd = 'EditResults';
            break;
        }
        case 'UpdateUnPresence':
        {    $checked = $_POST['c'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $mysqli->query('update olimp_actions set presence="N", task_results=""  where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
                }
            }
            $cmd = 'EditResults';
            break;
        }
        case 'UpdateTechUnPresence':
        {    $checked = $_POST['c'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $mysqli->query('update olimp_actions set presence="P", task_results=""  where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
                }
            }
            $cmd = 'EditResults';
            break;
        }

        case 'Annulate':
        {    $results = $_POST['r'];
            $checked = $_POST['c'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if ($task_result == '') {
                        $mysqli->query('update olimp_actions set presence="A" where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.' - Annulated!")');
                    }
                }
            }
            $cmd = 'EditResults';
            break;
        }
    }


// Формирование кода страницы

    switch ($cmd) {
        case 'SelectPerson':
        {    $where = '';

        }
        case 'EditResults':
        {    $res = $mysqli->query('select s.task_count as task_count,e.name as exam,s.classes as classes,p.name as place,st.name as stage,f.name as form,DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_stage_types as st on (st.id=s.stage) left join olimp_forms as f on (f.id=s.form) left join olimp_places as p on (p.id=s.place) where s.id=' . $stage);
            $stage_data = $res->fetch_assoc();
            $res = $mysqli->query('select count(person_id) from olimp_actions where stage_id=' . $stage);
            if ($row = $res->fetch_row()) {
                $action_count = $row[0];
                $page_count = ceil($action_count / $pppage);
                if ($page > $page_count) {
                    $page = $page_count;
                }
                $pages = '';
                for ($i = 1; $i <= $page_count; $i++) {
                    if ($i == $page) {
                                $pages .= '&nbsp;<span style="font-weight:bold; color:#FF3333">[' . $i . ']</span>';
                    } else {
                            $pages .= '&nbsp;<a href="?page=' . $i . '">[' . $i . ']</a>';
                    }
                }
                $lines = '';
                $tasks = '';
                $num = ($page - 1) * $pppage + 1;
                for ($i = 1; $i <= $stage_data['task_count']; $i++) {
                    $tasks .= '<td style="text-align:center;">' . $i . '</td>';
                }
                $even = false;
                $res = $mysqli->query('select concat_ws(" ",p.surename,p.name,p.patronymic) as fio, a.person_id as person, a.presence as presence, a.task_results as results from olimp_actions as a left join olimp_persons as p on (p.id=a.person_id) where a.stage_id=' . $stage . ' order by fio limit ' . ($num - 1) . ',' . $pppage);
                while ($res && $row = $res->fetch_assoc()) {
                    $r = explode(',', $row['results']);
                    $task_results = '';
                    for ($i = 1; $i <= $stage_data['task_count']; $i++) {
                        $task_results .= str_replace(
                            array('%person%','%num%','%value%'),
                            array($row['person'],$i,$r[$i - 1]),
                            $tpl['stage_result']['task_cell']
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
                        case 'P': {    $presence = 'тех.неявка';
                            break;
                        }
                        default: {    $presence = '-';
                        }
                    }
                    $lines .= str_replace(array('%num%','%fio%','%presence%','%results%','%person%'), array($num,$row['fio'] . ' (' . $row['person'] . ')',$presence,$task_results,$row['person']), $tpl['stage_result']['result_line' . ($even ? 1 : 2)]);
                    $num++;
                    $even = !$even;
                }
                $body = str_replace(
                    array('%stage%','%exam%','%stage_name%','%classes%','%place%','%form%','%dt%','%pages%','%lines%','%tasks%'),
                    array($stage, $stage_data['exam'],$stage_data['stage'],$stage_data['classes'],$stage_data['place'],$stage_data['form'],$stage_data['dt'],$pages,$lines,$tasks),
                    $tpl['stage_result']['by_alphabet']
                );
                break;
            }

        }
        default:
        {    // Выбор этапа работы
            $ppp = array(10,25,50,100,250,500,1000);
            $pppages = '';
            foreach ($ppp as $p) {
                $pppages .= '<option' . ($pppage == $p ? ' selected' : '') . '>' . $p . '</option>';
            }
            $sgs = array(1 => 'Сортировать по алфавиту, группы не учитывать, выводить постранично',2 => 'Выводить по группам, внутри групп сортировать по алфавиту',);
            $sort_groups = '';
            foreach ($sgs as $sgk => $sgv) {
                $sort_groups .= '<option' . ($sort_group == $sgk ? ' selected' : '') . '>' . $sgv . '</option>';
            }

            $filter_places = '<option value=-1 ' . ($filter_place == -1 ? ' selected' : '') . '>Все площадки</option>';
            $res = $mysqli->query('select id,name from olimp_places where id in (' . ($user_places['Access'] ? $user_places['Access'] : '0') . ') order by id');
            while ($row = $res->fetch_row()) {
                $filter_places .= '<option value=' . $row[0] . ' ' . ($filter_place == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
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
            $filter_dates = '<option value=-1 ' . ($filter_date == -1 ? ' selected' : '') . '>Все даты</option>';
            $res = $mysqli->query('select DISTINCT DATE_FORMAT(date_exam,"%Y-%m-%d") as d1, DATE_FORMAT(date_exam,"%d.%m.%Y")  from olimp_stages order by date_exam DESC');
            while ($row = $res->fetch_row()) {
                $filter_dates .= '<option value="' . $row[0] . '" ' . ($filter_date == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }


            $stages = '';
        //$where='';
            $where = '(s.year="' . $year . '")';
            if ($user_places['Access']) {
                $where .= ' AND (p.id in (' . $user_places['Access'] . '))';
            } else {
                $where = '(false)';
            }
            if ($filter_place > 0) {
                $where .= ($where ? ' AND ' : '') . '(p.id=' . $filter_place . ')';
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
            $res = $mysqli->query('select s.id as id,e.name as exam,s.classes as classes,p.name as place,st.name as stage,f.name as form,DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_stage_types as st on (st.id=s.stage) left join olimp_forms as f on (f.id=s.form) left join olimp_places as p on (p.id=s.place)' . $where . ' order by s.stage, s.date_exam, s.exam');
            while ($row = $res->fetch_assoc()) {
                $stages .= str_replace(
                    array('%stage%','%exam%','%stage_name%','%classes%','%place%','%form%','%dt%'),
                    array($row['id'],    $row['exam'],$row['stage'],$row['classes'],$row['place'],$row['form'],$row['dt']),
                    $tpl['stage_result']['stage_line']
                );
            }
            $body = str_replace(array('%pppages%','%sort_groups%','%stages%','%filter_places%','%filter_exams%','%filter_stages%','%filter_forms%','%filter_dates%','%year%'), array($pppages,$sort_groups,$stages,$filter_places,$filter_exams,$filter_stages,$filter_forms,$filter_dates,$year), $tpl['stage_result']['select_stage']);
        }
    }

    return str_replace('%error%', $error, $body);
}

$tpl['stage_result']['select_stage'] =
'<h1>Выберите этап олимпиады для ввода/коррекции результатов</h1>%error%
<form action="?" method=post>
<select name="sort_group" size=0>%sort_groups%</select><br><br>
<table>
<tr><td colspan=5>Год проведения:&nbsp;<input name="year" type="text" value="%year%" size=10></td></tr>
<tr><td>Площадка:&nbsp;</td><td><select name="filter_place" size=0>%filter_places%</select></td><td> Форма:&nbsp;</td><td><select name="filter_form" size=0>%filter_forms%</select></td>
<td rowspan=2><button type=submit name="btnFilter"><br>Применить фильтр<br><br></button></td></tr>
<tr><td>Предмет:&nbsp;</td><td><select name="filter_exam" size=0>%filter_exams%</select></td><td> Этап:&nbsp;</td><td><select name="filter_stage" size=0>%filter_stages%</select></td></tr>
<tr><td> Дата:&nbsp;</td><td><select name="filter_date" size=0>%filter_dates%</select></td></tr>
</table>
Выводить <select name="pppage" size=0> %pppages% </select> участников на страницу.
%stages%
<br>
<div style="text-align:center"><input type=submit name="btnSelectStage" value="  Перейти к вводу/коррекции оценок"></div>
</form>
<br><br><br><br>
';


$tpl['stage_result']['stage_line'] =
'<div style="background:#E0FFE0; padding:4px; margin:4px;">
<input type="radio" name="stage" value="%stage%" id="stage%stage%">
<label for="stage%stage%">%exam%, %stage_name% %dt%,  %form% форма, %classes% классы, площадка %place%</label>
</div>
';

$tpl['stage_result']['by_alphabet'] = '<h2>%exam%, %stage_name% %dt%,  %form% форма, %classes% классы, площадка %place%</h2>


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

<a href="?cmd=Return">&laquo;&laquo; Вернуться к списку этапов</a>
<div style="float:right;">Страницы: %pages%</div><div style="clear:both;">&nbsp;</div>
<form action="?" method=post>
<input type="hidden" name="cmd" value="UpdateResults">
<table cellspacin=1 cellpadding=2 border=0 width=100%>
<tr bgcolor="#CCCCff"><td>#</td><td width=100%>ФИО</td><td>Явка</td>%tasks%</tr>
%lines%
</table>
<div style="text-align:center"><input type=submit name="btnUpdateResults" value="  Внести изменения">&nbsp;&nbsp;<input type=submit name="btnUpdateUnPresence" value="  Ввести неявки ">&nbsp;&nbsp;<input type=submit name="btnUpdatePresence" value="  Ввести явки ! ">&nbsp;&nbsp;&nbsp;&nbsp;<button type=submit name="btnUpdateTechUnPresence" value="1"><span style="color:#CC0000; font-weight:bold;">!! Техническая неявка  !!</span></button>&nbsp;&nbsp;<button type=submit name="btnAnnulate" value="1"><span style="color:#CC0000; font-weight:bold;">!! Аннулировать работы  !!</span></button></div>
</form>
<div style="float:right;">Страницы: %pages%</div>
<a href="?cmd=Return">&laquo;&laquo; Вернуться к списку этапов</a><div style="clear:both;">&nbsp;</div>
';


$tpl['stage_result']['result_line1'] = '<tr class="ResultRow EvenRow"  id="r%person%"><td>%num%</td><td>%fio%</td><td>%presence%</td>%results%<td><input type="checkbox" name="c[%person%]" id="cr%person%"></td></tr>';
$tpl['stage_result']['result_line2'] = '<tr class="ResultRow" id="r%person%"><td>%num%</td><td>%fio%</td><td>%presence%</td>%results%<td><input type="checkbox" name="c[%person%]" id="cr%person%"></td></tr>';
$tpl['stage_result']['task_cell'] = '<td><input type="text" name="r[%person%][%num%]" value="%value%" size=3 class="task_cell"></td>';


$body = build_page();
