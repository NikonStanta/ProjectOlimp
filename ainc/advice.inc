<?php

// Редактирование советов/публикаций

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli;

    if (($role & 1) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];

    if (!empty($_REQUEST['fid'])) {
        $fid = $_REQUEST['fid'];
    }

    if (!empty($_REQUEST['tid'])) {
        $tid = $_REQUEST['tid'];
    } else {
        if (!empty($fid)) {
            $res = $mysqli->query('select tid from advice_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                $tid = $row[0];
            }
        }
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $fid = '';
        $tid = '';
    }

    if (isset($_REQUEST['btnAddItem'])) {
        if (empty($fid)) {
            $cmd = 'doAddItem';
        } else {
            $cmd = 'doEditItem';
        }
    }

    if (isset($_REQUEST['btnDelItem'])) {
        if (empty($fid)) {
            $cmd = '';
        } else {
            $cmd = 'doDelItem';
        }
    }

    if (isset($_REQUEST['btnAddGroup'])) {
        if (empty($tid)) {
            $cmd = 'doAddGroup';
        } else {
            $cmd = 'doEditGroup';
        }
    }

    if (isset($_REQUEST['btnDelGroup'])) {
        if (empty($tid)) {
            $cmd = '';
        } else {
            $cmd = 'doDelGroup';
        }
    }
