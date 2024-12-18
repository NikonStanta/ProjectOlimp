<?php

//    Типы элементов меню
//    0    -    Текст                            [-]
//    1    -    Внешняя ссылка                    [txt = href]
//    2    -    Другое меню по имени            [txt = menu name]
//    3    -    Страница                        [hid = page id, txt - params]
//    4    -    Страница раздела по умолчанию    [hid = div id, txt - params]
//    5    -    Список разделов    видимых            [-]
//    6    -    Список разделов    всех            [-]
//    7    -    Страницы раздела видимые        [hid = div id]
//    8    -    Страницы раздела все            [hid = div id]
//    9    -    Для видимых разделов меню        [txt = menu name]
//    10    -    Для всех разделов меню            [txt = menu name]
//    11    -    Список групп                    [hid = div id]
//    12    -    Страницы группы видимые            [hid = group id]
//    13    -    Страницы группы все                [hid = group id]
//    14    -    Для всех групп раздела меню        [hid = div id, txt = menu name]
//
//    $tplex[<имя меню>]
//
//
//  text# text#c
//  ref#  ref#c
//  item# item#c
//

function get_menu($did, $name, $tpl, $tplex = '', $agid = 0)
{
    global $PAGE,$SITE_DIVS,$SITE_DIVPREFIX,$SITE_VARS, $mysqli;
    $mdid = $PAGE['did'];
    $res = $mysqli->query("select id from site_menu where ((did=$mdid) OR (did=0)) AND (name='$name') order by did desc");
    $row = $res->fetch_row();
    $mid = $row[0] + 0;
    if (empty($mid)) {
        return preg_replace(array('/%menu%/'), array(''), $tpl['main']);
    }
    $setskip = false;
    $aftercurr = false;
    $res = $mysqli->query('select tp,title,level,hid,txt,ord,ext1,ext2 from menu_items where mid=' . $mid . ' order by ord');
    $itemCount = $res->num_rows;
    $itemNum = 0;
    $itemPos = 'f';
    $w = '';
    while ($row = $res->fetch_row()) {
        $itemNum++;
        if ($itemNum == $itemCount) {
            $itemPos = 'l';
        }
        $tp = $row[0];
        $title = $row[1];
        $level = $row[2];
        $hid = $row[3];
        $txt = $row[4];
        $ord = $row[5];
        $ext1 = $row[6];
        $ext2 = $row[7];
        if (($setskip) && ($itemPos != 'l') && isset($tpl)) {
            $w .= $tpl['skip'];
        } else {
            $setskip = true;
        }

        switch ($tp) {
            case '0':
            {    // Текст
                $w .= preg_replace('/%name%/', $title, ($PAGE['did'] == $did) ? $tpl['text' . $level . 'c'] : $tpl['text' . $level]);
                break;
            }
            case '1':
            {    // Внешняя ссылка
                $w .= preg_replace(
                    array('/%name%/','/%ref%/'),
                    array($title,$txt),
                    ($PAGE['did'] == $did) ? $tpl['ref' . $level . 'c'] : $tpl['ref' . $level]
                );
                    break;
            }
            case '2':
            {    // Другое меню по имени
                $w .= get_menu($did, $txt, $tplex[$txt], $tplex);
                break;
            }
            case '3':
            {    //    Страница
                $res2 = $mysqli->query("select did,name,title,ttlm from site_pages where id=$hid");
                $row2 = $res2->fetch_row();
                if (empty($row2)) {
                    break;
                }
                if (empty($title) || ($title == '<Новый пункт меню>')) {
                    $ttlm = empty($row2[3]) ? $row2[2] : $row2[3];
                } else {
                    $ttlm = $title;
                }

                $iscurr = ($PAGE['id'] == $hid);

                if (($setskip) && ($itemPos != 'f')) {
                    $w .= array_key_exists('skip_' . ($aftercurr ? 'c' : 'n') . ($iscurr ? 'c' : 'n'), $tpl) ? $tpl['skip_' . ($aftercurr ? 'c' : 'n') . ($iscurr ? 'c' : 'n')] : '';
                }

                $w .= ($PAGE['id'] == $hid) ? $tpl['befor_c' . $itemPos] : $tpl['befor_n' . $itemPos];
                /*print_r($row2);
                echo "<br>".$row2[0]."<br>";
                print_r($SITE_DIVPREFIX);
                echo "<br>";
                if (empty($row2[0])) {echo "<br>!!!!"; print_r($row2); echo "!!!!<br>";}
                if (empty($row2)) {echo "<br>!!!!!!!! ROW2 IS EMPTY!!!! <br>";}
                if ($SITE_DIVPREFIX[$row2[0]]){}*/
                $w .= preg_replace(
                    array('/%name%/','/%ref%/','/%param%/'),
                    array($ttlm,(array_key_exists($row2[0], $SITE_DIVPREFIX) && !empty($SITE_DIVPREFIX[$row2[0]]) ? '/' . $SITE_DIVPREFIX[$row2[0]] : '') . '/' . $row2[1] . $SITE_VARS['Default ext'],$txt),
                    ($PAGE['id'] == $hid) ? $tpl['item' . $level . 'c'] : $tpl['item' . $level]
                );
                    $w .= ($PAGE['id'] == $hid) ? $tpl['after_c' . $itemPos] : $tpl['after_n' . $itemPos];

                    $aftercurr = $iscurr;
                    break;
            }
            case '4':
            {    //    Страница раздела по умолчанию
                $res2 = $mysqli->query("select d.def_page,p.name,d.name from site_divs as d left join site_pages as p on (p.id=d.def_page) where d.id=$hid");
                $row2 = $res2->fetch_row();

                $iscurr = ($PAGE['id'] == $hid);

                if (($setskip) && ($itemPos != 'f') && isset($tpl)) {
                    $w .= array_key_exists('skip_' . ($aftercurr ? 'c' : 'n') . ($iscurr ? 'c' : 'n'), $tpl) ? $tpl['skip_' . ($aftercurr ? 'c' : 'n') . ($iscurr ? 'c' : 'n')] : '';
                }

                if (isset($tpl)) {
                    $w .= ($PAGE['did'] == $hid) ? $tpl['befor_c' . $itemPos] : $tpl['befor_n' . $itemPos];
                    $w .= preg_replace(
                        array('/%name%/', '/%ref%/', '/%param%/'),
                        array(empty($title) ? $row2[2] : $title,
                            (!empty($SITE_DIVPREFIX[$hid]) ? '/' : '') . $SITE_DIVPREFIX[$hid] .
                            '/' . $row2[1] . $SITE_VARS['Default ext'], $txt),
                        ($PAGE['did'] == $hid) ? $tpl['item' . $level . 'c'] : $tpl['item' . $level]
                    );
                    $w .= ($PAGE['did'] == $hid) ? $tpl['after_c' . $itemPos] : $tpl['after_n' . $itemPos];
                }

                $aftercurr = $iscurr;
                break;
            }
            case '5':
            {    // Список разделов видимых
                $res2 = $mysqli->query("select d.def_page,p.name,d.name,d.id from site_divs as d left join site_pages as p on (p.id=d.def_page) where d.menu1>0 order by d.ord");
                while ($row2 = $res2->fetch_row()) {
                    $w .= preg_replace(
                        array('/%name%/','/%ref%/'),
                        array($row2[2],(!empty($SITE_DIVPREFIX[$row2[3]]) ? '/' : '') . $SITE_DIVPREFIX[$row2[3]] .
                        '/' . $row2[1] . $SITE_VARS['Default ext']),
                        ($PAGE['id'] == $row2[0]) ? $tpl['item' . $level . 'c'] : $tpl['item' . $level]
                    );
                }
                break;
            }
            case '6':
            {    // Список разделов всех
                $res2 = $mysqli->query("select d.def_page,p.name,d.name,d.id from site_divs as d left join site_pages as p on (p.id=d.def_page) order by d.ord");
                while ($row2 = $res2->fetch_row()) {
                    $w .= preg_replace(
                        array('/%name%/','/%ref%/'),
                        array($row2[2],(!empty($SITE_DIVPREFIX[$row2[3]]) ? '/' : '') . $SITE_DIVPREFIX[$row2[3]] .
                        '/' . $row2[1] . $SITE_VARS['Default ext']),
                        ($PAGE['id'] == $row2[0]) ? $tpl['item' . $level . 'c'] : $tpl['item' . $level]
                    );
                }
                break;
            }
            case '7':
            {    // Страницы раздела видимые
                $cdid = ($hid < 1 ? ($did < 1 ? $PAGE['did'] : $did) : $hid);
                $res2 = $mysqli->query("select d.def_page,p.name,d.name from site_divs as d left join site_pages as p on (p.id=d.def_page) where (d.id=$cdid)");
                $row2 = $res2->fetch_row();
                $def_page_id = $row2[0];
                $def_page_name = $row2[1];
                $div_name = $row2[2];
                $res2 = $mysqli->query('select id,name,title,ttlm,locked from site_pages where (did=' . ($cdid) . ') and (menu1>0) and (clonefor=0) order by ord');
                $ww = '';
                while ($row2 = $res2->fetch_row()) {
                    $item_or_text = ($row2[4] + 0) ? 'hitem' : 'item';
                    if (isset($tpl)) {
                        $ww .= preg_replace(
                            array('/%name%/', '/%ref%/'),
                            array(empty($row2[3]) ? $row2[2] : $row2[3], (!empty($SITE_DIVPREFIX[$cdid]) ? '/' : '') . $SITE_DIVPREFIX[$cdid] .
                                '/' . $row2[1] . $SITE_VARS['Default ext']),
                            ($PAGE['id'] == $row2[0]) ?
                                $tpl[$item_or_text . $level . 'c'] :
                            $tpl[$item_or_text . $level]
                        );
                    }
                }
                if (isset($tpl)) {
                    $w .= preg_replace(
                        array('/%name%/','/%ref%/','/%menu%/'),
                        array($div_name,(!empty($SITE_DIVPREFIX[$hid]) ? '/' . $SITE_DIVPREFIX[$hid] : '') . '/' . $def_page_name .
                        $SITE_VARS['Default ext'],
                        $ww),
                        ($PAGE['did'] == $cdid) ? $tpl['div' . $level . 'c'] : $tpl['div' . $level]
                    );
                }
                break;
            }
            case '8':
            {    // Страницы раздела все
                $cdid = ($hid < 1 ? ($did < 1 ? $PAGE['did'] : $did) : $hid);
                $res2 = $mysqli->query("select d.def_page,p.name,d.name from site_divs as d left join site_pages as p on (p.id=d.def_page) where (d.id=$cdid)");
                $row2 = $res2->fetch_row();
                $def_page_id = $row2[0];
                $def_page_name = $row2[1];
                $div_name = $row2[3];
                $res2 = $mysqli->query('select id,name,title,locked from site_pages where (did=' . ($cdid) . ')   and (clonefor=0) order by ord');
                $ww = '';
                while ($row2 = $res2->fetch_row()) {
                    $item_or_text = $row2[3] ? 'hitem' : 'item';
                    $ww .= preg_replace(
                        array('/%name%/','/%ref%/'),
                        array($row2[2],(!empty($SITE_DIVPREFIX[$cdid]) ? '/' : '') . $SITE_DIVPREFIX[$cdid] .
                        '/' . $row2[1] . $SITE_VARS['Default ext']),
                        ($PAGE['id'] == $row2[0]) ? $tpl[$item_or_text . $level . 'c'] : $tpl[$item_or_text . $level]
                    );
                }
                $w .= preg_replace(
                    array('/%name%/','/%ref%/','/%menu%/'),
                    array($div_name,(!empty($SITE_DIVPREFIX[$hid]) ? '/' : '') . $SITE_DIVPREFIX[$hid] . '/' . $def_page_name .
                    $SITE_VARS['Default ext'],
                    $ww),
                    ($PAGE['did'] == $cdid) ? $tpl['div' . $level . 'c'] : $tpl['div' . $level]
                );
                    break;
            }
            case '9':
            {    // Для видимых разделов меню
                $res2 = $mysqli->query("select d.id,p.name,d.name from site_divs as d left join site_pages as p on (p.id=d.def_page) where (d.menu1>0) order by d.ord");
                while ($row2 = $res2->fetch_row()) {
                    $w .= preg_replace(
                        array('/%name%/','/%ref%/','/%menu%/'),
                        array($row2[2],(!empty($SITE_DIVPREFIX[$row2[0]]) ? '/' : '') . $SITE_DIVPREFIX[$row2[0]] . '/' . $row2[1] .
                        $SITE_VARS['Default ext'],get_menu($row2[0], $txt, $tplex[$txt], $tplex)),
                        ($PAGE['did'] == $row2[0]) ? $tpl['submenu' . $level . 'c'] : $tpl['submenu' . $level]
                    );
                }
                break;
            }
            case '10':
            {    // Для всех разделов меню
                $res2 = $mysqli->query("select d.id,p.name,d.name from site_divs as d left join site_pages as p on (p.id=d.def_page) order by d.ord");
                while ($row2 = $res2->fetch_row()) {
                    $w .= preg_replace(
                        array('/%name%/','/%ref%/','/%menu%/'),
                        array($row2[2],(!empty($SITE_DIVPREFIX[$row2[0]]) ? '/' : '') . $SITE_DIVPREFIX[$row2[0]] . '/' . $row2[1] .
                        $SITE_VARS['Default ext'],get_menu($row2[0], $txt, $tplex[$txt], $tplex)),
                        ($PAGE['did'] == $row2[0]) ? $tpl['submenu' . $level . 'c'] : $tpl['submenu' . $level]
                    );
                }
                break;
            }


            case '12':
            {    // Страницы группы видимые
                $gid = $hid < 1 ? $agid : $hid;
                $res2 = $mysqli->query("select name,did from site_pagegrp where id=$gid");
                $row2 = $res2->fetch_row();
                $gname = $row2[0];
                $cdid = $row2[1];
                $res2 = $mysqli->query('select id,name,title,ttlm from site_pages where (page_group=' . $gid . ') and (menu1>0)   and (clonefor=0) order by ord');
                $ww = '';
                while ($row2 = $res2->fetch_row()) {
                    $ww .= preg_replace(
                        array('/%name%/','/%ref%/'),
                        array(empty($row2[3]) ? $row2[2] : $row2[3],(!empty($SITE_DIVPREFIX[$cdid]) ? '/' : '') . $SITE_DIVPREFIX[$cdid] .
                        '/' . $row2[1] . $SITE_VARS['Default ext']),
                        ($PAGE['id'] == $row2[0]) ? $tpl['item' . $level . 'c'] : $tpl['item' . $level]
                    );
                }
                $w .= preg_replace(array('/%name%/','/%menu%/'), array($gname,$ww), ($PAGE['did'] == $hid) ? $tpl['div' . $level . 'c'] : $tpl['div' . $level]);
                break;
            }
            case '13':
            {    // Страницы группы вcе
                $gid = $hid < 1 ? $agid : $hid;
                $res2 = $mysqli->query("select name,did from site_pagegrp where id=$gid");
                $row2 = $res2->fetch_row();
                $gname = $row2[0];
                $cdid = $row2[1];
                $res2 = $mysqli->query('select id,name,title,ttlm from site_pages where (page_group=' . $gid . ')   and (clonefor=0) order by ord');
                $ww = '';
                while ($row2 = $res2->fetch_row()) {
                    $ww .= preg_replace(
                        array('/%name%/','/%ref%/'),
                        array(empty($row2[3]) ? $row2[2] : $row2[3],(!empty($SITE_DIVPREFIX[$cdid]) ? '/' : '') . $SITE_DIVPREFIX[$cdid] .
                        '/' . $row2[1] . $SITE_VARS['Default ext']),
                        ($PAGE['id'] == $row2[0]) ? $tpl['item' . $level . 'c'] : $tpl['item' . $level]
                    );
                }
                $w .= preg_replace(array('/%name%/','/%menu%/'), array($gname,$ww), ($PAGE['did'] == $hid) ? $tpl['div' . $level . 'c'] : $tpl['div' . $level]);
                break;
            }
            case '14':
            {    // Для всех групп раздела меню
                $cdid = ($hid < 1 ? ($did < 1 ? $PAGE['did'] : $did) : $hid);
                $res2 = $mysqli->query("select id,name from site_pagegrp where did=$cdid order by ord");
                while ($row2 = $res2->fetch_row()) {
                    $w .= preg_replace(
                        array('/%name%/','/%menu%/'),
                        array($row2[1],get_menu($cdid, $txt, $tplex[$txt], $tplex, $row2[0])),
                        ($PAGE['gid'] == $row2[0]) ? $tpl['submenu' . $level . 'c'] : $tpl['submenu' . $level]
                    );
                }
                break;
            }
        }
        $w = preg_replace(array('/%ord%/','/%ext1%/','/%ext2%/'), array($ord,$ext1,$ext2), $w);
        $itemPos = '';
    }
    return preg_replace(array('/%menu%/'), array($w), $tpl['main']);
}

