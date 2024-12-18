<?php

// Редактирование настроек сайта - Шаблоны
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$mysqli;

    if (($role & 2) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];
    $tid = $_REQUEST['tid'] + 0;

    if (isset($_REQUEST['btnAdd'])) {
        if (empty($tid)) {
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
        {    if (!empty($tid)) {
                if (!empty($_REQUEST['name'])) {
                    $name = ($_REQUEST['name']);
                } else {
                    $name = '';
                }
                if (!empty($_REQUEST['title'])) {
                    $title = ($_REQUEST['title']);
                } else {
                    $title = '';
                }
                if (!empty($_REQUEST['txt'])) {
                    $txt = ($_REQUEST['txt']);
                } else {
                    $txt = '';
                }
                if (!empty($_REQUEST['tp'])) {
                    $tp = $_REQUEST['tp'] + 0;
                } else {
                    $tp = 0;
                }
                if (($tp < 0) || ($tp > 1)) {
                    $tp = 0;
                }
                $txt = preg_replace(array('/@@refpar@@/','/@@formrefpar@@/'), array('%refpar%','%formrefpar%'), $txt);
                if ($mysqli->query('update site_templates set tp=' . $tp . ', name="' . $mysqli->real_escape_string($name) . '", title="' . $mysqli->real_escape_string($title) . '", txt="' . $mysqli->real_escape_string($txt) . '" where id=' . $tid)) {
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
                $name = ($_REQUEST['name']);
        } else {
            $name = '';
        }
        if (!empty($_REQUEST['title'])) {
            $title = ($_REQUEST['title']);
        } else {
            $title = '';
        }
        if (!empty($_REQUEST['txt'])) {
            $txt = ($_REQUEST['txt']);
        } else {
            $txt = '';
        }
        if (!empty($_REQUEST['tp'])) {
            $tp = $_REQUEST['tp'] + 0;
        } else {
            $tp = 0;
        }
        if (($tp < 0) || ($tp > 1)) {
            $tp = 0;
        }
            $txt = preg_replace(array('/@@refpar@@/','/@@formrefpar@@/'), array('%refpar%','%formrefpar%'), $txt);
        if ($mysqli->query('insert into site_templates set tp=' . $tp . ', name="' . $mysqli->real_escape_string($name) . '", title="' . $mysqli->real_escape_string($title) . '", txt="' . $mysqli->real_escape_string($txt) . '"')) {
            $cmd = '';
        } else {
            $cmd = 'Add';
        }
        break;
        }
        case 'doLink':
        {    if (!empty($tid)) {
                $div_tpl = $_REQUEST['divlink'];
                $page_tpl = $_REQUEST['pagelink'];
                $mysqli->query('delete from site_div_tpl where tplid=' . $tid);
                $mysqli->query('delete from site_page_tpl where tplid=' . $tid);
                if (is_array($div_tpl)) {
                    foreach ($div_tpl as $k => $v) {
                        $mysqli->query('insert into site_div_tpl set tplid=' . $tid . ', did=' . $k);
                    }
                }
                if (is_array($page_tpl)) {
                    foreach ($page_tpl as $k => $v) {
                        $mysqli->query('insert into site_page_tpl set tplid=' . $tid . ', pgid=' . $k);
                    }
                }
        }
            $cmd = '';
            break;
        }
        case 'doDel':
        {    if (!empty($tid)) {
                $mysqli->query('delete from site_templates where id=' . $tid);
                $mysqli->query('delete from site_div_tpl where tplid=' . $tid);
                $mysqli->query('delete from site_page_tpl where tplid=' . $tid);
        }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $res = $mysqli->query('select tp,name,title,txt from site_templates where id=' . $tid);
            if ($row = $res->fetch_row()) {
                $old_tp = $row[0];
                $old_name = $row[1];
                $old_title = $row[2];
                $old_txt = $row[3];
            }
        }
        case 'Add':
        {    if (!empty($_REQUEST['name'])) {
                $name = ($_REQUEST['name']);
        } else {
            $name = $old_name;
        }
        if (!empty($_REQUEST['title'])) {
            $title = ($_REQUEST['title']);
        } else {
            $title = $old_title;
        }
        if (!empty($_REQUEST['txt'])) {
            $txt = ($_REQUEST['txt']);
        } else {
            $txt = $old_txt;
        }
            $txt = preg_replace(array('/%refpar%/','/%formrefpar%/'), array('@@refpar@@','@@formrefpar@@'), $txt);
        if (!empty($_REQUEST['tp'])) {
            $tp = $_REQUEST['tp'] + 0;
        } else {
            $tp = $old_tp;
        }
        if (($tp < 0) || ($tp > 1)) {
            $tp = 0;
        }

            $txt = htmlspecialchars($txt);

            $tps = '<option value=0' . ($tp == 0 ? ' selected' : '') . '>Шаблон</option><option value=1' . ($tp == 1 ? ' selected' : '') . '>Набор шаблонов</option>';

            $body = preg_replace(
                array('/%id%/','/%name%/','/%title%/','/%txt%/','/%tps%/'),
                array($tid,$name,$title,$txt,$tps),
                $tpl['tpls']['edit']
            );
        break;
        }
        case 'Del':
        {    $res = $mysqli->query('select name,title from site_templates where id=' . $tid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%id%/','/%name%/','/%title%/'),
                    array($tid,$row[0],$row[1]),
                    $tpl['tpls']['del']
                );
            }
            $cmd = '';
            break;
        }
        case 'Links':
        {    $res = $mysqli->query('select did from site_div_tpl where tplid=' . $tid);
            while ($row = $res->fetch_row()) {
                $div_tpl[$row[0]] = 1;
            }
            $res = $mysqli->query('select pgid from site_page_tpl where tplid=' . $tid);
            while ($row = $res->fetch_row()) {
                $page_tpl[$row[0]] = 1;
            }
            $list = preg_replace(
                array('/%did%/','/%name%/','/%sel%/'),
                array(0,'Все разделы',!empty($div_tpl[0]) ? ' checked' : ''),
                $tpl['tpls']['divlink']
            );
                $divid = 0;
                $res = $mysqli->query('select d.id,d.name,p.id,p.title from site_divs as d left join site_pages as p on (d.id=p.did) order by d.ord,p.ord');
            while ($row = $res->fetch_row()) {
                if ($divid != $row[0]) {
                    $divid = $row[0];
                        $list .= preg_replace(
                            array('/%did%/','/%name%/','/%sel%/'),
                            array($row[0],$row[1],!empty($div_tpl[$row[0]]) ? ' checked' : ''),
                            $tpl['tpls']['divlink']
                        );
                }
                $list .= preg_replace(
                    array('/%pgid%/','/%name%/','/%sel%/'),
                    array($row[2],$row[3],!empty($page_tpl[$row[2]]) ? ' checked' : ''),
                    $tpl['tpls']['pagelink']
                );
            }
                $body = preg_replace(array('/%list%/','/%tid%/'), array($list,$tid), $tpl['tpls']['links']);
                break;
        }
        default:
        {    // Список
            $list = '';
            $even = false;
            $res = $mysqli->query('select id,name,title from site_templates order by name');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%name%/','/%title%/','/%bgcolor%/'),
                    array($row[0],$row[1],$row[2],$tpl[$even ? 'even' : 'odd']),
                    $tpl['tpls']['line']
                );
                $even = !$even;
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['tpls']['main']);
        }
    }
    return $body;
}

$body = build_page();
