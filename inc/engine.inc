<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE);






include_once('Pechenkin_A1620/mobile_detect.inc'); //
// include_once('Pechenkin_A1620/identif_device_type.inc'); //
include_once('Pechenkin_A1620/main_mobile_page.inc'); //
$detect = new Mobile_Detect();
if(!isset($_SESSION['use_desktop'])){
	$_SESSION['use_desktop'] = 'no';
}
// !array_key_exists('use_desktop', $_GET)
// if(($detect->isMobile() || $detect->isTablet()) && !isset($_GET['use_desktop']) && $_SESSION['use_desktop'] != 'yes' ||
// 	isset($_GET['use_desktop'])&& $_GET['use_desktop'] != 'yes'){
if(($detect->isMobile() || $detect->isTablet()) && !array_key_exists('use_desktop', $_GET) && $_SESSION['use_desktop'] != 'yes' ||
	array_key_exists('use_desktop', $_GET)&& $_GET['use_desktop'] != 'yes'){
	header("Content-Type: text/html; charset=utf-8");
	header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
	Header("Expires: " . gmdate("D, d M Y H:i:s") . " GMT");
	Header("Pragma: no-cach");
	Header("Cache-Control: no-cach, must-revalidate");
	
	$_SESSION['use_desktop'] = 'no';
	echo $mobile_page;
	exit();
}
if(isset($_GET['use_desktop'])&& $_GET['use_desktop'] == 'yes'){
	$_SESSION['use_desktop'] = 'yes';
} elseif (isset($_GET['use_desktop'])&& $_GET['use_desktop'] != 'yes'){
	$_SESSION['use_desktop'] = 'no';
}

include_once('../inc/init.inc');
include_once('../inc/page.inc');
include_once('../inc/menu.inc');
include_once('../inc/cabinet.inc');
include_once('../inc/logonout.inc');

include('../inc/build_page' . $PAGE['type'] . '.inc');

if ($PAGE['fullout'] == 'Y') {
    die(build_page($PAGE['id'], $PAGE['subtype']));
}

if (empty($PAGE['meta_keywords'])) {
    $PAGE['meta_keywords'] = $SITE_VARS['meta_keywords'];
}
if (empty($PAGE['meta_descr'])) {
    $PAGE['meta_descr'] = $SITE_VARS['meta_descr'];
}
$PAGE['meta_extra'] = $SITE_VARS['meta_extra'] . $PAGE['meta_extra'];
if (!empty($PAGE['ttla'])) {
    $PAGE['win_title'] = $PAGE['ttla'];
}

$res = $mysqli->query('select name,tpl,did from site_menu where (did=0) OR (did=' . $PAGE['did'] . ') order by did desc');  //desc - порядок убывания
while ($row = $res->fetch_row()) {
    if (empty($_PAGE_VAR_VALUES[$row[0]])) {
        $_PAGE_VAR_VALUES[trim($row[0])] = get_menu(0, $row[0], $tpl[$row[1] ? $row[1] : $row[0]]);
    }
}

if (empty($_REQUEST['print'])) {
    $OUT = $tpl['main'];
} else {
    $OUT = $tpl['print'];
    $refpar = '?print=on';
    $formrefpar = '<hidden name=print value=on>';
}

// Построение списка новостей
$res = $mysqli->query('select p.title,p.name,d.prefix from site_pages as p left join site_divs as d on (d.id=p.did) where p.page_tp=8 order by p.id');
if ($row = $res->fetch_row()) {
    $news_ref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1] . $SITE_VARS['Default ext'];
    $news_iref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1];
    $news_name = $row[0];

    if (empty($SITE_VARS['TopNewsDateFormat'])) {
        $TopNewsDateFormat = '%d.%m.%Y';
    } else {
        $TopNewsDateFormat = $SITE_VARS['TopNewsDateFormat'];
    }

    $res = $mysqli->query('select id,date_format(dt,"' . $TopNewsDateFormat . '"),ttl,txt1,LENGTH(txt2),img from news where top=1 order by dt desc');
    $num = $res->num_rows;        // !!!
    $i = 1;
    $even = true;
    $news_list = '';
    while ($row = $res->fetch_row()) {
        $news_list .= preg_replace(
            array('/%id%/','/%dt%/','/%ttl%/','/%txt%/','/%refid%/','/%img%/'),
            array($row[0],$row[1],$row[2],$row[3],$news_iref . sprintf('%08d', $row[0]) . $SITE_VARS['Default ext'],$row[5]),
            $tpl['news']['line' . ($even ? '1' : '2') . ($row[5] > 0 ? 'd' : 'n') . (empty($_REQUEST['print']) ? '' : 'p')]
        );
         $i++;
        $even = !$even;
    }
} else {
    $news_ref = '';
    $news_name = '';
    $news_list = '';
}


