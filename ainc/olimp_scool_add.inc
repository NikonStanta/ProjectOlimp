<?php

if (($role & 32) == 0) {
    return $tpl['noaccess'];
}

if (isset($_POST['sent'])) {
    foreach ($_POST as $key => $value) {
        $$key = addslashes($value);
    }

    $query = "insert into olimp_places(`abbr`, `name`, `address`, `org_id`)
                values('$abbr', '$name', '$address', '$org_id')";

    $mysqli->query($query);

    header('location:/olimp_scool_list.html');
    exit;
}




$body = '
<form method="post" id="newPotok">
    <input type="hidden" name="sent" value="1">
    

    <div class="line">
        <label>Абревиатура:</label>
        <input type="text" name="abbr">
    </div>
    
    <div class="line">
        <label>Название:</label>
        <input type="text" name="name">
    </div>
    
    <div class="line">
        <label>Адрес:</label>
        <textarea name="address"></textarea>
    </div>
    
    <div class="line">
        <label>org_id:</label>
        <input type="text" name="org_id">
    </div>
    
    <div class="line">
        <label></label>
        <button>Сохранить</button>
    </div>

   



</form>
';
