<?php
session_start();
// $style_path = "css"; // Путь на локальном компьютере.
$style_path = "css/Pechenkin_A1620/css"; // Путь на сервере.
// $script_path = "js"; // Путь на локальном компьютере.
$script_path = "css/Pechenkin_A1620/js"; // Путь на сервере.
$microtime = (microtime(true) . rand());
// if(isset($_SESSION['uid']) && $_SESSION['uid'] > 0){ // && $_SESSION['uid'] > 0.
if(array_key_exists('uid', $_SESSION) && $_SESSION['uid'] > 0){ // && $_SESSION['uid'] > 0.
  $is_login = 'true';
  $left_menu = '
  <div class="left-menu-popup menu-popup" id="left-menu-popup">
  <ul>
    <li><a href="cabinet/profile" class="submenuTrigger inner-a">Профиль</a>
    <li><a href="cabinet/documents" class="submenuTrigger inner-a">Документы</a>
    <li><a href="cabinet/stages" class="submenuTrigger inner-a">Регистрация</a>
    <li><a href="cabinet/distance" class="submenuTrigger inner-a">Участие</a>
    <li><a href="cabinet/works" class="submenuTrigger inner-a">Загрузка</a>
    <li><a href="cabinet/results" class="submenuTrigger inner-a">Результаты</a>
    <li><a href="" class="submenuTrigger" id="outbtn">Выход</a>
    <li><a href="http://dev.ehope.ru?use_desktop=yes" target="_blank" class="submenuTrigger" id = "go-to-desktop">Перейти на настольную версию сайта</a>
  </ul>
  </div> ';  
  $main_content = '
  <!--
  <div id="main-content">    
    <div id="text-content">
      <div class="msg_success">Пользователь: '.$_SESSION['nic'].', id: '.$_SESSION['uid'].'.</div>
    </div>
  </div>
  -->
  <div id="page-content-wrap">
    <div id="page-msg" class="msg_success">
      Пользователь: '.$_SESSION['nic'].', id: '.$_SESSION['uid'].'.
    </div>
    <div id="page-main-content">      
    </div>
  </div>
  ';
} else {
  $is_login = 'false';
  $left_menu = '<div class="left-menu-popup menu-popup" id="left-menu-popup">
  <ul>
    <li id="left-menu-login"><a href="user/login" class="submenuTrigger inner-a">Вход в кабинет</a>
    <li id="left-menu-signup"><a href="user/signup" class="submenuTrigger inner-a">Регистрация</a>
    <li><a href="http://dev.ehope.ru?use_desktop=yes" target="_blank" class="submenuTrigger" id = "go-to-desktop">Перейти на настольную версию сайта</a>            
  </ul>
  </div>';
  $main_content = '
  <!-- <div id="main-content">
    <div class="login-and-signup-btns" id="login-and-signup-btns">
      <ul>
        <li><a href="user/login" class="submenuTrigger loginTrigger inner-a">Вход в кабинет</a>
        <li><a href="user/signup" class="submenuTrigger signupTrigger inner-a">Регистрация</a>
      </ul>
    </div>
    <div id="text-content"></div>    
  </div>
  -->
  <div id="page-content-wrap">
    <div id="page-msg">
    </div>
    <div id="page-main-content">   
      <div class="login-and-signup-btns" id="login-and-signup-btns">
        <ul>
          <li><a href="user/login" class="submenuTrigger loginTrigger inner-a">Вход в кабинет</a>
          <li><a href="user/signup" class="submenuTrigger signupTrigger inner-a">Регистрация</a>
        </ul>
      </div>   
    </div>
  </div>
  ';
}

$mobile_page = '
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <meta http-equiv="Cache-Control" content="no-cache" />
  <meat http-equiv="Pragma" content="no-cache"></meat>
  <meat http-equiv="Expires" content="0"></meat>
  <title>Надежда энергетики</title>
  <link rel="stylesheet" href="%STYLE_PATH%/style.css?v=%MICROTIME%" />
  <!-- <script src="%SCRIPT_PATH%/jquery-1.11.1.js?v=%MICROTIME%"></script> -->
  <script src="%SCRIPT_PATH%/jquery-3.7.1.min.js?v=%MICROTIME%" defer></script>
  <script src="%SCRIPT_PATH%/func_ui.js?v=%MICROTIME%" defer></script>
  <script src="%SCRIPT_PATH%/main_functions.js?v=%MICROTIME%" defer></script>
  <script src="%SCRIPT_PATH%/initialise.js?v=%MICROTIME%" defer></script>

