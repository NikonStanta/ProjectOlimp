<?php

// Редактирование содержания сайта - Актуально
include_once 'ckeditor/ckeditor.php';

function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$mysqli;

    if (($role & 1) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];
    $aid = $_REQUEST['aid'];

    if (isset($_REQUEST['btnAdd'])) {
        if (empty($aid)) {
            $cmd = 'doAdd';
        } else {
            $cmd = 'doEdit';
        }
    }

    if (isset($_REQUEST['btnDel'])) {
        if (empty($aid)) {
            $cmd = '';
        } else {
            $cmd = 'doDel';
        }
    }

    if (isset($_REQUEST['btnCancel'])) {
            $cmd = '';
        $aid = '';
    }

// Выполнение команд

    switch ($cmd) {
        case 'doEdit':
        {    if (!empty($_REQUEST['ttl'])) {
                $ttl = stripslashes($_REQUEST['ttl']);
        } else {
            $ttl = '<Новое объявление>';
        }
        if (!empty($_REQUEST['txt1'])) {
            $txt1 = stripslashes($_REQUEST['txt1']);
        } else {
            $txt1 = '';
        }
        if (!empty($_REQUEST['txt2'])) {
            $txt2 = stripslashes($_REQUEST['txt2']);
        } else {
            $txt2 = '';
        }
        if (!empty($_REQUEST['dt'])) {
            $dt = $_REQUEST['dt'];
        } else {
            $dt = '';
        }
        if (!empty($_REQUEST['ord'])) {
            $ord = $_REQUEST['ord'];
        } else {
            $res = $mysqli->query("select max(ord) from actual");
            $row = $res->fetch_row();
            $ord = $row[0] + 10;
        }
            $actual_divs = $_REQUEST['actual_divs'];

            $ttl = addslashes(stripslashes($ttl));
            $txt1 = addslashes(stripslashes($txt1));
            $txt2 = addslashes(stripslashes($txt2));

        if ($mysqli->query('update actual set ttl="' . $ttl . '", txt1="' . $txt1 . '", txt2="' . $txt2 . '", ord=' . $ord . ' , dt=' . ((!empty($dt) && $dt != '0000-00-00') ? '"' . $dt . '"' : 'Now()') . ' where id=' . $aid)) {
            $cmd = '';
            $mysqli->query('delete from actual_div where aid=' . $aid);
            if (is_array($actual_divs)) {
                foreach ($actual_divs as $k => $v) {
                        $mysqli->query('insert into actual_div(did,aid) values(' . $k . ',' . $aid . ')');
                }
            }
        } else {
            $cmd = 'Edit';
        }
        break;

        }
        case 'doAdd':
        {    if (!empty($_REQUEST['ttl'])) {
                $ttl = stripslashes($_REQUEST['ttl']);
        } else {
            $ttl = '<Новое объявление>';
        }
        if (!empty($_REQUEST['txt1'])) {
            $txt1 = stripslashes($_REQUEST['txt1']);
        } else {
            $txt1 = '';
        }
        if (!empty($_REQUEST['txt2'])) {
            $txt2 = stripslashes($_REQUEST['txt2']);
        } else {
            $txt2 = '';
        }
        if (!empty($_REQUEST['dt'])) {
            $dt = $_REQUEST['dt'];
        } else {
            $dt = '';
        }
        if (!empty($_REQUEST['ord'])) {
            $ord = $_REQUEST['ord'];
        } else {
            $res = $mysqli->query("select max(ord) from actual");
            $row = $res->fetch_row();
            $ord = $row[0] + 10;
        }
            $actual_divs = $_REQUEST['actual_divs'];
            $ttl = addslashes(stripslashes($ttl));
            $txt1 = addslashes(stripslashes($txt1));
            $txt2 = addslashes(stripslashes($txt2));

        if ($mysqli->query('insert into actual set ttl="' . $ttl . '", txt1="' . $txt1 . '", txt2="' . $txt2 . '", ord=' . $ord . ', dt=' . ((!empty($dt) && $dt != '0000-00-00') ? '"' . $dt . '"' : 'Now()'))) {
            $cmd = '';
            $aid = $mysqli->insert_id;
            if (is_array($actual_divs)) {
                foreach ($actual_divs as $k => $v) {
                    $mysqli->query('insert into actual_div(did,aid) values(' . $k . ',' . $aid . ')');
                }
            }
        } else {
            $cmd = 'Add';
        }
        break;
        }
        case 'doDel':
        {    $mysqli->query('delete from actual where id=' . $aid);
            $mysqli->query('delete from actual_div where aid=' . $aid);
            $cmd = '';
            break;
        }
        case 'Up':
        {    $res = $mysqli->query('select ord from actual where id=' . $aid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from actual where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update actual set ord=' . $upord . ' where id=' . $aid);
                    $mysqli->query('update actual set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $res = $mysqli->query('select ttl,txt1,txt2,ord,dt from actual where id=' . $aid);
            if ($row = $res->fetch_row()) {
                $old_ttl = $row[0];
                $old_txt1 = $row[1];
                $old_txt2 = $row[2];
                $old_ord = $row[3];
                $old_dt = $row[4];

                $res = $mysqli->query('select did from actual_div where aid=' . $aid);
                while ($row = $res->fetch_row()) {
                    $old_actual_divs[$row[0]] = true;
                }
            }
        }
        case 'Add':
        {    if (!empty($_REQUEST['ttl'])) {
                $ttl = stripslashes($_REQUEST['ttl']);
        } else {
            $ttl = $old_ttl;
        }
        if (!empty($_REQUEST['txt1'])) {
            $txt1 = stripslashes($_REQUEST['txt1']);
        } else {
            $txt1 = $old_txt1;
        }
        if (!empty($_REQUEST['txt2'])) {
            $txt2 = stripslashes($_REQUEST['txt2']);
        } else {
            $txt2 = $old_txt2;
        }
        if (!empty($_REQUEST['ord'])) {
            $ord = $_REQUEST['ord'];
        } else {
            $ord = $old_ord;
        }
        if (!empty($_REQUEST['dt'])) {
            $dt = $_REQUEST['dt'];
        } else {
            $dt = $old_dt;
        }
            $actual_divs = $_REQUEST['actual_divs'];
        if (!is_array($actual_divs)) {
            $actual_divs = $old_actual_divs;
        }

            $divs = '';
            $res = $mysqli->query('select id,name from site_divs');
        while ($row = $res->fetch_row()) {
            $divs .= '<input type=checkbox name="actual_divs[' . $row[0] . ']" value=1' . (!empty($actual_divs[$row[0]]) ? ' checked' : '') . ' id="ad' . $row[0] . '"><label for="ad' . $row[0] . '">' . $row[1] . '</label><br>';
        }
            $ttl = htmlspecialchars(stripslashes($ttl));
            $txt1 = stripslashes(preg_replace("/\n|\r/", '', $txt1));
            $txt2 = stripslashes(preg_replace("/\n|\r/", '', $txt2));

            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $txt1 = $CKE->editor('txt1', $txt1);
            $txt1 = $sw->textareaAttributes; # Was $txt1=$sw->textareaAttributes; # Was $sw->getHtml();
            $txt1 = preg_replace('/[\\\][\\\]n/', '', $txt1);

            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $txt1 = $CKE->editor('txt2', $txt2);
            $txt2 = $sw->textareaAttributes; # Was $txt2=$sw->textareaAttributes; # Was $sw->getHtml();
            $txt2 = preg_replace('/[\\\][\\\]n/', '', $txt2);

            $body = preg_replace(
                array('/%aid%/','/%ttl%/','/%txt1%/','/%txt2%/','/%ord%/','/%dt%/','/%actual_divs%/'),
                array($aid,$ttl,$txt1,$txt2,$ord,$dt,$divs),
                $tpl['actual']['edit']
            );

        break;
        }
        case 'Del':
        {    $res = $mysqli->query('select ttl,txt1 from actual where id=' . $aid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%aid%/','/%ttl%/','/%txt1%/'),
                    array($aid,$row[0],$row[1]),
                    $tpl['actual']['del']
                );
                break;
            } else {
                $cmd = '';
            }
        }
        default:
        {    // Список
            $list = '';
            $even = false;
            $upid = 0;
            $res = $mysqli->query('select id,ttl from actual order by ord');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%ttl%/','/%upid%/','/%bgcolor%/'),
                    array($row[0],$row[1],$upid,$tpl[$even ? 'even' : 'odd']),
                    ($upid == 0 ? $tpl['actual']['line'] : $tpl['actual']['lineup'])
                );
                $upid = $row[0];
                $even = !$even;
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['actual']['main']);
        }
    }
    return $body;
}

include_once 'ckeditor/ckeditor.php'; //mark3

$body = build_page();
