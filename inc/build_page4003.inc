<?php

// Личный кабинет. Заочный этап.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');
include_once('../inc/cabinet2.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    $var_dir = '/documents';
    $var_file_prefix = 'ehope_variant';
    $var_file_ext = 'rar';

    if ($USER_INFO['uid'] < 1) {
        return $tpl['cabinet']['logon_form'];
    }


    if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) {
        return $tpl['cabinet']['closed'];
    }

// Перерегистрация участников прошлого года
    $message = '';
    include_once('../inc/reregistrate.inc');

    $check_result = check_persons_data($USER_INFO['pid']);
    $error = '';
    $warning = '';

    $content1 = '';
// Варианты заочного этапа
    $res = $mysqli->query('select a.stage_id,e.name as exam_name,e.abbr as exam_abbr,s.form, s.stage as stage, a.audit as audit1, g.audit as audit2, a.presence, s.var_pwd as varpwd, s.date_exam from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_exams as e on (e.id=s.exam) left join olimp_groups as g ON (g.id=a.group_id) where (s.date_post>Now()) AND (a.varnum>0) AND (a.presence<>"D") AND (a.presence<>"Y") AND (a.presence<>"N") AND (a.person_id=' . $USER_INFO['pid'] . ') order by s.date_exam');
    while ($row = $res->fetch_assoc()) {
        $stage = '***' . $row['stage'] . '***';
        switch ($row['stage']) {
            case 1:
                $stage = 'Отборочный этап';
                break;
            case 2:
                $stage = 'Заключительный этап';
                break;
            case 3:
                $stage = 'Тренировочный этап';
                break;
        }

        switch ($row['form']) {
            case 2:
                $stage .= ' в заочной форме';
                $varpwd = '';
                $delay = '';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . '.' . $var_file_ext;
                check_pwd_param($row['stage_id'], $varpwd, $delay);
                $info = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl['cabinet']['info_2']);
                break;
            case 4:
                $stage .= ', творческий конкурс в заочной форме';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . 'tk.' . $var_file_ext;
                check_pwd_param($row['stage_id'], $varpwd, $delay);
                $info = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl['cabinet']['info_4']);
                break;
            case 6:
                $stage .= ' в очной форме с применением дистанционных технологий';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . ($row['stage'] == 2 ? 'l' : '') . '.' . $var_file_ext;
                check_vcl_param($row['stage_id'], $vclink, $varpwd, $delay);
                $info = str_replace(array('%stage_id%','%vclink%','%varpwd%','%delay%'), array($row['stage_id'],$vclink,$varpwd,$delay), $tpl['cabinet']['info_6']);
                break;
            default:
                $stage .= ' в заочной форме';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . '.' . $var_file_ext;
        }

        $content1 .= str_replace(
            array('%stage_id%','%exam_name%','%action%','%stage%','%info%'),
            array($row['stage_id'],$row['exam_name'],$filename,$stage,$info),
            $tpl['cabinet_dist_stage']['get_var_ref']
        );
    }


// Бланки

    $content1 .= $tpl['cabinet_dist_stage']['get_blanks_ref'];

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
        array('/%error%/','/%warning%/','/%message%/','/%doc_list%/'),
        array($error,$warning,$message,$content1),
        $tpl['cabinet_dist_stage']['main']
    );

    return $body;
}
