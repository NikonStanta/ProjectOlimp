<?php

ini_set('memory_limit', '1024M');
set_time_limit(600);
// Этапы олимпиады
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$user_IdOrg,$mysqli,$filter_region,$error;

    if (($role & 512) == 0) {
        return $tpl['noaccess'];
    }

    $user_places = getUserPlaces($uid);

/*
echo '<PRE>';
print_r($user_places);
echo '<PRE>';
echo $user_IdOrg;
echo '<hr>';
*/

    if (empty($user_places['Access']) || ($user_IdOrg < 1)) {
        return $tpl['noaccess'];
    }

    $_works_folder = '../works/';
    function mime_type2ext($tp)
    {
        switch ($tp) {
            case 'application/pdf':
                return '.pdf';
            case 'application/zip':
                return '.zip';
            case 'application/gzip':
                return '.gzip';
            case 'application/x-rar':
                return '.rar';
            case 'image/jpeg':
            case 'image/pjpeg':
                return '.jpeg';
            case 'image/png':
                return '.png';
        }
        return '.unknown';
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

    if (empty($orgName)) {
        return $tpl['noaccess'];
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

//

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

    switch ($cmd) {
        case 'work':
        {    $work_sid = $_REQUEST['sid'] + 0;
            $work_pid = $_REQUEST['pid'] + 0;
            $work_other = $_REQUEST['other'];
            $work_filename = $work_sid . '_' . $work_pid . ($work_other ? '_' . $work_other : '');
            $scan_filename = $_works_folder . $work_filename;
            if (file_exists($scan_filename)) {
                $mime = mime_content_type($scan_filename);
                header('Content-Type: ' . $mime);
                header('Content-disposition: attachment; filename=' . $work_filename . mime_type2ext($mime));
                header('Content-Length: ' . filesize($scan_filename));
                if (ob_get_level()) {
                    ob_end_clean();
                }
                readfile($scan_filename);
            } else {
                echo 'Not found ' . $scan_filename;
            }
            return '<!--DONE-->';
        }
        case 'view':
        {    $view_dt = preg_replace('/([0-9]{2})\.([0-9]{2})\.([0-9]{4})/', '$3-$2-$1', $_REQUEST['dt']);
            $view_exam = $_REQUEST['exam'];
            $view_region = '';
            switch ($filter_region) {
                case -1: {    break;
                }
                case 0:  {    $view_region .= ' AND (adr.region NOT IN (' . $orgRegions . ') )';
                    break;
                }
                default: {    $view_region .= ' AND (adr.region IN (' . $orgRegions . ') )';
                    break;
                }
            }
            $list = '';

        //$role=3;

            if (($role & 512 + 4) != 0) {
                $order_by = ' fio';
            } else {
                $order_by = 'w.stage_id, cipher';
            }
            $res = $mysqli->query('SELECT concat(s.cipher,CAST(g.cipher AS CHAR),"-",CAST(a.cipher AS CHAR)) as cipher, a.stage_id as stage_id, a.person_id as person_id, a.cipher as hasCipher, CONCAT_WS(" ",p.Surename,p.Name,p.Patronymic) AS fio FROM olimp_actions as a LEFT JOIN olimp_works as w USING(stage_id, person_id) LEFT JOIN olimp_groups as g ON (g.id=a.group_id) LEFT JOIN olimp_stages as s ON (s.id=a.stage_id) LEFT JOIN olimp_persons as p ON(p.id=a.person_id) LEFT JOIN olimp_schools as sc ON(sc.id=p.school) LEFT JOIN olimp_address as adr ON (adr.id=sc.address) WHERE (s.year="' . $year . '") AND (w.status IN ("L","U","R","A") OR w.status IS NULL) AND (s.exam=' . $view_exam . ') AND (s.date_exam LIKE "' . $view_dt . '%") ' . $view_region . '  HAVING cipher IS NOT NULL ORDER BY ' . $order_by);

        // $res = $mysqli->query('SELECT concat(s.cipher,CAST(g.cipher AS CHAR),"-",CAST(a.cipher AS CHAR)) as cipher, w.stage_id as stage_id, w.person_id as person_id, a.cipher as hasCipher, CONCAT_WS(" ",p.Surename,p.Name,p.Patronymic) AS fio FROM olimp_works as w LEFT JOIN olimp_actions as a USING(stage_id, person_id) LEFT JOIN olimp_groups as g ON (g.id=a.group_id) LEFT JOIN olimp_stages as s ON (s.id=w.stage_id) LEFT JOIN olimp_persons as p ON(p.id=w.person_id) LEFT JOIN olimp_schools as sc ON(sc.id=p.school) LEFT JOIN olimp_address as adr ON (adr.id=sc.address) WHERE (s.year="'.$year.'") AND (w.status IN ("L","U","R","A")) AND (s.exam='.$view_exam.') AND (s.date_exam LIKE "'.$view_dt.'%") '.$view_region.'  HAVING cipher IS NOT NULL ORDER BY '.$order_by);

            while ($row = $res->fetch_assoc()) {
                $scan_filename = $_works_folder . $row['stage_id'] . '_' . $row['person_id'];
                if (file_exists($scan_filename)) {
                    $mime = mime_content_type($scan_filename);
                    $filesize = round(filesize($scan_filename) / 1048576.0, 2) . 'M';
                } else {
                    $mime = '';
                    $filesize = '';
                }
                $person = (($role & 512 + 4) != 0) ? $row['fio'] : $row['stage_id'] . '-' . $row['person_id'];
                if (!$row['hasCipher']) {
                    $row['cipher'] = 'нет шифра';
                }
                $work_files = glob($scan_filename . '*');
                $others = '';
                foreach ($work_files as $other_file) {
                    if ($other_file != $scan_filename) {
                                $other_mime = mime_content_type($other_file);
                                    $other_filesize = round(filesize($other_file) / 1048576.0, 2) . 'M';
                                    $other_filename = str_replace($scan_filename . '_', '', $other_file);
                                    $others .= ($others ? '<br>' : '') . str_replace(
                                        array('%stage_id%','%person_id%','%mime%','%size%','%other%'),
                                        array($row['stage_id'],$row['person_id'],$other_mime,$other_filesize,$other_filename),
                                        $tpl['works']['view_other_ref']
                                    );
                    }
                }
                $list .= str_replace(
                    array('%person%','%cipher%','%stage_id%','%person_id%','%mime%','%size%','%others%'),
                    array($person,$row['cipher'],$row['stage_id'],$row['person_id'],$mime,$filesize,$others),
                    $tpl['works']['view_line' . (empty($filesize) ? '_nofile' : '')]
                );
            }
            $body = str_replace(
                array('%dt%','%exam%','%works%'),
                array($_REQUEST['dt'],$view_exam,$list),
                $tpl['works']['view_body']
            );

                        break;
        }
        case 'download':
        {    $download_dt = preg_replace('/([0-9]{2})\.([0-9]{2})\.([0-9]{4})/', '$3-$2-$1', $_REQUEST['dt']);
            $download_exam = $_REQUEST['exam'];
            $download_region = '';
            switch ($filter_region) {
                case -1: {    break;
                }
                case 0:  {    $download_region .= ' AND (adr.region NOT IN (' . $orgRegions . ') )';
                    break;
                }
                default: {    $download_region .= ' AND (adr.region IN (' . $orgRegions . ') )';
                    break;
                }
            }
            $zipname = 'works_' . $filter_region . '_' . $download_dt . '_' . $download_exam . '.zip';
            if (file_exists($_works_folder . $zipname)) {
                unlink($_works_folder . $zipname);
            }
            $zip = new ZipArchive();
            $res = $zip->open($_works_folder . $zipname, ZipArchive::CREATE);
            if ($res === true) {
                $res = $mysqli->query('SELECT concat(s.cipher,CAST(g.cipher AS CHAR),"-",CAST(a.cipher AS CHAR)) as ch, w.stage_id as stage_id, w.person_id as person_id FROM olimp_works as w LEFT JOIN olimp_actions as a USING(stage_id, person_id) LEFT JOIN olimp_groups as g ON (g.id=a.group_id) LEFT JOIN olimp_stages as s ON (s.id=w.stage_id) LEFT JOIN olimp_persons as p ON(p.id=w.person_id) LEFT JOIN olimp_schools as sc ON(sc.id=p.school) LEFT JOIN olimp_address as adr ON (adr.id=sc.address) WHERE (s.year="' . $year . '") AND (w.status IN ("L","U","R","A")) AND (s.exam=' . $download_exam . ') AND (s.date_exam LIKE "' . $download_dt . '%") ' . $download_region . '  HAVING ch IS NOT NULL');

                while ($row = $res->fetch_assoc()) {
                    $scan_filename = $_works_folder . $row['stage_id'] . '_' . $row['person_id'];
                    $mime_ext = mime_type2ext(mime_content_type($scan_filename));
                    $work_filename = $row['ch'] . '_' . $row['stage_id'] . '_' . $row['person_id'] . $mime_ext;
                    $zip->addFile($scan_filename, $work_filename);
                }

                $zip->close();
                header('Content-Type: application/zip');
                header('Content-disposition: attachment; filename=' . $zipname);
                header('Content-Length: ' . filesize($_works_folder . $zipname));

                if (ob_get_level()) {
                    ob_end_clean();
                }

                readfile($_works_folder . $zipname);
                if (file_exists($_works_folder . $zipname)) {
                    unlink($_works_folder . $zipname);
                }
                return '<!--DONE-->';
            } else {
                echo 'ошибка';
            }

            break;
        }
        default:
        {    // Выбор этапа работы
            $where = '(s.year="' . $year . '") AND (w.status IN ("L","U","A","R")) ';
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
            if ($where) {
                $where = ' WHERE ' . $where;
            }

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
            $stages = '';


            $even = 0;
            $stage_dt = '';
            $stage_exam = '';


            $res = $mysqli->query('select e.name as exam,p.school_class as school_class,s.stage as stage, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as dt, count(DISTINCT w.person_id) as cnt, s.exam as exam_id FROM olimp_stages as s left join olimp_exams as e on (e.id=s.exam) left join olimp_works as w on (w.stage_id=s.id) left join olimp_persons as p on (p.id=w.person_id) LEFT JOIN olimp_schools as sc ON(sc.id=p.school) LEFT JOIN olimp_address as adr ON (adr.id=sc.address) ' . $where . ' group by stage,dt,exam,school_class,exam_id ORDER BY stage,s.date_exam');

            while ($row = $res->fetch_assoc()) {
                if (!isset($sta[$row['dt']])) {
                    $sta[$row['dt']]['rows'] = 0;
                }

                if (isset($sta[$row['dt']]['exams'][$row['exam']])) {
                    $sta[$row['dt']]['exams'][$row['exam']][$row['school_class']] += $row['cnt'];
                    $sta[$row['dt']]['exams'][$row['exam']]['summ'] += $row['cnt'];
                } else {
                    $sta[$row['dt']]['exams'][$row['exam']] = array(5 => 0,6 => 0,7 => 0,8 => 0,9 => 0,10 => 0,11 => 0,'summ');
                    $sta[$row['dt']]['exams'][$row['exam']][$row['school_class']] = $row['cnt'];
                    $sta[$row['dt']]['exams'][$row['exam']]['summ'] = $row['cnt'];
                    $sta[$row['dt']]['stage'] = $row['stage'];
                    $sta[$row['dt']]['rows'] += 1;
                }
                $sta_ex[$row['dt']][$row['exam']]['exam_id'] = $row['exam_id'];
            }

            //print_r($sta);

            function draw_stage_list($sta, $stage, $sta_ex)
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

                //print_r($v); echo '<hr>';

                    foreach ($v['exams'] as $exam => $ww) {
                        $exam_id = $sta_ex[$date][$exam]['exam_id'];
                        $stages .= str_replace(
                            array('%dt_rows%','%exam%','%stage%','%dt%','%even%',
                            '%5%','%6%','%7%','%8%','%9%','%10%','%11%','%summ%','%exam_id%'),
                            array($v['rows'],$exam,$v['stage'],$date,
                            $even ? 'even' : 'odd',$ww[5],$ww[6],$ww[7],$ww[8],$ww[9],$ww[10],$ww[11],$ww['summ'],$exam_id),
                            $tpl['works']['stage_line' . ($newdate ? '1' : 'N')]
                        );
                        $newdate = false;
                        $even = !$even;
                    }
                }
                return $stages;
            }

            $s = draw_stage_list($sta, 2, $sta_ex);
            if ($s) {
                $stages .= str_replace('%name%', 'Заключительный этап', $tpl['works']['stage_tline']) . $s;
            }
            $s = draw_stage_list($sta, 1, $sta_ex);
            if ($s) {
                $stages .= str_replace('%name%', 'Отборочный этап', $tpl['works']['stage_tline']) . $s;
            }
            $s = draw_stage_list($sta, 3, $sta_ex);
            if ($s) {
                $stages .= str_replace('%name%', 'Тренировочный этап', $tpl['works']['stage_tline']) . $s;
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
            $body = str_replace(array('%pppages%','%sort_groups%','%stages%','%filter_exams%','%filter_stages%','%filter_dates%','%regions%','%year%'), array($pppages,$sort_groups,$stages,$filter_exams,$filter_stages,$filter_dates,$_regions,$year), $tpl['works']['stages_list']);
        }
    }

    return str_replace('%error%', $error, $body);
}


