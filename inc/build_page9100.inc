<?php

// Загрузка варианта

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    $var_dir = '../vars';

    $stage_id = $_POST['stage_id'] + 0;

    if (($stage_id < 0) || ($USER_INFO['pid'] < 1)) {
        die('111' . header('HTTP/1.1 404 Not Found'));
    }

    $res = $mysqli->query('select exam,form,var_prefix,stage from olimp_stages where id=' . $stage_id);
    if ($row = $res->fetch_row()) {
        $exam = $row[0];
        $var_prefix = $row[2];
    } else {
        die('222' . header('HTTP/1.1 404 Not Found'));
    }
    if ($row[1] != 2 && $row[1] != 4 && $row[1] != 6) {
        die('333' . header('HTTP/1.1 404 Not Found'));
    }

    if ($row[1] == 4) {
        $file_postfix = 'tk';
    } else {
        $file_postfix = ($row[3] == 2) ? 'l' : '';
    }

    if (($exam == 1) && ($PAGE['name'] != 'ehope_variant_m' . $file_postfix)) {
        die('Запрошен неверный файл ' . $PAGE['name']);
    }
    if (($exam == 2) && ($PAGE['name'] != 'ehope_variant_f' . $file_postfix)) {
        die('Запрошен неверный файл ' . $PAGE['name']);
    }
    if (($exam == 3) && ($PAGE['name'] != 'ehope_variant_i' . $file_postfix)) {
        die('Запрошен неверный файл ' . $PAGE['name']);
    }
    if (($exam == 4) && ($PAGE['name'] != 'ehope_variant_k' . $file_postfix)) {
        die('Запрошен неверный файл ' . $PAGE['name']);
    }

    $res = $mysqli->query('select varnum,presence from olimp_actions where (stage_id=' . $stage_id . ') AND (person_id=' . $USER_INFO['pid'] . ')');
    if ($row = $res->fetch_row()) {
        $varnum = $row[0];
    } else {
        die(header('HTTP/1.1 404 Not Found'));
    }

    if (($row[1] != 'R') && ($row[1] != 'L') && ($row[1] != 'Y') && ($row[1] != 'W')) {
        die('444' . header('HTTP/1.1 404 Not Found'));
    }

    $file_name = $var_dir . '/' . $var_prefix . ($varnum < 10 ? '0' : '') . $varnum . '.rar';

    //die($file_name);

    if (!(file_exists($file_name))) {
        die($file_name . header('HTTP/1.1 404 Not Found'));
    }

    if ($row[1] == 'R') {
        $mysqli->query('update olimp_actions set presence="L" where (stage_id=' . $stage_id . ') AND (person_id=' . $USER_INFO['pid'] . ')');
    }

    $s = filesize($file_name);

    $f1 = fopen($file_name, 'rb');

    header('Accept-Ranges: none');
    header('Cache-Control: no-cache');
    header('Pragma: no-cache');

    header("Content-Transfer-Encoding: binary");
//    header('Content-Disposition: attachment; filename="' . $PAGE['name'] . '"');

    header('Content-Type: application/rar');
//    header('Content-Length: '.$s);

    fpassthru($f1);

    fclose($f1);
}
