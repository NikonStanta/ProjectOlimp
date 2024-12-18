<?php
session_start();
$formrefpar = '';
$refpar = '';
// init_mobile.php

// Подключиться к базе данных.
// На сервере.
$mysqli = new mysqli('localhost', 'p638056_ehope_sql', 'qW6tZ7(N0_hY8eY%wD2q', 'p638056_ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
$mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
$mysqli->query('SET SESSION sql_mode = ""');
// Локально
// $mysqli = new mysqli('localhost', 'root', 'mysql', 'ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
// $mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
// $mysqli->query('SET SESSION sql_mode = ""');

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

// page.php
$_GET['_PAGE'] = $_POST['page'];
preg_match('%/?([^./0-9]*)(/)?([^./0-9]*){0,1}([0-9]*){0,1}([a-z]{0,}){0,1}%', $_GET['_PAGE'], $ww);

$PAGE['PREFIX'] = $ww[1];
$PAGE['PAGE'] = $ww[3];
$PAGE['REQID'] = $ww[4];
$PAGE['REQTP'] = $ww[5];

$_SESSION['current_page'] = $PAGE['PREFIX'] . "/" . $PAGE['PAGE']; // Для перехода на мобильную версию сайта.
// Задан неверный префикс раздела
if (!array_search($PAGE['PREFIX'], $SITE_DIVPREFIX)) {
    reset($SITE_DIVPREFIX);
    $PAGE['PREFIX'] = current($SITE_DIVPREFIX); // берем префикс первого раздела
    // Страница не задана, возможно это не префикс а имя страницы а префикс пустой
    if (empty($PAGE['PAGE'])) {    // Если есть раздел с пустым префиксом, то был не префикс а имя страницы а префикс пустой
        if (array_search('', $SITE_DIVPREFIX)) {
            $PAGE['PREFIX'] = '';
            $PAGE['PAGE'] = $ww[1];
        }
    }
}

$PAGE['name'] = (!empty($PAGE['PREFIX']) ? $PAGE['PREFIX'] . '/' : '') . $PAGE['PAGE'];

$PAGE['index_page'] = empty($PAGE['name']);

//  Нет такой страницы в разделе
/*
if (empty($SITE_PAGE_IDS[$PAGE['name']])) {    // Выбираем страницу по умолчанию для раздела
    $PAGE['PAGE'] = $SITE_PREFIXPAGE[$PAGE['PREFIX']];
    $PAGE['name'] = (!empty($PAGE['PREFIX']) ? $PAGE['PREFIX'] . '/' : '') . $PAGE['PAGE'];
    if (!$PAGE['index_page']) {
        header("HTTP/1.1 301 Moved Permanently");
        header("Location: /" . $PAGE['name'] . $PAGE['REQID'] . $PAGE['REQTP'] . $SITE_VARS['Default ext']);
        die();
    }
}
    */

$PAGE['id'] = $SITE_PAGE_IDS[$PAGE['name']] + 0;
// Параметры страницы
$res = $mysqli->query('select p.did,p.name,p.title,p.theme,p.page_tp,p.page_subtp,i.title,p.keywords,p.descr,p.meta,p.ttla,t.fullout from site_pages as p left join site_themes as i on (i.id=p.theme) left join types_page as t on (t.id=p.page_tp) where p.id=' . $PAGE['id']);
$row = $res->fetch_row();
$PAGE['did'] = $row[0] + 0;
$PAGE['div_name'] = $SITE_DIVS[$row[0]];
$PAGE['name'] = $row[1];
$PAGE['title'] = $row[2];
$PAGE['win_title'] = $SITE_VARS['Site browser title'] . ($row[2] ? ': ' : '') . $row[2];
$PAGE['theme'] = $row[3];
$PAGE['type'] = $row[4];
$PAGE['subtype'] = $row[5];
$PAGE['theme_ttl'] = $row[6];
$PAGE['meta_keywords'] = $row[7];
$PAGE['meta_descr'] = $row[8];
$PAGE['meta_extra'] = $row[9];
$PAGE['ttla'] = $row[10];
$PAGE['fullout'] = $row[11];

$PAGE['selfref'] = 'http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];

// Шаблоны

// Общие шаблоны
$res = $mysqli->query('select t.tp,t.name,t.txt from site_div_tpl as pt left join site_templates as t on (t.id=pt.tplid) where pt.did=0');
while ($row = $res->fetch_assoc()) {
    if ($row['tp'] == 0) {
        $tpl[$row['name']] = $row['txt'];
    } else {
        $w1 = explode('::TEMPLATE::', $row['txt']);
        foreach ($w1 as $v) {
            $w2 = explode('::', $v);
            $w[$w2[0]] = $w2[1];
        }
        $tpl[$row['name']] = $w;
    }
}
// Шаблоны раздела
$res = $mysqli->query('select t.tp,t.name,t.txt from site_div_tpl as pt left join site_templates as t on (t.id=pt.tplid) where pt.did=' . $PAGE['did']);
while ($row = $res->fetch_assoc()) {
    if ($row['tp'] == 0) {
        $tpl[$row['name']] = $row['txt'];
    } else {
        $w1 = explode('::TEMPLATE::', $row['txt']);
        foreach ($w1 as $v) {
            $w2 = explode('::', $v);
            $w[$w2[0]] = $w2[1];
        }
        $tpl[$row['name']] = $w;
    }
}
// Шаблоны страницы
$res = $mysqli->query('select t.tp,t.name,t.txt from site_page_tpl as pt left join site_templates as t on (t.id=pt.tplid) where pt.pgid=' . $PAGE['id']);
while ($row = $res->fetch_assoc()) {
    if ($row['tp'] == 0) {
        $tpl[$row['name']] = $row['txt'];
    } else {
        $w1 = explode('::TEMPLATE::', $row['txt']);
        foreach ($w1 as $v) {
            $w2 = explode('::', $v);
            //$w[$w2[0]]=$w2[1];
            if (count($w2) < 2) {
                continue;
            }
            $tpl[$row['name']][$w2[0]] = $w2[1];
        }
    //        $tpl[$row['name']]=$w;
    }
}



$PAGE['path_nodes'][] = array('name' => $PAGE['div_name'],'ref' => $PAGE['PREFIX'] . '/');
$PAGE['path_nodes'][] = array('name' => $PAGE['title'],'ref' => ($PAGE['PREFIX'] ? $PAGE['PREFIX'] . '/' : '') . $PAGE['name'] . $SITE_VARS['Default ext'] . $refpar);

//$PAGE['path']=preg_replace(array('/%divname%/','/%pagename%/','/%pageref%/','/%divref%/'),
//    array($PAGE['div_name'],$PAGE['title'],$PAGE['name'],$SITE_DEFPAGE[$PAGE['did']]['name']),$tpl['path']);

function _build_path()
{
    global $PAGE,$tpl,$U_CASE,$TL_CASE;
    $p = '';
    if (array_key_exists('path', $tpl)) {
        end($PAGE['path_nodes']);
        $l = key($PAGE['path_nodes']);
        foreach ($PAGE['path_nodes'] as $k => $w) {
            $p .= preg_replace(array('/%name%/','/%ref%/'), $w, ($k == $l) ? $tpl['path']['lastnode'] : $tpl['path']['node']);
        }
        $p = preg_replace(array('/%path%/'), $p, $tpl['path']['main']);
        $p = preg_replace($U_CASE, $TL_CASE, $p);
    }
    return $p;
}


// logonout.php

if (array_key_exists('uid', $_SESSION) && $_SESSION['uid'] > 0) {
  //if ($_SESSION['debug']>0) echo 'select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.role from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where u.id='.$_SESSION['uid'];

# AND (FIND_IN_SET('.$_SESSION['role'].',role)>0)
# select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.nic="DanshinRA") AND (FIND_IN_SET(1,role)>0)
# $res=$mysqli->query('select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.nic="DanshinRA") AND (FIND_IN_SET(1,role)>0)');
  $res = $mysqli->query('select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z,u.id from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.id="' . $_SESSION['uid'] . '" OR u.nic="' . $_SESSION['uid'] . '") AND (FIND_IN_SET(' . $_SESSION['role'] . ',role)>0)');

  if ($row = $res->fetch_row()) {
      $USER_INFO['uid'] = $row[6];
      $USER_INFO['fio'] = $row[1];
      $USER_INFO['role'] = $_SESSION['role'];
      $USER_INFO['roles'] = $_SESSION['roles'];
      $USER_INFO['pid'] = $row[2];
      $USER_INFO['class'] = $row[3];
      $USER_INFO['year'] = $row[4];
      $USER_INFO['printed_z'] = $row[5];
  } else {
      $_SESSION['uid'] = -1;
  }
} elseif (!array_key_exists('uid', $_SESSION)) {
  $_SESSION['uid'] = -1;   //Не было
}
if ($_SESSION['uid'] < 1) {
  $USER_INFO['uid'] = 0;
  $USER_INFO['fio'] = '';
  $USER_INFO['role'] = 0;
  $USER_INFO['roles'] = 0;
  $USER_INFO['pid'] = 0;
  $USER_INFO['class'] = 0;
  $USER_INFO['year'] = 0;
  $USER_INFO['printed_z'] = 'N';
}


