<?php
//$_connection_charset='utf8';
include_once('../../ainc/connect_db.inc');
include_once('../../ainc/sess.inc');

//header('Content-Type: application/json');

if ($role < 3) {
    die('{"status":"noaccess"}');
}

if (empty($_REQUEST['IdPerson'])) {
    die('<tr><td>Персона не задана</td></tr>');
}
$IdPerson = (int)$_REQUEST['IdPerson'];
$exam = (int)$_REQUEST['IdExam'];
$action = $_REQUEST['Action'];

switch ($action) {
    case 'accept':
    {
        $score = (int)$_REQUEST['Score'];

        $res = $mysqli->query(
            'SELECT s.id, e.name, s.classes, s.date_exam as date, p.name as place 
         FROM olimp_stages as s LEFT JOIN olimp_exams as e on s.exam = e.id LEFT JOIN olimp_places as p on p.id = s.place
         WHERE s.stage=0 AND s.year='. date('Y') . ' AND s.exam= ' . $exam
        );
        $stages = "<select id='stage' name='stage' required style='width: 90%;'>";
        while ($row = $res->fetch_assoc()) {
            $idd = $row['id']; $examm = $row['name']; $classs = $row['classes']; $place = $row['place']; $date = $row['date'];
            $stages .= "<option value='$idd'>$date $place - $examm $classs класс</option>";
        }
        if ($stages == "<select id='stage' name='stage' required style='width: 90%;'>") {
            $stages = "<span>Подходящих потоков не найдено.</span>";
        } else {
            $stages .= '</select>';
        }

        $rank = array(1 => 'участник',2 => 'призер 3',3 => 'призер 2',4 => 'победитель');
        $ranks = "<select id='rank' name='rank' required style='width: 90%;'>";
        foreach ($rank as $v => $item) {
            $ranks .= "<option value='$v'>$item</option>";
        }
        $ranks .= "</select>";

        $list="
        <tr>
        <td><input id='score' type='number' min='0' max='100' name='score' value='$score' class='w100 input-text qty text' placeholder='Результат в баллах от 0 до 100' required style='width: 90%;'></td>
        <td style='width: 220px; padding-top:2px;'>Балл участника</td>
        </tr>
        <tr>
        <td>$ranks</td>
        <td style='width: 220px; padding-top:2px;'>Степень</td>
        </tr>
        <tr>
        <td>$stages</td>
        <td style='width: 220px; padding-top:2px;'>Поток</td>
        </tr>
        ";
        break;
    }
    case 'reject':
    {
        $fill_in_text = 'Текст по умолчанию (настравивается в olimp_confirm_reculc_dialog.php)';
        $list="<tr>" .
            "<td><textarea id='commentInput' rows='4' style='width: 100%;' name='refuse_comment' placeholder='Комментарий при отказе'>$fill_in_text</textarea></td>" .
            "<td style=\'width: 220px; padding-top:2px;\'>Комментарий об отказе участнику</td>" .
            "</tr>";
        break;
    }
}

echo $list;
