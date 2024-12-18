<?php

// Личный кабинет. Участие в этапах.

include_once('common.php');
include_once('cabinet.php');
include_once('cabinet2.php');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;
    $strRespond = []; // $strRespond['error'] > 0 - признак ошибки. $strRespond['error_text'] - описание ошибки.
    $strRespond['Listeners']['count'] = 0;
    $strRespond['debug'] = "";

    if ($USER_INFO['uid'] < 1) {
        $strRespond['error'] = '1';
        $strRespond['error_text'] = 'Необходимо войти в личный кабинет';
        return $strRespond;
        //return $tpl['cabinet']['logon_form']; // tpl
    }

    if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) {
        $strRespond['error'] = '1';
        $strRespond['error_text'] = 'Личный кабинет закрыт.';
        return $strRespond;
        // return $tpl['cabinet']['closed']; // tpl
    }

    $check_result = check_persons_data($USER_INFO['pid']);
//$check_result['All is entered'] = true;
    $error = '';
    $warning = '';
    $max_file_size = 10485760;
    $docs_dir = '../docs/applications';
//if ($USER_INFO['pid']<10) print_r($check_result);

    $allowed_file_types = array(
        'Application' => array(/*'image/jpeg'=>0,*/'application/pdf' => 1/*,'application/x-rar-compressed'=>0*/),
    );

    $selected_tab = array_key_exists('selected_tab', $_REQUEST) ? (int)$_REQUEST['selected_tab'] : 0; // _REQUEST
    if (empty($selected_tab)) {
        $selected_tab = 3;
    }
    $selected_stage = array_key_exists('active_stage', $_REQUEST) ? (int)$_REQUEST['active_stage'] : 0; // _REQUEST

    $cmd = array_key_exists('cmd', $_REQUEST) ? trim($_REQUEST['cmd']) : ''; // _REQUEST

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
    $res = $mysqli->query('select * from olimp_places');
    while ($row = $res->fetch_assoc()) {
        $olimp_places[$row['id']] = $row;
    }
    $olimp_partners = array();
    $res = $mysqli->query('SELECT * FROM olimp_partners');
    while ($row = $res->fetch_assoc()) {
        foreach (explode(',', $row['exams']) as $exam) {
            $olimp_partners[$exam][] = [
                'id' => $row['id'],
                'name' => $row['name'],
            ];
        }
    }
    $olimp_reculcs = array();
    $res = $mysqli->query('SELECT * from olimp_reculc where person_id = ' . $USER_INFO['pid']);
    while ($row = $res->fetch_assoc()) {
        $olimp_reculcs[$row['exam']] = $row;
    }


    $reg_state_txt = array(0 => '',1 => 'планируется', 2 => 'идет регистрация', 3 => 'регистрация завершена', 4 => 'этап завершен');


// Перерегистрация участников прошлого года
    $message = '';
    include_once('reregistrate.php');


    $_CURR_YEAR = (int)$SITE_VARS['current_year'];

    $olimp_actions = array();
#select a.stage_id, a.group_id, a.presence, a.`rank`, g.name as group_name, s.stage as stage, s.exam as exam, s.form as form from olimp_actions as a left join olimp_groups as g on (g.id=a.group_id) left join olimp_stages as s on (s.id=a.stage_id) where s.year=2023 AND a.person_id=78503
    $res = $mysqli->query('select a.stage_id, a.group_id, a.presence, a.`rank`, g.name as group_name, s.stage as stage, s.exam as exam, s.form as form from olimp_actions as a left join olimp_groups as g on (g.id=a.group_id) left join olimp_stages as s on (s.id=a.stage_id) where s.year=' . $_CURR_YEAR . ' AND a.person_id=' . $USER_INFO['pid']);
    while ($row = $res->fetch_assoc()) {
        $olimp_actions[$row['stage_id']] = $row;
    }


    $all_regs = array();
    $stage_pass = array();
    foreach ($olimp_stage_types as $s => $v) {
        foreach ($olimp_exams as $e => $w) {
            $all_regs[$s][$e] = false;
            $all_regs_tk[$s][$e] = false;
            $stage_pass[$s][$e] = false;
        }
    }

//'R'-зарегистрировался, 'Y'-явка, 'N'-неявка, 'L'-работа загружена, 'D'-регистрация отменена, 'W'-ожидание работы, 'A'-работа аннулирована, 'E'-ошибка, 'P'-пропущен
    foreach ($olimp_actions as $row) {
        if ($row['form'] != 4) {  // творческие конкурсы
            if (($row['presence'] != 'N') && ($row['presence'] != 'D') && ($row['presence'] != 'E') && ($row['presence'] != 'P')) {
                $all_regs[$row['stage']][$row['exam']] = true;
            }
        } else {
            if (($row['presence'] != 'N') && ($row['presence'] != 'D') && ($row['presence'] != 'E')) {
                $all_regs_tk[$row['stage']][$row['exam']] = true;
            }
        }
        if ($row['rank'] > '1') {
            $stage_pass[$row['stage'] + 1][$row['exam']] = true;
        }
    }


//if ($_SESSION['debug'] || $_REQUEST['debug']) print_r($all_regs);

    $actions_changed = false;
    $reculcs_changed = false;


    $cur_dt = date('Ymd');
    $cur_dtt = date('YmdHis');
    $cur_date = date('Y-m-d');