</head>
<body>
  <div class="header">
    <a class="left-menu-trigger menu-trigger" href="#"></a>
    <div class="logo-background"> 
      <div class="logo">
        <a href="main/main" class="logo-a"><img src="img/logo1.gif" alt="Image" class="logo-img"></a>
      </div>
    </div>
    <a class="right-menu-trigger menu-trigger" href="#"></a>
  </div>
  <div class="menu">
    <!-- Левое меню -->
    %LEFT_MENU%
    
    <!-- Правое меню -->
    <div class="right-menu-popup menu-popup">
      <ul>
        <li><a href="#" class="submenuTrigger" id="submenu1Trigger">Об олимпиаде</a>
          <ul class="submenu" id="submenu1">
            <li><a href="about/statement" class="inner-a">Положение об олимпиаде</a></li>
            <li><a href="about/reglament" class="inner-a">Регламент проведения</a></li>
            <li><a href="about/istoriya_olimpiadi" class="inner-a">История олимпиады</a></li>
            <li><a href="about/organizatori" class="inner-a">Организаторы олимпиады</a></li>
            <li><a href="about/partner" class="inner-a">Партнеры олимпиады</a></li>
          </ul>
        </li>
        <li><a href="#" class="submenuTrigger" id="submenu2Trigger">Информация о текущем сезоне</a>
          <ul class="submenu" id="submenu2">
            <li><a href="actual/grafik" class="inner-a">График мероприятий</a></li>
            <li><a href="actual/result" class="inner-a">Результаты Олимпиады</a></li>
            <li><a href="actual/criterii" class="inner-a">Критерии</a></li>
            <li><a href="actual/tasks" class="inner-a">Материалы заданий</a></li>
          </ul>
        </li>
        <li><a href="#" class="submenuTrigger" id="submenu3Trigger">Участникам</a>
          <ul class="submenu" id="submenu3">
            <li><a href="olymp/nextnear" class="inner-a">Ближайшие мероприятия</a></li>
            <li><a href="olymp/metodic" class="inner-a">Методические материалы</a></li>
            <li><a href="olymp/liter" class="inner-a">Перечень рекомендуемой литературы</a></li>
            <li><a href="olymp/tasks" class="inner-a">Материалы заданий прошлых лет</a></li>
            <li><a href="olymp/orgadd" class="inner-a">Организационная поддержка участников</a></li>
            <li><a href="olymp/obrazci" class="inner-a">Бланки и образцы документов</a></li>
            <li><a href="olymp/pdf" class="inner-a">Работа с форматом файла PDF</a></li>
          </ul>
        </li>
        <li><a href="#" class="submenuTrigger" id="submenu4Trigger">Новичкам</a>
          <ul class="submenu" id="submenu4">
            <li><a href="novice/kak_prinyat_uchastie" class="inner-a">Как принять участие</a></li>
            <li><a href="novice/otveti_na_voprosi" class="inner-a">Часто задаваемые вопросы</a></li>
          </ul>
        </li>
        <li><a href="#" class="submenuTrigger" id="submenu5Trigger">Призерам прошлых лет</a>
          <ul class="submenu" id="submenu5">
            <li><a href="lastyear/fisgia" class="inner-a">Внесение сведений в ФИС ГИА и приема</a></li>
            <li><a href="lastyear/actionplan_20" class="inner-a">Как засчитать призовое место прошлого года</a></li>
          </ul>
        </li>
        <li><a href="contact/contact" class="submenuTrigger inner-a" id="submenu7Trigger">Контакты</a>
        </li>
      </ul>
    </div>
  </div>
  %MAIN_CONTENT%
  <!--<div id="main-content">
    <div class="login-and-signup-btns" id="login-and-signup-btns">
      <ul>
        <li><a href="user/login" class="submenuTrigger loginTrigger inner-a">Вход в кабинет</a>
        <li><a href="user/signup" class="submenuTrigger signupTrigger inner-a">Регистрация</a>
      </ul>
    </div>
    <div id="text-content"></div>    
  </div>-->
  <script>
	let is_login = %IS_LOGIN%;
	let currentPageOnDesktop = "%CURRENT_PAGE_DESKTOP%";
  </script>
</body>
</html> ';

if(array_key_exists('current_page', $_SESSION)){
  $current_page_desktop = $_SESSION['current_page'];
} else {
  $current_page_desktop = "";
}

$mobile_page = preg_replace(
  array("/%SCRIPT_PATH%/", "/%STYLE_PATH%/", "/%MICROTIME%/", "/%LEFT_MENU%/", "/%IS_LOGIN%/", "/%MAIN_CONTENT%/", "/%CURRENT_PAGE_DESKTOP%/"),
  array($script_path, $style_path, $microtime, $left_menu, $is_login, $main_content, $current_page_desktop),
  $mobile_page
);

// echo $mobile_page;