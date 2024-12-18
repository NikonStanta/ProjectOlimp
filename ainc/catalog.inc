<?php

// Редактирование страниц каталога 2009-12-xx
include_once('_fields.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$_image_dir,$_image_ref,$_image_exts,$SITE_VARS,$mysqli;

    if (($role & 1) == 0) {
        return $tpl['noaccess'];
    }

    if (!empty($SITE_VARS['DEFAULT_IMG_PREFIX'])) {
        define('DEFAULT_IMG_PREFIX', $SITE_VARS['DEFAULT_IMG_PREFIX']);
    } else {
        define('DEFAULT_IMG_PREFIX', 'cat');
    }

    if (!empty($SITE_VARS['DEFAULT_IMG_DIR'])) {
        define('DEFAULT_IMG_DIR', $SITE_VARS['DEFAULT_IMG_DIR']);
    } else {
        define('DEFAULT_IMG_DIR', 'catalog');
    }

    $cmd = $_REQUEST['cmd'];

    if (!empty($_REQUEST['lid'])) {
        $lid = $_REQUEST['lid'];
    }

    if (!empty($_REQUEST['liid'])) {
        $liid = $_REQUEST['liid'];
    }

    if (!empty($_REQUEST['iid'])) {
        $iid = $_REQUEST['iid'];
    }

    if (!empty($_REQUEST['gid'])) {
        $gid = $_REQUEST['gid'];
    } else {
        if (!empty($iid)) {
            $res = $mysqli->query('select gid from catalog_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $gid = $row[0];
            }
        }
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $iid = '';
    }

    if (isset($_REQUEST['btnCancelLot'])) {
        $cmd = 'Lots';
        $liid = '';
    }

    if (isset($_REQUEST['btnResize'])) {
        $cmd = 'Resize';
    }

    if (isset($_REQUEST['btnAddItem'])) {
        if (empty($iid)) {
            $cmd = 'doAddItem';
        } else {
            $cmd = 'doEditItem';
        }
    }

    $delImages = '';
    if (isset($_REQUEST['btnDelImage'])) {
        if (empty($iid)) {
            $cmd = '';
        } else {
            $del_buttons = $_REQUEST['btnDelImage'];
            foreach ($del_buttons as $k => $w) {
                $delImages[$k] = $k;
            }
            $cmd = 'doDelImage';
        }
    }

    if (isset($_REQUEST['btnDetail'])) {
        if (empty($iid)) {
            $cmd = '';
        } else {
            $cmd = 'doDetail';
        }
    }

    if (isset($_REQUEST['btnDelItem'])) {
        if (empty($iid)) {
            $cmd = '';
        } else {
            $cmd = 'doDelItem';
        }
    }

    if (isset($_REQUEST['btnAddGroup'])) {
        if (empty($gid)) {
            $cmd = 'doAddGroup';
        } else {
            $cmd = 'doEditGroup';
        }
    }

    if (isset($_REQUEST['btnDelGroup'])) {
        if (empty($gid)) {
            $cmd = '';
        } else {
            $cmd = 'doDelGroup';
        }
    }

    if (isset($_REQUEST['btnAddLot'])) {
        if (empty($lid)) {
            $cmd = 'doAddLot';
        } else {
            $cmd = 'doEditLot';
        }
    }

    if (isset($_REQUEST['btnAddLotItem'])) {
        if ($_REQUEST['xcmd'] == 'add') {
            $cmd = 'doAddLotItem';
        } else {
            $cmd = 'doEditLotItem';
        }
    }

// Список каталогов

    $catalogs = array();
    $res = $mysqli->query('select p.id,p.title,p.ttlm,d.id,d.name from site_pages as p left join site_divs as d on (d.id=p.did) where p.page_tp=3 order by d.ord');
    while ($row = $res->fetch_row()) {
        $catalogs[$row[0]] = array('ttl' => $row[1],'ttlm' => $row[2],'pgid' => $row[3],'ttld' => $row[4]);
    }

    if (!is_array($catalogs)) {
        return $tpl['pages']['noaccess'];
    }

    // Текущий раздел
    if (!empty($_REQUEST['pgid'])) {
        $pgid = $_REQUEST['pgid'];
    } else {
        if (!empty($gid)) {
            $res = $mysqli->query('select pgid from catalog_groups where id=' . $gid);
            if ($row = $res->fetch_row()) {
                $pgid = $row[0];
            }
        }
        if (empty($pgid)) {
            $pgid = key($catalogs);
        }
    }

    // Проверка прав доступа и правильности данных
    /*
    $res=$mysqli->query('select ISNULL(u.uid) from site_pages as p left join site_divs as d on (d.id=p.did) left join user_divs as u on ((u.pgid=d.id) AND (u.uid='.$uid.')) where p.id='.$pgid);
    if ($row=$res->fetch_row())
    {    if ($row[5]!=0)
        {    return $tpl['pages']['noaccess'];
        }
        if (empty($gid))
        {    $res=$mysqli->query('select id from catalog_groups where pgid='.$pgid);
            if ($row=$res->fetch_row())
            {    $gid=$row[0];
            }
            $iid=0;
        }
        else
        {    $res=$mysqli->query('select pgid from catalog_groups where id='.$gid);
            if ($row=$res->fetch_row())
            {    if ($pgid!=$row[0])
                {    $gid=0;
                    $iid=0;
                }
            }
            if (empty($gid))
            {    $res=$mysqli->query('select id from catalog_groups where pgid='.$pgid);
                if ($row=$res->fetch_row())
                {    $gid=$row[0];
                    $iid=0;
                }
            }
        }

    }
    else
    {    return $tpl['pages']['noaccess'];
    }
    */
    $ok = false;
    if (!empty($gid)) {
        $res = $mysqli->query('select img_prefix,img_dir from catalog_groups where id=' . $gid);
        if ($row = $res->fetch_row()) {
            $img_prefix = $row[0];
            $image_dir = $_image_dir . '/' . $row[1];
            $image_ref = $_image_ref . '/' . $row[1];
            $ok = true;
        }
    }
    if (!$ok) {
        $img_prefix = DEFAULT_IMG_PREFIX;
        $image_dir = $_image_dir . '/' . DEFAULT_IMG_DIR;
        $image_ref = $_image_ref . '/' . DEFAULT_IMG_DIR;
    }

    // Список каталогов, к которым есть доступ
    $dm = preg_replace(
        array('/%ref%/','/%ttl%/'),
        array('','Каталоги сайта'),
        $tpl['menu1']['ttl']
    );
    foreach ($catalogs as $k => $v) {
        if ($k == $pgid) {
            $dm .= preg_replace(
                array('/%ref%/','/%ttl%/','/%refpar%/'),
                array('catalog.html',$v['ttl'],'%refpar%&pgid=' . $k),
                $tpl['menu1']['citem']
            );
        } else {
            $dm .= preg_replace(
                array('/%ref%/','/%ttl%/','/%refpar%/'),
                array('catalog.html',$v['ttl'],'%refpar%&pgid=' . $k),
                $tpl['menu1']['item']
            );
        }
    }
    $pagemenu = preg_replace('/%menu%/', $dm, $tpl['menu1']['main']);

// Выполнение команд

    $text = '';
    foreach ($_REQUEST as $k => $v) {
        if (preg_match('/^text_(.*)/', $k, $w)) {
            $text[$w[1]]['text'] = $v;
        }
    }

    switch ($cmd) {
        case 'Resize':
        {    $images = $_REQUEST['images'];
            $tables = $_REQUEST['tables'];
            $cmd = 'Edit';
            break;
        }
        case 'doDelItem':
        {    if (!empty($iid)) {
                $iref = $image_dir . '/' . $img_prefix . $iid . '.jpg';
                if (file_exists($iref)) {
                    unlink($iref);
                }

                delete_fields(-$iid);

                $mysqli->query('delete from catalog_items where id=' . $iid);
                $mysqli->query('delete from catalog_item_params where iid=' . $iid);

                $cmd = '';
        }
            break;
        }
        case 'doDelGroup':
        {    if (!empty($gid)) {
                $mysqli->query('delete from catalog_groups where id=' . $gid);
                $mysqli->query('delete from catalog_group_params where gid=' . $gid);

                $cmd = '';
        }
            break;
        }
        case 'doDelImage':
        {    if (!empty($iid)) {
                foreach ($delImages as $w) {
                    if ($w == '1') {
                        $w = '';
                    }
                    $iref = $image_dir . '/' . $img_prefix . $iid . $w . '.jpg';
                    if (file_exists($iref)) {
                        unlink($iref);
                    }
                }
                $cmd = 'EditItem';
        }
                break;
        }
        case 'doAddGroup':
        {    $name = $_REQUEST['name'];
            $img_prefix = $_REQUEST['img_prefix'];
            if (empty($img_prefix)) {
                $img_prefix = DEFAULT_IMG_PREFIX;
            }
            $img_dir = $_REQUEST['img_dir'];
            if (empty($img_dir)) {
                $img_dir = DEFAULT_IMG_DIR;
            }
            if (empty($name)) {
                $name = '<Новая группа>';
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from catalog_groups where pgid=$pgid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }
            $img_count = $_REQUEST['img_count'] + 0;
            $tpl_prefix = trim($_REQUEST['tpl_prefix']);
            $tpl_adm_prefix = trim($_REQUEST['tpl_adm_prefix']);


            if ($mysqli->query('insert into catalog_groups set pgid=' . $pgid . ', ord=' . $ord . ', name="' . $name . '", img_prefix="' . $img_prefix . '", img_dir="' . $img_dir . '", img_count=' . $img_count . ', tpl_prefix="' . $tpl_prefix . '", tpl_adm_prefix="' . $tpl_adm_prefix . '"')) {
                $gid = $mysqli->insert_id;
                $cmd = '';
            } else {
                $cmd = 'AddGroup';
            }
            break;
        }
        case 'doEditGroup':
        {    $name = $_REQUEST['name'];
            $img_prefix = $_REQUEST['img_prefix'];
            if (empty($img_prefix)) {
                $img_prefix = DEFAULT_IMG_PREFIX;
            }
            $img_dir = $_REQUEST['img_dir'];
            if (empty($img_dir)) {
                $img_dir = DEFAULT_IMG_DIR;
            }
            if (empty($name)) {
                $name = '<Новая группа>';
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from catalog_groups where pgid=$pgid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }
            $img_count = $_REQUEST['img_count'] + 0;
            $tpl_prefix = trim($_REQUEST['tpl_prefix']);
            $tpl_adm_prefix = trim($_REQUEST['tpl_adm_prefix']);

            if ($mysqli->query('update catalog_groups set pgid=' . $pgid . ', ord=' . $ord . ', name="' . $name . '", img_prefix="' . $img_prefix . '", img_dir="' . $img_dir . '", img_count=' . $img_count . ', tpl_prefix="' . $tpl_prefix . '", tpl_adm_prefix="' . $tpl_adm_prefix . '" where id=' . $gid)) {
                $cmd = '';
            } else {
                $cmd = 'EditGroup';
            }
            break;
        }
        case 'doAddItem':
        {    $subtype = $_REQUEST['subtype'] + 0;
            if (empty($subtype)) {
                $subtype = 1;
            }
            $name = stripslashes($_REQUEST['name']);
            if (empty($name)) {
                $name = '<Новый товар>';
            }
            $descr = stripslashes($_REQUEST['descr']);
            $ord = $_REQUEST['ord'] + 0;
            $rate = $_REQUEST['rate'] + 0;
            if ($rate > 10) {
                $rate = 10;
            }
            if ($rate < 1) {
                $rate = 1;
            }

            $subttl1 = stripslashes($_REQUEST['subttl1']);
            $subttl2 = stripslashes($_REQUEST['subttl2']);

            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from catalog_items where gid=$gid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }


            if ($mysqli->query('insert into catalog_items set gid=' . $gid . ', subtype=' . $subtype . ', ord=' . $ord . ', name="' . addslashes($name) . '", descr="' . addslashes($descr) . '", rate=' . $rate . ', subttl1="' . addslashes($subttl1) . '", subttl2="' . addslashes($subttl2) . '"')) {
                $iid = $mysqli->insert_id;
                // Основные картинки
                $image_file = $_FILES['imagefile'];
                if (is_uploaded_file($image_file['tmp_name'])) {
                    if ($image_size = @getimagesize($image_file['tmp_name'])) {
                        $iref = $image_dir . '/' . $img_prefix . $iid . '.jpg';
                        copy($image_file['tmp_name'], $iref);
                        chown($iref, 'apache');
                        chmod($iref, 0775);
                    }
                }
                $simage_file = $_FILES['simagefile'];
                if (is_uploaded_file($simage_file['tmp_name'])) {
                    if ($simage_size = @getimagesize($simage_file['tmp_name'])) {
                        $siref = $image_dir . '/' . $img_prefix . $iid . 's1.jpg';
                        copy($image_file['tmp_name'], $siref);
                        chown($siref, 'apache');
                        chmod($siref, 0775);
                    }
                }
                $bimage_file = $_FILES['bimagefile'];
                if (is_uploaded_file($bimage_file['tmp_name'])) {
                    if ($bimage_size = @getimagesize($bimage_file['tmp_name'])) {
                        $biref = $image_dir . '/' . $img_prefix . $iid . 'b1.jpg';
                        copy($image_file['tmp_name'], $biref);
                        chown($biref, 'apache');
                        chmod($biref, 0775);
                    }
                }
                // Дополнительные картинки

                for ($image_num = 2; $image_num < 9; $image_num++) {
                    $simage_file = $_FILES['simagefile' . $image_num];
                    if (is_uploaded_file($simage_file['tmp_name'])) {
                        if ($simage_size = @getimagesize($simage_file['tmp_name'])) {
                            $siref = $image_dir . '/' . $img_prefix . $iid . 's' . $image_num . '.jpg';
                            copy($simage_file['tmp_name'], $siref);
                            chown($siref, 'apache');
                            chmod($siref, 0775);
                        }
                    }
                    $bimage_file = $_FILES['bimagefile' . $image_num];
                    if (is_uploaded_file($bimage_file['tmp_name'])) {
                        if ($bimage_size = @getimagesize($bimage_file['tmp_name'])) {
                            $biref = $image_dir . '/' . $img_prefix . $iid . 'b' . $image_num . '.jpg';
                            copy($bimage_file['tmp_name'], $biref);
                            chown($biref, 'apache');
                            chmod($biref, 0775);
                        }
                    }
                }

                update_fields(-$iid, $subtype);

                // Дополнительные параметры

                $extra_params = $_REQUEST['param1'];
                if (is_array($extra_params)) {
                    foreach ($extra_params as $k => $v) {
                        if (!empty($v)) {
                            $mysqli->query('insert into catalog_item_params set iid=' . $iid . ', pid=' . $k . ', pval="' . $v . '"');
                        }
                    }
                }

                $extra_params = $_REQUEST['param2'];
                if (is_array($extra_params)) {
                    foreach ($extra_params as $k => $v) {
                        if ($v > 0) {
                            $mysqli->query('insert into catalog_item_params set iid=' . $iid . ', pid=' . $k . ', pval="' . $v . '"');
                        }
                    }
                }

                $extra_params = $_REQUEST['param3'];
                if (is_array($extra_params)) {
                    foreach ($extra_params as $k => $v) {
                        if ($v == 1) {
                            $mysqli->query('insert into catalog_item_params set iid=' . $iid . ', pid=' . $k . ', pval="' . $v . '"');
                        }
                    }
                }

                $cmd = '';
            } else {
                $cmd = 'AddItem';
            }
            break;
        }
        case 'doEditItem':
        {    $subtype = $_REQUEST['subtype'] + 0;
            if (empty($subtype)) {
                $subtype = 1;
            }
            $name = stripslashes($_REQUEST['name']);
            if (empty($name)) {
                $name = '<Новый товар>';
            }
            $descr = stripslashes($_REQUEST['descr']);
            $ord = $_REQUEST['ord'] + 0;
            $rate = $_REQUEST['rate'] + 0;
            if ($rate > 10) {
                $rate = 10;
            }
            if ($rate < 1) {
                $rate = 1;
            }
            $subttl1 = stripslashes($_REQUEST['subttl1']);
            $subttl2 = stripslashes($_REQUEST['subttl2']);
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from catalog_items where gid=$gid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }
            if ($mysqli->query('update catalog_items set gid=' . $gid . ', subtype=' . $subtype . ', ord=' . $ord . ', name="' . addslashes($name) . '", descr="' . addslashes($descr) . '", rate=' . $rate . ', subttl1="' . addslashes($subttl1) . '", subttl2="' . addslashes($subttl2) . '" where id=' . $iid)) {
                $image_file = $_FILES['imagefile'];

                // Основные картинки
                $image_file = $_FILES['imagefile'];
                if (is_uploaded_file($image_file['tmp_name'])) {
                    if ($image_size = @getimagesize($image_file['tmp_name'])) {
                        $iref = $image_dir . '/' . $img_prefix . $iid . '.jpg';
                        copy($image_file['tmp_name'], $iref);
                        chown($iref, 'apache');
                        chmod($iref, 0775);
                    }
                }
                $simage_file = $_FILES['simagefile'];
                if (is_uploaded_file($simage_file['tmp_name'])) {
                    if ($simage_size = @getimagesize($simage_file['tmp_name'])) {
                        $siref = $image_dir . '/' . $img_prefix . $iid . 's1.jpg';
                        copy($simage_file['tmp_name'], $siref);
                        chown($siref, 'apache');
                        chmod($siref, 0775);
                    }
                }
                $bimage_file = $_FILES['bimagefile'];
                if (is_uploaded_file($bimage_file['tmp_name'])) {
                    if ($bimage_size = @getimagesize($bimage_file['tmp_name'])) {
                        $biref = $image_dir . '/' . $img_prefix . $iid . 'b1.jpg';
                        copy($bimage_file['tmp_name'], $biref);
                        chown($biref, 'apache');
                        chmod($biref, 0775);
                    }
                }
                // Дополнительные картинки

                for ($image_num = 2; $image_num < 9; $image_num++) {
                    $simage_file = $_FILES['simagefile' . $image_num];
                    if (is_uploaded_file($simage_file['tmp_name'])) {
                        if ($simage_size = @getimagesize($simage_file['tmp_name'])) {
                            $siref = $image_dir . '/' . $img_prefix . $iid . 's' . $image_num . '.jpg';
                            copy($simage_file['tmp_name'], $siref);
                            chown($siref, 'apache');
                            chmod($siref, 0775);
                        }
                    }
                    $bimage_file = $_FILES['bimagefile' . $image_num];
                    if (is_uploaded_file($bimage_file['tmp_name'])) {
                        if ($bimage_size = @getimagesize($bimage_file['tmp_name'])) {
                            $biref = $image_dir . '/' . $img_prefix . $iid . 'b' . $image_num . '.jpg';
                            copy($bimage_file['tmp_name'], $biref);
                            chown($biref, 'apache');
                            chmod($biref, 0775);
                        }
                    }
                }

                update_fields(-$iid, $subtype);

                // Дополнительные параметры

                $mysqli->query('delete from catalog_item_params where iid=' . $iid);

                $extra_params = $_REQUEST['param1'];
                if (is_array($extra_params)) {
                    foreach ($extra_params as $k => $v) {
                        if (!empty($v)) {
                            $mysqli->query('insert into catalog_item_params set iid=' . $iid . ', pid=' . $k . ', pval="' . $v . '"');
                        }
                    }
                }

                $extra_params = $_REQUEST['param2'];
                if (is_array($extra_params)) {
                    foreach ($extra_params as $k => $v) {
                        if ($v > 0) {
                            $mysqli->query('insert into catalog_item_params set iid=' . $iid . ', pid=' . $k . ', pval="' . $v . '"');
                        }
                    }
                }

                $extra_params = $_REQUEST['param3'];
                if (is_array($extra_params)) {
                    foreach ($extra_params as $k => $v) {
                        if (($v == 1) || ($v == 'on')) {
                            $mysqli->query('insert into catalog_item_params set iid=' . $iid . ', pid=' . $k . ', pval="' . $v . '"');
                        }
                    }
                }

                $cmd = '';
            } else {
                $cmd = 'EditItem';
            }

            break;
        }
        case 'doDetail':
        {    $images = $_REQUEST['images'];
            $tables = $_REQUEST['tables'];
            // Обновление текстовых полей
            if (is_array($text)) {
                foreach ($text as $k => $v) {
                    $res = $mysqli->query('select id from fields_text where (name="' . $k . '") AND (pgid=' . (-$iid) . ')');
                    if ($row = $res->fetch_row()) {
                        $field_id = $row[0];
                        $txt = stripslashes($v['text']);
                        $mysqli->query('update fields_text set txt="' . addslashes($txt) . '" where id=' . $field_id);
                    }
                }
            }
            // Обновление картинок
            if (is_array($images)) {
                foreach ($images as $k => $v) {
                    $res = $mysqli->query('select id,tp from fields_image where (name="' . $k . '") AND (pgid=' . (-$iid) . ')');
                    if ($row = $res->fetch_row()) {
                        $image_id = $row[0];
                        $image_tp = $row[1];
                        $image_w = ($v['w'] + 0) > 0 ? $v['w'] + 0 : 0;
                        $image_h = ($v['h'] + 0) > 0 ? $v['h'] + 0 : 0;
                        $image_alt = trim(addslashes(stripslashes($v['alt'])));
                        $image_attr = trim(addslashes(stripslashes($v['attr'])));
                        $image_file = $_FILES['imagefile_' . $k];
                        if (is_uploaded_file($image_file['tmp_name'])) {
                            if ($image_size = @getimagesize($image_file['tmp_name'])) {
                                $old_iref = $image_dir . '/img' . $image_id . '.' . $image_tp;
                                $image_tp = $_image_exts[$image_size[2]];
                                $new_iref = $image_dir . '/img' . $image_id . '.' . $image_tp;
                                if (file_exists($old_iref)) {
                                    unlink($old_iref);
                                }
                                copy($image_file['tmp_name'], $new_iref);
                                chown($new_iref, 'apache');
                                chmod($new_iref, 0775);
                                if ($image_w == 0) {
                                    $image_w = $image_size[0];
                                }
                                if ($image_h == 0) {
                                    $image_h = $image_size[1];
                                }
                                $image_uploaded = true;
                            } else {
                                $image_uploaded = false;
                            }
                        } else {
                            $image_uploaded = false;
                        }

                        $mysqli->query(
                            'update fields_image set alt="' . $image_alt . '", attr="' . $image_attr . '"' .
                            (($image_w > 0) ? ', w=' . $image_w : '') .
                            (($image_h > 0) ? ', h=' . $image_h : '') .
                            (($image_uploaded) ? ', tp="' . $image_tp . '"' : '') .
                            ' where id=' . $image_id
                        );
                    }
                }
            }

            // Обновление таблиц

            if (is_array($tables)) {
                foreach ($tables as $k => $v) {
                    $res = $mysqli->query('select id from fields_table where (name="' . $k . '") AND (pgid=' . (-$iid) . ')');
                    if ($row = $res->fetch_row()) {
                        $field_id = $row[0];
                        $row_count = $v['rows'] + 0;
                        $col_count = $v['cols'] + 0;
                        $table_tp = $v['tp'] + 0;
                        $rows = $v['cells'];

                        $cells = '';
                        for ($rn = 0; $rn < $row_count; $rn++) {
                            $w = '';
                            for ($cn = 0; $cn < $col_count; $cn++) {
                                $w .= ($cn > 0 ? '::' : '') . $rows[$rn][$cn];
                            }
                            $cells .= ($rn > 0 ? '::::' : '') . $w;
                        }
                        $mysqli->query('update fields_table set rows=' . $row_count . ', cols=' . $col_count .
                        ', tp=' . $table_tp . ', txt="' . addslashes(stripslashes($cells)) . '" where id=' . $field_id);
                    }
                }
            }
            $cmd = '';
            break;
        }