// Действия

    switch ($cmd) {
        case 'unreg1':  // Отмена регистрации
        {   $strRespond['debug'] .= "unreg1 |";
            $selected_tab = 2;
            $res = $mysqli->query('select presence from olimp_actions where (stage_id=' . ((int)$_POST['stage_id']) . ') AND (person_id=' . $USER_INFO['pid'] . ')'); // _REQUEST
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'L') {
                    $query = 'update olimp_actions set presence="D" where (stage_id=' . ((int)$_POST['stage_id']) . ') AND (person_id=' . $USER_INFO['pid'] . ')'; // _REQUEST
                } else { 
                    $query = 'delete from olimp_actions where (stage_id=' . ((int)$_POST['stage_id']) . ') AND (person_id=' . $USER_INFO['pid'] . ')'; // _REQUEST
                }
                if ($mysqli->query($query)) {
                    $message = 'Регистрация на участие отменена';
                    $strRespond['error'] = 0;
                    $strRespond['status_text'] = $message;
                }
            }
            $actions_changed = true;
            break;
        }
        case 'reg1':  // Регистрация
        {
            $strRespond['debug'] .= "reg1 |"; 

    //  !!!!!!!!!!!!!!!!!!!!!     Нужно добавить проверку, что на этот этап можно регистрироваться...    !!!!!!!!!!!!!!!!!!!!!!


            if ($check_result['All is entered']
                &&
                ($check_result['printed_z'] == 'L' || $check_result['printed_z'] == 'A')
                &&
                (!isset($olimp_reculcs[$row['exam']]) || $olimp_reculcs[$row['exam']]['status'] == 'D' || $olimp_reculcs[$row['exam']]['status'] == 'N')
            ) {
                $selected_tab = 2;
                $strRespond['debug'] .= "179 |";
                $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
                $mysqli->query('CALL addToGroup(' . $USER_INFO['pid'] . ', ' . ((int)$_POST['stage_id']) . ', @G)'); // _REQUEST
                $mysqli->query('UNLOCK TABLES');
                $res = $mysqli->query('select @G');
                if ($row = $res->fetch_row()) {
                    $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $_POST['stage_id']);
                    $row2 = $res2->fetch_assoc();
                    $strRespond['debug'] .= "187 |";
                    if ($row2['form'] == 1) {
                        $strRespond['debug'] .= "189 |";
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0], // _REQUEST
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text1r'] // tpl
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message'] // tpl
                        );
                        $strRespond['error'] = 0;
                        $strRespond['status_text'] = $message;
                    }
                    // Выдаем вариант

                    if (($row2['form'] == 2) || ($row2['form'] == 4) || ($row2['form'] == 6)) {
                        $strRespond['debug'] .= "208 |";
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0], // _REQUEST
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text1rz'] // tpl
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message'] // tpl
                        );
                        $strRespond['error'] = 0;
                        $strRespond['status_text'] = $message;
                        $strRespond['debug'] .= "221 |";
                        if (!empty($row2['var_prefix'])) {
                            $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ((int)$_POST['stage_id']) . ')'); // _REQUEST
                            $row3 = $res3->fetch_row();
                            if ($row3[0] != 'L') {
                                $vars = array();
                                for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                    $vars[$i] = $i;
                                }
                                shuffle($vars);
                                $varnum = $vars[0];
                                $strRespond['debug'] .= "232 |";
                                $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ((int)$_POST['stage_id']) . ')'); // _REQUEST
                            }
                        }
                    }
                }
            } else {                 
                $strRespond['debug'] .= "241 |";
                $error = $tpl['cabinet_stages'][$check_result['All is entered'] ? 'error_application' : 'error_personal_data']; // tpl
                $strRespond['error'] = 1;
                $strRespond['error_text'] = $error;
            }
            $actions_changed = true;
            break;
        }
        case 'unreg2':  // Отмена регистрации на закл. этап
        {    $selected_tab = 3;
            $res = $mysqli->query('select presence from olimp_actions where (stage_id=' . ((int)$_POST['stage_id']) . ') AND (person_id=' . $USER_INFO['pid'] . ')'); // _REQUEST
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'L') {
                    $query = 'update olimp_actions set presence="D" where (stage_id=' . ((int)$_POST['stage_id']) . ') AND (person_id=' . $USER_INFO['pid'] . ')'; // _REQUEST
                } else {
                    $query = 'delete from olimp_actions where (stage_id=' . ((int)$_POST['stage_id']) . ') AND (person_id=' . $USER_INFO['pid'] . ')'; // _REQUEST
                }
                if ($mysqli->query($query)) {
                    $message = 'Регистрация на участие отменена';
                    $strRespond['error'] = 0;
                    $strRespond['status_text'] = $message;
                }
            }
            $actions_changed = true;
            break;
        }
        case 'reg2':  // Регистрация на закл. этап
        {


    //  !!!!!!!!!!!!!!!!!!!!!     Проверка, что на этот этап можно регистрироваться...    !!!!!!!!!!!!!!!!!!!!!!


            $res = $mysqli->query('select exam,classes,stage,DATE_FORMAT(date_breg,"%Y%m%d") as bdt, DATE_FORMAT(date_ereg,"%Y%m%d") as edt from olimp_stages where id=' . ($_POST['stage_id'] + 0)); // _REQUEST
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
            $mysqli->query('CALL addToGroup(' . $USER_INFO['pid'] . ', ' . ((int)$_POST['stage_id']) . ', @G)'); // _REQUEST
            $mysqli->query('UNLOCK TABLES');
            $res = $mysqli->query('select @G');
            if ($row = $res->fetch_row()) {
                $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $_POST['stage_id']); // _REQUEST
                $row2 = $res2->fetch_assoc();
                if ($row2['form'] == 1) {
                    $message =
                    str_replace(
                        array('%stage_id%','%dialog_title%','%dialog_text%'),
                        array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0], // _REQUEST
                        str_replace(
                            array('%dt%','%exam%','%place%','%group%'),
                            array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                            $tpl['cabinet_stages']['action_text2r'] // tpl
                        )
                        ),
                        $tpl['cabinet_stages']['modal_message'] // tpl
                    );
                    $strRespond['error'] = 0;
                    $strRespond['status_text'] = $message;
                }

                // Выдаем вариант

                if (($row2['form'] == 2) || ($row2['form'] == 4) || ($row2['form'] == 6)) {
                    $message =
                    str_replace(
                        array('%stage_id%','%dialog_title%','%dialog_text%'),
                        array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0], // _REQUEST
                        str_replace(
                            array('%dt%','%exam%','%place%','%group%'),
                            array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                            $tpl['cabinet_stages']['action_text2rz'] // tpl
                        )
                        ),
                        $tpl['cabinet_stages']['modal_message'] // tpl
                    );
                    $strRespond['error'] = 0;
                    $strRespond['status_text'] = $message;

                    if (!empty($row2['var_prefix'])) {
                        $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')'); // _REQUEST
                        $row3 = $res3->fetch_row();
                        if ($row3[0] != 'L') {
                            $vars = array();
                            for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                $vars[$i] = $i;
                            }
                                shuffle($vars);
                                $varnum = $vars[0];
                                $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')'); // _REQUEST
                        }
                    }
                }
            }
            $actions_changed = true;
            break;
        }

    // Тренировочный этап

        case 'unreg3':  // Отмена регистрации
        {    $selected_tab = 4;
            $res = $mysqli->query('select presence from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')'); // _REQUEST
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'L') {
                    $query = 'update olimp_actions set presence="D" where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')'; // _REQUEST
                } else {
                    $query = 'delete from olimp_actions where (stage_id=' . ($_POST['stage_id'] + 0) . ') AND (person_id=' . $USER_INFO['pid'] . ')'; // _REQUEST
                }
                if ($mysqli->query($query)) {
                    $message = 'Регистрация на участие отменена';
                    $strRespond['error'] = 0;
                    $strRespond['status_text'] = $message;
                }
            }
            $actions_changed = true;
            break;
        }
        case 'reg3':  // Регистрация
        {


    //  !!!!!!!!!!!!!!!!!!!!!     Нужно добавить проверку, что на этот этап можно регистрироваться...    !!!!!!!!!!!!!!!!!!!!!!

            $selected_tab = 4;
            if ($check_result['All is entered'] && ($check_result['printed_z'] == 'L' || $check_result['printed_z'] == 'A') && (($check_result['IdentDoc'] == 'Accepted') || ($check_result['IdentDoc'] == 'Loaded') || ($check_result['IdentDoc'] == 'Updated'))) {
                $mysqli->query('LOCK TABLES olimp_groups WRITE, olimp_groups as g READ, olimp_actions WRITE, olimp_actions as a READ, olimp_stages as s WRITE');
                $mysqli->query('CALL addToGroup(' . $USER_INFO['pid'] . ', ' . ((int)$_POST['stage_id']) . ', @G)'); // _REQUEST
                $mysqli->query('UNLOCK TABLES');
                $res = $mysqli->query('select @G');
                if ($row = $res->fetch_row()) {
                    $res2 = $mysqli->query('select DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, s.exam, s.form, s.stage, s.place, s.var_prefix, s.min_var, s.max_var from olimp_stages as s where s.id=' . $_POST['stage_id']); // _REQUEST
                    $row2 = $res2->fetch_assoc();

                    if ($row2['form'] == 1) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0],
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text3r'] // tpl
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message'] // tpl
                        );
                        $strRespond['error'] = 0;
                        $strRespond['status_text'] = $message;
                    }
                    // Выдаем вариант

                    if (($row2['form'] == 2) || ($row2['form'] == 4) || ($row2['form'] == 6)) {
                        $message =
                        str_replace(
                            array('%stage_id%','%dialog_title%','%dialog_text%'),
                            array($_POST['stage_id'],'Вы зарегистрированы в группу ' . $row[0], // _REQUEST
                            str_replace(
                                array('%dt%','%exam%','%place%','%group%'),
                                array($row2['exam_dt'],$olimp_exams[$row2['exam']]['name'],$olimp_places[$row2['place']]['name'],$row[0]),
                                $tpl['cabinet_stages']['action_text3rz'] // tpl
                            )
                            ),
                            $tpl['cabinet_stages']['modal_message'] // tpl
                        );
                        $strRespond['error'] = 0;
                        $strRespond['status_text'] = $message;
                        if (!empty($row2['var_prefix'])) {
                            $res3 = $mysqli->query('select presence from olimp_actions where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')'); // _REQUEST 
                            $row3 = $res3->fetch_row();
                            if ($row3[0] != 'L') {
                                $vars = array();
                                for ($i = $row2['min_var']; $i <= $row2['max_var']; $i++) {
                                    $vars[$i] = $i;
                                }
                                shuffle($vars);
                                $varnum = $vars[0];
                                $mysqli->query('update olimp_actions set varnum=' . $varnum . ' where (person_id=' . $USER_INFO['pid'] . ') AND (stage_id=' . ($_POST['stage_id'] + 0) . ')'); // _REQUEST
                            }
                        }
                    }
                }
            } else {
                $error = $tpl['cabinet_stages'][$check_result['All is entered'] ? 'error_application' : 'error_personal_data']; // tpl
            }
            $actions_changed = true;
            break;
        }

        // Перезачёт

        case 'reg_reculc':
        {
            $ex = (int)$_POST['active_stage']; // _REQUEST
            if ($check_result['All is entered'] &&
                ($check_result['printed_z'] == 'L' || $check_result['printed_z'] == 'A')
            ) {
                if (!isset($olimp_reculcs[$ex]) || $olimp_reculcs[$ex]['status'] == 'N') {
                    if ($olimp_reculcs[$ex]['status'] == 'N') {
                        $res = $mysqli->query('DELETE FROM olimp_reculc WHERE (exam=' . (int)$_POST['active_stage'] . ') AND (person_id=' . $USER_INFO['pid'] . ')'); // _REQUEST
                        if (!$res) {
                            $error = 'Ошибка. Что-то пошло не так. Заявка не отправлена.';
                            $strRespond['error'] = 1;
                            $strRespond['error_text'] = $error;
                            break;
                        }
                    }
                    if (is_uploaded_file($_FILES['reculcApplication']['tmp_name'])) { // _FILES
                        $FileType = mime_content_type($_FILES['reculcApplication']['tmp_name']); // _FILES
                        if (!array_key_exists($FileType, $allowed_file_types['Application'])) {
                            $error = 'Недопустимый формат файла. Заявление не загружено.';
                            $strRespond['error'] = 1;
                            $strRespond['error_text'] = $error;
                            break;
                        }

                        if ($_FILES['reculcApplication']['size'] <= $max_file_size) { // FILES
                            $application_file = $docs_dir . '/reculc_application' . $USER_INFO['pid'] . "_$ex";
                            if (file_exists($application_file)) {
                                $file_exist = true;
                            } else {
                                $file_exist = false;
                            }
                            if (move_uploaded_file($_FILES['reculcApplication']['tmp_name'], $application_file)) { // Финальная проверка успешности загружаемого файла // _FILES
                                $res = $mysqli->query('INSERT INTO olimp_reculc(`person_id`, `partner_id`, `exam`, `score`, `status`) 
                            VALUES(' . $USER_INFO['pid'] . ',' . $_POST['questionA'] . ',' . $_POST['active_stage'] . ',' . (int)$_POST['questionB'] . ', "R")'); // _REQUEST
                            } else {
                                if ($file_exist) {
                                    $error = 'Ошибка при загрузке файла. Новая заявка не отправлена.';
                                    $strRespond['error'] = 1;
                                    $strRespond['error_text'] = $error;
                                } else {
                                    $error = 'Ошибка при загрузке файла. Заявка не отправлена.';
                                    $strRespond['error'] = 1;
                                    $strRespond['error_text'] = $error;
                                }
                            }
                        } else {
                            $error = 'Слишком большой файл. Заявка не отправлена.';
                            $strRespond['error'] = 1;
                            $strRespond['error_text'] = $error;
                            break;
                        }
                    } else {
                        $error = 'Ошибка при загрузке файла, возможно слишком большой файл. Заявка не отправлена.';
                        $strRespond['error'] = 1;
                        $strRespond['error_text'] = $error;
                        break;
                    }
                } else {
                    $error = 'Заявка уже подана. Чтобы отправить заявку повторно, отзовите предыдущую.';
                    $strRespond['error'] = 1;
                    $strRespond['error_text'] = $error;
                    break;
                }
            } else {
                $error = $tpl['cabinet_stages'][$check_result['All is entered'] ? 'error_application' : 'error_personal_data']; // tpl
                $strRespond['error'] = 1;
                $strRespond['error_text'] = $error;
            }
            $reculcs_changed = true;
            break;
        }
        case 'unreg_reculc':
        {
            $res = $mysqli->query('select `status`, `stage_id` from olimp_reculc where (exam=' . (int)$_POST['active_stage'] . ') AND (person_id=' . $USER_INFO['pid'] . ')'); // _REQUEST
            if ($row = $res->fetch_row()) {
                if ($row[0] == 'Y') {
                    $mysqli->query("UPDATE olimp_actions SET presence='D'  WHERE person_id=" . $USER_INFO['pid'] . " AND stage_id=" . $row[1]);
                }
                $query = 'delete from olimp_reculc where (exam=' . (int)$_POST['active_stage'] . ') AND (person_id=' . $USER_INFO['pid'] . ')';
                if ($mysqli->query($query)) {
                    $olimp_reculcs[(int)$_POST['active_stage']]['status'] = 'D';
                    $message = 'Регистрация на участие отменена';
                    $strRespond['error'] = 0;
                    $strRespond['status_text'] = $message;
                }
            }
            break;
        }
    } // switch($cmd)


