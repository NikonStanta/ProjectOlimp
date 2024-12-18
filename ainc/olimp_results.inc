<?php

// Преобразование результатов

//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu;

    $user_right = check_user_right(3);

    if (!$user_right['Access']) {
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


    if (isset($_REQUEST['btnCalcSum'])) {
        $cmd = 'calcSum';
    }

    if (isset($_REQUEST['btnCalcSumAll'])) {
        $cmd = 'calcSumAll';
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

    if (empty($cmd) && ($stage > 0)) {
        $cmd = 'EditResults';
    }

    if (empty($stage) || ($cmd == 'Return')) {
        if ($cmd != 'calcSumAll') {
            $cmd = '';
        }
        $page = 0;
        $stage = 0;
    }

// Выполнение команд

    switch ($cmd) {
        case 'UpdateResults':
        {
            $results = $_POST['r'];
            $checked = $_POST['c'];
            $ranks = $_POST['rank'];
            if (is_array($checked)) {
                $person_ids = implode(',', array_keys($checked));
                $res = $mysqli->query('select person_id,task_results,rank from olimp_actions where (stage_id=' . $stage . ') AND (person_id in (' . $person_ids . '))');
                while ($row = $res->fetch_row()) {
                    $old_result[$row[0]] = $row[1];
                    $old_rank[$row[0]] = $row[2];
                }
                foreach ($checked as $person_id => $result) {
                    $task_result = trim(implode(',', $results[$person_id]));
                    if (!preg_match('/[0-9]/', $task_result)) {
                        $task_result = '';
                    }
                    if (($task_result != $old_result[$person_id]) || ($old_rank[$person_id] != $ranks[$person_id])) {
                        $mysqli->query('update olimp_actions set ' . ($task_result != '' ? 'presence="Y", ' : '') . ' task_results="' . $task_result . '", rank="' . $ranks[$person_id] . '" where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
                        $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values (' . $_SESSION['uid'] . ',1, "(person_id=' . $person_id . ') AND (stage_id=' . $stage . ')", "' . $old_result[$person_id] . '", "' . $task_result . '")');
                    }
                }
            }
            $cmd = 'EditResults';
            break;
        }
        case 'calcSum':
        {    $res = $mysqli->query('select s.task_count as task_count, s.task_weight as task_weight from olimp_stages as s where s.id=' . $stage);
            $stage_data = $res->fetch_assoc();
            $r = explode(',', $stage_data['task_weight']);
            for ($i = 0; $i < $stage_data['task_count']; $i++) {
                if (!empty($r[$i])) {
                        $task_weight[$i] = $r[$i];
                } else {
                    $task_weight[$i] = 1;
                }
            }
            $res = $mysqli->query('select a.person_id,a.task_results as results from olimp_actions as a where a.stage_id=' . $stage);
            while ($row = $res->fetch_assoc()) {
                $r = explode(',', $row['results']);
                $task_results = '';
                $psum = 0;
                for ($i = 0; $i < $stage_data['task_count']; $i++) {
                    $psum += $r[$i] * $task_weight[$i];
                }
                $mysqli->query('update olimp_actions as a set result=' . $psum . '  where (a.stage_id=' . $stage . ') AND (a.person_id=' . $row['person_id'] . ')');
            }
            $cmd = 'EditResults';
            break;
        }
        case 'calcSumAll':
        {    echo 'Пересчитываем все суммы';
            $res2 = $mysqli->query('select s.id as stage_id,s.task_count as task_count from olimp_stages as s');
            while ($stage_data = mysql_fetch_assoc($res2)) {
                $res = $mysqli->query('select a.person_id,a.task_results as results from olimp_actions as a where a.stage_id=' . $stage_data['stage_id']);
                while ($row = $res->fetch_assoc()) {
                    $r = explode(',', $row['results']);
                    $task_results = '';
                    $psum = 0;
                    for ($i = 0; $i < $stage_data['task_count']; $i++) {
                        $psum += $r[$i];
                    }
                    $mysqli->query('update olimp_actions as a set result=' . $psum . '  where (a.stage_id=' . $stage_data['stage_id'] . ') AND (a.person_id=' . $row['person_id'] . ')');
                }
            }
            $cmd = '';
            break;
        }
        case 'UpdatePresence':
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
                        $mysqli->query('update olimp_actions set presence="Y" where (stage_id=' . $stage . ') AND (person_id=' . $person_id . ')');
    //                    $mysqli->query('insert into admin_log (uid,cat,object,old_val,new_val) values ('.$_SESSION['uid'].',1, "(person_id='.$person_id.') AND (stage_id='.$stage.')", "'.$old_result[$person_id].'", "'.$task_result.'")');
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
                $res = $mysqli->query('select concat_ws(" ",p.surename,p.name,p.patronymic) as fio, a.person_id as person, a.presence as presence, a.task_results as results, a.result, a.`rank` from olimp_actions as a left join olimp_persons as p on (p.id=a.person_id) where a.stage_id=' . $stage . ' order by fio limit ' . ($num - 1) . ',' . $pppage);
                while ($row = $res->fetch_assoc()) {
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
                        default: {    $presence = '-';
                        }
                    }
                    $row['rank'] += 0;

                    $rank = '<select name=rank[' . $row['person'] . '] size=-1 id="' . $row['person'] . '" class="Rank">' .
                    '<option value=0 ' . ($row['rank'] < 1 ? ' selected' : '' ) . '>не определено</option>' .
                    '<option value=1 ' . ($row['rank'] == 1 ? ' selected' : '' ) . '>участник</option>' .
                    '<option value=2 ' . ($row['rank'] == 2 ? ' selected' : '' ) . '>призер 3</option>' .
                    '<option value=3 ' . ($row['rank'] == 3 ? ' selected' : '' ) . '>призер 2</option>' .
                    '<option value=4 ' . ($row['rank'] == 4 ? ' selected' : '' ) . '>победитель</option>' .
                    '</select>';

                    $lines .= str_replace(array('%num%','%fio%','%presence%','%results%','%person%','%sum%','%rank%'), array($num,$row['fio'] . ' (' . $row['person'] . ')',$presence,$task_results,$row['person'],$row['result'],$rank), $tpl['stage_result']['result_line' . ($even ? 1 : 2)]);
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
            $stages = '';

            $res = $mysqli->query('select s.id as id,e.name as exam,s.classes as classes,p.name as place,st.name as stage,f.name as form,DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_stage_types as st on (st.id=s.stage) left join olimp_forms as f on (f.id=s.form) left join olimp_places as p on (p.id=s.place) order by s.stage, s.date_exam, s.exam');
            while ($row = $res->fetch_assoc()) {
                $stages .= str_replace(
                    array('%stage%','%exam%','%stage_name%','%classes%','%place%','%form%','%dt%'),
                    array($row['id'],    $row['exam'],$row['stage'],$row['classes'],$row['place'],$row['form'],$row['dt']),
                    $tpl['stage_result']['stage_line']
                );
            }
            $body = str_replace(array('%pppages%','%sort_groups%','%stages%'), array($pppages,$sort_groups,$stages), $tpl['stage_result']['select_stage']);
        }
    }

    return str_replace('%error%', $error, $body);
}

$tpl['stage_result']['select_stage'] =
'<h1>Выберите этап олимпиады для ввода/коррекции результатов</h1>%error%

<form action="?" method=post>
<select name="sort_group" size=0>%sort_groups%</select><br><br>
Выводить <select name="pppage" size=0> %pppages% </select> участников на страницу.
%stages%
<br>
<div style="text-align:center"><input type=submit name="btnSelectStage" value="  Перейти к вводу/коррекции оценок">&nbsp;&nbsp;<input type=submit name="btnCalcSumAll" value="  Подсчитать все суммы "></div>
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



// }); 

$( ".Rank" ).change(

function () {
    var cbid="cr"+this.id; 
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
<tr bgcolor="#CCCCff"><td>#</td><td width=100%>ФИО</td><td>Явка</td>%tasks%<td>Сумма</td><td>Статус</td></tr>
%lines%
</table>
<div style="text-align:center"><input type=submit name="btnUpdateResults" value="  Внести изменения">&nbsp;&nbsp;&nbsp;<input type=submit name="btnCalcSum" value="  Подсчитать сумму "></div>
</form>
<div style="float:right;">Страницы: %pages%</div>
<a href="?cmd=Return">&laquo;&laquo; Вернуться к списку этапов</a><div style="clear:both;">&nbsp;</div>
';


$tpl['stage_result']['result_line1'] = '<tr class="ResultRow EvenRow"  id="r%person%"><td>%num%</td><td>%fio%</td><td>%presence%</td>%results%<td><b>%sum%</b></td><td>%rank%</td><td><input type="checkbox" name="c[%person%]" id="cr%person%"></td></tr>';
$tpl['stage_result']['result_line2'] = '<tr class="ResultRow" id="r%person%"><td>%num%</td><td>%fio%</td><td>%presence%</td>%results%<td><b>%sum%</b></td><td>%rank%</td><td><input type="checkbox" name="c[%person%]" id="cr%person%"></td></tr>';
$tpl['stage_result']['task_cell'] = '<td><input type="text" name="r[%person%][%num%]" value="%value%" size=3 class="task_cell"></td>';


$body = build_page();
