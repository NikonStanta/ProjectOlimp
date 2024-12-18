<?php
session_start();

$_small_to_big = array(
    "а" => "А", "б" => "Б", "в" => "В", "г" => "Г", "д" => "Д", "е" => "Е","ё" => "Ё", "ж" => "Ж","з" => "З",
    "и" => "И","й" => "Й","к" => "К","л" => "Л","м" => "М","н" => "Н","о" => "О","п" => "П",
    "р" => "Р","с" => "С","т" => "Т","у" => "У","ф" => "Ф","х" => "Х","ц" => "Ц","ч" => "Ч",
    "ш" => "Ш","щ" => "Щ", "ы" => "Ы","э" => "Э","ю" => "Ю","я" => "Я","ь" => "Ь","ъ" => "Ъ");
    
    $_big_to_small = array(
    "А" => "а","Б" => "б", "В" => "в","Г" => "г","Д" => "д","Е" => "е","Ё" => "ё","Ж" => "ж","З" => "з",
    "И" => "и", "Й" => "й","К" => "к","Л" => "л","М" => "м","Н" => "н","О" => "о","П" => "п",
    "Р" => "р","С" => "с", "Т" => "т","У" => "у","Ф" => "ф","Х" => "х","Ц" => "ц","Ч" => "ч",
    "Ш" => "ш","Щ" => "щ", "Ы" => "ы","Э" => "э","Ю" => "ю","Я" => "я", "Ь" => "ь","Ъ" => "ъ");