// Вывод

    if ($actions_changed) {
        $olimp_actions = array();
        $res = $mysqli->query('select a.stage_id, a.group_id, a.presence, a.`rank`, g.name as group_name, s.stage as stage, s.exam as exam,s.form as form from olimp_actions as a left join olimp_groups as g on (g.id=a.group_id) left join olimp_stages as s on (s.id=a.stage_id) where s.year=' . $_CURR_YEAR . ' AND a.person_id=' . $USER_INFO['pid']);
        while ($row = $res->fetch_assoc()) {
            $olimp_actions[$row['stage_id']] = $row;
        }

        $all_regs = array();
        $stage_pass = array();
        foreach ($olimp_stage_types as $s => $v) {
            foreach ($olimp_exams as $e => $w) {           /// ????????????????????????????????????????????
                $all_regs[$s][$e] = false;
                $all_regs_tk[$s][$e] = false;
                $stage_pass[$s][$e] = false;
            }
        }
        foreach ($olimp_actions as $row) {
            if ($row['form'] != 4) {  // творческие конкурсы
                if (($row['presence'] != 'N') && ($row['presence'] != 'D') && ($row['presence'] != 'E')) {
                    $all_regs[$row['stage']][$row['exam']] = true;
                }
            } else {
                if (($row['presence'] != 'N') && ($row['presence'] != 'D') && ($row['presence'] != 'E')) {
                    $all_regs_tk[$row['stage']][$row['exam']] = true;
                }
            }
            if ($row['rank'] > '1') {
                $stage_pass[$row['stage'] + 1][$row['exam']] = true;
            }
        }
        //if ($_SESSION['debug'] || $_REQUEST['debug']) print_r($all_regs);
    }
    if ($reculcs_changed) {
        $res = $mysqli->query('SELECT * from olimp_reculc where person_id = ' . $USER_INFO['pid'] . ' and exam = ' . (int)$_POST['active_stage']); // _REQUEST
        while ($row = $res->fetch_assoc()) {
            $olimp_reculcs[$row['exam']] = $row;
        }
    }


