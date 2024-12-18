<?php

// Построение страниц каталога

include_once('../inc/_text_page.inc');

function search_catalog_item_match($iid, $SEARCH)
{
    global $mysqli;

    if (!is_array($SEARCH)) {
        return 1;
    }

    for ($i = 0; $i < 10; $i++) {
        $matches[$i] = false;
        $grp_use[$i] = false;
    }
    // Параметры с их именами
    $res = $mysqli->query('select g.name,i.pval from catalog_item_params as i left join catalog_group_params as g on (g.id=i.pid) where i.iid=' . $iid);
    while ($row = $res->fetch_row()) {
        $params[$row[0]] = $row[1];
    }

    $match = true;
    foreach ($SEARCH as $k => $v) {
        if (!empty($v)) {
            $param_cond = substr($k, 0, 1);
            $param_group = substr($k, 1, 1);
            $param_empty = substr($k, 2, 1);
            $param_name = substr($k, 3);
            if ($_REQUEST['debug']) {
                echo "$param_cond  $param_group  $param_empty  $param_name search=$v  value=$params[$param_name] ";
            }
            $grp_use[$param_group] = true;
            if (!empty($params[$param_name])) {
                switch ($param_cond) {
                    case 'E':    {    $matches[$param_group] = $matches[$param_group] || ($params[$param_name] == $v);
                        break;
                    }
                    case 'N':    {    $matches[$param_group] = $matches[$param_group] || ($params[$param_name] != $v);
                        break;
                    }
                    case 'g':    {    $matches[$param_group] = $matches[$param_group] || (($params[$param_name] + 0) > ($v + 0));
                        break;
                    }
                    case 'l':    {    $matches[$param_group] = $matches[$param_group] || (($params[$param_name] + 0) < ($v + 0));
                        break;
                    }
                    case 'G':    {    $matches[$param_group] = $matches[$param_group] || (($params[$param_name] + 0) >= ($v + 0));
                        break;
                    }
                    case 'L':    {    $matches[$param_group] = $matches[$param_group] || (($params[$param_name] + 0) <= ($v + 0));
                        break;
                    }
                    case 'M':    {    $matches[$param_group] = $matches[$param_group] || !(strpos($params[$param_name], $v) === false);
                        break;
                    }
                    case 'm':    {    $matches[$param_group] = $matches[$param_group] || (strpos($params[$param_name], $v) === false);
                        break;
                    }
                }
            } else {
                $matches[$param_group] = $matches[$param_group] and ($param_empty == 1);
            }
        }
    }
    $match = true;
    for ($i = 0; $i < 10; $i++) {
        if ($grp_use[$i]) {
            $match = $match && $matches[$i];
        }
        if ($_REQUEST['debug']) {
            echo '<br>';
            echo "GROUP $i " . ($grp_use[$i] ? 'used' : 'not used');
            echo '<br>' . ($matches[$i] ? 'YES' : 'NO');
            echo '-->' . ($match ? 'YES' : 'NO') . '<br>';
        }
    }
    if ($_REQUEST['debug']) {
        echo ($match ? 'matched' : 'not matched') . '<hr>';
    }
    return $match ? 1 : -1;
}

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$tpl,$cart,$_PAGE_VAR_VALUES;

    $catalog_id = $pgid; // !!!!!!!!!!!!!!!!!!

