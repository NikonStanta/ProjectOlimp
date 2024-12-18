<?php

// Документы. Пропуск участника.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    if ($USER_INFO['uid'] < 1) {
        return $tpl['cabinet']['logon_form'];
    }

    require_once('./tcpdf/config/lang/rus.php');
    require_once('./tcpdf/tcpdf.php');

    $res = $mysqli->query('select reg_num,surename,name,patronymic from olimp_persons where uid=' . $USER_INFO['uid']);
    $person = $res->fetch_assoc();


    $regnum = $person['reg_num'];
    $surename = $person['surename'];
    $name = $person['name'];
    $patronymic = $person['patronymic'];

    $title1 = $SITE_VARS['doc_propusk_title1'];
    $text1 = $SITE_VARS['doc_propusk_text1'];
    $text2 = $SITE_VARS['doc_propusk_text2'];

    $surename = str_pad(ucfirst_r($surename), 32);
    $name = str_pad(ucfirst_r($name), 32);
    $patronymic = str_pad(ucfirst_r($patronymic), 32);


// create new PDF document
    $pdf = new TCPDF('L', PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

// set header and footer fonts
    $pdf->setPrintHeader(false);
    $pdf->setPrintFooter(false);
//set margins
//setPageUnit( string $unit)
//$pdf->setHeaderMargin(0);
    $pdf->SetMargins(10, 10, 10);
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
    'PrintPageRange' => array(),
    'NumCopies' => 1
    );

// Check the example n. 60 for advanced page settings

// set pdf viewer preferences
    $pdf->setViewerPreferences($preferences);

// ---------------------------------------------------------
// 'dejavusans','dejavuserif'

    $pdf->SetDrawColor(0, 0, 0);
    $pdf->SetTextColor(0, 0, 0);

    $pdf->AddPage();

    $pdf->SetFont('dejavusans', '', 12);
    $pdf->SetXY(175, 10);
    $pdf->Cell(80, 8, iconv("cp1251", "utf-8", 'ОЛИМПИАДА ШКОЛЬНИКОВ'), 0, 0, 'C');
    $pdf->SetFontSize(16);
    $pdf->SetXY(175, 17);
    $pdf->Cell(80, 8, iconv("cp1251", "utf-8", '«НАДЕЖДА ЭНЕРГЕТИКИ»'), 0, 0, 'C');
    $pdf->Image('./img/document_logo.gif', 190, 27, 50, 50, 'GIF');

    $pdf->SetFontSize(32);
    $pdf->SetXY(175, 80);
    $pdf->Cell(85, 15, iconv("cp1251", "utf-8", 'ПРОПУСК'), 0, 0, 'C');
    $pdf->SetFontSize(26);
    $pdf->SetXY(10, 10);
    $pdf->Cell(130, 20, iconv("cp1251", "utf-8", $title1), 0, 0, 'C');

    $pdf->SetFontSize(12);
    $pdf->writeHTMLCell(130, 100, 10, 32, iconv("cp1251", "utf-8", $text1));

    $pdf->Line(10, 104, 286, 104);
    $pdf->Line(148, 10, 148, 100);

    $pdf->AddPage();

    $pdf->Cell(30, 40, iconv("cp1251", "utf-8", 'Фото 3х4 см'), 1, 1, 'C');

    $pdf->SetFontSize(12);
    $pdf->SetXY(42, 43);
    $pdf->Cell(20, 10, iconv("cp1251", "utf-8", 'М.П.'), 0, 0, 'L');
    $pdf->SetFontSize(18);
    $x = 70;
    $w = 9;
    $pdf->SetXY($x, 90);
    for ($i = 0; $i < 5; $i++) {
        $pdf->Cell($w, 10, $regnum[$i], 'LTB', 0, 'C');
    }
    $pdf->Cell($w, 10, '-', 'L', 0, 'C');
    $pdf->Cell($w, 10, $regnum[6], 'LTB', 0, 'C');
    $pdf->Cell($w, 10, $regnum[7], 1, 0, 'C');

    $pdf->SetFontSize(9);
    $pdf->SetXY(10, 65);
    $pdf->Cell(90, 6, iconv("cp1251", "utf-8", 'Фамилия, Имя, Отчество участника'), 0, 0, 'L');
    $pdf->SetXY(80, 10);
    $pdf->Cell(90, 8, iconv("cp1251", "utf-8", 'Для заметок'), 0, 0, 'С');
    $pdf->Line(80, 25, 135, 25);
    $pdf->Line(80, 35, 135, 35);
    $pdf->Line(80, 45, 135, 45);
    $pdf->Line(80, 55, 135, 55);

    $pdf->Line(148, 10, 148, 100);

    $pdf->Line(10, 104, 286, 104);

    $x = 10;
    $y = 70;
    $w = 4.2;
    $h = 5;
    $pdf->SetXY($x, $y);
    for ($i = 0; $i < 32; $i++) {
        $pdf->Cell($w, $h, iconv("cp1251", "utf-8", $surename[$i]), ($i != 31 ? 'LTB' : 1), 0, 'C');
    }
    $pdf->SetXY($x, $y + $h);
    for ($i = 0; $i < 32; $i++) {
        $pdf->Cell($w, $h, iconv("cp1251", "utf-8", $name[$i]), ($i != 31 ? 'LTB' : 1), 0, 'C');
    }
    $pdf->SetXY($x, $y + 2 * $h);
    for ($i = 0; $i < 32; $i++) {
        $pdf->Cell($w, $h, iconv("cp1251", "utf-8", $patronymic[$i]), ($i != 31 ? 'LTB' : 1), 0, 'C');
    }

    $pdf->SetFillColor(50, 50, 50);
    $pdf->writeHTMLCell(130, 90, 155, 10, iconv("cp1251", "utf-8", $text2));


    $res = $mysqli->query('insert into log_print_doc set doc="Пропуск", uid=' . $USER_INFO['uid']);
    $res = $mysqli->query('update olimp_persons set printed_p="Y" where uid=' . $USER_INFO['uid']);

//Close and output PDF document
    $pdf->Output('propusk.pdf', 'I');
}
