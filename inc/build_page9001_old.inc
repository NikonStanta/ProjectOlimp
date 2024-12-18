<?php

// Документы. Заявление участника.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    if ($USER_INFO['uid'] < 1) {
        return $tpl['cabinet']['logon_form'];
    }

/*
echo getcwd().'<br>';
if (readfile('/var/www/libs/tcpdf/config/lang/rus.php')) echo 'ok<br>';
die("2");
*/

    require_once('TCPDF-main/tcpdf.php');
    require_once('TCPDF-main/config/lang/rus.php');

    $res = $mysqli->query('select p.reg_num, DATE_FORMAT(p.reg_date,"%d.%m.%Y") as reg_date, DATE_FORMAT(p.upd_date,"%d.%m.%Y") as upd_date, p.surename, p.name, p.patronymic, p.gender, DATE_FORMAT(p.birthday,"%d.%m.%Y") as birthday,
c.name as citizenship, dt.name as doc_type, d.doc_ser as doc_ser, d.doc_num as doc_num, DATE_FORMAT(d.doc_date,"%d.%m.%Y") as doc_date, 
d.doc_where as doc_where, DATE_FORMAT(d.doc_period,"%d.%m.%Y") as doc_period, p.tel,p.email,p.school_class,p.teacher_m,p.teacher_f, p.teacher_i,
p.addr_reg as addr_reg, p.addr_post as addr_post, st.name as school_type, s.school_type_ex as school_type_ex, s.name as school_name, s.address as addr_school 
from olimp_persons as p left join olimp_docs_ic as d on (d.id=p.doc_ident) left join olimp_doc_types as dt on (dt.id=d.doc_type) left join olimp_schools as s on (s.id=p.school) 
left join olimp_school_types as st on (st.id=s.school_type) 
left join olimp_citizenships as c on (c.id=p.citizenship) where p.uid=' . $USER_INFO['uid']);

    $person = $res->fetch_assoc();

    $res = $mysqli->query('select a.zip_code,c.name as country, concat_ws(" ",r.name,r.socr) as region, a.area, a.city, a.street, a.house, a.building, a.apartament, a.addr_text from 
olimp_address as a left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where a.id=' . $person['addr_reg']);
    $addr = $res->fetch_assoc();

    $regnum = $person['reg_num'];
    $surename = $person['surename'];
    $name = $person['name'];
    $patronymic = $person['patronymic'];
    $psurename = $person['surename'];
    $pname = $person['name'];
    $ppatronymic = $person['patronymic'];
    $gender = ($person['gender'] == 'M' ? 'М' : 'Ж');
    $birthday = $person['birthday'];
    $citizenship = $person['citizenship'];
    $doc_type = $person['doc_type'];
    $doc_dt = $person['doc_date'];
    $doc_ser = $person['doc_ser'];
    $doc_num = $person['doc_num'];
    $doc_where = $person['doc_where'];

    $addr_zip = $addr['zip_code'];
    $addr_country = $addr['country'];
    $addr_region = $addr['region'];
    $addr_area = $addr['area'];
    $addr_city = $addr['city'];
    $addr_street = $addr['street'];
    $addr_house = $addr['house'];
    $addr_building = $addr['building'];
    $addr_apartament = $addr['apartament'];

    if ($person['addr_post'] > 0) {
        $res = $mysqli->query('
select a.zip_code,c.name as country, concat_ws(" ",r.name,r.socr) as region, a.area, a.city, a.street, a.house, a.building, a.apartament, a.addr_text from 
olimp_address as a left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where a.id=' . $person[addr_post]);
        $addr = $res->fetch_assoc();
        $paddr_zip = $addr['zip_code'];
        $paddr_country = $addr['country'];
        $paddr_region = $addr['region'];
        $paddr_area = $addr['area'];
        $paddr_city = $addr['city'];
        $paddr_street = $addr['street'];
        $paddr_house = $addr['house'];
        $paddr_building = $addr['building'];
        $paddr_apartament = $addr['apartament'];
    } else {
        $paddr_zip = '';
        $paddr_country = '';
        $paddr_region = '';
        $paddr_area = '';
        $paddr_city = '';
        $paddr_street = '';
        $paddr_house = '';
        $paddr_building = '';
        $paddr_apartament = '';
    }

    $school_class = $person['school_class'];
    $school_name = (empty($person['school_type_ex']) ? $person['school_type'] : $person['school_type_ex']) . ' ' . $person['school_name'];

    if ($person['addr_school'] > 0) {
        $res = $mysqli->query('
select a.zip_code,c.name as country, concat_ws(" ",r.name,r.socr) as region, a.area, a.city, a.street, a.house, a.building, a.apartament, a.addr_text from 
olimp_address as a left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where a.id=' . $person['addr_school']);
        $addr = $res->fetch_assoc();
        $school_zip = $addr['zip_code'];
        $school_country = $addr['country'];
        $school_region = $addr['region'];
        $school_area = $addr['area'];
        $school_city = $addr['city'];
        $school_street = $addr['street'];
        $school_house = $addr['house'];
        $school_building = $addr['building'];
    } else {
        $school_zip = '';
        $school_country = '';
        $school_region = '';
        $school_area = '';
        $school_city = '';
        $school_street = '';
        $school_house = '';
        $school_building = '';
    }

    $tel = $person['tel'];
    $email = $person['email'];

    $teacher_m = $person['teacher_m'];
    $teacher_f = $person['teacher_f'];
    $teacher_i = $person['teacher_i'];

    $reg_date = ($person['upd_date'] == '00.00.0000' ? $person['reg_date'] : $person['upd_date']);

    $surename = str_pad(ucfirst_r($surename), 32);
    $name = str_pad(ucfirst_r($name), 32);
    $patronymic = str_pad(ucfirst_r($patronymic), 32);


// create new PDF document
    $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

// set header and footer fonts
    $pdf->setPrintHeader(false);
    $pdf->setPrintFooter(false);
//set margins
//setPageUnit( string $unit)
//$pdf->setHeaderMargin(0);
    $pdf->SetMargins(25, 10, 10);
    $pdf->setCellMargins(0, 0, 0, 0);
    $pdf->setCellPadding(0.5);


//set auto page breaks
    $pdf->SetAutoPageBreak(true, 10);

//set some language-dependent strings
    $pdf->setLanguageArray($l);

// set array for viewer preferences
    $preferences = array(
    'HideToolbar' => true,
    'HideMenubar' => true,
    'HideWindowUI' => true,
    'FitWindow' => true,
    'CenterWindow' => true,
    'DisplayDocTitle' => true,
    'NonFullScreenPageMode' => 'UseNone', // UseNone, UseOutlines, UseThumbs, UseOC
    'ViewArea' => 'CropBox', // MediaBox,CropBox, BleedBox, TrimBox, ArtBox
    'ViewClip' => 'CropBox', // CropBox, BleedBox, TrimBox, ArtBox
    'PrintArea' => 'MediaBox', // CropBox, BleedBox, TrimBox, ArtBox
    'PrintClip' => 'MediaBox', // CropBox, BleedBox, TrimBox, ArtBox
    'PrintScaling' => 'None', // None, AppDefault
    'Duplex' => 'Simplex', // Simplex, DuplexFlipShortEdge, DuplexFlipLongEdge
    'PickTrayByPDFSize' => true,
    'PrintPageRange' => array(1,1),
    'NumCopies' => 1
    );

// Check the example n. 60 for advanced page settings

// set pdf viewer preferences
    $pdf->setViewerPreferences($preferences);

// ---------------------------------------------------------
// 'dejavusans','dejavuserif'


    $pdf->AddPage();
    if (!empty($_SESSION['debug'])) {    //$pdf->Output('zayavlenie.pdf', 'I');
        //die();
    }

    $pdf->SetFont('dejavusans', '', 10);
    $pdf->SetDrawColor(0, 0, 0);
    $pdf->SetTextColor(0, 0, 0);

    $pdf->Cell(30, 40, iconv("utf-8", "utf-8", 'Фото 3x4 см'), 1, 1, 'C');

    $pdf->SetFontSize(18);
    $x = 130;
    $w = 9;
    $pdf->SetXY(129, 10);
    for ($i = 0; $i < 5; $i++) {
        $pdf->Cell($w, 10, $regnum[$i], 'LTB', 0, 'C');
    }
    $pdf->Cell($w, 10, '-', 'L', 0, 'C');
    $pdf->Cell($w, 10, $regnum[6], 'LTB', 0, 'C');
    $pdf->Cell($w, 10, $regnum[7], 1, 0, 'C');

    $pdf->SetFontSize(10);
    $pdf->MultiCell(85, 0, iconv("utf-8", "utf-8", 'В оргкомитет Олимпиады школьников "Надежда энергетики"'), 0, 'R', false, 0, 115, 25);

    $pdf->Image('./img/document_logo.gif', 80, 10, 26, 26, 'GIF');

    $pdf->SetFontSize(20);
    $pdf->SetXY(90, 40);
    $pdf->Cell(35, 8, iconv("utf-8", "utf-8", 'ЗАЯВЛЕНИЕ'), 0, 0, 'C');

    $x = 30;
    $y = 53;
    $w = 5.31;
    $h = 5.5;
    $pdf->SetFontSize(10);
    $pdf->SetXY(25, 53);
    $pdf->Cell($w, $h, iconv("utf-8", "utf-8", 'Я,'), 0, 0, 'L');
    $pdf->SetXY(31, $y);

    for ($i = 0; $i < 32; $i++) {
        $pdf->Cell($w, $h, iconv("utf-8", "utf-8", $surename[$i]), ($i != 31 ? 'LTB' : 1), 0, 'C');
    }
    $pdf->SetXY(31, $y + $h);
    for ($i = 0; $i < 32; $i++) {
        $pdf->Cell($w, $h, iconv("utf-8", "utf-8", $name[$i]), ($i != 31 ? 'LTB' : 1), 0, 'C');
    }
    $pdf->SetXY(31, $y + 2 * $h);
    for ($i = 0; $i < 32; $i++) {
        $pdf->Cell($w, $h, iconv("utf-8", "utf-8", $patronymic[$i]), ($i != 31 ? 'LTB' : 1), 0, 'C');
    }
    $y += 3 * $h + 1;
    $pdf->SetFontSize(8);
    $pdf->SetXY(25, $y);
    $pdf->Write(6, iconv('utf-8', 'utf-8', 'прошу допустить меня к участию в олимпиаде школьников "Надежда энергетики"'), false, 0, 'L');  // по следующим предметам:
// $y+=6;
// $pdf->SetXY(42,$y); $pdf->Cell(3,3,'',1,0,'L'); $pdf->Cell(25,4,iconv('utf-8','utf-8', ' - Математика'),0,0,'L'); $pdf->Cell(3,3,'',1,0,'L'); $pdf->Cell(16,4,iconv('utf-8','utf-8', ' - Физика   '),0,0,'L');  $pdf->Cell(3,3,'',1,0,'L'); $pdf->Cell(25,4,iconv('utf-8','utf-8', ' - Информатика   '),0,0,'L');
//$pdf->SetFontSize(7);
//$pdf->Cell(16,4,iconv('utf-8','utf-8', '(отметьте интересующие Вас предметы любым значком)'),0,0,'L');
    $y += 6;
    $pdf->SetFont('dejavusans', 'B', 9);
    $pdf->SetXY(25, $y);
    $pdf->Cell(16, 4, iconv('utf-8', 'utf-8', 'О себе сообщаю следующие сведения:'), 0, 0, 'L');
    $pdf->SetFont('dejavusans', '', 8);
    $pdf->SetXY(115, $y);
    $pdf->Cell(8, 4, iconv('utf-8', 'utf-8', 'пол:'), 0, 0, 'L');
    $pdf->Cell(4, 4, iconv('utf-8', 'utf-8', $gender), 1, 0, 'C');
    $pdf->SetXY(135, $y);
    $pdf->Cell(26, 4, iconv('utf-8', 'utf-8', 'дата рождения:'), 0, 0, 'R');
    $pdf->Cell(4, 4, $birthday[0], 1, 0, 'C');
    $pdf->Cell(4, 4, $birthday[1], 1, 0, 'C');
    $pdf->Cell(4, 4, '/', 0, 0, 'C');
    $pdf->Cell(4, 4, $birthday[3], 1, 0, 'C');
    $pdf->Cell(4, 4, $birthday[4], 1, 0, 'C');
    $pdf->Cell(4, 4, '/', 0, 0, 'C');
    $pdf->Cell(4, 4, $birthday[6], 1, 0, 'C');
    $pdf->Cell(4, 4, $birthday[7], 1, 0, 'C');
    $pdf->Cell(4, 4, $birthday[8], 1, 0, 'C');
    $pdf->Cell(4, 4, $birthday[9], 1, 0, 'C');
    $y += 6;
    $pdf->SetXY(25, $y);
    $pdf->SetFont('dejavusans', 'B', 8);
    $pdf->Write(6, iconv('utf-8', 'utf-8', 'Документ, удостоверяющий личность и гражданство:'), false, 0, 'L');
    $pdf->SetFont('dejavusans', '', 8);
    $pdf->SetXY(135, $y);
    $pdf->Cell(26, 4, iconv('utf-8', 'utf-8', 'гражданство:'), 0, 0, 'R');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $citizenship), 'B', 0, 'C', false, false, 1);
    $y += 6;
    $pdf->SetXY(25, $y);
    $pdf->Cell(25, 4, iconv('utf-8', 'utf-8', 'Вид документа:'), 0, 0, 'L');
    $pdf->Cell(75, 4, iconv('utf-8', 'utf-8', $doc_type), 'B', 0, 'C');
    $pdf->SetXY(135, $y);
    $pdf->Cell(26, 4, iconv('utf-8', 'utf-8', 'когда выдан:'), 0, 0, 'R');
    $pdf->Cell(4, 4, $doc_dt[0], 1, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[1], 1, 0, 'C');
    $pdf->Cell(4, 4, '/', 0, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[3], 1, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[4], 1, 0, 'C');
    $pdf->Cell(4, 4, '/', 0, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[6], 1, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[7], 1, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[8], 1, 0, 'C');
    $pdf->Cell(4, 4, $doc_dt[9], 1, 0, 'C');
    $y += 6.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(11, 4, iconv('utf-8', 'utf-8', 'Серия:'), 0, 0, 'L');
    $pdf->Cell(18, 4, iconv('utf-8', 'utf-8', $doc_ser), 'B', 0, 'C');
    $pdf->Cell(11, 4, iconv('utf-8', 'utf-8', 'номер:'), 0, 0, 'L');
    $pdf->Cell(18, 4, iconv('utf-8', 'utf-8', $doc_num), 'B', 0, 'C');
    $pdf->Cell(18, 4, iconv('utf-8', 'utf-8', 'кем выдан:'), 0, 0, 'L');
    $pdf->SetFontSize(7);
    $pdf->MultiCell(99, 7, iconv("utf-8", "utf-8", $doc_where), 'B', 'L', false, 0, 102, $y - 2.5, true, 1, false, false, 7, 'B');

    $y += 5.5;
    $pdf->SetFont('dejavusans', 'B', 8);
    $pdf->SetXY(25, $y);
    $pdf->Write(6, iconv('utf-8', 'utf-8', 'Адрес постоянной регистрации:'), false, 0, 'L');
    $pdf->SetFont('dejavusans', '', 8);
    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(29, 4, iconv('utf-8', 'utf-8', 'Почтовый индекс'), 0, 0, 'L');
    $pdf->Cell(17, 4, iconv('utf-8', 'utf-8', $addr_zip), 'B', 0, 'C');
    $pdf->Cell(13, 4, iconv('utf-8', 'utf-8', 'страна'), 0, 0, 'L');
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', $addr_country), 'B', 0, 'C');
    $pdf->Cell(12, 4, iconv('utf-8', 'utf-8', 'регион'), 0, 0, 'L');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $addr_region), 'B', 0, 'C', false, false, 1);

    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(11, 4, iconv('utf-8', 'utf-8', 'район'), 0, 0, 'L');
    $pdf->Cell(68, 4, iconv('utf-8', 'utf-8', $addr_area), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', 'населенный пункт'), 0, 0, 'L');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $addr_city), 'B', 0, 'C', false, false, 1);

    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(11, 4, iconv('utf-8', 'utf-8', 'улица'), 0, 0, 'L');
    $pdf->Cell(90, 4, iconv('utf-8', 'utf-8', $addr_street), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(8, 4, iconv('utf-8', 'utf-8', 'дом'), 0, 0, 'С');
    $pdf->Cell(15, 4, iconv('utf-8', 'utf-8', $addr_house), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(8, 4, iconv('utf-8', 'utf-8', 'корп.'), 0, 0, 'С');
    $pdf->Cell(15, 4, iconv('utf-8', 'utf-8', $addr_building), 'B', 0, 'C');
    $pdf->Cell(8, 4, iconv('utf-8', 'utf-8', 'кв.'), 0, 0, 'С');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $addr_apartament), 'B', 0, 'C', false, false, 1);



/*
$y+=6;
$pdf->SetFont('dejavusans','B', 8);
$pdf->SetXY(25,$y);
$pdf->Write(6,iconv('utf-8','utf-8', 'Почтовый адрес, адрес временной регистрации (если отличается от адреса постоянной регистрации)'),false,0,'L');
$pdf->SetFont('dejavusans','', 8);
$y+=5.5;
$pdf->SetXY(25,$y);
$pdf->Cell(29,4,iconv('utf-8','utf-8', 'Почтовый индекс'),0,0,'L'); $pdf->Cell(17,4,iconv('utf-8','utf-8', $paddr_zip),'B',0,'C');
$pdf->Cell(13,4,iconv('utf-8','utf-8', 'страна'),0,0,'L'); $pdf->Cell(30,4,iconv('utf-8','utf-8', $paddr_country),'B',0,'C');
$pdf->Cell(12,4,iconv('utf-8','utf-8', 'регион'),0,0,'L'); $pdf->Cell(0,4,iconv('utf-8','utf-8', $paddr_region),'B',0,'C',false,false,1);

$y+=5.5;
$pdf->SetXY(25,$y);
$pdf->Cell(11,4,iconv('utf-8','utf-8', 'район'),0,0,'L'); $pdf->Cell(68,4,iconv('utf-8','utf-8', $paddr_area),'B',0,'C',false,false,1);
$pdf->Cell(30,4,iconv('utf-8','utf-8', 'населенный пункт:'),0,0,'L'); $pdf->Cell(0,4,iconv('utf-8','utf-8', $paddr_city),'B',0,'C',false,false,1);

$y+=5.5;
$pdf->SetXY(25,$y);
$pdf->Cell(11,4,iconv('utf-8','utf-8', 'улица'),0,0,'L'); $pdf->Cell(90,4,iconv('utf-8','utf-8', $paddr_street),'B',0,'C',false,false,1);
$pdf->Cell(8,4,iconv('utf-8','utf-8', 'дом'),0,0,'С'); $pdf->Cell(15,4,iconv('utf-8','utf-8', $paddr_house),'B',0,'C',false,false,1);
$pdf->Cell(8,4,iconv('utf-8','utf-8', 'корп.'),0,0,'С'); $pdf->Cell(15,4,iconv('utf-8','utf-8', $paddr_building),'B',0,'C');
$pdf->Cell(8,4,iconv('utf-8','utf-8', 'кв.'),0,0,'С'); $pdf->Cell(0,4,iconv('utf-8','utf-8', $paddr_apartament),'B',0,'C',false,false,1);

*/

    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->SetFont('dejavusans', 'B', 8);
    $pdf->Cell(35, 4, iconv('utf-8', 'utf-8', 'Контактный телефон'), 0, 0, 'L');
    $pdf->SetFont('dejavusans', '', 8);
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', $tel), 'B', 0, 'C', false, false, 1);
    $pdf->SetFont('dejavusans', 'B', 8);
    $pdf->Cell(12, 4, 'e-mail', 0, 0, 'L');
    $pdf->SetFont('dejavusans', '', 8);
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $email), 'B', 0, 'C', false, false, 1);


    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->SetFont('dejavusans', 'B', 8);
    $pdf->Cell(20, 4, iconv('utf-8', 'utf-8', 'Обучаюсь в'), 0, 0, 'L');
    $pdf->SetFont('dejavusans', '', 8);
    $pdf->Cell(12, 4, iconv('utf-8', 'utf-8', $school_class), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', 'классе указанного ниже учебного заведения'), '', 0, 'L', false, false, 1);

/*
$y+=5.5;
$pdf->SetFont('dejavusans','B', 8);
$pdf->SetXY(25,$y);
$pdf->Write(6,iconv('utf-8','utf-8', 'Сведения об учебном заведении:'),false,0,'L');
*/

    $y += 5.5;
    $pdf->SetFont('dejavusans', '', 8);
    $pdf->SetXY(25, $y);
    $pdf->Cell(25, 4, iconv('utf-8', 'utf-8', 'Наименование'), 0, 0, 'L');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $school_name), 'B', 0, 'C', false, false, 1);

    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(29, 4, iconv('utf-8', 'utf-8', 'Почтовый индекс'), 0, 0, 'L');
    $pdf->Cell(17, 4, iconv('utf-8', 'utf-8', $school_zip), 'B', 0, 'C');
    $pdf->Cell(13, 4, iconv('utf-8', 'utf-8', 'страна'), 0, 0, 'L');
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', $school_country), 'B', 0, 'C');
    $pdf->Cell(12, 4, iconv('utf-8', 'utf-8', 'регион'), 0, 0, 'L');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $school_region), 'B', 0, 'C', false, false, 1);

    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(11, 4, iconv('utf-8', 'utf-8', 'район'), 0, 0, 'L');
    $pdf->Cell(68, 4, iconv('utf-8', 'utf-8', $school_area), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', 'населенный пункт:'), 0, 0, 'L');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', $school_city), 'B', 0, 'C', false, false, 1);

    $y += 5.5;
    $pdf->SetXY(25, $y);
    $pdf->Cell(11, 4, iconv('utf-8', 'utf-8', 'улица'), 0, 0, 'L');
    $pdf->Cell(90, 4, iconv('utf-8', 'utf-8', $school_street), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(8, 4, iconv('utf-8', 'utf-8', 'дом'), 0, 0, 'С');
    $pdf->Cell(15, 4, iconv('utf-8', 'utf-8', $school_house), 'B', 0, 'C', false, false, 1);
    $pdf->Cell(8, 4, iconv('utf-8', 'utf-8', 'корп.'), 0, 0, 'С');
    $pdf->Cell(15, 4, iconv('utf-8', 'utf-8', $school_building), 'B', 0, 'C');

/*
$y+=5.5;
$pdf->SetXY(25,$y);
$pdf->Cell(32,3.5,iconv('utf-8','utf-8', 'Учитель математики'),0,0,'L',false,'',0,true); $pdf->Cell(0,3.5,iconv('utf-8','utf-8', $teacher_m),'B',0,'C',false,false,1,true);
$y+=5.5;
$pdf->SetXY(25,$y);
$pdf->Cell(26,3.5,iconv('utf-8','utf-8', 'Учитель физики'),0,0,'L',false,'',0,true); $pdf->Cell(0,3.5,iconv('utf-8','utf-8', $teacher_f),'B',0,'C',false,false,1,true);
$y+=5.5;
$pdf->SetXY(25,$y);
$pdf->Cell(35,4,iconv('utf-8','utf-8', 'Учитель информатики'),0,0,'L',false,'',0,true); $pdf->Cell(0,3.5,iconv('utf-8','utf-8', $teacher_i),'B',0,'C',false,false,1,true);
*/

    $y += 10;
    $pdf->SetXY(25, $y);
    $pdf->SetFont('dejavusans', 'B', 8);
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', 'Персональные данные, предоставленные мною сознательно и добровольно, соответствуют'), '', 2, 'L');
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', 'действительности и корректны.'), '', 2, 'L');
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', 'Ознакомлен с действующим Порядком проведения олимпиад школьников,'), '', 1, 'L');
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', 'Положением об Олимпиаде школьников "Надежда энергетики" и регламентом её проведения.'), '', 1, 'L');
    $pdf->SetFont('dejavusans', '', 6);
    $pdf->Cell(0, 3, iconv('utf-8', 'utf-8', 'оформляется родителем (законным представителем) несовершеннолетнего участника'), '', 1, 'C');


    $y += 25;
    $pdf->SetXY(25, $y);
    $pdf->SetFont('dejavusans', '', 8);

    $y += 4;

    $pdf->SetFont('dejavusans', '', 9);
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', ' '), 'B', 1, 'C', false, false, 1);
    $pdf->SetFont('dejavusans', '', 6);
    $pdf->Cell(0, 3, iconv('utf-8', 'utf-8', 'Фамилия имя и отчество родителя (законного представителя) участника, степень родства'), '', 1, 'C');
    $pdf->SetFont('dejavusans', '', 9);
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', ' '), 'B', 1, 'C', false, false, 1);
    $pdf->SetFont('dejavusans', '', 6);
    $pdf->Cell(0, 3, iconv('utf-8', 'utf-8', ' контактный телефон, e-mail  родителя (законного представителя) участника, '), '', 1, 'C');

    $pdf->SetFont('dejavusans', '', 8);
    $y += 16;
    $pdf->SetXY(25, $y);
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', $reg_date), 'B', 0, 'C', false, false, 1);
    $y += 4;
    $pdf->SetXY(180, $y);
    $pdf->SetFont('dejavusans', '', 7);
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', 'подпись'), 'T', 0, 'C', false, false, 1);


    $y += 90;
    $pdf->SetXY(130, $y);
    $pdf->SetFontSize(18);
    $x = 130;
    $w = 9;

    for ($i = 0; $i < 5; $i++) {
        $pdf->Cell($w, 10, $regnum[$i], 'LTB', 0, 'C');
    }
    $pdf->Cell($w, 10, '-', 'L', 0, 'C');
    $pdf->Cell($w, 10, $regnum[6], 'LTB', 0, 'C');
    $pdf->Cell($w, 10, $regnum[7], 1, 0, 'C');

    $pdf->SetFontSize(10);
    $pdf->MultiCell(85, 0, iconv("utf-8", "utf-8", 'В оргкомитет Олимпиады школьников "Надежда энергетики"'), 0, 'R', false, 0, 115, 25);

    $pdf->Image('./img/document_logo.gif', 25, 10, 26, 26, 'GIF');

    $pdf->SetXY(90, 30);

    $pdf->SetFont('dejavusans', 'B', 10);
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', 'Согласие'), '', 1, 'L');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', 'на обработку персональных данных'), '', 1, 'C');

    $pdf->SetFont('dejavusans', '', 8);

    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "Я, ____________________________________________________________________________________, являющийся законным представителем участника, в дальнейшем – Представитель, разрешаю оргкомитету Олимпиады школьников «Надежда энергетики» в лице ФГБОУ ВО «Национальный исследовательский университет «МЭИ», ФГБОУ ВО «Ивановский государственный энергетический университет имени В.И. Ленина», ФГБОУ ВО «Казанский государственный энергетический университет», ФГАОУ ВО «Сибирский федеральный университет», ФГБОУ ВО «Новгородский государственный университет имени Ярослава Мудрого», далее – Оргкомитет, обработку персональных данных участника (ФИО: $psurename $pname $ppatronymic), в дальнейшем – Субъекта, указанных в пункте 3, на следующих условиях.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "1. Представитель дает согласие на обработку Оргкомитетом персональных данных Субъекта, то есть совершения следующих действий: сбор, систематизацию, накопление, хранение, уточнение (обновление, изменение), использование, обезличивание, блокирование, уничтожение персональных данных и на передачу персональных данных в государственные информационные ресурсы (в том числе: в ФИС ГИА и Приема; в Российский совет олимпиад школьников; в Государственный информационный ресурс о детях, проявивших выдающиеся способности).\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "2. Оргкомитет обязуется использовать данные Субъекта для обеспечения и мониторинга участия в олимпиаде в соответствии с действующим законодательством Российской Федерации. Оргкомитет может раскрыть правоохранительным органам любую информацию по официальному запросу только в случаях, установленных законодательством Российской Федерации.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3. Перечень персональных данных, передаваемых Оргкомитету на обработку:\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.1. Фамилия, имя, отчество, дата рождения;\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.2. Пол, реквизиты документа, удостоверяющего личность и гражданство, номер СНИЛС;\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.3. Адрес постоянной регистрации, почтовый адрес, контактный телефон, контактный адрес электронной почты;\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.4. Наименование и адрес учебного заведения, класс.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "4. Отзыв согласия по инициативе Представителя возможен по письменному заявлению Представителя Субъекта.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "5. Срок, в течение которого действует согласие на обработку - до 31 декабря 2026 года.\n"));


    $pdf->SetFont('dejavusans', 'B', 10);
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', 'Согласие'), '', 1, 'C');
    $pdf->Cell(0, 4, iconv('utf-8', 'utf-8', 'на распространение персональных данных'), '', 1, 'C');

    $pdf->SetFont('dejavusans', '', 8);

    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "Я, ____________________________________________________________________________________, являющийся законным представителем участника, в дальнейшем – Представитель, разрешаю оргкомитету Олимпиады школьников «Надежда энергетики» в лице ФГБОУ ВО «Национальный исследовательский университет «МЭИ», ФГБОУ ВО «Ивановский государственный энергетический университет имени В.И. Ленина», ФГБОУ ВО «Казанский государственный энергетический университет», ФГАОУ ВО «Сибирский федеральный университет», ФГБОУ ВО «Новгородский государственный университет имени Ярослава Мудрого», далее – Оргкомитет, распространение персональных данных участника (ФИО: $psurename $pname $ppatronymic), в дальнейшем – Субъекта, указанных в пункте 3, на следующих условиях.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "1. Представитель дает согласие на распространение Оргкомитетом персональных данных Субъекта с целью организации и проведения Оргкомитетом олимпиады школьников и на публикацию персональных данных Субъекта в телекоммуникационной сети Интернет по адресу https://energy-hope.ru\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "2. Оргкомитет обязуется использовать данные Субъекта для обеспечения и мониторинга участия в олимпиаде в соответствии с действующим законодательством Российской Федерации. Оргкомитет может раскрыть правоохранительным органам любую информацию по официальному запросу только в случаях, установленных законодательством Российской Федерации.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3. Перечень персональных данных, передаваемых Оргкомитету с целью распространения на бумажных носителях и с использованием средств автоматизации:\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.1. Фамилия, имя, отчество;\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.2. Дата рождения;\n"));
    $pdf->SetX(35);
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "3.3. Результаты участия в этапах олимпиады.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "4. Представитель даёт согласие на публикацию олимпиадной работы Субъекта в сети Интернет.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "5. Субъект по письменному запросу имеет право на получение информации, касающейся обработки его персональных данных.\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "6. При поступлении в Оргкомитет письменного заявления о прекращении действия настоящего Согласия, персональные данные деперсонализируются в 15-дневный срок (кроме сведений, хранение которых обусловлено требованиями законодательства Российской Федерации).\n"));
    $pdf->Multicell(0, 3, iconv("utf-8", "utf-8", "7. Срок, в течение которого действует согласие на распространение - до 31 декабря 2022 года.\n"));
    $pdf->SetFont('dejavusans', '', 6);



    $pdf->SetFont('dejavusans', '', 8);

    $y += 10;

    $pdf->SetFont('dejavusans', '', 9);
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', ' '), 'B', 1, 'C', false, false, 1);
    $pdf->SetFont('dejavusans', '', 6);
    $pdf->Cell(0, 3, iconv('utf-8', 'utf-8', 'Фамилия имя и отчество родителя (законного представителя) участника, степень родства'), '', 1, 'C');

    $pdf->SetFont('dejavusans', '', 9);
    $pdf->Cell(0, 3.5, iconv('utf-8', 'utf-8', ' '), 'B', 1, 'C', false, false, 1);
    $pdf->SetFont('dejavusans', '', 6);
    $pdf->Cell(0, 3, iconv('utf-8', 'utf-8', ' контактный телефон, e-mail  родителя (законного представителя) участника, '), '', 1, 'C');

    $pdf->SetFont('dejavusans', '', 8);
    $y += 0;
    $pdf->SetX(25);
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', $reg_date), 'B', 0, 'C', false, false, 1);
    $y += 0;
    $pdf->SetXY(170, $y - 27);

    $pdf->SetFont('dejavusans', '', 7);
    $pdf->Cell(30, 4, iconv('utf-8', 'utf-8', 'подпись Представителя'), 'T', 1, 'C', false, false, 1);



    $res = $mysqli->query('insert into log_print_doc set doc="Заявление", uid=' . $USER_INFO['uid']);
    $res = $mysqli->query('update olimp_persons set printed_z="Y" where uid=' . $USER_INFO['uid'] . 'and printed_z!="A"');

//Close and output PDF document
    $pdf->Output('zayavlenie.pdf', 'I');
}