// Lots

        case 'doAddLot':
        {    $code = $_REQUEST['code'];
            $name = $_REQUEST['name'];
            $descr = $_REQUEST['descr'];
            $price = $_REQUEST['price'] + 0;
            $active = $_REQUEST['active'] + 0;
            if (empty($name)) {
                $name = '<Новый лот>';
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(l.ord) from catalog_lots as l left join catalog_lot_items as li on (li.lid=l.id) where li.iid=$iid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('insert into catalog_lots set active=' . $active . ', price=' . $price . ', ord=' . $ord . ', name="' . $name . '", code="' . $code . '", descr="' . $descr . '"')) {
                $lid = $mysqli->insert_id;
                $mysqli->query('insert into catalog_lot_items set iid=' . $iid . ', lid=' . $lid . ', ord=10, amount=1');

                $cmd = 'Lots';
            } else {
                $cmd = 'AddLot';
            }
            break;
        }
        case 'doEditLot':
        {    $code = $_REQUEST['code'];
            $name = $_REQUEST['name'];
            $descr = $_REQUEST['descr'];
            $price = $_REQUEST['price'] + 0;
            $active = $_REQUEST['active'] + 0;
            if (empty($name)) {
                $name = '<Новый лот>';
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(l.ord) from catalog_lots as l left join catalog_lot_items as li on (li.lid=l.id) where li.iid=$iid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('update catalog_lots set active=' . $active . ', price=' . $price . ', ord=' . $ord . ', name="' . $name . '", code="' . $code . '", descr="' . $descr . '" where id=' . $lid)) {
                $cmd = 'Lots';
            } else {
                $cmd = 'EditLot';
            }
            break;
        }

        case 'doAddLotItem':
        {    $amount = $_REQUEST['amount'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from catalog_lot_items where (lid=$lid)");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }

            if ($mysqli->query('insert into catalog_lot_items set iid=' . $liid . ', lid=' . $lid . ', ord=' . $ord . ', amount=' . $amount)) {
                $liid = $mysqli->insert_id;
                $cmd = 'Lots';
            } else {
                $cmd = 'AddLotItem';
            }
            break;
        }

        case 'doEditLotItem':
        {    $amount = $_REQUEST['amount'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $res = $mysqli->query("select max(ord) from catalog_lot_items where iid=$iid");
                $row = $res->fetch_row();
                $ord = $row[0] + 10;
            }
            $oldliid = $_REQUEST['oldliid'];
            if ($mysqli->query('update catalog_lot_items set iid=' . $liid . ', ord=' . $ord . ', amount=' . $amount . ' where (iid=' . $oldliid . ') AND (lid=' . $lid . ')')) {
                $cmd = 'Lots';
            } else {
                $cmd = 'EditLotItem';
            }
            break;
        }

        case 'doDelLot':
        {    if (!empty($lid)) {
                $mysqli->query('delete from catalog_lot_items where lid=' . $lid);
                $mysqli->query('delete from catalog_lots where id=' . $lid);
                $cmd = 'Lots';
        }
            break;
        }

        case 'doDelLotItem':
        {    if (!empty($lid)) {
                $mysqli->query('delete from catalog_lot_items where (lid=' . $lid . ') AND (iid=' . $liid . ')');
                $cmd = 'Lots';
        }
            break;
        }

        case 'UpLot':
        {    $res = $mysqli->query('select ord from catalog_lots where id=' . $lid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $uplid = $_REQUEST['uplid'];
                $res = $mysqli->query('select ord from catalog_lots where id=' . $uplid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update catalog_lots set ord=' . $upord . ' where id=' . $lid);
                    $mysqli->query('update catalog_lots set ord=' . $ord . ' where id=' . $uplid);
                }
            }
            $cmd = 'Lots';
            break;
        }

        case 'UpLotItem':
        {    $res = $mysqli->query('select ord from catalog_lot_items where (iid=' . $liid . ') AND (lid=' . $lid . ')');
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upliid = $_REQUEST['upliid'];
                $res = $mysqli->query('select ord from catalog_lot_items where (iid=' . $upliid . ') AND (lid=' . $lid . ')');
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update catalog_lot_items set ord=' . $upord . ' where (iid=' . $liid . ') AND (lid=' . $lid . ')');
                    $mysqli->query('update catalog_lot_items set ord=' . $ord . ' where (iid=' . $upliid . ') AND (lid=' . $lid . ')');
                }
            }
            $cmd = 'Lots';
            break;
        }

