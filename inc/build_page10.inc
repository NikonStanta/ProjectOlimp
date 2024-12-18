<?php

// Построение карты сайта

function build_page($pgid, $subtype)
{
    global $SITE_VARS,$SITE_DIVS,$tpl;

// Построение меню по разделам
    $dmenu0 = get_page_list(1, 0, $tpl['menu']);
    $dmenu1 = get_page_list(2, 0, $tpl['menu']);
    $dmenu2 = get_page_list(3, 0, $tpl['menu']);
    $dmenu3 = get_page_list(4, 0, $tpl['menu']);
    $dmenu4 = get_page_list(5, 0, $tpl['menu']);
    $dmenu5 = get_page_list(6, 0, $tpl['menu']);
    $dmenu6 = get_page_list(7, 0, $tpl['menu']);

// Построение тела страницы
    $body = preg_replace(
        array('/%divname1%/','/%divname2%/','/%divname3%/','/%divname4%/','/%divname5%/','/%divname6%/','/%divref1%/','/%divref2%/','/%divref3%/','/%divref4%/','/%divref5%/','/%divref6%/','/%divmenu0%/','/%divmenu1%/','/%divmenu2%/','/%divmenu3%/','/%divmenu4%/','/%divmenu5%/','/%divmenu6%/'),
        array($SITE_DIVS[2],$SITE_DIVS[3],$SITE_DIVS[4],$SITE_DIVS[5],$SITE_DIVS[6],$SITE_DIVS[7],$SITE_DEFPAGE[2]['name'],$SITE_DEFPAGE[3]['name'],$SITE_DEFPAGE[4]['name'],$SITE_DEFPAGE[5]['name'],$SITE_DEFPAGE[6]['name'],$SITE_DEFPAGE[7]['name'],$dmenu0,$dmenu1,$dmenu2,$dmenu3,$dmenu4,$dmenu5,$dmenu6),
        $tpl['sitemap']['body']
    );

    return $body;
}