// Завершенные этапы

    $strRespond['completed_stages'] = '';
    $template_completed_stage = '
        <div class = "msg_simple" style="color: black">
        <h3>
            <div class="">%title_dt% %title_exam% %title_stage% (%title_form% форма)  %place% </div>
            <div>%title_reg%</div>
        </h3>
        <div>
        <span >Для учащихся %classes% классов</span><br>
        %dates_reg%<br>
        Этап проводится на площадке %place% по адресу %place_address% <br>
        %dates_exam%<br>
        %descr%<br>%current_reg%
        %buttons%<br>
        </div>
        </div>'; // На основе $tpl['cabinet_stages']['stage'].

        $stage = '';
        $stage_num = 0;
        $stages = '';
                $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes from olimp_stages as s where s.year=' . $_CURR_YEAR . ' AND (s.date_exam<"' . $cur_date . '") AND (FIND_IN_SET("' . $USER_INFO['class'] . '",s.classes)>0) order by s.date_exam desc');
    while ($row = $res->fetch_assoc()) {
        $reg_bdt = str_replace('-', '', $row['date_breg']);
        $reg_edt = str_replace('- :', '', $row['date_ereg']);

        $reg_state = 3;
//                    $title_icons=str_replace('%icon%',($reg_stage==2 ? 'ui-icon-unlocked' : 'ui-icon-locked'),$tpl['cabinet_stages']['stage_title_icon']); 
        $buttons = '';
        $cur_reg = '';
        if (array_key_exists($row['id'], $olimp_actions) && is_array($olimp_actions[$row['id']])) {
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                $cur_reg = str_replace(array('%group%','%info%'), array($olimp_actions[$row['id']]['group_name'],''), $tpl['cabinet_stages']['stage_current_reg']); // tpl
            }
        }


        $stage = preg_replace(
            array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
            '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
            '/%dates_exam%/',
            '/%dates_reg%/',
            '/%current_reg%/','/%buttons%/'
            ),
            @array($row['exam_dt'],$olimp_exams[$row['exam']]['name'], $olimp_stage_types[$row['stage']]['name'],$olimp_forms[$row['form']]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), array_key_exists('stage_date_exam' . $row['form'], $tpl['cabinet_stages']) ? $tpl['cabinet_stages']['stage_date_exam' . $row['form']] : ''), // tpl
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]), // tpl
            $cur_reg,''
            ),
            $tpl['cabinet_stages']['stage'] // tpl
        );

        $strRespond['completed_stages'] .= preg_replace(
            array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
            '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
            '/%dates_exam%/',
            '/%dates_reg%/',
            '/%current_reg%/','/%buttons%/'
            ),
            @array($row['exam_dt'],$olimp_exams[$row['exam']]['name'], $olimp_stage_types[$row['stage']]['name'],$olimp_forms[$row['form']]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), array_key_exists('stage_date_exam' . $row['form'], $tpl['cabinet_stages']) ? $tpl['cabinet_stages']['stage_date_exam' . $row['form']] : ''), // tpl
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]), // tpl
            $cur_reg,''
            ),
            $template_completed_stage // Для Мобильной версии.
        );
        
        
        $strRespond['error'] = 0;
        /*$strRespond['status_text'] = '$stage: line 592.';

        $strRespond[$stage_num]['title_dt'] = $row['exam_dt'];
        $strRespond[$stage_num]['title_exam'] = $olimp_exams[$row['exam']]['name'];
        $strRespond[$stage_num]['title_stage'] = $olimp_stage_types[$row['stage']]['name'];
        $strRespond[$stage_num]['title_form'] = $olimp_forms[$row['form']]['name'];
        $strRespond[$stage_num]['place'] = $olimp_places[$row['place']]['name'];
        $strRespond[$stage_num]['place_address'] = $olimp_places[$row['place']]['address'];
        $strRespond[$stage_num]['title_reg'] = $reg_state_txt[$reg_state];
        $strRespond[$stage_num]['descr'] = $row['descr'];
        $strRespond[$stage_num]['classes'] = $row['classes'];
        $strRespond[$stage_num]['dates_exam'] =  preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],$row['post_dt']), array_key_exists('stage_date_exam' . $row['form'], $tpl['cabinet_stages']) ? $tpl['cabinet_stages']['stage_date_exam' . $row['form']] : '');
        $strRespond[$stage_num]['dates_reg'] = preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]);
        $strRespond[$stage_num]['current_reg'] = $cur_reg;
        $strRespond[$stage_num]['buttons'] = '';
        */

        $stages .= $stage;
        $stage_num++;
    }
                $content1 = preg_replace(
                    array('/%num%/','/%selected_stage%/','/%stages%/'),
                    array(1,(int)$selected_stage,$stages),
                    $tpl['cabinet_stages']['stages'] // tpl
                );
                $strRespond['error'] = 0;
                $strRespond['status_text'] = '$content1: line 628.';
                $strRespond['content1']['num'] = 1;
                $strRespond['content1']['selected_stage'] = (int)$selected_stage;
                $strRespond['content1']['stages'] = $stages;




// Отборочные этапы не завершенные
        $strRespond['qualifying_stages'] = ''; 
        $stage = '';
        $stage_num = 0;
        $stages = '';
                //$res=$mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes from olimp_stages as s where s.year='.$_CURR_YEAR.' AND (s.stage=1) AND ((s.date_exam>="'.$cur_date.'") OR (s.date_post>="'.$cur_date.'")) AND (FIND_IN_SET("'.$USER_INFO['class'].'",s.classes)>0) order by s.date_exam desc');
                $res = $mysqli->query(
                    'select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, 
                            DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, 
                            s.descr, s.classes as classes, p.class_m, p.class_f, p.class_i, p.class_k 
                        from olimp_stages as s left join olimp_persons as p on (p.id = "' . $USER_INFO['pid'] . '") 
                        where s.year=' . $_CURR_YEAR . ' AND (s.stage=1) AND ((s.date_exam>="' . $cur_date . '") OR (s.date_post>="' . $cur_date . '")) AND (
                        FIND_IN_SET(
                            CASE 
                                WHEN (class_m >0 and s.exam=1) 
                                    THEN class_m 
                            WHEN (class_f >0 and s.exam=2) 
                                    THEN class_f 
                            WHEN (class_i >0 and s.exam=3) 
                                    THEN class_i 
                            WHEN (class_k >0 and s.exam=4) 
                                    THEN class_k 
                                ELSE 
                                    "' . $USER_INFO['class'] . '" 
                            END ,s.classes)>0) order by s.date_exam desc'
                );

