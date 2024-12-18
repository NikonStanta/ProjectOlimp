<?php

// Личный кабинет. Документы.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');
include_once('../inc/cabinet2.inc');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    if ($USER_INFO['uid'] < 1) {
        return $tpl['cabinet']['logon_form'];
    }

    if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) {
        return $tpl['cabinet']['closed'];
    }

// Перерегистрация участников прошлого года
    $message = '';
    include_once('../inc/reregistrate.inc');

    $check_result = check_persons_data($USER_INFO['pid']);

    $docs_dir = '../docs/applications';
    $max_file_size = 10485760;
    $max_file_sizes = array();
    $error = '';
    $warning = '';
//$allowed_file_types=array('image/jpeg'=>0,'application/pdf'=>1,'application/x-rar-compressed'=>0);

    $allowed_file_types = array(
    'Application' => array(/*'image/jpeg'=>0,*/'application/pdf' => 1/*,'application/x-rar-compressed'=>0*/),
    'SchoolDoc' => array('image/jpeg' => 1,'application/pdf' => 1),
    'IdentDoc' => array('image/jpeg' => 1,'application/pdf' => 1),
    'IdentPhoto' => array('image/jpeg' => 1)
    );

    $cmd = '';

    if (!empty($_POST['btnUploadApplication'])) {
        $cmd = 'UploadApplication';
    }
    if (!empty($_POST['btnUploadIdentDoc'])) {
        $cmd = 'UploadIdentDoc';
    }
    if (!empty($_POST['btnUploadSchoolDoc'])) {
        $cmd = 'UploadSchoolDoc';
    }
    if (!empty($_POST['btnUploadIdentPhoto'])) {
        $cmd = 'UploadIdentPhoto';
    }

    if (!empty($_POST['btnDeleteApplication'])) {
        $cmd = 'DeleteApplication';
    }


    switch ($cmd) {
        case 'DeleteApplication':
        {
            $res = $mysqli->query('select count(stage_id) from olimp_actions as a left join olimp_stages as s on(s.id=a.stage_id) where s.year="' . $USER_INFO['year'] . '"  AND person_id=' . ($USER_INFO['pid'] + 0));
            $row = $res->fetch_row();
            $USER_INFO['stage_regs'] = $row[0];
            $block_edit_application = (!empty($USER_INFO['stage_regs'])) || (!empty($SITE_VARS['block_edit'])) || ($USER_INFO['printed_z'] == 'A');
            if (!$block_edit_application) {
                $mysqli->query('update olimp_persons set printed_z="N" WHERE id=' . $USER_INFO['pid']);
                $USER_INFO['printed_z'] = 'N';
            } else {
                $error = 'Невозможно удалить заявление. Вы уже зарегистрированы в этап. Свяжитесь с оргкомитетом в случае необходимости изменения данных в личном кабинете.';
            }
        }

        case 'UploadApplication':
        {    $cmd = '';
            if (is_uploaded_file($_FILES['applicationfile']['tmp_name'])) {
                $FileType = mime_content_type($_FILES['applicationfile']['tmp_name']);
                if (!array_key_exists($FileType, $allowed_file_types['Application'])) {
                    $error = 'Недопустимый формат файла. Заявление не загружено.';
                    break;
                }

                if ($_FILES['applicationfile']['size'] <= $max_file_size) {
                    $application_file = $docs_dir . '/application' . $USER_INFO['pid'];
                    if (file_exists($application_file)) {
                        $application_file_old = $docs_dir . '/application' . $USER_INFO['pid'] . '_' . date('YmdHis');
                        rename($application_file, $application_file_old);
                    } else {
                        $application_file_old = '';
                    }
                    if (move_uploaded_file($_FILES['applicationfile']['tmp_name'], $application_file)) {
                        $mysqli->query('update olimp_persons set printed_z="L" WHERE id=' . $USER_INFO['pid']);
                        $USER_INFO['printed_z'] = 'L';
                        $message = 'Заявление загружено.';
                    } else {
                        if ($application_file_old) {
                                        rename($application_file_old, $application_file);
                                            $error = 'Ошибка при загрузке файла. Новое заявление не загружено.';
                        } else {
                            $error = 'Ошибка при загрузке файла. Заявление не загружено.';
                        }
                    }
                } else {
                    $error = 'Слишком большой файл. Заявление не загружено.';
                    break;
                }
            } else {
                $error = 'Ошибка при загрузке файла, возможно слишком большой файл. Заявление не загружено.';
                break;
            }
            break;
        }
        case 'UploadIdentDoc':
        {    $cmd = '';
            if (is_uploaded_file($_FILES['identdocfile']['tmp_name'])) {
                $FileType = mime_content_type($_FILES['identdocfile']['tmp_name']);
                if (!array_key_exists($FileType, $allowed_file_types['IdentDoc'])) {
                    $error = 'Недопустимый формат файла. Документ не загружен.';
                    break;
                }

                if ($_FILES['identdocfile']['size'] <= $max_file_size) {
                    $identdoc_file = $docs_dir . '/identdoc' . $USER_INFO['pid'];
                    $_LoadStatus = 'Loaded';
                    if (file_exists($identdoc_file)) {
                        $identdoc_file_old = $docs_dir . '/identdoc' . $USER_INFO['pid'] . '_' . date('YmdHis');
                        rename($identdoc_file, $identdoc_file_old);
                        $_LoadStatus = 'Updated';
                    } else {
                        $identdoc_file_old = '';
                    }
                    if (move_uploaded_file($_FILES['identdocfile']['tmp_name'], $identdoc_file)) {
                        $mysqli->query('update olimp_persons set IdentDoc="' . $_LoadStatus . '" WHERE id=' . $USER_INFO['pid']);
                        $check_result['IdentDoc'] = $_LoadStatus;
                        $message = 'Документ загружен.';
                    } else {
                        if ($identdoc_file_old) {
                                        rename($identdoc_file_old, $identdoc_file);
                                            $error = 'Ошибка при загрузке файла. Новый документ не загружен.';
                        } else {
                            $error = 'Ошибка при загрузке файла. Документ не загружен.';
                        }
                    }
                } else {
                    $error = 'Слишком большой файл. Документ не загружен.';
                    break;
                }
            } else {
                $error = 'Ошибка при загрузке файла. Документ не загружен.';
                break;
            }
            break;
        }
        case 'UploadSchoolDoc':
        {    $cmd = '';
            if (is_uploaded_file($_FILES['schooldocfile']['tmp_name'])) {
                $FileType = mime_content_type($_FILES['schooldocfile']['tmp_name']);
                if (!array_key_exists($FileType, $allowed_file_types['SchoolDoc'])) {
                    $error = 'Недопустимый формат файла. Справка не загружена.';
                    break;
                }

                if ($_FILES['schooldocfile']['size'] <= $max_file_size) {
                    $schooldoc_file = $docs_dir . '/schooldoc' . $USER_INFO['pid'];
                    $_LoadStatus = 'Loaded';
                    if (file_exists($schooldoc_file)) {
                        $schooldoc_file_old = $docs_dir . '/schooldoc' . $USER_INFO['pid'] . '_' . date('YmdHis');
                        rename($schooldoc_file, $schooldoc_file_old);
                        $_LoadStatus = 'Updated';
                    } else {
                        $schooldoc_file_old = '';
                    }
                    if (move_uploaded_file($_FILES['schooldocfile']['tmp_name'], $schooldoc_file)) {
                        $mysqli->query('update olimp_persons set SchoolDoc="' . $_LoadStatus . '" WHERE id=' . $USER_INFO['pid']);
                        $check_result['SchoolDoc'] = $_LoadStatus;
                        $message = 'Справка загружена.';
                    } else {
                        if ($schooldoc_file_old) {
                                        rename($schooldoc_file_old, $schooldoc_file);
                                            $error = 'Ошибка при загрузке файла. Новая справка не загружена.';
                        } else {
                            $error = 'Ошибка при загрузке файла. Справка не загружена.';
                        }
                    }
                } else {
                    $error = 'Слишком большой файл. Справка не загружена.';
                    break;
                }
            } else {
                $error = 'Ошибка при загрузке файла. Справка не загружена.';
                break;
            }
            break;
        }

        case 'UploadIdentPhoto':
        {    $cmd = '';
            if (is_uploaded_file($_FILES['identphotofile']['tmp_name'])) {
                $FileType = mime_content_type($_FILES['identphotofile']['tmp_name']);
                if (!array_key_exists($FileType, $allowed_file_types['IdentPhoto'])) {
                    $error = 'Недопустимый формат файла. Фотография не загружена.';
                    break;
                }

                if ($_FILES['identphotofile']['size'] <= $max_file_size) {
                    $identphoto_file = $docs_dir . '/identphoto' . $USER_INFO['pid'];
                    $_LoadStatus = 'Loaded';
                    if (file_exists($identphoto_file)) {
                        $identphoto_file_old = $docs_dir . '/identphoto' . $USER_INFO['pid'] . '_' . date('YmdHis');
                        rename($identphoto_file, $identphoto_file_old);
                        $_LoadStatus = 'Updated';
                    } else {
                        $identphoto_file_old = '';
                    }
                    if (move_uploaded_file($_FILES['identphotofile']['tmp_name'], $identphoto_file)) {
                        $mysqli->query('update olimp_persons set IdentPhoto="' . $_LoadStatus . '" WHERE id=' . $USER_INFO['pid']);
                        $check_result['IdentPhoto'] = $_LoadStatus;
                        $message = 'Фотография загружена.';
                    } else {
                        if ($identphoto_file_old) {
                                        rename($identphoto_file_old, $identphoto_file);
                                            $error = 'Ошибка при загрузке файла. Новая фотография не загружена.';
                        } else {
                            $error = 'Ошибка при загрузке файла. Фотография не загружена.';
                        }
                    }
                } else {
                    $error = 'Слишком большой файл. Фотография не загружена.';
                    break;
                }
            } else {
                $error = 'Ошибка при загрузке файла. Фотография не загружена.';
                break;
            }
            break;
        }
    }

    $content1 = '';
