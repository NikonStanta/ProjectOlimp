<?php

// Редактирование текстовых страниц

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$_image_dir,$_image_ref,$_image_exts,$mysqli;
    $image_dir = $_image_dir . '/content';
    $image_ref = $_image_ref . '/content';
    $cmd = '';

    if (($role & 1) == 0) {
        return $tpl['noaccess'];
    }

//    Определим редактируемую страницу
    if (!empty($_REQUEST['pid'])) {
        $pid = $_REQUEST['pid'];
        $res = $mysqli->query('select did,page_tp from site_pages where id=' . $pid);
        if ($row = $res->fetch_row()) {
            if (($row[1] == 2) || ($row[1] == 1)) {
                $did = $row[0];
                $cmd = 'Edit';
            } else {
                return $tpl['pages']['nopage'];
            }
        } else {
            return $tpl['pages']['nopage'];
        }
    }

    if (isset($_REQUEST['btnAdd']) && !empty($pid)) {
        $cmd = 'doEdit';
    }

    if (isset($_REQUEST['btnCancel']) && !empty($pid)) {
        $cmd = '';
        $pid = '';
    }

    if (isset($_REQUEST['btnResize']) && !empty($pid)) {
        $cmd = 'Resize';
    }


// Список разделов

    $divs = array();
    /*$res=$mysqli->query('select d.id,d.name,ISNULL(u.uid) from site_divs as d left join user_divs as u on ((u.did=d.id) AND     (u.uid='.$uid.')) order by d.ord');
    while ($row=$res->fetch_row())
    {    if ((($row[2]==0) or ($role==4)) and ($row[1]<>'Home'))
        {    $divs[$row[0]]=$row[1];
        }
    }*/
    $res = $mysqli->query('select d.id,d.name from site_divs as d order by d.ord');
    while ($row = $res->fetch_row()) {
        $divs[$row[0]] = $row[1];
    }

    if (!is_array($divs)) {
        return $tpl['pages']['noaccess'];
    }

    // Текущий раздел
    if (empty($did)) {
        if (!empty($_REQUEST['did'])) {
            $did = $_REQUEST['did'];
        } else {
            $did = key($divs);
        }
    }

    // Список разделов, к которым есть доступ
    $dm = preg_replace(
        array('/%ref%/','/%ttl%/'),
        array('','Разделы сайта'),
        $tpl['menu1']['ttl']
    );
    foreach ($divs as $k => $v) {
        if ($k == $did) {
            $dm .= preg_replace(
                array('/%ref%/','/%ttl%/','/%refpar%/'),
                array('pages.html',$v,'%refpar%&did=' . $k),
                $tpl['menu1']['citem']
            );
        } else {
            $dm .= preg_replace(
                array('/%ref%/','/%ttl%/','/%refpar%/'),
                array('pages.html',$v,'%refpar%&did=' . $k),
                $tpl['menu1']['item']
            );
        }
    }
    $pagemenu = preg_replace('/%menu%/', $dm, $tpl['menu1']['main']);

