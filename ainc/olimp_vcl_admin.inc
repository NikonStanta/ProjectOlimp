<?php

// Проведение олимпиады с ВКС
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli,$error,$message;

    if (($role & 64) == 0) {
        return $tpl['noaccess'];
    }

//$user_places=getUserPlaces($uid);

//if (empty($user_places['Access']))
//{    return $tpl['noaccess'];
//}

    $presence_text = array(
    'R' => 'Зарегистрировался',
    'L' => 'Скачал вариант',
    'N' => 'Не явился',
    'Y' => 'Принял участие',
    'A' => 'Удален/Работа аннулирована',
    'D' => 'Отказался от участия',
    'W' => 'Выполняет работу',
    'We' => 'Завершил работу',
    'E' => 'Ошибка');


    if (isset($_REQUEST['cmd'])) {
        $cmd = $_REQUEST['cmd'];
    } else {
        $cmd = '';
    }

    if (isset($_REQUEST['btnStart'])) {
        $cmd = 'start';
    }
    if (isset($_REQUEST['btnEnd'])) {
        $cmd = 'end';
    }

    if (!array_key_exists('vcl_admin_mode', $_SESSION)) {
        $_SESSION['vcl_admin_mode'] = '';
    }
    if (array_key_exists('vcl_admin_SAId', $_SESSION)) {
        $SAId = $_SESSION['vcl_admin_SAId'] + 0;
    }
    if (empty($SAId)) {
        $_SESSION['vcl_admin_mode'] = '';
    }