// =============  Шаблоны  =================================================================================

$tpl['works']['stages_list'] =
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
<tr><td colspan=10>Год проведения:&nbsp;<input name="year" type="text" value="%year%" size=10></td></tr>
<tr><td>Организатор:</td><td colspan=8>%regions%</td><td rowspan=2><button type=submit name="btnFilter"><br>Применить фильтр<br><br></button></td></tr>
<tr><td>Предмет:&nbsp;</td><td><select name="filter_exam" size=0>%filter_exams%</select></td><td> Этап:&nbsp;</td><td><select name="filter_stage" size=0>%filter_stages%</select></td><td> Дата:&nbsp;</td><td><select name="filter_date" size=0>%filter_dates%</select></td></tr>
</table>
<br>
<table cellpadding=2 id="stages">
<tr id=ttl><td>Дата</td><td>Предмет</td><td>5</td><td>6</td><td>7</td><td>8</td><td>9</td><td>10</td><td>11</td><td>Всего</td><td>Действия</td></tr>
%stages%
</table>
<br>
</form>
<br><br>
';

$tpl['works']['stage_line1'] =
'<tr class=%even%><td rowspan="%dt_rows%">%dt%</td><td>%exam%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum">%summ%</td><td><a target="_blank" href="?cmd=download&dt=%dt%&exam=%exam_id%">[Скачать работы]</a>&nbsp;<a href="?cmd=view&dt=%dt%&exam=%exam_id%">[Список участников]</a></td></tr>
';

