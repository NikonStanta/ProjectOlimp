<?php

$mysqli->query('delete from sess where dt < "' . date("Y-m-d h:m:s", time() - 3600) . '"');

$cook_sid = $_COOKIE['cook_sid'];
$usid = $_GET['usid'];
if (empty($usid)) {
    $usid = $_POST['usid'];
}

function new_sess($uid = 0)
{
    $b1 = uniqid('pd', true);
    $b2 = 0;
    do {
        $b2++;
        $s = md5("$b2--$b1");
    } while (!$mysqli->query("insert into sess (id,dt,uid) values ('$s',now(),$uid)"));
    setcookie('cook_sid', $s);
    return $s;
}

$use_cookie = false;
if (isset($cook_sid)) {
    $sid = $cook_sid;
    $use_cookie = true;
} else {
    if (isset($usid)) {
        $sid = $usid;
    } else {
        $sid = new_sess();
    }
}

$uid = 0;
$user = '';
$role = 0;

$res = $mysqli->query("select uid from sess where id='$sid'");
if (mysql_num_rows($res) > 0) {
    $row = $res->fetch_row();
    if (!empty($row[0])) {
        $res2 = $mysqli->query('select fio,nic,role from users where id=' . $row[0]);
        if (mysql_num_rows($res2) > 0) {
            $uid = $row[0];
            $row2 = $res2->fetch_row();
            $user = $row2[0];
            $nic = $row2[1];
            $role = $row2[2];
        }
    }
} else {
    $sid = new_sess();
    $use_cookie = false;
}

$logged_onoff = false;


if (($_POST['btnLogon']) || ($_REQUEST['cmd'] == 'logon')) {
    $res = $mysqli->query('select id,fio,role from users where (nic="' . $_REQUEST['nic'] . '") AND (pwd=PASSWORD("' . $_REQUEST['pwd'] . '"))');
    if ($row = $res->fetch_row()) {
        $uid = $row[0];
        $user = $row[1];
        $role = $row[2];
        $logged_onoff = true;
    }
}

if (($_POST['btnLogout']) || ($_REQUEST['cmd'] == 'logout')) {
    $uid = 0;
    $user = '';
    $role = 0;
    $nic = '';
    $logged_onoff = true;
}

if (!$use_cookie) {
    $refpar = "?usid=$sid";
    $formrefpar = "<input type=hidden name=usid value=$sid>";
} else {
    $refpar = "?";
    $formrefpar = "";
}

$mysqli->query("update sess set uid=$uid,dt=now() where id='$sid'");
