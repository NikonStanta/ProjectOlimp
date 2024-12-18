<?php

// Формируемая страница
//$PAGE['name']=trim(preg_replace(array('%^/%','%\.[^\.]*$%'),'',$_GET['_PAGE']));

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
if (empty($SITE_PAGE_IDS[$PAGE['name']])) {    // Выбираем страницу по умолчанию для раздела
    $PAGE['PAGE'] = $SITE_PREFIXPAGE[$PAGE['PREFIX']];
    $PAGE['name'] = (!empty($PAGE['PREFIX']) ? $PAGE['PREFIX'] . '/' : '') . $PAGE['PAGE'];
    if (!$PAGE['index_page']) {
        header("HTTP/1.1 301 Moved Permanently");
        header("Location: /" . $PAGE['name'] . $PAGE['REQID'] . $PAGE['REQTP'] . $SITE_VARS['Default ext']);
        die();
    }
}

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
