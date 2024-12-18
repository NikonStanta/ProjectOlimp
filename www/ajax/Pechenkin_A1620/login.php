<?php
session_start();

if (isset($_POST['login']) && isset($_POST['password'])) {
  // Подключение БД на сервере.
  $mysqli = new mysqli('localhost', 'p638056_ehope_sql', 'qW6tZ7(N0_hY8eY%wD2q', 'p638056_ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
  $mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
  $mysqli->query('SET SESSION sql_mode = ""');
  // Локальное подключение БД.
  // $mysqli = new mysqli('localhost', 'root', 'mysql', 'ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
  // $mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
  // $mysqli->query('SET SESSION sql_mode = ""');

  $login = json_decode($_POST['login']);
  $password = json_decode($_POST['password']);

  $nic = trim($login);
  $_SESSION["nic"] = $nic;

  $pwd = $mysqli->real_escape_string(trim($password));      // !!
  $role = 1; // Для мобильной версии только роль "участник".

  $res = $mysqli->query('select id,locked,role from olimp_users  where (nic="' . $nic . '") AND (pwd=UNHEX(SHA("' . $pwd . '"))) AND (FIND_IN_SET(' . $role . ',role)>0)');
  $date = array(

  );
  if ($row = $res->fetch_row()) {
    $_SESSION["uid"] = $row[0];
    $_SESSION["role"] = $role;
    $_SESSION["roles"] = $row[2];
    $mysqli->query('update olimp_users set logs_success=logs_success+1,logs_attempt=0, logs_dt=Now() where id=' . $_SESSION['uid']);

    $date["login_status"] = 1;
  } else {
    $date["login_status"] = 0;
  }
  // Получение данных о пользователе.
  $res = $mysqli->query('select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z,u.id from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.id="' . $_SESSION['uid'] . '" OR u.nic="' . $_SESSION['uid'] . '") AND (FIND_IN_SET(' . $_SESSION['role'] . ',role)>0)');
  if ($row = $res->fetch_row()) {
    // FIXME: разобраться, какие данные сохранять и где их хранить.
    $date['fio'] = $row[1];
    $_SESSION['pid'] = $row[2];
    // $USER_INFO['class'] = $row[3];
    // $USER_INFO['year'] = $row[4];
    // $USER_INFO['printed_z'] = $row[5];
  } else {
    $_SESSION['uid'] = -1;
  }
  // Обработка данных, введенных пользователем.  
  $date["uid"] = $_SESSION["uid"];
  $date["nic"] = $nic;
  $date["login"] = $nic;
  $date["password"] = $pwd;
  echo json_encode($date);
}

if (isset($_POST['logout'])) {
  $_SESSION["nic"] = 0;
  $_SESSION["uid"] = 0;
  $_SESSION["role"] = 0;
  $_SESSION["roles"] = 0;
  unset($_SESSION["nic"]);
  unset($_SESSION["uid"]);
  unset($_SESSION["role"]);
  unset($_SESSION["roles"]);

  // $answer['result'] = 'ok';
  $answer = array(
    "result" => "ok"
  );
  echo json_encode($answer);
}

function is_login()
{
  $res = false;
  if (isset($_SESSION['id'])) {
    $res = true;
  }
  return $res;
}