$_PAGE_VAR_VALUES['news_block'] = preg_replace('/%news%/', $news_list, $tpl['news']['block']);

// Построение списка актуально.

$res = $mysqli->query('select p.title,p.name,d.prefix from site_pages as p left join site_divs as d on (d.id=p.did) where p.page_tp=7 order by p.id');
if ($row = $res->fetch_row()) {
    $actual_ref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1] . $SITE_VARS['Default ext'];
    $actual_iref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1];
    $actual_name = $row[0];

    if (empty($SITE_VARS['TopNewsDateFormat'])) {
        $TopNewsDateFormat = '%d.%m.%Y';
    } else {
        $TopNewsDateFormat = $SITE_VARS['TopNewsDateFormat'];
    }

    $res = $mysqli->query('select a.id,date_format(a.dt,"' . $TopNewsDateFormat . '"),a.ttl,a.txt1,LENGTH(a.txt2) from actual as a left join actual_div as d on (d.aid=a.id) where d.did=' . $PAGE['did'] . ' order by dt desc');

    $num = $res->num_rows;
    $i = 1;
    $even = true;
    $actual_list = '';
    while ($row = $res->fetch_row()) {
        $actual_list .= preg_replace(
            array('/%id%/','/%dt%/','/%ttl%/','/%txt%/','/%refid%/'),
            array($row[0],$row[1],$row[2],$row[3],$actual_iref . sprintf('%08d', $row[0]) . $SITE_VARS['Default ext']),
            $tpl['actual']['block_line' . ($even ? '1' : '2') . ($row[4] > 0 ? 'd' : 'n')]
        );
         $i++;
        $even = !$even;
    }
} else {
    $actual_ref = '';
    $actual_name = '';
    $actual_list = '';
}


$_PAGE_VAR_VALUES['actual_block'] = preg_replace('/%lines%/', $actual_list, $tpl['actual']['block']);


// Построение списка советов/публикаций
$res = $mysqli->query('select p.title,p.name,d.prefix from site_pages as p left join site_divs as d on (d.id=p.did) where p.page_tp=1001 order by p.id');
if ($row = $res->fetch_row()) {
    $advice_ref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1] . $SITE_VARS['Default ext'];
    $advice_iref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1];
    $advice_name = $row[0];

    $res = $mysqli->query('select a.id,a.title,a.abstract,LENGTH(a.txt) from advice_data as a left join advice_topics as t on (t.id=a.tid) where a.top=1 order by t.ord, a.ord');
    $num = $res->num_rows;
    $i = 1;
    $even = true;
    $advice_list = '';
    while ($row = $res->fetch_row()) {
        $advice_list .= preg_replace(
            array('/%id%/','/%title%/','/%abstract%/','/%refid%/'),
            array($row[0],$row[1],$row[2],$advice_iref . sprintf('%08d', $row[0]) . $SITE_VARS['Default ext']),
            $tpl['advice']['line' . ($even ? '1' : '2') . ($row[3] > 0 ? 'd' : 'n')]
        );
         $i++;
        $even = !$even;
    }
} else {
    $advice_ref = '';
    $advice_name = '';
    $advice_list = '';
}


$_PAGE_VAR_VALUES['advice_block'] = preg_replace('/%lines%/', $advice_list, $tpl['advice']['block']);


// Выборка faq


$res = $mysqli->query('select p.title,p.name,d.prefix from site_pages as p left join site_divs as d on (d.id=p.did) where p.page_tp=5 order by p.id');
if ($row = $res->fetch_row()) {
    $faq_ref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1] . $SITE_VARS['Default ext'];
    $faq_iref = '/' . $row[2] . ($row[2] ? '/' : '') . $row[1];
    $faq_name = $row[0];

    if (!empty($SITE_VARS['FAQTopicIdLength'])) {
        $_GROUP_LEN = $SITE_VARS['FAQTopicIdLength'];
    } else {
        $_GROUP_LEN = 4;
    }
    if (!empty($SITE_VARS['FAQItemIdLength'])) {
        $_ITEM_LEN = $SITE_VARS['FAQItemIdLength'];
    } else {
        $_ITEM_LEN = 4;
    }

    $faq_aids = array();

    $res = $mysqli->query('select id from faq_data');
    while ($row = $res->fetch_row()) {
        $faq_aids[] = $row[0];
    }

    if (empty($SITE_VARS['FAQsCount'])) {
        $FAQsCount = 2;
    } else {
        $FAQsCount = $SITE_VARS['FAQsCount'];
    }

    shuffle($faq_aids);

    $faq_aids = array_slice($faq_aids, 0, $FAQsCount);

    $faq_ids = '';

    foreach ($faq_aids as $w) {
        $faq_ids .= ($faq_ids ? ',' : '') . $w;
    }

    $faq_list = '';
    $i = 1;
    $even = true;
    $res = $mysqli->query('select id,tid,question,abstract from faq_data where id in (' . $faq_ids . ')');
    while ($row = $res->fetch_row()) {
        $faq_list .= preg_replace(
            array('/%id%/','/%tid%/','/%question%/','/%abstract%/','/%refid%/'),
            array($row[0],$row[1],$row[2],$row[3],$faq_iref . sprintf('%0' . $_GROUP_LEN . 'd', $row[1]) . $SITE_VARS['Default ext'] . '#' . sprintf('%0' . $_ITEM_LEN . 'd', $row[0])),
            $tpl['faq']['line' . ($even ? '1' : '2')]
        );
         $i++;
        $even = !$even;
    }

    $_PAGE_VAR_VALUES['faq_block'] = preg_replace('/%faq%/', $faq_list, $tpl['faq']['block']);
}

