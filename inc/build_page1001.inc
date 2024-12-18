<?php

// Построение страницы советы

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$tpl;

    if (!empty($SITE_VARS['adviceTopicIdLength'])) {
        $_GROUP_LEN = $SITE_VARS['adviceTopicIdLength'];
    } else {
        $_GROUP_LEN = 4;
    }

    $cmd = '';

    if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 'i') || ($PAGE['REQTP'] == 'I')) && (($PAGE['REQID'] + 0) > 0)) {
        $iid = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['iid'])) {
            $iid = $_REQUEST['iid'];
        } else {
            $iid = 0;
        }
    }

    if ($iid == 0) {
        if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 't') || ($PAGE['REQTP'] == 'T')) && (($PAGE['REQID'] + 0) > 0)) {
            $tid = $PAGE['REQID'] + 0;
        } else {
            if (!empty($_REQUEST['tid'])) {
                $tid = $_REQUEST['tid'];
            } else {
                $tid = 0;
            }
        }
    } else {
        $tid = 0;
    }


// Список тематик советов

        $res = $mysqli->query('select id,name from advice_topics order by ord');
    while ($row = $res->fetch_row()) {
        $topics[$row[0]] = $row[1];
    }


        // Выбранный совет
    if ($iid > 0) {
        $res = $mysqli->query('select title,txt,tid from advice_data where id=' . $iid);
        if ($row = $res->fetch_row()) {
            $advice_title = $row[0];
            $advice_txt = $row[1];
            $tid = $row[2];
        } else {
            $iid = 0;
        }
    }

        // Выбранная тематика
    if (empty($tid) || empty($topics[$tid])) {
        $tid = key($topics);
    }

        // Список тематик

    if (count($topics) > 1) {
        $w = '';
        foreach ($topics as $k => $v) {
            $w .= preg_replace(
                array('/%id%/','/%name%/','/%refid%/'),
                array($k,$v,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'd', $k) . $SITE_VARS['Default ext']),
                $k == $tid ? $tpl['advice']['topic_cline'] : $tpl['advice']['topic_line']
            );
        }
        $topic_list = preg_replace('/%list%/', $w, $tpl['advice']['topics']);
    } else {
        $topic_list = '';
    }


        // Советы выбранной тематики

        $advice_list = '';
        $res = $mysqli->query('select id,title,abstract from advice_data where tid=' . $tid . ' order by top desc, ord desc');
    while ($row = $res->fetch_row()) {
        $advice_list .= preg_replace(
            array('/%id%/','/%title%/','/%abstract%/'),
            array(stripslashes($row[0]),stripslashes($row[1]),stripslashes($row[2])),
            $tpl['advice']['data_line']
        );
    }


    if ($iid > 0) {
        $body = preg_replace(array('/%topic%/','/%topics%/','/%list%/','/%title%/','/%txt%/','/%tid%/'), array($topics[$tid],$topic_list,$advice_list,$advice_title,$advice_txt,$tid), $tpl['advice']['body_det']);
    } else {
        $body = preg_replace(array('/%topic%/','/%topics%/','/%list%/'), array($topics[$tid],$topic_list,$advice_list), $tpl['advice']['body_list']);
    }

    return $body;
}
