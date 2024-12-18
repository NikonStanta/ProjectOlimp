<?php

$formrefpar = '';
$refpar = '?';
$_OLYMP_YEAR = 2023;
include_once('../ainc/connect_db.inc');
include_once('../ainc/sess.inc');
// Построение структуры сайта
//
$sitemap[1000] = array('id' => 1000,'pid' => 0,'type' => 'script','ttl' => 'Структура','tpl' => 'page1', 'ref' => 'pagedefs.html', 'top' => 3);
$sitemap['noaccess'] = array('id' => 1,'pid' => 1000,'type' => 'script','ttl' => 'Нет доступа','tpl' => 'page1', 'ref' => 'noaccess.html');
$sitemap['divdefs'] = array('id' => 1001,'pid' => 1000,'type' => 'script','ttl' => 'Разделы','tpl' => 'page1', 'ref' => 'divdefs.html');
$sitemap['pagedefs'] = array('id' => 1002,'pid' => 1000,'type' => 'script','ttl' => 'Страницы','tpl' => 'page1', 'ref' => 'pagedefs.html');
$sitemap['menus'] = array('id' => 1003,'pid' => 1000,'type' => 'script','ttl' => 'Меню','tpl' => 'page1', 'ref' => 'menus.html');
//
$sitemap[2000] = array('id' => 2000,'pid' => 0,'type' => 'script','ttl' => 'Содержание','tpl' => 'page1', 'ref' => 'pages.html');
$sitemap['pages'] = array('id' => 2001,'pid' => 2000,'type' => 'script','ttl' => 'Страницы','tpl' => 'page1', 'ref' => 'pages.html');
$sitemap['news'] = array('id' => 2003,'pid' => 2000,'type' => 'script','ttl' => 'Новости','tpl' => 'page1', 'ref' => 'news.html');
$sitemap['actual'] = array('id' => 2004,'pid' => 2000,'type' => 'script','ttl' => 'Объявления','tpl' => 'page1', 'ref' => 'actual.html');
$sitemap['faq'] = array('id' => 2005,'pid' => 2000,'type' => 'script','ttl' => 'Вопросы/ответы','tpl' => 'page1', 'ref' => 'faq.html');
$sitemap['advice'] = array('id' => 2008,'pid' => 2000,'type' => 'script','ttl' => 'Советы','tpl' => 'page1', 'ref' => 'advice.html');
$sitemap['catalog'] = array('id' => 2006,'pid' => 2000,'type' => 'script','ttl' => 'Каталог','tpl' => 'page1', 'ref' => 'catalog.html');
$sitemap[3000] = array('id' => 3000,'pid' => 0,'type' => 'script','ttl' => 'Настройки','tpl' => 'page1', 'ref' => 'params.html');
$sitemap['params'] = array('id' => 3001,'pid' => 3000,'type' => 'script','ttl' => 'Параметры','tpl' => 'page1', 'ref' => 'params.html');
$sitemap['tpls'] = array('id' => 3002,'pid' => 3000,'type' => 'script','ttl' => 'Шаблоны','tpl' => 'page1', 'ref' => 'tpls.html');
$sitemap['patterns'] = array('id' => 3004,'pid' => 3000,'type' => 'script','ttl' => 'Макеты страниц','tpl' => 'page1', 'ref' => 'patterns.html');
//
$sitemap[4000] = array('id' => 4000,'pid' => 0,'type' => 'script','ttl' => 'Администратор','tpl' => 'page1', 'ref' => 'users.html', 'top' => 3);
$sitemap['users'] = array('id' => 4001,'pid' => 4000,'type' => 'script','ttl' => 'Пользователи','tpl' => 'page1', 'ref' => 'users.html', 'top' => 3);
$sitemap['fed'] = array('id' => 4002,'pid' => 4000,'type' => 'script','ttl' => 'FED','tpl' => 'page1', 'ref' => 'fed.html', 'top' => 3);
//
$sitemap[5000] = array('id' => 5000,'pid' => 0,'type' => 'script','ttl' => 'Олимпиада Админ','tpl' => 'page1', 'ref' => 'olimp_confirm_person.html');
$sitemap['olimp_confirm_person'] = array('id' => 5001,'pid' => 5000,'type' => 'script','ttl' => 'Подтверждение данных','tpl' => 'page1', 'ref' => 'olimp_confirm_person.html');
$sitemap['olimp_confirm_reculc'] = array('id' => 5004,'pid' => 5000,'type' => 'script','ttl' => 'Подтверждение перезачёта','tpl' => 'page1', 'ref' => 'olimp_confirm_reculc.html');
$sitemap['olimp_clean_schools'] = array('id' => 5003,'pid' => 5000,'type' => 'script','ttl' => 'Чистка школ','tpl' => 'page1', 'ref' => 'olimp_clean_schools.html');
$sitemap['olimp_stages'] = array('id' => 5010,'pid' => 5000,'type' => 'script','ttl' => 'Этапы олимпиады','tpl' => 'page1', 'ref' => 'olimp_stages.html');
$sitemap['olimp_scool_list'] = array('id' => 5011,'pid' => 5000,'type' => 'script','ttl' => 'Список площадок','tpl' => 'page1', 'ref' => 'olimp_scool_list.html');
$sitemap['olimp_scool_add'] = array('id' => 5012,'pid' => 5000,'type' => 'script','ttl' => 'Добавить площадку','tpl' => 'page1', 'ref' => 'olimp_scool_add.html');
$sitemap['olimp_scool_edit'] = array('id' => 5013,'pid' => 5000,'type' => 'script','ttl' => 'Редактирование площадки','tpl' => 'page1', 'ref' => 'olimp_scool_edit.html');
$sitemap['olimp_scool_del'] = array('id' => 5014,'pid' => 5000,'type' => 'script','ttl' => 'Удаление площадки','tpl' => 'page1', 'ref' => 'olimp_scool_del.html');
$sitemap['olimp_stage_edit'] = array('id' => 5015,'pid' => 5000,'type' => 'script','ttl' => 'Редактирование потока','tpl' => 'page1', 'ref' => 'olimp_stage_edit.html');
$sitemap['olimp_stage_del'] = array('id' => 5016,'pid' => 5000,'type' => 'script','ttl' => 'Удаление потока','tpl' => 'page1', 'ref' => 'olimp_stage_del.html');
$sitemap['olimp_stage_mass_edit'] = array('id' => 5017,'pid' => 5000,'type' => 'script','ttl' => 'Массовое редактирование потоков','tpl' => 'page1', 'ref' => 'olimp_stage_mass_edit.html');
$sitemap['olimp_stage_mass_del'] = array('id' => 5018,'pid' => 5000,'type' => 'script','ttl' => 'Массовое удаление потоков','tpl' => 'page1', 'ref' => 'olimp_stage_mass_del.html');
$sitemap['olimp_stage_add'] = array('id' => 5019,'pid' => 5000,'type' => 'script','ttl' => 'Добавление потока','tpl' => 'page1', 'ref' => 'olimp_stage_add.html');
$sitemap['olimp_stages_add'] = array('id' => 5020,'pid' => 5000,'type' => 'script','ttl' => 'Добавление потоков','tpl' => 'page1', 'ref' => 'olimp_stages_add.html');
$sitemap['olimp_stages_ob'] = array('id' => 5021,'pid' => 5000,'type' => 'script','ttl' => 'Объединение потоков','tpl' => 'page1', 'ref' => 'olimp_stages_ob.html');
$sitemap['olimp_vcl_manage'] = array('id' => 5022,'pid' => 5000,'type' => 'script','ttl' => 'Подготовка олимпиады с ВКС','tpl' => 'page1', 'ref' => 'olimp_vcl_manage.html');
//
$sitemap[6000] = array('id' => 6000,'pid' => 0,'type' => 'script','ttl' => 'Проведение олимпиады','tpl' => 'page1', 'ref' => 'olimp_stage_load.html');
$sitemap['olimp_stage_reg'] = array('id' => 6001,'pid' => 6000,'type' => 'script','ttl' => 'Регистрация участника этапа','tpl' => 'page1', 'ref' => 'olimp_stage_reg.html');
$sitemap['olimp_fstage_reg'] = array('id' => 6002,'pid' => 6000,'type' => 'script','ttl' => 'Регистрация участника закл. этапа','tpl' => 'page1', 'ref' => 'olimp_fstage_reg.html');
$sitemap['olimp_stage_load'] = array('id' => 6003,'pid' => 6000,'type' => 'script','ttl' => 'Загруженность потоков','tpl' => 'page1', 'ref' => 'olimp_stage_load.html');
$sitemap['olimp_vcl_admin'] = array('id' => 6004,'pid' => 6000,'type' => 'script','ttl' => 'Проведение олимпиады с ВКС','tpl' => 'page1', 'ref' => 'olimp_vcl_admin.html');
//
$sitemap[7000] = array('id' => 7000,'pid' => 0,'type' => 'script','ttl' => 'Обработка результатов','tpl' => 'page1', 'ref' => 'olimp_fill_results.html');
$sitemap['olimp_placing'] = array('id' => 7001,'pid' => 7000,'type' => 'script','ttl' => 'Рассадка','tpl' => 'page1', 'ref' => 'olimp_placing.html');
$sitemap['olimp_cipher'] = array('id' => 7002,'pid' => 7000,'type' => 'script','ttl' => 'Шифровка','tpl' => 'page1', 'ref' => 'olimp_cipher.html');
$sitemap['olimp_cipher_old'] = array('id' => 7102,'pid' => 7000,'type' => 'script','ttl' => 'Шифровка (старая)','tpl' => 'page1', 'ref' => 'olimp_cipher_old.html');
$sitemap['olimp_fill_results'] = array('id' => 7003,'pid' => 7000,'type' => 'script','ttl' => 'Ввод результатов','tpl' => 'page1', 'ref' => 'olimp_fill_results.html');
$sitemap['olimp_decode'] = array('id' => 7006,'pid' => 7000,'type' => 'script','ttl' => 'Расшифровка и коррекция','tpl' => 'page1', 'ref' => 'olimp_decode.html');
$sitemap['olimp_stage_results'] = array('id' => 7004,'pid' => 7000,'type' => 'script','ttl' => 'Дополнительный ввод результатов','tpl' => 'page1', 'ref' => 'olimp_stage_results.html');
$sitemap['olimp_appl_results'] = array('id' => 7005,'pid' => 7000,'type' => 'script','ttl' => 'Преобразование результатов','tpl' => 'page1', 'ref' => 'olimp_appl_results.html');
$sitemap['olimp_works'] = array('id' => 7007,'pid' => 7000,'type' => 'script','ttl' => 'Работы дистанционных этапов','tpl' => 'page1', 'ref' => 'olimp_works.html');
//$sitemap['fed']=array('id'=>5002,'pid'=>5000,'type'=>'script','ttl'=>'FED','tpl'=>'page1', 'ref'=>'fed.html');

