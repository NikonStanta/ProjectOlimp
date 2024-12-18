<?php

function text_page($pgid, $subtype, $image_dir)
{
    global $SITE_VARS,$tpl,$_PAGE_VAR_VALUES,$mysqli;

// Получаем макет страницы

    $res = $mysqli->query('select maket from pages_text where id=' . $subtype);
    $row = $res->fetch_row();
    $maket = $row[0];

// Получаем текстовые поля

    $res = $mysqli->query('select name,txt from fields_text where pgid=' . $pgid . ' order by title');
    while ($row = $res->fetch_row()) {
        $fields[$row[0]] = stripslashes($row[1]);
        $names[] = "/%$row[0]%/";
    }

// Получаем поля картинки

    $res = $mysqli->query('select name,id,w,h,tp,title,attr from fields_image where pgid=' . $pgid . ' order by title');
    while ($row = $res->fetch_row()) {
        $fields[$row[0]] = '<img src="' . $image_dir . '/img' . $row[1] . '.' . $row[4] . '" width=' . $row[2] . ' height=' . $row[3] . ' alt="' . $row[5] . '" ' . $row[6] . '>';
        $names[] = "/%$row[0]%/";
    }

// Получаем поля таблицы

    $res = $mysqli->query('select t.name,t.cols,t.rows,t.txt,f.main,f.ttl_row,f.ttl_cells,f.even_row,f.odd_row,f.even_cells,f.odd_cells from fields_table as t left join types_table as f on (f.id=t.tp) where t.pgid=' . $pgid . ' order by title');
    while ($row = $res->fetch_row()) {
        $ccount = $row[1];
        $rcount = $row[2];
        $main_tpl = $row[4];
        $ttl_row = $row[5];
        $ttl_cells = explode('::', $row[6]);
        $even_row = $row[7];
        $odd_row = $row[8];
        $even_cells = explode('::', $row[9]);
        $odd_cells = explode('::', $row[10]);
        $rows = explode('::::', stripslashes($row[3]));
        $r = 0;
        $wt = '';
        foreach ($rows as $v) {
            $wa = explode('::', $v);
            $c = 0;
            $wr = '';

            if ($r == 0) {
                $cells_tpl = $ttl_cells;
                $row_tpl = $ttl_row;
            } elseif ($r == 1) {
                $cells_tpl = $odd_cells;
                $row_tpl = $odd_row;
            } else {
                $cells_tpl = $even_cells;
                $row_tpl = $even_row;
            }

            foreach ($wa as $w) {
                $wr .= preg_replace('/%txt%/', $w, $cells_tpl[$c]);
                $c++;
                if ($c >= count($cells_tpl)) {
                    $c = 0;
                }
            }

            $wt .= preg_replace('/%row%/', $wr, $row_tpl);
            $r++;
            if ($r > 2) {
                $r = 1;
            }
        }

        $fields[$row[0]] = preg_replace('/%rows%/', $wt, $main_tpl);
        $names[] = "/%$row[0]%/";
    }
    if (is_array($names)) {
        foreach ($fields as $k => $v) {
            $_PAGE_VAR_VALUES['PAGE_FIELD_' . $k] = $v;
        }
        return $body = preg_replace($names, $fields, $maket);
    } else {
        return '';
    }
}
