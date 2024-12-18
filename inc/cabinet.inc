<?php

include_once('dadata.inc');

function cabinet_user_box_menu()
{
	global $tpl, $PAGE;
	$menu = '';
	$menu .= preg_replace(
		array('/%ref%/', '/%icon%/', '/%text%/'),
		array('/cabinet/profile.html', 'ui-icon-person', 'Профиль участника'),
		$tpl['reg']['cabinet_box_menu_line' . ($PAGE['type'] == 4000 ? 'c' : '')]
	);
	$menu .= preg_replace(
		array('/%ref%/', '/%icon%/', '/%text%/'),
		array('/cabinet/documents.html', 'ui-icon-folder-collapsed', 'Документы участника'),
		$tpl['reg']['cabinet_box_menu_line' . ($PAGE['type'] == 4002 ? 'c' : '')]
	);
	$menu .= preg_replace(
		array('/%ref%/', '/%icon%/', '/%text%/'),
		array('/cabinet/stages.html', 'ui-icon-gear', 'Участие в олимпиаде'),
		$tpl['reg']['cabinet_box_menu_line' . ($PAGE['type'] == 4001 ? 'c' : '')]
	);
	$menu .= preg_replace(
		array('/%ref%/', '/%icon%/', '/%text%/'),
		array('/cabinet/distance.html', 'ui-icon-mail-closed', 'Дистанционный или заочный этап'),
		$tpl['reg']['cabinet_box_menu_line' . ($PAGE['type'] == 4003 ? 'c' : '')]
	);
	$menu .= preg_replace(
		array('/%ref%/', '/%icon%/', '/%text%/'),
		array('/cabinet/results.html', 'ui-icon-gear', 'Результаты'),
		$tpl['reg']['cabinet_box_menu_line' . ($PAGE['type'] == 4004 ? 'c' : '')]
	);
	$menu .= preg_replace(
		array('/%ref%/', '/%icon%/', '/%text%/'),
		array('/cabinet/works.html', 'ui-icon-gear', 'Загрузка и просмотр работ'),
		$tpl['reg']['cabinet_box_menu_line' . ($PAGE['type'] == 4005 ? 'c' : '')]
	);
	return $menu;
}

function cabinet_document_text($id)
{
	global $mysqli;
	$res = $mysqli->query('select d.doc_ser,d.doc_num, DATE_FORMAT(d.doc_date,"%d.%m.%Y") as doc_date,d.doc_where,d.doc_code,DATE_FORMAT(d.doc_period,"%d.%m.%Y") as doc_period,t.name from olimp_docs_ic as d left join olimp_doc_types as t on (t.id=d.doc_type) where d.id=' . $id);
	$row = $res->fetch_assoc();
	$document_ic = $row['name'] .
		($row['doc_ser'] ? ' серия: ' . $row['doc_ser'] : '') .
		($row['doc_num'] ? ' номер: ' . $row['doc_num'] : '') .
		($row['doc_where'] ? ' выдан: ' . $row['doc_where'] : '') .
		($row['doc_date'] ? ' дата выдачи: ' . $row['doc_date'] : '') .
		($row['doc_code'] ? ' код подразделения: ' . $row['doc_code'] : '') .
		($row['doc_period'] != '00.00.0000' ? ' действительно до: ' . $row['doc_period'] : '');
	return $document_ic;
}

function cabinet_document_form($id, $tpl, $data)
{
	global $mysqli;
	if ($id > 0) {
		$res = $mysqli->query('select d.doc_type,d.doc_ser,d.doc_num, DATE_FORMAT(d.doc_date,"%d.%m.%Y") as doc_date,d.doc_where,d.doc_code,DATE_FORMAT(d.doc_period,"%d.%m.%Y") as doc_period from olimp_docs_ic as d where d.id=' . $id);
		if ($row = $res->fetch_assoc()) $ok = true;
	}
	if (!$ok) {
		$row['doc_type'] = 1;
		$row['doc_ser'] = '';
		$row['doc_num'] = '';
		$row['doc_where'] = '';
		$row['doc_date'] = '';
		$row['doc_code'] = '';
		$row['doc_period'] = '';
	}
	if (is_array($data)) {
		if ($data['doc_type'] > 0) $row['doc_type'] = $data['doc_type'];
		$row['doc_ser'] = $data['doc_ser'];
		$row['doc_num'] = $data['doc_num'];
		$row['doc_where'] = $data['doc_where'];
		$row['doc_date'] = $data['doc_date'];
		$row['doc_code'] = $data['doc_code'];
		$row['doc_period'] = $data['doc_period'];
	}
	$doc_types = '';
	$res2 = $mysqli->query('select id,name from olimp_doc_types');
	while ($row2 = $res2->fetch_assoc()) $doc_types .= '<option value=' . $row2['id'] . ($row2['id'] == $row['doc_type'] ? ' selected' : '') . '>' . $row2['name'] . '</option>';
	$doc_form = preg_replace(
		array('/%doc_types%/', '/%doc_ser%/', '/%doc_num%/', '/%doc_where%/', '/%doc_date%/', '/%doc_code%/', '/%doc_period%/'),
		array($doc_types, $row['doc_ser'], $row['doc_num'], htmlspecialchars($row['doc_where']), $row['doc_date'], $row['doc_code'], $row['doc_period']),
		$tpl['doc_ic_form']
	);

	return $doc_form;
}