// Выполнение команд

    if ($_SESSION['vcl_admin_mode'] == 'InsideAudit') {
        $res = $mysqli->query('SELECT DISTINCT au.link AS vclink, IFNULL(au.startDT,"") AS startDT, IFNULL(au.endDT,"") AS endDT, IFNULL(u1.fio,0) as startAdmin, IFNULL(u2.fio,0) as endAdmin, aud.name AS AuditName, str.name AS StreamName FROM olymp_stream_audits as au left join users as u1 ON(u1.id=au.startAdmin) left join users as u2 ON(u2.id=au.endAdmin) left join olymp_streams as str ON(str.id=au.IdStream) left join olimp_audit as aud ON(aud.id=au.IdAudit) WHERE (au.id=' . $SAId . ')');
        $VCLParams = $res->fetch_assoc();
    }

    switch ($cmd) {
        case 'enter':
        {    if (array_key_exists('SAId', $_REQUEST)) {
                $SAId = $_REQUEST['SAId'] + 0;
        }
            $res = $mysqli->query('SELECT count(distinct sa.id) as cnt FROM olymp_stream_audits as sa left join olymp_streams as s ON(s.id=sa.IdStream) left join olimp_audit as au ON(au.id=sa.IdAudit) left join olymp_stream_audit_admins as saadm ON(saadm.IdStreamAudit=sa.Id) left join olymp_stream_admins as sadm ON(sadm.IdStream=sa.IdStream) where (sadm.IdPerson=' . $uid . ' OR saadm.IdPerson=' . $uid . ') AND (s.opened>0) AND (sa.id=' . $SAId . ')');
        if ($row = $res->fetch_assoc()) {
            if ($row['cnt'] > 0) {
                $_SESSION['vcl_admin_mode'] = 'InsideAudit';
                    $_SESSION['vcl_admin_SAId'] = $SAId;
            } else {
                $SAId = 0;
                $error = 'Комната недоступна';
            }
        } else {
            $SAId = 0;
            $error = 'Комната недоступна';
        }
        break;
        }
        case 'leave':
        {    $_SESSION['vcl_admin_mode'] = '';
            $_SESSION['vcl_admin_SAId'] = 0;
            break;
        }
        case 'start':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        if (!empty($VCLParams['startDT'])) {
            $error = 'Олимпиада уже начата!';
            break;
        }
        $mysqli->query('update olymp_stream_audits set startDT=Now(), startAdmin=' . $uid . ' where id=' . $SAId);
        $mysqli->query('update olimp_actions AS a LEFT JOIN olimp_groups AS g ON(g.id=a.group_id) set startDT=Now(), startAdmin=' . $uid . ' WHERE (a.presence="W") AND ((a.audit=' . $SAId . ') OR ((g.audit=' . $SAId . ') AND (a.audit=0)))');

        break;
        }
        case 'end':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        if (!empty($VCLParams['endDT'])) {
            $error = 'Олимпиада уже завершена!';
            break;
        }
        $res = $mysqli->query('select count(a.person_id) as cnt FROM olimp_actions AS a LEFT JOIN olimp_groups AS g ON(g.id=a.group_id) WHERE (a.presence="W") AND ISNULL(a.endDT) AND ((a.audit=' . $SAId . ') OR ((g.audit=' . $SAId . ') AND (a.audit=0)))');
        $row = $res->fetch_assoc();
        if ($row['cnt'] == 0) {
            $mysqli->query('update olymp_stream_audits set endDT=Now(), endAdmin=' . $uid . ' where id=' . $SAId);
        } else {
            $error = 'Для завершения олимпиады необходимо принять работы всех участников!';
        }
        break;
        }
        case 'cmdStart1':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        $IdPerson = array_key_exists('IdPerson', $_REQUEST) ? $_REQUEST['IdPerson'] + 0 : 0;
        if (empty($IdPerson)) {
            $error = 'Неверный ID участника';
            break;
        }
        $mysqli->query('update olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) set a.presence="W" where (a.person_id=' . $IdPerson . ') AND ((a.audit=' . $SAId . ') OR (g.audit=' . $SAId . '))');
        break;
        }
        case 'cmdReset':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        $IdPerson = array_key_exists('IdPerson', $_REQUEST) ? $_REQUEST['IdPerson'] + 0 : 0;
        if (empty($IdPerson)) {
            break;
        }
        $mysqli->query('update olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) set a.presence="R", a.startDT=NULL, a.startAdmin=NULL, a.endDT=NULL, a.endAdmin=NULL where (a.person_id=' . $IdPerson . ') AND ((a.audit=' . $SAId . ') OR (g.audit=' . $SAId . '))');
        break;
        }
        case 'cmdStart2':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        $IdPerson = array_key_exists('IdPerson', $_REQUEST) ? $_REQUEST['IdPerson'] + 0 : 0;
        if (empty($IdPerson)) {
            break;
        }
        $mysqli->query('update olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) set a.presence="W", a.startDT=Now(), a.startAdmin=' . $uid . ' where (a.person_id=' . $IdPerson . ') AND ((a.audit=' . $SAId . ') OR (g.audit=' . $SAId . '))');
        break;
        }
        case 'cmdAccept':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        $IdPerson = array_key_exists('IdPerson', $_REQUEST) ? $_REQUEST['IdPerson'] + 0 : 0;
        if (empty($IdPerson)) {
            break;
        }
        $mysqli->query('update olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) set a.endDT=Now(), a.endAdmin=' . $uid . ' where (a.person_id=' . $IdPerson . ') AND ((a.audit=' . $SAId . ') OR (g.audit=' . $SAId . '))');
        break;
        }
        case 'cmdReject':
        {    if ($_SESSION['vcl_admin_mode'] != 'InsideAudit') {
                break;
        }
        $IdPerson = array_key_exists('IdPerson', $_REQUEST) ? $_REQUEST['IdPerson'] + 0 : 0;
        if (empty($IdPerson)) {
            break;
        }
        $mysqli->query('update olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) set a.presence="A", a.endDT=Now(), a.endAdmin=' . $uid . ' where (a.person_id=' . $IdPerson . ') AND ((a.audit=' . $SAId . ') OR (g.audit=' . $SAId . '))');
        break;
        }
    }