// Подключение БД на сервере.
$mysqli = new mysqli('localhost', 'p638056_ehope_sql', 'qW6tZ7(N0_hY8eY%wD2q', 'p638056_ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
$mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
$mysqli->query('SET SESSION sql_mode = ""');
// Локальное подключение БД.
// $mysqli = new mysqli('localhost', 'root', 'mysql', 'ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
// $mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
// $mysqli->query('SET SESSION sql_mode = ""');

if (isset($_POST['regStep'])) {
    $regStep = trim($_POST['regStep'], '"');
    $surname = trim($_POST['regSurename'], '"');
    $name = trim($_POST['regName'], '"');
    $patronymic = trim($_POST['regPatronymic'], '"');
    $birthday = trim($_POST['regBirthday'], '"');
    $email = trim($_POST['regEmail'], '"');
    $questionA = trim($_POST['regQuestionA'], '"');
    $questionB = trim($_POST['regQuestionB'], '"');
    $answer = trim($_POST['regAnswer'], '"');
    $hash = trim($_POST['hash'], '"');
    $nic = "";

    $debug = 0;
    $reg_status = array();
    if ($debug == 1) {
        if ($regStep == "1") {
            $reg_status = array(
                "signup_status" => "",
                // "reg_opened" => $reg_opened,
                "regStep" => "2", //$regStep,
                "sname" => $surname,
                "name" => $name,
                "pname" => $patronymic,
                "birthday" => $birthday,
                "email" => $email,
                "questionA" => $questionA,
                "questionB" => $questionB,
                "answer" => $answer,
                "hash" => $hash
            );
        } elseif ($regStep == "2") {
            $nic = trim($_POST['nic'], '"');
            $reg_status = array(
                "signup_status" => "",
                // "reg_opened" => $reg_opened,
                "regStep" => "3", //$regStep,
                "sname" => $surname,
                "name" => $name,
                "pname" => $patronymic,
                "birthday" => $birthday,
                "email" => $email,
                "questionA" => $questionA,
                "questionB" => $questionB,
                "answer" => $answer,
                "hash" => $hash,
                "nic" => $nic,

                "regnum" => "testRegnum",
                "pwd" => "testPwd",
            );
        }
        $reg_status["signup_status"] = $regStep;
        $reg_status["error"] = $regStep;
        $reg_status["nics"][0] = "Var 0";
        $reg_status["nics"][1] = "Var 1";
        $reg_status["nics"][2] = "Var 2";
        $reg_status["nics"][3] = "Var 3";
    }
    $reg_status = registration($mysqli, $reg_status);

    //   switch($reg_status['error']){
//     case "0": // Регистрация закрыта.
//         $data["signup_status"] = "0";
//   }

    echo json_encode($reg_status);


}


function registration($mysqli, $reg_status)
{
    $nicCount = 0;
    //return '<h2>Регистрация участников олимпиады сезона 2015/2016 г.г. будет открыта после 20 октября.</h2>';
    // Получить информацию о статусе регистрации (открыта/закрыта).
    $res = $mysqli->query('select name,value from vars_string');
    while ($row = $res->fetch_row()) {
        $SITE_VARS[$row[0]] = $row[1];
    }
    $build_page_res = $reg_status; // Массив для возвращения результата работы функции.

    $build_page_res["regStep"] = -1; // Значение по умолчанию.
    $build_page_res["error"] = -1; // -1 означ, что ошибки не возникло.
    $build_page_res["error_text"] = "Начальное значение";

    if (!$SITE_VARS['reg_opened']) {
        // $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['closed'];
        $build_page_res["regStep"] = $_POST['regStep'];
        $build_page_res["error"] = 0;
        $build_page_res["error_text"] = "Регистрация закрыта.";
        return $build_page_res;
    }

    $questions = array(
        1 => 'Дата рождения матери',
        2 => 'Дата рождения отца',
        3 => 'Серия и номер паспорта отца',
        4 => 'Серия и номер паспорта матери',
        5 => 'Любимый актер/актриса кино',
        6 => 'Любимый фильм',
        7 => 'Как вас называли в детстве',
        8 => 'Любимое блюдо'
    );

    $build_page_res["error_text"] = "Trace: 0, POST[regStep] = " . $_POST['regStep'];
    if (($_POST['regStep'] > 0) && ($_POST['regStep'] < 3)) {
        // $build_page_res["error_text"] = "Trace: 1, POST[regStep] = " . $_POST['regStep'];
        $regStep = $_POST['regStep'];
        $build_page_res["regStep"] = $regStep;
    } else {
        $regStep = 0;
        $build_page_res["regStep"] = $regStep;
    }

    $sname = ucfirst_r(trim(strip_tags($_POST['regSurename'])));
    $name = ucfirst_r(trim(strip_tags($_POST['regName'])));
    $pname = ucfirst_r(trim(strip_tags($_POST['regPatronymic'])));

    $sname = mb_strtolower($sname, 'UTF-8');
    $name = mb_strtolower($name, 'UTF-8');
    $pname = mb_strtolower($pname, 'UTF-8');

    $sname = mb_strtoupper(mb_substr($sname, 0, 1, 'UTF-8'), 'UTF-8') . mb_substr($sname, 1, null, 'UTF-8');
    $name = mb_strtoupper(mb_substr($name, 0, 1, 'UTF-8'), 'UTF-8') . mb_substr($name, 1, null, 'UTF-8');
    $pname = mb_strtoupper(mb_substr($pname, 0, 1, 'UTF-8'), 'UTF-8') . mb_substr($pname, 1, null, 'UTF-8');

    $birthday = trim(strip_tags($_POST['regBirthday']));
    $email = trim(strip_tags($_POST['regEmail']));
    $question = trim(strip_tags($_POST['regQuestionA']));
    $question_text = trim(strip_tags($_POST['regQuestionB']));
    if (!empty($question_text)) {
        $question = 9;
    }
    $answer = trim($_POST['regAnswer']);

    if (empty($_POST['hash']) || ($_POST['hash'] != $_SESSION['hash'])) {
        $regStep = 0;
        $_POST['regStep'] = 1;
        // FOR DEBUG: (удалить позднее).
        $build_page_res["error"] = 1;
        // $build_page_res["error_text"] = "Trace: hash problem, Не совпадают хэши:<br/>Session: ". $_SESSION['hash'] . "<br/>Post: " . $_POST['hash'];
        $build_page_res["error_text"] = "Для поторной попытки перезагрузите страницу.";
    }

    //30     Проверим введенные данные
    switch ($regStep) { // switch 1.
        case 2: // Если пользователь с указанным nic уже существует, то отправить на 1-ю стадию регистрации.
        {
            $nic = trim($_POST['nic']);
            $build_page_res["error_text"] = "Trace: switch 1 case 2 nic = $nic";
            $res = $mysqli->query('select id from olimp_users where nic="' . $nic . '"');
            if (is_object($res) && $res->num_rows > 0) {
                $regStep = 1;
                $build_page_res["regStep"] = $regStep;
            }
        }
        case 1: {
            $build_page_res["error_text"] = "Trace: switch 1 case 1";
            if (empty($name) || empty($sname)) {
                $error = 'Не введены фамилия и имя участника';
                $regStep = 0;
                $build_page_res["regStep"] = $regStep;
                $build_page_res["error"] = 2;
                $build_page_res["error_text"] = "Не введены фамилия и имя участника.";
                break;
            }
            $pregMatch_check_Symbols = 'абвгдеёжзийклмнопрстуфхцчщшыьъюяэ\- ';
            if (preg_match("/[^$pregMatch_check_Symbols]/", mb_strtolower($sname)) || preg_match("/[^$pregMatch_check_Symbols]/", mb_strtolower($name)) || preg_match("/[^$pregMatch_check_Symbols]/", mb_strtolower($pname))) {
                $error = 'Недопустимые символы в фамилии, имени или отчестве участника';
                $regStep = 0;
                // $build_page_res["error_text"] = "Trace: switch 1 ошибка в данных 1";
                $build_page_res["error"] = 3;
                $build_page_res["error_text"] = "Недопустимые символы в фамилии, имени или отчестве участника.";
                break;
            }
            if (empty($question_text) && (($question > 8) || ($question < 1))) {
                $error = 'Необходимо выбрать вопрос или ввести текст своего вопроса';
                $regStep = 0;
                // $build_page_res["error_text"] = "Trace: switch 1 ошибка в данных 2";
                $build_page_res["error"] = 4;
                $build_page_res["error_text"] = "Необходимо выбрать вопрос или ввести текст своего вопроса.";
                break;
            }
            if (empty($answer)) {
                $error = 'Необходимо ввести текст ответа';
                $regStep = 0;
                // $build_page_res["error_text"] = "Trace: switch 1 ошибка в данных 3";
                $build_page_res["error"] = 5;
                $build_page_res["error_text"] = "Необходимо ввести текст ответа.";
                break;
            }
            if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/', $birthday, $dts)) {
                $error = 'Неверный формат или отсутствует дата рождения';
                $regStep = 0;
                // $build_page_res["error_text"] = "Trace: switch 1 ошибка в данных 4";
                $build_page_res["error"] = 6;
                $build_page_res["error_text"] = "Неверный формат или отсутствует дата рождения.";
                break;
            }
            if ($regStep == 0 && isset($error)) { // Для формирвоания ajax ответа об ошибке.
                $build_page_res["regStep"] = $regStep;
                // $build_page_res["error"] = 1;
                // $build_page_res["error_text"] = $error;
                // $build_page_res["error_text"] = "Trace: switch 1 ошибка в данных";
            }
        }
    }


    // Действия
    // $build_page_res["error_text"] = "Trace: перед switch 2, \$regStep: $regStep";
    switch ($regStep) {
        case 0: {
            $_SESSION['just_reg_nic'] = '';
            $_SESSION['just_reg_pwd'] = '';
            $_SESSION['just_reg_fio'] = '';
            $_SESSION['hash'] = '';
            break;
        }
        case 1:  // Проверка ФИО и Д.Р. //  Генерация логина
        {
            if ($_SESSION['just_reg_fio'] == ($sname . ' ' . $name . ' ' . $pname)) { // Защита от Refresh
                $nic = $_SESSION['just_reg_nic'];
                $pwd = $_SESSION['just_reg_pwd'];
                $regStep = 3;
                $build_page_res["regStep"] = $regStep;
                $build_page_res["error_text"] = "Trace: switch 2 case 1 refresh defend";
                break;
            }
            $build_page_res["error_text"] = "Trace: switch 2 case 1";

            $res = $mysqli->query('select u.nic from olimp_persons as p left join olimp_users as u on (u.id=p.uid) where (p.surename="' . $mysqli->real_escape_string($sname) . '") AND  (p.name="' . $mysqli->real_escape_string($name) . '") AND (p.patronymic="' . $mysqli->real_escape_string($pname) .
                '") AND (p.birthday="' . $dts[3] . '-' . $dts[2] . '-' . $dts[1] . '")');
            if ($row = $res->fetch_row()) {
                $regStep = 4;
                $build_page_res["regStep"] = $regStep;
                $founded_nic = $row[0];
                // $error = str_replace(array('%sname%','%name%','%pname%','%birthday%','%found_nic%'), array($sname,$name,$pname,$birthday,$row[0]), $tpl['reg_user']['already']);
                $_SESSION['just_reg_nic'] = '';
                $_SESSION['just_reg_pwd'] = '';
                $_SESSION['just_reg_fio'] = '';
                // $build_page_res["error_text"] = "Trace: switch 2 пользователь существует";
                break;
            }
            // $build_page_res["error_text"] = "Trace: switch 2 генерация логина";
            //     Сгенерируем логин
            $tsname = ucfirst(strtolower(translit($sname)));
            $tname = strtoupper(translit($name));
            $tpname = strtoupper(translit($pname));
            $nic = $tsname . $tname[0] . $tpname[0];
            $res = $mysqli->query('select id from olimp_users where nic="' . $nic . '"');
            if ($res->num_rows > 0) {
                // nic занят, генерируем варианты

                $nics = array();
                $wnic = $nic . '_' . $birthday;
                $res = $mysqli->query('select id from olimp_users where nic="' . $mysqli->real_escape_string($wnic) . '"');
                if ($res->num_rows < 1) {
                    $nics[] = $wnic;
                }

                $wnic = $nic . $dts[3];
                $res = $mysqli->query('select id from olimp_users where nic="' . $mysqli->real_escape_string($wnic) . '"');
                if ($res->num_rows < 1) {
                    $nicCount++;
                    $nics[$nicCount] = $wnic;
                }
                $wnic = $nic . date('Y');
                $res = $mysqli->query('select id from olimp_users where nic="' . $mysqli->real_escape_string($wnic) . '"');
                if ($res->num_rows < 1) {
                    $nicCount++;
                    $nics[$nicCount] = $wnic;
                }
                $w = 1;
                $wnext = true;
                do {
                    $w++;
                    $wnic = $nic . '_' . $w;
                    $res = $mysqli->query('select id from olimp_users where nic="' . $mysqli->real_escape_string($wnic) . '"');
                    if ($res->num_rows < 1) {
                        $nicCount++;
                        $nics[$nicCount] = $wnic;
                        $wnext = false;
                    }
                } while ($wnext);
                $regStep = 2;
                // Для формирования ajax ответа.
                $build_page_res["regStep"] = $regStep;
                $build_page_res["nics"] = $nics;
                break;
            }
        }

        case 2:  // Регистрация
        {
            if ($_SESSION['just_reg_fio'] == ($sname . ' ' . $name . ' ' . $pname)) { // Защита от Refresh
                $nic = $_SESSION['just_reg_nic'];
                $pwd = $_SESSION['just_reg_pwd'];
                $regStep = 3;
                $build_page_res["regStep"] = $regStep;
                break;
            }
            // if ($_SESSION['debug'] > 0) {
            //     print_r($_SESSION);
            // }
            $pwd = gen_pwd(8);
            $ok = false;
            $uid = -1;
            $pid = -1;
            if ($question <> 9) {
                $question_text = $questions[$question];
            }
            if ($mysqli->query('insert into olimp_users set nic="' . $mysqli->real_escape_string($nic) . '", pwd=UNHEX(SHA("' . $pwd . '")), question="' . $mysqli->real_escape_string($question_text) . '", answer="' . $mysqli->real_escape_string($answer) . '", dt=Now(), role=1')) {
                $uid = $mysqli->insert_id;
                // $build_page_res["error_text"] = "Trace: switch 3 case 2 после uid insert_id $uid, pwd 3:{$dts[3]}, 2: {$dts[2]}, 1: {$dts[1]}";
                if (
                    $mysqli->query('insert into olimp_persons set surename="' . $mysqli->real_escape_string($sname) . '", name="' . $mysqli->real_escape_string($name) . '", patronymic="' . $mysqli->real_escape_string($pname) .
                        '", birthday="' . $dts[3] . '-' . $dts[2] . '-' . $dts[1] . '", uid=' . $uid . ', email="' . $email . '"')
                ) {
                    $pid = $mysqli->insert_id;
                    $build_page_res["error_text"] = "Trace: switch 3 case 2 после pid insert_id: $pid, pwd 3:{$dts[3]}, 2: {$dts[2]}, 1: {$dts[1]}";
                    // Получим регистрационный номер
                    if ($pid < 1) {
                        break;
                    }
                    if ($mysqli->query('insert into _reg_nums (reg_num) select max(reg_num)+1 from _reg_nums')) {
                        $_regnum = $mysqli->insert_id;
                        $regnum = $_regnum . '-' . $SITE_VARS['reg_num_postfix'];

                        $build_page_res['regnum'] = $regnum;

                        if ($mysqli->query('update olimp_persons set reg_num="' . $regnum . '" where id=' . $pid)) {
                            $ok = true;
                            $mysqli->query('update _reg_nums set uid=' . $pid . ' where reg_num=' . $_regnum);
                        }
                    }
                }
            }
            if ($ok) {
                $regStep = 3;
                $build_page_res['regStep'] = 3;
                $_SESSION['just_reg_nic'] = $nic;
                $_SESSION['just_reg_pwd'] = $pwd;
                $_SESSION['just_reg_fio'] = $sname . ' ' . $name . ' ' . $pname;
            } else {
                $error = "Непредвиденная ошибка при регистрации пользователя (Сервис временно недоступен), попробуйте попытку позже $uid,$pid";
                if ($uid > -1) {
                    $mysqli->query('delete from olimp_users where id=' . $uid);
                }
                if ($pid > -1) {
                    $mysqli->query('delete from olimp_persons where id=' . $pid);
                }
                $regStep = 1;
                $_SESSION['just_reg_nic'] = '';
                $_SESSION['just_reg_pwd'] = '';
                $_SESSION['just_reg_fio'] = '';
                $_SESSION['hash'] = '';

                $build_page_res['regStep'] = $regStep;
                $build_page_res['error'] = 1;
                $build_page_res['error_text'] .= $error;
            }
        }
    }

    // Вывод

    switch ($regStep) { // switch 3.
        case 2: //  Выбор логина (из списка, в случае, если сгенерированный логин занят).
            // {    $nic_list = '';
            //     if (!empty($error)) {
            //         $error = preg_replace('/%error%/', $error, $tpl['reg_user']['error']);
            //     }
            //     foreach ($nics as $w) {
            //         if (empty($nic_list)) {
            //             $nic_list .= preg_replace('/%nic%/', $w, $tpl['reg_user']['nic1']);
            //         } else {
            //             $nic_list .= preg_replace('/%nic%/', $w, $tpl['reg_user']['nic']);
            //         }
            //     }

            //     $body = preg_replace(
            //         array('/%regSurename%/','/%regName%/','/%regPatronymic%/','/%regBirthday%/','/%regEmail%/','/%regQuestionA%/','/%regQuestionB%/','/%regAnswer%/','/%nic_list%/','/%hash%/'),
            //         array($sname,$name,$pname,$birthday,$email,$question,$question_text,$answer,$nic_list,$_SESSION['hash']),
            //         $tpl['reg_user']['step2']
            //     );
            //         $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['box2'];
            $build_page_res['sname'] = $sname;
            $build_page_res['name'] = $name;
            $build_page_res['pname'] = $pname;
            $build_page_res['nic'] = $nic;
            $build_page_res['birthday'] = $birthday;
            $build_page_res['email'] = $email;
            $build_page_res['question'] = $question;
            $build_page_res['questionA'] = $question;
            $build_page_res['questionB'] = $question_text;
            $build_page_res['question_text'] = $question_text;
            $build_page_res['answer'] = $answer;
            $build_page_res['nic_list'] = $nics;
            $build_page_res['hash'] = $_SESSION['hash'];

            $build_page_res["error_text"] = "Trace: switch 3 case 2";
            // $build_page_res['pwd'] = $pwd;
            // $build_page_res['regnum'] = $regnum;
            break;
        //}
        case 3: //  Результат регистрации
            // {    $body = preg_replace(
            //     array('/%regSurename%/','/%regName%/','/%regPatronymic%/','/%nic%/','/%pwd%/','/%regnum%/'),
            //     array($sname,$name,$pname,$nic,$pwd,$regnum),
            //     $tpl['reg_user']['step3']
            //     );
            //         $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['box3'];
            $build_page_res['sname'] = $sname;
            $build_page_res['name'] = $name;
            $build_page_res['pname'] = $pname;
            $build_page_res['nic'] = $nic;
            $build_page_res['pwd'] = $pwd;
            $build_page_res['regnum'] = $regnum;
            break;
        // }
        default: // Форма регистрации
        // if(!empty($error)){

        // }
        // {    if (!empty($error)) {
        //         $error = preg_replace('/%error%/', preg_replace(array('/%title%/','/%text%/'), array('Ошибка!',$error), $tpl['common']['error']), $tpl['reg_user']['error']);
        // }
        // $questions_list = '';
        // foreach ($questions as $k => $w) {
        //     $questions_list .= '<option value=' . $k . ($k == $question ? ' selected' : '') . '>' . $w . '</option>';
        // }
        // $_SESSION['hash'] = md5(microtime());
        // $body = preg_replace(
        //     array('/%regSurename%/','/%regName%/','/%regPatronymic%/','/%regBirthday%/','/%questions_list%/','/%regQuestionB%/','/%regAnswer%/','/%regEmail%/','/%error%/','/%hash%/'),
        //     array($sname,$name,$pname,$birthday,$questions_list,$question_text,$answer,$email,$error,$_SESSION['hash']),
        //     $tpl['reg_user']['step1']
        //     );
        //         $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['box1'];
        // }
    }

    return $build_page_res;

}


