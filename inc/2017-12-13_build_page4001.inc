<?php

// Личный кабинет. Участие в этапах.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');
include_once('../inc/cabinet2.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    if ($USER_INFO['uid'] < 1) {
        return $tpl['cabinet']['logon_form'];
    }
    $check_result = check_persons_data($USER_INFO['pid']);

    $selected_tab = trim($_REQUEST['selected_tab']);
    if (empty($selected_tab)) {
        $selected_tab = 2;
    }
    $selected_stage = trim($_REQUEST['active_stage']) + 0;

    $cmd = trim($_REQUEST['cmd']);

    $olimp_exams = '';
    $res = $mysqli->query('select * from olimp_exams');
    while ($row = $res->fetch_assoc()) {
        $olimp_exams[$row['id']] = $row;
    }
    $olimp_forms = '';
    $res = $mysqli->query('select * from olimp_forms');
    while ($row = $res->fetch_assoc()) {
        $olimp_forms[$row['id']] = $row;
    }
    $olimp_stage_types = '';
    $res = $mysqli->query('select * from olimp_stage_types');
    while ($row = $res->fetch_assoc()) {
        $olimp_stage_types[$row['id']] = $row;
    }
    $olimp_places = '';
    $res = $mysqli->query('select * from olimp_places');
    while ($row = $res->fetch_assoc()) {
        $olimp_places[$row['id']] = $row;
    }


    $reg_state_txt = array(0 => '',1 => 'планируется', 2 => 'идет регистрация', 3 => 'регистрация завершена', 4 => 'этап завершен');


// Перерегистрация участников прошлого года
    $message = '';
    include_once('../inc/reregistrate.inc');

    $user_actions = '';
    $res = $mysqli->query('select a.stage_id, a.group_id, a.presence, a.`rank`, g.name as group_name, s.stage as stage, s.exam as exam from olimp_actions as a left join olimp_groups as g on (g.id=a.group_id) left join olimp_stages as s on (s.id=a.stage_id) where a.person_id=' . $USER_INFO['pid']);
    while ($row = $res->fetch_assoc()) {
        $olimp_actions[$row['stage_id']] = $row;
    }


    $all_regs = '';
    $stage_pass = '';
    foreach ($olimp_stage_types as $s => $v) {
        foreach ($olimp_exams as $e => $w) {   // ??????????????????????????????????????????
            $all_regs[$s][$e] = false;
            $stage_pass[$s][$e] = false;
        }
    }
    if (is_array($olimp_actions)) {
        foreach ($olimp_actions as $row) {
            if (($row['presence'] != 'N') && ($row['presence'] != 'D') && ($row['presence'] != 'E')) {
                $all_regs[$row['stage']][$row['exam']] = true;
            }
            if ($row['rank'] > '1') {
                $stage_pass[$row['stage'] + 1][$row['exam']] = true;
            }
        }
    }

    if ($_SESSION['debug'] || $_REQUEST['debug']) {
        print_r($all_regs);
    }

    $actions_changed = false;


    $cur_dt = date('Ymd');
    $cur_dtt = date('YmdHis');
    $cur_date = date('Y-m-d');


