<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE);
include_once('../ainc/init.inc');
include_once('../ainc/templates.inc');


if ($uid < 1) {
    $out = preg_replace(array('/%body%/','/%page_title%/'), array($tpl['logon'],$page_title), $tpl['page2']['main']);
    $out = preg_replace(array('/%refpar%/','/%formrefpar%/'), array($refpar,$formrefpar), $out);
    echo $out;
    exit;
}

$page = trim(preg_replace(array('%^/%','%\.[^\.]*$%'), '', $_GET['_PAGE']));

if (empty($page)) {
    $page = $default_page;
}
if (!$sitemap[$page]['type']) {
    $page = $default_page;
}
$id = $sitemap[$page]['id'];
$tplname = $sitemap[$page]['tpl'];
$out = $tpl[$tplname]['main'];

$xid = $id;
$pg = $page;
while (($pid = $sitemap[$pg]['pid']) > 0) {
    $xid = $pid;
    reset($sitemap);
    foreach ($sitemap as $k => $v) {
        if ($v['id'] == $pid) {
            $_page_title = $v['ttl'] . '-' . $page_title;        #markPavl changed $_page_title to $page_title
            $pg = $k;
        }
    }
}

$page_title .= '-' . $_page_title . $sitemap[$page]['ttl'];

$m1 = '';
$m2 = '';
$w = '';
//foreach ($_menu1[$role] as $v)
foreach ($_menu1 as $v) {
    $vv = $sitemap[$v['page']];
    if ($vv['id'] == $xid) {    // Меню для текущего раздела
        $m1 .= preg_replace(array('/%ref%/','/%ttl%/'), array($vv['ref'],$vv['ttl']), $tpl['menu1']['ttl']);
        $sub_menu = $v['submenu'];
        foreach ($sub_menu as $wv) {
            $sv = $sitemap[$wv['page']];
            if ($sv['id'] == $id) {
                $m1 .= preg_replace(array('/%ref%/','/%ttl%/'), array($sv['ref'],$sv['ttl']), $tpl['menu1']['citem']);
            } else {
                $m1 .= preg_replace(array('/%ref%/','/%ttl%/'), array($sv['ref'],$sv['ttl']), $tpl['menu1']['item']);
            }
        }
        $m2 .= preg_replace(array('/%ref%/','/%ttl%/'), array($vv['ref'],$vv['ttl']), $tpl['menu2']['citem']);
    } else // Список разделов
    {
        $m2 .= preg_replace(array('/%ref%/','/%ttl%/'), array($vv['ref'],$vv['ttl']), $tpl['menu2']['item']);
    }
}

$menu1 = preg_replace('/%menu%/', $m1, $tpl['menu1']['main']);
$menu2 = preg_replace('/%menu%/', $m2, $tpl['menu2']['main']);
$pagemenu = '';
$extra_js = '';

switch ($sitemap[$page]['type']) {
    case 'html':
    {    $body = @implode('', @file('../ainc/' . $page . '.inc'));
        break;
    }
    case 'script': {
        print_r('../ainc/' . $page . '.inc');
        include('../ainc/' . $page . '.inc');
        break;
    }
    case 'redirect':
    {    die(header('Location: ' . $sitemap[$page]['ref']));
        break;
    }
}

// Формирование страницы

$out = preg_replace(
    array('/%page_title%/','/%extra_js%/','/%menu1%/','/%menu2%/','/%pagemenu%/','/%user%/','/%title%/','/%body%/'),
    array($page_title,$extra_js,$menu1,$menu2,$pagemenu,$user,$sitemap[$page]['ttl'],$body),
    $out
);

$out = preg_replace(array('/%refpar%/','/%formrefpar%/'), array($refpar,$formrefpar), $out);

if ($body != '<!--DONE-->') {
    echo $out;
}
