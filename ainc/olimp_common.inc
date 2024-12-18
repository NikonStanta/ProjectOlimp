<?php

$kladr_regions = array();
$res = $mysqli->query('select * from kladr_regions');
while ($row = $res->fetch_assoc()) {
    $kladr_regions[$row['id']] = $row;
}

$olimp_doc_types = array();
$res = $mysqli->query('select * from olimp_doc_types');
while ($row = $res->fetch_assoc()) {
    $olimp_doc_types[$row['id']] = $row;
}

$olimp_school_types = array();
$res = $mysqli->query('select * from olimp_school_types');
while ($row = $res->fetch_assoc()) {
    $olimp_school_types[$row['id']] = $row;
}

$olimp_countrys = array();
$res = $mysqli->query('select * from olimp_countrys');
while ($row = $res->fetch_assoc()) {
    $olimp_countrys[$row['id']] = $row;
}

$olimp_citizenships = array();
$res = $mysqli->query('select * from olimp_citizenships');
while ($row = $res->fetch_assoc()) {
    $olimp_citizenships[$row['id']] = $row;
}


function GetPersonData($person_id)
{
    global $mysqli;

    $res = $mysqli->query('select p.*, c.name as citizenship_name from olimp_persons as p left join olimp_citizenships as c on (c.id=p.citizenship) where p.id=' . $person_id);
    if ($person = $res->fetch_assoc()) {
        if ($person['addr_reg'] > 0) {
            $res = $mysqli->query('select a.*, concat_ws(" ",r.name,r.socr) as region_name, c.name as country_name from olimp_address as a left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where a.id=' . $person['addr_reg']);
            $person['addr_reg_data'] = $res->fetch_assoc();
        } else {
            $person['addr_reg_data'] = '';
        }
        if ($person['addr_post'] > 0) {
            $res = $mysqli->query('select a.*, concat_ws(" ",r.name,r.socr) as region_name, c.name as country_name from olimp_address as a left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where a.id=' . $person['addr_post']);
            $person['addr_post_data'] = $res->fetch_assoc();
        } else {
            $person['addr_post_data'] = '';
        }
        if ($person['school'] > 0) {
            $res = $mysqli->query('select s.*, IFNULL(st.name,"") as school_type_name from olimp_schools as s left join olimp_school_types as st on (st.id=s.school_type) where s.id=' . $person['school']);
            $person['school_data'] = $res->fetch_assoc();
        } else {
            $person['school_data'] = '';
        }
        if ($school['address'] > 0) {
            $res = $mysqli->query('select * from olimp_address where id=' . $school['address']);
            $school['addr_school_data'] = $res->fetch_assoc();
        } else {
            $school['addr_school_data'] = '';
        }
        if ($person['doc_ident'] > 0) {
            $res = $mysqli->query('select d.*,t.name as doc_type_name from olimp_docs_ic as d left join olimp_doc_types as t on (t.id=d.doc_type) where d.id=' . $person['doc_ident']);
            $person['doc_ident_data'] = $res->fetch_assoc();
        } else {
            $person['doc_ident_data'] = '';
        }
    } else {
        $person = '';
    }
    return $person;
}


function check_user_right($right)
{
    global $mysqli;
    $ur = array('Access' => false, 'Read' => false, 'Insert' => false, 'Update' => false, 'Delete' => false, 'PrivRead' => false, 'PrivInsert' => false, 'PrivUpdate' => false, 'PrivDelete' => false);

    $res = $mysqli->query('select actions from user_right where (rid=' . $right . ') AND (uid=0)');
    if ($row = $res->fetch_row()) {
        $r = ',,' . $row[0] . ',';
        $ur['Access'] = true;
        $ur['Read'] = (strpos($r, ',R,') > 0);
        $ur['Insert'] = (strpos($r, ',I,') > 0);
        $ur['Update'] = (strpos($r, ',U,') > 0);
        $ur['Delete'] = (strpos($r, ',D,') > 0);
        $ur['PrivRead'] = (strpos($r, ',PR,') > 0);
        $ur['PrivInsert'] = (strpos($r, ',PI,') > 0);
        $ur['PrivUpdate'] = (strpos($r, ',PU,') > 0);
        $ur['PrivDelete'] = (strpos($r, ',PD,') > 0);
    }
    $res = $mysqli->query('select actions from user_right where (rid=' . $right . ') AND (uid=' . $_SESSION['uid'] . ')');
    if ($row = $res->fetch_row()) {
        $r = ',,' . $row[0] . ',';
        $ur['Access'] = true;
        $ur['Read'] = (strpos($r, ',R,') > 0);
        $ur['Insert'] = (strpos($r, ',I,') > 0);
        $ur['Update'] = (strpos($r, ',U,') > 0);
        $ur['Delete'] = (strpos($r, ',D,') > 0);
        $ur['PrivRead'] = (strpos($r, ',PR,') > 0);
        $ur['PrivInsert'] = (strpos($r, ',PI,') > 0);
        $ur['PrivUpdate'] = (strpos($r, ',PU,') > 0);
        $ur['PrivDelete'] = (strpos($r, ',PD,') > 0);
    }
    return $ur;
}

function getUserPlaces($uid)
{
    global $mysqli;
    $user_places = array('rights' => array(),'All' => '','Read' => '','None' => '','Access' => '');
    $res = $mysqli->query('select olimp_place,user_right from user_places where user_id=' . ($uid + 0));
    while ($row = $res->fetch_assoc()) {
        $user_places['rights'][$row['olimp_place']] = $row['user_right'];
        $user_places[$row['user_right']] .= ($user_places[$row['user_right']] ? ',' : '') . $row['olimp_place'];
        if ($row['user_right'] != 'None') {
            $user_places['Access'] .= ($user_places['Access'] ? ',' : '') . $row['olimp_place'];
        }
    }
    return $user_places;
}
