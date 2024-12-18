<?php

// Редактирование структуры сайта - Меню

function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$user,$pagemenu,$image_dir,$image_ref,$_image_exts,$mysqli;

    $_menu_tps = array(
    0    => '    Текст',
    1    => '    Внешняя ссылка',
    2    => '    Другое меню по имени',
    3    => '    Страница',
    4    => '    Страница раздела по умолчанию',
    5    => '    Список разделов    видимых',
    6    => '    Список разделов    всех',
    7    => '    Страницы раздела видимые',
    8    => '    Страницы раздела все',
    9    => '    Для видимых разделов меню',
    10    => '    Для всех разделов меню',
    11    => '    Список групп',
    12    => '    Страницы группы видимые',
    13    => '    Страницы группы все',
    14    => '    Для всех групп раздела меню'
    );

    if (($role & 2) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];
    $mid = $_REQUEST['mid'];
    $iid = $_REQUEST['iid'];
    $mttl = $_REQUEST['mttl'];

    if (empty($mid) && !empty($iid)) {
        $res = $mysqli->query('select mid from menu_items where id=' . $iid);
        $row = $res->fetch_row();
        $mid = $row[0];
    }

    if (isset($_REQUEST['btnAdd'])) {
        if (empty($mid)) {
            $cmd = 'doAdd';
        } else {
            $cmd = 'doEdit';
        }
    }

    if (isset($_REQUEST['btnAddI'])) {
        if (empty($iid)) {
            $cmd = 'doAddItem';
        } else {
            $cmd = 'doEditItem';
        }
    }

    if (isset($_REQUEST['btnDel']) && empty($iid)) {
        $cmd = 'doDel';
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $mid = '';
    }

    if (isset($_REQUEST['btnCancelI'])) {
        $cmd = 'ListItem';
        $iid = '';
    }


// Список разделов

    $divs = array();
    $res = $mysqli->query('select d.id,d.name from site_divs as d order by d.ord');
    while ($row = $res->fetch_row()) {
        $divs[$row[0]] = $row[1];
    }

    $divs[0] = 'Все разделы';
