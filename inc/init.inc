<?php

$formrefpar = '';
$refpar = '';
session_start();

if (isset($_REQUEST['debug'])) {
    $_SESSION['debug'] = $_REQUEST['debug'];
}

if (array_key_exists('debug', $_SESSION) && !empty($_SESSION['debug'])) {
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    ini_set('error_reporting', E_ALL);
}

$mysqli = new mysqli('localhost', 'p638056_ehope_sql', 'qW6tZ7(N0_hY8eY%wD2q', 'p638056_ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
$mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
$mysqli->query('SET SESSION sql_mode = ""');


// Системные переменные
$res = $mysqli->query('select name,value from vars_string');
while ($row = $res->fetch_row()) {
    $SITE_VARS[$row[0]] = $row[1];
}
$res = $mysqli->query('select name,value from vars_text');
while ($row = $res->fetch_row()) {
    $SITE_VARS[$row[0]] = $row[1];
}

// Типы страниц
$res = $mysqli->query('select id,name from types_page');
while ($row = $res->fetch_row()) {
    $SITE_PAGE_TYPES[$row[0]] = $row[1];
}

// Структура сайта
$res = $mysqli->query('select d.id,d.name,d.def_page,p.name,d.prefix,d.menu1,d.email from site_divs as d left join site_pages as p on (p.id=d.def_page) order by d.ord');
while ($row = $res->fetch_row()) {
    $SITE_DIVS[$row[0]] = $row[1];
    $SITE_DIVPREFIX[$row[0]] = trim($row[4]);
    $SITE_DIVMENU[$row[0]] = $row[5] + 0;
    $SITE_DIVEMAIL[$row[0]] = $row[6];
    $SITE_DEFPAGE[$row[0]] = array('id' => $row[2],'name' => $row[4] . (!empty($row[4]) ? '/' : '') . $row[3]);
    $SITE_PREFIXPAGE[trim($row[4])] = $row[3];
    $res2 = $mysqli->query('select id,name,title from site_pages where (locked=0) and (did=' . $row[0] . ') order by ord');
    $_SITE_LINKS['REF_DIV_ID_' . $row[0]] = '/' . $SITE_DEFPAGE[$row[0]]['name'] . $SITE_VARS['Default ext'];
    $_PAGE_VAR_VALUES['REF_DIV_ID_' . $row[0]] = '/' . $SITE_DEFPAGE[$row[0]]['name'] . $SITE_VARS['Default ext'];
    while ($row2 = $res2->fetch_assoc()) {
        if (!isset($w)) {
            $w = array();
        }
        $w[$row2['id']] = $row2;
        $full_name = (!empty($row[4]) ? $row[4] . '/' : '') . $row2['name'];
        $w[$row2['id']]['name'] = $full_name;
        $SITE_PAGE_IDS[$full_name] = $row2['id'];
        $_SITE_LINKS['REF_PAGE_ID_' . $row2['id']] = '/' . $full_name . $SITE_VARS['Default ext'];
        $_PAGE_VAR_VALUES['REF_PAGE_ID_' . $row2['id']] = '/' . $full_name . $SITE_VARS['Default ext'];
        //$_SITE_LINKS['REF_PAGE_ID_'.$row2[id]]='/'.$full_name.$SITE_VARS['Default ext'];          // put id in ''  ==>  'id'
        //$_PAGE_VAR_VALUES['REF_PAGE_ID_'.$row2[id]]='/'.$full_name.$SITE_VARS['Default ext'];
    }
    $SITE_PAGES[$row[0]] = $w;
}

$L_CASE = array('/й/','/ц/','/у/','/к/','/е/','/н/','/г/','/ш/','/щ/','/з/','/х/','/ъ/','/ф/','/ы/','/в/','/а/','/п/','/р/','/о/','/л/','/д/','/ж/','/э/','/я/','/ч/','/с/','/м/','/и/','/т/','/ь/','/б/','/ю/','/ё/','/q/','/w/','/e/','/r/','/t/','/y/','/u/','/i/','/o/','/p/','/a/','/s/','/d/','/f/','/g/','/h/','/j/','/k/','/l/','/z/','/x/','/c/','/v/','/b/','/n/','/m/');
$TL_CASE = array('й','ц','у','к','е','н','г','ш','щ','з','х','ъ','ф','ы','в','а','п','р','о','л','д','ж','э','я','ч','с','м','и','т','ь','б','ю','ё','q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m');
$U_CASE = array('/Й/','/Ц/','/У/','/К/','/Е/','/Н/','/Г/','/Ш/','/Щ/','/З/','/Х/','/Ъ/','/Ф/','/Ы/','/В/','/А/','/П/','/Р/','/О/','/Л/','/Д/','/Ж/','/Э/','/Я/','/Ч/','/С/','/М/','/И/','/Т/','/Ь/','/Б/','/Ю/','/Ё/','/Q/','/W/','/E/','/R/','/T/','/Y/','/U/','/I/','/O/','/P/','/A/','/S/','/D/','/F/','/G/','/H/','/J/','/K/','/L/','/Z/','/X/','/C/','/V/','/B/','/N/','/M/');
$TU_CASE = array('Й','Ц','У','К','Е','Н','Г','Ш','Щ','З','Х','Ъ','Ф','Ы','В','А','П','Р','О','Л','Д','Ж','Э','Я','Ч','С','М','И','Т','Ь','Б','Ю','Ё','Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M');
