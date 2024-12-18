<?php

// Загрузка диплома работы

function build_page($pgid, $subtype)
{
    global $SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    $exam = 0;

    $year = isset($PAGE['REQID']) ? $PAGE['REQID'] + 0 : 0;

    switch ($PAGE['REQTP'] . '') {
        case 'm': {$exam = 1;
            $ex = "M";
            break;
        }
        case 'p': {$exam = 2;
            $ex = "P";
            break;
        }
        case 'i': {$exam = 3;
            $ex = "I";
            break;
        }
        case 'k': {$exam = 4;
            $ex = "K";
            break;
        }
    }

    $doc_dir = '';
    switch ($PAGE['name']) {
        case 'ehope_diploma':
        {    $doc_dir = '../docs/diplomas' . $year;
            break;
        }
        case 'ehope_certificate':
        {    $doc_dir = '../docs/certificates' . $year;
            break;
        }
    }


    if (empty($exam) || empty($year) || empty($doc_dir)) {
        die('Запрошен неверный файл ');
    }


    $file_name = $doc_dir . '/' . $ex . $USER_INFO['pid'] . '.pdf';    //echo $file_name;


    if (!(file_exists($file_name))) {
        die('Запрошен неверный файл '); // die(header('HTTP/1.1 404 Not Found'));
    }


    $s = filesize($file_name);

    $f1 = fopen($file_name, 'rb');

    header('Accept-Ranges: none');
    header('Cache-Control: no-cache');
    header('Pragma: no-cache');

    header("Content-Transfer-Encoding: binary");
//    header('Content-Disposition: attachment; filename="' . $PAGE['name'] . '"');

    header('Content-Type: application/pdf');
//    header('Content-Length: '.$s);

    fpassthru($f1);

    fclose($f1);
}
