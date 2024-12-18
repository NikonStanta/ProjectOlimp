<?php
session_start();
// На сервере.
$mysqli = new mysqli('localhost', 'p638056_ehope_sql', 'qW6tZ7(N0_hY8eY%wD2q', 'p638056_ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
$mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
$mysqli->query('SET SESSION sql_mode = ""');
// Локально
// $mysqli = new mysqli('localhost', 'root', 'mysql', 'ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
// $mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
// $mysqli->query('SET SESSION sql_mode = ""');

if ($_SESSION['uid'] < 1) {
  // return $tpl['cabinet']['logon_form'];
  $respond = array(
    "error" => 1,
    "error_text" => "Необходимо войти в личный кабинет."
  );
  echo json_encode($respond);
  return;
}
// Проверяю открыт ли кабинет.
$res = $mysqli->query('SELECT value FROM vars_string WHERE name = "cabinet_opened"');
$row = $res->fetch_assoc();
$cabinet_opened = $row['value'];
if (!$cabinet_opened && empty($_SESSION['debug'])) {
  // return $tpl['cabinet']['closed'];
  $respond = array(
    "error" => 2,
    "error_text" => "Личный кабинет закрыт."
  );
  echo json_encode($respond);
  return;
}

$strRespond = array(
  "loadfiles" => "yes"
);

$docs_dir = '../docs/applications';
$max_file_size = 10485760;
$errors = [];
$path = '';

$allowed_file_types = array(
  'Application' => array(/*'image/jpeg'=>0,*/ 'application/pdf' => 1/*,'application/x-rar-compressed'=>0*/),
  'SchoolDoc' => array('image/jpeg' => 1, 'application/pdf' => 1),
  'IdentDoc' => array('image/jpeg' => 1, 'application/pdf' => 1),
  'IdentPhoto' => array('image/jpeg' => 1)
);

if (isset($_GET['UploadSchoolDoc'])) { // Загрузка справки школьника.
  //$strRespond['UploadSchoolDoc'] = "yes";
  if (is_uploaded_file($_FILES['schooldocfile']['tmp_name'])) {
    $FileType = mime_content_type($_FILES['schooldocfile']['tmp_name']);
    // Проверка размера файла
    if (!array_key_exists($FileType, $allowed_file_types['SchoolDoc'])) {
      $respond = array(
        "error" => 3,
        "error_text" => "Недопустимый формат файла. Документ не загружен. "
      );
      echo json_encode($respond);
      return;
    }
    // Проверка типа файла.
    if ($_FILES['schooldocfile']['size'] > $max_file_size) {
      $respond = array(
        "error" => 3,
        "error_text" => "Слишком большой файл. Справка не загружена."
      );
      echo json_encode($respond);
      return;
    }
    // Сохранение файла.
    $schooldoc_file = $docs_dir . '/schooldoc' . $_SESSION['uid'];
    $_LoadStatus = 'Loaded';
    if (file_exists($schooldoc_file)) {
      $schooldoc_file_old = $docs_dir . '/schooldoc' . $_SESSION['uid'] . '_' . date('YmdHis');
      rename($schooldoc_file, $schooldoc_file_old);
      $_LoadStatus = 'Updated';
    } else {
      $schooldoc_file_old = '';
    }

    if (move_uploaded_file($_FILES['schooldocfile']['tmp_name'], $schooldoc_file)) {
      $mysqli->query('update olimp_persons set SchoolDoc="' . $_LoadStatus . '" WHERE id=' . $_SESSION['uid']);
      $check_result['SchoolDoc'] = $_LoadStatus;
      $respond = array(
        "error" => 0,
        "error_text" => "Справка загружена."
      );
      echo json_encode($respond);
      return;
    } else {
      if ($schooldoc_file_old) {
        rename($schooldoc_file_old, $schooldoc_file);
        $respond = array(
          "error" => 3,
          "error_text" => "Ошибка при загрузке файла. Новая справка не загружена."
        );
        echo json_encode($respond);
        return;
      } else {
        $respond = array(
          "error" => 3,
          "error_text" => "Ошибка при загрузке файла. Справка не загружена."
        );
        echo json_encode($respond);
        return;
      }
    }
  } else {
    $respond = array(
      "error" => 3,
      "error_text" => "Отсутствует загружаемый файл."
    );
    echo json_encode($respond);
  }

}

if (isset($_GET['UploadIdentDoc'])) { // Загрузка документа, идентфицирующего участника.
  //$strRespond['UploadSchoolDoc'] = "yes";
  if (is_uploaded_file($_FILES['identdocfile']['tmp_name'])) {
    $FileType = mime_content_type($_FILES['identdocfile']['tmp_name']);
    // Проверка размера файла
    if (!array_key_exists($FileType, $allowed_file_types['IdentDoc'])) {
      $respond = array(
        "error" => 3,
        "error_text" => "Недопустимый формат файла. Документ не загружен. "
      );
      echo json_encode($respond);
      return;
    }
    // Проверка типа файла.
    if ($_FILES['identdocfile']['size'] > $max_file_size) {
      $respond = array(
        "error" => 3,
        "error_text" => "Слишком большой файл. Справка не загружена."
      );
      echo json_encode($respond);
      return;
    }
    // Сохранение файла.
    $identdoc_file = $docs_dir . '/identdoc' . $_SESSION['uid'];
    $_LoadStatus = 'Loaded';
    if (file_exists($identdoc_file)) {
      $identdoc_file_old = $docs_dir . '/identdoc' . $_SESSION['uid'] . '_' . date('YmdHis');
      rename($identdoc_file, $identdoc_file_old);
      $_LoadStatus = 'Updated';
    } else {
      $identdoc_file_old = '';
    }

    if (move_uploaded_file($_FILES['identdocfile']['tmp_name'], $identdoc_file)) {
      $mysqli->query('update olimp_persons set IdentDoc="' . $_LoadStatus . '" WHERE id=' . $_SESSION['uid']);
      $check_result['IdentDoc'] = $_LoadStatus;
      $respond = array(
        "error" => 0,
        "error_text" => "Справка загружена."
      );
      echo json_encode($respond);
      return;
    } else {
      if ($identdoc_file_old) {
        rename($identdoc_file_old, $identdoc_file);
        $respond = array(
          "error" => 3,
          "error_text" => "Ошибка при загрузке файла. Новая справка не загружена."
        );
        echo json_encode($respond);
        return;
      } else {
        $respond = array(
          "error" => 3,
          "error_text" => "Ошибка при загрузке файла. Справка не загружена."
        );
        echo json_encode($respond);
        return;
      }
    }
  } else {
    $respond = array(
      "error" => 3,
      "error_text" => "Отсутствует загружаемый файл."
    );
    echo json_encode($respond);
  }

}

if (isset($_GET['UploadIdentPhoto'])) { // Загрузка фотографии.
  //$strRespond['UploadSchoolDoc'] = "yes";
  if (is_uploaded_file($_FILES['identphotofile']['tmp_name'])) {
    $FileType = mime_content_type($_FILES['identphotofile']['tmp_name']);
    // Проверка размера файла
    if (!array_key_exists($FileType, $allowed_file_types['IdentPhoto'])) {
      $respond = array(
        "error" => 3,
        "error_text" => "Недопустимый формат файла. Документ не загружен. "
      );
      echo json_encode($respond);
      return;
    }
    // Проверка типа файла.
    if ($_FILES['identphotofile']['size'] > $max_file_size) {
      $respond = array(
        "error" => 3,
        "error_text" => "Слишком большой файл. Справка не загружена."
      );
      echo json_encode($respond);
      return;
    }
    // Сохранение файла.
    $identphoto_file = $docs_dir . '/identphoto' . $_SESSION['uid'];
    $_LoadStatus = 'Loaded';
    if (file_exists($identphoto_file)) {
      $identphoto_file_old = $docs_dir . '/identphoto' . $_SESSION['uid'] . '_' . date('YmdHis');
      rename($identphoto_file, $identphoto_file_old);
      $_LoadStatus = 'Updated';
    } else {
      $identphoto_file_old = '';
    }

    if (move_uploaded_file($_FILES['identphotofile']['tmp_name'], $identphoto_file)) {
      $mysqli->query('update olimp_persons set IdentPhoto="' . $_LoadStatus . '" WHERE id=' . $_SESSION['uid']);
      $check_result['IdentPhoto'] = $_LoadStatus;
      $respond = array(
        "error" => 0,
        "error_text" => "Справка загружена."
      );
      echo json_encode($respond);
      return;
    } else {
      if ($identphoto_file_old) {
        rename($identphoto_file_old, $identphoto_file);
        $respond = array(
          "error" => 3,
          "error_text" => "Ошибка при загрузке файла. Новая справка не загружена."
        );
        echo json_encode($respond);
        return;
      } else {
        $respond = array(
          "error" => 3,
          "error_text" => "Ошибка при загрузке файла. Справка не загружена."
        );
        echo json_encode($respond);
        return;
      }
    }
  } else {
    $respond = array(
      "error" => 3,
      "error_text" => "Отсутствует загружаемый файл."
    );
    echo json_encode($respond);
  }

}

//print_r($_FILES);
// $strRespond['_FILES'] = print_r($_FILES, true); 
// echo json_encode($strRespond);