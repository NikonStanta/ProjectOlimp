<?php

// Редактирование настроек сайта - Параметры

include_once 'ckeditor/ckeditor.php'; //mark3
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$mysqli;

    if (($role & (1 + 2)) == 0) {
        return $tpl['noaccess'];
    }

    if (array_key_exists("cmd", $_REQUEST)) {
    }
    {
        $cmd = $_REQUEST["cmd"];
    }

    if (isset($_REQUEST['btnDel'])) {
        $cmd = 'doDel';
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
    }

// Выполнение команд

    switch ($cmd) {
        case 'doEdit':
        {    if (!empty($_REQUEST['name'])) {
                $name = stripslashes($_REQUEST['name']);
        } else {
            $name = '';
        }
        if (!empty($_REQUEST['value'])) {
            $value = stripslashes($_REQUEST['value']);
        } else {
            $value = '';
        }
        if (!empty($_REQUEST['descr'])) {
            $descr = stripslashes($_REQUEST['descr']);
        } else {
            $descr = '';
        }
        if (!empty($_REQUEST['tp'])) {
            $tp = $_REQUEST['tp'] + 0;
        } else {
            $tp = 1;
        }
        if (($tp < 1) || ($tp > 2)) {
            $tp = 1;
        }
        if ($tp == 1) {
            $value = substr($value, 0, 255);
        }
        if (!empty($name)) {
            if ($mysqli->query('update ' . ($tp == 1 ? 'vars_string' : 'vars_text') . ' set value="' . addslashes($value) . '", descr="' . addslashes($descr) . '" where name="' . $name . '"')) {
                $cmd = '';
            } else {
                $cmd = 'Edit';
            }
        } else {
            $cmd = 'Edit';
        }
        break;
        }
        case 'doAdd':
        {    if (!empty($_REQUEST['name'])) {
                $name = stripslashes($_REQUEST['name']);
        } else {
            $name = '';
        }
        if (!empty($_REQUEST['value'])) {
            $value = stripslashes($_REQUEST['value']);
        } else {
            $value = '';
        }
        if (!empty($_REQUEST['descr'])) {
            $descr = stripslashes($_REQUEST['descr']);
        } else {
            $descr = '';
        }
        if (!empty($_REQUEST['tp'])) {
            $tp = $_REQUEST['tp'] + 0;
        } else {
            $tp = 1;
        }
        if (($tp < 1) || ($tp > 2)) {
            $tp = 1;
        }
        if ($tp == 1) {
            $value = substr($value, 0, 255);
        }
        if (!empty($name)) {
            if ($mysqli->query('insert into ' . ($tp == 1 ? 'vars_string' : 'vars_text') . ' set name="' . $name . '", value="' . addslashes($value) . '", descr="' . addslashes($descr) . '"')) {
                $cmd = '';
            } else {
                $cmd = 'Add';
            }
        } else {
            $cmd = 'Add';
        }
        break;
        }
        case 'doDel':
        {    if ((!empty($_REQUEST['tp'])) && (!empty($_REQUEST['name']))) {
                $tp = $_REQUEST['tp'] + 0;
                $name = $_REQUEST['name'];
                if ($tp == 1) {
                    $mysqli->query('delete from vars_string where name="' . $name . '"');
                } elseif ($tp == 2) {
                    $mysqli->query('delete from vars_text where name="' . $name . '"');
                }
        }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $name = $_REQUEST['name'];
            $tp = $_REQUEST['tp'] + 0;
            if (($tp == 1) || ($tp == 2)) {
                $res = $mysqli->query('select value,descr from ' . ($tp == 1 ? 'vars_string' : 'vars_text') . ' where name="' . $name . '"');
                if ($row = $res->fetch_row()) {
                    $old_value = $row[0];
                    $old_descr = $row[1];
                }
            }
        }
        case 'Add':
        {    if (!empty($_REQUEST['name'])) {
                $name = $_REQUEST['name'];
        } else {
            $name = '';
        }
        if (!empty($_REQUEST['value'])) {
            $value = $_REQUEST['value'];
        } else {
            $value = $old_value;
        }
        if (!empty($_REQUEST['descr'])) {
            $descr = $_REQUEST['descr'];
        } else {
            $descr = $old_descr;
        }
        if (!empty($_REQUEST['tp'])) {
            $tp = $_REQUEST['tp'] + 0;
        } else {
            $tp = 1;
        }
        if (($tp < 1) || ($tp > 2)) {
            $tp = 1;
        }
            $value = $value;
            $descr = $descr;

            $value = stripslashes(preg_replace("/\n|\r/", '', $value));

            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $value = $CKE->editor('value', $value);

            $value = preg_replace('/[\\\][\\\]n/', '', $value);

            $body = preg_replace(
                array('/%name%/','/%value%/','/%descr%/','/%tp%/'),
                array($name,$value,$descr,$tp),
                $tpl['params'][$cmd == 'Edit' ? 'edit' : 'add']
            );
        break;
        }
        case 'Del':
        {    $name = $_REQUEST['name'];
            $tp = $_REQUEST['tp'] + 0;
            if (($tp == 1) || ($tp == 2)) {
                $res = $mysqli->query('select value,descr from ' . ($tp == 1 ? 'vars_string' : 'vars_text') . ' where name="' . $name . '"');
                if ($row = $res->fetch_row()) {
                    $body = preg_replace(
                        array('/%name%/','/%descr%/','/%tp%/'),
                        array($name,$row[0],$tp),
                        $tpl['params']['del']
                    );
                    break;
                }
            } else {
                $cmd = '';
            }
        }
        default:
        {    // Список
            $list1 = '';
            $even = false;
            $res = $mysqli->query('select name,descr from vars_string order by name');
            while ($row = $res->fetch_row()) {
                $list1 .= preg_replace(
                    array('/%tp%/','/%name%/','/%nameref%/','/%descr%/','/%bgcolor%/'),
                    array(1,$row[0],urlencode($row[0]),$row[1],$tpl[$even ? 'even' : 'odd']),
                    $tpl['params']['line']
                );
                $even = !$even;
            }
            $list2 = '';
            $even = false;
            $res = $mysqli->query('select name,descr from vars_text order by name');
            while ($row = $res->fetch_row()) {
                $list2 .= preg_replace(
                    array('/%tp%/','/%name%/','/%nameref%/','/%descr%/','/%bgcolor%/'),
                    array(2,$row[0],urlencode($row[0]),$row[1],$tpl[$even ? 'even' : 'odd']),
                    $tpl['params']['line']
                );
                $even = !$even;
            }
            $body = preg_replace(array('/%list1%/','/%list2%/'), array($list1,$list2), $tpl['params']['main']);
        }
    }
    return $body;
}

$body = build_page();