// ---------------------------------- /Lots

        case 'UpItem':
        {    $res = $mysqli->query('select ord from catalog_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from catalog_items where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update catalog_items set ord=' . $upord . ' where id=' . $iid);
                    $mysqli->query('update catalog_items set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }
        case 'SortItems':
        {    if ($res = $mysqli->query('select id,name from catalog_items where gid=' . $gid)) {
                while ($row = $res->fetch_row()) {
                    $item_ids[$row[1]] = $row[0];
                }
                ksort($item_ids, SORT_STRING);
                $cord = 10;
                foreach ($item_ids as $k => $v) {
                    $mysqli->query('update catalog_items set ord=' . $cord . ' where id=' . $v);
                    $cord += 10;
                }
        }
            $cmd = '';
            break;
        }

        case 'UpGroup':
        {    $res = $mysqli->query('select ord from catalog_groups where id=' . $gid);
            if ($row = $res->fetch_row()) {
                $ord = $row[0];
                $upid = $_REQUEST['upid'];
                $res = $mysqli->query('select ord from catalog_groups where id=' . $upid);
                if ($row = $res->fetch_row()) {
                    $upord = $row[0];
                    $mysqli->query('update catalog_groups set ord=' . $upord . ' where id=' . $gid);
                    $mysqli->query('update catalog_groups set ord=' . $ord . ' where id=' . $upid);
                }
            }
            $cmd = '';
            break;
        }

        case 'CopyGroup':
        {    if ($res = $mysqli->query('select * from catalog_groups where id=' . $gid)) {
                $row = $res->fetch_assoc();
                if ($mysqli->query('insert into catalog_groups set pgid=' . $row['pgid'] . ', ord=' . ($row['ord'] + 5) . ', name="' . ($row['name'] . '(копия)') . '", img_prefix="' . $row['img_prefix'] . '", img_dir="' . $row['img_dir'] . '", img_count=' . $row['img_count'] . ', tpl_prefix="' . $row['tpl_prefix'] . '", tpl_adm_prefix="' . $row['tpl_adm_prefix'] . '"')) {
                    $newgid = $mysqli->insert_id;
                    $res2 = $mysqli->query('select * from catalog_group_params where gid=' . $gid);
                    while ($row2 = $res2->fetch_assoc()) {
                        $mysqli->query('insert into catalog_group_params set gid=' . $newgid . ', name="' . $row2['name'] . '", title="' . $row2['title'] . '", descr="' . $row2['descr'] . '", par_tp=' . $row2['par_tp'] . ', param="' . $row2['param'] . '"');
                    }
                    $ord = 10;
                    $res2 = $mysqli->query('select id from catalog_groups where pgid=' . $pgid);
                    while ($row2 = $res2->fetch_row()) {
                        $mysqli->query('update catalog_groups set ord=' . $ord . ' where id=' . $row2[0]);

                        $ord += 10;
                    }
                }
        }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'EditGroup':
        {    $res = $mysqli->query('select pgid,name,ord,img_prefix,img_dir,img_count,tpl_prefix,tpl_adm_prefix from catalog_groups where id=' . $gid);
            if ($row = $res->fetch_row()) {
                $old_pgid = $row[0];
                $old_name = $row[1];
                $old_ord = $row[2];
                $old_img_prefix = $row[3];
                $old_img_dir = $row[4];
                $img_count = $row[5] + 0;
                $tpl_prefix = $row[6];
                $tpl_adm_prefix = $row[7];
            }
        }
        case 'AddGroup':
        {    if (empty($pgid)) {
                $pgid = $old_pgid;
        }
            $name = $_REQUEST['name'];
            $img_prefix = $_REQUEST['img_prefix'];
            $img_dir = $_REQUEST['img_dir'];
        if (empty($name)) {
            $name = $old_name;
        }
        if (empty($img_prefix)) {
            $img_prefix = $old_img_prefix;
        }
        if (empty($img_dir)) {
            $img_dir = $old_img_dir;
        }
            $ord = $_REQUEST['ord'] + 0;
        if (empty($ord)) {
            $ord = $old_ord;
        }
            $pgids = '';
        foreach ($catalogs as $k => $v) {
            $pgids .= '<option value=' . $k . ($pgid == $k ? ' selected' : '') . '>' . $v['ttld'] . '-' . $v['ttl'] . '</option>';
        }

        if ($cmd == 'AddGroup') {
            $img_count = 0;
            $tpl_prefix = '';
            $tpl_adm_prefix = '';
        }
        if ($_REQUEST['pass'] == 1) {
            $img_count = $_REQUEST['img_count'] + 0;
            $tpl_prefix = trim($_REQUEST['tpl_prefix']);
            $tpl_adm_prefix = trim($_REQUEST['tpl_adm_prefix']);
        }

            $body = preg_replace(
                array('/%gid%/','/%pgids%/','/%name%/','/%ord%/','/%img_prefix%/','/%img_dir%/',
                    '/%img_count%/','/%tpl_prefix%/','/%tpl_adm_prefix%/'),
                array($gid,$pgids,$name,$ord,$img_prefix,$img_dir,$img_count,$tpl_prefix,$tpl_adm_prefix),
                $tpl['catalog']['editGroup']
            );
        break;
        }
        case 'EditItem':
        {    $res = $mysqli->query('select gid,subtype,name,descr,ord,rate,subttl1,subttl2,price,param,active from catalog_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $old_gid = $row[0];
                $old_subtype = $row[1];
                $old_name = $row[2];
                $old_descr = $row[3];
                $old_ord = $row[4];
                $old_rate = $row[5];
                $old_subttl1 = $row[6];
                $old_subttl2 = $row[7];
                $iref = $image_dir . '/' . $img_prefix . $iid . '.jpg';

                // extra params

                $res2 = $mysqli->query('select pid,pval from catalog_item_params where iid=' . $iid);
                while ($row2 = $res2->fetch_row()) {
                    $old_param_vals[$row2[0]] = $row2[1];
                }
            }
        }
        case 'AddItem':
        {
            $res = $mysqli->query('select id,name from catalog_groups where pgid=' . $pgid);
            $first_group = 0;
            while ($row = $res->fetch_row()) {
                $gids .= '<option value=' . $row[0] . ($gid == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
                if (empty($first_group)) {
                    $first_group = $row[0];
                }
            }

            if (empty($gid)) {
                $gid = $old_gid;
            }
            if (empty($gid)) {
                $gid = $first_group;
            }

            // @@@@@@@@@@@@@@@@@@@@@@@
            $res2 = $mysqli->query('select tpl_adm_prefix, img_count from catalog_groups where id=' . $gid);
            if ($row2 = $res2->fetch_row()) {
                if (!empty($row2[0])) {
                    $tpl_prefix = $row2[0] . '_';
                } else {
                    $tpl_prefix = '';
                }
                $img_count = $row2[1];
            }
            // @@@@@@@@@@@@@@@@@@@@@@@

            if (empty($tpl[$tpl_prefix . 'catalog']['extra_image_line'])) {
                $tpl[$tpl_prefix . 'catalog']['extra_image_line'] = $tpl['catalog']['extra_image_line'];
            }
            if (empty($tpl[$tpl_prefix . 'catalog']['delImageBtn'])) {
                $tpl[$tpl_prefix . 'catalog']['delImageBtn'] = $tpl['catalog']['delImageBtn'];
            }
            if (empty($tpl[$tpl_prefix . 'catalog']['editItem'])) {
                $tpl[$tpl_prefix . 'catalog']['editItem'] = $tpl['catalog']['editItem'];
            }
            if (empty($tpl[$tpl_prefix . 'catalog']['extraParam1'])) {
                $tpl[$tpl_prefix . 'catalog']['extraParam1'] = $tpl['catalog']['extraParam1'];
            }
            if (empty($tpl[$tpl_prefix . 'catalog']['extraParam2'])) {
                $tpl[$tpl_prefix . 'catalog']['extraParam2'] = $tpl['catalog']['extraParam2'];
            }
            if (empty($tpl[$tpl_prefix . 'catalog']['extraParam3'])) {
                $tpl[$tpl_prefix . 'catalog']['extraParam3'] = $tpl['catalog']['extraParam3'];
            }
            if (empty($tpl[$tpl_prefix . 'catalog']['extraParam4'])) {
                $tpl[$tpl_prefix . 'catalog']['extraParam4'] = $tpl['catalog']['extraParam4'];
            }

            $subtype = $_REQUEST['subtype'] + 0;
            if (empty($subtype)) {
                $subtype = $old_subtype;
            }
            $name = $_REQUEST['name'];
            if (empty($name)) {
                $name = $old_name;
            }
            $descr = $_REQUEST['descr'];
            if (empty($descr)) {
                $descr = $old_descr;
            }
            $ord = $_REQUEST['ord'] + 0;
            if (empty($ord)) {
                $ord = $old_ord;
            }
            $rate = $_REQUEST['rate'] + 0;
            if (($rate < 1) || ($rate > 10)) {
                $rate = $old_rate;
            }
            $subttl1 = $_REQUEST['subttl1'];
            if (empty($subttl1)) {
                $subttl1 = $old_subttl1;
            }
            $subttl2 = $_REQUEST['subttl2'];
            if (empty($subttl2)) {
                $subttl2 = $old_subttl2;
            }


            $subtypes = '<option value=-1' . ($subtype == -1 ? ' selected' : '') . '>Без подробного описания</option>';
            $res = $mysqli->query('select id,name from pages_text');
            while ($row = $res->fetch_row()) {
                $subtypes .= '<option value=' . $row[0] . ($subtype == $row[0] ? ' selected' : '') . '>' . $row[1] . '</option>';
            }

            // Основные картинки

            $iref = 'img/' . $image_ref . '/' . $img_prefix . $iid . '.jpg';
            if (file_exists($iref)) {
                $ibtn = preg_replace('/%id%/', '1', $tpl[$tpl_prefix . 'catalog']['delImageBtn']);
            } else {
                $iref = 'aimg/noimage.gif';
                $ibtn = '';
            }

            $siref[1] = 'img/' . $image_ref . '/' . $img_prefix . $iid . 's1.jpg';
            if (file_exists($siref[1])) {
                $sibtn[1] = preg_replace('/%id%/', 's1', $tpl[$tpl_prefix . 'catalog']['delImageBtn']);
            } else {
                $siref[1] = 'aimg/noimage.gif';
                $sibtn[1] = '';
            }

            $biref[1] = 'img/' . $image_ref . '/' . $img_prefix . $iid . 'b1.jpg';
            if (file_exists($biref[1])) {
                $bibtn[1] = preg_replace('/%id%/', 'b1', $tpl[$tpl_prefix . 'catalog']['delImageBtn']);
            } else {
                $biref[1] = 'aimg/noimage.gif';
                $bibtn[1] = '';
            }

            // Дополнительные картинки

            for ($image_num = 2; $image_num < 9; $image_num++) {
                $siref[$image_num] = 'img/' . $image_ref . '/' . $img_prefix . $iid . 's' . $image_num . '.jpg';
                if (file_exists($siref[$image_num])) {
                    $sibtn[$image_num] = preg_replace('/%id%/', 's' . $image_num, $tpl[$tpl_prefix . 'catalog']['delImageBtn']);
                } else {
                    $siref[$image_num] = 'aimg/noimage.gif';
                    $sibtn[$image_num] = '';
                }

                $biref[$image_num] = 'img/' . $image_ref . '/' . $img_prefix . $iid . 'b' . $image_num . '.jpg';
                if (file_exists($biref[$image_num])) {
                    $bibtn[$image_num] = preg_replace('/%id%/', 'b' . $image_num, $tpl[$tpl_prefix . 'catalog']['delImageBtn']);
                } else {
                    $biref[$image_num] = 'aimg/noimage.gif';
                    $bibtn[$image_num] = '';
                }
            }

            $descr = stripslashes(preg_replace("/\n|\r/", '', $descr));

            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $txt1 = $CKE->editor('descr', $descr);

            $descr = $sw->textareaAttributes; # Was $sw->getHtml();

            $descr = preg_replace('/[\\\][\\\]n/', '', $descr);

            $extra_images = '';
            for ($image_num = 2; $image_num < 9; $image_num++) {
                $extra_images .= preg_replace(
                    array('/%sdelbtn%/','/%simage%/','/%bdelbtn%/','/%bimage%/','/%num%/'),
                    array($sibtn[$image_num],$siref[$image_num] . '?rnd=' . rand(),
                        $bibtn[$image_num],$biref[$image_num] . '?rnd=' . rand(),$image_num),
                    $tpl[$tpl_prefix . 'catalog']['extra_image_line']
                );
            }

            // Дополнительные параметры
            $extra_params = '';
            $res2 = $mysqli->query('select id,name,title,par_tp,param from catalog_group_params where gid=' . $gid);
            while ($row2 = $res2->fetch_row()) {
                switch ($row2[3]) {
                    case 2:
                    {    $options = '<option value=-1' . (empty($old_param_vals[$row2[0]]) ? ' selected ' : '' ) . '>Не задано</option> ';
                        $aoptions = explode(',', $row2[4]);
                        foreach ($aoptions as $k => $v) {
                            $options .= '<option value=' . ($k + 1) . (($old_param_vals[$row2[0]] == ($k + 1)) ? ' selected ' : '' ) . '>' . $v . '</option> ';
                        }
                        $extra_params .= preg_replace(
                            array('/%pid%/','/%name%/','/%title%/','/%options%/'),
                            array($row2[0],$row2[1],$row2[2],$options),
                            $tpl[$tpl_prefix . 'catalog']['extraParam2']
                        );
                                break;
                    }
                    case 3:
                    {    $extra_params .= preg_replace(
                        array('/%pid%/','/%name%/','/%title%/','/%value%/'),
                        array($row2[0],$row2[1],$row2[2],$old_param_vals[$row2[0]] ? ' checked ' : ''),
                        $tpl[$tpl_prefix . 'catalog']['extraParam3']
                    );
                                break;
                    }
                    case 5:
                    {    $options = '<option value=-1' . (empty($old_param_vals[$row2[0]]) ? ' selected ' : '' ) . '>Не задано</option> ';
                        $res3 = $mysqli->query('select id,param from catalog_group_par_lookup where grp=' . $row2[4]);
                        while ($row3 = $res3->fetch_row()) {
                            $options .= '<option value=' . ($row3[0]) . (($old_param_vals[$row2[0]] == $row3[0]) ? ' selected ' : '' ) . '>' . $row3[1] . '</option> ';
                        }
                        $extra_params .= preg_replace(
                            array('/%pid%/','/%name%/','/%title%/','/%options%/'),
                            array($row2[0],$row2[1],$row2[2],$options),
                            $tpl[$tpl_prefix . 'catalog']['extraParam2']
                        );
                                break;
                    }
                    case 6:
                    {    $extra_params .= preg_replace(
                        array('/%pid%/','/%name%/','/%title%/','/%value%/','/%rows%/'),
                        array($row2[0],$row2[1],$row2[2],$old_param_vals[$row2[0]],$row2[4]),
                        $tpl[$tpl_prefix . 'catalog']['extraParam4']
                    );
                                break;
                    }
                    default:
                    {    $extra_params .= preg_replace(
                        array('/%pid%/','/%name%/','/%title%/','/%value%/'),
                        array($row2[0],$row2[1],$row2[2],$old_param_vals[$row2[0]]),
                        $tpl[$tpl_prefix . 'catalog']['extraParam1']
                    );
                    }
                }
            }

            $body = preg_replace(
                array('/%iid%/','/%gids%/','/%subtypes%/','/%name%/','/%descr%/','/%ord%/','/%image%/',
                    '/%delbtn%/','/%rate%/','/%subttl1%/','/%subttl2%/','/%sdelbtn%/','/%simage%/',
                    '/%bdelbtn%/','/%bimage%/','/%extra_images%/','/%extra_params%/'
                    ),
                array($iid,$gids,$subtypes,$name,$descr,$ord,$iref . '?rnd=' . rand(),$ibtn,$rate,$subttl1,$subttl2,
                    $sibtn[1],$siref[1] . '?rnd=' . rand(),
                    $bibtn[1],$biref[1] . '?rnd=' . rand(),$extra_images,$extra_params
                    ),
                $tpl[$tpl_prefix . 'catalog']['editItem']
            );

            break;
        }
        case 'EditDetail':
        {    // Получаем текстовые поля

            $res = $mysqli->query('select name,txt,title from fields_text where pgid=' . -($iid));
            while ($row = $res->fetch_row()) {
                if (empty($text[$row[0]])) {
                    $text[$row[0]]['text'] = stripslashes($row[1]);
                }
                $text[$row[0]]['title'] = stripslashes($row[2]);
                $text[$row[0]]['ok'] = true;
            }

            // Получаем поля картинки
            $res = $mysqli->query('select name,id,w,h,tp,alt,attr,title from fields_image where pgid=' . (-$iid));
            while ($row = $res->fetch_row()) {
                if (empty($images[$row[0]])) {
                    unset($w);
                    $w['w'] = $row[2];
                    $w['h'] = $row[3];
                    $w['alt'] = $row[5];
                    $w['attr'] = $row[6];
                    $images[$row[0]] = $w;
                }
                $images[$row[0]]['id'] = $row[1];
                $images[$row[0]]['tp'] = $row[4];

                $images[$row[0]]['title'] = $row[7];

                $iref = $image_dir . '/img' . $images[$row[0]]['id'] . '.' . $images[$row[0]]['tp'];

                if (file_exists($iref)) {
                    $rnd = md5(microtime());
                    $images[$row[0]]['loaded'] = true;
                    $images[$row[0]]['ref'] = $iref . "?$rnd";
                } else {
                    $images[$row[0]]['loaded'] = false;
                    $images[$row[0]]['ref'] = $image_dir . '/noimage.gif';
                }
                $images[$row[0]]['ok'] = true;
            }

            // Получаем поля таблицы

            $res = $mysqli->query('select t.name,t.cols,t.rows,t.txt,t.title,f.id from fields_table as t left join types_table as f on (f.id=t.tp) where t.pgid=' . (-$iid));
            while ($row = $res->fetch_row()) {
                if (empty($tables[$row[0]])) {
                    $tables[$row[0]]['cols'] = $row[1];
                    $tables[$row[0]]['rows'] = $row[2];
                    $tables[$row[0]]['tp'] = $row[5];
                    $wr = explode('::::', $row[3]);
                    $r = 0;
                    unset($cells);
                    foreach ($wr as $v) {
                        $cells[$r++] = explode('::', $v);
                    }
                    $tables[$row[0]]['cells'] = stripslashes($cells);
                }
                $tables[$row[0]]['title'] = $row[4];
                $tables[$row[0]]['ok'] = true;
            }

            // Получаем типы таблиц

            $res = $mysqli->query('select id,name from types_table');
            while ($row = $res->fetch_row()) {
                $table_types[$row[0]] = $row[1];
            }

            // Выводим текстовые поля

            $text_fields = '';
            $text_field_js = '';

            if (is_array($text)) {
                foreach ($text as $k => $v) {
                    if ($v['ok']) {
                        $v['text'] = stripslashes(preg_replace("/\n|\r/", '', $v['text']));

                        $CKE = new CKEditor();
                        $CKE->returnOutput = true;

                        $txt1 = $CKE->editor('text_' . $k, $v['text']);

                        $txt = $sw->textareaAttributes; # Was $txt=$sw->textareaAttributes; # Was $sw->getHtml();

                        $txt = preg_replace('/[\\\][\\\]n/', '', $txt);

                        $text_fields .= preg_replace(
                            array('/%text%/','/%title%/'),
                            array($txt,$v['title']),
                            $tpl['pages']['textfield']
                        );
                    }
                }
            }

            // Выводим картинки

            $image_fields = '';

            if (is_array($images)) {
                foreach ($images as $k => $v) {
                    if ($v['ok']) {
                        $image_fields .= preg_replace(
                            array('/%name%/','/%id%/','/%title%/','/%w%/','/%h%/','/%alt%/','/%attr%/','/%image%/'),
                            array($k,$v['id'],$v['title'],$v['w'],$v['h'],$v['alt'],$v['attr'],$v['ref']
                            ),
                            $tpl['pages']['imagefield']
                        );
                    }
                }
            }

            // Выводим таблицы

            $table_fields = '';

            if (is_array($tables)) {
                foreach ($tables as $k => $v) {
                    if ($v['ok']) {
                        // Начало обработки таблицы
                        $w = '';
                        $rrc = count($v['cells']);
                        for ($rn = 0; $rn < $v['rows']; $rn++) {
                            if ($rn < $rrc) {
                                $ra = $v['cells'][$rn];
                                $rcc = count($ra);
                            } else {
                                $ra = '';
                                $rcc = 0;
                            }
                            $rw = '';
                            for ($cn = 0; $cn < $v['cols']; $cn++) {
                                if ($cn < $rcc) {
                                                        $rw .= preg_replace(
                                                            array('/%text%/','/%rn%/','/%cn%/'),
                                                            array($ra[$cn],$rn,$cn),
                                                            $tpl['pages']['cell']
                                                        );
                                } else {
                                    $rw .= preg_replace(
                                        array('/%text%/','/%rn%/','/%cn%/'),
                                        array('',$rn,$cn),
                                        $tpl['pages']['cell']
                                    );
                                }
                            }
                            $w .= preg_replace(array('/%row%/','/%name%/'), array($rw,$k), $tpl['pages']['row']);
                        }

                        $table_type = '';
                        foreach ($table_types as $tk => $tv) {
                            $table_type .= '<option value="' . $tk . '"' . ($tk == $v['tp'] ? ' selected' : '') . '>' . $tv . '</option>';
                        }

                        $table_fields .= preg_replace(
                            array('/%name%/','/%title%/','/%cells%/','/%rows%/','/%cols%/','/%tp%/'),
                            array($k,$v['title'],$w,$v['rows'],$v['cols'],$table_type),
                            $tpl['pages']['tablefield']
                        );
                            //  конец обработки таблицы
                    }
                }
            }

            $body = preg_replace(
                array('/%iid%/','/%text%/','/%text_js%/','/%images%/','/%tables%/'),
                array($iid,$text_fields,$text_field_js,$image_fields,$table_fields),
                $tpl['catalog']['detail']
            );

                break;
        }
        case 'DelItem':
        {    $res = $mysqli->query('select name,descr from catalog_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%iid%/','/%name%/','/%descr%/'),
                    array($iid,$row[0],$row[1]),
                    $tpl['catalog']['delItem']
                );
                break;
            } else {
                $cmd = '';
            }
        }
        case 'DelGroup':
        {    $res = $mysqli->query('select g.name,count(i.id) from catalog_groups as g left join catalog_items as i on (i.gid=g.id) where g.id=' . $gid . ' group by g.name');
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%gid%/','/%name%/','/%count%/'),
                    array($gid,$row[0],$row[1]),
                    (($row[1] + 0) > 0) ? $tpl['catalog']['nodelGroup'] : $tpl['catalog']['delGroup']
                );
                break;
            } else {
                $cmd = '';
            }
        }

