<?php

// Сертификат участника.

ini_set('error_reporting', E_ALL);
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    if (!array_key_exists('exam', $_GET) || !($exam = (int)filter_input(INPUT_GET, 'exam', FILTER_SANITIZE_NUMBER_INT)) || !in_array($exam, array(1,2,3,4))) {
        return 'Неверный запрос';
    }
    if (array_key_exists('pid', $USER_INFO) && $USER_INFO['pid'] > 0) {
        $IdPerson = $USER_INFO['pid'];
        $Year = $SITE_VARS['current_year'];
        $Authorized = true;
    } elseif (array_key_exists('pid', $_GET) && ($IdPerson = (int)filter_input(INPUT_GET, 'pid', FILTER_SANITIZE_NUMBER_INT)) && ($IdPerson > 0) && array_key_exists('year', $_GET) && ($Year = (int)filter_input(INPUT_GET, 'year', FILTER_SANITIZE_NUMBER_INT)) && ($Year > 0) && array_key_exists('sign', $_GET) && ($sign = filter_input(INPUT_GET, 'sign', FILTER_SANITIZE_STRING))) {
        $req_sign = urldecode($sign);
        $Authorized = false;
    } else {
        return 'Неверный запрос';
    }

    $res = $mysqli->query('select p.reg_num,p.surename,p.name,p.patronymic,DATE_FORMAT(p.birthday,"%d.%m.%Y") AS bdt, p.school_class AS class from olimp_persons as p where id=' . $IdPerson);
    if ($res->num_rows < 1) {
        return 'Участник не найден';
    }
    $person = $res->fetch_assoc();

    $person['surename'] = ucfirst($person['surename']);
    $person['name'] = ucfirst($person['name']);
    $person['patronymic'] = ucfirst($person['patronymic']);

    $res = $mysqli->query('select a.`rank` as `rank`,a.result as result,s.stage as stage,s.classes, s.year as year, s.form as form from olimp_actions AS a LEFT JOIN olimp_stages as s ON (s.id=a.stage_id) where s.year=2021/*' . $Year . '*/ AND a.`rank` IN ("1","2","3","4") AND a.person_id=' . $IdPerson . ' AND s.exam=' . $exam . ' order by s.date_exam');
    if ($res->num_rows < 1) {
        if ($Authorized) {
            return 'Сертификат не найден';
        } else {
            return 'Сертификат не найден';
        }  // Заготовка на случай разных текстов
    }

    $results = array();
    while ($row = $res->fetch_assoc()) {
        $results[$row['stage']] = $row;
        $stage = $row;
    }

    $sign = sha1($IdPerson . $person['surename'] . $person['name'] . $person['patronymic'] . $person['bdt'] . $stage['year'] . $exam . 'Wu5S%jp_72GV-Fl=91i*sVzj$sgQ#ALb');
    if (!$Authorized && ($req_sign != $sign)) {
        return 'Сертификат не найден';
    }

    $sign = urlencode($sign);
    $URL = 'https://www.energy-hope.ru/documents/ehope_participant_cert?exam=' . $exam . '&pid=' . $IdPerson . '&year=' . $stage['year'] . '&sign=' . $sign;

    $exams = array(1 => 'Математика', 2 => 'Физика', 3 => 'Информатика', 4 => 'Комплекс предметов (математика, физика, информатика)');
    $forms = array(1 => 'в очной форме', 2 => 'в заочной форме', 4 => 'в заочной форме', 6 => 'в дистанционной форме (с системой контроля)');
    $ranks = array(1 => 'участник',2 => 'призер III ст.',3 => 'призер II ст.',4 => 'победитель');
    $balls = array(0 => 'баллов',1 => 'балл',2 => 'балла',3 => 'балла',4 => 'балла',5 => 'баллов',6 => 'баллов',7 => 'баллов',8 => 'баллов',9 => 'баллов',10 => 'баллов',11 => 'баллов',12 => 'баллов',13 => 'баллов',14 => 'баллов');


//require_once('./tcpdf/config/lang/rus.php');
//require_once('./tcpdf/tcpdf.php');
    require_once('TCPDF-main/tcpdf.php');
    require_once('TCPDF-main/config/lang/rus.php');


    $LeftMargin = 20;
    $RightMargin = $LeftMargin;

// create new PDF document
    $pdf = new TCPDF('P', 'mm', 'A4', true, 'UTF-8', false);

// set header and footer fonts
    $pdf->setPrintHeader(false);
    $pdf->setPrintFooter(false);
//set margins
//setPageUnit( string $unit)
//$pdf->setHeaderMargin(0);
    $pdf->SetMargins($LeftMargin, 16, $RightMargin, true);
    $pdf->setCellMargins(0, 0, 0, 0);
    $pdf->setCellPadding(0.5);


//set auto page breaks
    $pdf->SetAutoPageBreak(false, 10);

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
    'PrintPageRange' => array(),
    'NumCopies' => 1
    );

// Check the example n. 60 for advanced page settings

// set pdf viewer preferences
    $pdf->setViewerPreferences($preferences);