function gen_pwd($number)
{
    $letters = array(
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'r',
        's',
        't',
        'u',
        'v',
        'x',
        'y',
        'z',
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'r',
        's',
        't',
        'u',
        'v',
        'x',
        'y',
        'z',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P',
        'R',
        'S',
        'T',
        'U',
        'V',
        'X',
        'Y',
        'Z',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
        'G',
        'H',
        'I',
        'J',
        'K',
        'L',
        'M',
        'N',
        'O',
        'P',
        'R',
        'S',
        'T',
        'U',
        'V',
        'X',
        'Y',
        'Z',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '.',
        '_',
        '-',
        '.',
        '_',
        '-'
    );
    shuffle($letters);
    $pass = $letters[0];
    for ($i = 1; $i < $number; $i++) {
        $pass .= $letters[$i];
    }
    return $pass;
}

function translit($text)
{
    $trans = array(
        "а" => "a",
        "б" => "b",
        "в" => "v",
        "г" => "g",
        "д" => "d",
        "е" => "e",
        "ё" => "e",
        "ж" => "zh",
        "з" => "z",
        "и" => "i",
        "й" => "y",
        "к" => "k",
        "л" => "l",
        "м" => "m",
        "н" => "n",
        "о" => "o",
        "п" => "p",
        "р" => "r",
        "с" => "s",
        "т" => "t",
        "у" => "u",
        "ф" => "f",
        "х" => "kh",
        "ц" => "ts",
        "ч" => "ch",
        "ш" => "sh",
        "щ" => "shch",
        "ы" => "y",
        "э" => "e",
        "ю" => "yu",
        "я" => "ya",
        "А" => "A",
        "Б" => "B",
        "В" => "V",
        "Г" => "G",
        "Д" => "D",
        "Е" => "E",
        "Ё" => "E",
        "Ж" => "Zh",
        "З" => "Z",
        "И" => "I",
        "Й" => "Y",
        "К" => "K",
        "Л" => "L",
        "М" => "M",
        "Н" => "N",
        "О" => "O",
        "П" => "P",
        "Р" => "R",
        "С" => "S",
        "Т" => "T",
        "У" => "U",
        "Ф" => "F",
        "Х" => "Kh",
        "Ц" => "Ts",
        "Ч" => "Ch",
        "Ш" => "Sh",
        "Щ" => "Shch",
        "Ы" => "Y",
        "Э" => "E",
        "Ю" => "Yu",
        "Я" => "Ya",
        "ь" => "",
        "Ь" => "",
        "ъ" => "",
        "Ъ" => ""
    );
    if (preg_match("/[а-яА-Я]/", $text)) {
        return strtr($text, $trans);
    } else {
        return $text;
    }
}