#select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes, p.class_m, p.class_f, p.class_i, p.class_k from olimp_stages as s left join olimp_persons as p on (p.id = "78503") where s.year=2023 AND (s.stage=1) AND ((s.date_exam>="2023-05-10") OR (s.date_post>="2023-05-10")) AND (FIND_IN_SET(CASE WHEN (class_m >0 and s.exam=1) THEN class_m WHEN (class_f >0 and s.exam=2) THEN class_f WHEN (class_i >0 and s.exam=3) THEN class_i WHEN (class_k >0 and s.exam=4) THEN class_k ELSE "10" END ,s.classes)>0) order by s.date_exam desc
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
//'R'-зарегистрировался, 'Y'-явка, 'N'-неявка, 'L'-работа загружена, 'D'-регистрация отменена, 'W'-ожидание работы, 'A'-работа аннулирована, 'E'-ошибка, 'P'-пропущен
//                    $title_icons=str_replace('%icon%',($reg_stage==2 ? 'ui-icon-unlocked' : 'ui-icon-locked'),$tpl['cabinet_stages']['stage_title_icon']);
        $buttons = '';
        $cur_reg = '';
        $buttons_mobile = '';
        $cur_reg_mobile = '';
        
        $strRespond['debug'] .= " 730 | ";
        // Если поток с таким id есть в списке $olimp_actions
        if ( @is_array($olimp_actions[$row['id']])) { // ?. isset($olimp_acitons[$row['id']]) &&
            $strRespond['debug'] .= " 734 |";
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                $strRespond['debug'] .= " 736 |";
                $cur_reg = str_replace('%group%', $olimp_actions[$row['id']]['group_name'], $tpl['cabinet_stages']['stage_current_reg']); // tpl
                // $strRespond['error'] = 0;
                // $strRespond['status_text'] = '$content1: line 691.';
                $tpl_stage_current_reg = '<div class="ui-widget">
                    <div class="ui-state-highlight ui-corner-all" style="padding: 0 .7em;"> 
                    <p><span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>
                    <strong>Внимание!</strong> Вы зарегистрированы для участия в этом этапе. Номер группы: %group%</p><!--%info%-->
                </div></div>';
                $cur_reg_mobile = str_replace('%group%', $olimp_actions[$row['id']]['group_name'], $tpl_stage_current_reg);
            }

            // Если текущая дата позже начала рег, раньше конца рег, раньше начала экз, то можно отказаться
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
                        $tpl['cabinet_stages']['action_text1u'] // tpl
                    )
                    ),
                    $tpl['cabinet_stages']['button_confirm'] // tpl
                );

                $tpl_action_text1u = '<p style="margin-top:15px; font-weight:bold;">Уважаемый участник олимпиады!</p><p  style="margin:8px 0px; text-align:justify;">Вы намерены отказаться от участия в отборочном этапе олимпиады %dt% по предмету %exam% на площадке %place%. <p  style="margin:8px 0px; text-align:justify;">В этом случае Вы исключаетесь из списка участников на этот день и сможете зарегистрироваться на участие в отборочном этапе в другой день .<p  style="margin:8px 0px; text-align:justify;"> Пожалуйста, подтвердите ваше намерение.'; // $tpl['cabinet_stages']['action_text1u']
                $tpl_button_confirm = '<br>
                    <form action="#" method="post" id="formStageAction%stage_id%">

                    <input type="hidden" name="active_stage" value="%stage_num%">
                    <input type="hidden" name="stage_id" value="%stage_id%">
                    <input type="hidden" name="cmd" value="%cmd%">
                    <div style="text-align:center;">
                    <input type="button" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>
                    <div id="dialogStageAction%stage_id%" title="%dialog_title%" class = "msg_warning" style = "display: none">
                        <p>%dialog_text%</p>    
                        <input type="button" id="cancelStageAction%stage_id%" value="%button_text1%"></input>
                        <input type="button" id="closeBtn%stage_id%" value="%button_text2%"></input>
                    </div>

                    </div>
                    </form>
                    <script>
                        $("#btnStageAction%stage_id%").click(function(){
                            $("#dialogStageAction%stage_id%").slideDown(1000);
                        });
                        $("#closeBtn%stage_id%").click(function(){
                            $("#dialogStageAction%stage_id%").slideUp(1000);
                        });
                    </script>                    
                    '; // $tpl['cabinet_stages']['button_confirm']
                    
                
                $buttons_mobile = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%','%button_text1%','%button_text2%',
                    '%dialog_title%',
                    '%dialog_text%'
                    ),
                    array($stage_num,$row['id'],'unreg1','Отказаться от участия в этом этапе','Да, отказаться от участия','Нет, не отказываться',
                    'Требуется подтверждение отказа',
                    str_replace(
                        array('%dt%','%exam%','%place%'),
                        array($row['exam_dt'],strtolower($olimp_exams[$row['exam']]['name']),$olimp_places[$row['place']]['name']),
                        $tpl_action_text1u // tpl
                    )
                    ),
                    $tpl_button_confirm // tpl
                );
                $i = $strRespond['Listeners']['count'];
                $strRespond['Listeners'][$i]['elem_id'] = str_replace("%stage_id%", $row['id'], "cancelStageAction%stage_id%");
                $strRespond['Listeners'][$i]['type'] = 'cancel_registration';
                $strRespond['Listeners'][$i]['stage_id'] = $row['id'];
                $strRespond['Listeners']['count']++;
            }
        }

        if (((!$all_regs[$row['stage']][$row['exam']]) || ( $row['form'] == 4))
            &&
            (!$all_regs_tk[$row['stage']][$row['exam']])
            &&
            ($reg_state == 2)
            &&
            (!isset($olimp_reculcs[$row['exam']]) || $olimp_reculcs[$row['exam']]['status'] == 'D' || $olimp_reculcs[$row['exam']]['status'] == 'N')
        ) {
            if ($check_result['All is entered'] && ($check_result['printed_z'] == 'L' || $check_result['printed_z'] == 'A') && (($check_result['IdentDoc'] == 'Accepted') || ($check_result['IdentDoc'] == 'Loaded') || ($check_result['IdentDoc'] == 'Updated'))) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg1','Зарегистрироваться на участие в этом этапе'),
                    $tpl['cabinet_stages']['button'] // tpl
                );

                $tpl_button = '<form action="" method="post" id="formStageAction%stage_id%">
                    <input type="hidden" name="active_stage" value="%stage_num%">
                    <input type="hidden" name="stage_id" value="%stage_id%">
                    <input type="hidden" name="cmd" value="%cmd%">
                    <div style="text-align:center;">
                    <input type="button" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>

                    </div>
                    </form>';//$tpl['cabinet_stages']['button']
                $buttons_mobile = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg1','Зарегистрироваться на участие в этом этапе'),
                    $tpl_button // tpl
                );

                $i = $strRespond['Listeners']['count'];
                $strRespond['Listeners'][$i]['elem_id'] = str_replace("%stage_id%", $row['id'], "btnStageAction%stage_id%");
                $strRespond['Listeners'][$i]['type'] = 'registration';
                $strRespond['Listeners'][$i]['stage_id'] = $row['id'];
                $strRespond['Listeners']['count']++;
            } else {
                $buttons = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages'][$check_result['All is entered'] ? 'error_application' : 'error_personal_data']), $tpl['common']['error']), // tpl
                    $tpl['cabinet']['error'] // tpl
                );

                $tpl_error = '%error%<br>'; // $tpl['cabinet']['error'].
                $buttons_mobile = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages'][$check_result['All is entered'] ? 'error_application' : 'error_personal_data']), $tpl['common']['error']), // tpl
                    $tpl_error // tpl
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

        $template_qualifying_stage = '
            <div class = "msg_simple" style="color: black">
            <h3>
                <div class="">%title_dt% %title_exam% %title_stage% (%title_form% форма)  %place% </div>
                <div>%title_reg%</div>
            </h3>
            <div>
            <span >Для учащихся %classes% классов</span><br>
            %dates_reg%<br>
            Этап проводится на площадке %place% по адресу %place_address% <br>
            %dates_exam%<br>
            %descr%<br>%current_reg%
            %buttons%<br>
            </div>
            </div>
        ';

        $strRespond['qualifying_stages'] .= preg_replace(
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
            $cur_reg_mobile,$buttons_mobile
            ),
            $template_qualifying_stage
        );


        $stages .= $stage;
        $stage_num++;
    }
                $content2 = preg_replace(
                    array('/%num%/','/%selected_stage%/','/%stages%/'),
                    array(2, (int)$selected_stage, $stages),
                    $tpl['cabinet_stages']['stages']
                );



// Тренировочный этап