function cabinet_address_text($id)
{
	global $mysqli;
	$res = $mysqli->query('select  a.country as country,c.name as country_name,a.zip_code,concat_ws(" ",r.name,r.socr) as region,a.area,a.city,a.street,a.house,a.building,a.apartament,a.addr_text,a.kladr_street from olimp_address as a left join olimp_countrys as c on (c.id=a.country) left outer join kladr_regions as r on (r.id=a.region) where a.id=' . $id);

	$row = $res->fetch_assoc();
	$w = $row['country_name'];
	if ($row['zip_code']) $w .= ($w ? ', ' : '') . $row['zip_code'];
	if ($row['country'] == 1) {
		if ($row['region']) $w .= ($w ? ', ' : '') . $row['region'];
		if ($row['area']) $w .= ($w ? ', ' : '') . $row['area'];
		if ($row['city']) $w .= ($w ? ', ' : '') . $row['city'];
		if ($row['street']) $w .= ($w ? ', ' : '') . $row['street'];
		if ($row['house']) $w .= ($w ? ', ' : '') . $row['house'];
		if ($row['building']) $w .= ($w ? ', корп.' : '') . $row['building'];
		if ($row['apartament']) $w .= ($w ? ', кв.' : '') . $row['apartament'];
	}
	if ($row['addr_text']) $w .= ($w ? ', ' : '') . $row['addr_text'];

	return $w;
}

function cabinet_address_text_kladr($id)
{
	global $dadata;
	global $mysqli;
	$w = '';
	$res = $mysqli->query('select kladr_street, isKladrApproved from olimp_address where id=' . $id);
	$row = $res->fetch_assoc();

	// не предлагаем менять адрес, если кладр не найден, либо адрес из ФИАС уже одобрен пользователен
	if (empty($row['kladr_street'])) {
		return $w;
	} else if ($row['isKladrApproved'] == 'Y') {
		return $w;
	}

	$fields = array("query" => $row['kladr_street'], "count" => 1);
	$result = $dadata->findById("address", $fields);
	$data = $result['suggestions'][0];
    $_SESSION['dadata_address' . $id] = $data;
	$country = $data['data']['country'];
	$address = $data['unrestricted_value'];

	if ($country == 'Россия') {
		$w = $country;
		if ($address) $w .= ($w ? ', ' : '') . $address;
	}
	return $w;
}

function debug_to_console($data)
{
	$output = $data;
	if (is_array($output))
		$output = implode(',', $output);

	echo "<script>console.log('Debug Objects: " . $output . "' );</script>";
}

function cabinet_address_form($id, $tpl, $varname, $data)
{
	global $mysqli;
	$ok = false;
	if ($id > 0) {
		$res = $mysqli->query('select country,zip_code,region,area,city,street,house,building,apartament,addr_text,kladr_street from olimp_address where id=' . $id);
		if ($row = $res->fetch_assoc()) $ok = true;
	}
	if (!$ok) {
		$row['country'] = 0;
		$row['zip_code'] = '';
		$row['region'] = '';
		$row['area'] = '';
		$row['city'] = '';
		$row['street'] = '';
		$row['house'] = '';
		$row['building'] = '';
		$row['apartament'] = '';
		$row['addr_text'] = '';
		$row['kladr_street'] = '';
	}
	if (is_array($data)) {
		$row['country'] = $data['country'];
		$row['zip_code'] = $data['zip_code'];
		$row['region'] = $data['region'];
		$row['area'] = $data['area'];
		$row['city'] = $data['city'];
		$row['street'] = $data['street'];
		$row['house'] = $data['house'];
		$row['building'] = $data['building'];
		$row['apartament'] = $data['apartament'];
		$row['addr_text'] = $data['addr_text'];
		$row['kladr_street'] = $data['kladr_street'];
	}
	$countrys = '';
	$res2 = $mysqli->query('select id,name from olimp_countrys');
	while ($row2 = $res2->fetch_assoc()) $countrys .= '<option value=' . $row2['id'] . ($row2['id'] == $row['country'] ? ' selected' : '') . '>' . $row2['name'] . '</option>';
	
	$regionName = '';
	$res2 = $mysqli->query('select id,name,socr from kladr_regions where id = "'. $row['region'] .'"');
	if ($row2 = $res2->fetch_assoc()) {
		$regionName = $row2['name'] . ' ' . $row2['socr'];
	}

	$address_form = preg_replace(
		array('/%countrys%/', '/%zip_code%/', '/%region%/', '/%regionId%/', '/%area%/', '/%city%/', '/%street%/', '/%house%/', '/%building%/', '/%apartament%/', '/%addr_text%/', '/%kladr_street%/', '/%varname%/'),
		array($countrys, $row['zip_code'], $regionName, $row['region'], $row['area'], $row['city'], $row['street'], $row['house'], $row['building'], $row['apartament'], $row['addr_text'], $row['kladr_street'], $varname),
		is_array($tpl) ? $tpl['address_form'] : $tpl
	);

	return $address_form;
}