// Формирование кода страницы


    switch ($_SESSION['vcl_admin_mode']) {
        case 'InsideAudit':
        {    $res = $mysqli->query('SELECT DISTINCT au.link AS vclink, au.admin_link AS avclink, IFNULL(au.startDT,"") AS startDT, IFNULL(au.endDT,"") AS endDT, IFNULL(u1.fio,0) as startAdmin, IFNULL(u2.fio,0) as endAdmin, aud.name AS AuditName, str.name AS StreamName FROM olymp_stream_audits as au left join users as u1 ON(u1.id=au.startAdmin) left join users as u2 ON(u2.id=au.endAdmin) left join olymp_streams as str ON(str.id=au.IdStream) left join olimp_audit as aud ON(aud.id=au.IdAudit) WHERE (au.id=' . $SAId . ')');
            $VCLParams = $res->fetch_assoc();
            if (empty($VCLParams['startDT'])) {
                $stage = 1;
                $stageHeader = str_replace(array('%SAId%','%vclink%','%avclink%','%StreamName%','%AuditName%'), array($SAId,$VCLParams['vclink'],$VCLParams['avclink'],$VCLParams['StreamName'],$VCLParams['AuditName']), $tpl['VCL']['header_stage1']);
            } elseif (empty($VCLParams['endDT'])) {
                $stage = 2;
                $stageHeader = str_replace(array('%SAId%','%vclink%','%avclink%','%StreamName%','%AuditName%','%startDT%','%startAdmin%'), array($SAId,$VCLParams['vclink'],$VCLParams['avclink'],$VCLParams['StreamName'],$VCLParams['AuditName'],$VCLParams['startDT'],$VCLParams['startAdmin']), $tpl['VCL']['header_stage2']);
            } else {
                $stage = 3;
                $stageHeader = str_replace(array('%SAId%','%vclink%','%avclink%','%StreamName%','%AuditName%','%startDT%','%startAdmin%','%endDT%','%endAdmin%'), array($SAId,$VCLParams['vclink'],$VCLParams['avclink'],$VCLParams['StreamName'],$VCLParams['AuditName'],$VCLParams['startDT'],$VCLParams['startAdmin'],$VCLParams['endDT'],$VCLParams['endAdmin']), $tpl['VCL']['header_stage3']);
            }
            $persons_list = '';
            $res = $mysqli->query('SELECT DISTINCT p.id AS IdPerson, CONCAT_WS(" ",p.surename,p.name,p.patronymic) as fio, p.school_class AS school_class, a.presence AS presence, IFNULL(a.startDT,"") as startDT, IFNULL(a.endDT,"") as endDT, s.var_pwd as varPwd, p.tel as tel, p.email as email FROM olimp_actions AS a LEFT JOIN olimp_groups AS g ON(g.id=a.group_id) LEFT JOIN olimp_persons AS p ON (p.id=a.person_id) LEFT JOIN olimp_stages as s ON(s.id=a.stage_id) WHERE (a.audit=' . $SAId . ') OR ((g.audit=' . $SAId . ') AND (a.audit=0))');
            while ($row = $res->fetch_assoc()) {
                $commands = '';
                $comments = '';
                if ($row['presence'] == 'W' && !empty($row['endDT'])) {
                    $row['presence'] = 'We';
                }
                switch ($stage) {
                    case 1:
                    {    switch ($row['presence']) {
                            case 'R':
                                {    $commands = str_replace('%IdPerson%', $row['IdPerson'], $tpl['VCL']['commands_1_R']);
                                    $comments = $tpl['VCL']['comment_1_R'];
                                    break;
                            }
                            case 'D':
                            case 'N':
                            {    $commands = str_replace('%IdPerson%', $row['IdPerson'], $tpl['VCL']['commands_1_D']);
                                $comments = $tpl['VCL']['comment_1_D'];
                        break;
                            }
                            case 'L':
                            {    $commands = str_replace('%IdPerson%', $row['IdPerson'], $tpl['VCL']['commands_1_L']);
                                break;
                            }
                    }
                    break;
                    }
                    case 2:
                    {    switch ($row['presence']) {
                            case 'W':
                            {    $commands = str_replace('%IdPerson%', $row['IdPerson'], $tpl['VCL']['commands_2_W']);
                                break;
                            }
                            case 'We':
                            {    $commands = str_replace('%IdPerson%', $row['IdPerson'], $tpl['VCL']['commands_2_We']);
                                break;
                            }
                            case 'L':
                            {    $commands = str_replace('%IdPerson%', $row['IdPerson'], $tpl['VCL']['commands_2_L']);
                                $comments = $tpl['VCL']['comment_2_L'];
                                break;
                            }
                    }
                    break;
                    }
                    case 3:
                    {    switch ($row['presence']) {
                            case 'R':
                            {
                                break;
                            }
                    }
                    break;
                    }
                }
                $xPresence = '';
                if ($row['presence'] == "W") {
                    if ($row['startDT']) {
                        $xPresence = '<br>(' . $row['startDT'] . ')';
                    } else {
                        $xPresence = '<br>(не приступил)';
                    }
                }
                if ($row['presence'] == "We") {
                    $xPresence = '<br>(' . $row['startDT'] . ' - ' . $row['endDT'] . ')';
                }

                $persons_list .= str_replace(array('%SAId%','%IdPerson%','%fio%','%school_class%','%presence%','%commands%','%comments%','%varPwd%','%tel%','%email%'), array($SAId,$row['IdPerson'],$row['fio'],$row['school_class'],$presence_text[$row['presence']] . $xPresence,$commands,$comments,$row['varPwd'],$row['tel'],$row['email']), $tpl['VCL']['person_list_line']);
            }

            $body = str_replace(
                array('%SAId%','%person_list%','%header%'),
                array($SAId,$persons_list,$stageHeader),
                $tpl['VCL']['person_list']
            );
                break;
        }
        default:
        {    // Список доступных комнат
            $audit_list = '';
            $audit_stream = '';
            $res = $mysqli->query('SELECT distinct sa.id AS SAId,sa.IdAudit AS AuId,au.name AS AuditName,s.name AS StreamName FROM olymp_stream_audits as sa left join olymp_streams as s ON(s.id=sa.IdStream) left join olimp_audit as au ON(au.id=sa.IdAudit) left join olymp_stream_audit_admins as saadm ON(saadm.IdStreamAudit=sa.Id) left join olymp_stream_admins as sadm ON(sadm.IdStream=sa.IdStream) where (sadm.IdPerson=' . $uid . ' OR saadm.IdPerson=' . $uid . ') AND (s.opened>0) ORDER BY s.dt,s.name,au.name');
            while ($row = $res->fetch_assoc()) {
                if ($row['StreamName'] != $audit_stream) {
                        $audit_stream = $row['StreamName'];
                            $audit_list .= str_replace(array('%StreamName%'), array($row['StreamName']), $tpl['VCL']['audit_list_stream']);
                }
                $audit_list .= str_replace(array('%SAId%','%AuId%','%AuditName%','%StreamName%'), array($row['SAId'],$row['AuId'],$row['AuditName'],$row['StreamName']), $tpl['VCL']['audit_list_line']);
            }


            $body = str_replace(
                array('%audit_list%'),
                array($audit_list),
                $tpl['VCL']['audit_list']
            );
        }
    }

    if (!empty($error)) {
        $error = str_replace(array('%title%','%text%'), array('Ошибка!',$error), $tpl['common']['error']);
    }
    if (!empty($message)) {
        $message = str_replace(array('%title%','%text%'), array('Внимание!',$message), $tpl['common']['highlight']);
    }

    return str_replace(array('%error%','%message%'), array($error,$message), $body);
}



