<?
if (($role & 32)==0)
{    return $tpl['noaccess'];
}

$ids = array();
foreach($_GET['ids'] as $id){
    $ids[] = $id;
}

if(!isset($_POST['sent'])){
    $body = '<form method="post"><input type="hidden" name="sent" value=1>';
    $body .= 'Выберите площадку: <select name="newId">';
    $ids = implode(',', $ids);
    
    $query = "select a.id, b.name from olimp_stages as a left join olimp_places as b on(a.place = b.id) where a.id in ($ids) group by a.place";
    $res = $mysqli->query($query);
    while($row = $res->fetch_assoc()){
        $body .= '<option value="'.$row['id'].'">'.$row['name'].'</option>';
    }
    $body .= '</select><br /><br /><button>Слить потоки</button></form>';
}

if($_POST['sent']){
    $newId = (int)$_POST['newId'];
    if($newId and is_array($ids) and count($ids) > 1){
        unset($ids[array_search($newId, $ids)]);
        $ids = implode(',', $ids);
        
        $query = "update olimp_actions set stage_id=$newId where stage_id in ($ids)";
        $mysqli->query($query);
        
        $query = "delete from olimp_stages where id in ($ids)";
        $mysqli->query($query);
    }
    
    header('location:/olimp_stages.html');
    exit;
}