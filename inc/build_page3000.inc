<?php

// Построение страницы регистрации участника

include_once('../inc/common.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl;

//return '<h2>Регистрация участников олимпиады сезона 2015/2016 г.г. будет открыта после 20 октября.</h2>';

    if (!$SITE_VARS['reg_opened']) {
        $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['closed'];
         return '...';
    }

    $questions = array(
    1 => 'Дата рождения матери',
    2 => 'Дата рождения отца',
    3 => 'Серия и номер паспорта отца',
    4 => 'Серия и номер паспорта матери',
    5 => 'Любимый актер/актриса кино',
    6 => 'Любимый фильм',
    7 => 'Как вас называли в детстве',
    8 => 'Любимое блюдо');

    if (($_POST['regStep'] > 0) && ($_POST['regStep'] < 3)) {
        $regStep = $_POST['regStep'];
    } else {
        $regStep = 0;
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
    }

//30     Проверим введенные данные
    switch ($regStep) {
        case 2:
        {    $nic = trim($_POST['nic']);
            $res = $mysqli->query('select id from olimp_users where nic="' . $nic . '"');
            if ($res->num_rows > 0) {
                $regStep = 1;
            }
        }
        case 1:
        {
            if (empty($name) || empty($sname)) {
                $error = 'Не введены фамилия и имя участника';
                $regStep = 0;
                break;
            }

            $pregMatch_check_Symbols = 'абвгдеёжзийклмнопрстуфхцчщшыьъюяэ\- ';
            if (preg_match("/[^$pregMatch_check_Symbols]/", mb_strtolower($sname)) || preg_match("/[^$pregMatch_check_Symbols]/", mb_strtolower($name)) || preg_match("/[^$pregMatch_check_Symbols]/", mb_strtolower($pname))) {
                $error = 'Недопустимые символы в фамилии, имени или отчестве участника';
                $regStep = 0;
                break;
            }

            if (empty($question_text) && (($question > 8) || ($question < 1))) {
                $error = 'Необходимо выбрать вопрос или ввести текст своего вопроса';
                $regStep = 0;
                break;
            }
            if (empty($answer)) {
                $error = 'Необходимо ввести текст ответа';
                $regStep = 0;
                break;
            }
            if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/', $birthday, $dts)) {
                $error = 'Неверный формат или отсутствует дата рождения';
                $regStep = 0;
                break;
            }
        }
    }


// Действия

    switch ($regStep) {
        case 0:
        {    $_SESSION['just_reg_nic'] = '';
            $_SESSION['just_reg_pwd'] = '';
            $_SESSION['just_reg_fio'] = '';
            $_SESSION['hash'] = '';
            break;
        }
        case 1:  // Проверка ФИО и Д.Р. /  Генерация логина
        {    if ($_SESSION['just_reg_fio'] == ($sname . ' ' . $name . ' ' . $pname)) { // Защита от Refresh
                $nic = $_SESSION['just_reg_nic'];
                $pwd = $_SESSION['just_reg_pwd'];
                $regStep = 3;
                break;
        }

        $res = $mysqli->query('select u.nic from olimp_persons as p left join olimp_users as u on (u.id=p.uid) where (p.surename="' . $mysqli->real_escape_string($sname) . '") AND  (p.name="' . $mysqli->real_escape_string($name) . '") AND (p.patronymic="' . $mysqli->real_escape_string($pname) .
                    '") AND (p.birthday="' . $dts[3] . '-' . $dts[2] . '-' . $dts[1] . '")');
            if ($row = $res->fetch_row()) {
                $regStep = 4;
                $founded_nic = $row[0];
                $error = str_replace(array('%sname%','%name%','%pname%','%birthday%','%found_nic%'), array($sname,$name,$pname,$birthday,$row[0]), $tpl['reg_user']['already']);
                $_SESSION['just_reg_nic'] = '';
                $_SESSION['just_reg_pwd'] = '';
                $_SESSION['just_reg_fio'] = '';
                break;
            }


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
                break;
            }
        }

        case 2:  // Регистрация
        {    if ($_SESSION['just_reg_fio'] == ($sname . ' ' . $name . ' ' . $pname)) { // Защита от Refresh
                $nic = $_SESSION['just_reg_nic'];
                $pwd = $_SESSION['just_reg_pwd'];
                $regStep = 3;
                break;
        }
        if ($_SESSION['debug'] > 0) {
            print_r($_SESSION);
        }
        $pwd = gen_pwd(8);
        $ok = false;
        $uid = -1;
        $pid = -1;
        if ($question <> 9) {
            $question_text = $questions[$question];
        }
        if ($mysqli->query('insert into olimp_users set nic="' . $mysqli->real_escape_string($nic) . '", pwd=UNHEX(SHA("' . $pwd . '")), question="' . $mysqli->real_escape_string($question_text) . '", answer="' . $mysqli->real_escape_string($answer) . '", dt=Now(), role=1')) {
            $uid = $mysqli->insert_id;

            if (
                $mysqli->query('insert into olimp_persons set surename="' . $mysqli->real_escape_string($sname) . '", name="' . $mysqli->real_escape_string($name) . '", patronymic="' . $mysqli->real_escape_string($pname) .
                '", birthday="' . $dts[3] . '-' . $dts[2] . '-' . $dts[1] . '", uid=' . $uid . ', email="' . $email . '"')
            ) {
                $pid = $mysqli->insert_id;
                // Получим регистрационный номер
                if ($pid < 1) {
                    break;
                }
                if ($mysqli->query('insert into _reg_nums (reg_num) select max(reg_num)+1 from _reg_nums')) {
                    $_regnum = $mysqli->insert_id;
                    $regnum = $_regnum . '-' . $SITE_VARS['reg_num_postfix'];
                    if ($mysqli->query('update olimp_persons set reg_num="' . $regnum . '" where id=' . $pid)) {
                        $ok = true;
                        $mysqli->query('update _reg_nums set uid=' . $pid . ' where reg_num=' . $_regnum);
                    }
                }
            }
        }
        if ($ok) {
            $regStep = 3;
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
        }
        }
    }