// Действия

    switch ($cmd) {
        case 'unreg1':  // Отмена регистрации
        {    $selected_tab = 2;
            $res = $mysqli->query('select presence from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')');
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'L') {
                    $query = 'update olimp_actions set presence="D" where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                } else {
                    $query = 'delete from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                }
                if ($mysqli->query($query)) {
                    $message = 'Регистрация на участие отменена';
                }
            }
            $actions_changed = true;
            break;
        }
        case 'reg1':  // Регистрация
        {


    //  !!!!!!!!!!!!!!!!!!!!!     Нужно добавить проверку, что на этот этап можно регистрироваться...    !!!!!!!!!!!!!!!!!!!!!!


            if ($check_result['All is entered']) {
                $selected_tab = 2;

                $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
                $mysqli->query('CALL addToGroup(' . $USER_INFO['pid'] . ', ' . ($_POST['stage_id'] + 0) . ', @G)');
                $mysqli->query('UNLOCK TABLES');
                $res = $mysqli->query('select @G');
                if ($row = $res->fetch_row()) {
                    $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $_POST['stage_id']);
                    $row2 = $res2->fetch_assoc();

                    if ($row2['form'] == 1) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text1r']
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message']
                        );
                    }
                    // Выдаем вариант

                    if ($row2['form'] == 2) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text1rz']
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message']
                        );
                        if (!empty($row2['var_prefix'])) {
                            $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')');
                            $row3 = $res3->fetch_row();
                            if ($row3[0] != 'L') {
                                $vars = '';
                                for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                    $vars[$i] = $i;
                                }
                                shuffle($vars);
                                $varnum = $vars[0];
                                $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')');
                            }
                        }
                    }
                }
            } else {
                $error = $tpl['cabinet_stages']['error_personal_data'];
            }
            $actions_changed = true;
            break;
        }
        case 'unreg2':  // Отмена регистрации на закл. этап
        {    $selected_tab = 3;
            $res = $mysqli->query('select presence from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')');
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'L') {
                    $query = 'update olimp_actions set presence="D" where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                } else {
                    $query = 'delete from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                }
                if ($mysqli->query($query)) {
                    $message = 'Регистрация на участие отменена';
                }
            }
            $actions_changed = true;
            break;
        }
        case 'reg2':  // Регистрация на закл. этап
        {


    //  !!!!!!!!!!!!!!!!!!!!!     Проверка, что на этот этап можно регистрироваться...    !!!!!!!!!!!!!!!!!!!!!!


            $res = $mysqli->query('select exam,classes,stage,DATE_FORMAT(date_breg,"%Y%m%d") as bdt, DATE_FORMAT(date_ereg,"%Y%m%d") as edt from olimp_stages where id=' . ($_POST['stage_id'] + 0));
            $row_stage = $res->fetch_assoc();

    // На этот предмет можно регистрироваться
            if (!$stage_pass[2][$row_stage['exam']]) {
                break;
            }

    // На этот предмет ещё не зарегистрирован
            if ($all_regs[2][$row_stage['exam']]) {
                break;
            }

    // ...

            $selected_tab = 3;

            $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
            $mysqli->query('CALL addToGroup(' . $USER_INFO['pid'] . ', ' . ($_POST['stage_id'] + 0) . ', @G)');
            $mysqli->query('UNLOCK TABLES');
            $res = $mysqli->query('select @G');
            if ($row = $res->fetch_row()) {
                $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $_POST['stage_id']);
                $row2 = $res2->fetch_assoc();

                $message =
                str_replace(
                    array('%stage_id%','%dialog_title%','%dialog_text%'),
                    array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0],
                        str_replace(
                            array('%dt%','%exam%','%place%','%group%'),
                            array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                            $tpl['cabinet_stages']['action_text2r']
                        )
                    ),
                    $tpl['cabinet_stages']['modal_message']
                );
            }
            $actions_changed = true;
            break;
        }

    // Тренировочный этап

        case 'unreg3':  // Отмена регистрации
        {    $selected_tab = 4;
            $res = $mysqli->query('select presence from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')');
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'L') {
                        $query = 'update olimp_actions set presence="D" where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                } else {
                    $query = 'delete from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                }
                if ($mysqli->query($query)) {
                    $message = 'Регистрация на участие отменена';
                }
            }
            $actions_changed = true;
            break;
        }
        case 'reg3':  // Регистрация
        {


    //  !!!!!!!!!!!!!!!!!!!!!     Нужно добавить проверку, что на этот этап можно регистрироваться...    !!!!!!!!!!!!!!!!!!!!!!

            $selected_tab = 4;
            if ($check_result['All is entered']) {
                $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
                $mysqli->query('CALL addToGroup(' . $USER_INFO['pid'] . ', ' . ($_POST['stage_id'] + 0) . ', @G)');
                $mysqli->query('UNLOCK TABLES');
                $res = $mysqli->query('select @G');
                if ($row = $res->fetch_row()) {
                    $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $_POST['stage_id']);
                    $row2 = $res2->fetch_assoc();

                    if ($row2['form'] == 1) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text3r']
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message']
                        );
                    }
                    // Выдаем вариант

                    if ($row2['form'] == 2) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text3rz']
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message']
                        );
                        if (!empty($row2['var_prefix'])) {
                            $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')');
                            $row3 = $res3->fetch_row();
                            if ($row3[0] != 'L') {
                                $vars = '';
                                for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                    $vars[$i] = $i;
                                }
                                shuffle($vars);
                                $varnum = $vars[0];
                                $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')');
                            }
                        }
                    }
                }
            } else {
                $error = $tpl['cabinet_stages']['error_personal_data'];
            }
            $actions_changed = true;
            break;
        }
    } // switch($cmd)


