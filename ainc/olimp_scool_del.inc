<?php

if (($role & 32) == 0) {
    return $tpl['noaccess'];
}

if (!$id = (int)$_GET['id']) {
    header('location:/olimp_scool_list.html');
    exit;
}

$query = "delete from olimp_places where id = $id";
$mysqli->query($query);
header('location:' . $_SERVER['HTTP_REFERER']);
exit;
