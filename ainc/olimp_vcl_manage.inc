<?php

// Подготовка олимпиады с ВКС
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli,$error,$message;

    if (($role & 16) == 0) {
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

    $vcl_forms = '6';

    if (isset($_REQUEST['cmd'])) {
        $cmd = $_REQUEST['cmd'];
    } else {
        $cmd = '';
    }

    if (isset($_REQUEST['btnApplyStreamParams'])) {
        $cmd = 'ApplyStreamParams';
    }

    if (isset($_REQUEST['btnChangePwd']) && isset($_REQUEST['changeVarPwd']) && is_array($_REQUEST['changeVarPwd'])) {
        $changeVarPwd = $_REQUEST['changeVarPwd'];
        $cmd = 'ChangePwd';
    }

    if (isset($_REQUEST['btnRemoveStages']) && isset($_REQUEST['IdStage']) && is_array($_REQUEST['IdStage'])) {
        $IdStages = $_REQUEST['IdStage'];
        $cmd = 'RemoveStages';
    }

    if (isset($_REQUEST['btnLinkStages']) && isset($_REQUEST['IdStage']) && is_array($_REQUEST['IdStage'])) {
        $IdStages = $_REQUEST['IdStage'];
        $cmd = 'LinkStages';
    }

    if (isset($_REQUEST['btnLinkAudits']) && isset($_REQUEST['IdAudit']) && is_array($_REQUEST['IdAudit'])) {
        $IdAudits = $_REQUEST['IdAudit'];
        $cmd = 'LinkAudits';
    }

    if (isset($_REQUEST['btnReleaseAudits']) && isset($_REQUEST['IdAudit']) && is_array($_REQUEST['IdAudit'])) {
        $IdAudits = $_REQUEST['IdAudit'];
        $cmd = 'ReleaseAudits';
    }

    if (isset($_REQUEST['btnPlaceGroups'])) {
        $cmd = 'PlaceGroups';
    }

    if (isset($_REQUEST['removeGroup']) && is_array($_REQUEST['removeGroup'])) {
        $IdGroups = $_REQUEST['removeGroup'];
        $cmd = 'RemoveGroups';
    }

    if (isset($_REQUEST['removeGroups']) && isset($_REQUEST['IdGroup']) && is_array($_REQUEST['IdGroup'])) {
        $IdGroups = $_REQUEST['IdGroup'];
        $cmd = 'RemoveGroups';
    }

    if (isset($_REQUEST['btnApplyParams'])) {
        $cmd = 'ApplyParams';
    }

    if (isset($_REQUEST['DismissAdmin']) && is_array($_REQUEST['DismissAdmin'])) {
        $IdAdmins = $_REQUEST['DismissAdmin'];
        $cmd = 'DismissAdmins';
    }

    if (isset($_REQUEST['DismissAdmins']) && isset($_REQUEST['IdAdmin']) && is_array($_REQUEST['IdAdmin'])) {
        $IdAdmins = $_REQUEST['IdAdmin'];
        $cmd = 'DismissAdmins';
    }

    if (isset($_REQUEST['btnTransAudit']) && isset($_REQUEST['transAudit']) && is_array($_REQUEST['transAudit'])) {
        $transAudit = $_REQUEST['transAudit'];
        $cmd = 'TransAudit';
    }

    if (isset($_REQUEST['btnRestoreAudit']) && isset($_REQUEST['restoreAudit']) && is_array($_REQUEST['restoreAudit'])) {
        $restoreAudit = $_REQUEST['restoreAudit'];
        $cmd = 'RestoreAudit';
    }


    if (!array_key_exists('vcl_manage_mode', $_SESSION)) {
        $_SESSION['vcl_manage_mode'] = '';
    }
    if (array_key_exists('vcl_manage_SId', $_SESSION)) {
        $SId = $_SESSION['vcl_manage_SId'] + 0;
    }
    if (array_key_exists('vcl_manage_SAId', $_SESSION)) {
        $SAId = $_SESSION['vcl_manage_SAId'] + 0;
    }
    if (empty($SId)) {
        $_SESSION['vcl_manage_mode'] = '';
    }
    if (($_SESSION['vcl_manage_mode'] == 'InsideAudit') && empty($SAId)) {
        $_SESSION['vcl_manage_mode'] = '';
    }

// Выполнение команд

    if ($_SESSION['vcl_manage_mode'] == 'InsideStream') {
        $res = $mysqli->query('SELECT DISTINCT s.name AS StreamName, IFNULL(s.dt,"") AS StreamDT, s.opened AS StreamOpened FROM olymp_streams as s WHERE (s.id=' . $SId . ')');
        $StreamParams = $res->fetch_assoc();
    }
    if ($_SESSION['vcl_manage_mode'] == 'InsideAudit') {
        $res = $mysqli->query('SELECT DISTINCT au.link AS vclink, IFNULL(au.startDT,"") AS startDT, IFNULL(au.endDT,"") AS endDT, IFNULL(u1.fio,0) as startAdmin, IFNULL(u2.fio,0) as endAdmin, aud.name AS AuditName, str.name AS StreamName FROM olymp_stream_audits as au left join users as u1 ON(u1.id=au.startAdmin) left join users as u2 ON(u2.id=au.endAdmin) left join olymp_streams as str ON(str.id=au.IdStream) left join olimp_audit as aud ON(aud.id=au.IdAudit) WHERE (au.id=' . $SAId . ')');
        $VCLParams = $res->fetch_assoc();
    }

    switch ($cmd) {
        case 'enterStream':
        {    if (array_key_exists('SId', $_REQUEST)) {
                $SId = $_REQUEST['SId'] + 0;
        }
        if (!empty($SId)) {    //  !!!!!!!!!!!  Проверка, что смена существует и доступен пользователю
            $_SESSION['vcl_manage_mode'] = 'InsideStream';
            $_SESSION['vcl_manage_SId'] = $SId;
        } else {
            $error = 'Неверный поток!';
        }
        break;
        }
        case 'leaveStream':
        {    $_SESSION['vcl_manage_mode'] = '';
            $_SESSION['vcl_manage_SId'] = 0;
            break;
        }

        case 'ApplyStreamParams':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideStream') {
                break;
        }
        if (empty($_REQUEST['StreamName']) || empty($_REQUEST['StreamDT'])) {
            break;
        }
        $StreamName = trim($_REQUEST['StreamName']);
        $StreamDT = trim($_REQUEST['StreamDT']);
        $StreamOpened = $_REQUEST['StreamOpened'] + 0;
        if (empty($StreamName) || empty($StreamDT) || !preg_match('/[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}/', $StreamDT)) {
            break;
        }
        $mysqli->query('update olymp_streams set name="' . $mysqli->real_escape_string($StreamName) . '", dt="' . $mysqli->real_escape_string($StreamDT) . '", opened=' . $StreamOpened . ' where id=' . $SId);
        if (!empty($_REQUEST['IdAdmin']) || is_array($_REQUEST['IdAdmin'])) {
            $mysqli->query('delete from olymp_stream_admins where IdStream=' . $SId);
            foreach ($_REQUEST['IdAdmin'] as $IdAdmin => $w) {
                $mysqli->query('INSERT INTO olymp_stream_admins VALUES (' . $SId . ', ' . $IdAdmin . ')');
            }
        }
        break;
        }

        case 'enterAudit':
        {    if (array_key_exists('SAId', $_REQUEST)) {
                $SAId = $_REQUEST['SAId'] + 0;
        }
        if (!empty($SAId)) {    //  !!!!!!!!!!!  Проверка, что аудитория существует и в правильтном потоке
            $_SESSION['vcl_manage_mode'] = 'InsideAudit';
            $_SESSION['vcl_manage_SAId'] = $SAId;
        } else {
            $error = 'Неверная комната!';
        }
        break;
        }
        case 'leaveAudit':
        {    $_SESSION['vcl_manage_mode'] = 'InsideStream';
            $_SESSION['vcl_manage_SAId'] = 0;
            break;
        }

        case 'ChangePwd':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideStream') {
                break;
        }
        if (empty($changeVarPwd) || !is_array($changeVarPwd) || count($changeVarPwd) < 1) {
            $error = 'Пароли не заданы';
            break;
        }
        foreach ($changeVarPwd as $IdStage => $varPwd) {
            $mysqli->query('update olimp_stages set var_pwd="' . $mysqli->real_escape_string($varPwd) . '" where (id=' . $IdStage . ') AND (stream=' . $SId . ')');
        }

        break;
        }
        case 'RemoveStages':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideStream') {
                break;
        }
        if (empty($IdStages) || !is_array($IdStages) || count($IdStages) < 1) {
            $error = 'Потоки не заданы';
            break;
        }
        $skipped = 0;
        foreach ($IdStages as $IdStage => $w) {
            $inUse = 0;
            // Рассаженные группы
            $res = $mysqli->query('select count(distinct g.id) as cnt from olimp_groups as g left join olymp_stream_audits as sa ON(sa.id=g.audit) WHERE (g.stage=' . $IdStage . ') AND (sa.IdStream=' . $SId . ')');
            $row = $res->fetch_assoc();
            $inUse = $row['cnt'];
            // Пересаженные участники (НЕ ПРОТЕСТИРОВАНО ПРИ НАЛИЧИИ УЧАСТНИКА)
            $res = $mysqli->query('select count(distinct a.person_id) as cnt from olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) left join olymp_stream_audits as sa ON(sa.id=a.audit) WHERE (g.stage=' . $IdStage . ') AND (sa.IdStream=' . $SId . ')');
            $row = $res->fetch_assoc();
            $inUse += $row['cnt'];

            if ($inUse == 0) {
                $mysqli->query('update olimp_stages set stream=0 where (id=' . $IdStage . ') AND (stream=' . $SId . ')');
            } else {
                $skipped += 1;
            }
        }
        if ($skipped > 0) {
            $error = 'Не удалены потоки (' . $skipped . '), в которых есть рассаженные участники';
        }
        break;
        }

        case 'LinkStages':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideStream') {
                break;
        }
        if (empty($IdStages) || !is_array($IdStages) || count($IdStages) < 1) {
            $error = 'Потоки не заданы';
            break;
        }
        $StageIds = '';
        foreach ($IdStages as $StageId => $w) {
            $StageIds .= ($StageIds ? ', ' : '') . $StageId;
        }
        $res = $mysqli->query('select count(distinct s.id) as cnt from olimp_stages as s where (s.id in (' . $StageIds . ')) AND (s.stream<>0) AND (s.stream<>' . $SId . ')');
        $row = $res->fetch_assoc();
        if ($row['cnt'] > 0) {
            $error = 'Поток уже привязан к другой смене!';
            break;
        }
        $mysqli->query('update olimp_stages set stream=' . $SId . ' where id IN (' . $StageIds . ')');

        break;
        }

        case 'LinkAudits':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideStream') {
                break;
        }
        if (empty($IdAudits) || !is_array($IdAudits) || count($IdAudits) < 1) {
            $error = 'Аудитории не заданы';
            break;
        }
        foreach ($IdAudits as $AuditId => $w) {
            $res = $mysqli->query('select count(distinct sa.id) as cnt from olymp_stream_audits as sa where (sa.IdAudit=' . $AuditId . ') AND (sa.IdStream=' . $SId . ')');
            $row = $res->fetch_assoc();
            if ($row['cnt'] == 0) {
                $mysqli->query('INSERT INTO olymp_stream_audits(IdStream,IdAudit) VALUES (' . $SId . ', ' . $AuditId . ')');
            }
        }
        break;
        }

        case 'ReleaseAudits':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideStream') {
                break;
        }
        if (empty($IdAudits) || !is_array($IdAudits) || count($IdAudits) < 1) {
            $error = 'Аудитории не заданы';
            break;
        }
        foreach ($IdAudits as $AuditId => $w) {
            $res = $mysqli->query('select count(distinct sa.id) as cnt from olymp_stream_audits as sa where (sa.IdAudit=' . $AuditId . ') AND (sa.IdStream=' . $SId . ')');
            $row = $res->fetch_assoc();
            if ($row['cnt'] > 0) {
                $mysqli->query('DELETE FROM olymp_stream_audits WHERE (IdStream=' . $SId . ') AND (IdAudit=' . $AuditId . ')');
            }
        }
        break;
        }

        case 'PlaceGroups':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideAudit') {
                break;
        }
        if (!array_key_exists('IdGroup', $_REQUEST) && !is_array($_REQUEST['IdGroup'])) {
            $error = 'Группы не заданы';
            break;
        }
        $GIds = '';
        foreach ($_REQUEST['IdGroup'] as $GId => $w) {
            $GIds .= ($GIds ? ', ' : '') . $GId;
        }

        $res = $mysqli->query('select count(distinct g.id) as cnt from olimp_groups as g where (g.id in (' . $GIds . ')) AND (g.audit<>0)');
        $row = $res->fetch_assoc();
        if ($row['cnt'] > 0) {
            $error = 'Размещаемая группа(группы) уже размещена в другой комнате!';
            break;
        }
        $res = $mysqli->query('select count(distinct g.id) as cnt from olimp_groups as g left join olimp_stages as s ON(s.id=g.stage) where (g.id in (' . $GIds . ')) AND (s.stream<>' . $SId . ')');
        $row = $res->fetch_assoc();
        if ($row['cnt'] > 0) {
            $error = 'Размещаемая группа(группы) из другого потока!';
            break;
        }
        $mysqli->query('update olimp_groups set audit=' . $SAId . ' where id IN (' . $GIds . ')');
        break;
        }
        case 'RemoveGroups':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideAudit') {
                break;
        }
        if (empty($IdGroups) || !is_array($IdGroups) || count($IdGroups) < 1) {
            $error = 'Группы не заданы';
            break;
        }
        $GIds = '';
        foreach ($IdGroups as $GId => $w) {
            $GIds .= ($GIds ? ', ' : '') . $GId;
        }

        $res = $mysqli->query('select count(distinct g.id) as cnt from olimp_groups as g where (g.id in (' . $GIds . ')) AND (g.audit<>0) AND(g.audit<>' . $SAId . ')');
        $row = $res->fetch_assoc();
        if ($row['cnt'] > 0) {
            $error = 'Группа(группы) размещена в другой комнате!';
            break;
        }
        $mysqli->query('update olimp_groups set audit=0 where id IN (' . $GIds . ') AND audit=' . $SAId);
        break;
        }

        case 'ApplyParams':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideAudit') {
                break;
        }
        if (array_key_exists('vclink', $_REQUEST) && array_key_exists('avclink', $_REQUEST)) {
            $mysqli->query('update olymp_stream_audits set link="' . $mysqli->real_escape_string($_REQUEST['vclink']) . '", admin_link="' . $mysqli->real_escape_string($_REQUEST['avclink']) . '" where id=' . $SAId);
        }
        if (!array_key_exists('IdAdmin', $_REQUEST) && !is_array($_REQUEST['IdAdmin'])) {
            break;
        }
        $AIds = '';
        foreach ($_REQUEST['IdAdmin'] as $AId => $w) {
            $AIds .= ($AIds ? ', ' : '') . $AId;
        }
        $res = $mysqli->query('select u.id as IdPerson, IFNULL(saadm.IdStreamAudit,0) as inAudit from users as u left join olymp_stream_audit_admins as saadm ON(saadm.IdPerson=u.id AND saadm.IdStreamAudit=' . $SAId . ') where u.id IN (' . $AIds . ')');
        while ($row = $res->fetch_assoc()) {
            if ($row['inAudit'] == 0) {
                $mysqli->query('insert into olymp_stream_audit_admins set IdStreamAudit=' . $SAId . ', IdPerson=' . $row['IdPerson']);
            }
        }
        break;
        }
        case 'DismissAdmins':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideAudit') {
                break;
        }
        if (empty($IdAdmins) || !is_array($IdAdmins) || count($IdAdmins) < 1) {
            $error = 'Освобождаемые администраторы не заданы';
            break;
        }
        $AIds = '';
        foreach ($IdAdmins as $AId => $w) {
            $AIds .= ($AIds ? ', ' : '') . $AId;
        }

        $mysqli->query('delete from olymp_stream_audit_admins where IdPerson IN (' . $AIds . ') AND IdStreamAudit=' . $SAId);
        break;
        }
        case 'TransAudit':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideAudit') {
                break;
        }
        if (empty($transAudit) || !is_array($transAudit) || count($transAudit) < 1) {
            $error = 'Пересаживаемые участники не заданы';
            break;
        }
        if (empty($_REQUEST['transToAudit'])) {
            $error = 'Не задана целевая комната';
            break;
        }
        // ПРОВЕРКА НА ПРИНАДЛЕЖНОСТЬ КОМНАТЫ СМЕНЕ

        $PIds = '';
        foreach ($transAudit as $PId => $w) {
            $PIds .= ($PIds ? ', ' : '') . $PId;
        }

        $mysqli->query('update olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) set a.audit=' . ($_REQUEST['transToAudit'] + 0) . ' where (a.person_id IN (' . $PIds . ')) AND ((a.audit=' . $SAId . ') OR (g.audit=' . $SAId . '))');

        break;
        }

        case 'RestoreAudit':
        {    if ($_SESSION['vcl_manage_mode'] != 'InsideAudit') {
                break;
        }
        if (empty($restoreAudit) || !is_array($restoreAudit) || count($restoreAudit) < 1) {
            $error = 'Пересаживаемые участники не заданы';
            break;
        }

        $PIds = '';
        foreach ($restoreAudit as $PId => $w) {
            $PIds .= ($PIds ? ', ' : '') . $PId;
        }

        $mysqli->query('update olimp_actions as a set a.audit=0 where (a.person_id IN (' . $PIds . ')) AND (a.audit=' . $SAId . ')');

        break;
        }
    }

