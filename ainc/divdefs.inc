<?php

// Редактирование структуры сайта - Разделы
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$user,$pagemenu,$image_dir,$image_ref,$_image_exts,$mysqli;

    if (($role & 2) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];

//    Определим обрабатываемый раздел
    if (!empty($_REQUEST['did'])) {
        $did = $_REQUEST['did'];
        $res = $mysqli->query('select id from site_divs where id=' . $did);
        if (!$row = $res->fetch_row()) {
            return $tpl['divdefs']['nodiv'];
        }
    }

    if (isset($_REQUEST['btnAdd']) && !empty($did)) {
        $cmd = 'doEdit';
    }

    if (isset($_REQUEST['btnAdd']) && empty($did)) {
        $cmd = 'doAdd';
    }

    if (empty($cmd) && !empty($did)) {
        $cmd = 'Edit';
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $pid = '';
    }
// Выполнение команд

    switch ($cmd) {
        case 'doEdit':
        {    $oldcmd = 'Edit';
            $res = $mysqli->query('select name,def_page,prefix,ord from site_divs where id=' . $did);
            if ($row = $res->fetch_row()) {
                if (!empty($_REQUEST['name'])) {
                    $div_name = $_REQUEST['name'];
                } else {
                    $div_name = $row[0];
                }
                if (!empty($_REQUEST['def_page'])) {
                    $def_page = $_REQUEST['def_page'];
                } else {
                    $def_page = $row[1];
                }
                $div_prefix = trim($_REQUEST['prefix']);
                if (!empty($_REQUEST['ord'])) {
                    $div_ord = $_REQUEST['ord'];
                } else {
                    $div_ord = $row[3] + 0;
                }
                $div_menu1 = $_REQUEST['menu1'] ? 1 : 0;
                $div_email = trim($_REQUEST['email']);
                if ($mysqli->query("update site_divs set name='$div_name', def_page=$def_page, prefix='$div_prefix', ord=$div_ord, email='$div_email', menu1=$div_menu1 where id=$did")) {
                    $cmd = '';
                } else {
                    $cmd = 'Edit';
                }
            } else {
                $cmd = 'Edit';
            }

            break;
        }
        case 'doAdd':
        {    $oldcmd = 'Add';
            $div_name = $_REQUEST['name'];
            $def_page = $_REQUEST['def_page'] + 0;
            $div_prefix = trim($_REQUEST['prefix']);
            $div_ord = $_REQUEST['ord'] + 0;
            $div_menu1 = $_REQUEST['menu1'] ? 1 : 0;
            $div_email = trim($_REQUEST['email']);
            if ($mysqli->query("insert into site_divs set name='$div_name', def_page=$def_page, prefix='$div_prefix', ord=$div_ord, email='$div_email', menu1=$div_menu1")) {
                $cmd = '';
                $did = $mysqli->insert_id;
            } else {
                $cmd = 'Add';
            }

            break;
        }
        case 'Up':
        {    $res = $mysqli->query('select ord from site_divs where id=' . $did);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from site_divs where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update site_divs set ord=' . $upord . ' where id=' . $did);
                    $mysqli->query('update site_divs set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }
        case 'doDel':
        {    $res = $mysqli->query("select count(id) from site_pages where did=$did");
            $row = $res->fetch_row();
            if ($row[0] == 0) {
                $mysqli->query("delete from site_divs where id=$did");
            }
            $cmd = '';
            $did = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Add':
        case 'Edit':
        {    if (($cmd == 'Edit') && (!empty($did))) {
                $res = $mysqli->query('select name,def_page,ord,prefix,email,menu1 from site_divs where id=' . $did);
                $row = $res->fetch_row();
                if (!empty($_REQUEST['name'])) {
                    $div_name = $_REQUEST['name'];
                } else {
                    $div_name = $row[0];
                }
                if (!empty($_REQUEST['def_page'])) {
                    $def_page = $_REQUEST['def_page'];
                } else {
                    $def_page = $row[1];
                }
                if (!empty($_REQUEST['ord'])) {
                    $div_ord = $_REQUEST['ord'];
                } else {
                    $div_ord = $row[2];
                }
                if (!empty($_REQUEST['prefix'])) {
                    $div_prefix = $_REQUEST['prefix'];
                } else {
                    $div_prefix = $row[3];
                }
                if (!empty($_REQUEST['email'])) {
                    $div_email = $_REQUEST['email'];
                } else {
                    $div_email = $row[4];
                }
                if (empty($oldcmd)) {
                    $div_menu1 = $row[5];
                } else {
                    $div_menu1 = $_REQUEST['menu1'] ? 1 : 0;
                }

                $div_pages = '';
                $res = $mysqli->query('select id,title from site_pages where did=' . $did);
                while ($row = $res->fetch_row()) {
                    $div_pages .= '<option value=' . $row[0] . ($row[0] == $def_page ? ' selected' : '') . '>' . $row[1] . '</option>';
                }
        }
            $body = preg_replace(
                array('/%did%/','/%name%/','/%def_page%/','/%ord%/','/%prefix%/','/%email%/','/%pgmenu1%/'),
                array($did,$div_name,$div_pages,$div_ord,$div_prefix,$div_email,$div_menu1 ? ' checked' : ''),
                $tpl['divdefs']['edit']
            );

                break;
        }
        case 'Del':
        {    $res = $mysqli->query('select d.name,count(p.id) from site_divs as d left join site_pages as p on (p.did=d.id) where d.id=' . $did . ' group by d.name');
            if ($row = $res->fetch_row()) {
                if ($row[1] > 0) {
                    $body = preg_replace(
                        array('/%did%/','/%ttl%/','/%count%/'),
                        array($did,$row[0],$row[1]),
                        $tpl['divdefs']['nodel']
                    );
                } else {
                    $body = preg_replace(
                        array('/%did%/','/%ttl%/','/%count%/'),
                        array($did,$row[0],$row[1]),
                        $tpl['divdefs']['del']
                    );
                }
                break;
            } else {
                $cmd = '';
            }
        }

        default:
        {    // Список страниц
            $list = '';
            $even = false;
            $upid = 0;
            $res = $mysqli->query('select d.id,d.name,p.title from site_divs as d left join site_pages as p on (p.id=d.def_page) order by d.ord');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%name%/','/%def_page%/','/%bgcolor%/','/%upid%/'),
                    array($row[0],$row[1],$row[2],$tpl[$even ? 'even' : 'odd'],$upid),
                    ($upid == 0 ? $tpl['divdefs']['divline'] : $tpl['divdefs']['divlineup'])
                );
                $even = !$even;
                $upid = $row[0];
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['divdefs']['main']);
        }
    }
    return $body;
}

$body = build_page();