$tpl['VCL']['audit_list'] = '%error%<table>%audit_list%</table>';

$tpl['VCL']['audit_list_stream'] = '<tr><td>%StreamName%</td></tr>';
$tpl['VCL']['audit_list_line'] = '<tr><td><a href="?cmd=enter&SAId=%SAId%">%AuditName%</a></td></tr>';

$tpl['VCL']['person_list'] = '
<style>
.tblHeader td {color: white; background:darkblue; }
#personList {border-collapse: collapse;}
#personList td {border:1px solid blue; padding:10px;}
#personList .personFIO {font-weight:bold;}
img.identIcon {width:75px; height:100px; position:static;}
img.identPhoto {border:3px dashed green; display:none; position:fixed; top:10px; right:10px; max-width:800px;}
div#stageHeader {margin-bottom:10px;}
div#stageHeader #status {font-size:14px; color:blue; font-weight:bold;}
.vcl_warning {color:red; font-weight:bold;}

</style>
<script>

$(function() {

$(".identIcon").hover(
function () {
    $("#"+$(this).attr("id")+"Big").show();
    //alert($(this).attr("id")+"Big");
  },
function () {
    $("#"+$(this).attr("id")+"Big").hide();
  }
);

window.setTimeout(function(){location.reload();}, 300000);

}); 

