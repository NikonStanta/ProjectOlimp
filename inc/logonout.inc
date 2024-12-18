<?php

$logonout = '';
if (isset($_POST['btnLogon']) || isset($_POST['btnLogon_x'])) {
    $logonout = 'logon';
}
if (isset($_POST['btnLogout']) || isset($_POST['btnLogout_x'])) {
    $logonout = 'logout';
}
if (empty($logonout) && isset($_REQUEST['cmd']) && ($_REQUEST['cmd'] == 'logon')) {
    $logonout = 'logon';
}
if (empty($logonout) && isset($_REQUEST['cmd']) && ($_REQUEST['cmd'] == 'logout')) {
    $logonout = 'logout';
}
switch ($logonout) {
    case 'logon':
    {    $nic = trim($_REQUEST['logon_name']);
        $pwd = $mysqli->real_escape_string(trim($_REQUEST['logon_pwd']));      // !!
        $role = $_REQUEST['logon_status'] + 0;

        $res = $mysqli->query('select id,locked,role from olimp_users  where (nic="' . $nic . '") AND (pwd=UNHEX(SHA("' . $pwd . '"))) AND (FIND_IN_SET(' . $role . ',role)>0)');
        if ($row = $res->fetch_row()) {
            $_SESSION['uid'] = $row[0];
            $_SESSION['role'] = $role;
            $_SESSION['roles'] = $row[2];
            $mysqli->query('update olimp_users set logs_success=logs_success+1,logs_attempt=0, logs_dt=Now() where id=' . $_SESSION['uid']);
        } else {
/*
 if ($nic==KirillovaKS)
{$_SESSION['uid']=45769;
$_SESSION['role']=1;
$_SESSION['roles']=1;
}
*/
        }
        break;
    }
    case 'logout':
    {    $_SESSION['uid'] = 0;
        $_SESSION['role'] = 0;
        $_SESSION['roles'] = '0';

        if (($_SESSION['master_hash'] == $_REQUEST['master_hash'])) {
            $res = $mysqli->query('select role from olimp_users  where (id=' . ($_SESSION['master_uid'] + 0) . ') AND (FIND_IN_SET(' . ($_SESSION['master_role'] + 0) . ',role)>0)');
            if ($row = $res->fetch_row()) {
                $role = $_SESSION['master_role'];
                $_SESSION['uid'] = $_SESSION['master_uid'];
                $_SESSION['role'] = $role;
                $_SESSION['roles'] = $row[0];
            }
        }
        $_SESSION['master_uid'] = 0;
        $_SESSION['master_hash'] = '';
        $_SESSION['master_role'] = 0;
        break;
    }
}

if (!empty($_REQUEST['show_roles'])) {
    echo $_SESSION['roles'];
}

if ((!empty($_REQUEST['set_uid'])) && (strpos($_SESSION['roles'], '4') !== false)) {
    $_SESSION['master_uid'] = $_SESSION['uid'];
    $_SESSION['master_role'] = $_SESSION['role'];
    $_SESSION['master_hash'] = md5(microtime());
    if ($_REQUEST['set_uid'] > 0) {
        $_SESSION['uid'] = $_REQUEST['set_uid'];
        $role = 1;
        $_SESSION['role'] = 1;
        $_SESSION['roles'] = '1';
    } elseif ($_REQUEST['set_uid'] < 0) {
        $res = $mysqli->query('select uid from olimp_persons where id=' . (0 - $_REQUEST['set_uid']) . '');
        if ($row = $res->fetch_row()) {
            $_SESSION['uid'] = $row[0];
            $role = 1;
            $_SESSION['role'] = 1;
            $_SESSION['roles'] = '1';
        }
    } else {
        $res = $mysqli->query('select id from olimp_users where nic="' . trim($_REQUEST['set_uid']) . '"');
        if ($row = $res->fetch_row()) {
            $_SESSION['uid'] = $row[0];
            $role = 1;
            $_SESSION['role'] = 1;
            $_SESSION['roles'] = '1';
        }
    }
}


if (array_key_exists('uid', $_SESSION) && $_SESSION['uid'] > 0) {
    //if ($_SESSION['debug']>0) echo 'select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.role from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where u.id='.$_SESSION['uid'];

# AND (FIND_IN_SET('.$_SESSION['role'].',role)>0)
# select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.nic="DanshinRA") AND (FIND_IN_SET(1,role)>0)
# $res=$mysqli->query('select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.nic="DanshinRA") AND (FIND_IN_SET(1,role)>0)');
    $res = $mysqli->query('select u.locked,concat_ws(" ",p.surename,p.name,p.patronymic),p.id,p.school_class,p.olymp_year,p.printed_z,u.id from olimp_users as u left join olimp_persons as p on (u.id=p.uid) where (u.id="' . $_SESSION['uid'] . '" OR u.nic="' . $_SESSION['uid'] . '") AND (FIND_IN_SET(' . $_SESSION['role'] . ',role)>0)');

    if ($row = $res->fetch_row()) {
        $USER_INFO['uid'] = $row[6];
        $USER_INFO['fio'] = $row[1];
        $USER_INFO['role'] = $_SESSION['role'];
        $USER_INFO['roles'] = $_SESSION['roles'];
        $USER_INFO['pid'] = $row[2];
        $USER_INFO['class'] = $row[3];
        $USER_INFO['year'] = $row[4];
        $USER_INFO['printed_z'] = $row[5];
    } else {
        $_SESSION['uid'] = -1;
    }
} elseif (!array_key_exists('uid', $_SESSION)) {
    $_SESSION['uid'] = -1;   //Не было
}
if ($_SESSION['uid'] < 1) {
    $USER_INFO['uid'] = 0;
    $USER_INFO['fio'] = '';
    $USER_INFO['role'] = 0;
    $USER_INFO['roles'] = 0;
    $USER_INFO['pid'] = 0;
    $USER_INFO['class'] = 0;
    $USER_INFO['year'] = 0;
    $USER_INFO['printed_z'] = 'N';
}