// этапы не завершенные
    $strRespond['traning_stages'] = '';    
        $stage = '';
        $stage_num = 0;
        $stages = '';
                //$res=$mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, s.var_pwd as varpwd, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes from olimp_stages as s where s.year='.$_CURR_YEAR.' AND (s.stage=3) AND ((s.date_exam>="'.$cur_date.'") OR (s.date_post>="'.$cur_date.'")) AND (FIND_IN_SET("'.$USER_INFO['class'].'",s.classes)>0) order by s.date_exam desc');
                $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, s.var_pwd as varpwd, s.limit, s.regs, s.exam, s.form, s.stage, s.place, s.descr, s.classes as classes, p.class_m, p.class_f, p.class_i, p.class_k from olimp_stages as s left join olimp_persons as p on (p.id = "' . $USER_INFO['pid'] . '") where s.year=' . $_CURR_YEAR . ' AND (s.stage=3) AND ((s.date_exam>="' . $cur_date . '") OR (s.date_post>="' . $cur_date . '")) AND (
                        FIND_IN_SET(
                            CASE 
                                WHEN (class_m >0 and s.exam=1) 
                                    THEN class_m 
                            WHEN (class_f >0 and s.exam=2) 
                                    THEN class_f 
                            WHEN (class_i >0 and s.exam=3) 
                                    THEN class_i 
                            WHEN (class_k >0 and s.exam=4) 
                                    THEN class_k 
                                ELSE 
                                    "' . $USER_INFO['class'] . '" 
                            END ,s.classes)>0) order by s.date_exam desc');
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
        $strRespond['buttons'] = "";
        $strRespond['cur_reg'] = "";
        $buttons_mobile = '';
        $cur_reg_mobile = '';
        $info_mobile = "";

        $info_template_mobile = '';
        if (array_key_exists($row['id'], $olimp_actions) && is_array($olimp_actions[$row['id']])) {    //if ($olimp_actions[$row['id']]['presence']!='D')
            //    $cur_reg=str_replace('%group%',$olimp_actions[$row['id']]['group_name'],$tpl['cabinet_stages']['stage_current_reg']);

            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                switch ($row['form']) {
                    case 2:
                    //$stage.=' в заочной форме';
                        $varpwd = '';
                        $delay = '';
                    //check_pwd_param($row['stage_id'],$varpwd,$delay);
                        $info = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['id'],$varpwd,$delay), $tpl['cabinet']['info_2']);
                        
                        $info_2_template = '<table id="stage_info%stage_id%"><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>';
                        $info_mobile = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['id'],$varpwd,$delay), $info_2_template);
                        break;
                    case 6:
                        check_vcl_param($row['id'], $vclink, $varpwd, $delay);
                        $info = str_replace(array('%stage_id%','%vclink%','%varpwd%','%delay%'), array($row['id'],$vclink,$varpwd,$delay), $tpl['cabinet']['info_6']);

                        $info_6_template = '<table id="stage_info%stage_id%"><tr><td>Ссылка для подключения к видеоконференции:</td><td id="vclink%stage_id%">%vclink%</td></tr><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>';
                        $info_mobile = str_replace(array('%stage_id%','%vclink%','%varpwd%','%delay%'), array($row['id'],$vclink,$varpwd,$delay), $info_6_template);
                        break;
                    default:
                        $info = '';
                        $info_mobile = '';
                }
                $cur_reg = str_replace(array('%group%','%info%'), array($olimp_actions[$row['id']]['group_name'],$info), $tpl['cabinet_stages']['stage_current_reg']);

                $template_stage_current_reg = '
                    <div class="ui-widget">
                    <div class="ui-state-highlight ui-corner-all" style="padding: 0 .7em;"> 
                    <p><span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>
                    <strong>Внимание!</strong> Вы зарегистрированы для участия в этом этапе. Номер группы: %group%</p><!--%info%-->
                    </div></div>
                ';
                $cur_reg_mobile = str_replace(array('%group%','%info%'), array($olimp_actions[$row['id']]['group_name'],$info_mobile), $template_stage_current_reg);
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

                $tpl_action_text3u = '<p style="margin-top:15px; font-weight:bold;">Уважаемый участник олимпиады!</p><p  style="margin:8px 0px; text-align:justify;">Вы намерены отказаться от участия в тренировочном этапе олимпиады %dt% по предмету %exam% на площадке %place%. <p  style="margin:8px 0px; text-align:justify;">В этом случае Вы исключаетесь из списка участников на этот день и сможете зарегистрироваться на участие в тренировочном этапе в другой день .<p  style="margin:8px 0px; text-align:justify;"> Пожалуйста, подтвердите ваше намерение.';
                $tpl_button_confirm = '<br>
                    <form action="#" method="post" id="formStageAction%stage_id%">

                    <input type="hidden" name="active_stage" value="%stage_num%">
                    <input type="hidden" name="stage_id" value="%stage_id%">
                    <input type="hidden" name="cmd" value="%cmd%">
                    <div style="text-align:center;">
                    <input type="button" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>
                    <div id="dialogStageAction%stage_id%" title="%dialog_title%" class = "msg_warning" style = "display: none">
                        <p>%dialog_text%</p>    
                        <input type="button" id="cancelStageAction%stage_id%" value="%button_text1%"></input>
                        <input type="button" id="closeBtn%stage_id%" value="%button_text2%"></input>
                    </div>

                    </div>
                    </form>
                    <script>
                        $("#btnStageAction%stage_id%").click(function(){
                            $("#dialogStageAction%stage_id%").slideDown(1000);
                        });
                        $("#closeBtn%stage_id%").click(function(){
                            $("#dialogStageAction%stage_id%").slideUp(1000);
                        });
                    </script>   
                ';
                
                $buttons_mobile = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%','%button_text1%','%button_text2%',
                    '%dialog_title%',
                    '%dialog_text%'
                    ),
                    array($stage_num,$row['id'],'unreg3','Отказаться от участия в этом этапе','Да, отказаться от участия','Нет, не отказываться',
                    'Требуется подтверждение отказа',
                    str_replace(
                        array('%dt%','%exam%','%place%'),
                        array($row['exam_dt'],strtolower($olimp_exams[$row['exam']]['name']),$olimp_places[$row['place']]['name']),
                        $tpl_action_text3u
                    )
                    ),
                    $tpl_button_confirm
                );
                $i = $strRespond['Listeners']['count'];
                $strRespond['Listeners'][$i]['elem_id'] = str_replace("%stage_id%", $row['id'], "cancelStageAction%stage_id%");
                $strRespond['Listeners'][$i]['type'] = 'cancel_registration';
                $strRespond['Listeners'][$i]['stage_id'] = $row['id'];
                $strRespond['Listeners']['count']++;
            }
        } elseif (($reg_state == 2)) { //if ((!$all_regs[$row['stage']][$row['exam']])&&($reg_state==2))
            if ($check_result['All is entered'] && ($check_result['printed_z'] == 'L' || $check_result['printed_z'] == 'A') && (($check_result['IdentDoc'] == 'Accepted') || ($check_result['IdentDoc'] == 'Loaded') || ($check_result['IdentDoc'] == 'Updated')) /* && ($check_result['Idents_loaded'])*/) {
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg3','Зарегистрироваться на участие в этом этапе'),
                    $tpl['cabinet_stages']['button']
                );
                $tpl_button = '
                    <br>
                    <form action="#" method="post" id="formStageAction%stage_id%">

                    <input type="hidden" name="active_stage" value="%stage_num%">
                    <input type="hidden" name="stage_id" value="%stage_id%">
                    <input type="hidden" name="cmd" value="%cmd%">
                    <div style="text-align:center;">
                    <input type="submit" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>

                    </div>
                    </form>
                ';
                $buttons_mobile = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg3','Зарегистрироваться на участие в этом этапе'),
                    $tpl_button
                );
                $i = $strRespond['Listeners']['count'];
                $strRespond['Listeners'][$i]['elem_id'] = str_replace("%stage_id%", $row['id'], "btnStageAction%stage_id%");
                $strRespond['Listeners'][$i]['type'] = 'registration';
                $strRespond['Listeners'][$i]['stage_id'] = $row['id'];
                $strRespond['Listeners']['count']++;
            } else {
                
                $buttons = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages'][$check_result['All is entered'] ? ($check_result['Idents_loaded'] ? 'error_application' : 'error_ident') : 'error_personal_data']), $tpl['common']['error']),
                    $tpl['cabinet']['error']
                );
                $tpl_error = '%error%<br>';
                $buttons_mobile = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages'][$check_result['All is entered'] ? ($check_result['Idents_loaded'] ? 'error_application' : 'error_ident') : 'error_personal_data']), $tpl['common']['error']),
                    $tpl_error
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

        $template_traning_stage = '
        <div class = "msg_simple" style="color: black">
        <h3><div>%title_dt% %title_exam% %title_stage% (%title_form% форма)  %place% </div><div>%title_reg%</div></h3>
        <div>
        <span>Для учащихся %classes% классов</span><br>
        %dates_reg%<br>
        Этап проводится на площадке %place% по адресу %place_address% <br>
        %dates_exam%<br>
        %descr%<br>%current_reg%
        %buttons%<br>
        </div>
        </div>
		'; // На основе $tpl['cabinet_stages']['stage']. // Тренировочный этап.

        $strRespond['traning_stages'] .= preg_replace(
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
            $cur_reg_mobile, $buttons_mobile
            ),
            $template_traning_stage
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

        $strRespond['final_stages'] = '';
        $stage = '';
        $stage_num = 0;
        $stages = '';
        //$res=$mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, s.limit, s.regs, s.exam, s.place, s.descr, s.classes as classes, s.form as form from olimp_stages as s where s.year='.$_CURR_YEAR.' AND (s.stage=2) AND (FIND_IN_SET("'.$USER_INFO['class'].'",s.classes)>0) order by s.exam,s.place');
        $res = $mysqli->query('select s.id as id, s.date_exam, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt,s.date_breg, DATE_FORMAT(s.date_breg,"%d.%m.%Y") as reg_bdt, s.date_ereg, DATE_FORMAT(s.date_ereg,"%d.%m.%Y %H:%i") as reg_edt, s.limit, s.regs, s.exam, s.place, s.descr, s.classes as classes, p.class_m, p.class_f, p.class_i, p.class_k, s.form as form from olimp_stages as s left join olimp_persons as p on (p.id = "' . $USER_INFO['pid'] . '") where s.year=' . $_CURR_YEAR . ' AND (s.stage=2) AND (
                        FIND_IN_SET(
                            CASE 
                                WHEN (class_m >0 and s.exam=1) 
                                    THEN class_m 
                            WHEN (class_f >0 and s.exam=2) 
                                    THEN class_f 
                            WHEN (class_i >0 and s.exam=3) 
                                    THEN class_i 
                            WHEN (class_k >0 and s.exam=4) 
                                    THEN class_k 
                                ELSE 
                                    "' . $USER_INFO['class'] . '" 
                            END ,s.classes)>0) order by s.exam,s.place');
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
        $buttons_mobile = '';
        $cur_reg_mobile = '';
        $info_mobile = '';
        if (array_key_exists($row['id'], $olimp_actions) && is_array($olimp_actions[$row['id']])) {
            if ($olimp_actions[$row['id']]['presence'] != 'D') {
                switch ($row['form']) {
                    case 2:
                        $varpwd = '';
                        $delay = '';
                        $info = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['id'],$varpwd,$delay), $tpl['cabinet']['info_2']);

                        $tpl_info_2 = '<table id="stage_info%stage_id%"><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>'; // $tpl['cabinet']['info_2'];
                        $info_mobile = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['id'],$varpwd,$delay), $tpl_info_2);
                        break;
                    case 6:
                        check_vcl_param($row['id'], $vclink, $varpwd, $delay);
                        $info = str_replace(array('%stage_id%','%vclink%','%varpwd%','%delay%'), array($row['id'],$vclink,$varpwd,$delay), $tpl['cabinet']['info_6']);

                        $tpl_info_6 = '<table id="stage_info%stage_id%"><tr><td>Ссылка для подключения к видеоконференции:</td><td id="vclink%stage_id%">%vclink%</td></tr><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>'; // $tpl['cabinet']['info_6']
                        $info_mobile = str_replace(array('%stage_id%','%vclink%','%varpwd%','%delay%'), array($row['id'],$vclink,$varpwd,$delay), $tpl_info_6);
                        break;
                    default:
                        $info = '';
                        $info_mobile = '';
                }
                $cur_reg = str_replace(array('%group%','%info%'), array($olimp_actions[$row['id']]['group_name'],$info), $tpl['cabinet_stages']['stage_current_reg']);
                $tpl_stage_current_reg = '
                    <div class="ui-widget">
                    <div class="ui-state-highlight ui-corner-all" style="padding: 0 .7em;"> 
                    <p><span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>
                    <strong>Внимание!</strong> Вы зарегистрированы для участия в этом этапе. Номер группы: %group%</p><!--%info%-->
                    </div></div>
                '; // $tpl['cabinet_stages']['stage_current_reg']
                $cur_reg_mobile = str_replace(array('%group%','%info%'), array($olimp_actions[$row['id']]['group_name'],$info), $tpl_stage_current_reg);
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

                $tpl_action_text2u = '
                    <p>Вы намерены отказаться от участия в заключительном этапе олимпиады %dt% по предмету %exam% на площадке %place%. В этом случае Вы исключаетесь из списка участников в этом потоке и сможете зарегистрироваться на участие в заключительном этапе в другом потоке при наличии.<p> Пожалуйста, подтвердите ваше намерение.
                '; // $tpl['cabinet_stages']['action_text2u']
                $tpl_button_confirm = '
                    <form action="#" method="post" id="formStageAction%stage_id%">

                    <input type="hidden" name="active_stage" value="%stage_num%">
                    <input type="hidden" name="stage_id" value="%stage_id%">
                    <input type="hidden" name="cmd" value="%cmd%">
                    <div style="text-align:center;">
                    <input type="button" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>
                    <input type="button" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>
                    <div id="dialogStageAction%stage_id%" title="%dialog_title%" class = "msg_warning" style = "display: none">
                        <p>%dialog_text%</p>    
                        <input type="button" id="cancelStageAction%stage_id%" value="%button_text1%"></input>
                        <input type="button" id="closeBtn%stage_id%" value="%button_text2%"></input>
                    </div>

                    </div>
                    </form>
                    <script>
                        $("#btnStageAction%stage_id%").click(function(){
                            $("#dialogStageAction%stage_id%").slideDown(1000);
                        });
                        $("#closeBtn%stage_id%").click(function(){
                            $("#dialogStageAction%stage_id%").slideUp(1000);
                        });
                    </script>
                '; // $tpl['cabinet_stages']['button_confirm']
                $buttons_mobile = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%','%button_text1%','%button_text2%',
                    '%dialog_title%',
                    '%dialog_text%'
                    ),
                    array($stage_num,$row['id'],'unreg2','Отказаться от участия в этом этапе','Да, отказаться от участия','Нет, не отказываться',
                    'Требуется подтверждение отказа',
                    str_replace(
                        array('%dt%','%exam%','%place%'),
                        array($row['exam_dt'],strtolower($olimp_exams[$row['exam']]['name']),$olimp_places[$row['place']]['name']),
                        $tpl_action_text2u
                    )
                    ),
                    $tpl_button_confirm
                );
                $i = $strRespond['Listeners']['count'];
                $strRespond['Listeners'][$i]['elem_id'] = str_replace("%stage_id%", $row['id'], "cancelStageAction%stage_id%");
                $strRespond['Listeners'][$i]['type'] = 'cancel_registration';
                $strRespond['Listeners'][$i]['stage_id'] = $row['id'];
                $strRespond['Listeners']['count']++;
            }
        }

        if ((!$all_regs[2][$row['exam']]) && ($reg_state == 2)) {
            if ($check_result['All is entered'] && ($check_result['printed_z'] == 'A')  && ($check_result['IdentDoc'] == 'Accepted')) { //$check_result['Idents_loaded'])
                $buttons = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg2','Зарегистрироваться на участие в этом этапе'),
                    $tpl['cabinet_stages']['button']
                );

                $tpl_button = '
                    <form action="#" method="post" id="formStageAction%stage_id%">

                    <input type="hidden" name="active_stage" value="%stage_num%">
                    <input type="hidden" name="stage_id" value="%stage_id%">
                    <input type="hidden" name="cmd" value="%cmd%">
                    <div style="text-align:center;">
                    <input type="submit" name="btnStageAction" value="%title%" id="btnStageAction%stage_id%" class=\'ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all\'>

                    </div>
                    </form>
                '; // $tpl['cabinet_stages']['button']
                $buttons_mobile = str_replace(
                    array('%stage_num%','%stage_id%','%cmd%','%title%'),
                    array($stage_num,$row['id'],'reg2','Зарегистрироваться на участие в этом этапе'),
                    $tpl_button
                );
                $i = $strRespond['Listeners']['count'];
                $strRespond['Listeners'][$i]['elem_id'] = str_replace("%stage_id%", $row['id'], "btnStageAction%stage_id%");
                $strRespond['Listeners'][$i]['type'] = 'registration';
                $strRespond['Listeners'][$i]['stage_id'] = $row['id'];
                $strRespond['Listeners']['count']++;
            } else {
                $buttons = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages'][$check_result['All is entered'] ? ($check_result['Idents_loaded'] ? 'error_application' : 'error_ident') : 'error_personal_data']), $tpl['common']['error']),
                    $tpl['cabinet']['error']
                );

                $tpl_error = '%error%<br>'; // $tpl['cabinet']['error']
                $buttons_mobile = preg_replace(
                    '/%error%/',
                    preg_replace(array('/%title%/','/%text%/'), array('Внимание!',$tpl['cabinet_stages'][$check_result['All is entered'] ? ($check_result['Idents_loaded'] ? 'error_application' : 'error_ident') : 'error_personal_data']), $tpl['common']['error']),
                    $tpl_error
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
            array($row['exam_dt'],$olimp_exams[$row['exam']]['name'],
            $olimp_stage_types[2]['name'],
            $olimp_forms[1]['name'],
            $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
            preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],''), array_key_exists('stage_date_exam' . $row['form'], $tpl['cabinet_stages']) ? $tpl['cabinet_stages']['stage_date_exam' . $row['form']] : ''),
            preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]),
            $cur_reg,$buttons
            ),
            $tpl['cabinet_stages']['stage']
        );


        if (is_array($stage_pass[2]) && $stage_pass[2][$row['exam']]) {
            $stages .= $stage;
            $stage_num++;

            $template_final_stage = '
                <div class = "msg_simple" style="color: black">
                <h3><div>%title_dt% %title_exam% %title_stage% (%title_form% форма)  %place% </div><div>%title_reg%</div></h3>
                <div>
                <span>Для учащихся %classes% классов</span><br>
                %dates_reg%<br>
                Этап проводится на площадке %place% по адресу %place_address% <br>
                %dates_exam%<br>
                %descr%<br>%current_reg%
                %buttons%<br>
                </div>
                </div>
            ';

            $strRespond['final_stages'] .= preg_replace(
                array('/%title_dt%/','/%title_exam%/', '/%title_stage%/','/%title_form%/',
                '/%place%/', '/%place_address%/','/%title_reg%/', '/%descr%/', '/%classes%/',
                '/%dates_exam%/',
                '/%dates_reg%/',
                '/%current_reg%/','/%buttons%/'
                ),
                array($row['exam_dt'],$olimp_exams[$row['exam']]['name'],
                $olimp_stage_types[2]['name'],
                $olimp_forms[1]['name'],
                $olimp_places[$row['place']]['name'],$olimp_places[$row['place']]['address'],$reg_state_txt[$reg_state], $row['descr'], $row['classes'],
                preg_replace(array('/%dt%/','/%dt_post%/'), array($row['exam_dt'],''), array_key_exists('stage_date_exam' . $row['form'], $tpl['cabinet_stages']) ? $tpl['cabinet_stages']['stage_date_exam' . $row['form']] : ''),
                preg_replace(array('/%bdt%/','/%edt%/'), array($row['reg_bdt'],$row['reg_edt']), $tpl['cabinet_stages']['stage_date_reg' . $reg_state]),
                $cur_reg_mobile, $buttons_mobile
                ),
                $template_final_stage
            );
        }
    }
        $content3 = preg_replace(
            array('/%num%/','/%selected_stage%/','/%stages%/'),
            array(3, (int)$selected_stage, $stages),
            $tpl['cabinet_stages']['stages']
        );




