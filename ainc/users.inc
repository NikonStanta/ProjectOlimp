<?php

// Администрирование сайта - Пользователи
function build_page()
{
    global $tpl,$refpar,$formrefpar,$pagedefs,$uid,$role,$user,$mysqli;

    //$roles=array(2=>'Оператор',3=>'Партнер',4=>'Администратор');

    if (($role & 4) == 0) {
        return $tpl['noaccess'];
    }

    $res = $mysqli->query('select name,descr,code from user_roles order by ord,code');
    while ($row = $res->fetch_assoc()) {
        $roles[$row['code']] = $row;
    }

    $cmd = $_REQUEST['cmd'];

    if (!empty($_REQUEST['user_id'])) {
        $user_id = $_REQUEST['user_id'];
    }

    if (isset($_REQUEST['btnAdd'])) {
        if (empty($user_id)) {
            $cmd = 'doAdd';
        } else {
            $cmd = 'doEdit';
        }
    }

    if (isset($_REQUEST['btnPlaces'])) {
        if (empty($user_id)) {
            $cmd = '';
        } else {
            $cmd = 'doPlaces';
        }
    }


    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
        $user_id = '';
    }



// Выполнение команд

    switch ($cmd) {
        case 'doDel':
        {    $mysqli->query('delete from user_divs where uid=' . $user_id);
            $mysqli->query('delete from users where id=' . $user_id);
            break;
        }
        case 'doAdd':
        {    if (!empty($_REQUEST['user_nic'])) {
                $user_nic = $_REQUEST['user_nic'];
        } else {
            $user_nic = 'new user';
        }
        if (!empty($_REQUEST['user_fio'])) {
            $user_fio = $_REQUEST['user_fio'];
        } else {
            $user_fio = 'Новый пользователь';
        }
        if (!empty($_REQUEST['user_phone'])) {
            $user_phone = $_REQUEST['user_phone'];
        } else {
            $user_phone = '';
        }
        if (!empty($_REQUEST['user_email'])) {
            $user_email = $_REQUEST['user_email'];
        } else {
            $user_email = '';
        }

            $user_role = 0;
        if (array_key_exists('user_roles', $_REQUEST) && is_array($_REQUEST['user_roles'])) {
            foreach ($_REQUEST['user_roles'] as $code => $role) {
                if (array_key_exists($code, $roles)) {
                    $user_role = $user_role | $code;
                }
            }
        }

        if (is_array($_REQUEST['user_divs'])) {
            $user_divs = $_REQUEST['user_divs'];
        }
        if (!empty($_REQUEST['user_pwd1'])) {
            $user_pwd1 = $_REQUEST['user_pwd1'];
        } else {
            $user_pwd1 = '';
        }
        if (!empty($_REQUEST['user_pwd2'])) {
            $user_pwd2 = $_REQUEST['user_pwd2'];
        } else {
            $user_pwd2 = '';
        }
        if (!empty($user_pwd1) && ($user_pwd1 == $user_pwd2)) {
            $pwd_query = ', pwd=SHA1("' . $user_pwd1 . '")';
        } else {
            $pwd_query = '';
        }

        if (
            $mysqli->query('insert into users set nic="' . $user_nic . '", fio="' . $user_fio . '", phone="' . $user_phone .
                '", email="' . $user_email . '", role=' . $user_role . $pwd_query)
        ) {
            $user_id = $mysqli->insert_id;
            if (is_array($user_divs)) {
                foreach ($user_divs as $k => $v) {
                    $mysqli->query('insert into user_divs(uid,did) values(' . $user_id . ',' . $k . ')');
                }
            }
        } else {
            $cmd = 'Add';
        }
            $cmd = '';
        break;

        }
        case 'doEdit':
        {    if (!empty($_REQUEST['user_nic'])) {
                $user_nic = $_REQUEST['user_nic'];
        } else {
            $user_nic = 'new user';
        }
        if (!empty($_REQUEST['user_fio'])) {
            $user_fio = $_REQUEST['user_fio'];
        } else {
            $user_fio = 'Новый пользователь';
        }
        if (!empty($_REQUEST['user_phone'])) {
            $user_phone = $_REQUEST['user_phone'];
        } else {
            $user_phone = '';
        }
        if (!empty($_REQUEST['user_email'])) {
            $user_email = $_REQUEST['user_email'];
        } else {
            $user_email = '';
        }

            $user_role = 0;
        if (array_key_exists('user_roles', $_REQUEST) && is_array($_REQUEST['user_roles'])) {
            foreach ($_REQUEST['user_roles'] as $code => $role) {
                if (array_key_exists($code, $roles)) {
                    $user_role = $user_role | $code;
                }
            }
        }

        if (is_array($_REQUEST['user_divs'])) {
            $user_divs = $_REQUEST['user_divs'];
        }
        if (!empty($_REQUEST['user_pwd1'])) {
            $user_pwd1 = $_REQUEST['user_pwd1'];
        } else {
            $user_pwd1 = '';
        }
        if (!empty($_REQUEST['user_pwd2'])) {
            $user_pwd2 = $_REQUEST['user_pwd2'];
        } else {
            $user_pwd2 = '';
        }
        if (!empty($user_pwd1) && ($user_pwd1 == $user_pwd2)) {
            $pwd_query = ', pwd=SHA1("' . $user_pwd1 . '")';
        } else {
            $pwd_query = '';
        }

        if (
            $mysqli->query('update users set nic="' . $user_nic . '", fio="' . $user_fio . '", phone="' . $user_phone .
                '", email="' . $user_email . '", role=' . $user_role . $pwd_query . ' where id=' . $user_id)
        ) {
            if (is_array($user_divs)) {
                $mysqli->query('delete from user_divs where uid=' . $user_id);
                foreach ($user_divs as $k => $v) {
                    $mysqli->query('insert into user_divs(uid,did) values(' . $user_id . ',' . $k . ')');
                }
            }
        } else {
            $cmd = 'Edit';
        }
            $cmd = '';
        break;
        }

        case 'doPlaces':
        {    if (is_array($_REQUEST['user_rights'])) {
                $user_rights = $_REQUEST['user_rights'];
                $mysqli->query('delete from user_places where user_id=' . $user_id);
                foreach ($user_rights as $k => $v) {
                    $mysqli->query('insert into user_places(user_id,olimp_place,user_right) values(' . $user_id . ',' . $k . ',"' . $v . '")');
                }
                $cmd = '';
        } else {
            $cmd = 'Places';
        }

        break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        case 'Edit':
        {    $res = $mysqli->query('select nic,fio,phone,email,role from users where id=' . $user_id);
            if ($row = $res->fetch_row()) {
                $old_nic = $row[0];
                $old_fio = $row[1];
                $old_phone = $row[2];
                $old_email = $row[3];
                $old_role = $row[4];

                $res = $mysqli->query('select did from user_divs where uid=' . $user_id);
                while ($row = $res->fetch_row()) {
                    $old_user_divs[$row[0]] = true;
                }
            }
        }
        case 'Add':
        {    if (!empty($_REQUEST['user_nic'])) {
                $user_nic = $_REQUEST['user_nic'];
        } else {
            $user_nic = $old_nic;
        }
        if (!empty($_REQUEST['user_fio'])) {
            $user_fio = $_REQUEST['user_fio'];
        } else {
            $user_fio = $old_fio;
        }
        if (!empty($_REQUEST['user_phone'])) {
            $user_phone = $_REQUEST['user_phone'];
        } else {
            $user_phone = $old_phone;
        }
        if (!empty($_REQUEST['user_email'])) {
            $user_email = $_REQUEST['user_email'];
        } else {
            $user_email = $old_email;
        }
        if (!empty($_REQUEST['user_role'])) {
            $user_role = $_REQUEST['user_role'];
        } else {
            $user_role = $old_role;
        }
        if (is_array($_REQUEST['user_divs'])) {
            $user_divs = $_REQUEST['user_divs'];
        } else {
            $user_divs = $old_user_divs;
        }

            $divs = '';
            $res = $mysqli->query('select id,name from site_divs');
        while ($row = $res->fetch_row()) {
            if ($row[1] != 'Home') {
                $divs .= '<input type=checkbox name="user_divs[' . $row[0] . ']" value=1' . (!empty($user_divs[$row[0]]) ? ' checked' : '') . ' id="ud' . $row[0] . '"><label for="ud' . $row[0] . '">' . $row[1] . '</label><br>';
            }
        }

            $user_roles = '';
        foreach ($roles as $k => $v) {
            $user_roles .= '<input type="checkbox" name="user_roles[' . $k . ']" value=' . $k . (($user_role & $k) ? ' checked' : '') . ' id="user_role' . $k . '"> <label for="user_role' . $k . '">' . $v['name'] . '</label><br>';
        }

            $body = preg_replace(
                array('/%id%/','/%user_nic%/','/%user_fio%/','/%user_phone%/','/%user_email%/','/%user_roles%/',
                    '/%user_divs%/'),
                array($user_id,$user_nic,$user_fio,$user_phone,$user_email,$user_roles,$divs),
                $tpl['users']['edit']
            );

        break;
        }
        case 'Del':
        {    $res = $mysqli->query('select nic,fio,role from users where id=' . $user_id);
            if ($row = $res->fetch_row()) {
                if ($row[0] != 'admin') {
                    $body = preg_replace(
                        array('/%id%/','/%user_nic%/','/%user_fio%/','/%user_role%/',
                        ),
                        array($user_id,$row[0],$row[1],$roles[$row[2]]),
                        $tpl['users']['del']
                    );
                } else {
                    $body = $tpl['users']['nodel'];
                }
                break;
            }
        }
        case 'Places':
        {    $res = $mysqli->query('select nic,fio,role from users where id=' . $user_id);
            if ($row = $res->fetch_row()) {
                $user_nic = $row[0];
                $user_fio = $row[1];
                $user_role = $row[2];
                $places = '';
                $even = false;

                $res = $mysqli->query('select p.id,p.name,IFNULL(up.user_right,"None") from olimp_places as p left join user_places as up ON (up.olimp_place=p.id AND up.user_id="' . $user_id . '") order by p.org_id, p.name');
                while ($row = $res->fetch_row()) {
                    $places .= preg_replace(
                        array('/%place_id%/','/%place%/','/%SelectedAll%/','/%SelectedRead%/','/%SelectedNone%/','/%rowclass%/'),
                        array($row[0],$row[1],$row[2] == 'All' ? 'Selected' : '',$row[2] == 'Read' ? 'Selected' : '',$row[2] == 'None' ? 'Selected' : '',$even ? 'even' : 'odd'),
                        $tpl['users']['places_line']
                    );
                    $even = !$even;
                }

                $body = preg_replace(
                    array('/%user_id%/','/%user_nic%/','/%user_fio%/','/%user_places%/'),
                    array($user_id,$user_nic,$user_fio,$places),
                    $tpl['users']['places']
                );

                break;
            }
        }




        default:
        {    // Список пользователей
            $list = '';
            $even = false;
            $res = $mysqli->query('select id,nic,fio,role from users');
            while ($row = $res->fetch_row()) {
                $roles_list = '';
                foreach ($roles as $code => $role) {
                    if ($row[3] & $code) {
                        $roles_list .= ($roles_list ? ', ' : '') . $role['name'];
                    }
                }
                $list .= preg_replace(
                    array('/%id%/','/%nic%/','/%fio%/','/%role%/','/%bgcolor%/'),
                    array($row[0],$row[1],$row[2],$roles_list,$tpl[$even ? 'even' : 'odd']),
                    $tpl['users']['line']
                );
                $even = !$even;
            }
            $body = preg_replace(array('/%list%/'), array($list), $tpl['users']['main']);
        }
    }
    return $body;
}

$body = build_page();
