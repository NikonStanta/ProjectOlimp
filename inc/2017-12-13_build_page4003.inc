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

// Перерегистрация участников прошлого года
    $message = '';
    include_once('../inc/reregistrate.inc');

    $check_result = check_persons_data($USER_INFO['pid']);

    $content1 = '';
// Варианты заочного этапа
    $res = $mysqli->query('select a.stage_id,e.name as exam_name,e.abbr as exam_abbr from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_exams as e on (e.id=s.exam) where (s.date_post>Now()) AND (a.varnum>0) AND (a.presence<>"D") AND (a.presence<>"Y") AND (a.presence<>"N") AND (a.person_id=' . $USER_INFO['pid'] . ') order by s.date_exam');
    while ($row = $res->fetch_assoc()) {
        $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . '.' . $var_file_ext;
        $content1 .= str_replace(
            array('%stage_id%','%exam_name%','%action%'),
            array($row['stage_id'],$row['exam_name'],$filename),
            $tpl['cabinet_dist_stage']['get_var_ref']
        );
    }


// Бланки

    $content1 .= $tpl['cabinet_dist_stage']['get_blanks_ref'];

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
        array('/%error%/','/%warning%/','/%message%/','/%doc_list%/'),
        array($error,$warning,$message,$content1),
        $tpl['cabinet_dist_stage']['main']
    );

    return $body;
}
