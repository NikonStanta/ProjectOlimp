<?php

// Перерегистрация участника на следующий год.

//$mysqli = new mysqli('localhost', 'ehope_sql','Vk@FJHCd-y(IyJ)7M6rQ','ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
if ($USER_INFO['year'] != $SITE_VARS['current_year']) {
    $reg_class = $USER_INFO['class'] + $SITE_VARS['current_year'] - $USER_INFO['year'];
    if ($reg_class > 11) {
        $reg_class = 20;
    }
    if ($reg_class != 20) {
//        if ($mysqli->query('insert into _reg_nums (reg_num) select max(reg_num)+1 from _reg_nums'))
//        {    $regnum=$mysqli->insert_id.'-'.$SITE_VARS['reg_num_postfix'];
//        }
//        else{print_r('FALSEE');}
        $res = $mysqli->query('select max(reg_num) from _reg_nums');
        $row = $res->fetch_row();
        $regnum = $row[0] + 1;
        $mysqli->query("insert into _reg_nums (reg_num) $regnum");
        $regnum = $regnum . '-' . $SITE_VARS['reg_num_postfix'];
        $mysqli->query('update olimp_persons set olymp_year=' . $SITE_VARS['current_year'] . ', school_class=' . $reg_class . ', reg_num="' . $regnum . '", confirmed_person="N", confirmed_school="N", confirmed_pass="N", confirmed_permission="N", ready="N", printed_z="N", printed_p="N", IdentDoc="Not", IdentPhoto="Not", SchoolDoc="Not", reg_date=Now() where id=' . $USER_INFO['pid']);
        $message = str_replace('%regnum%', $regnum, $tpl['cabinet']['rereg_modal_message']);
        $USER_INFO['year'] = $SITE_VARS['current_year'];
    } else {
        $mysqli->query('update olimp_persons set  olymp_year=' . $SITE_VARS['current_year'] . ', school_class=20 where id=' . $USER_INFO['pid']);
        $message = $tpl['cabinet']['rereg_modal_message_no'];
    }
    //mysql_query('delete from olimp_actions where person_id='.$USER_INFO['pid']);
}