// Вывод

    if ($actions_changed) {
        $olimp_actions = '';
        $res = $mysqli->query('select a.stage_id, a.group_id, a.presence, a.`rank`, g.name as group_name, s.stage as stage, s.exam as exam from olimp_actions as a left join olimp_groups as g on (g.id=a.group_id) left join olimp_stages as s on (s.id=a.stage_id) where a.person_id=' . $USER_INFO['pid']);
        while ($row = $res->fetch_assoc()) {
            $olimp_actions[$row['stage_id']] = $row;
        }

        $all_regs = '';
        $stage_pass = '';
        foreach ($olimp_stage_types as $s => $v) {
            foreach ($olimp_exams as $e => $w) {           /// ????????????????????????????????????????????
                $all_regs[$s][$e] = false;
                $stage_pass[$s][$e] = false;
            }
        }
        foreach ($olimp_actions as $row) {
            if (($row['presence'] != 'N') && ($row['presence'] != 'D') && ($row['presence'] != 'E')) {
                $all_regs[$row['stage']][$row['exam']] = true;
            }
            if ($row['rank'] > '1') {
                $stage_pass[$row['stage'] + 1][$row['exam']] = true;
            }
        }

        if ($_SESSION['debug'] || $_REQUEST['debug']) {
            print_r($all_regs);
        }
    }

// Завершенные этапы

        $stage = '';
        $stage_num = 0;
        $stages = '';
                $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes from olimp_stages as s where (s.date_exam<"' . $cur_date . '") AND (FIND_IN_SET("' . $USER_INFO['class'] . '",s.classes)>0) order by s.date_exam desc');
    while ($row = $res->fetch_assoc()) {
        $reg_bdt = str_replace('-', '', $row['date_breg']);
        $reg_edt = str_replace('- :', '', $row['date_ereg']);

        $reg_state = 3;
//                    $title_icons=str_replace('%icon%',($reg_stage==2 ? 'ui-icon-unlocked' : 'ui-icon-locked'),$tpl['cabinet_stages']['stage_title_icon']);
        $buttons = '';
        $cur_reg = '';
        if (is_array($olimp_actions[$row['id']])) {
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                $cur_reg = str_replace('%group%', $olimp_actions[$row['id']]['group_name'], $tpl['cabinet_stages']['stage_current_reg']);
            }
        }


        $stage = preg_replace(
            array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
            '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
            '/%dates_exam%/',
            '/%dates_reg%/',
            '/%current_reg%/','/%buttons%/'
            ),
            array($row['exam_dt'],$olimp_exams[$row['exam']]['name'], $olimp_stage_types[$row['stage']]['name'],$olimp_forms[$row['form']]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), $tpl['cabinet_stages']['stage_date_exam' . $row['form']]),
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]),
            $cur_reg,''
            ),
            $tpl['cabinet_stages']['stage']
        );



        $stages .= $stage;
        $stage_num++;
    }
                $content1 = preg_replace(
                    array('/%num%/','/%selected_stage%/','/%stages%/'),
                    array(1,$selected_stage + 0,$stages),
                    $tpl['cabinet_stages']['stages']
                );