// Заявление на участие

    if ($check_result['All is entered']) {
        $content1 .= str_replace(
            array('%title%','%text%','%action%','%name%','%btntitle%','%errors%','%nameLoad%','%btntitleLoad%','%filename%','%LoadDisabled%','%Loaded%'),
            array($tpl['cabinet_docs']['title_doc1'],$tpl['cabinet_docs'][$check_result['printed_z'] == 'A' ? 'text_doc1a' : ($check_result['printed_z'] == 'R' ? 'text_doc1n' : 'text_doc1r')],'/documents/zayavlenie.pdf','zayavlenie',$tpl['cabinet_docs']['title_btn1'],'','UploadApplication','Загрузить новое заявление','applicationfile','',(($USER_INFO['printed_z'] == 'A') || ($USER_INFO['printed_z'] == 'L') ) ? '<span style="font-weight:bold; color:green;">Загружено. Заявление находится на проверке в оргкомитете олимпиады<br></span><input type="submit" name="btnDeleteApplication" id="btnDeleteApplication" value="Удалить загруженное заявление" class="ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all">' : '<span style="font-weight:bold; color:red;">Необходимо загрузить&nbsp;&nbsp;&nbsp;&nbsp;</span>'),
            //$tpl['cabinet_docs'][$check_result['Person confirmed'] ? 'doc_Available' : 'doc_Required']);
            $tpl['cabinet_docs'][$check_result['printed_z'] == 'A' ? 'doc_Available' : 'doc_Required']
        );
    } else {
        $errlist = '<ul>';
        $wa = $check_result['errors'];
        foreach ($wa as $w) {
            $errlist .= '<li>' . $w . '</li>';
        }
        $errlist .= '</ul>';
        $content1 .= str_replace(
            array('%title%','%text%','%action%','%name%','%btntitle%','%errors%'),
            array($tpl['cabinet_docs']['title_doc1'],$tpl['cabinet_docs']['text_doc1u'],'','','',$errlist),
            $tpl['cabinet_docs']['doc_Unavailable']
        );
    }



