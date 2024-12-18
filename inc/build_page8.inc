<?php

// Построение страницы новости

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$tpl,$PAGE,$refpar, $_PAGE_VAR_VALUES;

    $cmd = '';

    if (!empty($SITE_VARS['NewsIdLength'])) {
        $_ITEM_LEN = $SITE_VARS['NewsIdLength'];
    } else {
        $_ITEM_LEN = 8;
    }
    if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 'n') || ($PAGE['REQTP'] == 'N')) && (intval($PAGE['REQID']) > 0)) {
        $nid = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['nid'])) {
            $nid = $_REQUEST['nid'];
        } else {
            $nid = 0;
        }
    }

    if ((($PAGE['REQTP'] == 'y') || ($PAGE['REQTP'] == 'Y')) && (($PAGE['REQID'] + 0) > 0)) {
        $cur_year = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['year'])) {
            $cur_year = $_REQUEST['year'];
        } else {
            $cur_year = 0;
        }
    }

// Формирование страницы

// Выбранная новость - подробный текст

    if ($nid > 0) {
        $res = $mysqli->query('select date_format(dt,"%d.%m.%Y"),ttl,txt2,date_format(dt,"%Y"),id,img from news where id=' . $nid);
        if ($res->num_rows < 1) {
            $nid = 0;
        }
    }

    if (($nid <= 0) && (($tpl['news']['force_top'] + 0) == 1)) {
        if ($cur_year < 1900) {
            $res = $mysqli->query('select date_format(dt,"%d.%m.%Y"),ttl,txt2,date_format(dt,"%Y"),id,img from news order by dt desc LIMIT 1');
        } else {
            $res = $mysqli->query('select date_format(dt,"%d.%m.%Y"),ttl,txt2,date_format(dt,"%Y"),id,img from news where dt between "' . $cur_year . '-01-01" AND "' . $cur_year . '-12-31" order by dt desc LIMIT 1');
        }
        $nid = -1;
    }

    if ($cur_year < 1900) {
        $cur_year = date("Y");
    }

    $top = '';
    if (($nid != 0) && ($row = $res->fetch_row())) {
        $nid = $row[4];
        $top = preg_replace(
            array('/%id%/','/%dt%/','/%ttl%/','/%txt%/','/%img%/'),
            array($nid,$row[0],stripslashes($row[1]),stripslashes($row[2]),$row[5]),
            $tpl['news']['top']
        );
        $cur_year = $row[3];
    }


// Все новости за год

    $res = $mysqli->query('select id,date_format(dt,"%d.%m.%Y"),ttl,txt1,LENGTH(txt2),img from news where dt between "' . $cur_year . '-01-01" AND "' . $cur_year . '-12-31" order by dt desc');
    $w = '';
    while ($row = $res->fetch_row()) {
        $w .= preg_replace(
            array('/%id%/','/%dt%/','/%ttl%/','/%txt%/','/%refid%/','/%img%/'),
            array($row[0],$row[1],stripslashes($row[2]),stripslashes($row[3]),
            $PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar,stripslashes($row[5])),
            ($row[3] > 0) ? $tpl['news']['txt2'] : $tpl['news']['txt1']
        );
    }

    $news = preg_replace('/%list%/', $w, $tpl['news']['list']);

    $res = $mysqli->query('select DISTINCT date_format(dt,"%Y") as y from news order by dt desc');
    $w = '';
    while ($row = $res->fetch_row()) {
        $w .= preg_replace(
            array('/%refid%/','/%year%/'),
            array($PAGE['name'] . sprintf('%04dy', $row[0]) . $SITE_VARS['Default ext'] . $refpar,$row[0]),
            ($row[0] == $cur_year) ? $tpl['news']['cur_year'] : $tpl['news']['year']
        );
    }

    $years = preg_replace('/%years%/', $w, $tpl['news']['years']);

// Последние %LasNewsCount% новостей

    if (!empty($SITE_VARS['LasNewsCount'])) {
        $_LAST_NEWS_COUNT = $SITE_VARS['LasNewsCount'];
    } else {
        $_LAST_NEWS_COUNT = 5;
    }

    $res = $mysqli->query('select id,date_format(dt,"%d.%m.%Y"),ttl,txt1,LENGTH(txt2),img from news order by dt desc LIMIT ' . $_LAST_NEWS_COUNT);
    $w = '';
    while ($row = $res->fetch_row()) {
        $w .= preg_replace(
            array('/%id%/','/%dt%/','/%ttl%/','/%txt%/','/%refid%/','/%img%/'),
            array($row[0],$row[1],stripslashes($row[2]),stripslashes($row[3]),
            $PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar,stripslashes($row[5])),
            ($row[3] > 0) ? $tpl['news']['lasttxt2' . (($row[0] == $nid) ? 'c' : '')] : $tpl['news']['lasttxt1' . (($row[0] == $nid) ? 'c' : '')]
        );
    //    echo ('lasttxt2'.(($row[0]==$nid) ? 'c' : '')).'<br>';
    }

    $lastnews = preg_replace('/%list%/', $w, $tpl['news']['last']);

    if (!empty($SITE_VARS['LastNewsVarName'])) {
        $_PAGE_VAR_VALUES[$SITE_VARS['LastNewsVarName']] = $_PAGE_VAR_VALUES[$SITE_VARS['LastNewsVarName']] . $lastnews;
    }

    if (empty($top)) {
        $body = preg_replace(array('/%list%/','/%cur_year%/','/%years%/','/%last%/'), array($news,$cur_year,$years,$lastnews), $tpl['news']['main_list']);
    } else {
        $body = preg_replace(array('/%top%/','/%list%/','/%cur_year%/','/%years%/','/%last%/'), array($top,$news,$cur_year,$years,$lastnews), $tpl['news']['main_top']);
    }

    return $body;
}