// Отборочные этапы не завершенные
        $stage = '';
        $stage_num = 0;
        $stages = '';
                $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes from olimp_stages as s where (s.stage=1) AND ((s.date_exam>="' . $cur_date . '") OR (s.date_post>="' . $cur_date . '")) AND (FIND_IN_SET("' . $USER_INFO['class'] . '",s.classes)>0) order by s.date_exam desc');
    while ($row = $res->fetch_assoc()) {
        $reg_bdt = str_replace('-', '', $row['date_breg']);
        $reg_edt = str_replace(array('-',' ',':'), '', $row['date_ereg']);
        $exam_dtt = preg_replace('/[\-\ \.\:]/', '', $row['date_exam']);

        $reg_state = 0;
        if ($cur_dt < $reg_bdt) {
            $reg_state = 1;
        }
        if ($cur_dtt > $reg_edt) {
            $reg_state = 3;
        }
        if (($cur_dt >= $reg_bdt) && ($cur_dtt <= $reg_edt)) {
            $reg_state = 2;
        }

//if ($_SESSION['debug']>0) echo "REG SATE = $reg_state  dt=$cur_dtt  edt=$reg_edt exam=$exam_dtt<hr>";

//                    $title_icons=str_replace('%icon%',($reg_stage==2 ? 'ui-icon-unlocked' : 'ui-icon-locked'),$tpl['cabinet_stages']['stage_title_icon']);
        $buttons = '';
        $cur_reg = '';
        if (is_array($olimp_actions[$row['id']])) {
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                $cur_reg = str_replace('%group%', $olimp_actions[$row['id']]['group_name'], $tpl['cabinet_stages']['stage_current_reg']);
            }



            if (($reg_state == 2) && ($cur_dtt < $exam_dtt)) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%','%button_text1%','%button_text2%',
                    '%dialog_title%',
                    '%dialog_text%'
                    ),
                    array($stage_num,$row['id'],'unreg1','Отказаться от участия в этом этапе','Да, отказаться от участия','Нет, не отказываться',
                    'Требуется подтверждение отказа',
                    str_replace(
                        array('%dt%','%exam%','%place%'),
                        array($row['exam_dt'],strtolower($olimp_exams[$row['exam']]['name']),$olimp_places[$row['place']]['name']),
                        $tpl['cabinet_stages']['action_text1u']
                    )
                    ),
                    $tpl['cabinet_stages']['button_confirm']
                );
            }
        }

        if ((!$all_regs[$row['stage']][$row['exam']]) && ($reg_state == 2)) {
            if ($check_result['All is entered']) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg1','Зарегистрироваться на участие в этом этапе'),
                    $tpl['cabinet_stages']['button']
                );
            } else {
                $buttons = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages']['error_personal_data']), $tpl['common']['error']),
                    $tpl['cabinet']['error']
                );
            }
        }

        $stage = preg_replace(
            array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
            '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
            '/%dates_exam%/',
            '/%dates_reg%/',
            '/%current_reg%/','/%buttons%/'
            ),
            array($row['exam_dt'],$olimp_exams[$row['exam']]['name'], $olimp_stage_types[$row['stage']]['name'],$olimp_forms[$row['form']]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), $tpl['cabinet_stages']['stage_date_exam' . $row['form']]),
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]),
            $cur_reg,$buttons
            ),
            $tpl['cabinet_stages']['stage']
        );



        $stages .= $stage;
        $stage_num++;
    }
                $content2 = preg_replace(
                    array('/%num%/','/%selected_stage%/','/%stages%/'),
                    array(2,$selected_stage + 0,$stages),
                    $tpl['cabinet_stages']['stages']
                );



// Тренировочный этап


