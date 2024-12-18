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

$list='AAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAA';
echo $list;
$_doc_folder='../../docs/applications/';

$application='';
$schooldoc='';
$identdoc='';
$identphoto='';

$allFiles=scandir($_doc_folder);
foreach ($allFiles as $file) {
    if (preg_match('/([^0-9]*)([0-9]*)(\_([0-9]*))*/', $file, $parts)) {
        if ($parts[2]==$IdPerson) {
            switch ($parts[1]) {
                case 'application':
                    if (empty($parts[4])) {
                        $application.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=application&docOther=" target="_blank">Последняя версия</a><br>';
                    } else {
                        $application.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=application&docOther='.$parts[4].'" target="_blank">версия '.$parts[4].'</a><br>';
                    }
                    break;
                case 'schooldoc':
                    if (empty($parts[4])) {
                        $schooldoc.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=schooldoc&docOther=" target="_blank">Последняя версия</a><br>';
                    } else {
                        $schooldoc.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=schooldoc&docOther='.$parts[4].'" target="_blank">версия '.$parts[4].'</a><br>';
                    }
                    break;
                case 'identdoc':
                    if (empty($parts[4])) {
                        $identdoc.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=identdoc&docOther=" target="_blank">Последняя версия</a><br>';
                    } else {
                        $identdoc.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=identdoc&docOther='.$parts[4].'" target="_blank">версия '.$parts[4].'</a><br>';
                    }
                    break;
                case 'identphoto':
                    if (empty($parts[4])) {
                        $identphoto.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=identphoto&docOther=" target="_blank">Последняя версия</a><br>';
                    } else {
                        $identphoto.='<a href="/ajax/olimp_confirm_person_view_doc.php?IdPerson='.$IdPerson.'&docType=identphoto&docOther='.$parts[4].'" target="_blank">версия '.$parts[4].'</a><br>';
                    }
                    break;
            }
        }
    }
}

if ($application) {
    $list.='<tr><td>Заявление</td><td>
<input type=radio name="application" value="Skip" id="applicationS" checked><label for="applicationS"> Не изменять статус</label><br>
<input type=radio name="application" value="A" id="applicationA"><label for="applicationA"> Принять документ</label><br>
<input type=radio name="application" value="R" id="applicationR"><label for="applicationR"> Отклонить документ</label><br>
<input type=radio name="application" value="L" id="applicationL"><label for="applicationL"> Сбросить статус (L)</label><br>
</td><td>'.$application.'</td></tr>';
} else {
    $list.='<tr><td colspan=3>Заявление не предоставлено</td></tr>';
}

if ($schooldoc) {
    $list.='<tr><td>Справка из школы</td><td>
<input type=radio name="schooldoc" value="Skip" id="schooldocS" checked><label for="schooldocS"> Не изменять статус</label><br>
<input type=radio name="schooldoc" value="Accepted" id="schooldocA"><label for="schooldocA"> Принять документ</label><br>
<input type=radio name="schooldoc" value="Rejected" id="schooldocR"><label for="schooldocR"> Отклонить документ</label><br>
<input type=radio name="schooldoc" value="Loaded" id="schooldocL"><label for="schooldocL"> Сбросить статус (Loaded)</label><br>
</td><td>'.$schooldoc.'</td></tr>';
} else {
    $list.='<tr><td colspan=3>Справка из школы не представлена</td></tr>';
}

if ($identdoc) {
    $list.='<tr><td>Документ, удостоверяющий личность</td><td>
<input type=radio name="identdoc" value="Skip" id="identdocS" checked><label for="identdocS"> Не изменять статус</label><br>
<input type=radio name="identdoc" value="Accepted" id="identdocA"><label for="identdocA"> Принять документ</label><br>
<input type=radio name="identdoc" value="Rejected" id="identdocR"><label for="identdocR"> Отклонить документ</label><br>
<input type=radio name="identdoc" value="Loaded" id="identdocL"><label for="identdocL"> Сбросить статус (Loaded)</label><br>
</td><td>'.$identdoc.'</td></tr>';
} else {
    $list.='<tr><td colspan=3>Документ, удостоверяющий личность, не представлен</td></tr>';
}

if ($identphoto) {
    $list.='<tr><td>Фотография, удостоверяющая личность</td><td>
<input type=radio name="identphoto" value="Skip" id="identphotoS" checked><label for="identphotoS"> Не изменять статус</label><br>
<input type=radio name="identphoto" value="Accepted" id="identphotoA"><label for="identphotoA"> Принять документ</label><br>
<input type=radio name="identphoto" value="Rejected" id="identphotoR"><label for="identphotoR"> Отклонить документ</label><br>
<input type=radio name="identphoto" value="Loaded" id="identphotoL"><label for="identphotoL"> Сбросить статус (Loaded)</label><br>
</td><td>'.$identphoto.'</td></tr>';
} else {
    $list.='<tr><td colspan=3>Фотография не представлена</td></tr>';
}

echo $list;
