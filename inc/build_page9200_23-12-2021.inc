<?php

// Загрузка скана работы

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    $var_dir = '../works';

    $exam = 0;
    if ($PAGE['name'] == 'ehope_work_m') {
        $exam = 1;
        $ex = "M";
    }
    if ($PAGE['name'] == 'ehope_work_p') {
        $exam = 2;
        $ex = "P";
    }
    if ($PAGE['name'] == 'ehope_work_i') {
        $exam = 3;
        $ex = "I";
    }
    if ($PAGE['name'] == 'ehope_work_k') {
        $exam = 4;
        $ex = "K";
    }



    if ($exam == 0) {
        die('Запрошен неверный файл ' . $PAGE['name']);
    }


    $res = $mysqli->query('SELECT a.stage_id FROM olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) WHERE s.exam=' . $exam . ' and a.person_id=' . $USER_INFO['pid'] . ' and s.stage=1 and a.presence="Y"');
    if ($row = $res->fetch_row()) {
        $stage_id = $row[0];
    } else {
        die('Запрошен неверный файл');
    }


    $file_name = $var_dir . '/' . $ex . $USER_INFO['pid'] . '.pdf';    //echo $file_name;


    die($file_name);
    //if (!(file_exists($file_name)))  die(header('HTTP/1.1 404 Not Found'));

    //$mysqli->query('update olimp_actions set presence="L" where (stage_id='.$stage_id.') AND (person_id='.$USER_INFO['pid'].')');

    $s = filesize($file_name);

    $f1 = fopen($file_name, 'rb');

    header('Accept-Ranges: none');
    header('Cache-Control: no-cache');
    header('Pragma: no-cache');

    header("Content-Transfer-Encoding: binary");
//    header('Content-Disposition: attachment; filename="' . $PAGE['name'] . '"');

    header('Content-Type: application/pdf');
//    header('Content-Length: '.$s);

    fpassthru($f1);

    fclose($f1);
}
