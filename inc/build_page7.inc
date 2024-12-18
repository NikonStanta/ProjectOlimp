<?php

// Построение страницы актуально

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$tpl,$PAGE,$refpar;
    $cmd = '';
    if (!empty($SITE_VARS['ActualIdLength'])) {
        $_ITEM_LEN = $SITE_VARS['ActualIdLength'];
    } else {
        $_ITEM_LEN = 8;
    }

    if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 'n') || ($PAGE['REQTP'] == 'N')) && (($PAGE['REQID'] + 0) > 0)) {
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


// Выбранная статья - подробный текст

    if ($cur_year < 1900) {
        $cur_year = date("Y");
    }

    if ($nid > 0) {
        $res = $mysqli->query('select date_format(dt,"%d.%m.%Y"),ttl,txt2,date_format(dt,"%Y") from actual where id=' . $nid);
        if ($res->num_rows < 1) {
            $nid = 0;
        }
    }

    if ($nid <= 0) {
        $res = $mysqli->query('select date_format(dt,"%d.%m.%Y"),ttl,txt2,date_format(dt,"%Y") from actual where dt between "' . $cur_year . '-01-01" AND "' . $cur_year . '-12-31" order by dt desc LIMIT 1');
    }

    if ($row = $res->fetch_row()) {
        $top = preg_replace(
            array('/%dt%/','/%ttl%/','/%txt%/'),
            array($row[0],stripslashes($row[1]),stripslashes($row[2])),
            $tpl['actual']['top']
        );
        $cur_year = $row[3];
    } else {
        $top = '';
    }


// Все статьи за год

    $res = $mysqli->query('select id,date_format(dt,"%d.%m.%Y"),ttl,txt1,LENGTH(txt2) from actual where dt between "' . $cur_year . '-01-01" AND "' . $cur_year . '-12-31" order by dt desc');
    $w = '';
    while ($row = $res->fetch_row()) {
        $w .= preg_replace(
            array('/%id%/','/%dt%/','/%ttl%/','/%txt%/','/%refid%/'),
            array($row[0],$row[1],stripslashes($row[2]),stripslashes($row[3]),
            $PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar),
            ($row[3] > 0) ? $tpl['actual']['txtp2'] : $tpl['actual']['txtp1']
        );
    }

    $actual = preg_replace('/%list%/', $w, $tpl['actual']['list']);
    $res = $mysqli->query('select DISTINCT date_format(dt,"%Y") as y from actual order by dt desc');
    $w = '';
    while ($row = $res->fetch_row()) {
        $w .= preg_replace(array('/%refid%/','/%year%/'), array($PAGE['name'] . sprintf('%04dy', $row[0]) . $SITE_VARS['Default ext'] . $refpar,$row[0]), ($row[0] == $cur_year) ? $tpl['actual']['cur_year'] : $tpl['actual']['year']);
    }

    $years = preg_replace('/%years%/', $w, $tpl['actual']['years']);
    $body = preg_replace(array('/%top%/','/%list%/','/%cur_year%/','/%years%/'), array($top,$actual,$cur_year,$years), $tpl['actual']['main']);
    return $body;
}
