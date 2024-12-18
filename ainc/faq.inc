<?php

// Редактирование вопросов и ответов

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
            $res = $mysqli->query('select tid from faq_data where id=' . $fid);
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
                $mysqli->query('delete from faq_data where id=' . $fid);
                $cmd = '';
        }
            break;

        }
        case 'doDelGroup':
        {    if (!empty($tid)) {
                $mysqli->query('delete from faq_topics where id=' . $tid);
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
                $res = $mysqli->query("select max(ord) from faq_topics");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('insert into faq_topics set ord=' . $ord . ', name="' . $name . '"')) {
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
                    $res = $mysqli->query("select max(ord) from faq_topics");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('update faq_topics set ord=' . $ord . ', name="' . $name . '" where id=' . $tid)) {
                    $cmd = '';
            } else {
                    $cmd = 'EditGroup';
            }
            break;

        }
        case 'doAddItem':
        {    $question = stripslashes($_REQUEST['question']);
            if (empty($question)) {
                $question = '<Новый вопрос>';
            }
            $answer = stripslashes($_REQUEST['answer']);
            $abstract = stripslashes($_REQUEST['abstract']);
            $tid = $_REQUEST['tid'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from faq_data where tid=$tid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('insert into faq_data set tid=' . $tid . ', ord=' . $ord . ', question="' . addslashes($question) . '", answer="' . addslashes($answer) . '", abstract="' . addslashes($abstract) . '"')) {
                $fid = $mysqli->insert_id;
                $cmd = '';
            } else {
                    $cmd = 'AddItem';
            }
            break;

        }
        case 'doEditItem':
        {    $question = stripslashes($_REQUEST['question']);
            if (empty($question)) {
                $question = '<Новый вопрос>';
            }
            $answer = stripslashes($_REQUEST['answer']);
            $abstract = stripslashes($_REQUEST['abstract']);
            $tid = $_REQUEST['tid'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from faq_data where tid=$tid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('update faq_data set tid=' . $tid . ', ord=' . $ord . ', question="' . addslashes($question) . '", answer="' . addslashes($answer) . '", answer="' . addslashes($answer) . '", abstract="' . addslashes($abstract) . '" where id=' . $fid)) {
                $cmd = '';
            } else {
                    $cmd = 'AddItem';
            }
            break;

        }
        case 'UpItem':
        {    $res = $mysqli->query('select ord from faq_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from faq_data where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update faq_data set ord=' . $upord . ' where id=' . $fid);
                    $mysqli->query('update faq_data set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;

        }
        case 'UpGroup':
        {    $res = $mysqli->query('select ord from faq_topics where id=' . $tid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from faq_topics where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update faq_topics set ord=' . $upord . ' where id=' . $tid);
                    $mysqli->query('update faq_topics set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;

        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'EditGroup':
        {    $res = $mysqli->query('select name,ord from faq_topics where id=' . $tid);
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

            $body = preg_replace(array('/%tid%/','/%name%/','/%ord%/'), array($tid,$name,$ord), $tpl['faq']['editGroup']);
            break;

        }
        case 'EditItem':
        {    $res = $mysqli->query('select tid,question,answer,ord,abstract from faq_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                        $old_tid = $row[0];
                $old_question = $row[1];
                $old_answer = $row[2];
                $old_ord = $row[3];
                $old_abstract = $row[4];
            }

        }
        case 'AddItem':
        {    if (empty($tid)) {
                $tid = $old_tid;
        }
            $question = $_REQUEST['question'];
        if (empty($question)) {
            $question = $old_question;
        }
            $answer = $_REQUEST['answer'];
        if (empty($answer)) {
            $answer = $old_answer;
        }
        if (empty($abstract)) {
            $abstract = $old_abstract;
        }
            $ord = $_REQUEST['ord'] + 0;
        if (empty($ord)) {
            $ord = $old_ord;
        }

            $answer = stripslashes(preg_replace("/\n|\r/", '', $answer));
        $abstract = stripslashes(preg_replace("/\n|\r/", '', $abstract));
        $question = stripslashes(preg_replace("/\n|\r/", '', $question));
        $tids = '';
        $res = $mysqli->query('select id,name from faq_topics');
        while ($row = $res->fetch_row()) {
            $tids .= '<option value=' . $row[0] . ($tid == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
        }

            $CKE = new CKEditor();
        $CKE->returnOutput = true;
        $txt1 = $CKE->editor('abstract', $abstract);
        $abstract = $sw->textareaAttributes;
# Was $sw->getHtml();
            $abstract = preg_replace('/[\\\][\\\]n/', '', $abstract);
        $CKE = new CKEditor();
        $CKE->returnOutput = true;
        $txt1 = $CKE->editor('answer', $answer);
        $answer = $sw->textareaAttributes;
# Was $sw->getHtml();
            $answer = preg_replace('/[\\\][\\\]n/', '', $answer);
        $body = preg_replace(array('/%fid%/','/%tids%/','/%question%/','/%answer%/','/%ord%/','/%abstract%/'), array($fid,$tids,$question,$answer,$ord,$abstract), $tpl['faq']['editItem']);
        break;

        }
        case 'DelItem':
        {    $res = $mysqli->query('select question,answer from faq_data where id=' . $fid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(array('/%fid%/','/%question%/','/%answer%/'), array($fid,$row[0],$row[1]), $tpl['faq']['delItem']);
                break;
            } else {
                $cmd = '';
            }

        }
        case 'DelGroup':
        {    $res = $mysqli->query('select t.name,count(f.id) from faq_topics as t left join faq_data as f on (f.tid=t.id) where t.id=' . $tid . ' group by t.name');
            if ($row = $res->fetch_row()) {
                $body = preg_replace(array('/%tid%/','/%name%/','/%count%/'), array($tid,$row[0],$row[1]), (($row[1] + 0) > 0) ? $tpl['faq']['nodelGroup'] : $tpl['faq']['delGroup']);
                break;
            } else {
                    $cmd = '';
            }

        }
        default:
        {    // Список вопросов по темам
            $list = '';
            $even = false;
            $tid = -1;
            $upfid = 0;
            $uptid = 0;
            $res = $mysqli->query('select t.id,t.name,f.id,f.question from faq_topics as t left join faq_data as f on (f.tid=t.id) order by t.ord,f.ord');
            while ($row = $res->fetch_row()) {
                if ($row[0] != $tid) {
                    $list .= preg_replace(
                        array('/%tid%/','/%name%/','/%upid%/'),
                        array($row[0],$row[1],$uptid),
                        ($uptid == 0 ? $tpl['faq']['groupline'] : $tpl['faq']['grouplineup'])
                    );
                    $tid = $row[0];
                    $uptid = $row[0];
                    $upfid = 0;
                    $even = false;
                }
                if (!empty($row[2])) {
                    $list .= preg_replace(
                        array('/%fid%/','/%upid%/','/%question%/','/%bgcolor%/'),
                        array($row[2],$upfid,substr(strip_tags($row[3]), 0, 255),$tpl[$even ? 'even' : 'odd']),
                        ($upfid == 0 ? $tpl['faq']['itemline'] : $tpl['faq']['itemlineup'])
                    );
                    $upfid = $row[2];
                    $even = !$even;
                }
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['faq']['main']);

        }
    }
    return $body;
}
include_once 'ckeditor/ckeditor.php';
//mark3
$body = build_page();