// ---------------------------------------------------------
// 'dejavusans','dejavuserif'

    $pdf->SetDrawColor(0, 0, 0);


    $pdf->AddPage();

    $pdf->Image('./../docs/background-diplom-blank.jpg', -2.5, 0, 210, 0, 'JPEG');

    $pdf->SetFont('dejavusans', 'B', 14);
    $pdf->SetTextColor(0, 0, 0);
    $y = 123;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'СЕРТИФИКАТ УЧАСТНИКА'), 0, 0, 'C');

    $pdf->SetFont('dejavusans', '', 26);
    $pdf->SetFontSize(26);
    $y += 12;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", $person['surename']), 0, 0, 'C', false, '', 1);
    $pdf->SetFontSize(24);
    $y += 10;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", $person['name'] . ' ' . $person['patronymic']), 0, 0, 'C', false, '', 1);
    $pdf->SetFontSize(14);
    $y += 15;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'дата рождения: ' . $person['bdt']), 0, 0, 'C', false, '', 1);
    $y += 6;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'класс обучения: ' . $stage['classes']), 0, 0, 'C', false, '', 1);
    $pdf->SetFont('dejavusans', 'B', 14);
    $y += 12;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'является участником'), 0, 0, 'C', false, '', 1);
    $pdf->SetFont('dejavusans', '', 14);
    $y += 11;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'Олимпиады школьников "Надежда энергетики"'), 0, 0, 'C', false, '', 1);
    $y += 10;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'по предмету'), 0, 0, 'C', false, '', 1);
    $y += 10;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", $exams[$exam]), 0, 0, 'C', false, '', 1);
    $y += 10;
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'сезона ' . ($stage['year'] - 1) . '/' . $stage['year'] . ' учебного года'), 0, 0, 'C', false, '', 1);

    $y = 237;
    $pdf->SetFont('dejavusans', '', 10);
    $pdf->SetXY($LeftMargin, $y);
    $pdf->Cell(130, 0, iconv("utf-8", "utf-8", 'Результаты участника:'), 0, 0, 'L', false, '', 1);

// a.rank as rank,a.result as result,s.stage as stage,s.form
    if (array_key_exists('1', $results)) {
        $b = $results[1]['result'] < 15 ? $results[1]['result'] : $results[1]['result'] - floor($results[1]['result'] / 10) * 10;
        $txt1 = 'Отборочный этап ' . $forms[$results[1]['form']] . ': ';
        $txt2 = $ranks[$results[1]['rank']] . ' (' . $results[1]['result'] . ' ' . $balls[$b] . ')';
        $y += 8;
        $pdf->SetXY($LeftMargin, $y);
        $pdf->Cell(130, 0, iconv("utf-8", "utf-8", $txt1), 0, 0, 'L', false, '', 1);
        $y += 5;
        $pdf->SetXY($LeftMargin, $y);
        $pdf->Cell(130, 0, iconv("utf-8", "utf-8", $txt2), 0, 0, 'L', false, '', 1);
    }
    if (array_key_exists('2', $results)) {
        $b = $results[2]['result'] < 15 ? $results[2]['result'] : $results[2]['result'] - floor($results[2]['result'] / 10) * 10;
        $txt1 = 'Заключительный этап ' . $forms[$results[2]['form']] . ': ';
        $txt2 = $ranks[$results[2]['rank']] . ' (' . $results[2]['result'] . ' ' . $balls[$b] . ')';
        $y += 8;
        $pdf->SetXY($LeftMargin, $y);
        $pdf->Cell(130, 0, iconv("utf-8", "utf-8", $txt1), 0, 0, 'L', false, '', 1);
        $y += 5;
        $pdf->SetXY($LeftMargin, $y);
        $pdf->Cell(130, 0, iconv("utf-8", "utf-8", $txt2), 0, 0, 'L', false, '', 1);
    }

// set style for barcode
    $style = array(
    'border' => false,
    'padding' => 0,
    'fgcolor' => array(0,0,0),
    'bgcolor' => array(255,255,255),
    'module_width' => 1, // width of a single module in points
    'module_height' => 1 // height of a single module in points
    );

// QRCODE,M : QR-CODE Medium error correction
    $pdf->write2DBarcode($URL, 'QRCODE,Q', 155, 237, 35, 35, $style, 'N');

    $pdf->SetFont('dejavusans', '', 9);
    $pdf->SetXY($LeftMargin, 275);
    $pdf->Cell(0, 0, iconv("utf-8", "utf-8", 'Проверка сертификата: ' . $URL), 0, 0, 'C', false, '', 1);

    if (!$Authorized) {
        $pdf->SetDrawColor(32, 128, 32);
        $pdf->SetTextColor(32, 128, 32);
        $pdf->SetFont('dejavusans', '', 14);
        $pdf->SetXY(7, 25);
        $pdf->StartTransform();
        $pdf->Rotate(15);
        $pdf->Cell(74, 12, iconv("utf-8", "utf-8", 'Сертификат подтвержден'), 1, 0, 'C', false, '', 1);
        $pdf->StopTransform();
    }

//Close and output PDF document
    $pdf->Output('ehope_participant_cert.pdf', 'I');
}
