<?php

// Подтверждение данных пользователя
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu;

    if (($role & 8) == 0) {
        return $tpl['noaccess'];
    }


    $person_id = $_POST['person_id'] + 0;

    if (isset($_REQUEST['btnCheckPerson'])) {
        $cmd = ($person_id == 0 ? 'SelectPerson' : 'CheckPerson');
    }

    if (isset($_REQUEST['btnConfirmPerson'])) {
        $cmd = ($person_id == 0 ? 'SelectPerson' : 'doConfirmPerson');
    }


    $sname = trim($_POST['sname']);
    $name = trim($_POST['name']);
    $pname = trim($_POST['pname']);
    $regnum = trim($_POST['regnum']);
    $person = $_POST['person'] + 0;
    $pass = $_POST['pass'] + 0;
    $school = $_POST['school'] + 0;




// Выполнение команд

    switch ($cmd) {
        case 'doConfirmPerson':
        {    if (!empty($person_id)) {
                $confirmation = '';
                if ($person > 0) {
                    $confirmation = 'confirm_person="Y"';
                }
                if ($pass > 0) {
                    $confirmation = 'confirm_pass="Y"';
                }
                if ($school > 0) {
                    $confirmation = 'confirm_school="Y"';
                }
                if ($confirmation) {
                    if ($mysqli->query('update olimp_persons set confirm_person="Y" where id=' . $person_id)) {
                        $message = 'Данные подтверждены';
                    } else {
                        $message = 'Данные не подтверждены!';
                    }
                } else {
                    $message = 'Данные не подтверждены!';
                }
                $cmd = '';
        }
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'SelectPerson':
        {    $where = '';
            $person_id = 0;
            if (!empty($sname)) {
                $where = '(surename like"' . $sname . '%")';
            }
            if (!empty($name)) {
                $where = ($where ? ' AND ' : '') . '(name like"' . $name . '%")';
            }
            if (!empty($pname)) {
                $where = ($where ? ' AND ' : '') . '(patronymic like"' . $pname . '%")';
            }
            if (!empty($regnum)) {
                $where = ($where ? ' AND ' : '') . '(reg_num="' . $regnum . '")';
            }

            if (!empty($where)) {
                $res = $mysqli->query('select id,concat_ws(" ",reg_num,surename,name,patronymic),DATE_FORMAT(birthday,"%d.%m.%Y") as birthday from olimp_persons where ' . $where);
                $c = mysql_num_rows($res);
                if ($c == 1) {
                    $row = $res->fetch_row();
                    $person_id = $row[0];
                } elseif ($c > 1) {
                    $lines = '';
                    while ($row = $res->fetch_row()) {
                        $lines .= str_replace(array('%person_id%','%person_data%'), array($row[0],$row[1] . ' (д.р. ' . $row[2] . ')'), $tpl['confirm_person']['form2_line']);
                    }
                    $body = str_replace(
                        array('%person%','%pass%','%school%','%lines%'),
                        array(($_POST['person'] + 0 ? ' checked' : ''),($_POST['pass'] + 0 ? ' checked' : ''),($_POST['school'] + 0 ? ' checked' : ''),$lines),
                        $tpl['confirm_person']['form2']
                    );

                    break;
                }
            }
        }
        case 'CheckPerson':
        {    if ($person_id > 0) {
                $person = GetPersonData($person_id);
                print_r($person);
                $body = '';
                break;
        }
        }
        default:
        {    //
            $body = $tpl['confirm_person']['form1'];
        }
    }


    return str_replace('%error%', $error, $body);
}

$tpl['confirm_person']['form1'] =
'<h1>Подтверждение данных участников олимпиады</h1>%error%
<form action="#" method=post>
<table cellspacing=0 cellpadding=4 border=0>
<tr><td>Фамилия:</td><td><input name="sname" type="text"></td></tr>
<tr><td>Имя:</td><td><input name="name" type="text"></td></tr>
<tr><td>Отчество:</td><td><input name="pname" type="text"></td></tr>
<tr><td>Рег. номер:</td><td><input name="regnum" type="text"></td></tr>
</table>
<h2>Что представлено:</h2>
<input type="checkbox" id="person" name="person" value=1> <label for="person">Заявление.</label><br>
<input type="checkbox" id="pass" name="pass" value=1> <label for="pass">Документ удостоверяющий личность и гражданство.</label><br>
<input type="checkbox" id="school" name="school" value=1> <label for="school">Справка из школы.</label><br>
<br><br>
<input type=submit name="btnCheckPerson" value="Найти участника">
</form>
<br><br><br><br>
';

$tpl['confirm_person']['form2'] =
'<h1>Выберите участника олимпиады</h1>%error%
<form action="#" method=post>
<br><br>
<input type="hidden" name="person" value=1 %person%>
<input type="hidden" name="pass" value=1 %pass%>
<input type="hidden" name="school" %school%>
%lines%
<br><br>
<input type=submit name="btnCheckPerson" value="Выбрать участника">&nbsp;&nbsp;&nbsp;&nbsp;<input type=submit name="btnCancel" value="Назад">
</form>
<br><br><br><br>
';

$tpl['confirm_person']['form2_line'] =
'<input type="radio" name="person_id" value="%person_id%" id="person_id%person_id%"> <label for="person_id%person_id%">%person_data%</label><br>';




$body = build_page();