$tpl['works']['stage_lineN'] =
'<tr class=%even%><td>%exam%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum">%summ%</td><td><a target="_blank" href="?cmd=download&dt=%dt%&exam=%exam_id%">[Скачать работы]</a>&nbsp;<a href="?cmd=view&dt=%dt%&exam=%exam_id%">[Список участников]</a></td></tr>
';


$tpl['works']['stage_tline'] = '<tr><td colspan=11 class=tline>%name%</td></tr>';


$tpl['works']['view_body'] =
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
<h2>Список участников</h2>
Предмет: %exam%<br>
Дата: %dt%<br><br>
<table cellpadding=2 id="stages">
<tr id=ttl><td>Участник</td><td>Шифр</td><td>Работа</td><td>Другие файлы</td></tr>
%works%
</table>
<br>
</form>
<br><br>
<a href="?cmd=">Назад</a>
<br><br>
';

$tpl['works']['view_line'] =
'<tr class=%even%><td>%person%</td><td>%cipher%</td><td><a target="_blank" href="?cmd=work&pid=%person_id%&sid=%stage_id%">[Скачать %mime% %size%]</a></td><td>%others%</td></td></tr>
';

$tpl['works']['view_line_nofile'] =
'<tr class=%even%><td>%person%</td><td>%cipher%</td><td>[Работа не загружена]</td><td>%others%</td></td></tr>
';


$tpl['works']['view_other_ref'] =
'<a target="_blank" href="?cmd=work&pid=%person_id%&sid=%stage_id%&other=%other%">%other% [Скачать %mime% %size%]</a>';

// шаблоны ведомостей

$tpl['works']['list_header'] = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
<title>Олимпиада школьников "Надежда энергетики". Шифровальные ведомости.</title>
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



$tpl['works']['list'] = '<h1>Шифровальная ведомость</h1><h2>Группа %group_name%  (шифр %cipher%)</h2><h2>Предмет: %exam%</h2>
<table cellspacin=1 cellpadding=2 border=0>
<tr bgcolor="#CCCCff"><td style="width:10cm"  class="first">ФИО (дата рождения)</td><td style="width:1.8cm">Явка</td><td style="width:2cm"  class="first">Шифр</td></tr>
%lines%
</table>
<div style="page-break-after: always;"></div>
';


$tpl['works']['list_line'] = '<tr><td class="first">%fio% (%bdt%)</td><td>%presence%</td><td class="first" style="white-space:nowrap;">%cipher%</td></tr>';





$body = build_page();