$default_page = 'noaccess';
if ($role & 4) {
    $default_page = 'users';
}
if ($role & 2) {
    $default_page = 'pagedefs';
}
if ($role & 1) {
    $default_page = 'page';
}
if ($role & 64) {
    $default_page = 'olimp_vcl_admin';
}
if ($role & 128) {
    $default_page = 'olimp_fill_results';
}
if ($role & 16) {
    $default_page = 'olimp_stage_reg';
}
if ($role & 8) {
    $default_page = 'olimp_confirm_person';
}
if ($role & 512) {
    $default_page = 'olimp_works';
}
if ($role & 256) {
    $default_page = 'olimp_cipher';
}
if ($role & 32) {
    $default_page = 'olimp_stages';
}

$page_title = 'Олимпиада школьников "Надежда энергетики"  - Система администрирования';
$_image_dir = 'img';
$_site_root = '/home/ehope/data/www/energy-hope.ru/admin/';
$_menu1 = array();
// Для текущего пользователя

$submenu = array();
if ($role & 2) {
    $submenu[] = array('page' => 'divdefs','submenu' => '');
}
if ($role & 2) {
    $submenu[] = array('page' => 'pagedefs','submenu' => '');
}
if ($role & 2) {
    $submenu[] = array('page' => 'menus','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '1000', 'submenu' => $submenu);
}

$submenu = array();
if ($role & 1) {
    $submenu[] = array('page' => 'pages','submenu' => '');
}
if ($role & 1) {
    $submenu[] = array('page' => 'catalog','submenu' => '');
}
if ($role & 1) {
    $submenu[] = array('page' => 'news','submenu' => '');
}
if ($role & 1) {
    $submenu[] = array('page' => 'actual','submenu' => '');
}
if ($role & 1) {
    $submenu[] = array('page' => 'faq','submenu' => '');
}
if ($role & 1) {
    $submenu[] = array('page' => 'advice','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '2000', 'submenu' => $submenu);
}

$submenu = array();
if ($role & 1 + 2) {
    $submenu[] = array('page' => 'params','submenu' => '');
}
if ($role & 2) {
    $submenu[] = array('page' => 'tpls','submenu' => '');
}
if ($role & 2) {
    $submenu[] = array('page' => 'patterns','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '3000', 'submenu' => $submenu);
}

$submenu = array();
if ($role & 4) {
    $submenu[] = array('page' => 'users','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '4000', 'submenu' => $submenu);
}

$submenu = array();
if ($role & 8) {
    $submenu[] = array('page' => 'olimp_confirm_person','submenu' => '');
}
if ($role & 8) {
    $submenu[] = array('page' => 'olimp_confirm_reculc','submenu' => '');
}
if ($role & 8) {
    $submenu[] = array('page' => 'olimp_clean_schools','submenu' => '');
}
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_stages','submenu' => '');
}
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_scool_list','submenu' => '');
}
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_scool_add','submenu' => '');
}
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_stage_add','submenu' => '');
}
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_stages_add','submenu' => '');
}
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_vcl_manage','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '5000', 'submenu' => $submenu);
}

