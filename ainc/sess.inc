<?php

session_start();

$uid = 0;
$user = '';
$role = 0;

if (array_key_exists('uid', $_SESSION) && $_SESSION['uid'] > 0) {
    $res2 = $mysqli->query('select fio,nic,role,IdOrg from users where id=' . $_SESSION['uid']);
    if ($row2 = $res2->fetch_row()) {
        $uid = $_SESSION['uid'];
        $user = $row2[0];
        $nic = $row2[1];
        $role = $row2[2];
        $user_IdOrg = $row2[3];
    } else {
        $uid = 0;
        $user = '';
        $role = 0;
        $nic = '';
        $user_IdOrg = 0;
    }
}

$logged_onoff = false;
#'select id,fio,role,IdOrg from users where nic="'.$mysqli->real_escape_string($_REQUEST['nic']).'" AND pwd=PASSWORD("'.$mysqli->real_escape_string($_REQUEST['pwd']).'")'
#'select id,fio,role,IdOrg from users where nic="'.$mysqli->real_escape_string($_REQUEST['nic']).'"'
if (!empty($_POST['btnLogon']) || (array_key_exists('cmd', $_REQUEST) && ($_REQUEST['cmd'] == 'logon'))) {
    $res = $mysqli->query(
        'select id,fio,role,IdOrg from users where nic="' . $mysqli->real_escape_string($_REQUEST['nic']) . '" AND (pwd=SHA1("' . $mysqli->real_escape_string($_REQUEST['pwd']) . '"))'
    ); //AND pwd=PASSWORD("'.$mysqli->real_escape_string($_REQUEST['pwd']).'")
    if ($res == false) {
        print_r(11111);
    } elseif ($res == null) {
        print_r(2222);
    }
    print_r($res);
    if ($row = $res->fetch_row()) {
        $uid = $row[0];
        $user = $row[1];
        $role = $row[2];
        $user_IdOrg = $row[3];
        $logged_onoff = true;
    }
}

if (!empty($_POST['btnLogout']) || (array_key_exists('cmd', $_REQUEST) && ($_REQUEST['cmd'] == 'logout'))) {
    $uid = 0;
    $user = '';
    $role = 0;
    $nic = '';
    $logged_onoff = true;
    $user_IdOrg = 0;
}

$_SESSION['uid'] = $uid;