// Выполнение команд

    $text = array();
    foreach ($_REQUEST as $k => $v) {
        if (preg_match('/^text(.*)/i', $k, $w)) {
            $text[$w[0]]['text'] = $v;
        }
    }

    switch ($cmd) {
        case 'Resize':
        {    $images = $_REQUEST['images'];
            $tables = $_REQUEST['tables'];
            $cmd = 'Edit';
            break;
        }
        case 'doEdit':
        {    $images = $_REQUEST['images'];
            $tables = $_REQUEST['tables'];
            // Обновление текстовых полей
            if (is_array($text)) {
                foreach ($text as $k => $v) {
                    $res = $mysqli->query('select id from fields_text where (name="' . $k . '") AND (pgid=' . $pid . ') order by title');
                    if ($row = $res->fetch_row()) {
                        $field_id = $row[0];
                        $txt = addslashes($v['text']);
                        $mysqli->query('update fields_text set txt="' . $txt . '" where id=' . $field_id);
                    }
                }
            }
            // Обновление картинок
            if (is_array($images)) {
                foreach ($images as $k => $v) {
                    $res = $mysqli->query('select id,tp from fields_image where (name="' . $k . '") AND (pgid=' . $pid . ') order by title');
                    if ($row = $res->fetch_row()) {
                        $image_id = $row[0];
                        $image_tp = $row[1];

                        $image_w = ($v['w'] + 0) > 0 ? $v['w'] + 0 : 0;
                        $image_h = ($v['h'] + 0) > 0 ? $v['h'] + 0 : 0;
                        $image_alt = trim(addslashes($v['alt']));
                        $image_attr = trim(addslashes($v['attr']));
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
                    $res = $mysqli->query('select id from fields_table where (name="' . $k . '") AND (pgid=' . $pid . ') order by title');
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
                        ', tp=' . $table_tp . ', txt="' . addslashes($cells) . '" where id=' . $field_id);
                    }
                }
            }
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    //$res=$mysqli->query('select p.title,p.page_subtp,p.ord,m.name from site_pages as p left join pages_text as m on (m.id=p.page_subtp) where p.id='.$pid);
            //$row=$res->fetch_row();


            // Получаем текстовые поля
            $res = $mysqli->query('select name,txt,title from fields_text where pgid=' . $pid . ' order by title');
            while ($row = $res->fetch_row()) {
                if (empty($text[$row[0]])) {
                    $text[$row[0]]['text'] = $row[1];
                }
                $text[$row[0]]['title'] = $row[2];
                $text[$row[0]]['ok'] = true;
            }

            // Получаем поля картинки
            $res = $mysqli->query('select name,id,w,h,tp,alt,attr,title from fields_image where pgid=' . $pid . ' order by title');
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

            $res = $mysqli->query('select t.name,t.cols,t.rows,t.txt,t.title,f.id from fields_table as t left join types_table as f on (f.id=t.tp) where t.pgid=' . $pid . ' order by title');
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
                    $tables[$row[0]]['cells'] = $cells;
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
                    if ($v['ok']) {    #$v['text']=stripslashes(preg_replace("/\n|\r/",'',$v['text']));
                        $CKE = new CKEditor();
                        $CKE->returnOutput = true;
                        $CKEcofg = array();
                        $CKEcofg['toolbar'] = array(
                            array( 'Source', '-', 'Bold', 'Italic', 'Underline', 'Strike' ),
                            array( 'Image', 'Link', 'Unlink', 'Anchor' )
                        );
                        $CKEevents['instanceReady'] = 'function (ev) {alert("Loaded: " + ev.editor.name);}';
        //                        $CKEcofg = array(
        //                            array( 'Width', '100%' ),
        //                        );
                        $txt = $CKE->editor($k, $v['text'], $CKEcofg, $CKEevents);

                        #$txt=$sw->basePath; # Was $txt=$sw->textareaAttributes; # Was $sw->getHtml();

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
                array('/%pid%/','/%text%/','/%text_js%/','/%images%/','/%tables%/'),
                array($pid,$text_fields,$text_field_js,$image_fields,$table_fields),
                $tpl['pages']['edit']
            );

                break;
        }
        default:
        {    // Список страниц
            $list = '';
            $even = false;
            $res = $mysqli->query('select p.id,CONCAT(p.title," (",p.name," )"),m.name from site_pages as p left join pages_text as m on (m.id=p.page_subtp) where (did=' . $did . ') AND ((page_tp=1) OR (page_tp=2)) AND (clonefor=0) order by ord');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%ttl%/','/%maket%/','/%bgcolor%/'),
                    array($row[0],$row[1],$row[2],$tpl[$even ? 'even' : 'odd']),
                    $tpl['pages']['pageline']
                );
                    // Список клонов
                    $res2 = $mysqli->query('select p.id,p.title,m.name from site_pages as p left join pages_text as m on (m.id=p.page_subtp) where (clonefor=' . $row[0] . ') order by ord');
                while ($row2 = $res2->fetch_row()) {
                    if (($row2[0] + 0) > 0) {
                        $list .= preg_replace(
                            array('/%id%/','/%ttl%/','/%maket%/','/%bgcolor%/'),
                            array($row2[0],$tpl['pages']['clone_text'] . $row2[1],$row2[2],$tpl[$even ? 'even' : 'odd']),
                            $tpl['pages']['pageline']
                        );
                    }
                }

                $even = !$even;
            }
            $body = preg_replace(array('/%list%/','/%div%/'), array($list,$divs[$did]), $tpl['pages']['main']);
        }
    }
    return $body;
}

include_once 'ckeditor/ckeditor.php';//mark3

$body = build_page();