//    if (!empty($SITE_VARS['price_multiplyer']))
//    {    $w=$SITE_VARS[$SITE_VARS['price_multiplyer']]+0;
//        if ($w>0) { $price_multiplyer=$w; } else {$price_multiplyer=1;}
//    }

    //replace_order();

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

    $daynum = array('Mon' => 1,'Tue' => 2,'Wed' => 3,'Thu' => 4,'Fri' => 5,'Sat' => 6,'Sun' => 7);
    $day = $daynum[date('D')];

    $dayshift = $tpl['catalog']['day_shift'] + 0;
    if (isset($_REQUEST['dayshift'])) {
        $dayshift = $_REQUEST['dayshift'];
    }

    $w = time() + $dayshift * 86400;
    $dt = date('Y-m-d', $w);

    // Способ сортировки

    $gso = trim($tpl['catalog']['group_sort_order']);
    if (empty($gso)) {
        $gso = 'OAAA';
    }
    $group_sort_order = '';
    for ($i = 0; $i < strlen($gso); $i += 2) {
        $group_sort_order .= ($group_sort_order ? ', ' : '') . ($gso[$i] == 'A' ? 'name' : ($gso[$i] == 'O' ? 'ord' : '')) . ($gso[$i + 1] == 'R' ? 'DESC' : '');
    }
    if (empty($group_sort_order)) {
        $group_sort_order = 'ord';
    }

    $iso = trim($tpl['catalog']['item_sort_order']);
    if (empty($iso)) {
        $iso = 'AAOA';
    }
    $item_sort_order = '';
    for ($i = 0; $i < strlen($iso); $i += 2) {
        $item_sort_order .= ($item_sort_order ? ', ' : '') . ($iso[$i] == 'A' ? 'i.name' : ($iso[$i] == 'O' ? 'i.ord' : ($iso[$i] == 'D' ? 'i.dt' : ''))) . ($iso[$i + 1] == 'R' ? 'DESC' : '');
    }
    if (empty($item_sort_order)) {
        $item_sort_order = 'name';
    }

    $show_groups = trim($tpl['catalog']['show_groups']);            // CUR | MOVE | ALL
    $show_items_cur = trim($tpl['catalog']['show_items_cur']);        // NO | n | ALL
    $show_items_all = trim($tpl['catalog']['show_items_all']);        // NO | n | ALL

    // Список групп каталога текущего раздела
    if ($res = $mysqli->query('select id,name from catalog_groups where pgid=' . $catalog_id . ' order by ' . $group_sort_order)) {
        while ($row = $res->fetch_row()) {
            $groups[$row[0]] = $row[1];
        }
    } else {
        return     preg_replace(array('/%groups%/','/%items%/'), array('',''), $tpl['catalog']['body1']);
    }

    // Текущая группа и товар

    if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 'i') || ($PAGE['REQTP'] == 'I')) && (($PAGE['REQID'] + 0) > 0)) {
        $iid = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['iid'])) {
            $iid = $_REQUEST['iid'] + 0;
        } else {
            $iid = 0;
        }
    }

    if ((($PAGE['REQTP'] == 'g') || ($PAGE['REQTP'] == 'G')) and (($PAGE['REQID'] + 0) > 0)) {
        $group_id = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['gid'])) {
            $group_id = $_REQUEST['gid'] + 0;
        } else {
            if ($show_groups == 'ALL') {
                $group_id = 0;
            } else {
                $group_id = key($groups);
            }
        }
    }


    $first_group = '';

    if (empty($tpl['catalog']['firstgroup'])) {
        $tpl['catalog']['firstgroup'] = $tpl['catalog']['group'];
    }
    if (empty($tpl['catalog']['fg_itemline'])) {
        $tpl['catalog']['fg_itemline'] = $tpl['catalog']['itemline'];
    }
    if (empty($tpl['catalog']['fg_itemlined'])) {
        $tpl['catalog']['fg_itemlined'] = $tpl['catalog']['itemlined'];
    }
    if (empty($tpl['catalog']['fg_itemiline'])) {
        $tpl['catalog']['fg_itemiline'] = $tpl['catalog']['itemiline'];
    }
    if (empty($tpl['catalog']['fg_itemilined'])) {
        $tpl['catalog']['fg_itemilined'] = $tpl['catalog']['itemilined'];
    }
    if (empty($tpl['catalog']['fg_columns'])) {
        $tpl['catalog']['fg_columns'] = $tpl['catalog']['columns'];
    }
    if (empty($tpl['catalog']['fg_skip'])) {
        $tpl['catalog']['fg_skip'] = $tpl['catalog']['skip'];
    }
    if (empty($tpl['catalog']['fg_line'])) {
        $tpl['catalog']['fg_line'] = $tpl['catalog']['line'];
    }
    if (empty($tpl['catalog']['fg_skipitem'])) {
        $tpl['catalog']['fg_skipitem'] = $tpl['catalog']['skipitem'];
    }


    // Меню
    if (!empty($tpl['catalog']['menu_var_name'])) {
        $menu = '';
        foreach ($groups as $gk => $gv) {
            if ($gk == $gid) {
                $items = '';
                $res = $mysqli->query('select i.id,i.name from catalog_items as i where i.gid=' . $gid . ' order by i.ord, i.dt desc, i.id desc');
                while ($row = $res->fetch_row()) {
                    $items .= preg_replace(array('/%id%/','/%name%/','/%ref%/'), array($row[0],$row[1],$PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar), $tpl['catalog'][$row[0] == $iid ? 'menu_icline' : 'menu_iline']);
                }
                if (count($groups) > 1) {
                    $menu .= preg_replace(array('/%id%/','/%name%/','/%ref%/','/%items%/'), array($gk,$gv,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $gk) . $SITE_VARS['Default ext'] . $refpar,$items), $tpl['catalog']['menu_gcline']);
                } else {
                    $menu .= preg_replace(array('/%items%/'), array($items), $tpl['catalog']['menu_items']);
                }
            } else {
                if (count($groups) > 1) {
                            $menu .= preg_replace(array('/%id%/','/%name%/','/%ref%/'), array($gk,$gv,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $gk) . $SITE_VARS['Default ext'] . $refpar), $tpl['catalog']['menu_gline']);
                }
            }
        }
        $_PAGE_VAR_VALUES[trim($tpl['catalog']['menu_var_name'])] = $menu;
    }

        // !!!!!!!!!!!!!!

    $USE_SEARCH = false;

    $SEARCH_GROUP = $_REQUEST['SEARCH_GROUP'];
    $SEARCH_PARAMS = $_REQUEST['SEARCH_PARAMS'];

    if (!empty($SEARCH_GROUP) && (!empty($groups[$SEARCH_GROUP]))) {
        $gn = $groups[$SEARCH_GROUP];
        unset($groups);
        $groups[$SEARCH_GROUP] = $gn;
//        $USE_SEARCH=true;
    }


    if (is_array($SEARCH_PARAMS)) {
        $USE_SEARCH = true;
    }

    if (empty($iid)) {    // Формируем список групп, если их больше одной
        $catalog_items = '';
        if (is_array($groups)) {
            foreach ($groups as $k => $v) {
                $catalog_groups .= preg_replace(
                    array('/%id%/','/%gid%/','/%name%/','/%refid%/'),
                    array($k,$k,$v,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $k) . $SITE_VARS['Default ext'] . $refpar),
                    ($k == $gid) ? $tpl['catalog']['groupcline'] : $tpl['catalog']['groupline']
                );
                            $gid = $k;

                            $res = $mysqli->query('select img_prefix,img_dir,tpl_prefix,img_count from catalog_groups where id=' . $gid);
                            $row = $res->fetch_row();
                            $img_prefix = $row[0] ? $row[0] : DEFAULT_IMG_PREFIX;
                            $img_dir = $row[1] ? $row[1] : DEFAULT_IMG_DIR;
                            $tpl_prefix = trim($row[2]);                            // !!!!!!!!!!!!!!!!!!!!
                if (!empty($tpl_prefix)) {
                    if (empty($tpl['catalog'][$tpl_prefix . 'firstgroup'])) {
                                            $tpl['catalog'][$tpl_prefix . 'firstgroup'] = $tpl['catalog']['firstgroup'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_itemline'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_itemline'] = $tpl['catalog']['fg_itemline'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_itemlined'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_itemlined'] = $tpl['catalog']['fg_itemlined'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_itemiline'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_itemiline'] = $tpl['catalog']['fg_itemiline'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_itemilined'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_itemilined'] = $tpl['catalog']['fg_itemilined'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_columns'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_columns'] = $tpl['catalog']['fg_columns'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_skip'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_skip'] = $tpl['catalog']['fg_skip'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_skipitem'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_skipitem'] = $tpl['catalog']['fg_skipitem'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'fg_line'])) {
                        $tpl['catalog'][$tpl_prefix . 'fg_line'] = $tpl['catalog']['fg_line'];
                    }

                    if (empty($tpl['catalog'][$tpl_prefix . 'group'])) {
                        $tpl['catalog'][$tpl_prefix . 'group'] = $tpl['catalog']['group'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'itemline'])) {
                        $tpl['catalog'][$tpl_prefix . 'itemline'] = $tpl['catalog']['itemline'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'itemlined'])) {
                        $tpl['catalog'][$tpl_prefix . 'itemlined'] = $tpl['catalog']['itemlined'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'itemiline'])) {
                        $tpl['catalog'][$tpl_prefix . 'itemiline'] = $tpl['catalog']['itemiline'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'itemilined'])) {
                        $tpl['catalog'][$tpl_prefix . 'itemilined'] = $tpl['catalog']['itemilined'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'columns'])) {
                        $tpl['catalog'][$tpl_prefix . 'columns'] = $tpl['catalog']['columns'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'skip'])) {
                        $tpl['catalog'][$tpl_prefix . 'skip'] = $tpl['catalog']['skip'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'skipitem'])) {
                        $tpl['catalog'][$tpl_prefix . 'skipitem'] = $tpl['catalog']['skipitem'];
                    }
                    if (empty($tpl['catalog'][$tpl_prefix . 'line'])) {
                        $tpl['catalog'][$tpl_prefix . 'line'] = $tpl['catalog']['line'];
                    }

                    if (empty($tpl['catalog'][$tpl_prefix . 'items'])) {
                        $tpl['catalog'][$tpl_prefix . 'items'] = $tpl['catalog']['items'];
                    }
                }

                            $img_count = $row[3] + 0;
                            $image_dir = '/' . $SITE_VARS['Image dir'] . '/' . $img_dir;
            // !!!!! Список дополнительных параметров для группы
                            unset($group_param_names);
                            unset($group_param_dvals);
                            unset($group_param_sel_on);
                            unset($group_param_sel_off);
                            unset($group_param_sel_clear);
                            $resd = $mysqli->query('select id,name,par_tp,param from catalog_group_params where gid=' . $gid);
                while ($rowd = $resd->fetch_row()) {
                    $group_param_names[$rowd[0]] = '/%' . $rowd[1] . '%/';
                    $group_param_dvals[$rowd[0]] = '';
                    $group_param_tp[$rowd[0]] = $rowd[2];
                    if ($rowd[2] == 2) {
                        $group_param_options[$rowd[0]] = explode(',', $rowd[3]);
                    }
                    if ($rowd[2] == 5) {
                        $resdl = $mysqli->query('select id,param from catalog_group_par_lookup where grp=' . $rowd[3]);
                        $lookup_options = '';
                        while ($rowdl = $resdl->fetch_row()) {
                            $lookup_options[$rowdl[0]] = $rowdl[1];
                        }
                        $group_param_options[$rowd[0]] = $lookup_options;
                    }

                    $group_param_sel_off[$rowd[0]] = '/<@\+' . $rowd[1] . '>.*?<\/@\+' . $rowd[1] . '>/';
                    $group_param_sel_on[$rowd[0]] = '/<@\-' . $rowd[1] . '>.*?<\/@\-' . $rowd[1] . '>/';
                    $group_param_sel_clear[$rowd[0]] = '/<@\+' . $rowd[1] . '>|<\/@\+' . $rowd[1] . '>|<@\-' . $rowd[1] . '>|<\/@\-' . $rowd[1] . '>/';
                }

                        // Формируем список продуктов
                            $limits = '';
                if ($gid == $group_id) {
                    $columns = $tpl['catalog'][$tpl_prefix . 'fg_columns'] + 0;
                    if (($show_items_cur + 0) > 0) {
                        $limits = ' limit ' . ($show_items_cur + 0);
                    }
                } else {
                    $columns = $tpl['catalog'][$tpl_prefix . 'columns'] + 0;
                    if (($show_items_all + 0) > 0) {
                        $limits = ' limit ' . ($show_items_all + 0);
                    }
                }
                if ($columns < 1) {
                    $columns = 1;
                }
                            $w = '';
                            $line = '';
                            $list = '';
                            $num = 1;

                            $res = $mysqli->query('select i.id,i.name,i.descr,i.subtype,i.rate,i.subttl1,i.subttl2,i.price,i.param,i.active from catalog_items as i where (i.gid=' . $gid . ') order by ' . $item_sort_order . $limits);
                while ($row = $res->fetch_row()) {
                    $active = $row[9] + 0;
                    if (($active >= 0) && $USE_SEARCH) {
                        $active = search_catalog_item_match($row[0], $SEARCH_PARAMS);
                    }
                    if ($active >= 0) {
                        $filename = $image_dir . '/' . $img_prefix . $row[0] . '.jpg';
                        $sfilename = $image_dir . '/' . $img_prefix . $row[0] . 's1.jpg';
                        $rate = $row[4] + 0;
                        if ($rate > 10) {
                            $rate = 10;
                        }
                        if ($rate < 1) {
                            $rate = 1;
                        }
                        $irate = $image_dir . '/rate' . $rate . '.gif';
                        $row[7] = sprintf('%.0f', floor($row[7] * $price_multiplyer / 10) * 10);

                        // !!!!! Значения дополнительных параметров

                        $group_param_vals = $group_param_dvals;
                        $group_param_sels = $group_param_sel_off;
                        $resd = $mysqli->query('select pid,pval from catalog_item_params where iid=' . $row[0]);
                        while ($rowd = $resd->fetch_row()) {
                            switch ($group_param_tp[$rowd[0]]) {
                                case 2:
                                {    $group_param_vals[$rowd[0]] = $group_param_options[$rowd[0]][$rowd[1] - 1];
                                    break;
                                }
                                case 5:
                                    {    $group_param_vals[$rowd[0]] = $group_param_options[$rowd[0]][$rowd[1]];
                                        break;
                                }
                                default:
                                    {    $group_param_vals[$rowd[0]] = $rowd[1];
                                }
                            }
                            if (!empty($rowd[1]) && !empty($group_param_sels[$rowd[0]])) {
                                $group_param_sels[$rowd[0]] = $group_param_sel_on[$rowd[0]];
                            }
                        }


                        $w = preg_replace(
                            array('/%id%/','/%name%/','/%descr%/','/%ref%/','/%refid%/','/%img_ref%/','/%simg_ref%/','/%img_rate%/',
                            '/%subttl1%/','/%subttl2%/','/%price%/','/%param%/','/%present%/'),
                            array($row[0],$row[1],$row[2],$PAGE['name'] . sprintf(
                                '%0' . $_ITEM_LEN . 'd',
                                $row[0]
                            ) . $SITE_VARS['Default ext'] . $refpar,sprintf('%0' . $_ITEM_LEN . 'd', $row[0]),$filename,$sfilename,
                            $irate,$row[5],    $row[6],$row[7],$row[8],$item_present),
                            file_exists($_SERVER['DOCUMENT_ROOT'] . $filename) ? $tpl['catalog'][$tpl_prefix . ($gid == $group_id ? 'fg_' : '') . 'itemiline' . ($row[3] != -1 ? 'd' : '')]
                               : $tpl['catalog'][$tpl_prefix . ($gid == $group_id ? 'fg_' : '') . 'itemline' . ($row[3] != -1 ? 'd' : '')]
                        );


                        if (is_array($group_param_names)) {
                            $w = preg_replace($group_param_names, $group_param_vals, $w);
                        }

                        if ($_REQUEST['debug']) {
                            echo htmlspecialchars($w);
                            echo '<hr>';
                        }

                        if (is_array($group_param_sels)) {
                            $w = preg_replace($group_param_sels, '', $w);
                        }

                        if (is_array($group_param_sel_clear)) {
                            $w = preg_replace($group_param_sel_clear, '', $w);
                        }

                        if ($_REQUEST['debug']) {
                            foreach ($group_param_vals as $__k => $__v) {
                                echo $group_param_names[$__k] . ' (' . $__k . ') => ' . $__v . ' ### ' . $group_param_sels[$__k] . '<br>';
                            }
                            echo '<br>' . $w . '<hr>';
                        }


                        if ($columns == 1) {
                            $list .= $w;
                        } else {
                            $line .= $w;
                                    $num += 1;
                            if ($num > $columns) {
                                $list .= preg_replace(array('/%line%/','/%gid%/'), array($line,$gid), $tpl['catalog'][$tpl_prefix . ($gid == $group_id ? 'fg_' : '') . 'line']);
                                $line = '';
                                $num = 1;
                            }
                        }
                    }
                }
                if (($columns > 1) and ($num > 1)) {
                    while ($num <= $columns) {
                            $line .= $tpl['catalog'][$tpl_prefix . ($gid == $group_id ? 'fg_' : '') . 'skipitem'];
                                            $num += 1;
                    }
                    $list .= preg_replace(array('/%line%/','/%gid%/'), array($line,$gid), $tpl['catalog'][$tpl_prefix . ($gid == $group_id ? 'fg_' : '') . 'line']);
                }

                if (!empty($list)) {
                    $list = preg_replace('/%items%/', $list, $tpl['catalog'][$tpl_prefix . 'items']);
                    $list = preg_replace('/%SKIP%/', $tpl['catalog'][$tpl_prefix . ($gid == $group_id ? 'fg_' : '') . 'skip'], $list);

                    if ($gid == $group_id) {
                        $w = preg_replace(
                            array('/%items%/','/%groupid%/','/%groupname%/'),
                            array($list,$gid,$v),
                            $tpl['catalog'][$tpl_prefix . 'firstgroup']
                        );
                        $first_group = $w;
                        if ($show_groups != 'MOVE') {
                            $catalog_items .= $w;
                        }
                    } else {
                        $catalog_items .= preg_replace(
                            array('/%items%/','/%groupid%/','/%groupname%/'),
                            array($list,$gid,$v),
                            $tpl['catalog'][$tpl_prefix . 'group']
                        );
                    }
                }
            }

            if ($show_groups == 'MOVE') {
                $catalog_items = $first_group . $catalog_items;
            }

            // закончили обработку группы
        }

        $catalog_groups = preg_replace('/%groups%/', $catalog_groups, $tpl['catalog']['groups']);
        $body = preg_replace(
            array('/%groups%/','/%items%/'),
            array($catalog_groups,$catalog_items),
            $tpl['catalog']['body1']
        );
    } else {    // Подробно о продукте
        $res = $mysqli->query('select i.id,i.name,i.descr,i.subttl1,i.subttl2,i.subtype,i.gid,g.img_count,g.img_prefix,g.img_dir,g.name,g.tpl_prefix from catalog_items as i left join catalog_groups as g on (g.id=i.gid) where i.id=' . $iid);
        $row = $res->fetch_row();
        $item_subtp = $row[5];
        $group_id = $row[6];
        $image_dir = '/' . $row[9];
        $img_prefix = $row[8];
        $img_count_max = $row[7];
        $groupname = $row[10];
        if (!empty($row[11])) {
            $tpl_prefix = $row[11] . '_';
        } else {
            $tpl_prefix = '';
        }

        if (!empty($tpl_prefix)) {
            if (empty($tpl['catalog'][$tpl_prefix . 'image_icon_columns'])) {
                $tpl['catalog'][$tpl_prefix . 'image_icon_columns'] = $tpl['catalog']['image_icon_columns'];
            }
            if (empty($tpl['catalog'][$tpl_prefix . 'image_init_line'])) {
                $tpl['catalog'][$tpl_prefix . 'image_init_line'] = $tpl['catalog']['image_init_line'];
            }
            if (empty($tpl['catalog'][$tpl_prefix . 'image_icon'])) {
                $tpl['catalog'][$tpl_prefix . 'image_icon'] = $tpl['catalog']['image_icon'];
            }
            if (empty($tpl['catalog'][$tpl_prefix . 'image_icons_line'])) {
                $tpl['catalog'][$tpl_prefix . 'image_icons_line'] = $tpl['catalog']['image_icons_line'];
            }
            if (empty($tpl['catalog'][$tpl_prefix . 'image_icon_skip'])) {
                $tpl['catalog'][$tpl_prefix . 'image_icon_skip'] = $tpl['catalog']['image_icon_skip'];
            }
            if (empty($tpl['catalog'][$tpl_prefix . 'images'])) {
                $tpl['catalog'][$tpl_prefix . 'images'] = $tpl['catalog']['images'];
            }
            if (empty($tpl['catalog'][$tpl_prefix . 'body2'])) {
                $tpl['catalog'][$tpl_prefix . 'body2'] = $tpl['catalog']['body2'];
            }
        }

        // Картинки

        $item_images = '';
        for ($image_num = 1; $image_num <= $img_count_max; $image_num++) {
            $siref = $image_dir . '/' . $img_prefix . $iid . 's' . $image_num . '.jpg';
            $biref = $image_dir . '/' . $img_prefix . $iid . 'b' . $image_num . '.jpg';
            if (file_exists($_SERVER['DOCUMENT_ROOT'] . $siref) && file_exists($_SERVER['DOCUMENT_ROOT'] . $biref)) {
                $item_images[$image_num]['siref'] = $siref;
                $item_images[$image_num]['biref'] = $biref;
            }
        }

        $images_init = '';
        $images_icons = '';
        $wi = '';
        $num = 1;
        $columns = $tpl['catalog'][$tpl_prefix . 'image_icon_columns'] + 0;
        $curr_image = $_REQUEST['image'] + 0;
        if (empty($item_images[$curr_image])) {
            $curr_image = 1;
        }

        if (is_array($item_images)) {
            foreach ($item_images as $k => $w) {
                $images_init .= preg_replace(
                    array('/%num%/','/%siref%/','/%biref%/'),
                    array($k,$w['siref'],$w['biref']),
                    $tpl['catalog'][$tpl_prefix . 'image_init_line']
                );
                $wi .= preg_replace(
                    array('/%num%/','/%siref%/','/%biref%/'),
                    array($k,$w['siref'],$w['biref']),
                    $tpl['catalog'][$tpl_prefix . 'image_icon']
                );
                if ($num == $columns) {
                    $images_icons .= preg_replace(
                        array('/%icons%/'),
                        array($wi),
                        $tpl['catalog'][$tpl_prefix . 'image_icons_line']
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
                    $tpl['catalog'][$tpl_prefix . 'image_icons_line']
                );
            }

            $images = preg_replace(
                array('/%image%/','/%init%/','/%icons%/'),
                array($item_images[$curr_image]['biref'],$images_init,$images_icons),
                $tpl['catalog'][$tpl_prefix . 'images']
            );
        } else {
            $images = $tpl['catalog'][$tpl_prefix . 'noimages'];
        }
         // закончили с картинками

        $w = text_page(-$iid, $item_subtp, $image_dir);
        $body = preg_replace(
            array('/%iid%/','/%gid%/','/%body%/','/%refid%/','/%name%/','/%descr%/','/%subttl1%/',
            '/%subttl2%/','/%price%/','/%images%/','/%groupname%/'),
            array($iid,$group_id,$w,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'dg', $gid) . $SITE_VARS['Default ext'],
            $row[1],$row[2],$row[3],$row[4],$row[6],$images,$groupname),
            $tpl['catalog'][$tpl_prefix . 'body2']
        );

        // !!!!! Список дополнительных параметров для группы ###
            $group_param_names = '';
            $group_param_dvals = '';
            $resd = $mysqli->query('select id,name,par_tp,param from catalog_group_params where gid=' . $group_id);
        while ($rowd = $resd->fetch_row()) {
            if (!empty($rowd[1])) {
                $group_param_names[$rowd[0]] = '/%' . $rowd[1] . '%/';
                    $group_param_dvals[$rowd[0]] = '';
                    $group_param_tp[$rowd[0]] = $rowd[2];
                if ($rowd[2] == 2) {
                    $group_param_options[$rowd[0]] = explode(',', $rowd[3]);
                }
                if ($rowd[2] == 5) {
                    $resdl = $mysqli->query('select id,param from catalog_group_par_lookup where grp=' . $rowd[3]);
                    $lookup_options = '';
                    while ($rowdl = $resdl->fetch_row()) {
                        $lookup_options[$rowdl[0]] = $rowdl[1];
                    }
                    $group_param_options[$rowd[0]] = $lookup_options;
                }
                    $group_param_sel_off[$rowd[0]] = '/<@\+' . $rowd[1] . '>.*?<\/@\+' . $rowd[1] . '>/s';
                    $group_param_sel_on[$rowd[0]] = '/<@\-' . $rowd[1] . '>.*?<\/@\-' . $rowd[1] . '>/s';
                    $group_param_sel_clear[$rowd[0]] = '/<@\+' . $rowd[1] . '>|<\/@\+' . $rowd[1] . '>|<@\-' . $rowd[1] . '>|<\/@\-' . $rowd[1] . '>/s';
            }
        }


        // !!!!! Значения дополнительных параметров
            $group_param_vals = $group_param_dvals;
            $group_param_sels = $group_param_sel_off;
            $resd = $mysqli->query('select pid,pval from catalog_item_params where iid=' . $row[0]);
        while ($rowd = $resd->fetch_row()) {
            switch ($group_param_tp[$rowd[0]]) {
                case 2:
                    {    $group_param_vals[$rowd[0]] = $group_param_options[$rowd[0]][$rowd[1] - 1];
                        break;
                }
                case 5:
                    {    $group_param_vals[$rowd[0]] = $group_param_options[$rowd[0]][$rowd[1]];
                        break;
                }
                default:
                    {    $group_param_vals[$rowd[0]] = $rowd[1];
                }
            }
            if (!empty($rowd[1]) && !empty($group_param_sels[$rowd[0]])) {
                $group_param_sels[$rowd[0]] = $group_param_sel_on[$rowd[0]];
            }
        }

        if (is_array($group_param_names)) {
            $body = preg_replace($group_param_names, $group_param_vals, $body);
        }
        if (is_array($group_param_sels)) {
            $body = preg_replace($group_param_sels, '', $body);
        }
        if (is_array($group_param_sel_clear)) {
            $body = preg_replace($group_param_sel_clear, '', $body);
        }


        $res = $mysqli->query('select id,name,subttl1,subttl2 from catalog_items where id=' . $iid);
        if ($row = $res->fetch_row()) {
            $PAGE['path_nodes'][] = array('name' => $row[1],
                'ref' => ($PAGE['PREFIX'] ? $PAGE['PREFIX'] . '/' : '') . $PAGE['name'] . sprintf('%0' . $_ITEM_LEN . 'd', $row[0]) . $SITE_VARS['Default ext'] . $refpar);
        }
    }

    return $body;
}