$_small_to_big = array(
    "а" => "А",
    "б" => "Б",
    "в" => "В",
    "г" => "Г",
    "д" => "Д",
    "е" => "Е",
    "ё" => "Ё",
    "ж" => "Ж",
    "з" => "З",
    "и" => "И",
    "й" => "Й",
    "к" => "К",
    "л" => "Л",
    "м" => "М",
    "н" => "Н",
    "о" => "О",
    "п" => "П",
    "р" => "Р",
    "с" => "С",
    "т" => "Т",
    "у" => "У",
    "ф" => "Ф",
    "х" => "Х",
    "ц" => "Ц",
    "ч" => "Ч",
    "ш" => "Ш",
    "щ" => "Щ",
    "ы" => "Ы",
    "э" => "Э",
    "ю" => "Ю",
    "я" => "Я",
    "ь" => "Ь",
    "ъ" => "Ъ"
);

$_big_to_small = array(
    "А" => "а",
    "Б" => "б",
    "В" => "в",
    "Г" => "г",
    "Д" => "д",
    "Е" => "е",
    "Ё" => "ё",
    "Ж" => "ж",
    "З" => "з",
    "И" => "и",
    "Й" => "й",
    "К" => "к",
    "Л" => "л",
    "М" => "м",
    "Н" => "н",
    "О" => "о",
    "П" => "п",
    "Р" => "р",
    "С" => "с",
    "Т" => "т",
    "У" => "у",
    "Ф" => "ф",
    "Х" => "х",
    "Ц" => "ц",
    "Ч" => "ч",
    "Ш" => "ш",
    "Щ" => "щ",
    "Ы" => "ы",
    "Э" => "э",
    "Ю" => "ю",
    "Я" => "я",
    "Ь" => "ь",
    "Ъ" => "ъ"
);


function ucfirst_r($text)
{
    global $_small_to_big, $_big_to_small;
    if (strlen($text) == 0) {
        return '';
    }
    $w = strtr($text, $_big_to_small);
    $w[0] = strtr($w[0], $_small_to_big);
    return $w . '';
}

function strtolower_r($text)
{
    global $_big_to_small;
    if (strlen($text) == 0) {
        return '';
    }
    return strtr($text, $_big_to_small) . '';
}

function strtoupper_r($text)
{
    global $_small_to_big;
    if (strlen($text) == 0) {
        return '';
    }
    return strtr($text, $_small_to_big) . '';
}
