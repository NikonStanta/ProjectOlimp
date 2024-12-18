<?php

// Этапы олимпиады
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli,$sort_groups,$error;

    if (($role & 32) == 0) {
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
            $cmd = '';
            break;
        }
    }


// Формирование кода страницы

    switch ($cmd) {
        case 'EditResults':
        {

        }
        default:
        {    // Выбор этапа работы
            $ppp = array(10,25,50,100,250,500,1000);
            $pppages = '';


            $filter_places = '<option value=-1 ' . ($filter_place == -1 ? ' selected' : '') . '>Все площадки</option>';
            $res = $mysqli->query('select id,name from olimp_places order by id');
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
            $where = '';
            if ($filter_place > 0) {
                $where = '(p.id=' . $filter_place . ')';
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
            $even = 0;
            $stage_dt = '';
            $stage_place = '';
            $stage_exam = '';
            $res = $mysqli->query('select s.id as id,e.name as exam,s.classes as classes,p.name as place,st.name as stage,f.name as form,DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_stage_types as st on (st.id=s.stage) left join olimp_forms as f on (f.id=s.form) left join olimp_places as p on (p.id=s.place)' . $where . ' order by s.stage, s.date_exam, place, exam, classes');
            while ($row = $res->fetch_assoc()) {
                $stages .= str_replace(
                    array('%stage_id%','%exam%','%stage%','%classes%','%place%','%form%','%dt%','%even%'),
                    array($row['id'],    $row['exam'],$row['stage'],$row['classes'],$row['place'],$row['form'],$row['dt'],$even ? 'even' : 'odd'),
                    $tpl['stages']['stage_line']
                );
                $even = !$even;
            }
            $body = str_replace(array('%pppages%','%sort_groups%','%stages%','%filter_places%','%filter_exams%','%filter_stages%','%filter_forms%','%filter_dates%'), array($pppages,$sort_groups,$stages,$filter_places,$filter_exams,$filter_stages,$filter_forms,$filter_dates), $tpl['stages']['stages_list']);
        }
    }

    return str_replace('%error%', $error, $body);
}

$tpl['stages']['stages_list'] =
'%error%
<style>
table#stages {border-top: 1px solid blue;border-left: 1px solid blue; border-collapse:collapse;}
#stages td {border-bottom: 1px solid blue;border-right: 1px solid blue;}
#stages #ttl td {background: #003366; color:white; font-weight:bold; text-align: center;}
#stages .even td {background: #ccffff; color:#000066;}
#stages .odd td {background: #F0ffff; color:#000066;}
</style>
<form action="?" method=post>

<table>
<tr><td>Площадка:&nbsp;</td><td><select name="filter_place" size=0>%filter_places%</select></td><td> Форма:&nbsp;</td><td><select name="filter_form" size=0>%filter_forms%</select></td>
<tr><td>Предмет:&nbsp;</td><td><select name="filter_exam" size=0>%filter_exams%</select></td><td> Этап:&nbsp;</td><td><select name="filter_stage" size=0>%filter_stages%</select></td></tr>
<tr><td> Дата:&nbsp;</td><td><select name="filter_date" size=0>%filter_dates%</select></td><tr>
<tr><td rowspan=2><button type=submit name="btnFilter"><br>Применить фильтр<br><br></button></td></tr>
</table>

<br>
<table cellpadding=2 id="stages">
<tr id=ttl><td></td><td></td><td>Дата</td><td>Площадка</td><td>Предмет</td><td>Классы</td><td>Этап</td><td>Форма</td><td>Действия</td></tr>
%stages%
</table>
<br>
</form>
<div>
    <div style="display:inline-block; vertical-align:top; margin-top:5px; margin-right:10px;">С выбранными:</div> <div class="ob" title="Объединить"></div> <div class="edit massedit" title="Редактировать"></div> <div class="del massdel" title="Удалить"></div>
<br><br><br><br>
';


$tpl['stages']['stage_line'] =
'<tr class=%even%><td><input type="checkbox" value="%stage_id%" class="masscb" /> </td><td>%stage_id%</td><td>%dt%</td><td>%place%</td><td>%exam%</td><td>%classes%</td><td>%stage%</td><td>%form%</td><td><nobr><div style="margin-left:5px;" class="edit" onclick="location.href=\'olimp_stage_edit.html?id=%stage_id%\'"></div> <div class="del" onclick="if(confirm(\'Вы действительно желаете удалить поток?\')) location.href=\'olimp_stage_del.html?id=%stage_id%\'"></div></td></nobr></td></tr>
';




$body = build_page();