// Вывод

    switch ($regStep) {
        case 2: //  Выбор логина
        {    $nic_list = '';
            if (!empty($error)) {
                $error = preg_replace('/%error%/', $error, $tpl['reg_user']['error']);
            }
            foreach ($nics as $w) {
                if (empty($nic_list)) {
                    $nic_list .= preg_replace('/%nic%/', $w, $tpl['reg_user']['nic1']);
                } else {
                    $nic_list .= preg_replace('/%nic%/', $w, $tpl['reg_user']['nic']);
                }
            }

            $body = preg_replace(
                array('/%regSurename%/','/%regName%/','/%regPatronymic%/','/%regBirthday%/','/%regEmail%/','/%regQuestionA%/','/%regQuestionB%/','/%regAnswer%/','/%nic_list%/','/%hash%/'),
                array($sname,$name,$pname,$birthday,$email,$question,$question_text,$answer,$nic_list,$_SESSION['hash']),
                $tpl['reg_user']['step2']
            );
                $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['box2'];
                            break;
        }
        case 3: //  Результат регистрации
        {    $body = preg_replace(
            array('/%regSurename%/','/%regName%/','/%regPatronymic%/','/%nic%/','/%pwd%/','/%regnum%/'),
            array($sname,$name,$pname,$nic,$pwd,$regnum),
            $tpl['reg_user']['step3']
            );
                $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['box3'];
                break;
        }
        default: // Форма регистрации
        {    if (!empty($error)) {
                $error = preg_replace('/%error%/', preg_replace(array('/%title%/','/%text%/'), array('Ошибка!',$error), $tpl['common']['error']), $tpl['reg_user']['error']);
        }
        $questions_list = '';
        foreach ($questions as $k => $w) {
            $questions_list .= '<option value=' . $k . ($k == $question ? ' selected' : '') . '>' . $w . '</option>';
        }
        $_SESSION['hash'] = md5(microtime());
        $body = preg_replace(
            array('/%regSurename%/','/%regName%/','/%regPatronymic%/','/%regBirthday%/','/%questions_list%/','/%regQuestionB%/','/%regAnswer%/','/%regEmail%/','/%error%/','/%hash%/'),
            array($sname,$name,$pname,$birthday,$questions_list,$question_text,$answer,$email,$error,$_SESSION['hash']),
            $tpl['reg_user']['step1']
            );
                $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX'] = $tpl['reg_user']['box1'];
        }
    }

    return $body;
}