</script>
%header%
%error%<table id="personList"><tr class="tblHeader"><td>Фото</td><td>ФИО</td><td>Класс</td><td>Пароль</td><td>Статус</td><td>Действия</td><td>Комментарий</td></tr>%person_list%</table>';

$tpl['VCL']['person_list_line'] = '<tr><td><img src="/ajax/olimp_get_photo.php?IdPerson=%IdPerson%" class="identIcon" id="identPhoto%IdPerson%"><img src="/ajax/olimp_get_photo.php?IdPerson=%IdPerson%" class="identPhoto" id="identPhoto%IdPerson%Big"></td><td><div class="personFIO">%fio% </div></td><td>%school_class%</td><td>%varPwd%</td><td>%presence%</td><td>%commands%</td><td>%comments%</td></tr>';
// id:%IdPerson% <br>%tel% <br>%email%

$tpl['VCL']['header_stage1'] = '
<div id="stageHeader"><h1>%StreamName%</h1><h2>%AuditName% [<a href="?cmd=leave">Выйти из комнаты</a>]</h2>
<h2>Ссылка для администратора: %avclink%</h2>
<h2>Ссылка для участников: %vclink%</h2>
<form action=""><span id="status">Олимпиада не начата </span>&nbsp;&nbsp;<button type="submit" name="btnStart" id="btnStart">Начать олимпиаду</button></form></div>
';
$tpl['VCL']['header_stage2'] = '
<div id="stageHeader"><h1>%StreamName%</h1><h2>%AuditName% [<a href="?cmd=leave">Выйти из комнаты</a>]</h2>
<h2>Ссылка для администратора: %avclink%</h2>
<h2>Ссылка для участников: %vclink%</h2>
<form action=""><span id="status">Олимпиада начата %startDT% (%startAdmin%)</span>&nbsp;&nbsp;<button type="submit" name="btnEnd" id="btnEnd">Завершить проведение олимпиады</button></form></div>
';
$tpl['VCL']['header_stage3'] = '
<div id="stageHeader"><h1>%StreamName%</h1><h2>%AuditName% [<a href="?cmd=leave">Выйти из комнаты</a>]</h2>
<h2>Ссылка для администратора: %avclink%</h2>
<h2>Ссылка для участников: %vclink%</h2>
<form><span id="status">Олимпиада проведена с %startDT% (%startAdmin%) по %endDT% (%endAdmin%)</span></form></div>
';

$tpl['VCL']['commands_1_R'] = '';
$tpl['VCL']['commands_1_D'] = '<a href="?cmd=cmdReset&IdPerson=%IdPerson%">[Вернуть исходный]</a>';
$tpl['VCL']['commands_1_L'] = '<a href="?cmd=cmdStart1&IdPerson=%IdPerson%">[Допустить к работе]</a>';

$tpl['VCL']['commands_2_W'] = '<a href="?cmd=cmdAccept&IdPerson=%IdPerson%">[Принять работу]</a><br><br><a href="?cmd=cmdReject&IdPerson=%IdPerson%">[Удалить участника]</a>';
$tpl['VCL']['commands_2_We'] = '';
$tpl['VCL']['commands_2_L'] = '<a href="?cmd=cmdStart2&IdPerson=%IdPerson%">[Допустить к работе]</a>';

$tpl['VCL']['comment_1_R'] = '<span class="vcl_warning">Не может начать работу, необходимо загрузить вариант!</span>';
$tpl['VCL']['comment_1_D'] = '<span class="vcl_warning">Не может начать работу! Возврат в исходный только с разрешения оргкомитета!</span>';

$tpl['VCL']['comment_2_L'] = '<span class="vcl_warning">Допуск только с разрешения оргкомитета!</span>';

$body = build_page();