//    11    -    Список групп                    [hid = div id]



function get_page_list($did, $menu, $tpl)
{
    global $PAGE,$SITE_DIVPREFIX,$SITE_DEFPAGE;
    $mysqli = new mysqli('localhost', 'ehope_sql', '...', 'ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
    $res = $mysqli->query('select p.id,p.title,p.name,p.did from site_pages as p where (p.locked=0) and (p.clonefor=0) AND (p.did=' . $did . ') ' . ($menu ? 'AND (p.menu1=1)' : '') . ' order by p.ord');

    while ($row = $res->fetch_row()) {
        $w .= preg_replace(
            array('/%name%/','/%ref%/'),
            array($row[1],(!empty($SITE_DIVPREFIX[$row[3]]) ? '/' : '') . $SITE_DIVPREFIX[$row[3]] . '/' . $row[2]),
            ($PAGE['id'] == $row[0]) ? $tpl['item1c'] : $tpl['item1']
        );
    }
    return preg_replace(array('/%menu%/'), array($w), $tpl['main']);
}

function get_div_list($tpl)
{
    global $PAGE,$SITE_DIVS,$SITE_DIVMENU,$SITE_DEFPAGE;
    reset($SITE_DIVS);
    foreach ($SITE_DIVS as $did => $name) {
        if ($SITE_DIVMENU[$did]) {
            $w .= preg_replace(
                array('/%name%/','/%ref%/'),
                array($name,'/' . $SITE_DEFPAGE[$did]['name']),
                ($PAGE['did'] == $did) ? $tpl['item1c'] : $tpl['item1']
            );
        }
    }
    return preg_replace(array('/%menu%/'), array($w), $tpl['main']);
}