// этапы не завершенные
        $stage = '';
        $stage_num = 0;
        $stages = '';
                $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes from olimp_stages as s where (s.stage=3) AND ((s.date_exam>="' . $cur_date . '") OR (s.date_post>="' . $cur_date . '")) AND (FIND_IN_SET("' . $USER_INFO['class'] . '",s.classes)>0) order by s.date_exam desc');
    while ($row = $res->fetch_assoc()) {
        $reg_bdt = str_replace('-', '', $row['date_breg']);
        $reg_edt = str_replace(array('-',' ',':'), '', $row['date_ereg']);
        $exam_dtt = preg_replace('/[\-\ \.\:]/', '', $row['date_exam']);

        $reg_state = 0;
        if ($cur_dt < $reg_bdt) {
            $reg_state = 1;
        }
        if ($cur_dtt > $reg_edt) {
            $reg_state = 3;
        }
        if (($cur_dt >= $reg_bdt) && ($cur_dtt <= $reg_edt)) {
            $reg_state = 2;
        }


//                    $title_icons=str_replace('%icon%',($reg_stage==2 ? 'ui-icon-unlocked' : 'ui-icon-locked'),$tpl['cabinet_stages']['stage_title_icon']);
        $buttons = '';
        $cur_reg = '';
        if (is_array($olimp_actions[$row['id']])) {
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                $cur_reg = str_replace('%group%', $olimp_actions[$row['id']]['group_name'], $tpl['cabinet_stages']['stage_current_reg']);
            }

// if ($_SESSION['debug']>0) echo "REG SATE = $reg_state  $cur_dtt  $exam_dtt<hr>";

            if (($reg_state == 2) && ($cur_dtt < $exam_dtt)) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%','%button_text1%','%button_text2%',
                    '%dialog_title%',
                    '%dialog_text%'
                    ),
                    array($stage_num,$row['id'],'unreg3','Отказаться от участия в этом этапе','Да, отказаться от участия','Нет, не отказываться',
                    'Требуется подтверждение отказа',
                    str_replace(
                        array('%dt%','%exam%','%place%'),
                        array($row['exam_dt'],strtolower($olimp_exams[$row['exam']]['name']),$olimp_places[$row['place']]['name']),
                        $tpl['cabinet_stages']['action_text3u']
                    )
                    ),
                    $tpl['cabinet_stages']['button_confirm']
                );
            }
        }

        if ((!$all_regs[$row['stage']][$row['exam']]) && ($reg_state == 2)) {
            if ($check_result['All is entered']) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg3','Зарегистрироваться на участие в этом этапе'),
                    $tpl['cabinet_stages']['button']
                );
            } else {
                $buttons = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages']['error_personal_data']), $tpl['common']['error']),
                    $tpl['cabinet']['error']
                );
            }
        }

        $stage = preg_replace(
            array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
            '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
            '/%dates_exam%/',
            '/%dates_reg%/',
            '/%current_reg%/','/%buttons%/'
            ),
            array($row['exam_dt'],$olimp_exams[$row['exam']]['name'], $olimp_stage_types[$row['stage']]['name'],$olimp_forms[$row['form']]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), $tpl['cabinet_stages']['stage_date_exam' . $row['form']]),
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]),
            $cur_reg,$buttons
            ),
            $tpl['cabinet_stages']['stage']
        );



        $stages .= $stage;
        $stage_num++;
    }
                $content4 = preg_replace(
                    array('/%num%/','/%selected_stage%/','/%stages%/'),
                    array(4,$selected_stage + 0,$stages),
                    $tpl['cabinet_stages']['stages']
                );