//if ($_SESSION['debug']>0)
    {
// Справка из школы

    switch ($check_result['SchoolDoc']) {
        case 'Loaded':
        case 'Updated':
            $tplName = 'doc_Loaded';
            $docStatus = '<span style="font-weight:bold; color:#95500c;">Справка загружен</span>';
            $btnTitle = 'Загрузить другую справку';
            $docReason = '<div style="font-weight:bold; color:#95500c; font-size:16px;">Загруженная справка на проверке в оргкомитете олимпиады</div>';
            break;
        case 'Rejected':
            $tplName = 'doc_NotLoaded';
            $docStatus = '<span style="font-weight:bold; color:red;">Необходимо загрузить</span>';
            $btnTitle = 'Загрузить справку';
            $docReason = '<div style="font-weight:bold; color:red; font-size:16px;">Ранее загруженная справка не принята:</div><br>';
            $res = $mysqli->query('select DATE_FORMAT(dt,"%Y-%m-%d %H:%i"),txt from olimp_doc_reasons where docType="school" AND idPerson=' . $USER_INFO['pid'] . ' order by dt desc');
            while ($row = $res->fetch_row()) {
                $docReason .= str_replace(array('%dt%','%txt%'), $row, $tpl['cabinet_docs']['docReason']);
            }
            break;
        case 'Accepted':
            $tplName = 'doc_Accepted';
            break;
        case 'Not':
        default:
            $tplName = 'doc_NotLoaded';
            $docStatus = '<span style="font-weight:bold; color:red;">Необходимо загрузить</span>';
            $btnTitle = 'Загрузить справку';
            $docReason = '';
            break;
    }

    $content1 .= str_replace(
        array('%title%','%text%','%image%','%loadCmd%','%loadButtonTitle%','%loadFilename%','MAX_FILE_SIZE','%docStatus%','%docReason%'),
        array(
            $tpl['cabinet_docs']['title_school'],
            $tpl['cabinet_docs']['text_school_' . $tplName],
            'idx_ban_doc.gif',
            'UploadSchoolDoc',
            $btnTitle,
            'schooldocfile',
            $max_file_size,
            $docStatus,
            $docReason),
        $tpl['cabinet_docs'][$tplName]
    );


// Идентификационный документ

    switch ($check_result['IdentDoc']) {
        case 'Loaded':
        case 'Updated':
            $tplName = 'doc_Loaded';
            $docStatus = '<span style="font-weight:bold; color:#95500c;">Документ загружен</span>';
            $btnTitle = 'Загрузить другой документ';
            $docReason = '<div style="font-weight:bold; color:#95500c; font-size:16px;">Загруженный документ на проверке в оргкомитете олимпиады</div>';
            break;
        case 'Rejected':
            $tplName = 'doc_NotLoaded';
            $docStatus = '<span style="font-weight:bold; color:red;">Необходимо загрузить</span>';
            $btnTitle = 'Загрузить документ';
            $docReason = '<div style="font-weight:bold; color:red; font-size:16px;">Ранее загруженный документ не принят:</div><br>';
            $res = $mysqli->query('select DATE_FORMAT(dt,"%Y-%m-%d %H:%i"),txt from olimp_doc_reasons where docType="ident" AND idPerson=' . $USER_INFO['pid'] . ' order by dt desc');
            while ($row = $res->fetch_row()) {
                $docReason .= str_replace(array('%dt%','%txt%'), $row, $tpl['cabinet_docs']['docReason']);
            }
            break;
        case 'Accepted':
            $tplName = 'doc_Accepted';
            break;
        case 'Not':
        default:
            $tplName = 'doc_NotLoaded';
            $docStatus = '<span style="font-weight:bold; color:red;">Необходимо загрузить</span>';
            $btnTitle = 'Загрузить документ';
            $docReason = '';
            break;
    }

    $content1 .= str_replace(
        array('%title%','%text%','%image%','%loadCmd%','%loadButtonTitle%','%loadFilename%','MAX_FILE_SIZE','%docStatus%','%docReason%'),
        array(
            $tpl['cabinet_docs']['title_ident'],
            $tpl['cabinet_docs']['text_ident_' . $tplName],
            'idx_ban_doc.gif',
            'UploadIdentDoc',
            $btnTitle,
            'identdocfile',
            $max_file_size,
            $docStatus,
            $docReason),
        $tpl['cabinet_docs'][$tplName]
    );

// Идентификационная фотография

    switch ($check_result['IdentPhoto']) {
        case 'Loaded':
        case 'Updated':
            $tplName = 'doc_Loaded';
            $docStatus = '<span style="font-weight:bold; color:#95500c;">Фотография загружена</span>';
            $btnTitle = 'Загрузить другую фотографию';
            $docReason = '<div style="font-weight:bold; color:#95500c; font-size:16px;">Загруженная фотография на проверке в оргкомитете олимпиады</div>';
            break;
        case 'Rejected':
            $tplName = 'doc_NotLoaded';
            $docStatus = '<span style="font-weight:bold; color:red;">Необходимо загрузить</span>';
            $btnTitle = 'Загрузить фотографию';
            $docReason = '<div style="font-weight:bold; color:red; font-size:16px;">Ранее загруженная фотография не принята:</div><br>';
            $res = $mysqli->query('select DATE_FORMAT(dt,"%Y-%m-%d %H:%i"),txt from olimp_doc_reasons where docType="photo" AND idPerson=' . $USER_INFO['pid'] . ' order by dt desc');
            while ($row = $res->fetch_row()) {
                $docReason .= str_replace(array('%dt%','%txt%'), $row, $tpl['cabinet_docs']['docReason']);
            }
            break;
        case 'Accepted':
            $tplName = 'doc_Accepted';
            break;
        case 'Not':
        default:
            $tplName = 'doc_NotLoaded';
            $docStatus = '<span style="font-weight:bold; color:red;">Необходимо загрузить</span>';
            $btnTitle = 'Загрузить фотографию';
            $docReason = '';
            break;
    }

    $content1 .= str_replace(
        array('%title%','%text%','%image%','%loadCmd%','%loadButtonTitle%','%loadFilename%','MAX_FILE_SIZE','%docStatus%','%docReason%'),
        array(
            $tpl['cabinet_docs']['title_photo'],
            $tpl['cabinet_docs']['text_photo_' . $tplName],
            'idx_ban_doc.gif',
            'UploadIdentPhoto',
            $btnTitle,
            'identphotofile',
            $max_file_size,
            $docStatus,
            $docReason),
        $tpl['cabinet_docs'][$tplName]
    );


    }

