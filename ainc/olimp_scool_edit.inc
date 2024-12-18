<?php

if (($role & 32) == 0) {
    return $tpl['noaccess'];
}

if (!$id = (int)$_GET['id']) {
    header('location:/olimp_scool_list.html');
    exit;
}

$query = "select * from olimp_places where id = $id";
$res = $mysqli->query($query);
if (!$myscool = $res->fetch_assoc()) {
    header('location:/olimp_scool_list.html');
    exit;
}

if (isset($_REQUEST['btnCancel'])) {
    $cmd = '';
}


if (isset($_POST['sent'])) {
    foreach ($_POST as $key => $value) {
        $$key = addslashes($value);
    }

    $query = "update olimp_places set `abbr`='$abbr', `name`='$name', `address`='$address', `org_id`='$org_id' where id = $id";
    $mysqli->query($query);
    header('location:/olimp_scool_list.html');
    exit;
}

$body = '
<form method="post" id="newPotok">
    <input type="hidden" name="sent" value="1">
    

    <div class="line">
        <label>Аббревиатура:</label>
        <input type="text" name="abbr" value="' . $myscool['abbr'] . '">
    </div>
    
    <div class="line">
        <label>Название:</label>
        <input type="text" name="name" value="' . $myscool['name'] . '">
    </div>
    
    <div class="line">
        <label>Адрес:</label>
        <textarea name="address">' . $myscool['address'] . '</textarea>
    </div>
    
    <div class="line">
        <label>org_id:</label>
        <input type="text" name="org_id" value="' . $myscool['org_id'] . '">
    </div>
    
    <div class="line">
        <label></label>
        <button>Сохранить</button> 
    </div>


';