// Заключительный этап

        $stage = '';
        $stage_num = 0;
        $stages = '';
        $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, s.limit, s.regs, s.exam, s.place, s.descr, s.classes as classes from olimp_stages as s where (s.stage=2) AND (FIND_IN_SET("' . $USER_INFO['class'] . '",s.classes)>0) order by s.exam,s.place');
    while ($row = $res->fetch_assoc()) {
        $reg_bdt = str_replace('-', '', $row['date_breg']);
        $reg_edt = str_replace(array('-',' ',':'), '', $row['date_ereg']);
        $exam_dtt = preg_replace('/[\-\ \.\:]/', '', $row['date_exam']);

        $reg_state = 0;
        if ($cur_dt < $reg_bdt) {
            $reg_state = 1;
        }
        if ($cur_dtt > $reg_edt) {
            $reg_state = 3;
        }
        if (($cur_dt >= $reg_bdt) && ($cur_dtt <= $reg_edt)) {
            $reg_state = 2;
        }

//            $title_icons=str_replace('%icon%',($reg_stage==2 ? 'ui-icon-unlocked' : 'ui-icon-locked'),$tpl['cabinet_stages']['stage_title_icon']);
        $buttons = '';
        $cur_reg = '';
        if (is_array($olimp_actions[$row['id']])) {
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                $cur_reg = str_replace('%group%', $olimp_actions[$row['id']]['group_name'], $tpl['cabinet_stages']['stage_current_reg']);
            }

            if (($reg_state == 2) && ($cur_dtt < $exam_dtt)) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%','%button_text1%','%button_text2%',
                    '%dialog_title%',
                    '%dialog_text%'
                    ),
                    array($stage_num,$row['id'],'unreg2','Отказаться от участия в этом этапе','Да, отказаться от участия','Нет, не отказываться',
                    'Требуется подтверждение отказа',
                    str_replace(
                        array('%dt%','%exam%','%place%'),
                        array($row['exam_dt'],strtolower($olimp_exams[$row['exam']]['name']),$olimp_places[$row['place']]['name']),
                        $tpl['cabinet_stages']['action_text2u']
                    )
                    ),
                    $tpl['cabinet_stages']['button_confirm']
                );
            }
        }

        if ((!$all_regs[2][$row['exam']]) && ($reg_state == 2)) {
            if ($check_result['All is entered']) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg2','Зарегистрироваться на участие в этом этапе'),
                    $tpl['cabinet_stages']['button']
                );
            } else {
                $buttons = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages']['error_personal_data']), $tpl['common']['error']),
                    $tpl['cabinet']['error']
                );
            }
        }

        $stage = preg_replace(
            array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
            '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
            '/%dates_exam%/',
            '/%dates_reg%/',
            '/%current_reg%/','/%buttons%/'
            ),
            array($row['exam_dt'],$olimp_exams[$row['exam']]['name'], $olimp_stage_types[$row['stage']]['name'],$olimp_forms[1]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), $tpl['cabinet_stages']['stage_date_exam' . $row['form']]),
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]),
            $cur_reg,$buttons
            ),
            $tpl['cabinet_stages']['stage']
        );


        if (is_array($stage_pass[2]) && $stage_pass[2][$row['exam']]) {
            $stages .= $stage;
            $stage_num++;
        }
    }
        $content3 = preg_replace(
            array('/%num%/','/%selected_stage%/','/%stages%/'),
            array(3,$selected_stage + 0,$stages),
            $tpl['cabinet_stages']['stages']
        );





// Формирование страницы

    if (!empty($error)) {
        $error = preg_replace('/%error%/', preg_replace(array('/%title%/','/%text%/'), array('Ошибка!',$error), $tpl['common']['error']), $tpl['cabinet']['error']);
    }
    if (!empty($warning)) {
        $warning = preg_replace('/%warning%/', preg_replace(
            array('/%title%/','/%text%/'),
            array('Внимание!',$warning),
            $tpl['common']['highlight']
        ), $tpl['cabinet']['warning']);
    }

    $body = preg_replace(
        array('/%error%/','/%warning%/','/%message%/','/%selected_tab%/','/%content1%/','/%content2%/','/%content3%/','/%content4%/'),
        array($error,$warning,$message,($selected_tab - 1),$content1,$content2,$content3,$content4),
        $tpl['cabinet_stages']['main']
    );

    return $body;
}
