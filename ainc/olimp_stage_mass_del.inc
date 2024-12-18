<?
if (($role & 32)==0)
{    return $tpl['noaccess'];
}
    $ids = array();
    foreach($_GET['ids'] as $id){
        $ids[] = $id;
    }
    $ids = implode(',', $ids);
    
    $query = "delete from olimp_stages where id in ($ids)";
    $mysqli->query($query);
    
    header('location:'.$_SERVER['HTTP_REFERER']);
    exit;