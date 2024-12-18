<?php
function is_mobile_device($str) : bool {
    $res = false;
    if(preg_match('%Mozilla%', $str) && preg_match('%Mobile Safari%', $str)){
        $res = true;
    }
    return $res;
}
?>