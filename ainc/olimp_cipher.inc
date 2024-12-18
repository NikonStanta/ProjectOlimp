<?php

// Этапы олимпиады
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

// Фильтр по организаторам
    if (isset($_POST['filter_org'])) {
        $filter_org = $_POST['filter_org'] + 0;
        if (empty($filter_org)) {
            $filter_org = -1;
        }
        $_SESSION['filter_org'] = $filter_org;
    } elseif (isset($_SESSION['filter_org'])) {
        $filter_org = $_SESSION['filter_org'];
    } else {
        $filter_org = -1;
    }

    $OnlyRegions = array();
    if ($filter_org > 0) {
        $ShowOnlyRegions = true;
        $res = $mysqli->query('select IdRegion from olimp_org_regions where IdOlimpOrg=' . $filter_org);
        while ($row = $res->fetch_row()) {
            if ($row[0] > 0) {
                $OnlyRegions[] = $row[0];
            } else {
                $OnlyRegions = array();
                $ShowOnlyRegions = false;
            }
        }
        $view_region = ' AND (adr.region IN (' . implode(',', $OnlyRegions) . ') )';
    } else {
        $ShowOnlyRegions = false;
        $view_region = '';
    }

    if (isset($_POST['filter_org'])) {
        if (isset($_POST['ShowRegs'])) {
            $ShowRegs = 1;
        } else {
            $ShowRegs = 0;
        }
        $_SESSION['ShowRegs'] = $ShowRegs;
    } elseif (isset($_SESSION['ShowRegs'])) {
        $ShowRegs = $_SESSION['ShowRegs'];
    } else {
        $ShowRegs = 0;
    }


    $res = $mysqli->query('SELECT r.IdRegion,o.name FROM olimp_org_regions AS r LEFT JOIN olimp_orgs AS o ON (o.id=r.IdOlimpOrg)');
    while ($row = $res->fetch_row()) {
        $Regions2Org[$row[0]] = $row[1];
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

    if (($filter_presence == 3) || ($filter_presence == 4)) {
        $filter_form = 2;
    }

    $where = '(s.year="' . $year . '")' . $view_region;
    if ($user_places['Access']) {
            $where .= ' AND (pl.id in (' . $user_places['Access'] . '))';
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
    }

    if ($where) {
        $where = ' WHERE ' . $where;
    }

// echo $where;


    switch ($cmd) {
        case 'cipher':
            {    $cipher_date = trim($_REQUEST['dt']);
            $cipher_date = preg_replace('/([0-9]{1,2})\.([0-9]{1,2})\.([0-9]{2,4})/', '$3-$2-$1', $cipher_date);
            $cipher_place = $_REQUEST['place_id'] + 0;

            if ($user_places['rights'][$cipher_place] != 'All') {
                $error = 'Нет доступа к шифровке этой площадки';
                die();
                $cmd = '';
                break;
            }

            $cipher_exam = $_REQUEST['exam_id'] + 0;
            $cipher_form = $_REQUEST['form_id'] + 0;
        // Подготовим массивы для шифровки

            $chipher_letters = array('Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M');
            $used_chiphers = array();
            $res = $mysqli->query('select id,cipher from olimp_stages');
            while ($row = $res->fetch_row()) {
                $used_chiphers[$row[1]] = $row[0];
            }
            $chipher_protocol = '';
            $res = $mysqli->query('select s.id as id,s.cipher as cipher from olimp_stages as s where (s.date_exam BETWEEN 
"' . $cipher_date . ' 00:00" AND "' . $cipher_date . ' 23:59") AND (s.exam=' . $cipher_exam . ') AND (s.place=' . $cipher_place . ') AND (s.form=' . $cipher_form . ')');
            while ($row = $res->fetch_assoc()) {
                // обновим шифры потоков
                            $stage_id = $row['id'];
                if (strlen($row['cipher']) == 0) {
                    $chipher = '';
                    while ($chipher == '') {
                        shuffle($chipher_letters);
                        $chipher = $chipher_letters[0];
                        shuffle($chipher_letters);
                        $chipher .= $chipher_letters[0];
                        if (array_key_exists($chipher, $used_chiphers)) {
                            $chipher = '';
                        }
                    }
                                $mysqli->query('UPDATE olimp_stages SET cipher="' . $chipher . '" WHERE id=' . $stage_id);
                                $used_chiphers[$chipher] = $stage_id;
                                $chipher_protocol .= '(' . $stage_id . ') создан шифр потока - ' . $chipher . '<br>';
                }
                // обновим шифры групп потока
                $used_group_ciphers = array();
                $res2 = $mysqli->query('select id,cipher from olimp_groups where (stage=' . $stage_id . ') AND (cipher>9)');
                while ($row2 = $res2->fetch_row()) {
                    $used_group_ciphers[$row2[1]] = $row2[0];
                }
                $res2 = $mysqli->query('select g.id as id,g.cipher as cipher from olimp_groups as g where g.stage=' . $stage_id);
                while ($row2 = $res2->fetch_assoc()) {
                    $group_id = $row2['id'];
                    if ($row2['cipher'] < 10) {
                        $gchipher = 0;
                        while ($gchipher < 10) {
                            $gchipher = rand(10, 99);
                            if (array_key_exists($gchipher, $used_group_ciphers)) {
                                $gchipher = 0;
                            }
                        }
                        $mysqli->query('UPDATE olimp_groups SET cipher="' . $gchipher . '" WHERE id=' . $group_id);
                        $used_group_ciphers[$gchipher] = $group_id;
                        $chipher_protocol .= '**(' . $group_id . ') создан шифр группы - ' . $gchipher . '<br>';
                    }

                // обновим шифры участников

                    $used_person_ciphers = array();
                    $person_count = 0;
                    $res3 = $mysqli->query('select person_id,cipher from olimp_actions where (stage_id=' . $stage_id . ') AND (group_id=' . $group_id . ') AND (cipher>9)');
                    while ($row3 = $res3->fetch_row()) {
                        $used_person_ciphers[$row3[1]] = $row3[0];
                    }
                    $res3 = $mysqli->query('select person_id,cipher from olimp_actions where (stage_id=' . $stage_id . ') AND (group_id=' . $group_id . ') AND (cipher<10)');
                    while ($row3 = $res3->fetch_assoc()) {
                        $person_id = $row3['person_id'];
                        $pchipher = 0;
                        while ($pchipher < 10) {
                            $pchipher = rand(10, 99);
                            if (array_key_exists($pchipher, $used_person_ciphers)) {
                                $pchipher = 0;
                            }
                        }
                        $mysqli->query('UPDATE olimp_actions SET cipher="' . $pchipher . '" WHERE (stage_id=' . $stage_id . ') AND (person_id=' . $person_id . ')');
                        $used_person_ciphers[$pchipher] = $person_id;
                        $person_count++;
                    }
                    if ($person_count > 0) {
                        $chipher_protocol .= '***(' . $group_id . ') созданы шифры для ' . $person_count . ' участников<br>';
                    }
                }
            }

                // echo $chipher_protocol;

                //echo 'done cipher'; die();

                $cmd = '';
            break;

        }
    }

    switch ($cmd) {
        case 'list':
        case 'list1':
        case 'list2':
        case 'list3':
        case 'list4':
        {    $lvl = $_REQUEST['lvl'] + 0;
            $group_ids = array();
            $w = '';
            $order_by = 'fio';
//if ($cmd=='list3') { $cmd='list1'; $order_by='fio'; }
        //if ($cmd=='list4') { $cmd='list2'; $order_by='fio'; }
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

        // echo 'select g.id from olimp_stages as s left join olimp_groups as g on (g.stage=s.id) where '.$w.' order by s.cipher,g.cipher';

            $res = $mysqli->query('select g.id from olimp_stages as s left join olimp_groups as g on (g.stage=s.id) where ' . $w . ' order by s.cipher,g.cipher');
            while ($row = $res->fetch_row()) {
                $group_ids[$row[0]] = $row[0];
            }

            $body = '';
            foreach ($group_ids as $gid) {
                if ($gid < 1) {
                    continue;
                }
                $res = $mysqli->query('select s.cipher,g.cipher,s.task_count,g.name,e.name,s.classes from olimp_groups as g left join olimp_stages as s on (s.id=g.stage) left join olimp_exams as e on (e.id=s.exam) where (s.cipher<>"") AND (g.cipher>0) AND (g.id=' . $gid . ')');
                if ($row = $res->fetch_row()) {
                    $cipher = $row[0] . $row[1];
                    $task_count = $row[2];
                    $group_name = $row[3];
                    $exam_name = $row[4];
                    $classes = $row[5];
                    $tasks = '';
                    for ($i = 1; $i <= $task_count; $i++) {
                        $tasks .= str_replace('%num%', $i, $tpl['res']['task_cell_head']);
                    }
                    $lines = '';
                    $res = $mysqli->query('select a.cipher as cipher,a.task_results as results,a.presence as presence,a.person_id as person_id, a.result, concat_ws(" ",p.surename,p.name,p.patronymic) as fio,DATE_FORMAT(p.birthday,"%d.%m.%Y") as bdt, adr.region as region from olimp_actions as a left join olimp_persons as p on (p.id=a.person_id) left join olimp_schools as sc ON(sc.id=p.school) left join olimp_address as adr ON(adr.id=sc.address) where a.group_id=' . $gid . ' order by ' . $order_by);
                    while ($row = $res->fetch_assoc()) {
                        $r = explode(',', $row['results']);
                        $task_results = '';
                        $t_cnt = $task_count < count($r) ? $task_count : count($r);
                        for ($i = 1; $i <= $t_cnt; $i++) {
                            $task_results .= str_replace(array('%person_id%','%num%','%value%'), array($row['person_id'],$i,$r[$i - 1]), $tpl['res'][$cmd . '_task_cell']);
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

                        if (!$ShowOnlyRegions || in_array($row['region'], $OnlyRegions)) {
                            $regions = $row['region'] . ' ' . (array_key_exists($row['region'], $Regions2Org) ? $Regions2Org[$row['region']] : 'МЭИ (Москва)');
                            $lines .= str_replace(
                                array('%cipher%','%presence%','%results%','%person%','%result%','%fio%','%bdt%','%region%'),
                                array($cipher . '-' . $row['cipher'],$presence,$task_results,$row['person_id'],$row['result'],$row['fio'],$row['bdt'],$regions),
                                $tpl['cipher'][$cmd . '_line' . ($ShowRegs ? '_reg' : '')]
                            );
                        }
                    }
                    if (!empty($lines)) {
                        $body .= str_replace(
                            array('%cipher%','%lines%','%tasks%','%group_id%','%group_name%','%exam%','%classes%'),
                            array($cipher, $lines, $tasks, $gid, $group_name, $exam_name, $classes),
                            $tpl['cipher'][$cmd . ($ShowRegs ? '_reg' : '')]
                        );
                    }
                }
            }
        //echo $tpl['cipher'][$cmd.'_header'].$body;
        //die();
            if (empty($body)) {
                $body = $tpl['cipher']['empty'];
            }
            break;

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

            $filter_dates = '<option value=-1 ' . ($filter_date == -1 ? ' selected' : '') . '>Все даты</option>';
            $res = $mysqli->query('select DISTINCT DATE_FORMAT(date_exam,"%Y-%m-%d") as d1, DATE_FORMAT(date_exam,"%d.%m.%Y")  from olimp_stages where year=' . $year . ' order by date_exam DESC');
            while ($row = $res->fetch_row()) {
                $filter_dates .= '<option value="' . $row[0] . '" ' . ($filter_date == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            $filter_presences = '<option value=-1 ' . ($filter_presence == -1 ? ' selected' : '') . '>Все участники</option>' .
            '<option value=1 ' . ($filter_presence == 1 ? ' selected' : '') . '>Явившиеся (приславшие работу)</option>' . '<option value=2 ' . ($filter_presence == 2 ? ' selected' : '') . '>Не явившиеся (отказавшиеся)</option>' . '<option value=3 ' . ($filter_presence == 3 ? ' selected' : '') . '>Скачавшие вариант</option>' . '<option value=4 ' . ($filter_presence == 4 ? ' selected' : '') . '>Не скачавшие вариант</option>';
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

            $filter_orgs = '<option value=-1 ' . ($filter_org == -1 ? ' selected' : '') . '>Все организаторы</option>';
            $res = $mysqli->query('select id,name from olimp_orgs order by id');
            while ($row = $res->fetch_row()) {
                $filter_orgs .= '<option value=' . $row[0] . ' ' . ($filter_org == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            $stages = '';
            $even = 0;
            $stage_dt = '';
            $stage_place = '';
            $stage_exam = '';
            $sta = array();
            $sta_ex = array();
            $res = $mysqli->query('select e.name as exam,p.school_class as school_class,pl.name as place,s.stage as stage,f.name as form, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt, 1 as cnt, s.exam as exam_id, s.place as place_id, s.form as form_id from olimp_actions as a LEFT JOIN olimp_stages as s on (s.id=a.stage_id) left join olimp_exams as e on (e.id=s.exam) left join olimp_forms as f on (f.id=s.form) left join olimp_places as pl on (pl.id=s.place) left join olimp_persons as p on (p.id=a.person_id) left join olimp_schools as sc ON(sc.id=p.school) left join olimp_address as adr ON(adr.id=sc.address) ' . $where);

        //echo 'select e.name as exam,p.school_class as school_class,pl.name as place,s.stage as stage,f.name as form, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as dt, count(DISTINCT CONCAT(a.person_id,dt)) as cnt, s.exam as exam_id, s.place as place_id, s.form as form_id from olimp_actions as a LEFT JOIN olimp_stages as s on (s.id=a.stage_id) left join olimp_exams as e on (e.id=s.exam) left join olimp_forms as f on (f.id=s.form) left join olimp_places as pl on (pl.id=s.place) left join olimp_persons as p on (p.id=a.person_id) left join olimp_schools as sc ON(sc.id=p.school) left join olimp_address as adr ON(adr.id=sc.address) '.$where.' group by exam,school_class,place,stage,form,dt,exam_id,place_id, form_id';


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
                $sta_ex[$row['dt']][$row['place']][$row['exam']][$row['form']]['place_id'] = $row['place_id'];
                $sta_ex[$row['dt']][$row['place']][$row['exam']][$row['form']]['exam_id'] = $row['exam_id'];
                $sta_ex[$row['dt']][$row['place']][$row['exam']][$row['form']]['form_id'] = $row['form_id'];
            }


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
                    foreach ($v as $place => $w) {
                        if (!is_array($w)) {
                            continue;
                        }
                        $newplace = true;
                        $place_rows = count($w);
                        foreach ($w as $exam => $ww) {
                            $exam_id = $sta_ex[$date][$place][$exam][$ww['form']]['exam_id'];
                            $place_id = $sta_ex[$date][$place][$exam][$ww['form']]['place_id'];
                            $form_id = $sta_ex[$date][$place][$exam][$ww['form']]['form_id'];
                            $stages .= str_replace(array('%dt_rows%','%place_rows%','%exam%','%place%','%form%','%dt%','%even%',
                                '%5%','%6%','%7%','%8%','%9%','%10%','%11%','%place_id%','%exam_id%','%form_id%'), array($v['rows'],$place_rows,$exam,$place,$ww['form'],$date,
                            $even ? 'even' : 'odd',$ww[5],$ww[6],$ww[7],$ww[8],$ww[9],$ww[10],$ww[11],$place_id,$exam_id,$form_id), $tpl['cipher']['stage_line' . ($newdate ? '1' : ($newplace ? '2' : 'N'))]);
                            $newdate = false;
                            $newplace = false;
                            $even = !$even;
                        }
                    }
                }
                return $stages;
            }

            $s = draw_stage_list($sta, 2, $sta_ex);
            if ($s) {
                $stages .= str_replace('%name%', 'Заключительный этап', $tpl['cipher']['stage_tline']) . $s;
            }
            $s = draw_stage_list($sta, 1, $sta_ex);
            if ($s) {
                $stages .= str_replace('%name%', 'Отборочный этап', $tpl['cipher']['stage_tline']) . $s;
            }
    /*$s=draw_stage_list($sta,0,$sta_ex);
    if ($s) $stages.=str_replace('%name%','Призеры прошлого года',$tpl['cipher']['stage_tline']).$s;*/
            $s = draw_stage_list($sta, 3, $sta_ex);
            if ($s) {
                $stages .= str_replace('%name%', 'Тренировочный этап', $tpl['cipher']['stage_tline']) . $s;
            }


            $body = str_replace(array('%pppages%','%sort_groups%','%stages%','%filter_places%','%filter_exams%','%filter_stages%','%filter_forms%','%filter_presences%','%filter_dates%','%filter_orgs%','%ShowRegsChecked%','%year%'), array($pppages,$sort_groups,$stages,$filter_places,$filter_exams,$filter_stages,$filter_forms,$filter_presences,$filter_dates,$filter_orgs,$ShowRegs > 0 ? ' checked ' : '',$year), $tpl['cipher']['stages_list']);

        }
    }

    return str_replace('%error%', $error, $body);
}

$tpl['cipher']['stages_list'] =
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
<tr><td colspan=5>Год проведения:&nbsp;<input name="year" type="text" value="%year%" size=10></td></tr>
<tr><td>Площадка:&nbsp;</td><td><select name="filter_place" size=0>%filter_places%</select></td><td> Форма:&nbsp;</td><td><select name="filter_form" size=0>%filter_forms%</select></td>
<td rowspan=3><button type=submit name="btnFilter"><br>Применить фильтр<br><br></button></td></tr>
<tr><td>Предмет:&nbsp;</td><td><select name="filter_exam" size=0>%filter_exams%</select></td><td> Этап:&nbsp;</td><td><select name="filter_stage" size=0>%filter_stages%</select></td></tr>
<tr><td>Участники:&nbsp;</td><td><select name="filter_presence" size=0>%filter_presences%</select></td><td> Дата:&nbsp;</td><td><select name="filter_date" size=0>%filter_dates%</select></td></tr>
<tr><td>Организаторы:&nbsp;</td><td><select name="filter_org" size=0>%filter_orgs%</select></td><td colspan=2><input type=checkbox name="ShowRegs" id="ShowRegs" %ShowRegsChecked%><label for="ShowRegs"> Показывать в ведомостях регион</label></td></tr>
</table>
<br>
<table cellpadding=2 id="stages">
<tr id=ttl><td>Дата</td><td>Площадка</td><td>Предмет</td><td>Форма</td><td>5</td><td>6</td><td>7</td><td>8</td><td>9</td><td>10</td><td>11</td><td>Всего</td></tr>
%stages%
</table>
<br>
</form>
<br><br>

';
$tpl['cipher']['stage_line1'] =
'<tr class=%even%><td rowspan="%dt_rows%">%dt%</td><td rowspan="%place_rows%">%place%</td><td>%exam%</td><td>%form%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum"><a href="?cmd=cipher&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%&form_id=%form_id%">[Шифровать]</a>&nbsp;&nbsp;<a href="?cmd=list&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%&form_id=%form_id%&lvl=3" target="_blank">[Шифр. ведомости]</a></td></tr>
';
$tpl['cipher']['stage_line2'] =
'<tr class=%even%><td rowspan="%place_rows%">%place%</td><td>%exam%</td><td>%form%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum"><a href="?cmd=cipher&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%&form_id=%form_id%">[Шифровать]</a>&nbsp;&nbsp;<a href="?cmd=list&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%&form_id=%form_id%&lvl=3" target="_blank">[Шифр. ведомости]</a></td></tr>
';
$tpl['cipher']['stage_lineN'] =
'<tr class=%even%><td>%exam%</td><td>%form%</td><td>%5%</td><td>%6%</td><td>%7%</td><td>%8%</td><td>%9%</td><td>%10%</td><td>%11%</td><td class="sum"><a href="?cmd=cipher&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%&form_id=%form_id%">[Шифровать]</a>&nbsp;&nbsp;<a href="?cmd=list&dt=%dt%&place_id=%place_id%&exam_id=%exam_id%&form_id=%form_id%&lvl=3" target="_blank">[Шифр. ведомости]</a></td></tr>
';
$tpl['cipher']['stage_tline'] = '<tr><td colspan=12 class=tline>%name%</td></tr>';
// шаблоны ведомостей

$tpl['cipher']['list_header'] = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
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
$tpl['cipher']['list'] = '<h1>Шифровальная ведомость</h1><h2>Группа %group_name%  (шифр %cipher%)</h2><h2>Предмет: %exam% (%classes%)</h2>
<table cellspacin=1 cellpadding=2 border=0>
<tr bgcolor="#CCCCff"><td style="width:10cm"  class="first">ФИО (дата рождения)</td><td style="width:1.8cm">Явка</td><td style="width:2cm"  class="first">Шифр</td></tr>
%lines%
</table>
<div style="page-break-after: always;"></div>
';
$tpl['cipher']['list_reg'] = '<h1>Шифровальная ведомость</h1><h2>Группа %group_name%  (шифр %cipher%)</h2><h2>Предмет: %exam% (%classes%)</h2>
<table cellspacin=1 cellpadding=2 border=0>
<tr bgcolor="#CCCCff"><td style="width:10cm"  class="first">ФИО (дата рождения)</td><td style="width:2cm">Регион</td><td style="width:1.8cm">Явка</td><td style="width:2cm"  class="first">Шифр</td></tr>
%lines%
</table>
<div style="page-break-after: always;"></div>
';
$tpl['cipher']['list_line'] = '<tr><td class="first">%fio% (%bdt%)</td><td>%presence%</td><td class="first" style="white-space:nowrap;">%cipher%</td></tr>';
$tpl['cipher']['list_line_reg'] = '<tr><td class="first">%fio% (%bdt%)</td><td>%region%</td><td>%presence%</td><td class="first" style="white-space:nowrap;">%cipher%</td></tr>';
$tpl['res']['task_cell_head'] = '';
$tpl['res']['list_task_cell'] = '';
$tpl['cipher']['empty'] = '<h2>Шифровка не выполнялась или нет участников выбранного организатора</h2>';
$body = build_page();
