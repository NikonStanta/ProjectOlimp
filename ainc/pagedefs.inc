<?php

// Редактирование структуры сайта - Страницы
include_once('_fields.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$user,$pagemenu,$_image_dir,$_image_ref,$_image_exts,$mysqli,$did;
    $image_dir = $_image_dir . '/content';
    $image_ref = $_image_ref . '/content';

    if (($role & 2) == 0) {
        return $tpl['noaccess'];
    }

    if (array_key_exists("cmd", $_REQUEST)) {
    }
    {
        $cmd = $_REQUEST["cmd"];
    }

//    Определим обрабатываемую страницу
    if (!empty($_REQUEST['pid'])) {
        $pid = $_REQUEST['pid'];
        $res = $mysqli->query('select did from site_pages where id=' . $pid);
        if ($row = $res->fetch_row()) {
            $did = $row[0];
        } else {
            return $tpl['pagedefs']['nopage'];
        }
    }

    if (isset($_REQUEST['btnAdd']) && !empty($pid)) {
        $cmd = 'doEdit';
    }

    if (isset($_REQUEST['btnAdd']) && empty($pid)) {
        $cmd = 'doAdd';
    }

    if (isset($_REQUEST['btnDel']) && empty($pid)) {
        $cmd = 'doDel';
    }

    if (empty($cmd) && !empty($pid)) {
        $cmd = 'Edit';
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $pid = '';
    }

// Список разделов

    $divs = array();
    $res = $mysqli->query('select d.id,d.name from site_divs as d order by d.ord');
    while ($row = $res->fetch_row()) {
        $divs[$row[0]] = $row[1];
    }

// Типы страниц

    $page_types = array();
    $res = $mysqli->query('select id,name from types_page');
    while ($row = $res->fetch_row()) {
        $page_types[$row[0]] = $row[1];
    }

// Выполнение команд

    switch ($cmd) {
        case 'Clone':    // Клонирование текстовой страницы
        {    if (empty($pid)) {
                break;
        }
            $res = $mysqli->query('select * from site_pages where id=' . $pid);
        if ($row = $res->fetch_assoc()) {
            if ($row['page_tp'] != 2) {
                break;
            }
            $res2 = $mysqli->query('select max(clonenum) from site_pages where clonefor=' . $pid);
            if ($row2 = $res2->fetch_row()) {
                $row['clonenum'] = $row2[0] + 1;
            } else {
                $row['clonenum'] = 1;
            }
            if ($row['clonenum'] > 26) {
                break;
            }
            $row['clonefor'] = $pid;
            $row['name'] .= '_' . chr(ord('a') + $row['clonenum'] - 1);
            $query = 'insert into site_pages set ';
            $query .= 'did=' . $row['did'] . ',';
            $query .= 'name="' . $row['name'] . '",';
            $query .= 'title="' . addslashes(stripslashes($row['title'])) . '",';
            $query .= 'ttlm="' . addslashes(stripslashes($row['ttlm'])) . '",';
            $query .= 'template=' . $row['template'] . ',';
            $query .= 'theme=' . $row['theme'] . ',';
            $query .= 'page_tp=' . $row['page_tp'] . ',';
            $query .= 'page_subtp=' . $row['page_subtp'] . ',';
            $query .= 'ord=' . $row['ord'] . ',';
            $query .= 'menu1=' . $row['menu1'] . ',';
            $query .= 'locked=' . $row['locked'] . ',';
            $query .= 'clonenum=' . $row['clonenum'] . ',';
            $query .= 'clonefor=' . $row['clonefor'] . ',';
            $query .= 'keywords="' . addslashes(stripslashes($row['keywords'])) . '",';
            $query .= 'descr="' . addslashes(stripslashes($row['descr'])) . '",';
            $query .= 'meta="' . addslashes(stripslashes($row['meta'])) . '",';
            $query .= 'ttla="' . addslashes(stripslashes($row['ttla'])) . '",';
            $query .= 'page_group=' . $row['page_group'];

            if ($mysqli->query($query)) {
                $new_pid = $mysqli->insert_id;
                // Копируем текстовые поля
                $res2 = $mysqli->query('select * from fields_text where pgid=' . $pid);
                while ($row2 = $res2->fetch_assoc()) {
                    if (($row2['id'] + 0) > 0) {
                        $mysqli->query('insert into fields_text set' .
                                ' pgid=' . $new_pid . ',' .
                                ' name="' . $row2['name'] . '",' .
                                ' tags="' . $row2['tags'] . '",' .
                                ' title="' . $row2['title'] . '",' .
                                ' txt="' . $row2['txt'] . '"');
                    }
                }
                // Копируем картинки
                $res2 = $mysqli->query('select * from fields_image where pgid=' . $pid);
                while ($row2 = $res2->fetch_assoc()) {
                    if (($row2['id'] + 0) > 0) {
                        if (
                            $mysqli->query('insert into fields_image set' .
                                    ' pgid=' . $new_pid . ',' .
                                    ' name="' . $row2['name'] . '",' .
                                    ' w=' . $row2['w'] . ',' .
                                    ' h=' . $row2['h'] . ',' .
                                    ' tp="' . $row2['tp'] . '",' .
                                    ' title="' . $row2['title'] . '",' .
                                    ' alt="' . $row2['alt'] . '",' .
                                    ' attr="' . $row2['attr'] . '"')
                        ) {    // Копируем файл
                                $img_id = $mysqli->insert_id;
                                $old_iref = $image_dir . '/img' . $row2['id'] . '.' . $row2['tp'];
                                $new_iref = $image_dir . '/img' . $img_id . '.' . $row2['tp'];
                            if (file_exists($old_iref)) {
                                            copy($old_iref, $new_iref);
                                            chown($new_iref, 'Apache');
                                            chmod($new_iref, 0775);
                            }
                        }
                    }
                }
                    // Копируем поля таблицы
                    $res2 = $mysqli->query('select * from fields_table where pgid=' . $pid);
                while ($row2 = $res2->fetch_assoc()) {
                    if (($row2['id'] + 0) > 0) {
                        $mysqli->query('insert into fields_table set' .
                                ' pgid=' . $new_pid . ',' .
                                ' name="' . $row2['name'] . '",' .
                                ' tp=' . $row2['tp'] . ',' .
                                ' cols=' . $row2['cols'] . ',' .
                                ' rows=' . $row2['rows'] . ',' .
                                ' title="' . $row2['title'] . '",' .
                                ' txt="' . $row2['txt'] . '"');
                    }
                }
            }
        }
            break;
        }
        case 'doDel':
        {    $res = $mysqli->query("select page_tp from site_pages where id=$pid");
            $row = $res->fetch_row();
            switch ($row[0]) {
            // Удаляем поля текстовой страницы
                case 2:
                {    delete_fields($pid);
                    break;
                }
            }
            $mysqli->query("delete from site_pages where id=$pid");
            $cmd = '';
            $pid = '';
            break;
        }
        case 'doAdd':
        {    $oldcmd = 'Add';
            if (!empty($_REQUEST['ttl'])) {
                $page_ttl = $_REQUEST['ttl'];
            } else {
                $page_ttl = 'Новая страница';
            }
            if (!empty($_REQUEST['name'])) {
                $page_name = $_REQUEST['name'];
            } else {
                $page_name = 'newpage';
            }
            if (!empty($_REQUEST['did'])) {
                $page_did = $_REQUEST['did'];
            } else {
                $page_did = '2';
            }
            $page_menu1 = $_REQUEST['menu1'] ? 1 : 0;
            $page_locked = $_REQUEST['locked'] ? 1 : 0;
            if (!empty($_REQUEST['ord'])) {
                $page_ord = $_REQUEST['ord'];
            } else {
                $res = $mysqli->query("select max(ord) from site_pages where did=$page_did");
                $row = $res->fetch_row();
                $page_ord = $row[0] + 10;
            }

            if (!empty($_REQUEST['theme'])) {
                $page_theme = $_REQUEST['theme'];
            } else {
                $res = $mysqli->query("select theme from site_pages where did=$page_did limit 0,1");
                $row = $res->fetch_row();
                $page_theme = $row[0];
            }
            if (!empty($_REQUEST['page_tp'])) {
                $page_tp = $_REQUEST['page_tp'];
            } else {
                $page_tp = 2;
            }
            if (!empty($_REQUEST['page_subtp'])) {
                $page_subtp = $_REQUEST['page_subtp'];
            } else {
                $page_subtp = 1;
            }
            if (!empty($_REQUEST['page_ttla'])) {
                $page_ttla = $_REQUEST['page_ttla'];
            } else {
                $page_ttla = '';
            }
            if (!empty($_REQUEST['page_keywords'])) {
                $page_keywords = $_REQUEST['page_keywords'];
            } else {
                $page_keywords = '';
            }
            if (!empty($_REQUEST['page_descr'])) {
                $page_descr = $_REQUEST['page_descr'];
            } else {
                $page_descr = '';
            }
            if (!empty($_REQUEST['page_meta'])) {
                $page_meta = $_REQUEST['page_meta'];
            } else {
                $page_meta = '';
            }
            if (!empty($_REQUEST['page_group'])) {
                $page_group = $_REQUEST['page_group'] + 0;
            } else {
                $page_group = 0;
            }
            if (!empty($_REQUEST['page_ttlm'])) {
                $page_ttlm = $_REQUEST['page_ttlm'];
            } else {
                $page_ttlm = '';
            }


            $page_ttl = addslashes(stripslashes($page_ttl));
            $page_ttla = addslashes(stripslashes($page_ttla));
            $page_keywords = addslashes(stripslashes($page_keywords));
            $page_descr = addslashes(stripslashes($page_descr));
            $page_meta = addslashes(stripslashes($page_meta));
            $page_ttlm = addslashes(stripslashes($page_ttlm));

            if ($mysqli->query("insert into site_pages set title='$page_ttl', name='$page_name', did=$page_did, ord=$page_ord, theme=$page_theme, page_tp=$page_tp, page_subtp=$page_subtp, menu1=$page_menu1, locked=$page_locked, ttla='$page_ttla', keywords='$page_keywords', descr='$page_descr', meta='$page_meta', page_group=$page_group, ttlm='$page_ttlm'")) {
                $pid = $mysqli->insert_id;
                switch ($page_tp) {
                // Добавляем поля текстовой страницы
                    case 1:
                    case 2:
                    {    update_fields($pid, $page_subtp);
                        break;
                    }
                }
                $cmd = '';
            } else {
                $cmd = 'Add';
            }
            break;
        }
        case 'doEdit':
        {    $oldcmd = 'Edit';
            $res = $mysqli->query('select title,name,did,ord,template,theme,page_tp,page_subtp,clonefor from site_pages where id=' . $pid);
            if ($row = $res->fetch_row()) {
                if (isset($_REQUEST['ttl'])) {
                    $page_ttl = $_REQUEST['ttl'];
                } else {
                    $page_ttl = $row[0];
                }
                if (isset($_REQUEST['name'])) {
                    $page_name = $_REQUEST['name'];
                } else {
                    $page_name = $row[1];
                }
                if (isset($_REQUEST['did'])) {
                    $page_did = $_REQUEST['did'];
                } else {
                    $page_did = $row[2];
                }
                if (isset($_REQUEST['ord'])) {
                    $page_ord = $_REQUEST['ord'];
                } else {
                    $page_ord = $row[3];
                }
                if (isset($_REQUEST['theme'])) {
                    $page_theme = $_REQUEST['theme'];
                } else {
                    $page_theme = $row[5];
                }
                if (isset($_REQUEST['page_tp'])) {
                    $page_tp = $_REQUEST['page_tp'];
                } else {
                    $page_tp = $row[6];
                }
                if (isset($_REQUEST['page_subtp'])) {
                    $page_subtp = $_REQUEST['page_subtp'];
                } else {
                    $page_subtp = $row[7];
                }
                $page_ttla = $_REQUEST['page_ttla'];
                $page_keywords = $_REQUEST['page_keywords'];
                $page_descr = $_REQUEST['page_descr'];
                $page_meta = $_REQUEST['page_meta'];
                $page_group = $_REQUEST['page_group'] + 0;
                $page_ttlm = $_REQUEST['page_ttlm'];
                $page_menu1 = $_REQUEST['menu1'] ? 1 : 0;
                $page_locked = $_REQUEST['locked'] ? 1 : 0;
                $page_old_tp = $row[6];
                $page_old_subtp = $row[7];
                $page_old_clonefor = $row[8];

                $page_ttl = addslashes(stripslashes($page_ttl));
                $page_ttla = addslashes(stripslashes($page_ttla));
                $page_keywords = addslashes(stripslashes($page_keywords));
                $page_descr = addslashes(stripslashes($page_descr));
                $page_meta = addslashes(stripslashes($page_meta));
                $page_ttlm = addslashes(stripslashes($page_ttlm));

                if ($mysqli->query("update site_pages set title='$page_ttl', name='$page_name', did=$page_did, ord=$page_ord,  theme=$page_theme, page_tp=$page_tp, page_subtp=$page_subtp, menu1=$page_menu1, locked=$page_locked, ttla='$page_ttla', keywords='$page_keywords', descr='$page_descr', meta='$page_meta', page_group=$page_group, ttlm='$page_ttlm' where id=$pid")) {
                    if ($page_tp != $page_old_tp) {
                        switch ($page_old_tp) {
                            // Удаляем поля текстовой страницы
                            case 1:
                            case 2:
                            {    delete_fields($pid);
                                break;
                            }
                        }
                        switch ($page_tp) {
                            // Добавляем поля текстовой страницы
                            case 1:
                            case 2:
                                {    update_fields($pid, $page_subtp);
                                    break;
                            }
                        }
                    } else {
                        if ($page_subtp != $page_old_subtp) {
                            switch ($page_tp) {
                        // Обновляем поля текстовой страницы
                                case 1:
                                case 2:
                                {    update_fields($pid, $page_subtp);
                                    break;
                                }
                            }
                        }
                    }
                    $cmd = '';
                } else {
                    $cmd = 'Edit';
                }
            } else {
                $cmd = 'Edit';
            }

            break;
        }
        case 'Up':
        {    $res = $mysqli->query('select ord from site_pages where id=' . $pid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from site_pages where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update site_pages set ord=' . $upord . ' where id=' . $pid);
                    $mysqli->query('update site_pages set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    if (empty($pid) || $pid == 0) {
                $pid = 0;
        }
            $res = $mysqli->query('select title,name,did,ord,theme,page_tp,page_subtp,menu1,locked,ttla,keywords,descr,meta,page_group,ttlm,clonefor from site_pages where id=' . $pid);
            $row = $res->fetch_row();
            print_r($res);
        if (!empty($_REQUEST['ttl'])) {
            $page_ttl = $_REQUEST['ttl'];
        } else {
            $page_ttl = $row[0];
        }
        if (!empty($_REQUEST['name'])) {
            $page_name = $_REQUEST['name'];
        } else {
            $page_name = $row[1];
        }
        if (!empty($_REQUEST['did'])) {
            $page_did = $_REQUEST['did'];
        } else {
            $page_did = $row[2];
        }
        if (!empty($_REQUEST['ord'])) {
            $page_ord = $_REQUEST['ord'];
        } else {
            $page_ord = $row[3];
        }
        if (!empty($_REQUEST['theme'])) {
            $page_theme = $_REQUEST['theme'];
        } else {
            $page_theme = $row[4];
        }
        if (!empty($_REQUEST['page_tp'])) {
            $page_tp = $_REQUEST['page_tp'];
        } else {
            $page_tp = $row[5];
        }
        if (!empty($_REQUEST['page_subtp'])) {
            $page_subtp = $_REQUEST['page_subtp'];
        } else {
            $page_subtp = $row[6];
        }
        if (!empty($_REQUEST['page_ttla'])) {
            $page_ttla = $_REQUEST['page_ttla'];
        } else {
            $page_ttla = $row[9];
        }
        if (!empty($_REQUEST['page_keywords'])) {
            $page_keywords = $_REQUEST['page_keywords'];
        } else {
            $page_keywords = $row[10];
        }
        if (!empty($_REQUEST['page_descr'])) {
            $page_descr = $_REQUEST['page_descr'];
        } else {
            $page_descr = $row[11];
        }
        if (!empty($_REQUEST['page_meta'])) {
            $page_meta = $_REQUEST['page_meta'];
        } else {
            $page_meta = $row[12];
        }
        if (!empty($_REQUEST['page_group'])) {
            $page_group = $_REQUEST['page_group'];
        } else {
            $page_group = $row[13];
        }
        if (!empty($_REQUEST['page_ttlm'])) {
            $page_ttlm = $_REQUEST['page_ttlm'];
        } else {
            $page_ttlm = $row[14];
        }
            $page_clonefor = $row[15];
        if (empty($oldcmd)) {
            $page_menu1 = $row[7];
            $page_locked = $row[8];
        } else {
            $page_menu1 = $_REQUEST['menu1'] ? 1 : 0;
            $page_locked = $_REQUEST['locked'] ? 1 : 0;
        }
            $page_dids = '';
        foreach ($divs as $k => $v) {
            $page_dids .= '<option value=' . $k . ($page_did == $k ? ' selected' : '') . '>' . $v . '</option>';
        }

        if ($page_tp != 1) {
            $w = '';
            $res = $mysqli->query('select id,title from site_themes');
            while ($row = $res->fetch_row()) {
                $iref = $image_ref . '/aimg/page_theme_' . $row[0] . '.jpg';
                $w .= preg_replace(
                    array('/%id%/','/%ref%/','/%ttl%/','/%sel%/'),
                    array($row[0],$iref,$row[1],$page_theme == $row[0] ? ' checked' : ''),
                    $tpl['pagedefs']['themeline']
                );
            }
            $page_themes = preg_replace(array('/%list%/'), array($w), $tpl['pagedefs']['themes']);
        } else {
            $page_themes = 'не используется';
        }

            $page_tps = '';
            $res = $mysqli->query('select id,name from types_page');
        while ($row = $res->fetch_row()) {
            $page_tps .= '<option value=' . $row[0] . ($page_tp == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
        }

            $w = '';
            $res = $mysqli->query('select id,name from site_pagegrp where did=' . $page_did);
            //while ($row=@$res->fetch_row())
            //{    $w.='<option value='.$row[0].($page_group==$row[0] ? ' selected' : '').'>'.$row[1].'</option>';
            //}
        if (empty($w)) {
            $page_groups = '';
        } else {
            $page_groups = preg_replace(array('/%list%/'), array($w), $tpl['pagedefs']['page_groups']);
        }

        switch ($page_tp) {
            case 1:
            case 2:
            {
                $w = '';
                $res = $mysqli->query('select id,name from pages_text');
                while ($row = $res->fetch_row()) {
                    $w .= '<option value=' . $row[0] . ($page_subtp == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
                }
                $page_subtype = preg_replace(array('/%list%/'), array($w), $tpl['pagedefs']['edit_text']);
                break;
            }
        }

            $body = preg_replace(
                array('/%pid%/','/%ttl%/','/%name%/','/%dids%/','/%ord%/','/%tpls%/','/%themes%/',
                '/%page_tps%/','/%subtype%/','/%pgmenu1%/','/%locked%/',
                '/%page_ttla%/','/%page_keywords%/','/%page_descr%/','/%page_meta%/','/%page_groups%/','/%page_ttlm%/',
                '/%did%/','/%gid%/'),
                array($pid,$page_ttl,$page_name,$page_dids,$page_ord,$page_tpls,$page_themes,$page_tps,$page_subtype,
                $page_menu1 ? ' checked' : '',$page_locked ? ' checked' : '',
                $page_ttla,$page_keywords,$page_descr,$page_meta,$page_groups,$page_ttlm,
                $page_did,$page_gid
                ),
                $page_clonefor == 0 ? $tpl['pagedefs']['edit'] :  $tpl['pagedefs']['editclone']
            );

            break;
        }
        case 'Del':
        {    $res = $mysqli->query('select title,name,did from site_pages where id=' . $pid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%pid%/','/%ttl%/','/%name%/','/%divname%/'),
                    array($pid,$row[0],$row[1],$divs[$row[2]]),
                    $tpl['pagedefs']['del']
                );
                break;
            } else {
                $cmd = '';
            }
        }
        default:
        {    // Список страниц
            $list = '';
            $even = false;
            $res = $mysqli->query('select p.id,p.title,p.name,p.page_tp,d.id from site_divs as d left join site_pages as p on  (d.id=p.did) where (p.clonefor=0) order by d.ord,p.ord');
            $curr_did = '';
            $upid = 0;
            while ($row = $res->fetch_row()) {
                if ($curr_did <> $row[4]) {
                    $curr_did = $row[4];
                    $even = false;
                    $upid = 0;
                    $list .= preg_replace(array('/%ttl%/'), array($divs[$curr_did]), $tpl['pagedefs']['divline']);
                }
                $xpage_id = $row[0];
                $xpage_ttl = $row[1];
                $xpage_name = $row[2];
                $xpage_type = $row[3];

                switch ($xpage_type) {
                    case 2:
                    {    $content = preg_replace('/%id%/', $xpage_id, $tpl['pagedefs']['content_text']);
                        $clone = preg_replace('/%id%/', $xpage_id, $tpl['pagedefs']['clone_btn']);
                        break;
                    }
                    default:
                    {    $content = '';
                        $clone = '';
                    }
                }
                if (($row[0] + 0) > 0) {
                    $list .= preg_replace(
                        array('/%id%/','/%ttl%/','/%name%/','/%tp%/','/%bgcolor%/',
                        '/%clone%/','/%content%/','/%upid%/'),
                        array($xpage_id,$xpage_ttl,$xpage_name,$page_types[$xpage_type],$tpl[$even ? 'even' : 'odd'],
                        $clone,$content,$upid),
                        ($upid == 0 ? $tpl['pagedefs']['pageline'] : $tpl['pagedefs']['pagelineup'])
                    );
                    $upid = $row[0];
                } else {
                    $list .= preg_replace('/%bgcolor%/', $tpl[$even ? 'even' : 'odd'], $tpl['pagedefs']['pagelineblank']);
                }

                // Выводим клоны, если есть

                if ($xpage_type == 2) {
                    $res2 = $mysqli->query('select p.id,p.title,p.name,p.page_tp,p.clonefor from site_pages as p where (p.clonefor=' . $xpage_id . ') order by p.clonenum');
                    while ($row2 = $res2->fetch_row()) {
                        $content = preg_replace('/%id%/', $row2[0], $tpl['pagedefs']['content_text']);
                        if (($row[0] + 0) > 0) {
                            $list .= preg_replace(
                                array('/%id%/','/%ttl%/','/%name%/','/%tp%/','/%bgcolor%/',
                                '/%clone%/','/%content%/','/%upid%/'),
                                array($row2[0],$tpl['pagedefs']['clone_text'] . $row2[1],$row2[2],$page_types[$row2[3]],
                                $tpl[$even ? 'even' : 'odd'],'',$content,0),
                                $tpl['pagedefs']['pageline']
                            );
                        }
                    }
                }

                $even = !$even;
            }


            $body = preg_replace(array('/%list%/','/%div%/'), array($list,$divs[$did]), $tpl['pagedefs']['main']);
        }
    }
    return $body;
}

$body = build_page();