function cabinet_address_check($data, $message, $req = true)
{
	if (empty($data['country'])) {
		if ($req) {
			return  $message . ' Не введена страна';
		} else {
			return  '';
		}
	}
	if (!preg_match('/\d{6,}/', $data['zip_code'])) {
		return $message . ' Неверный формат индекса или индекс не введен.';
	}
	if ($data['country'] == 1) {
		return '';
		//      if (($data['region']<1) || ($data['region']>99))
		//		{	return $message.' Неверный регион (нужно выбрать из списка).';		}
		//		if (empty($data['street']))
		//		{	return $message.' Не введено название улицы.';		}
		//		if (empty($data['house']))
		//		{	return $message.' Не введен номер дома.';		}
	} else {
		if (empty($data['addr_text'])) {
			return $message . ' Не введен текст адреса.';
		}
	}
	return '';
}


function cabinet_address_update($data, $id)
{
	global $mysqli;
	
	$res = $mysqli->query('select id from kladr_regions where locate(name,"'. $data['region'] .'") > 0');
	if ($row = $res->fetch_assoc()) {
		$region = $row['id'];
	}

	if ($id > 0) {
		if ($mysqli->query('update olimp_address set ' .
			'country=' . $data['country'] . ', ' .
			'zip_code="' . $data['zip_code'] . '", ' .
			'region=' . ($region + 0) . ', ' .
			'area="' . $data['area'] . '", ' .
			'city="' . $data['city'] . '", ' .
			'street="' . $data['street'] . '", ' .
			'house="' . $data['house'] . '", ' .
			'building="' . $data['building'] . '",' .
			'apartament="' . $data['apartament'] . '",' .
			'addr_text="' . $data['addr_text'] . '"' .
			'kladr_street="' . $data['kladr_street'] . '"' .
			'isKladrApproved="Y"' .
			' where id=' . $id)) return $id;
	}
	if ($mysqli->query('insert into olimp_address set ' .
		'country=' . $data['country'] . ', ' .
		'zip_code="' . $data['zip_code'] . '", ' .
		'region=' . ($region + 0) . ', ' .
		'area="' . $data['area'] . '", ' .
		'city="' . $data['city'] . '", ' .
		'street="' . $data['street'] . '", ' .
		'house="' . $data['house'] . '", ' .
		'building="' . $data['building'] . '",' .
		'apartament="' . $data['apartament'] . '",' .
		'kladr_street="' . $data['kladr_street'] . '",' .
		'isKladrApproved="Y",' .
		'addr_text="' . $data['addr_text'] . '"')) {
		return $mysqli->insert_id;
	} else {
		return 0;
	}
}

function cabinet_convert_dadata($id) {
	global $mysqli;

    $data = $_SESSION['dadata_address'. $id];
    if(empty($data)) {
        return '';
    }

	$data = $data['data'];

    $res = $mysqli->query('select id from kladr_regions where locate(name,"'. $data['region'] .'") > 0');
	if ($row = $res->fetch_assoc()) {
		$region = $row['id'];
	}

    if(empty($data['city_with_type'])) {
        $city = $data['settlement_with_type'];
    } else {
        $city = $data['city_with_type'];
    }

    $country=1;
    $postal_code = $data['postal_code'];
    $area = $data['area_with_type'];
    $street = $data['street_with_type'];

    $house = $data['house_type'] . ' ' . $data['house'];
    if($data['block']) {
        $house .= ' ' .$data['block_type'] . ' ' . $data['block'];
    }

    $fields = array(
        "country" => $country,
        "region" => $region,
        "zip_code" => $postal_code,
        "area" => $area,
        "city" => $city,
        "street" => $street,
        "house" => $house,
    );
    return $fields;
}