// Выполнение команд

    switch ($cmd) {
        case 'doDelItem':
        {    $mysqli->query("delete from menu_items where id=$iid");
            $cmd = 'ListItem';
            $iid = '';
            break;
        }
        case 'doEditItem':
        {    if (!empty($_REQUEST['tp'])) {
                $tp = $_REQUEST['tp'];
        } else {
            $tp = 0;
        }
        if (!empty($_REQUEST['level'])) {
            $level = $_REQUEST['level'];
        } else {
            $level = 1;
        }
        if (!empty($_REQUEST['title'])) {
            $title = $_REQUEST['title'];
        } else {
            $title = '<Новый пункт меню>';
        }
        if (!empty($_REQUEST['hid' . $tp])) {
            $hid = $_REQUEST['hid' . $tp];
        } else {
            $hid = 0;
        }
        if (!empty($_REQUEST['txt' . $tp])) {
            $txt = $_REQUEST['txt' . $tp];
        } else {
            $txt = '';
        }
        if (!empty($_REQUEST['ord'])) {
            $ord = $_REQUEST['ord'];
        } else {
            $res = $mysqli->query("select max(ord) from menu_items where mid=" . $mid);
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
        }
        if (empty($href)) {
            if (empty($pgid)) {
                $pgid = $divid;
            }
        } else {
            $pgid = 0;
        }

        if ($level < 0) {
            $level = 0;
        }
        if ($level > 2) {
            $level = 2;
        }
        if (!empty($_REQUEST['ext1'])) {
            $ext1 = $_REQUEST['ext1'];
        } else {
            $ext1 = '';
        }
        if (!empty($_REQUEST['ext2'])) {
            $ext2 = $_REQUEST['ext2'];
        } else {
            $ext2 = '';
        }

        if ($mysqli->query('update menu_items set tp=' . $tp . ', title="' . addslashes($title) . '", level=' . $level . ', hid=' . $hid . ', txt="' . addslashes($txt) . '", ext1="' . addslashes($ext1) . '", ext2="' . addslashes($ext2) . '", ord=' . $ord . ' where id=' . $iid)) {
            $cmd = 'ListItem';
        } else {
            $cmd = 'EditItem';
        }
        break;
        }
        case 'doAddItem':
        {    if (!empty($_REQUEST['tp'])) {
                $tp = $_REQUEST['tp'];
        } else {
            $tp = 0;
        }
        if (!empty($_REQUEST['level'])) {
            $level = $_REQUEST['level'];
        } else {
            $level = 1;
        }
        if (!empty($_REQUEST['title'])) {
            $title = $_REQUEST['title'];
        } else {
            $title = '<Новый пункт меню>';
        }
        if (!empty($_REQUEST['hid' . $tp])) {
            $hid = $_REQUEST['hid' . $tp];
        } else {
            $hid = 0;
        }
        if (!empty($_REQUEST['txt' . $tp])) {
            $txt = $_REQUEST['txt' . $tp];
        } else {
            $txt = '';
        }

        if (!empty($_REQUEST['ord'])) {
            $ord = $_REQUEST['ord'];
        } else {
            $res = $mysqli->query("select max(ord) from menu_items where mid=" . $mid);
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
        }
        if ($level < 0) {
            $level = 0;
        }
        if ($level > 2) {
            $level = 2;
        }

        if (!empty($_REQUEST['ext1'])) {
            $ext1 = $_REQUEST['ext1'];
        } else {
            $ext1 = '';
        }
        if (!empty($_REQUEST['ext2'])) {
            $ext2 = $_REQUEST['ext2'];
        } else {
            $ext2 = '';
        }

        if ($mysqli->query('insert into menu_items set mid=' . $mid . ', tp=' . $tp . ', title="' . addslashes($title) . '", level=' . $level . ', hid=' . $hid . ', txt="' . addslashes($txt) . '", ext1="' . addslashes($ext1) . '", ext2="' . addslashes($ext2) . '", ord=' . $ord)) {
            $cmd = 'ListItem';
        } else {
            $cmd = 'AddItem';
        }
        break;
        }

        case 'doEdit':
        {    if (!empty($mid)) {
                $query = '';
                if (isset($_REQUEST['title'])) {
                    $query = 'title="' . $_REQUEST['title'] . '"';
                }
                if (isset($_REQUEST['menu_did'])) {
                    $query .= (!empty($query) ? ', ' : '') . 'did=' . $_REQUEST['menu_did'];
                }
                if (isset($_REQUEST['menu_name'])) {
                    $query .= (!empty($query) ? ', ' : '') . 'name="' . $_REQUEST['menu_name'] . '"';
                }
                if (isset($_REQUEST['menu_tpl'])) {
                    $query .= (!empty($query) ? ', ' : '') . 'tpl="' . $_REQUEST['menu_tpl'] . '"';
                }

                if (!empty($query)) {
                    if ($mysqli->query("update site_menu set $query where id=$mid")) {
                        $cmd = '';
                    } else {
                        $cmd = 'Edit';
                    }
                } else {
                    $cmd = 'Edit';
                }
        } else {
            $cmd = '';
        }
        break;
        }
        case 'doAdd':
        {    $query = '';
            if (isset($_REQUEST['title'])) {
                $query = 'title="' . $_REQUEST['title'] . '"';
            }
            if (isset($_REQUEST['menu_did'])) {
                $query .= (!empty($query) ? ', ' : '') . 'did=' . $_REQUEST['menu_did'];
            }
            if (isset($_REQUEST['menu_name'])) {
                $query .= (!empty($query) ? ', ' : '') . 'name="' . $_REQUEST['menu_name'] . '"';
            }
            if (isset($_REQUEST['menu_tpl'])) {
                $query .= (!empty($query) ? ', ' : '') . 'tpl="' . $_REQUEST['menu_tpl'] . '"';
            }

            if (!empty($query)) {
                if ($mysqli->query("insert into site_menu set $query")) {
                    $cmd = '';
                } else {
                    $cmd = 'Add';
                }
            } else {
                $cmd = 'Add';
            }

            break;
        }
        case 'Up':
        {    $res = $mysqli->query('select ord from menu_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from menu_items where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update menu_items set ord=' . $upord . ' where id=' . $iid);
                    $mysqli->query('update menu_items set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = 'ListItem';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $res = $mysqli->query('select title,did,name,tpl from site_menu where id=' . $mid);
            $row = $res->fetch_row();
            $old_title = $row[0];
            $old_menu_did = $row[1];
            $old_menu_name = $row[2];
            $old_menu_tpl = $row[3];
        }
        case 'Add':
        {    if (!empty($_REQUEST['title'])) {
                $title = $_REQUEST['title'];
        } else {
            $title = empty($old_title) ? '<Новое меню>' : $old_title;
        }
        if (!empty($_REQUEST['menu_did'])) {
            $menu_did = $_REQUEST['manu_did'];
        } else {
            $menu_did = $old_menu_did + 0;
        }
        if (!empty($_REQUEST['menu_name'])) {
            $menu_name = $_REQUEST['menu_name'];
        } else {
            $menu_name = $old_menu_name;
        }
        if (!empty($_REQUEST['menu_tpl'])) {
            $menu_tpl = $_REQUEST['menu_tpl'];
        } else {
            $menu_tpl = $old_menu_tpl;
        }
            $page_dids = '';
        foreach ($divs as $k => $v) {
            $page_dids .= '<option value=' . $k . ($menu_did == $k ? ' selected' : '') . '>' . $v . '</option>';
        }

            $body = preg_replace(
                array('/%id%/','/%title%/','/%dids%/','/%menu_name%/','/%menu_tpl%/'),
                array($mid,$title,$page_dids,$menu_name,$menu_tpl),
                $tpl['menus']['edit']
            );

        break;
        }
        case 'EditItem':
        {    $res = $mysqli->query('select tp,level,title,hid,txt,ord,ext1,ext2 from menu_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $old_tp = $row[0];
                $old_level = $row[1];
                $old_title = $row[2];
                $old_hid = $row[3];
                $old_txt = $row[4];
                $old_ord = $row[5];
                $old_ext1 = $row[6];
                $old_ext2 = $row[7];
            }
        }
        case 'AddItem':
        {    if (!empty($_REQUEST['tp'])) {
                $tp = $_REQUEST['tp'];
        } else {
            $tp = $old_tp;
        }
        if (!empty($_REQUEST['level'])) {
            $level = $_REQUEST['level'];
        } else {
            $level = $old_level;
        }
        if (!empty($_REQUEST['title'])) {
            $title = $_REQUEST['title'];
        } else {
            $title = $old_title;
        }
        if (!empty($_REQUEST['hid' . $tp])) {
            $hid = $_REQUEST['hid' . $tp];
        } else {
            $hid = $old_hid;
        }
        if (!empty($_REQUEST['txt' . $tp])) {
            $txt = $_REQUEST['txt' . $tp];
        } else {
            $txt = $old_txt;
        }
        if (!empty($_REQUEST['ord'])) {
            $ord = $_REQUEST['ord'];
        } else {
            $ord = $old_ord;
        }
        if (isset($_REQUEST['ext1'])) {
            $ext1 = $_REQUEST['ext1'];
        } else {
            $ext1 = $old_ext1;
        }
        if (isset($_REQUEST['ext2'])) {
            $ext2 = $_REQUEST['ext2'];
        } else {
            $ext2 = $old_ext2;
        }

            $divs = '<option value=0>---</option>';
            $divss = '<option value=0>---</option>';
            $res = $mysqli->query('select id,name from site_divs order by ord');
        while ($row = $res->fetch_row()) {
            $divs .= '<option value=' . ($row[0]) . '>' . $row[1] . '</option>';
            $divss .= '<option value=' . ($row[0]) . ($row[0] == $hid ? ' selected' : '') . '>' . $row[1] . '</option>';
        }

            $pages = '<option value=0>---</option>';
            $res = $mysqli->query('select p.id,d.name,p.title from site_pages as p left join site_divs as d on (d.id=p.did) order by d.ord, p.ord');
        while ($row = $res->fetch_row()) {
            $pages .= '<option value=' . $row[0] . (($row[0] == $hid) && ($tp == 3) ? ' selected' : '') . '>' . $row[1] . '-' . $row[2] . '</option>';
        }

            $groups = '<option value=0>---</option>';
            $groupss = '<option value=0>---</option>';

            $res = $mysqli->query('select g.id,d.name,g.name from site_pagegrp as g left join site_divs as d on (d.id=g.did) order by d.ord, g.ord');
        while ($row = $res->fetch_row()) {
            $groups .= '<option value=' . $row[0] . '>' . $row[1] . '-' . $row[2] . '</option>';
            $groupss .= '<option value=' . $row[0] . (($row[0] == $hid) && (($tp == 12) || ($tp == 13)) ? ' selected' : '') . '>' . $row[1] . '-' . $row[2] . '</option>';
        }

            $menus = '<option value=0>---</option>';
            $menuss = '<option value=0>---</option>';

            $res = $mysqli->query('select DISTINCT name from site_menu group by name order by name');
        while ($row = $res->fetch_row()) {
            $menus .= '<option>' . $row[0] . '</option>';
            $menuss .= '<option ' . (($row[0] == $txt) ? ' selected' : '') . '>' . $row[0] . '</option>';
        }

            $w = preg_replace(
                array('/%iid%/','/%mid%/','/%title%/','/%level%/','/%ord%/','/%pages%/',
                    '/%sel' . $tp . '%/','/%txt' . $tp . '%/','/%divs' . $tp . '%/','/%groups' . $tp . '%/','/%menus' . $tp . '%/','/%ext1%/','/%ext2%/'),
                array($iid,$mid,$title,$level,$ord,$pages,' checked',$txt,$divss,$groupss,$menuss,$ext1,$ext2),
                $tpl['menus']['edititem']
            );

            $body = preg_replace(
                array('/%sel[0-9]{1,2}%/',"/%txt[0-9]{1,2}%/",'/%divs[0-9]{1,2}%/','/%groups[0-9]{1,2}%/','/%menus[0-9]{1,2}%/'),
                array('','',$divs,$groups,$menus),
                $w
            );

        break;
        }
        case 'DelItem':
        {    $res = $mysqli->query('select title from menu_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%id%/','/%ttl%/'),
                    array($iid,$row[0]),
                    $tpl['menus']['delitem']
                );
                break;
            } else {
                $cmd = '';
            }
            break;
        }
        case 'ListItem':
        {    // Список
            $res = $mysqli->query('select title from site_menu where id=' . $mid);
            $row = $res->fetch_row();
            $menu_name = $row[0];
            $list = '';
            $even = false;
            $upid = 0;
            $res = $mysqli->query('select i.id,i.title,p.name,i.txt,i.hid,i.tp from menu_items as i left join site_pages as p on (p.id=i.hid) where mid=' . $mid . ' order by i.ord');
            while ($row = $res->fetch_row()) {
                $w = $_menu_tps[$row[5]];
                $list .= preg_replace(
                    array('/%id%/','/%upid%/','/%ttl%/','/%page%/','/%bgcolor%/'),
                    array($row[0],$upid,$row[1],$w,$tpl[$even ? 'even' : 'odd']),
                    ($upid == 0) ? $tpl['menus']['itemline'] :  $tpl['menus']['itemlineup']
                );
                $upid = $row[0];
                $even = !$even;
            }
            $body = preg_replace(
                array('/%menu_name%/','/%list%/','/%mid%/'),
                array($menu_name,$list,$mid),
                $tpl['menus']['itemlist']
            );
                break;
        }
/*case 'Del':
        {    $res=$mysqli->query('select title,did,count(id) from menu_items where mid='.$mid);
            if ($row=$res->fetch_row())
            {    $body=preg_replace(
                array('/%id%/','/%ttl%/'),
                array($iid,$row[0]),
                $tpl['menus']['delitem']
                );
                break;
            }
            else
            {    $cmd='';
            }
            break;
        }*/
        default:
        {    // Список
            $list = '';
            $even = false;
            $res = $mysqli->query('select id,did,title,name from site_menu order by title');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%div%/','/%ttl%/','/%menu_name%/','/%bgcolor%/'),
                    array($row[0],$divs[$row[1]],$row[2],$row[3],$tpl[$even ? 'even' : 'odd']),
                    $tpl['menus']['line']
                );
                $even = !$even;
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['menus']['main']);
        }
    }
    return $body;
}

$body = build_page();
