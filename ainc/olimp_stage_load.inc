<?php

// Этапы олимпиады
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$user_IdOrg,$mysqli,$filter_region,$error;

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

    if (isset($_REQUEST['btnList'])) {
        $cmd = 'List';
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

// Фильтр по состоянию
    if (isset($_POST['filter_presence'])) {
        $filter_presence = $_POST['filter_presence'] + 0;
        if (empty($filter_presence)) {
            $filter_presence = -1;
        }
        $_SESSION['filter_presence'] = $filter_presence;
    } elseif (isset($_SESSION['filter_presence'])) {
        $filter_presence = $_SESSION['filter_presence'];
    } else {
        $filter_presence = -1;
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

//echo $_POST['filter_form'].'<hr>';

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

// Определим связанные с организатором регионы

    $orgRegions = '';
    $orgRegionsArray = array();
    $orgRegionsAll = false;

    $res = $mysqli->query('SELECT id,name FROM olimp_orgs');
    while ($row = $res->fetch_row()) {
        if ($row[0] == $user_IdOrg) {
            $orgName = $row[1];
        }
        $orgNames[$row[0]] = $row[1];
    }

    if (empty($orgName)) {    //return $tpl['noaccess'];
    }

    $res = $mysqli->query('SELECT IdOlimpOrg,IdRegion FROM olimp_org_regions');
    while ($row = $res->fetch_row()) {
        $orgRegionsArray[$row[0]][$row[1]] = $row[1];
        if (($row[0] == $user_IdOrg) && ($row[1] == 0)) {
            $orgRegionsAll = true;
        }
    }

    if ($orgRegionsAll) {
    // Фильтр по регионам

        if (isset($_POST['filter_region'])) {
            $filter_region = $_POST['filter_region'] + 0;
            $_SESSION['filter_region'] = $filter_region;
        } elseif (isset($_SESSION['filter_region'])) {
            $filter_region = $_SESSION['filter_region'];
        } elseif ($filter_region == $user_IdOrg) {
            $filter_region = 0; // Все регионы кроме партнеров
        } else {
            $filter_region = -1; // Все регионы
        }

        $orgRegions = '';
        if ($filter_region > 0) {
            if (array_key_exists($filter_region, $orgRegionsArray)) {
                foreach ($orgRegionsArray[$filter_region] as $IdRegion) {
                    $orgRegions .= ($orgRegions ? ',' : '') . $IdRegion;
                }
            } else {
                $orgRegions = -2;
            }
        } elseif ($filter_region == 0) {
            foreach ($orgRegionsArray as $_orgRegions) {
                foreach ($_orgRegions as $IdRegion) {
                    $orgRegions .= ($orgRegions ? ',' : '') . $IdRegion;
                }
            }
        }
    } else {
        foreach ($orgRegionsArray[$user_IdOrg] as $IdRegion) {
            $orgRegions .= ($orgRegions ? ',' : '') . $IdRegion;
        }
    }

    $_SESSION['orgRegions'] = $orgRegions;




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

    if ($cmd == 'Return') {
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

    if ((($filter_presence == 3) || ($filter_presence == 4)) && ($filter_form != 2)  && ($filter_form != 4) && ($filter_form != 6)) {
        $filter_form = 2;
    }

    $where = 's.year="' . $year . '" ';
    if ($user_places['Access']) {
        $where .= ' AND (pl.id in (' . $user_places['Access'] . '))';
    } else {
        $where .= ' AND (false)';
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
    switch ($filter_presence) {
        case 1: {$where .= ($where ? ' AND ' : '') . '(a.presence = "Y")';
            break;
        }
        case 2: {$where .= ($where ? ' AND ' : '') . '(a.presence in ("N","D"))';
            break;
        }
        case 3: {$where .= ($where ? ' AND ' : '') . '(a.presence = "L")';
            break;
        }
        case 4: {$where .= ($where ? ' AND ' : '') . '(a.presence = "R")';
            break;
        }
        case 5: {$where .= ($where ? ' AND ' : '') . '(a.presence = "W")';
            break;
        }
    }
    switch ($filter_region) {
        case -1: {    break;
        }
        case 0:  {    $where .= ' AND (adr.region NOT IN (' . $orgRegions . ') )';
            break;
        }
        default: {    $where .= ' AND (adr.region IN (' . $orgRegions . ') )';
            break;
        }
    }




    if ($where) {
        $where = ' WHERE ' . $where;
    }


    switch ($cmd) {
        case 'List':
        {    $res = $mysqli->query('select e.name as exam,p.school_class as school_class,pl.name as place,stp.name as stage,f.name as form, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt, concat_ws(" ",p.surename,p.name,p.patronymic) as fio, DATE_FORMAT(p.birthday, "%d.%m.%Y" ) as bdt, 

CONCAT_WS(" ",IF(LENGTH(sc.school_type_ex)>0, concat_ws(" ",IFNULL(st.name,""),sc.school_type_ex), IFNULL(st.name,"")) ,sc.name) as school,
CONCAT_WS(" ",sr.socr,sr.name,sa.city,sa.street,sa.house,IF(LENGTH(sa.building)>0,"/",""),sa.building) as sc_addr,

CONCAT_WS(" ",pr.socr,pr.name,pa.city,pa.street,pa.house,IF(LENGTH(pa.building)>0,"/",""),pa.building) as p_addr,

p.tel as tel, p.email as email, p.id as pid

 from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_forms as f on (f.id=s.form) left join olimp_places as pl on (pl.id=s.place) left join olimp_actions as a on (a.stage_id=s.id) left join olimp_persons as p on (p.id=a.person_id) 

left join olimp_schools as sc on (sc.id=p.school)
left join olimp_school_types as st on (st.id=sc.school_type)
left join olimp_address as sa on (sa.id=sc.address)
left join kladr_regions as sr on (sr.id=sa.region)

left join olimp_address as pa on (pa.id=p.addr_reg)
left join kladr_regions as pr on (pr.id=pa.region)

left join olimp_address as adr ON(adr.id=p.addr_reg)
left join kladr_regions as ar on (ar.id=adr.region)


left join olimp_stage_types as stp on (stp.id = s.stage)

' . $where . ' order by stage,dt,place,exam,school_class,fio');
            switch ($_REQUEST['list_format'] + 0) {
                case 2:     {    $format = 'CSV';
                    break;
                }
                default:     {    $format = 'HTML';
                }
            }


                $list_tpl = 'list' . ($_REQUEST['use_school'] ? '_school' : '') . ($_REQUEST['use_addr'] ? '_addr' : '');
                $line_tpl = 'list' . ($_REQUEST['use_school'] ? '_school' : '') . ($_REQUEST['use_addr'] ? '_addr' : '') . '_line';
                $list_dt = '';
                $list_place = '';
                $list_exam = '';
                $list_class = '';
                $list = '';
                $lines = '';
            while ($row = $res->fetch_assoc()) {
                if ($row['dt'] != $list_dt) {
                            $list_dt = $row['dt'];
                    if ($lines) {
                        $list .= str_replace('%list%', $lines, $tpl['stages'][$format][$list_tpl]);
                        $lines = '';
                        $num = 1;
                    }
                    $list .= str_replace(
                        array('%list_dt%','%form%','%stage%'),
                        array($list_dt,$row['form'],$row['stage']),
                        $tpl['stages'][$format]['list_dt_line']
                    );
                                    $list_place = '';
                                    $list_exam = '';
                                    $list_class = '';
                }
                if ($row['place'] != $list_place) {
                    $list_place = $row['place'];
                    if ($lines) {
                        $list .= str_replace('%list%', $lines, $tpl['stages'][$format][$list_tpl]);
                        $lines = '';
                    }
                    $lines = '';
                    $num = 1;
                    $list .= str_replace(
                        array('%list_dt%','%list_place%','%form%','%stage%'),
                        array($list_dt,$list_place,$row['form'],$row['stage']),
                        $tpl['stages'][$format]['list_place_line']
                    );
                    $list_exam = '';
                    $list_class = '';
                }
                if ($row['exam'] != $list_exam) {
                    $list_exam = $row['exam'];
                    if ($lines) {
                        $list .= str_replace('%list%', $lines, $tpl['stages'][$format][$list_tpl]);
                        $lines = '';
                    }
                    $lines = '';
                    $num = 1;
                    $list .= str_replace(
                        array('%list_dt%','%list_place%','%list_exam%','%form%','%stage%'),
                        array($list_dt,$list_place,$list_exam,$row['form'],$row['stage']),
                        $tpl['stages'][$format]['list_exam_line']
                    );
                    $list_class = '';
                }
                if ($row['school_class'] != $list_class) {
                    $list_class = $row['school_class'];
                    if ($lines) {
                        $list .= str_replace('%list%', $lines, $tpl['stages'][$format][$list_tpl]);
                        $lines = '';
                    }
                    $lines = '';
                    $num = 1;
                    $list .= str_replace(
                        array('%list_dt%','%list_place%','%list_exam%','%list_class%','%form%','%stage%'),
                        array($list_dt,$list_place,$list_exam,$list_class,$row['form'],$row['stage']),
                        $tpl['stages'][$format]['list_class_line']
                    );
                }
                if ($_REQUEST['use_tel']) {
                    $row['p_addr'] .= ($format == 'CSV' ? "\n" : '<br>') . 'тел.' . $row['tel'] . ($format == 'CSV' ? "\n" : '<br>') . 'email.' . $row['email'];
                }
                $lines .= str_replace(
                    array('%num%','%class%','%fio%','%birth%','%school%','%sc_addr%','%p_addr%','%pid%'),
                    array($num,$row['class'],$row['fio'],$row['bdt'],$row['school'],$row['sc_addr'],$row['p_addr'],$row['pid']),
                    $tpl['stages'][$format][$line_tpl]
                );
                $num++;
            }
            $list .= str_replace('%list%', $lines, $tpl['stages'][$format][$list_tpl]);

            switch ($format) {
                case 'CSV':
                {    header("Content-Type: application/octet-stream");
                    header('Content-Disposition: attachment; filename="list.csv"');
                    break;
                }
                default:     {    header("Content-Type: text/html");
                }
            }
            echo $tpl['stages'][$format]['header'] . $list;

            die();
            break;
        }
        default:
        {    // Выбор этапа работы
        //$where='(s.year="'.$year.'") ';
            if ($filter_exam > 0) {
                $where .= ($where ? ' AND ' : '') . '(e.id=' . $filter_exam . ')';
            }
            if ($filter_stage > -1) {
                $where .= ($where ? ' AND ' : '') . '(s.stage=' . $filter_stage . ')';
            }
            if ($filter_date > -1) {
                $where .= ($where ? ' AND ' : '') . '(s.date_exam BETWEEN "' . $filter_date . ' 00:00" AND "' . $filter_date . ' 23:59")';
            }

            switch ($filter_region) {
                case -1: {    break;
                }
                case 0:  {    $where .= ' AND (adr.region NOT IN (' . $orgRegions . ') )';
                    break;
                }
                default: {    $where .= ' AND (adr.region IN (' . $orgRegions . ') )';
                    break;
                }
            }

        //if ($where) $where=' WHERE '.$where;

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

            $filter_dates = '<option value=-1 ' . ($filter_date == -1 ? ' selected' : '') . '>Все даты</option>';
            $res = $mysqli->query('select DISTINCT DATE_FORMAT(date_exam,"%Y-%m-%d") as d1, DATE_FORMAT(date_exam,"%d.%m.%Y")  from olimp_stages where year="' . $year . '" order by date_exam DESC');
            while ($row = $res->fetch_row()) {
                $filter_dates .= '<option value="' . $row[0] . '" ' . ($filter_date == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            $filter_presences = '<option value=-1 ' . ($filter_presence == -1 ? ' selected' : '') . '>Все участники</option>' .
            '<option value=1 ' . ($filter_presence == 1 ? ' selected' : '') . '>Явившиеся (приславшие работу)</option>' . '<option value=2 ' . ($filter_presence == 2 ? ' selected' : '') . '>Не явившиеся (отказавшиеся)</option>' . '<option value=3 ' . ($filter_presence == 3 ? ' selected' : '') . '>Скачавшие вариант</option>' . '<option value=4 ' . ($filter_presence == 4 ? ' selected' : '') . '>Не скачавшие вариант</option>' . '<option value=5 ' . ($filter_presence == 5 ? ' selected' : '') . '>Ожидание</option>';


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

            $stages = '';


            $even = 0;
            $stage_dt = '';
            $stage_place = '';
            $stage_exam = '';
            $res = $mysqli->query('select e.name as exam,p.school_class as school_class,pl.name as place,s.stage as stage,f.name as form, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as dt, count(DISTINCT a.person_id) as cnt from olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_forms as f on (f.id=s.form) left join olimp_places as pl on (pl.id=s.place) left join olimp_actions as a on (a.stage_id=s.id) left join olimp_persons as p on (p.id=a.person_id) left join olimp_address as adr ON(adr.id=p.addr_reg) ' . $where . ' group by stage,dt,place,form,exam,school_class');

            while ($row = $res->fetch_assoc()) {
                if (!isset($sta[$row['dt']])) {
                    $sta[$row['dt']]['rows'] = 0;
                }

                if (isset($sta[$row['dt']][$row['place']][$row['exam']])) {
                    $sta[$row['dt']][$row['place']][$row['exam']][$row['school_class']] += $row['cnt'];
                } else {
                    $sta[$row['dt']][$row['place']][$row['exam']] = array(5 => 0,6 => 0,7 => 0,8 => 0,9 => 0,10 => 0,11 => 0);
                    $sta[$row['dt']][$row['place']][$row['exam']][$row['school_class']] = $row['cnt'];
                    $sta[$row['dt']][$row['place']][$row['exam']]['form'] = $row['form'];
                    $sta[$row['dt']]['stage'] = $row['stage'];
                    $sta[$row['dt']]['rows'] += 1;
                }
            }

            function draw_stage_list($sta, $stage)
            {
                global $tpl,$even;
                $stages = '';
                if (!is_array($sta)) {
                    return '';
                }
                foreach ($sta as $date => $v) {
                    if ($v['stage'] != $stage) {
                        continue;
                    }
                    if (!is_array($v)) {
                        continue;
                    }
                    $newdate = true;
                    foreach ($v as $place => $w) {
                        if (!is_array($w)) {
                            continue;
                        }
                        $newplace = true;
                        $place_rows = count($w);
                        foreach ($w as $exam => $ww) {
                            $stages .= str_replace(
                                array('%dt_rows%','%place_rows%','%exam%','%stage%','%place%','%form%','%dt%','%even%',
                                '%5%','%6%','%7%','%8%','%9%','%10%','%11%','%SUM%'),
                                array($v['rows'],$place_rows,$exam,$ww['stage'],$place,$ww['form'],$date,
                                $even ? 'even' : 'odd',$ww[5],$ww[6],$ww[7],$ww[8],$ww[9],$ww[10],$ww[11],
                                $ww[5] + $ww[6] + $ww[7] + $ww[8] + $ww[9] + $ww[10] + $ww[11]),
                                $tpl['stages']['stage_line' . ($newdate ? '1' : ($newplace ? '2' : 'N'))]
                            );
                            $newdate = false;
                            $newplace = false;
                            $even = !$even;
                        }
                    }
                }
                return $stages;
            }

            $s = draw_stage_list($sta, 2);
            if ($s) {
                $stages .= str_replace('%name%', 'Заключительный этап', $tpl['stages']['stage_tline']) . $s;
            }
            $s = draw_stage_list($sta, 1);
            if ($s) {
                $stages .= str_replace('%name%', 'Отборочный этап', $tpl['stages']['stage_tline']) . $s;
            }
            $s = draw_stage_list($sta, 0);
            if ($s) {
                $stages .= str_replace('%name%', 'Призеры прошлого года', $tpl['stages']['stage_tline']) . $s;
            }
            $s = draw_stage_list($sta, 3);
            if ($s) {
                $stages .= str_replace('%name%', 'Тренировочный этап', $tpl['stages']['stage_tline']) . $s;
            }


            if ($orgRegionsAll) {
                $region_options = '<option value=-1' . ($filter_region == -1 ? ' selected' : '') . '>Все регионы</option><option value=0' . ($filter_region == 0 ? ' selected' : '') . '>Все регионы кроме партнеров</option>';
                foreach ($orgNames as $_IdOrg => $_orgName) {
                    if ($_IdOrg <> $user_IdOrg) {
                        $region_options .= '<option value=' . $_IdOrg . ($filter_region == $_IdOrg ? ' selected' : '') . '>' . $orgName . ' ==> ' . $_orgName . '</option>';
                    }
                }
                $_regions = '<select name="filter_region" size=0>' . $region_options . '</select>';
            } else {
                $_regions = $orgName;
            }


            $body = str_replace(array('%pppages%','%sort_groups%','%stages%','%filter_places%','%filter_exams%','%filter_stages%','%filter_forms%','%filter_presences%','%filter_dates%','%year%','%regions%'), array($pppages,$sort_groups,$stages,$filter_places,$filter_exams,$filter_stages,$filter_forms,$filter_presences,$filter_dates,$year,$_regions), $tpl['stages']['stages_list']);
        }
    }

    return str_replace('%error%', $error, $body);
}

$tpl['stages']['stages_list'] =
'%error%
<style>
table#stages {border-top: 1px solid blue;border-left: 1px solid blue; border-collapse:collapse;}
#stages td {border-bottom: 1px solid blue;border-right: 1px solid blue; padding:4px 8px;}
#stages #ttl td {background: #003366; color:white; font-weight:bold; text-align: center;}
#stages .even td {background: #ccffff; color:#000066;}
#stages .odd td {background: #F0ffff; color:#000066;}
#stages .tline {background: #F0ffff; color:#000066; font-size:larger;}
#stages .sum {font-weight:bold;}
</style>
<form action="?" method=post>
<table>
<tr><td colspan=4>Год проведения:&nbsp;<input name="year" type="text" value="%year%" size=10></td>
<td rowspan=4><button type=submit name="btnFilter"><br>Применить фильтр<br><br></button></td></tr>
<tr><td>Площадка:&nbsp;</td><td><select name="filter_place" size=0>%filter_places%</select></td><td> Форма:&nbsp;</td><td><select name="filter_form" size=0>%filter_forms%</select></td></tr>
<tr><td>Организатор:</td><td colspan=8>%regions%</td></tr>
<tr><td>Предмет:&nbsp;</td><td><select name="filter_exam" size=0>%filter_exams%</select></td><td> Этап:&nbsp;</td><td><select name="filter_stage" size=0>%filter_stages%</select></td></tr>
<tr><td>Участники:&nbsp;</td><td><select name="filter_presence" size=0>%filter_presences%</select></td><td> Дата:&nbsp;</td><td><select name="filter_date" size=0>%filter_dates%</select></td></tr>
</table>
<br>
<table cellpadding=2 id="stages">
<tr id=ttl><td>Дата</td><td>Площадка</td><td>Предмет</td><td>Форма</td><td>5</td><td>6</td><td>7</td><td>8</td><td>9</td><td>10</td><td>11</td><td>Всего</td></tr>
%stages%
</table>
<br>
</form>
<br><br>
<form action="?" method=post target="_blank">
<fieldset title=" Список участников ">
<label for="use_school"><input type=checkbox name=use_school> Указать школы</label>
<label for="use_addr"><input type=checkbox name=use_addr> Указать адрес</label>
<label for="use_tel"><input type=checkbox name=use_tel> Указать в адресе телефон </label>
Формат: <select name="list_format" size=0><option value=1>HTML</option><option value=2>CSV</option></select>
<button type=submit name="btnList"><br>Сформировать список<br><br></button>
</fieldset>
</form>

<br><br>
';


$tpl['stages']['stage_line1'] =
'<tr class=%even%><td rowspan="%dt_rows%">%dt%</td><td rowspan="%place_rows%">%place%</td><td>%exam%</td><td>%form%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum">%SUM%</td></tr>
';
$tpl['stages']['stage_line2'] =
'<tr class=%even%><td rowspan="%place_rows%">%place%</td><td>%exam%</td><td>%form%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum">%SUM%</td></tr>
';
$tpl['stages']['stage_lineN'] =
'<tr class=%even%><td>%exam%</td><td>%form%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum">%SUM%</td></tr>
';


$tpl['stages']['stage_tline'] = '<tr><td colspan=12 class=tline>%name%</td></tr>';

$tpl['stages']['HTML']['list_dt_line'] = '<h1>Потоки %list_dt%</h1>';
$tpl['stages']['CSV']['list_dt_line'] = "%list_dt%\n";

$tpl['stages']['HTML']['list_place_line'] = '<h1>Площадка %list_place%</h1><h2>%stage% %form% форма %list_dt%</h2>';
$tpl['stages']['CSV']['list_place_line'] = "\"%list_place%\";\"%stage%\";\"%form%\";%list_dt%\n";

$tpl['stages']['HTML']['list_exam_line'] = '<h1>%list_exam%</h1></h2>';
$tpl['stages']['CSV']['list_exam_line'] = "\"%list_exam%\"\n";

$tpl['stages']['HTML']['list'] = '<table><tr><td>#</td><td>ФИО</td><td>Д.Р.</td></tr>%list%</table>';
$tpl['stages']['HTML']['list_school'] = '<table><tr><td>#</td><td>ФИО</td><td>Д.Р.</td><td>Школа</td><td>Адрес школы</td></tr>%list%</table>';
$tpl['stages']['HTML']['list_addr'] = '<table><tr><td>#</td><td>ФИО</td><td>Д.Р.</td><td>Адрес регистрации</td></tr>%list%</table>';
$tpl['stages']['HTML']['list_school_addr'] = '<table><tr><td>#</td><td>ФИО</td><td>Д.Р.</td><td>Школа</td><td>Адрес школы</td><td>Адрес регистрации</td></tr>%list%</table>';
$tpl['stages']['CSV']['list'] = '%list%';
$tpl['stages']['CSV']['list_school'] = '%list%';
$tpl['stages']['CSV']['list_addr'] = '%list%';
$tpl['stages']['CSV']['list_school_addr'] = '%list%';

$tpl['stages']['HTML']['list_class_line'] = '<h1>%list_class% класс</h1></h2>';
$tpl['stages']['CSV']['list_class_line'] = "\"%list_class% класс\"\n";


$tpl['stages']['HTML']['list_school_addr_line'] = '<tr><td>%num%</td><td>%fio%</td><td>%birth%</td><td>%school%</td><td>%sc_addr%</td><td>%p_addr%</td></tr>';
$tpl['stages']['CSV']['list_school_addr_line'] = "%pid%;\"%fio%\";\"%birth%\";\"%school%\";\"%sc_addr%\";\"%p_addr%\"\n";

$tpl['stages']['HTML']['list_school_line'] = '<tr><td>%num%</td><td>%fio%</td><td>%birth%</td><td>%school%</td><td>%sc_addr%</td></tr>';
$tpl['stages']['CSV']['list_school_line'] = "%pid%;\"%fio%\";\"%birth%\";\"%school%\";\"%sc_addr%\"\n";

$tpl['stages']['HTML']['list_addr_line'] = '<tr><td>%num%</td><td>%fio%</td><td>%birth%</td><td>%p_addr%</td></tr>';
$tpl['stages']['CSV']['list_addr_line'] = "%pid%;\"%fio%\";\"%birth%\";\"%p_addr%\"\n";

$tpl['stages']['HTML']['list_line'] = '<tr><td>%num%</td><td>%fio%</td><td>%birth%</td></tr>';
$tpl['stages']['CSV']['list_line'] = "%pid%;\"%fio%\";\"%birth%\"\n";

$tpl['stages']['HTML']['header'] = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<title>Олимпиада школьников "Надежда энергетики". Списки участников.</title>
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
}
</style>

';
$tpl['stages']['CSV']['header'] = '';

$body = build_page();
