<?php

// Редактирование содержания сайта - Новости
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$mysqli;

    if (($role & 1) == 0) {
        return $tpl['noaccess'];
    }

    $cmd = $_REQUEST['cmd'];
    $nid = $_REQUEST['nid'];

    if (isset($_REQUEST['btnAdd'])) {
        if (empty($nid)) {
            $cmd = 'doAdd';
        } else {
            $cmd = 'doEdit';
        }
    }

    if (isset($_REQUEST['btnDel'])) {
        if (empty($nid)) {
            $cmd = '';
        } else {
            $cmd = 'doDel';
        }
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $nid = '';
    }

// Выполнение команд

    switch ($cmd) {
        case 'doEdit':
        {    if (!empty($_REQUEST['ttl'])) {
                $ttl = $_REQUEST['ttl'];
        } else {
            $ttl = '';
        }
        if (!empty($_REQUEST['txt1'])) {
            $txt1 = $_REQUEST['txt1'];
        } else {
            $txt1 = '';
        }
        if (!empty($_REQUEST['txt2'])) {
            $txt2 = $_REQUEST['txt2'];
        } else {
            $txt2 = '';
        }

        if (!empty($_REQUEST['img'])) {
            $img = $_REQUEST['img'];
        } else {
            $img = '';
        }
        if (!empty($_REQUEST['dt'])) {
            $dt = $_REQUEST['dt'];
        } else {
            $dt = '';
        }
        if (!empty($_REQUEST['top'])) {
            $top = 1;
        } else {
            $top = 0;
        }

            $ttl = addslashes(stripslashes($ttl));
            $txt1 = addslashes(stripslashes($txt1));
            $txt2 = addslashes(stripslashes($txt2));
            $img = addslashes(stripslashes($img));

        if ($mysqli->query('update news set ttl="' . $ttl . '", txt1="' . $txt1 . '", txt2="' . $txt2 . '", dt=' . (!empty($dt) ? '"' . $dt . '"' : 'Now()') . ', img="' . $img . '", top=' . $top . ' where id=' . $nid)) {
            $cmd = '';
        } else {
            $cmd = 'Edit';
        }
        break;
        }
        case 'doAdd':
        {    if (!empty($_REQUEST['ttl'])) {
                $ttl = $_REQUEST['ttl'];
        } else {
            $ttl = '';
        }
        if (!empty($_REQUEST['txt1'])) {
            $txt1 = $_REQUEST['txt1'];
        } else {
            $txt1 = '';
        }
        if (!empty($_REQUEST['txt2'])) {
            $txt2 = $_REQUEST['txt2'];
        } else {
            $txt2 = '';
        }
        if (!empty($_REQUEST['img'])) {
            $img = $_REQUEST['img'];
        } else {
            $img = '';
        }
        if (!empty($_REQUEST['dt'])) {
            $dt = $_REQUEST['dt'];
        } else {
            $dt = '';
        }
        if (!empty($_REQUEST['top'])) {
            $top = 1;
        } else {
            $top = 0;
        }

            $ttl = addslashes(stripslashes($ttl));
            $txt1 = addslashes(stripslashes($txt1));
            $txt2 = addslashes(stripslashes($txt2));

        if ($mysqli->query('insert into news set ttl="' . $ttl . '", txt1="' . $txt1 . '", txt2="' . $txt2 . '", dt=' . (!empty($dt) ? '"' . $dt . '"' : 'Now()') . ', img="' . $img . '", top=' . $top)) {
            $cmd = '';
            $nid = $mysqli->insert_id;
        } else {
            $cmd = 'Add';
        }
        break;
        }
        case 'doDel':
        {    $mysqli->query('delete from news where id=' . $nid);
            $cmd = '';
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $res = $mysqli->query('select ttl,txt1,txt2,dt,top,img from news where id=' . $nid);
            if ($row = $res->fetch_row()) {
                $old_ttl = $row[0];
                $old_txt1 = $row[1];
                $old_txt2 = $row[2];
                $old_dt = $row[3];
                $old_top = $row[4];
                $old_img = $row[5];
            }
        }
        case 'Add':
        {    if (!empty($_REQUEST['ttl'])) {
                $ttl = $_REQUEST['ttl'];
        } else {
            $ttl = $old_ttl;
        }
        if (!empty($_REQUEST['txt1'])) {
            $txt1 = $_REQUEST['txt1'];
        } else {
            $txt1 = $old_txt1;
        }
        if (!empty($_REQUEST['txt2'])) {
            $txt2 = $_REQUEST['txt2'];
        } else {
            $txt2 = $old_txt2;
        }
        if (!empty($_REQUEST['dt'])) {
            $dt = $_REQUEST['dt'];
        } else {
            $dt = $old_dt;
        }
        if (!empty($_REQUEST['top'])) {
            $top = $_REQUEST['top'];
        } else {
            $top = $old_top;
        }
        if (!empty($_REQUEST['img'])) {
            $img = $_REQUEST['img'];
        } else {
            $img = $old_img;
        }

            $ttl = htmlspecialchars(stripslashes($ttl));
            $txt1 = stripslashes(preg_replace("/\n|\r/", '', $txt1));
            $txt2 = stripslashes(preg_replace("/\n|\r/", '', $txt2));


            $CKE = new CKEditor();
            $CKE->returnOutput = true;

            $txt1 = $CKE->editor('txt1', $txt1);

            $txt1 = preg_replace('/[\\\][\\\]n/', '', $txt1);

            $txt2 = $CKE->editor('txt2', $txt2);
            $txt2 = preg_replace('/[\\\][\\\]n/', '', $txt2);

            $img = $CKE->editor('img', $img);
            $img = preg_replace('/[\\\][\\\]n/', '', $img);

            $dir = '/img/news/'; // Папка с изображениями на сервере
            $f = scandir($_SERVER['DOCUMENT_ROOT'] . $dir);
        foreach ($f as $file) {
            if ($file != '.' and $file != '..') {
                //if(preg_match('/\.(png)/', $file)){ // Выводим только .png
                $imgs .= '<div style="margin:20px;"><img src="' . $dir . $file . '" style="width:200px;"/><br/>' . $file . '</div>';
            //$name_file_with_url .= '<a href="'. $domen .$dir.$array[$i].'" target="_blank">'.$array[$i].'</a>'."<br>\n";
            }
        }

            $body = preg_replace(
                array('/%nid%/','/%ttl%/','/%txt1%/','/%txt2%/','/%dt%/','/%top%/','/%img%/','/%imgs%/'),
                array($nid,$ttl,$txt1,$txt2,$dt,($top != 0 ? 'checked' : ''),$img,$imgs),
                $tpl['news']['edit']
            );

        break;
        }
        case 'Del':
        {    $res = $mysqli->query('select dt,ttl,txt1 from news where id=' . $nid);
            if ($row = $res->fetch_row()) {
                $body = preg_replace(
                    array('/%nid%/','/%dt%/','/%ttl%/','/%txt1%/'),
                    array($nid,$row[0],$row[1],$row[2]),
                    $tpl['news']['del']
                );
                break;
            } else {
                $cmd = '';
            }
        }
        default:
        {    // Список
            $list = '';
            $even = false;
            $res = $mysqli->query('select id,dt,ttl from news order by dt DESC');
            while ($row = $res->fetch_row()) {
                $list .= preg_replace(
                    array('/%id%/','/%dt%/','/%ttl%/','/%bgcolor%/'),
                    array($row[0],$row[1],(empty($row[2]) ? '<Без заголовка>' : $row[2]),$tpl[$even ? 'even' : 'odd']),
                    $tpl['news']['line']
                );
                $even = !$even;
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['news']['main']);
        }
    }
    return $body;
}
include_once 'ckeditor/ckeditor.php'; //mark3
$body = build_page();
