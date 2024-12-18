<?php

// Личный кабинет. Заочный этап.

include_once('common.php');
include_once('cabinet.php');
include_once('cabinet2.php');

function build_page($pgid, $subtype)
{
    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

    $var_dir = '/documents';
    $var_file_prefix = 'ehope_variant';
    $var_file_ext = 'rar';

    $strRespond['debug'] = 1;    // 1 - режим отладки, 0 - без режима отладки.
    $strRespond['error'] = 0;
    $strRespond['get_var_ref'] = "";
    $strRespond['get_blanks_ref'] = "";

    if ($USER_INFO['uid'] < 1) {
        return $tpl['cabinet']['logon_form'];
    }


    if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) {
        return $tpl['cabinet']['closed'];
    }

// Перерегистрация участников прошлого года
    $message = '';
    include_once('reregistrate.php');

    $check_result = check_persons_data($USER_INFO['pid']);
    $error = '';
    $warning = '';

    $content1 = '';
// Варианты заочного этапа
    $res = $mysqli->query('select a.stage_id,e.name as exam_name,e.abbr as exam_abbr,s.form, s.stage as stage, a.audit as audit1, g.audit as audit2, a.presence, s.var_pwd as varpwd, s.date_exam 
      from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_exams as e on (e.id=s.exam) left join olimp_groups as g ON (g.id=a.group_id) 
      where (s.date_post>Now()) AND (a.varnum>0) AND (a.presence<>"D") AND (a.presence<>"Y") AND (a.presence<>"N") AND (a.person_id=' . $USER_INFO['pid'] . ') order by s.date_exam');
    while ($row = $res->fetch_assoc()) {
        $stage = '***' . $row['stage'] . '***';
        $stage_mobile = '***' . $row['stage'] . '***';
        switch ($row['stage']) {
            case 1:
                $stage = 'Отборочный этап';
                $stage_mobile = 'Отборочный этап';
                break;
            case 2:
                $stage = 'Заключительный этап';
                $stage_mobile = 'Заключительный этап';
                break;
            case 3:
                $stage = 'Тренировочный этап';
                $stage_mobile = 'Тренировочный этап';
                break;
        }

        switch ($row['form']) {
            case 2:                
                $stage .= ' в заочной форме';                
                $varpwd = '';
                $delay = '';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . '.' . $var_file_ext;
                check_pwd_param($row['stage_id'], $varpwd, $delay);
                $info = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl['cabinet']['info_2']);
                // mobile ~ok.

                $tpl_cabient_info_2 = '
                  <script>
                    refresh_vcl_func%stage_id% = function(data)
                    {	if (data.status=="ok")
                      {	
                        $("#varpwd%stage_id%").text(data.varpwd);
                        setTimeout(refresh_vcl%stage_id%,data.delay*1000);
                    //alert(data.delay);
                      }
                      else
                      {	setTimeout(refresh_vcl%stage_id%,300000);
                    //alert("error");
                      }
                    }
                    $(function() { refresh_vcl%stage_id%(); });
                  </script>
                  <table id="stage_info%stage_id%"><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>
                ';

				$info_mobile = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl_cabient_info_2);
                break;
            case 4:
                $stage .= ', творческий конкурс в заочной форме';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . 'tk.' . $var_file_ext;
                check_pwd_param($row['stage_id'], $varpwd, $delay);
                $info = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl['cabinet']['info_4']);
                // mobile ~ok.

                $tpl_cabient_info_4 = '
                  <script>
                  refresh_vcl_func%stage_id% = function(data)
                  {	if (data.status=="ok")
                    {	
                      $("#varpwd%stage_id%").text(data.varpwd);
                      setTimeout(refresh_vcl%stage_id%,data.delay*1000);
                  //alert(data.delay);
                    }
                    else
                    {	setTimeout(refresh_vcl%stage_id%,300000);
                  //alert("error");
                    }
                  }
                  $(function() { refresh_vcl%stage_id%(); });
                </script>
                <table id="stage_info%stage_id%"><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>
                ';
				
				$info_mobile = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl_cabient_info_4);

                break;
            case 6:
                $stage .= ' в очной форме с применением дистанционных технологий';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . ($row['stage'] == 2 ? 'l' : '') . '.' . $var_file_ext;
                check_vcl_param($row['stage_id'], $vclink, $varpwd, $delay);
                $info = str_replace(array('%stage_id%','%vclink%','%varpwd%','%delay%'), array($row['stage_id'],$vclink,$varpwd,$delay), $tpl['cabinet']['info_6']);
                // mobile ~ok.
                
                $tpl_cabinet_info_6 = '
                <script>
                  refresh_vcl_func%stage_id% = function(data)
                  {	if (data.status=="ok")
                    {	$("#vclink%stage_id%").text(data.vclink);
                      //$("#varpwd%stage_id%").text(data.varpwd);
                      setTimeout(refresh_vcl%stage_id%,data.delay*1000);
                  //alert(data.delay);
                    }
                    else
                    {	setTimeout(refresh_vcl%stage_id%,300000);
                  //alert("error");
                    }
                  }
                  function refresh_vcl%stage_id%()
                  {	$.getJSON("/ajax/refresh_vcl.php", {stageid:%stage_id%}, refresh_vcl_func%stage_id%); 
                    //alert("ok-1");
                  }
                  $(function() { refresh_vcl%stage_id%(); });
                </script>
                <table id="stage_info%stage_id%"><tr><td>Ссылка для подключения к видеоконференции:</td><td id="vclink%stage_id%">%vclink%</td></tr><tr><td>Пароль на архив с вариантом:</td><td id="varpwd%stage_id%">%varpwd%</td></tr></table>
                ';
				
                $info_mobile = str_replace(array('%stage_id%','%varpwd%','%delay%'), array($row['stage_id'],$varpwd,$delay), $tpl_cabient_info_6);
                break;
            default:
                $stage .= ' в заочной форме';
                $filename = $var_dir . '/' . $var_file_prefix . '_' . $row['exam_abbr'] . '.' . $var_file_ext;
        }

        $content1 .= str_replace(
            array('%stage_id%','%exam_name%','%action%','%stage%','%info%'),
            array($row['stage_id'],$row['exam_name'],$filename,$stage,$info),
            $tpl['cabinet_dist_stage']['get_var_ref']
        );
		
		$tpl_get_var_ref = '
		<div class="ui-state-default ui-corner-all" style="padding: 0 .7em;"> 
		<p><img src="/img/idx_ban_books.gif" style="float: left; margin-left: 1em;">
		<div class="title1">Вариант задания по предмету %exam_name%</div>
		<div class="title2">%stage%</div>
		<p>Ваш персональный вариант задания по предмету %exam_name% сформирован в формате PDF и помещен в архив формата RAR, защищенный паролем. Вы можете скачать архив, нажав кнопку  "Получить вариант" ниже. Рекомендуем Вам сделать это заблаговременно, не откладывая на последний день перед началом этапа.</p>
		<br>
		<form action="%action%" target="_blank" method="post" >
			<input type="hidden" name="stage_id" value="%stage_id%">
			<div style="text-align:right; margin-right:20px;">
			<input type="submit" name="btnGetVar%stage_id%" id="btnGetVar%stage_id%" value="Получить вариант" class="ui-button ui-button-text-only ui-widget ui-state-default ui-corner-all"></div>
		</form>
		<div style="padding:10px; width:100%">%info% %varpwd%</div></div><br>
		';

        $strRespond['get_var_ref'] .= str_replace(
            array('%stage_id%','%exam_name%','%action%','%stage%','%info%'),
            array($row['stage_id'],$row['exam_name'],$filename,$stage,$info_mobile),
            $tpl_get_var_ref
        );

		$content1 .= str_replace(
            array('%stage_id%','%exam_name%','%action%','%stage%','%info%'),
            array($row['stage_id'],$row['exam_name'],$filename,$stage,$info),
            $tpl['cabinet_dist_stage']['get_var_ref']
        );
    }


// Бланки

    $content1 .= $tpl['cabinet_dist_stage']['get_blanks_ref'];

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
        $tpl['cabinet_dist_stage']['main']
    );

    return $strRespond;
}
