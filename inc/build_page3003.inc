<?php

// Построение страниц каталога

include_once('../inc/_text_page.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$tpl,$_PAGE_VAR_VALUES;

    if (!empty($SITE_VARS['DEFAULT_IMG_PREFIX'])) {
        define(DEFAULT_IMG_PREFIX, $SITE_VARS['DEFAULT_IMG_PREFIX']);
    } else {
        define(DEFAULT_IMG_PREFIX, 'cat');
    }

    if (!empty($SITE_VARS['DEFAULT_IMG_DIR'])) {
        define(DEFAULT_IMG_DIR, $SITE_VARS['DEFAULT_IMG_DIR']);
    } else {
        define(DEFAULT_IMG_DIR, 'catalog');
    }


    if (!empty($SITE_VARS['CatalogGroupIdLength'])) {
        $_GROUP_LEN = $SITE_VARS['CatalogGroupIdLength'];
    } else {
        $_GROUP_LEN = 6;
    }

    if (!empty($SITE_VARS['CatalogItemIdLength'])) {
        $_ITEM_LEN = $SITE_VARS['CatalogItemIdLength'];
    } else {
        $_ITEM_LEN = 8;
    }

    // Список групп каталога текущего раздела

    if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 'i') || ($PAGE['REQTP'] == 'I')) && (($PAGE['REQID'] + 0) > 0)) {
        $iid = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['iid'])) {
            $iid = $_REQUEST['iid'];
        } else {
            $iid = 0;
        }
    }

    if ((($PAGE['REQTP'] == 'g') || ($PAGE['REQTP'] == 'G')) and (($PAGE['REQID'] + 0) > 0)) {
        $gid = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['gid'])) {
            $gid = $_REQUEST['gid'];
        } else {
            $gid = 0;
        }
    }

    $res = $mysqli->query('select id,name from catalog_groups where pgid=' . $pgid . ' order by ord');
    while ($row = $res->fetch_row()) {
        $groups[$row[0]] = $row[1];
    }

    // Если задан продукт, восстановим раздел

    if ($iid > 0) {
        if ($res = $mysqli->query('select i.gid,i.name,g.name,g.pgid,i.subtype,i.descr from catalog_items as i left join catalog_groups as g on (g.id=i.gid) where i.id=' . $iid)) {
            $row = $res->fetch_row();
            if ($row[3] == $pgid) {
                $gid = $row[0];
                $item_name = $row[1];
                $group_name = $row[2];
                $item_subtp = $row[4];
                $item_descr = $row[5];
            } else {
                $iid = 0;
            }
        } else {
            $iid = 0;
        }
    }

    // Выбор раздела

    if (empty($gid)) {    //if (!is_array($groups)) return '';
        if (!empty($_REQUEST['gid'])) {
            $gid = $_REQUEST['gid'] + 0;
            if (empty($groups[$gid])) {
                $gid = key($groups);
            }
        } else {
            $gid = key($groups);
        }
    }

    // Меню
    if (!empty($tpl['catalog']['menu_var_name'])) {
        $menu = '';
        foreach ($groups as $gk => $gv) {
            if ($gk == $gid) {
                $items = '';
                $res = $mysqli->query('select i.id,i.name from catalog_items as i where i.gid=' . $gid . ' order by i.ord, i.dt desc, i.id desc');
                while ($row = $res->fetch_row()) {
                    $items .= preg_replace(array('/%name%/','/%ref%/'), array($row[1],$PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar), $tpl['catalog'][$row[0] == $iid ? 'menu_icline' : 'menu_iline']);
                }
                if (count($groups) > 1) {
                    $menu .= preg_replace(array('/%name%/','/%ref%/','/%items%/'), array($gv,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $gk) . $SITE_VARS['Default ext'] . $refpar,$items), $tpl['catalog']['menu_gcline']);
                } else {
                    $menu .= preg_replace(array('/%items%/'), array($items), $tpl['catalog']['menu_items']);
                }
            } else {
                if (count($groups) > 1) {
                            $menu .= preg_replace(array('/%name%/','/%ref%/'), array($gv,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $gk) . $SITE_VARS['Default ext'] . $refpar), $tpl['catalog']['menu_gline']);
                }
            }
        }
        $_PAGE_VAR_VALUES[trim($tpl['catalog']['menu_var_name'])] = $menu;
    }

    // Если продукт не выбран - список продуктов

    $res = $mysqli->query('select img_prefix,img_dir from catalog_groups where id=' . $gid);
    $row = $res->fetch_row();
    $img_prefix = $row[0] ? $row[0] : DEFAULT_IMG_PREFIX;
    $img_dir = $row[1] ? $row[1] : DEFAULT_IMG_DIR;
    $image_dir = $SITE_VARS['Image dir'] . '/' . $img_dir;



    if (empty($iid)) {    // Формируем список групп, если их больше одной
        $columns = $tpl['catalog']['columns'] + 0;
        if ($columns < 1) {
            $columns = 1;
        }
        $catalog_groups = '';
        $catalog_items = '';

        if (is_array($groups)) {
            foreach ($groups as $groupid => $groupname) {
                if (count($groups) > 1) {
                    $catalog_groups .= preg_replace(
                        array('/%id%/','/%name%/','/%refid%/'),
                        array($groupid,$groupname,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $k) . $SITE_VARS['Default ext'] . $refpar),
                        ($groupid == $gid) ? $tpl['catalog']['groupcline'] : $tpl['catalog']['groupline']
                    );
                }



        // Формируем список продуктов

                $w = '';
                $line = '';
                $list = '';
                $num = 1;

                $res = $mysqli->query('select i.id,i.name,i.descr,i.subtype,i.rate,i.subttl1,i.subttl2,i.price,i.param from catalog_items as i left join catalog_groups as g on (g.id=i.gid) where i.gid=' . $groupid . ' order by i.ord, i.dt desc, i.id desc');
                while ($row = $res->fetch_row()) {
                    $filename = $image_dir . '/' . $img_prefix . $row[0] . '.jpg';
                    $rate = $row[4] + 0;
                    if ($rate > 10) {
                        $rate = 10;
                    }
                    if ($rate < 1) {
                        $rate = 1;
                    }
                    $irate = $image_dir . '/rate' . $rate . '.gif';
                    $w = preg_replace(
                        array('/%id%/','/%name%/','/%descr%/','/%refid%/','/%img_ref%/','/%img_rate%/',
                        '/%subttl1%/','/%subttl2%/','/%price%/','/%param%/','/%amount%/'),
                        array($row[0],$row[1],$row[2],$PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar,$filename,$irate,$row[5],$row[6],$row[7],$row[8],$cart->items[$row[0]]['amount']),
                        file_exists($filename) ? $tpl['catalog']['itemiline' . ($row[3] != -1 ? 'd' : '')]
                           : $tpl['catalog']['itemline' . ($row[3] != -1 ? 'd' : '')]
                    );

                    if ($columns == 1) {
                        $list .= $w;
                    } else {
                        $line .= $w;
                                $num += 1;
                        if ($num > $columns) {
                            $list .= preg_replace('/%line%/', $line, $tpl['catalog']['line']);
                            $line = '';
                            $num = 1;
                        }
                    }
                }
                if (($columns > 1) and ($num > 1)) {
                    while ($num <= $columns) {
                        $line .= $tpl['catalog']['skipitem'];
                        $num += 1;
                    }
                    $list .= preg_replace('/%line%/', $line, $tpl['catalog']['line']);
                }
                if (!empty($list)) {
                    $list = preg_replace('/%items%/', $list, $tpl['catalog']['items']);
                    $list = preg_replace('/%SKIP%/', $tpl['catalog']['skip'], $list);
                    $catalog_items .= preg_replace(
                        array('/%items%/','/%groupid%/','/%groupname%/'),
                        array($list,$groupid,$groupname),
                        $tpl['catalog']['group']
                    );
                }
            }
        }

            $catalog_groups = preg_replace('/%groups%/', $catalog_groups, $tpl['catalog']['groups']);


            $body = preg_replace(
                array('/%groups%/','/%items%/'),
                array($catalog_groups,$catalog_items),
                $tpl['catalog']['body1']
            );
    } else {    // Подробно о продукте
        $res = $mysqli->query('select i.id,i.name,i.descr,i.subttl1,i.subttl2,i.subtype,i.price,g.img_prefix,g.img_dir,g.name from catalog_items as i left join catalog_groups as g on (g.id=i.gid) where i.id=' . $iid);
        $row = $res->fetch_row();
        $item_subtp = $row[5];
        $image_dir = $row[8];
        $img_prefix = $row[7];
        $groupname = $row[9];

        // Картинки

        $item_images = '';
        for ($image_num = 1; $image_num < 9; $image_num++) {
            $siref = 'img/' . $image_dir . '/' . $img_prefix . $iid . 's' . $image_num . '.jpg';
            $biref = 'img/' . $image_dir . '/' . $img_prefix . $iid . 'b' . $image_num . '.jpg';
            if (file_exists($siref) && file_exists($biref)) {
                $item_images[$image_num]['siref'] = $siref;
                $item_images[$image_num]['biref'] = $biref;
            }
        }

        $images_init = '';
        $images_icons = '';
        $wi = '';
        $num = 1;
        $columns = $tpl['catalog']['image_icon_columns'] + 0;
        $curr_image = $_REQUEST['image'] + 0;
        if (empty($item_images[$curr_image])) {
            $curr_image = 1;
        }

        foreach ($item_images as $k => $w) {
            $images_init .= preg_replace(
                array('/%num%/','/%siref%/','/%biref%/'),
                array($k,$w['siref'],$w['biref']),
                $tpl['catalog']['image_init_line']
            );
            $wi .= preg_replace(
                array('/%num%/','/%siref%/','/%biref%/'),
                array($k,$w['siref'],$w['biref']),
                $tpl['catalog']['image_icon']
            );
            if ($num == $columns) {
                $images_icons .= preg_replace(
                    array('/%icons%/'),
                    array($wi),
                    $tpl['catalog']['image_icons_line']
                );
                $num = 1;
                $wi = '';
            } else {
                $num++;
            }
        }
        if ($num != 1) {
            for ($i = $num; $i <= $columns; $i++) {
                $wi .= $tpl['catalog']['image_icon_skip'];
            }
            $images_icons .= preg_replace(
                array('/%icons%/'),
                array($wi),
                $tpl['catalog']['image_icons_line']
            );
        }

        $images = preg_replace(
            array('/%image%/','/%init%/','/%icons%/'),
            array($item_images[$curr_image]['biref'],$images_init,$images_icons),
            $tpl['catalog']['images']
        );

        $w = text_page(-$iid, $item_subtp, $image_dir);

    //

        $body = preg_replace(
            array('/%iid%/','/%gid%/','/%body%/','/%refid%/','/%name%/','/%descr%/','/%subttl1%/',
            '/%subttl2%/','/%price%/','/%images%/','/%groupname%/'),
            array($iid,$gid,$w,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $gid) . $SITE_VARS['Default ext'],
            $row[1],$row[2],$row[3],$row[4],$row[6],$images,$groupname),
            $tpl['catalog']['body2']
        );

        $res = $mysqli->query('select id,name,subttl1,subttl2 from catalog_items where id=' . $iid);
        if ($row = $res->fetch_row()) {
            $PAGE['path_nodes'][] = array('name' => $row[1],
            'ref' => ($PAGE['PREFIX'] ? $PAGE['PREFIX'] . '/' : '') . $PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar);
        }
    }

    return $body;
}
