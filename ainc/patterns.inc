<?php

// Редактирование структуры сайта - Макеты страниц
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$mysqli;

    if (($role & 2) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];
    $pid = $_REQUEST['pid'] + 0;

    if (isset($_REQUEST['btnAdd'])) {
        if (empty($pid)) {
            $cmd = 'doAdd';
        } else {
            $cmd = 'doEdit';
        }
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
        {    if (!empty($pid)) {
                if (!empty($_REQUEST['name'])) {
                            $name = stripslashes($_REQUEST['name']);
                } else {
                        $name = '';
                }
                if (!empty($_REQUEST['maket'])) {
                    $maket = stripslashes($_REQUEST['maket']);
                } else {
                    $maket = '';
                }
                if (!empty($_REQUEST['params'])) {
                    $params = stripslashes($_REQUEST['params']);
                } else {
                    $params = '';
                }

                if ($mysqli->query('update pages_text set name="' . addslashes($name) . '", maket="' . addslashes($maket) . '", params="' . addslashes($params) . '" where id=' . $pid)) {
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
        if (!empty($_REQUEST['maket'])) {
            $maket = stripslashes($_REQUEST['maket']);
        } else {
            $maket = '';
        }
        if (!empty($_REQUEST['params'])) {
            $params = stripslashes($_REQUEST['params']);
        } else {
            $params = '';
        }

        if ($mysqli->query('insert into pages_text set name="' . addslashes($name) . '", maket="' . addslashes($maket) . '", params="' . addslashes($params) . '"')) {
            $cmd = '';
        } else {
            $cmd = 'Add';
        }
            break;

        }
        case 'doDel':
        {    if (!empty($pid)) {
                $mysqli->query('delete from pages_text where id=' . $pid);
        }
            $cmd = '';
        break;

        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $res = $mysqli->query('select name,maket,params from pages_text where id=' . $pid);
            if ($row = $res->fetch_row()) {
                $old_name = $row[0];
                $old_maket = $row[1];
                $old_params = $row[2];
            }

        }
        case 'Add':
        {    if (!empty($_REQUEST['name'])) {
                $name = $_REQUEST['name'];
        } else {
            $name = $old_name;
        }
        if (!empty($_REQUEST['maket'])) {
            $maket = $_REQUEST['maket'];
        } else {
            $maket = $old_maket;
        }
        if (!empty($_REQUEST['params'])) {
            $params = $_REQUEST['params'];
        } else {
            $params = $old_params;
        }

            $maket = htmlspecialchars(stripslashes($maket));
        $body = preg_replace(array('/%id%/','/%name%/','/%maket%/','/%params%/'), array($pid,$name,$maket,$params), $tpl['patterns']['edit']);
        break;

        }
        case 'Del':
        {    $res = $mysqli->query('select name from pages_text where id=' . $pid);
            if ($row = $res->fetch_row()) {
                $name = $row[0];
                $res = $mysqli->query('select count(id) from site_pages where (page_tp=2) AND (page_subtp=' . $pid . ')');
                $row = $res->fetch_row();
                $c1 = $row[0];
                $res = $mysqli->query('select count(id) from catalog_items where subtype=' . $pid);
                $row = $res->fetch_row();
                $c2 = $row[0];
                if (($c1 + $c2) == 0) {
                    $body = preg_replace(array('/%id%/','/%name%/'), array($pid,$name), $tpl['patterns']['del']);
                } else {
                            $body = preg_replace(array('/%id%/','/%name%/','/%c1%/','/%c2%/'), array($pid,$name,$c1,$c2), $tpl['patterns']['nodel']);
                }
            }
            $cmd = '';
                break;

        }
        default:
        {    // Список
            $list = '';
            $even = false;
            $res = $mysqli->query('select id,name from pages_text order by name');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%name%/','/%bgcolor%/'),
                    array($row[0],$row[1],$tpl[$even ? 'even' : 'odd']),
                    $tpl['patterns']['line']
                );
                $even = !$even;
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['patterns']['main']);

        }
    }
    return $body;
}

$body = build_page();