// Лоты

        case 'EditLot':
        {    $res = $mysqli->query('select code,name,descr,price,ord,active from catalog_lots where id=' . $lid);
            if ($row = $res->fetch_row()) {
                $old_code = $row[0];
                $old_name = $row[1];
                $old_descr = $row[2];
                $old_price = $row[3];
                $old_ord = $row[4];
                $old_active = $row[5];
            }
        }
        case 'AddLot':
        {    $code = $_REQUEST['code'];
            $name = $_REQUEST['name'];
            $descr = $_REQUEST['descr'];
            $price = $_REQUEST['price'];
            $ord = $_REQUEST['ord'] + 0;

            $active = (isset($_REQUEST['active'])) ? $_REQUEST['active'] + 0 : $old_active;
            if (empty($code)) {
                $code = $old_code;
            }
            if (empty($name)) {
                $name = $old_name;
            }
            if (empty($descr)) {
                $descr = $old_descr;
            }
            if (empty($price)) {
                $price = $old_price;
            }
            if (empty($ord)) {
                $ord = $old_ord;
            }

            $actives = '<select name=active size=0><option value=1' . ($active == 1 ? ' selected' : '') . '>Лот в наличии и доступен для заказа</option><option value=0' . ($active == 0 ? ' selected' : '') . '>Лота нет в наличии, он недоступен для заказа</option><option value=-1' . ($active == -1 ? ' selected' : '') . '>Лота нет и не будет в наличии, он в архиве</option></select>';

            $body = preg_replace(
                array('/%iid%/','/%lid%/','/%code%/','/%name%/','/%descr%/','/%price%/','/%ord%/','/%actives%/'),
                array($iid,$lid,$code,$name,$descr,$price,$ord,$actives),
                $tpl['catalog']['editLot']
            );
            break;
        }

        case 'EditLotItem':
        {    $res = $mysqli->query('select amount,ord from catalog_lot_items where (iid=' . $liid . ') AND (lid=' . $lid . ')');
            if ($row = $res->fetch_row()) {
                $old_amount = $row[0];
                $old_ord = $row[1];
                $xcmd = 'edit';
            }
        }
        case 'AddLotItem':
        {    $amount = $_REQUEST['amount'] + 0;
            $ord = $_REQUEST['ord'] + 0;
            if (empty($amount)) {
                $amount = $old_amount;
            }
            if (empty($ord)) {
                $ord = $old_ord;
            }
            if (empty($liid)) {
                $liid = $iid;
            }
            if (empty($xcmd)) {
                $xcmd = 'add';
            }
            $liids = '';
            $res = $mysqli->query('select g.name,i.name,i.id from catalog_groups as g left join catalog_items as i on (g.id=i.gid) where g.pgid=' . $pgid . ' order by g.ord,i.ord');
            while ($row = $res->fetch_row()) {
                $liids .= '<option value=' . $row[2] . ($liid == $row[2] ? ' selected' : '') . '>' . $row[0] . ' - ' . $row[1] . '</option>';
            }
            $body = preg_replace(
                array('/%iid%/','/%lid%/','/%liids%/','/%amount%/','/%ord%/','/%xcmd%/','/%oldliid%/'),
                array($iid,$lid,$liids,$amount,$ord,$xcmd,$liid),
                $tpl['catalog']['editLotItem']
            );
            break;
        }

        case 'Lots':
        {    // Список лотов
            $list = '';
            $even = false;
            $uplid = 0;
            $upliid = 0;
            $clid = -1;

            $res = $mysqli->query('select name from catalog_items where id=' . $iid);
            if ($row = $res->fetch_row()) {
                $item_name = $row[0];
            }

            $res = $mysqli->query('select l.id,l.name,l.active from catalog_lots as l left join catalog_lot_items as li on (l.id=li.lid) where li.iid=' . $iid . ' order by l.ord');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%iid%/','/%lid%/','/%name%/','/%uplid%/','/%active%/'),
                    array($iid,$row[0],$row[1],$uplid,$tpl['catalog']['lotactive' . $row[2]]),
                    ($uplid == 0 ? $tpl['catalog']['lotline'] : $tpl['catalog']['lotupline'])
                );
                $clid = $row[0];
                $uplid = $row[0];
                $upliid = 0;

                $res2 = $mysqli->query('select li.iid,lii.name from catalog_lot_items as li left join catalog_items as lii on (lii.id=li.iid) where li.lid=' . $clid . ' order by li.ord');
                while ($row2 = $res2->fetch_row()) {
                    $list .= preg_replace(
                        array('/%iid%/','/%lid%/','/%liid%/','/%name%/','/%bgcolor%/','/%upliid%/'),
                        array($iid,$clid,$row2[0],$row2[1],$tpl[$even ? 'even' : 'odd'],$upliid),
                        ($upliid == 0 ? $tpl['catalog']['lotitemline'] : $tpl['catalog']['lotitemupline'])
                    );
                    $even = !$even;
                    $upliid = $row2[0];
                }
            }
            $body = preg_replace(array('/%list%/','/%name%/','/%iid%/'), array($list,$item_name,$iid), $tpl['catalog']['lots']);
            break;
        }

        case 'DelLot':
        {    $res = $mysqli->query('select code,name,descr from catalog_lots where id=' . $lid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%iid%/','/%lid%/','/%code%/','/%name%/','/%descr%/'),
                    array($iid,$lid,$row[0],$row[1]),
                    $tpl['catalog']['delLot']
                );
                break;
            } else {
                $cmd = '';
            }
        }

        case 'DelLotItem':
        {    $res = $mysqli->query('select name from catalog_items where id=' . $liid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%iid%/','/%lid%/','/%liid%/','/%name%/'),
                    array($iid,$lid,$liid,$row[0]),
                    $tpl['catalog']['delLotItem']
                );
                break;
            } else {
                $cmd = '';
            }
        }