// Пропуск
/*
$content1.=str_replace(
    array('%title%','%text%','%action%','%name%','%btntitle%','%errors%'),
    array($tpl['cabinet_docs']['title_doc2'],$tpl['cabinet_docs'][$check_result['Pass confirmed']  ? 'text_doc2a' : 'text_doc2r'],'/documents/propusk.pdf','propusk',$tpl['cabinet_docs']['title_btn2'],''),
    $tpl['cabinet_docs'][ 'doc_Available']);
*/
//    $tpl['cabinet_docs'][$check_result['Pass confirmed'] ? 'doc_Available' : 'doc_Required']);


// Формирование страницы

    $message = '';
    $msgs = build_message_list($USER_INFO['pid'], 1, true);
    foreach ($msgs as $msg) {
        $message .= str_replace(array('%id%','%dt%','%from%','%title%','%body%'), $msg, $tpl['cabinet']['message_line']);
    }
    if (!empty($message)) {
        $message = str_replace('%message%', $message, $tpl['cabinet']['message']);
    }

    if (!empty($error)) {
        $error = preg_replace('/%error%/', preg_replace(array('/%title%/','/%text%/'), array('Ошибка!',$error), $tpl['common']['error']), $tpl['cabinet']['error']);
    }
    if (!empty($warning)) {
        $warning = preg_replace('/%warning%/', preg_replace(
            array('/%title%/','/%text%/'),
            array('Внимание!',$warning),
            $tpl['common']['highlight']
        ), $tpl['cabinet']['warning']);
    }

    $body = preg_replace(
        array('/%error%/','/%warning%/','/%message%/','/%doc_list%/'),
        array($error,$warning,$message,$content1),
        $tpl['cabinet_docs']['main']
    );

    return $body;
}