$submenu = array();
if ($role & 16) {
    $submenu[] = array('page' => 'olimp_stage_load','submenu' => '');
}
if ($role & 16) {
    $submenu[] = array('page' => 'olimp_stage_reg','submenu' => '');
}
if ($role & 16) {
    $submenu[] = array('page' => 'olimp_fstage_reg','submenu' => '');
}
if ($role & 64) {
    $submenu[] = array('page' => 'olimp_vcl_admin','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '6000', 'submenu' => $submenu);
}

$submenu = array();
if ($role & 32) {
    $submenu[] = array('page' => 'olimp_placing','submenu' => '');
}
if ($role & 256) {
    $submenu[] = array('page' => 'olimp_cipher','submenu' => '');
}
if ($role & 128) {
    $submenu[] = array('page' => 'olimp_fill_results','submenu' => '');
}
if ($role & 256) {
    $submenu[] = array('page' => 'olimp_decode','submenu' => '');
}
if ($role & 256) {
    $submenu[] = array('page' => 'olimp_appl_results','submenu' => '');
}
if ($role & 256) {
    $submenu[] = array('page' => 'olimp_stage_results','submenu' => '');
}
if ($role & 512) {
    $submenu[] = array('page' => 'olimp_works','submenu' => '');
}
if (is_array($submenu)) {
    $_menu1[] = array('page' => '7000', 'submenu' => $submenu);
}

$_max_html_edit = 1024 * 50;
$_image_exts = array(
1 => 'GIF',
2 => 'JPG',
3 => 'PNG',
4 => 'SWF',
5 => 'PSD',
6 => 'BMP',
7 => 'TIFF',
8 => 'TIFF',
9 => 'JPC',
10 => 'JP2',
11 => 'JPX',
12 => 'JB2',
13 => 'SWC',
14 => 'IFF');
$L_CASE = array('/й/','/ц/','/у/','/к/','/е/','/н/','/г/','/ш/','/щ/','/з/','/х/','/ъ/','/ф/','/ы/','/в/','/а/','/п/','/р/','/о/','/л/','/д/','/ж/','/э/','/я/','/ч/','/с/','/м/','/и/','/т/','/ь/','/б/','/ю/','/ё/','/q/','/w/','/e/','/r/','/t/','/y/','/u/','/i/','/o/','/p/','/a/','/s/','/d/','/f/','/g/','/h/','/j/','/k/','/l/','/z/','/x/','/c/','/v/','/b/','/n/','/m/');
$TL_CASE = array('й','ц','у','к','е','н','г','ш','щ','з','х','ъ','ф','ы','в','а','п','р','о','л','д','ж','э','я','ч','с','м','и','т','ь','б','ю','ё','q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m');
$U_CASE = array('/Й/','/Ц/','/У/','/К/','/Е/','/Н/','/Г/','/Ш/','/Щ/','/З/','/Х/','/Ъ/','/Ф/','/Ы/','/В/','/А/','/П/','/Р/','/О/','/Л/','/Д/','/Ж/','/Э/','/Я/','/Ч/','/С/','/М/','/И/','/Т/','/Ь/','/Б/','/Ю/','/Ё/','/Q/','/W/','/E/','/R/','/T/','/Y/','/U/','/I/','/O/','/P/','/A/','/S/','/D/','/F/','/G/','/H/','/J/','/K/','/L/','/Z/','/X/','/C/','/V/','/B/','/N/','/M/');
$TU_CASE = array('Й','Ц','У','К','Е','Н','Г','Ш','Щ','З','Х','Ъ','Ф','Ы','В','А','П','Р','О','Л','Д','Ж','Э','Я','Ч','С','М','И','Т','Ь','Б','Ю','Ё','Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M');

#include_once('spaw2/spaw.inc.php');