// Формирование кода страницы

    switch ($_SESSION['vcl_manage_mode']) {
        case 'InsideAudit':
        {    $res = $mysqli->query('SELECT DISTINCT au.link AS vclink, au.admin_link AS avclink, IFNULL(au.startDT,"") AS startDT, IFNULL(au.endDT,"") AS endDT, IFNULL(u1.fio,0) as startAdmin, IFNULL(u2.fio,0) as endAdmin, aud.name AS AuditName, str.name AS StreamName FROM olymp_stream_audits as au left join users as u1 ON(u1.id=au.startAdmin) left join users as u2 ON(u2.id=au.endAdmin) left join olymp_streams as str ON(str.id=au.IdStream) left join olimp_audit as aud ON(aud.id=au.IdAudit) WHERE (au.id=' . $SAId . ')');
            $VCLParams = $res->fetch_assoc();

            //Группы
            $groups_list = '';
            $res = $mysqli->query('SELECT g.id as IdGroup, g.name as GroupName, s.classes as classes, count(distinct a.person_id) as cnt FROM olimp_groups as g left join olimp_actions AS a ON(a.group_id=g.id AND a.audit=0) left join olimp_stages as s ON(s.id=g.stage) WHERE g.audit=' . $SAId . ' GROUP BY g.id, g.name');
            while ($row = $res->fetch_assoc()) {
                $groups_list .= str_replace(array('%SAId%','%IdGroup%','%GroupName%','%cnt%','%classes%'), array($SAId,$row['IdGroup'],$row['GroupName'],$row['cnt'],$row['classes']), $tpl['VCL']['group_list_line']);
            }

            $unplaced_groups_list = '';
            $res = $mysqli->query('SELECT g.id as IdGroup, g.name as GroupName, s.classes as classes, count(distinct a.person_id) as cnt FROM olimp_groups as g left join olimp_actions AS a ON(a.group_id=g.id AND a.audit=0) left join olimp_stages as s ON(s.id=g.stage) WHERE (s.stream=' . $SId . ') AND (g.audit=0) GROUP BY g.id, g.name');
            while ($row = $res->fetch_assoc()) {
                $unplaced_groups_list .= str_replace(array('%SAId%','%IdGroup%','%GroupName%','%cnt%','%classes%'), array($SAId,$row['IdGroup'],$row['GroupName'],$row['cnt'],$row['classes']), $tpl['VCL']['unplaced_group_list_line']);
            }

            // Администраторы
            $admins_list = '';
            $unassigned_admins_list = '';
            $AdminIDs = '';
            $Admins = array();
            $res = $mysqli->query('SELECT u.id as IdPerson, u.fio as AdminName, IFNULL(saadm.IdStreamAudit,0) as inAudit FROM users as u left join olymp_stream_audit_admins AS saadm ON(saadm.IdPerson=u.id AND saadm.IdStreamAudit=' . $SAId . ') where (u.role & 64)>0 order by u.fio');
            while ($row = $res->fetch_assoc()) {
                $AdminIDs .= ($AdminIDs ? ', ' : '') . $row['IdPerson'];
                $Admins[$row['IdPerson']] = $row;
            }
            $res = $mysqli->query('SELECT saadm.IdPerson, GROUP_CONCAT(au.name SEPARATOR "<br>") as OtherAudits FROM olymp_stream_audit_admins AS saadm left join olymp_stream_audits as sa ON(sa.id=saadm.IdStreamAudit) left join olimp_audit as au ON(au.id=sa.IdAudit) where (sa.IdStream=' . $SId . ') AND (sa.id<>' . $SAId . ') group by saadm.IdPerson');
            while ($row = $res->fetch_assoc()) {
                $Admins[$row['IdPerson']]['OtherAudits'] = $row['OtherAudits'];
            }
            foreach ($Admins as $IdAdmin => $Admin) {
                if ($Admin['inAudit']) {
                    $admins_list .= str_replace(array('%SAId%','%IdPerson%','%AdminName%','%OtherAudits%'), array($SAId,$IdAdmin,$Admin['AdminName'],$Admin['OtherAudits']), $tpl['VCL']['admins_list_line']);
                } else {
                    $unassigned_admins_list .= str_replace(array('%SAId%','%IdPerson%','%AdminName%','%OtherAudits%'), array($SAId,$IdAdmin,$Admin['AdminName'],$Admin['OtherAudits']), $tpl['VCL']['unassigned_admins_list_line']);
                }
            }
            if (empty($admins_list)) {
                $admins_list = $tpl['VCL']['admins_list_empty'];
            }
            if (empty($unassigned_admins_list)) {
                $unassigned_admins_list = $tpl['VCL']['unassigned_admins_list_empty'];
            }

            // Участники
            $persons_list = '';
            $res = $mysqli->query('SELECT p.id AS IdPerson, CONCAT_WS(" ",p.surename,p.name,p.patronymic) as fio, p.school_class AS school_class, a.presence AS presence, IFNULL(a.startDT,"") as startDT, IFNULL(a.endDT,"") as endDT, s.var_pwd as varPwd, p.tel as tel, p.email as email, IFNULL(a.audit,0) as transAudit FROM olimp_actions AS a LEFT JOIN olimp_groups AS g ON(g.id=a.group_id) LEFT JOIN olimp_persons AS p ON (p.id=a.person_id) LEFT JOIN olimp_stages as s ON(s.id=a.stage_id) WHERE (a.audit=' . $SAId . ') OR ((g.audit=' . $SAId . ') AND (a.audit=0))');
            while ($row = $res->fetch_assoc()) {
                if ($row['presence'] == 'W' && !empty($row['endDT'])) {
                    $row['presence'] = 'We';
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

                $persons_list .= str_replace(array('%SAId%','%IdPerson%','%fio%','%school_class%','%presence%','%commands%','%comments%','%varPwd%','%tel%','%email%'), array($SAId,$row['IdPerson'],$row['fio'],$row['school_class'],$presence_text[$row['presence']] . $xPresence,$commands,$comments,$row['varPwd'],$row['tel'],$row['email']), $tpl['VCL'][$row['transAudit'] ? 'person_list_line_trans' : 'person_list_line']);
            }

            // Комнаты для пересадки

            $transAuditList = '';
            $res = $mysqli->query('SELECT sa.id as SAId, au.name as AuditName FROM olymp_stream_audits AS sa LEFT JOIN olimp_audit AS au ON(au.id=sa.IdAudit) WHERE (sa.IdStream=' . $SId . ') AND (sa.id<>' . $SAId . ')');
            while ($row = $res->fetch_assoc()) {
                $transAuditList .= '<option value=' . $row['SAId'] . '>' . $row['AuditName'] . '</option>';
            }

            $body = str_replace(
                array('%SAId%','%person_list%','%group_list%','%admin_list%','%unplaced_group_list%','%unassigned_admins_list%','%vclink%','%avclink%','%CurrentAuditName%','%transAuditList%'),
                array($SAId,$persons_list,$groups_list,$admins_list,$unplaced_groups_list,$unassigned_admins_list,$VCLParams['vclink'] ? $VCLParams['vclink'] : '--- не задана ---',$VCLParams['avclink'] ? $VCLParams['avclink'] : '--- не задана ---',$VCLParams['AuditName'],$transAuditList),
                $tpl['VCL']['person_list']
            );


                break;
        }
        case 'InsideStream':
        {    if (empty($StreamParams)) {
        }
            {    $res = $mysqli->query('SELECT DISTINCT s.name AS StreamName, IFNULL(s.dt,"") AS StreamDT, s.opened AS StreamOpened FROM olymp_streams as s WHERE (s.id=' . $SId . ')');
            $StreamParams = $res->fetch_assoc();
            }
            // Комнаты
            $audit_list = '';
            $audit_stream = '';
            $res = $mysqli->query('SELECT DISTINCT sa.id AS SAId,sa.IdAudit AS AuId,au.name AS AuditName, sa.link as vclink, GROUP_CONCAT(u.fio SEPARATOR ", ") as admins FROM olymp_stream_audits as sa left join olimp_audit as au ON(au.id=sa.IdAudit) left join olymp_stream_audit_admins as saadm ON(saadm.IdStreamAudit=sa.Id) left join users as u ON(u.id=saadm.IdPerson) WHERE sa.IdStream=' . $SId . ' GROUP BY sa.id,sa.IdAudit ORDER BY au.name');
        while ($row = $res->fetch_assoc()) {
            $res2 = $mysqli->query('SELECT count(DISTINCT a.person_id) as cnt FROM olimp_actions as a left join olimp_groups as g ON(g.id=a.group_id) where (a.audit=' . $row['SAId'] . ') OR ((g.audit=' . $row['SAId'] . ') AND (a.audit=0))');
            $row2 = $res2->fetch_assoc();
            $audit_list .= str_replace(array('%SAId%','%IdAudit%','%AuditName%','%vclink%','%admins%','%cnt%'), array($row['SAId'],$row['AuId'],$row['AuditName'],$row['vclink'],$row['admins'],$row2['cnt']), $tpl['VCL']['audit_list_line']);
        }
        if (empty($audit_list)) {
            $audit_list = $tpl['VCL']['empty_audit_list'];
        }

            $unlinked_audit_list = '';
            $res = $mysqli->query('SELECT au.id AS IdAudit,au.name AS AuditName, au.c1 as cnt, IFNULL(sa.id,0) as SAId FROM olimp_audit as au left join olymp_stream_audits as sa ON((sa.IdAudit=au.id) AND (sa.IdStream=' . $SId . ')) WHERE (au.form in (' . $vcl_forms . ')) HAVING SAId=0');
        while ($row = $res->fetch_assoc()) {
            $unlinked_audit_list .= str_replace(array('%IdAudit%','%AuditName%','%cnt%'), array($row['IdAudit'],$row['AuditName'],$row['cnt']), $tpl['VCL']['unlinked_audit_list_line']);
        }
        if (empty($unlinked_audit_list)) {
            $unlinked_audit_list = $tpl['VCL']['empty_unlinked_audit_list'];
        }
            //Потоки
            $stages_list = '';
            $res = $mysqli->query('SELECT s.id as IdStage, e.name as ExamName, s.classes as classes, s.date_exam as examDate, s.var_pwd as varPwd FROM olimp_stages as s left join olimp_exams AS e ON(e.id=s.exam) WHERE s.stream=' . $SId . ' order by s.exam,s.classes');
        while ($row = $res->fetch_assoc()) {
            $stages_list .= str_replace(array('%IdStage%','%ExamName%','%classes%','%examDate%','%varPwd%'), array($row['IdStage'],$row['ExamName'],$row['classes'],$row['examDate'],$row['varPwd']), $tpl['VCL']['stages_list_line']);
        }
        if (empty($stages_list)) {
            $stages_list = $tpl['VCL']['empty_stages_list'];
        }

            $unlinked_stages_list = '';
            $res = $mysqli->query('SELECT s.id as IdStage, e.name as ExamName, s.classes as classes, s.date_exam as examDate, s.var_pwd as varPwd FROM olimp_stages as s left join olimp_exams AS e ON(e.id=s.exam) WHERE s.stream=0 AND s.form=6 order by s.exam,s.classes');
        while ($row = $res->fetch_assoc()) {
            $unlinked_stages_list .= str_replace(array('%IdStage%','%ExamName%','%classes%','%examDate%','%varPwd%'), array($row['IdStage'],$row['ExamName'],$row['classes'],$row['examDate']), $tpl['VCL']['unlinked_stages_list_line']);
        }
        if (empty($unlinked_stages_list)) {
            $unlinked_stages_list = $tpl['VCL']['empty_unlinked_stages_list'];
        }

            // Администраторы

            $view_sadmins_list = '';
            $edit_sadmins_list = '';
            $res = $mysqli->query('SELECT u.id as IdPerson, u.fio as AdminName, IFNULL(sadm.IdStream,0) as inStream FROM users as u left join olymp_stream_admins AS sadm ON(sadm.IdPerson=u.id AND sadm.IdStream=' . $SId . ') where (u.role & 64)>0 order by u.fio');
        while ($row = $res->fetch_assoc()) {
            if ($row['inStream']) {
                    $view_sadmins_list .= str_replace(array('%SId%','%IdPerson%','%AdminName%'), array($SId,$row['IdPerson'],$row['AdminName']), $tpl['VCL']['view_sadmins_list_line']);
                            $edit_sadmins_list .= str_replace(array('%SId%','%IdPerson%','%AdminName%','%check%'), array($SId,$row['IdPerson'],$row['AdminName'],'checked'), $tpl['VCL']['edit_sadmins_list_line']);
            } else {
                $edit_sadmins_list .= str_replace(array('%SId%','%IdPerson%','%AdminName%','%check%'), array($SId,$row['IdPerson'],$row['AdminName'],''), $tpl['VCL']['edit_sadmins_list_line']);
            }
        }
        if (empty($view_sadmins_list)) {
            $view_sadmins_list = $tpl['VCL']['view_sadmins_list_empty'];
        }
        if (empty($edit_sadmins_list)) {
            $edit_sadmins_list = $tpl['VCL']['edit_sadmins_slist_empty'];
        }

            $body = str_replace(
                array('%audit_list%','%unlinked_audit_list%','%stages_list%','%unlinked_stages_list%', '%view_sadmins_list%','%edit_sadmins_list%', '%stream_name%','%stream_dt%','%opened%','%opened_text%','%opened_check%'),
                array($audit_list,$unlinked_audit_list,$stages_list,$unlinked_stages_list,$view_sadmins_list,$edit_sadmins_list,$StreamParams['StreamName'],$StreamParams['StreamDT'],$StreamParams['StreamOpened'],$StreamParams['StreamOpened'] ? 'Смена открыта' : 'Смена закрыта',$StreamParams['StreamOpened'] ? 'checked' : ''),
                $tpl['VCL']['InsideStream']
            );
            break;
        }
        default:
        {    // Список смен
            $stream_list = '';
            $res = $mysqli->query('SELECT s.id as SId, s.name AS StreamName, s.dt as dt, s.opened as opened, GROUP_CONCAT(u.fio SEPARATOR ", ") as admins FROM olymp_streams as s left join olymp_stream_admins as sadm ON(sadm.IdStream=s.id) left join users as u ON(u.id=sadm.IdPerson) GROUP BY s.id, s.name, s.dt, s.opened ORDER BY s.opened desc,s.dt,s.name');
            while ($row = $res->fetch_assoc()) {
                $stream_list .= str_replace(array('%SId%','%StreamName%','%dt%','%opened%','%admins%'), array($row['SId'],$row['StreamName'],$row['dt'],$row['opened'] ? 'открыта' : 'закрыта', $row['admins'] ? $row['admins'] : $tpl['VCL']['not_assigned']), $tpl['VCL']['stream_list_line']);
            }

            $body = str_replace(
                array('%stream_list%'),
                array($stream_list),
                $tpl['VCL']['stream_list']
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



$tpl['VCL']['person_list'] = '
<style>
#personList {border-collapse: collapse;}
#personList td {border:1px solid blue; padding:10px;}
#personList .personFIO {font-weight:bold;}
img.identIcon {width:75px; height:100px; position:static;}
img.identPhoto {border:3px dashed green; display:none; position:fixed; top:10px; right:10px; max-width:800px;}
div#stageHeader {margin-bottom:10px;}
.vcl_warning {color:red; font-weight:bold;}
#groupList,#unplacedGroupList,.auditAdminsList {border-collapse: collapse;}
#groupList td,#unplacedGroupList td,.auditAdminsList td {border:1px solid blue; padding:10px;}
#placeGroups, #auditParams {border:1px dashed blue; background:#F9F9FF;}
#placeGroups form, #auditParams form  {margin:10px; display:none;}
#placeGroupsHeader,#auditParamsHeader {background:darkblue; color:white; font-weight:bold; padding:8px;}
#placeGroupsHeader span,#auditParamsHeader span {color:white; font-weight:bold; padding:8px 0px;}
#removeGroupsForm,#dismissAdminsForm {margin-bottom:20px;}
.auditAdminsList {margin-top:10px;}
.groupCheck, .adminCheck, .transCheck, .restoreCheck {zoom:2;}
</style>
<script>
var GroupFormShow = 0;
var AuditParamsFormShow = 0;
$(function() {

//$("#placeGroups form").hide(100);

$(".identIcon").hover(
function () {
    $("#"+$(this).attr("id")+"Big").show();
    //alert($(this).attr("id")+"Big");
  },
function () {
    $("#"+$(this).attr("id")+"Big").hide();
  }
);

$("#placeGroupsHeader").click(
function()
{    if (GroupFormShow==0)
    {    GroupFormShow=1;
        $("#placeGroups form").show(600);
        $("#placeGroupsHeader span").text = "[-]";
    }
    else
    {    GroupFormShow=0;
        $("#placeGroups form").hide(600);
        $("#placeGroupsHeader span").text = "[+]";
    }
}
);

$("#auditParamsHeader").click(
function()
{    if (AuditParamsFormShow==0)
    {    AuditParamsFormShow=1;
        $("#auditParams form").show(600);
        $("#auditParamsHeader span").text = "[-]";
    }
    else
    {    AuditParamsFormShow=0;
        $("#auditParams form").hide(600);
        $("#auditParamsHeader span").text = "[+]";
    }
}
);

$("#vclink,#avclink").focus(
function()
{    if ($(this).val() == "--- не задана ---")
    {    $(this).val("");
    }
}
);


window.setTimeout(function(){location.reload();}, 300000);

}); 

</script>
<h1>%CurrentAuditName%</h1>
%error%<br><a href="?cmd=leaveAudit"><<= К списку комнат</a><br><br>
<h2>Параметры комнаты</h2>
Ссылка на ВКС для участника: %vclink%<br><br>
Ссылка на ВКС для администратора: %avclink%<br><br>

<form id="dismissAdminsForm" action="?" method="post">
<table class="auditAdminsList"><tr class="tblHeader"><td>ФИО</td><td>Другие комнаты</td><td><button type=submit name="DismissAdmins">Удалить всех администраторов</button></td></tr>%admin_list%</table>
</form>
<div id="auditParams">
<div id="auditParamsHeader"><span>[+]</span> Изменить параметры комнаты</div>
<form id="auditParamsForm" action="" method="post">
<div>Ссылка на ВКС для участника: <input type=text name="vclink" value="%vclink%" id="vclink" size=80></div>
<div>Ссылка на ВКС для администратора: <input type=text name="avclink" value="%avclink%" id="avclink" size=80></div>
<table class="auditAdminsList"><tr class="tblHeader"><td>ФИО</td><td>Другие комнаты</td><td>Выбрать администратора</td></tr>%unassigned_admins_list%</table><br><br>
<button type=submit name="btnApplyParams">Изменить ссылку и назначить отмеченных администраторов в эту комнату</button>
</form>
</div>

<h2>Группы в этой комнате</h2>
<form id="removeGroupsForm" action="?" method="post">
<table id="groupList"><tr class="tblHeader"><td>Группа</td><td>Участников</td><td>Классы</td><td><button type=submit name="removeGroups">Удалить все группы</button></td></tr>%group_list%</table>
</form>
<div id="placeGroups">
<div id="placeGroupsHeader"><span>[+]</span> Добавление групп</div>
<form id="placeGroupsForm" action="" method="post">
<table id="unplacedGroupList"><tr class="tblHeader"><td>Группа</td><td>Участников</td><td>Классы</td><td>Выбрать группы</td></tr>%unplaced_group_list%</table><br><br>
<button type=submit name="btnPlaceGroups">Добавить отмеченные группы в эту комнату</button>
</form>
</div>
<h2>Участники в этой комнате</h2>
<form action="?" method=post>
<button type=submit name="btnTransAudit">Пересадить выбранных участников в комнату:</button>&nbsp;&nbsp;&nbsp;<select name="transToAudit">%transAuditList%</select><br><br>
<button type=submit name="btnRestoreAudit">Вернуть выбранных участников в комнату их группы</button><br><br>
<table id="personList"><tr class="tblHeader"><td>Фото</td><td>ФИО</td><td>Класс</td><td>Пароль</td><td>Статус</td><td class="restoreAudit">Вернуть</td><td  class="transAudit">Пересадить</td></tr>%person_list%</table>
</form>
';

$tpl['VCL']['person_list_line'] = '<tr><td><img src="/ajax/olimp_get_photo.php?IdPerson=%IdPerson%" class="identIcon" id="identPhoto%IdPerson%"><img src="/ajax/olimp_get_photo.php?IdPerson=%IdPerson%" class="identPhoto" id="identPhoto%IdPerson%Big"></td><td><div class="personFIO">%fio% </div>id:%IdPerson% <br>%tel% <br>%email%</td><td>%school_class%</td><td>%varPwd%</td><td>%presence%</td><td  class="restoreAudit">&nbsp;</td><td class="transAudit"><input type="checkbox" name="transAudit[%IdPerson%]" class="transCheck" value=1></td></tr>';

$tpl['VCL']['person_list_line_trans'] = '<tr><td><img src="/ajax/olimp_get_photo.php?IdPerson=%IdPerson%" class="identIcon" id="identPhoto%IdPerson%"><img src="/ajax/olimp_get_photo.php?IdPerson=%IdPerson%" class="identPhoto" id="identPhoto%IdPerson%Big"></td><td><div class="personFIO">%fio% </div>id:%IdPerson% <br>%tel% <br>%email%</td><td>%school_class%</td><td>%varPwd%</td><td>%presence%</td><td  class="restoreAudit"><input type="checkbox" name="restoreAudit[%IdPerson%]" class="restoreCheck" value=1></td><td class="transAudit"><input type="checkbox" name="transAudit[%IdPerson%]" class="transCheck" value=1></td></tr>';


$tpl['VCL']['group_list_line'] = '<tr><td>%GroupName%</td><td>%cnt%</td><td>%classes%</td><td><button type=submit name="removeGroup[%IdGroup%]">Удалить группу из комнаты</button> <input type="hidden" name="IdGroup[%IdGroup%]" value=1></td></tr>';
$tpl['VCL']['unplaced_group_list_line'] = '<tr><td>%GroupName%</td><td>%cnt%</td><td>%classes%</td><td><input type="checkbox" name="IdGroup[%IdGroup%]" class="groupCheck" value=1></td></tr>';

$tpl['VCL']['admins_list_line'] = '<tr><td>%AdminName%</td><td>%OtherAudits%</td><td><button type=submit name="DismissAdmin[%IdPerson%]">Удалить администратора из комнаты</button> <input type="hidden" name="IdAdmin[%IdPerson%]" value=1></td></tr>';
$tpl['VCL']['unassigned_admins_list_line'] = '<tr><td>%AdminName%</td><td>%OtherAudits%</td><td><input type="checkbox" name="IdAdmin[%IdPerson%]" class="adminCheck" value=1></td></tr>';

$tpl['VCL']['admins_list_empty'] = '<tr><td colspan=3>Администраторы в эту комнату не назначены</td></tr>';
$tpl['VCL']['unassigned_admins_list_empty'] = '<tr><td colspan=3>Нет доступных администраторов</td></tr>';

// - InsideStream

$tpl['VCL']['InsideStream'] = '
<style>
.notAssigned {font-weight:bold; color:red;}
#auditList,#unlinkedAuditList,#stageList,#unlinkedStageList,.SAdminsList {border-collapse: collapse;}
#auditList td, #unlinkedAuditList td, #stageList td,#unlinkedStageList td,.SAdminsList td {border:1px solid blue; padding:10px;}
#stageList {margin-top:10px;}
.tblHeader td {color: white; background:darkblue; }
.stageCheck, .auditCheck, .adminCheck {zoom:2;}
.viewStage,.viewStageA,.viewStageP {}
.editStage, .editStageA, .editStageP {display:none;}
#viewSwitch, #editSwitch, #viewSwitchA, #editSwitchA, #viewSwitchP, #editSwitchP {padding: 10px 4px; border:1px solid gray; cursor:hand; color:blue;}
#viewSwitch, #viewSwitchA, #viewSwitchP {display:none;}
.buttonBar {margin:10px 0px 20px 0px}
</style>

<script>

$(function() {

$("#viewSwitch").click(
function()
{    $(".viewStage").show(600);
    $(".editStage").hide(600);
    $("#viewSwitch").hide();
    $("#editSwitch").show();
}
);

$("#editSwitch").click(
function()
{    $(".viewStage").hide(600);
    $(".editStage").show(600);
    $("#viewSwitch").show();
    $("#editSwitch").hide();
}
);

$("#viewSwitchA").click(
function()
{    $(".viewStageA").show(600);
    $(".editStageA").hide(600);
    $("#viewSwitchA").hide();
    $("#editSwitchA").show();
}
);

$("#editSwitchA").click(
function()
{    $(".viewStageA").hide(600);
    $(".editStageA").show(600);
    $("#viewSwitchA").show();
    $("#editSwitchA").hide();
}
);

$("#viewSwitchP").click(
function()
{    $(".viewStageP").show(600);
    $(".editStageP").hide(600);
    $("#viewSwitchP").hide();
    $("#editSwitchP").show();
}
);

$("#editSwitchP").click(
function()
{    $(".viewStageP").hide(600);
    $(".editStageP").show(600);
    $("#viewSwitchP").show();
    $("#editSwitchP").hide();
}
);

}); 

</script>
<h1>Параметры смены</h1>
%error%<br><a href="?cmd=leaveStream"><<= К списку смен</a><br><br>
<form action="?" method=post>
<div class="buttonBar">
<span id="viewSwitchP">Вернуться к просмотру</span><span id="editSwitchP">Перейти к изменению</span> <span class="editStageP">&nbsp;&nbsp;<button type=submit name="btnApplyStreamParams">Изменить параметры смены</button></span>
</div>
<div class="viewStageP">
Название смены: %stream_name%<br>
Плановая дата: %stream_dt%<br>
%opened_text%<br>
Администраторы:
<table class="SAdminsList"><tr class="tblHeader"><td>ФИО</td></tr>%view_sadmins_list%</table><br><br>
</div>

<div class="editStageP">
Название смены: <input type="text" name="StreamName" value="%stream_name%" size=80><br>
Плановая дата: <input type="text" name="StreamDT" value="%stream_dt%" size=20><br>
<input type="checkbox" name="StreamOpened" class="auditCheck" value=1 id="StreamOpened" %opened_check%><label for="StreamOpened"> Смена открыта</label><br>
Администраторы:
<table class="SAdminsList"><tr class="tblHeader"><td>ФИО</td><td>Выбрать администратора</td></tr>%edit_sadmins_list%</table><br><br>
</div>
</form>

<h2>Список комнат</h2>
<form action="?" method=post>
<div class="buttonBar">
<span id="viewSwitchA">Вернуться к просмотру</span><span id="editSwitchA">Перейти к изменению</span> <span class="editStageA">&nbsp;&nbsp;<button type=submit name="btnReleaseAudits">Освободить выбранные комнаты</button></span>
</div>
<table id="auditList"><tr class="tblHeader"><td>Комната</td><td>Ссылка</td><td>Наполненность</td><td>Администраторы</td><td class="editStageA">Выбрать</td></tr>%audit_list%</table>

<h2 class="editStageA">Неиспользованные комнаты &nbsp;&nbsp;<span ><button type=submit name="btnLinkAudits" class="editStageA">Добавить выбранные комнаты</button></span></h2>
<table id="unlinkedAuditList" class="editStageA"><tr class="tblHeader"><td>Комната</td><td>Вместимость</td><td class="editStageA">Выбрать</td></tr>%unlinked_audit_list%</table>

</form>

<h2>Список потоков</h2>
<form action="?" method=post>
<div class="buttonBar">
<span id="viewSwitch">Вернуться к просмотру</span><span id="editSwitch">Перейти к изменению</span> <span class="editStage"><button type=submit name="btnChangePwd">Изменить пароли</button>&nbsp;&nbsp;<button type=submit name="btnRemoveStages">Удалить выбранные потоки</button></span>
</div>
<table id="stageList"><tr class="tblHeader"><td>Предмет</td><td>Классы</td><td>Дата и время проведения</td><td>Пароль на варианты</td><td class="editStage">Выбрать</td></tr>%stages_list%</table>

<h2 class="editStage">Непривязанные потоки &nbsp;&nbsp;<span ><button type=submit name="btnLinkStages">Привязать выбранные потоки</button></span></h2>
<table id="unlinkedStageList" class="editStage"><tr class="tblHeader"><td>Предмет</td><td>Классы</td><td>Дата и время проведения</td><td class="editStage">Выбрать</td></tr>%unlinked_stages_list%</table>

</form>
';

$tpl['VCL']['audit_list_line'] = '<tr><td><a href="?cmd=enterAudit&SAId=%SAId%">%AuditName%</a></td><td>%vclink%</td><td>%cnt%</td><td>%admins%</td><td class="editStageA"><input type="checkbox" name="IdAudit[%IdAudit%]" class="auditCheck" value=1></tr>';

$tpl['VCL']['unlinked_audit_list_line'] = '<tr><td>%AuditName%</td><td>%cnt%</td><td><input type="checkbox" name="IdAudit[%IdAudit%]" class="auditCheck" value=1></tr>';

$tpl['VCL']['stages_list_line'] = '<tr><td>%ExamName%</td><td>%classes%</td><td>%examDate%</td><td><span class="viewStage">%varPwd%</span><input type=text size=40 class="editStage" name="changeVarPwd[%IdStage%]" value="%varPwd%"></td><td class="editStage"><input type="checkbox" name="IdStage[%IdStage%]" class="stageCheck" value=1></td></tr>';

$tpl['VCL']['unlinked_stages_list_line'] = '<tr><td>%ExamName%</td><td>%classes%</td><td>%examDate%</td><td><input type="checkbox" name="IdStage[%IdStage%]" class="stageCheck" value=1></td></tr>';

$tpl['VCL']['empty_stages_list'] = '<tr><td colspan=5>Нет потоков, привязанных к этой смене</td></tr>';
$tpl['VCL']['empty_unlinked_stages_list'] = '<tr><td colspan=5>Нет непривязанных потоков</td></tr>';

$tpl['VCL']['empty_audit_list'] = '<tr><td colspan=5>Нет аудиторий, привязанных к этой смене</td></tr>';
$tpl['VCL']['empty_unlinked_audit_list'] = '<tr><td colspan=5>Нет доступных аудиторий</td></tr>';

$tpl['VCL']['view_sadmins_list_line'] = '<tr><td>%AdminName%</td></tr>';
$tpl['VCL']['edit_sadmins_list_line'] = '<tr><td>%AdminName%</td><td><input type="checkbox" name="IdAdmin[%IdPerson%]" class="adminCheck" value=1 %check%></td></tr>';

$tpl['VCL']['view_sadmins_list_empty'] = '<tr><td>Администраторы в эту смену не назначены</td></tr>';
$tpl['VCL']['edit_sadmins_list_empty'] = '<tr><td colspan=2>Нет доступных администраторов</td></tr>';

// ---------------

$tpl['VCL']['not_assigned'] = '<span class="notAssigned">не назначены!</span>';

$tpl['VCL']['stream_list'] = '
<style>
.notAssigned {font-weight:bold; color:red;}
#streamList {border-collapse: collapse;}
#streamList td {border:1px solid blue; padding:10px;}
.tblHeader td {color: white; background:darkblue; }
</style>
<h1>Смены</h1>
%error%<br><table id="streamList"><tr class="tblHeader"><td>Смена</td><td>Плановая дата</td><td>Состояние</td><td>Администраторы</td></tr>%stream_list%</table>';

$tpl['VCL']['stream_list_line'] = '<tr><td><a href="?cmd=enterStream&SId=%SId%">%StreamName%</a></td><td>%dt%</td><td>%opened%</td><td>%admins%</td></tr>';


$body = build_page();
