<?php

function delete_fields($pid)
{
    global $mysqli, $image_dir;
    $mysqli->query('delete from fields_text where pgid=' . $pid);
    $mysqli->query('delete from fields_table where pgid=' . $pid);
    $res = $mysqli->query('select id,tp from fields_image where pgid=' . $pid);
    while ($row = $res->fetch_row()) {
        $iref = $image_dir . '/img' . $row[0] . '.' . $row[1];
        if (file_exists($iref)) {
            unlink($iref);
        }
    }
    $mysqli->query('delete from fields_image where pgid=' . $pid);
}

function update_fields($pid, $subtp)
{
    global $mysqli, $image_dir;
    $new_fields = array();
    $old_fields = array();

    $res = $mysqli->query('select params from pages_text where id=' . $subtp);
    $row = $res->fetch_row();
    $w = explode('::::', $row[0]);
    foreach ($w as $v) {
        $ww = explode('::', $v);
        $new_fields[trim($ww[0])]['tp'] = $ww[1] + 0;
        $new_fields[trim($ww[0])]['title'] = $ww[2];
        //$new_fields[trim($ww[0])]['w']=$ww[3]+0;
        //$new_fields[trim($ww[0])]['h']=$ww[4]+0;
        $new_fields[trim($ww[0])]['w'] = array_key_exists(3, $ww) ? (int)$ww[3] : 0;
        $new_fields[trim($ww[0])]['h'] = array_key_exists(4, $ww) ? (int)$ww[3] : 0;

        $new_fields[trim($ww[0])]['new'] = true;
    }

    $res = $mysqli->query('select id,name,title from fields_text where pgid=' . $pid);
    while ($row = $res->fetch_row()) {
        $old_fields[$row[1]]['id'] = $row[0];
        $old_fields[$row[1]]['tp'] = 1;
        if (!empty($new_fields[$row[1]]['tp']) && ($new_fields[$row[1]]['tp'] == 1)) { //Поле с таким именем было
            $old_fields[$row[1]]['del'] = false;
            $new_fields[$row[1]]['new'] = false;
        } else {
            $old_fields[$row[1]]['del'] = true;
            $old_fields[$row[1]]['title'] = $row[2];
        }
    }

    $res = $mysqli->query('select id,name,tp,title,w,h from fields_image where pgid=' . $pid);
    while ($row = $res->fetch_row()) {
        $old_fields[$row[1]]['id'] = $row[0];
        $old_fields[$row[1]]['tp'] = 2;
        $old_fields[$row[1]]['itp'] = $row[2];
        if (!empty($new_fields[$row[1]]['tp']) && ($new_fields[$row[1]]['tp'] == 2)) { //Поле с таким именем было
            $old_fields[$row[1]]['del'] = false;
            $new_fields[$row[1]]['new'] = false;
        } else {
            $old_fields[$row[1]]['del'] = true;
            $old_fields[$row[1]]['title'] = $row[3];
            $old_fields[$row[1]]['w'] = $row[4];
            $old_fields[$row[1]]['h'] = $row[5];
        }
    }

    $res = $mysqli->query('select id,name,title from fields_table where pgid=' . $pid);
    while ($row = $res->fetch_row()) {
        $old_fields[$row[1]]['id'] = $row[0];
        $old_fields[$row[1]]['tp'] = 3;
        if (!empty($new_fields[$row[1]]['tp']) && ($new_fields[$row[1]]['tp'] == 3)) { //Поле с таким именем было
            $old_fields[$row[1]]['del'] = false;
            $new_fields[$row[1]]['new'] = false;
        } else {
            $old_fields[$row[1]]['del'] = true;
            $old_fields[$row[1]]['title'] = $row[2];
        }
    }

    // Удаляем старые поля, которых нет в новом макете

    if (is_array($old_fields)) {
        foreach ($old_fields as $k => $v) {
            if ($v['del']) {
                switch ($v['tp']) {
                    case 1:
                    {    $mysqli->query('delete from fields_text where id=' . $v['id']);
                        break;
                    }
                    case 2:
                    {    $iref = $image_dir . '/img' . $v['id'] . '.' . $v['itp'];
                        if (file_exists($iref)) {
                            unlink($iref);
                        }
                        $mysqli->query('delete from fields_image where id=' . $v['id']);
                        break;
                    }
                    case 3:
                    {    $mysqli->query('delete from fields_table where id=' . $v['id']);
                        break;
                    }
                }
            }
        }
    }

    // Добавляем поля, которых не было в старом макете
    if (is_array($new_fields)) {
        foreach ($new_fields as $k => $v) {
            if ($v['new']) {
                switch ($v['tp']) {
                    case 1:
                    {    $mysqli->query('insert into fields_text set name="' . $k . '", pgid=' . $pid . ', title="' . $v['title'] . '"');
                        break;
                    }
                    case 2:
                    {    $mysqli->query('insert into fields_image set name="' . $k . '", pgid=' . $pid . ', title="' . $v['title'] . '", w=' . $v['w'] . ', h=' . $v['h']);
                        break;
                    }
                    case 3:
                    {    $mysqli->query('insert into fields_table set name="' . $k . '", pgid=' . $pid . ', title="' . $v['title'] . '"');
                        break;
                    }
                }
            } else {
                switch ($v['tp']) {
                    case 1:
                    {    $mysqli->query('update fields_text set title="' . $v['title'] . '" where id=' . $old_fields[$k]['id']);
                        break;
                    }
                    case 2:
                    {    $mysqli->query('update fields_image set title="' . $v['title'] . '", w=' . $v['w'] . ', h=' . $v['h'] . ' where id=' . $old_fields[$k]['id']);
                        break;
                    }
                    case 3:
                    {    $mysqli->query('update fields_table set title="' . $v['title'] . '" where id=' . $old_fields[$k]['id']);
                        break;
                    }
                }
            }
        }
    }
}