// ----------?


if ($USER_INFO['uid'] > 0) {
    $_PAGE_VAR_VALUES['reg'] = preg_replace(array('/%user_name%/'), array($USER_INFO['fio']), $tpl['reg']['logout']);
    if (!empty($_SESSION['master_uid'])) {
        $master = '<input type="hidden" name="master_hash" value="' . $_SESSION['master_hash'] . '">';
    } else {
        $master = '';
    }
    $_PAGE_VAR_VALUES['logon_cabinet'] = preg_replace(
        array('/%user_name%/','/%cabinet_menu%/','/%master%/','/%pid%/'),
        array($USER_INFO['fio'],cabinet_user_box_menu(),$master,$USER_INFO['uid']),
        $tpl['reg']['cabinet_box' . $_SESSION['role']]
    );
} else {
    $_PAGE_VAR_VALUES['reg'] = $tpl['reg']['logon'];
    $_PAGE_VAR_VALUES['logon_cabinet'] = $tpl['reg']['logon_box'];
}


// Тело страницы заданного типа

$body = build_page($PAGE['id'], $PAGE['subtype']);

$OUT = preg_replace('/%body%/', $body, $OUT);

$PAGE['path'] = _build_path();



// Формирование страницы

if (empty($PAGE['theme'])) {
    $PAGE['theme'] = 1;
}
if (empty($site_theme)) {
    $site_theme = 'blue';
}
$_PAGE_VAR_VALUES['site_theme'] = $site_theme;


preg_match_all('/(?<=%)[a-zA-Z_][a-zA-Z0-9_]*?(?=%)/', $OUT, $_PAGE_VAR_NAMES);
foreach ($_PAGE_VAR_VALUES as $k => $v) {
    $_PAGE_VAR_NAMES[0][] = $k;
}
$w = array_unique($_PAGE_VAR_NAMES[0]);
foreach ($w as $_VAR_NAME) {
    if (($_VAR_NAME != 'refpar') || ($_VAR_NAME != 'formrefpar')) {
        if (empty($_PAGE_VAR_VALUES[$_VAR_NAME])) {
            $_PAGE_VAR_VALUES[$_VAR_NAME] = array_key_exists($_VAR_NAME, $PAGE) ? $PAGE[$_VAR_NAME] :
                (array_key_exists($_VAR_NAME, $SITE_VARS) ? $SITE_VARS[$_VAR_NAME] :
                (array_key_exists($_VAR_NAME, $tpl) ? $tpl[$_VAR_NAME] :
                (array_key_exists($_VAR_NAME, $GLOBALS) ? $GLOBALS[$_VAR_NAME] : ''
                )));
        }
        $_PAGE_VAR_PATTERNS[$_VAR_NAME] = '/%' . $_VAR_NAME . '%/';
    }
}
ksort($_PAGE_VAR_PATTERNS);
ksort($_PAGE_VAR_VALUES);


$OUT = preg_replace($_PAGE_VAR_PATTERNS, $_PAGE_VAR_VALUES, $OUT);

$_SITE_LINK_NAMES = array();
foreach ($_SITE_LINKS as $k => $v) {
    $_SITE_LINK_NAMES[$k] = '/%' . $k . '%/';
}
$OUT = preg_replace($_SITE_LINK_NAMES, $_SITE_LINKS, $OUT);

$OUT = preg_replace(array('/%refpar%/','/%formrefpar%/'), array($refpar,$formrefpar), $OUT);

header("Content-Type: text/html; charset=utf-8");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
Header("Expires: " . gmdate("D, d M Y H:i:s") . " GMT");
Header("Pragma: no-cach");
Header("Cache-Control: no-cach, must-revalidate");

echo stripslashes($OUT);