function cabinet_address_approve_kladr($id)
{
	global $mysqli;
    $data = cabinet_convert_dadata($id);
    if(empty($data)) return 0;
    
    $_SESSION['dadata_address' . $id] = '';

    if ($id > 0) {
		if ($mysqli->query('update olimp_address set ' .
			'country=1,' .
			'zip_code="' . $data['zip_code'] . '", ' .
			'region=' . ($data['region'] + 0) . ', ' .
			'area="' . $data['area'] . '", ' .
			'city="' . $data['city'] . '", ' .
			'street="' . $data['street'] . '", ' .
			'house="' . $data['house']  . '", ' .
			'isKladrApproved="Y"' .
			' where id=' . $id)) return $id;
	}        
}

function cabinet_school_text($id)
{
	global $mysqli;
	$res = $mysqli->query('select t.name as school_type, s.school_type_ex, s.name, concat(c.name,", ",r.name," ",r.socr,", ", a.city," ",a.street," ", a.house) as city from olimp_schools as s left join olimp_school_types as t on (t.id=s.school_type) left join olimp_address as a on (a.id=s.address) left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where s.id=' . $id);
	if ($row = $res->fetch_assoc()) {
		$school = ($row['school_type_ex'] ? $row['school_type_ex'] : $row['school_type']) . ' ' . $row['name'] . ' (' . $row['city'] . ')';
	}
	return $school;
}

function ident_old_addresses()
{
	global $dadata;
	global $mysqli;
	//$res = $mysqli->query('select id, zip_code, region, area, city, street, house, kladr_street from olimp_address where kladr_street is NULL and country = "1" limit 10');
		$rrr = "UPDATE olimp_address SET kladr_street='0c089b04-099e-4e0e-955a-6bf1ce525f1a' WHERE id='70726';
        UPDATE olimp_address SET kladr_street='c775fbcf-439f-4688-b8e5-8e91f1bb9376' WHERE id=70728;
        UPDATE olimp_address SET kladr_street='1212d409-230e-4c21-889e-74cddb88bf8d' WHERE id=71222;
        UPDATE olimp_address SET kladr_street='8386dbc1-90fc-4851-8257-f14ed9ee9ed7' WHERE id=71223;
        UPDATE olimp_address SET kladr_street='cb988551-d36d-421b-8c55-ad4605d27add' WHERE id=71224;
        UPDATE olimp_address SET kladr_street='ad760d91-6683-4091-9b16-43bd07723a28' WHERE id=71225;";
        
	$res2 = $mysqli->query($rrr);
		
	
		// while ($row = $res->fetch_assoc()) 
	// {
	// 	$query = $row['zip_code'] . ', ' . $row['city'] . ', ' . $row['street'] . ', ' . $row['house'];
	// 	$fields = array("query" => $query, "locations" => array("region" => $query),"count" => 1);
	// 	$result = $dadata->suggest("address", $fields);

	// 	 if($result['suggestions']){
	// 	 	if($result['suggestions'][0]) {
	// 			$fias_id = $result['suggestions'][0]['data']['fias_id'];
	// 			print_r($fias_id);
	// 			print_r(' + ' );
	// 	// 		$fias_id = $result[0]['fias_id'];
	// 	// 		print_r($row['id']);
	// 	// 		print_r(' + ' );
	// 	// 		if($fias_id){
	// 	// 			//$res2 = $mysqli->query('update olimp_address set kladr_street = "'. $fias_id . '' WHERE id = "'. $row['id'] . '"');
	// 	// 		}
	// 	 	}
	// 	 }
		
	// }
}

// Список сообщений

function build_message_list($pid, $msg_tp, $mark_readed = false)
{
	global $mysqli;
	$msgs = array();
	$msg_ids = '';
	$res = $mysqli->query('select id as msg_id,DATE_FORMAT(msg_dt,"%d.%m.%Y %H:%i") as msg_dt,"" as msg_from,"" as msg_title,body as msg_body from olimp_messages where pid_to=' . $pid . ' AND msg_type>0 AND Now() BETWEEN show_since AND show_till order by msg_dt DESC');
	while ($row = $res->fetch_assoc()) {
		$msgs[$row['msg_id']] = $row;
		$msg_ids .= ($msg_ids ? ',' : '') . $row['msg_id'];
	}
	if ($mark_readed) $mysqli->query('update olimp_messages set read_cnt=read_cnt+1 where id in (' . $msg_ids . ')');
	return $msgs;
}