// -------------------------- / Lots

        default:
        {    // Список товаров по группам
            $list = '';
            $even = false;
            $gid = -1;
            $upiid = 0;
            $upgid = 0;
            $res = $mysqli->query('select g.id,g.name,i.id,i.name,i.active from catalog_groups as g left join catalog_items as i on (i.gid=g.id) where g.pgid=' . $pgid . ' order by g.ord,g.id,i.ord');
            while ($row = $res->fetch_row()) {
                if ($row[0] != $gid) {
                    $list .= preg_replace(
                        array('/%gid%/','/%name%/','/%upid%/'),
                        array($row[0],$row[1],$upgid),
                        ($upgid == 0 ? $tpl['catalog']['groupline'] : $tpl['catalog']['groupupline'])
                    );
                                    $gid = $row[0];
                                    $upgid = $row[0];
                                    $upiid = 0;
                }
                if ($row[2] > 0) {
                    $list .= preg_replace(
                        array('/%iid%/','/%name%/','/%bgcolor%/','/%upid%/','/%active%/'),
                        array($row[2],$row[3],$tpl[$even ? 'even' : 'odd'],$upiid,$tpl['catalog']['active' . $row[4]]),
                        ($upiid == 0 ? $tpl['catalog']['itemline'] : $tpl['catalog']['itemupline'])
                    );
                    $even = !$even;
                    $upiid = $row[2];
                }
            }
            $body = preg_replace(array('/%list%/','/%div%/','/%pgid%/'), array($list,$catalogs[$pgid]['ttld'] . '-' . $catalogs[$pgid]['ttl'],$pgid), $tpl['catalog']['main']);
        }
    }
    return $body;
}

include_once 'ckeditor/ckeditor.php'; //mark3

$body = build_page();