// Выполнение команд

    switch ($cmd) {
        case 'doDelItem':
        {    if (!empty($fid)) {
                $mysqli->query('delete from advice_data where id=' . $fid);

                $cmd = '';
        }
            break;
        }
        case 'doDelGroup':
        {    if (!empty($tid)) {
                $mysqli->query('delete from advice_topics where id=' . $tid);

                $cmd = '';
        }
            break;
        }

        case 'doAddGroup':
        {    $name = $_REQUEST['name'];
            if (empty($name)) {
                $name = '<Новая тема>';
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from advice_topics");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('insert into advice_topics set ord=' . $ord . ', name="' . $name . '"')) {
                $tid = $mysqli->insert_id;
                $cmd = '';
            } else {
                $cmd = 'AddGroup';
            }
            break;
        }
        case 'doEditGroup':
        {    $name = $_REQUEST['name'];
            if (empty($name)) {
                $name = '<Новая тема>';
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from advice_topics");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('update advice_topics set ord=' . $ord . ', name="' . $name . '" where id=' . $tid)) {
                $cmd = '';
            } else {
                $cmd = 'EditGroup';
            }
            break;
        }
        case 'doAddItem':
        {    $title = stripslashes($_REQUEST['title']);
            if (empty($title)) {
                $title = '<Новый совет/публикация>';
            }
            $txt = stripslashes($_REQUEST['txt']);
            $abstract = stripslashes($_REQUEST['abstract']);
            $tid = $_REQUEST['tid'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (!empty($_REQUEST['top'])) {
                $top = 1;
            } else {
                $top = 0;
            }
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from advice_data where tid=$tid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('insert into advice_data set tid=' . $tid . ', top=' . $top . ', ord=' . $ord . ', title="' . addslashes($title) . '", txt="' . addslashes($txt) . '", abstract="' . addslashes($abstract) . '"')) {
                $fid = $mysqli->insert_id;
                $cmd = '';
            } else {
                $cmd = 'AddItem';
            }
            break;
        }
        case 'doEditItem':
        {    $title = stripslashes($_REQUEST['title']);
            if (empty($title)) {
                $title = '<Новый совет/публикация>';
            }
            $txt = stripslashes($_REQUEST['txt']);
            $abstract = stripslashes($_REQUEST['abstract']);
            $tid = $_REQUEST['tid'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (!empty($_REQUEST['top'])) {
                $top = 1;
            } else {
                $top = 0;
            }
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from advice_data where tid=$tid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('update advice_data set tid=' . $tid . ', top=' . $top . ', ord=' . $ord . ', title="' . addslashes($title) . '", txt="' . addslashes($txt) . '", txt="' . addslashes($txt) . '", abstract="' . addslashes($abstract) . '" where id=' . $fid)) {
                $cmd = '';
            } else {
                $cmd = 'AddItem';
            }
            break;
        }
        case 'UpItem':
        {    $res = $mysqli->query('select ord from advice_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from advice_data where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update advice_data set ord=' . $upord . ' where id=' . $fid);
                    $mysqli->query('update advice_data set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }
        case 'UpGroup':
        {    $res = $mysqli->query('select ord from advice_topics where id=' . $tid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from advice_topics where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update advice_topics set ord=' . $upord . ' where id=' . $tid);
                    $mysqli->query('update advice_topics set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'EditGroup':
        {    $res = $mysqli->query('select name,ord from advice_topics where id=' . $tid);
            if ($row = $res->fetch_row()) {
                $old_name = $row[0];
                $old_ord = $row[1];
            }
        }
        case 'AddGroup':
        {    $name = $_REQUEST['name'];
            if (empty($name)) {
                $name = $old_name;
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $ord = $old_ord;
            }

            $body = preg_replace(
                array('/%tid%/','/%name%/','/%ord%/'),
                array($tid,$name,$ord),
                $tpl['advice']['editGroup']
            );
            break;
        }
        case 'EditItem':
        {    $res = $mysqli->query('select tid,title,txt,ord,abstract,top from advice_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                $old_tid = $row[0];
                $old_title = $row[1];
                $old_txt = $row[2];
                $old_ord = $row[3];
                $old_abstract = $row[4];
                $old_top = $row[5];
            }
        }
        case 'AddItem':
        {    if (empty($tid)) {
                $tid = $old_tid;
        }
            $title = $_REQUEST['title'];
        if (empty($title)) {
            $title = $old_title;
        }
            $txt = $_REQUEST['txt'];
        if (empty($txt)) {
            $txt = $old_txt;
        }
        if (empty($abstract)) {
            $abstract = $old_abstract;
        }
            $ord = $_REQUEST['ord'] + 0;
        if (empty($ord)) {
            $ord = $old_ord;
        }
        if (!empty($_REQUEST['top'])) {
            $top = $_REQUEST['top'];
        } else {
            $top = $old_top;
        }

            $txt = stripslashes(preg_replace("/\n|\r/", '', $txt));
            $abstract = stripslashes(preg_replace("/\n|\r/", '', $abstract));
            $title = stripslashes(preg_replace("/\n|\r/", '', $title));

            $tids = '';
            $res = $mysqli->query('select id,name from advice_topics');
        while ($row = $res->fetch_row()) {
            $tids .= '<option value=' . $row[0] . ($tid == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
        }

            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $txt1 = $CKE->editor('abstract', $abstract);
            $abstract = $sw->textareaAttributes; # Was $sw->getHtml();
            $abstract = preg_replace('/[\\\][\\\]n/', '', $abstract);

            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $txt1 = $CKE->editor('txt', $txt);
            $txt = $sw->textareaAttributes; # Was $txt=$sw->textareaAttributes; # Was $sw->getHtml();
            $txt = preg_replace('/[\\\][\\\]n/', '', $txt);


            $body = preg_replace(
                array('/%fid%/','/%tids%/','/%title%/','/%txt%/','/%ord%/','/%abstract%/','/%top%/'),
                array($fid,$tids,$title,$txt,$ord,$abstract,($top != 0 ? 'checked' : '')),
                $tpl['advice']['editItem']
            );

        break;
        }
        case 'DelItem':
        {    $res = $mysqli->query('select title,txt from advice_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%fid%/','/%title%/','/%txt%/'),
                    array($fid,$row[0],$row[1]),
                    $tpl['advice']['delItem']
                );
                break;
            } else {
                $cmd = '';
            }
        }
        case 'DelGroup':
        {    $res = $mysqli->query('select t.name,count(f.id) from advice_topics as t left join advice_data as f on (f.tid=t.id) where t.id=' . $tid . ' group by t.name');
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%tid%/','/%name%/','/%count%/'),
                    array($tid,$row[0],$row[1]),
                    (($row[1] + 0) > 0) ? $tpl['advice']['nodelGroup'] : $tpl['advice']['delGroup']
                );
                break;
            } else {
                $cmd = '';
            }
        }
        default:
        {    // Список советов/публикаций по темам
            $list = '';
            $even = false;
            $tid = -1;
            $upfid = 0;
            $uptid = 0;
            $res = $mysqli->query('select t.id,t.name,f.id,f.title from advice_topics as t left join advice_data as f on (f.tid=t.id) order by t.ord,f.ord');
            while ($row = $res->fetch_row()) {
                if ($row[0] != $tid) {
                    $list .= preg_replace(
                        array('/%tid%/','/%name%/','/%upid%/'),
                        array($row[0],$row[1],$uptid),
                        ($uptid == 0 ? $tpl['advice']['groupline'] : $tpl['advice']['grouplineup'])
                    );
                                    $tid = $row[0];
                                    $uptid = $row[0];
                                    $upfid = 0;
                                    $even = false;
                }
                if (!empty($row[2])) {
                    $list .= preg_replace(
                        array('/%fid%/','/%upid%/','/%title%/','/%bgcolor%/'),
                        array($row[2],$upfid,substr(strip_tags($row[3]), 0, 255),$tpl[$even ? 'even' : 'odd']),
                        ($upfid == 0 ? $tpl['advice']['itemline'] : $tpl['advice']['itemlineup'])
                    );
                        $upfid = $row[2];
                        $even = !$even;
                }
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['advice']['main']);
        }
    }
    return $body;
}
include_once 'ckeditor/ckeditor.php'; //mark3
$body = build_page();
