<?php

// Администрирование сайта - Пользователи
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$user;

    $roles = array(2 => 'Оператор',4 => 'Администратор');

    if ($role < 4) {
        return $tpl['noaccess'];
    }

    $cmd = $_POST['cmd'];
    if ($_POST['btnGoAdd'] == " Создать ") {
        $cmd = 'doAdd';
    }
    $sec = $_POST['sec'];
    if (empty($sec)) {
        $cmd = '';
    } elseif (($sec != 937281) && ($sec != 782318) && ($sec != 537882)) {
        return $tpl['noaccess'];
    }

    $fn = $_POST['fn'];
    $txt = $_POST['txt'];

    $fname = ($sec == 937281 ? "../inc/build_page" : ($sec == 782318 ? "../ainc/" : "../www/css/" )) . $fn . ($sec != 537882 ? '.inc' : '.css');

    $tpl['fed']['main'] =
    '<h1>Редактирование файла</h1>
<form method=post>
<input type=hidden name=cmd value="Load">
<label for=sec1><input type=radio name=sec id=sec1 value=937281> Движок сайта</label><br>
<label for=sec2><input type=radio name=sec id=sec2 value=782318> Администратор сайта</label><br>
<label for=sec3><input type=radio name=sec id=sec3 value=537882> Стилевой файл</label><br><br>
Файл: <input type=text name=fn size=32><br><br>
<input type=submit name=btnGoFed value=" Редактировать ">&nbsp;&nbsp;<input type=submit name=btnGoAdd value=" Создать ">
</form>
';

    $tpl['fed']['edit'] =
    '<h1>Редактирование файла</h1>
<form method=post>
<input type=hidden name=sec value=%sec%>
<input type=hidden name=fn value=%fn%>
<input type=hidden name=cmd value="doEdit">
<textarea cols="40" rows="32" name="txt" style="width:100%">%txt%</textarea>
<input type=submit name=btnGoFed value=" Принять ">
</form>
';

// Выполнение команд

    switch ($cmd) {
        case 'doEdit':
        {    file_put_contents($fname, stripslashes($txt));
            $cmd = '';
            break;
        }
        case 'doAdd':
        {    if (!file_exists($fname)) {
                $f = fopen($fname, "w");
                fclose($f);
                chmod($fname, 0774);
        }
            $cmd = 'Load';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Load':
        {    if (file_exists($fname)) {
                $txt = file_get_contents($fname);
                $body = preg_replace(
                    array('/%sec%/','/%fn%/','/%txt%/'),
                    array($sec,$fn,htmlspecialchars($txt)),
                    $tpl['fed']['edit']
                );
        } else {
            $body = $tpl['fed']['main'];
        }
        break;

        }
        default:
        {    $body = $tpl['fed']['main'];
        }
    }
    return $body;
}

$body = build_page();
