<?php

// Построение страницы вопросы и ответы

function build_page($pgid, $subtype)
{
    global $SITE_VARS,$PAGE,$tpl, $mysqli;

    if (!empty($SITE_VARS['FAQTopicIdLength'])) {
        $_GROUP_LEN = $SITE_VARS['FAQTopicIdLength'];
    } else {
        $_GROUP_LEN = 4;
    }
    if (!empty($SITE_VARS['FAQItemIdLength'])) {
        $_ITEM_LEN = $SITE_VARS['FAQItemIdLength'];
    } else {
        $_ITEM_LEN = 4;
    }

    $cmd = '';
    if ((($PAGE['REQTP'] == '') || ($PAGE['REQTP'] == 't') || ($PAGE['REQTP'] == 'T')) && (($PAGE['REQID'] + 0) > 0)) {
        $tid = $PAGE['REQID'] + 0;
    } else {
        if (!empty($_REQUEST['tid'])) {
            $tid = $_REQUEST['tid'];
        } else {
            $tid = 0;
        }
    }

    if ($_REQUEST['cmd'] == 'ask') {
        $cmd = 'Ask';
    }

    if (!empty($_REQUEST['btnSend'])) {
        $cmd = 'doAsk';
    }

// Обработка команд

    switch ($cmd) {
        case 'doAsk':
        {    $fio = trim(strip_tags($_POST['fio']));
            $org = trim(strip_tags($_POST['org']));
            $phone = trim(strip_tags($_POST['phone']));
            $email = trim(strip_tags($_POST['email']));
            $txt = nl2br(trim(strip_tags($_POST['txt'])));

        // Проверка правильности заполнения полей

            if (empty($txt)) {
                $error .= 'Необходимо ввести содержание вопроса. ';
            }

        // Построение и отправка вопроса

            if (empty($error)) {
                $body = preg_replace(array('/%fio%/','/%org%/','/%phone%/','/%email%/','/%txt%/'), array($fio,$org,$phone,$email,$txt), $tpl['faq']['mail']);
                $headers = "MIME-Version: 1.0\r\nX-Priority: 1\r\nX-MSMail-Priority: High\r\n";
                $headers .= "Content-type: text/html; charset=windows-1251\r\n";
                $headers .= "From: " . $SITE_VARS['Mail From'] . "\r\n";
                $subject = '=?Windows-1251?B?' . chunk_split(base64_encode('Вопрос')) . '=?=';
                if (!mail($SITE_VARS['Questions e-mail'], $subject, $body, $headers)) {
                    $error = 'Непредвиденная ошибка при отправке вопроса, попробуйте повторить через минуту.';
                    $cmd = 'Ask';
                }
            } else {
                $cmd = 'Ask';
            }
            break;

        }
    }

// Формирование страницы

    switch ($cmd) {
        case 'doAsk':
        {    $body = $tpl['faq']['sended'];
            break;

        }

        case 'Ask':
        {    // Форма вопроса
            $body = preg_replace(array('/%fio%/','/%org%/','/%phone%/','/%email%/','/%txt%/','/%error%/'), array($_POST['fio'],$_POST['org'],$_POST['phone'],$_POST['email'],$_POST['txt'],$error), $tpl['faq']['form']);
            break;

        }

        default:
        {

            // Список тематик вопросов

            $res = $mysqli->query('select id,name from faq_topics order by ord');
            while ($row = $res->fetch_row()) {
                    $topics[$row[0]] = $row[1];
            }

            // Выбранная тематика
            if (empty($tid) || empty($topics[$tid])) {
                    $tid = key($topics);
            }

            // Список тематик

            if (count($topics) > 1) {
                    $w = '';
                foreach ($topics as $k => $v) {
                    $w .= preg_replace(
                        array('/%id%/','/%name%/','/%refid%/'),
                        array($k,$v,$PAGE['name'] . sprintf('%0' . $_GROUP_LEN . 'd', $k) . $SITE_VARS['Default ext']),
                        $k == $tid ? $tpl['faq']['topic_cline'] : $tpl['faq']['topic_line']
                    );
                }
                $topic_list = preg_replace('/%list%/', $w, $tpl['faq']['topics']);
            } else {
                $topic_list = '';
            }

        // Вопросы выбранной тематики

            $w = '';
            $res = $mysqli->query('select question,answer,id from faq_data where tid=' . $tid . ' order by ord');
            while ($row = $res->fetch_row()) {
                $w .= preg_replace(
                    array('/%question%/','/%answer%/','/%id%/','/%bookid%/'),
                    array(stripslashes($row[0]),stripslashes($row[1]),$row[2],sprintf('%0' . $_ITEM_LEN . 'd', $row[2])),
                    $tpl['faq']['data_line']
                );
            }
            $body = preg_replace(
                array('/%topic%/','/%topics%/','/%list%/'),
                array($topics[$tid],$topic_list,$w),
                $tpl['faq']['body']
            );
                break;

        }
    }
    return $body;
}