//  Отборочные этапы зачёт партнёрских и призёрство прошлого года
    $modules = '';
    $num = 1;
    #R - подал заявку на перезачёт Y - заявка одобрена N - заявка отклонена D - заявка отозвана участником
    foreach ($olimp_exams as $k => $v) {
        $reg_state = 2; //ЗАГЛУШКА КОСТЫЛЬ Дата окончания регистрации должна определяться через админку
        $message = "";
        if (isset($olimp_reculcs[$k])) {
            if ($olimp_reculcs[$k]['status'] == 'R') {
                $message = $tpl['cabinet_stages']['pass_selection_message1'];
            } elseif ($olimp_reculcs[$k]['status'] == 'Y') {
                $message = $tpl['cabinet_stages']['pass_selection_message2'];
            } elseif ($olimp_reculcs[$k]['status'] == 'N') {
                $refuse_message = $olimp_reculcs[$k]['refuse_message'] != null ? preg_replace(
                    array('/%refuse_message%/'),
                    array($olimp_reculcs[$k]['refuse_message']),
                    $tpl['cabinet_stages']['refuse_message_block']
                ) : '';
                $message = preg_replace(
                    array('/%refuse_message_block%/'),
                    array($refuse_message),
                    $tpl['cabinet_stages']['pass_selection_message3']
                );
            }
        }
        if (isset($olimp_reculcs[$k]) &&
            ($olimp_reculcs[$k]['status'] == 'R' || $olimp_reculcs[$k]['status'] == 'Y')
        ) {
            $module_content = preg_replace(
                array('/%num%/'),
                array($k),
                $tpl['cabinet_stages']['pass_selection_module_recall']
            );
        } elseif (!isset($olimp_reculcs[$k]) ||
            $olimp_reculcs[$k]['status'] == 'N' ||
            $olimp_reculcs[$k]['status'] == 'D'
        ) {
            if (((!$all_regs[1][$k]))
                &&
                (!$all_regs_tk[1][$k])
                &&
                ($reg_state == 2)
            ) {
                if ($check_result['All is entered'] &&
                    ($check_result['printed_z'] == 'L' || $check_result['printed_z'] == 'A') &&
                    (($check_result['IdentDoc'] == 'Accepted') || ($check_result['IdentDoc'] == 'Loaded') || ($check_result['IdentDoc'] == 'Updated'))
                ) {
                    $questions_list1 = '';
                    $questionB = '';
                    $question = $olimp_partners[$k][0]['name'];
                    foreach ($olimp_partners[$k] as $w) {
                        $questions_list1 .= '<option ' . ($w['name'] == $question ? ' selected' : '') . ' value="' . $w['id'] . '">' . $w['name'] . '</option>';
                    }
                    $module_content = preg_replace(
                        array('/%MAX_FILE_SIZE%/', '/%questions_list1%/', '/%questionB%/', '/%num%/', '/%stage_num%/'),
                        array($max_file_size, $questions_list1, $questionB, $num, $k),
                        $tpl['cabinet_stages']['pass_selection_module_form']
                    );
                } else {
                    $module_content = preg_replace(
                        '/%error%/',
                        preg_replace(
                            array('/%title%/', '/%text%/'),
                            array('Внимание!', $tpl['cabinet_stages'][$check_result['All is entered'] ? 'error_application' : 'error_personal_data']),
                            $tpl['common']['error']
                        ),
                        $tpl['cabinet']['error']
                    );
                }
            } else {
                $module_content = preg_replace(
                    array('/%exam%/'),
                    array($v['name']),
                    $tpl['cabinet_stages']['pass_selection_module_locked']
                );
            }
        }
        $modules .= preg_replace(
            array('/%module_content%/', '/%exam%/', '/%message%/'),
            array($module_content, $v['name'], $message),
            $tpl['cabinet_stages']['pass_selection_module']
        );
        $num++;
    }

    $content5 = preg_replace(
        array('/%modules%/', '/%double-colon%/'),
        array($modules, "::"),
        $tpl['cabinet_stages']['pass_selection']
    );



// Формирование страницы


    $message = '';
    $msgs = build_message_list($USER_INFO['pid'], 1, true);
    foreach ($msgs as $msg) {
        $message .= str_replace(array('%id%','%dt%','%from%','%title%','%body%'), $msg, $tpl['cabinet']['message_line']);
    }
    if (!empty($message)) {
        $message = str_replace('%message%', $message, $tpl['cabinet']['message']);
    }

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
        array('/%error%/','/%warning%/','/%message%/','/%selected_tab%/','/%content1%/','/%content2%/','/%content3%/','/%content4%/','/%content5%/'),
        array($error,$warning,$message,($selected_tab - 1),$content1,$content2,$content3,$content4,$content5),
        $tpl['cabinet_stages']['main']
    );

    //return $body;
    return $strRespond;
}
