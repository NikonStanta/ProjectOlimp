<?php
// Почтовая форма 2014-07-03, 2021-07-20

function build_page($pgid,$subtype)
{	
		global $mysqli, $SITE_VARS,$tpl,$PAGE,$SITE_DIVEMAIL;
		
		$res=$mysqli->query('select f.id,\'f.mail_list\',f.usedb,f.sql_insert,
							f.tplname,\'f.mail_from\',f.mail_header from site_pages as p
 							left join site_forms as f on p.page_subtp=f.id where p.id='.$PAGE['id']);

if (!$row=$res->fetch_row()) return '';

$fid=$row[0]+0;
$maillist=explode(';',$row[1]);
$usedb=($row[2]+0)>0;
$sql_insert=$row[3];
$tpl_name=$row[4];
$mail_from=$row[5];
$mail_header=$row[6];

$res=$mysqli->query('select * from site_form_fields where fid='.$fid);

while ($row=$res->fetch_assoc())
{	
	$vars['name'][$row['name']]=$row['name'];
	$vars['tp'][$row['name']]=(int)$row['tp']+0;
	$vars['req'][$row['name']]=(int)$row['req']+0;
	$vars['value'][$row['name']]=$row['value'];
	$vars['max'][$row['name']]=(int)$row['max']+0;
	$vars['min'][$row['name']]=(int)$row['min']+0;
	$vars['error'][$row['name']]=$row['error'];
	$vars['options'][$row['name']]=$row['options'];
	$vars['sql_options'][$row['name']]=$row['sql_options'];
	$vars['pattern'][$row['name']]=$row['pattern'];
	$vars['tpl'][$row['name']]='/%'.$row['name'].'%/u';
	/* tp -
		0: string/text no tags
		1: string/text use tags
		2: number
		3: select
		4: checkbox
		req -
		0: none
		1: check
		2: required
	*/
}

if ($subtype==2 && $_SESSION['debug'])
{	print_r($_POST); echo '<hr>'; //print_r($vars);

}	

$cmd='form';
if (!empty($_POST['btnCheck'])) $cmd='check';
if (!empty($_POST['btnSend'])) $cmd='send';
$done=false;
$error='';

// Значения полей
if (is_array($vars['name']))
{
foreach ($vars['name'] as $k)
{	$w='';
	switch ($vars['tp'][$k])
	{
	case 1: 
		{	$w=trim(stripslashes($_POST[$k]));
			if ($w) $vars['value'][$k]=$w;
			break;
		}
	case 2: 
		{	if (!empty($_POST[$k])) 
			{	$vars['value'][$k]=(int)$_POST[$k];	}
			break;
		}
	case 4: 
		{	if (!empty($_POST[$k])) 
			{	$vars['value'][$k]= $_POST[$k];		
				$vars['value'][$k] = mysqli_real_escape_string($mysqli, implode(',', $vars['value'][$k]));	
			}
			break;
		}
	default:
		{	$w=trim(strip_tags(stripslashes($_POST[$k])));
			if ($w) $vars['value'][$k]=$w;
		}
	}
	// Проверка правильности заполнения полей
	
	if ($_SESSION['debug'])
	{	echo '<hr>'.$k.'<br>';
		echo $vars['value'][$k].'<br>';
		echo $vars['pattern'][$k].'<br>';
		if ($vars['req'][$k]>0)
		{	$w1=preg_match('/'.$vars['pattern'][$k].'/u',$vars['value'][$k],$w2);
			echo $vars['req'][$k].'<br>'.$w1.' matches:'.(is_array($w2) && array_key_exists(0,$w2) ? $w2[0] : ' no matches ').'<br>';
		}
	}
	
	if (($vars['req'][$k]>0)&&($vars['tp'][$k]<3))
	{	if (!preg_match('/'.$vars['pattern'][$k].'/u',$vars['value'][$k]))
		{	$error=$error.($error ? '; ' : '').$vars['error'][$k];
		}
	}
	if (($vars['req'][$k]>0)&&($vars['tp'][$k]==2)&&($vars['max'][$k]!=$vars['min'][$k])&&
			(($vars['value'][$k]>$vars['max'][$k])||($vars['value'][$k]<$vars['min'][$k])))
	{	$error=$error.($error ? '; ' : '').$vars['error'][$k];
	}
	if (($vars['req'][$k]==2) && empty($vars['value'][$k]))
	{	$error=$error.($error ? '; ' : '').$vars['error'][$k];
	}
	
	
	if ($_SESSION['debug'])
	{	echo $error.'<br>';
	}
}

// Подготовка полей  - списков

$select_options='';
$select_value_text='';
$select_tpls='';
foreach ($vars['name'] as $k)
{	if ($vars['tp'][$k]==3) // 'select' field
	{	$w=explode(';;',$vars['options'][$k]);
		$select_options[$k]='';
		$select_tpls[$k]='/%'.$k.'_options%/u';
		$vars['tpl'][$k.'_text']='/%'.$k.'_text%/u';
		$w5=true;
		foreach ($w as $w2)
		{	$w3=explode('::',$w2);
			$w4=trim($w3[1] ? $w3[1] : $w3[0]);
			if ($vars['value'][$k]==$w4)
			{	$vars['value'][$k.'_text']=$w3[0];
				$select_options[$k].='<option '.($w3[1] ? 'value="'.$w3[1].'" ' : '').
				 'selected>'.$w3[0].'</option>';
			}
			else
			{	$select_options[$k].='<option '.($w3[1] ? 'value="'.$w3[1].'" ' : '').'>'.$w3[0].'</option>';
			}
		}
	}
}

ksort($vars['value']);
ksort($vars['tpl']);

}
else
{	$vars['tpl']['_nofields_']='/%_nofields_%/u';
	$vars['value']['_nofields_']='';
}

if ($cmd=='form') $error='';

if (!empty($error)) { $cmd='form';}
	
// Построение и отправка данных формы

require ('phpmailer/PHPMailer.php');
require ('phpmailer/SMTP.php');
require ('phpmailer/Exception.php');

//if ($cmd=='send') {

//Create an instance; passing `true` enables exceptions
//$mail = new PHPMailer\PHPMailer\PHPMailer(true);

//try {
    //Server settings
//    $mail->SMTPDebug =  \PHPMailer\PHPMailer\SMTP::DEBUG_SERVER;                      //Enable verbose debug output
//    $mail->isSMTP();                                            //Send using SMTP
//    $mail->Host       = 'smtp.yandex.ru';                     //Set the SMTP server to send through
//    $mail->SMTPAuth   = true;                                   //Enable SMTP authentication
//    $mail->Username   = 'hikaru.nya@yandex.ru';                     //SMTP username
//    $mail->Password   = 'cwrdcmekzhpjsmnu';                               //SMTP password
//    $mail->SMTPSecure = \PHPMailer\PHPMailer\PHPMailer::ENCRYPTION_SMTPS;            //Enable implicit TLS encryption
//    $mail->Port       = 465;                                    //TCP port to connect to; use 587 if you have set `SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS`

    // Настройки вашей почты
//    $mail->Host       = 'smtp.yandex.ru'; // SMTP сервера вашей почты
//    $mail->Username   = 'hikaru.nya'; // Логин на почте
//    $mail->Password   = 'cwrdcmekzhpjsmnu'; // Пароль на почте
//    $mail->SMTPSecure = 'ssl';
//    $mail->Port       = 465;
//    $mail->setFrom('hikaru.nya@yandex.ru', 'AAAAAAA'); // Адрес самой почты и имя отправителя

    //Recipients
//    $mail->setFrom('hikaru.nya@yandex.ru', 'Mailer');
//    $mail->addAddress('hikaru.nya@yandex.ru', 'Joe User');     //Add a recipient
 //   $mail->addReplyTo('info@example.com', 'Information');
//    $mail->addCC('cc@example.com');
//    $mail->addBCC('bcc@example.com');

    //Attachments
//$mail->addAttachment('/var/tmp/file.tar.gz');         //Add attachments
//  $mail->addAttachment('/tmp/image.jpg', 'new.jpg');    //Optional name

    //Content
//    $mail->isHTML(true);                                  //Set email format to HTML
//    $mail->Subject = 'Here is the subject';
//    $mail->Body    = 'This is the HTML message body <b>in bold!</b>';
//    $mail->AltBody = 'This is the body in plain text for non-HTML mail clients';

//    $mail->send();
//    echo 'Message has been sent';
//} catch (Exception $e) {
//    echo "Message could not be sent. Mailer Error: {$mail->ErrorInfo}";
//}*/

//}

// проверка, что ошибки нет
//if ($cmd=='send')
//{
//if (!error_get_last()) {

    // Переменные, которые отправляет пользователь
   // $name = $_POST['email'] ;
   // $email = $_POST['email'];
   // $text = $_POST['text'];
   // $file = $_FILES['myfile'];
    
    
    // Формирование самого письма
    //$title = "Заголовок письма";
    //$body = "
    //<h2>Новое письмо</h2>
    //<b>Имя:</b> dfsf<br>
    //<b>Почта:</b> dsfasf<br><br>
    //<b>Сообщение:</b><br>asjdfn;askdnfdksfadf
    //";
    
    // Настройки PHPMailer
    //$mail = new PHPMailer(true);
    
    //$mail->isSMTP();   
    //$mail->CharSet = "UTF-8";
    //$mail->SMTPAuth   = true;
    //$mail->SMTPDebug = 2;
    //$mail->Debugoutput = function($str, $level) {$GLOBALS['data']['debug'][] = $str;};
    
    // Настройки вашей почты
    //$mail->Host       = 'smtp.yandex.ru'; // SMTP сервера вашей почты
    //$mail->Username   = 'hikaru.nya'; // Логин на почте
    //$mail->Password   = 'cwrdcmekzhpjsmnu'; // Пароль на почте
    //$mail->SMTPSecure = 'ssl';
    //$mail->Port       = 465;
    //$mail->setFrom('hikaru.nya@yandex.ru', 'AAAAAAA'); // Адрес самой почты и имя отправителя
    
    // Получатель письма
    //$mail->addAddress('skonyaeva@list.ru');  

    // Отправка сообщения
    //$mail->isHTML(true);
    //$mail->Subject = $title;
    //$mail->Body = $body;    
    
    // Проверяем отправленность сообщения
    //if ($mail->send()) {
    //    $data['result'] = "success";
    //    $data['info'] = "Сообщение успешно отправлено!";
    //} else {
    //    $data['result'] = "error";
    //    $data['info'] = "Сообщение не было отправлено. Ошибка при отправке письма";
    //    $data['desc'] = "Причина ошибки: {$mail->ErrorInfo}";
    //}
    
//} else {
//    $data['result'] = "error";
//    $data['info'] = "В коде присутствует ошибка";
//    $data['desc'] = error_get_last();
//}

//}

// Отправка результата
//header('Content-Type: application/json');
//echo json_encode($data);









//if ($cmd=='send')
//{
//	$mail_body='вавава';//preg_replace($vars['tpl'],$vars['value'],$tpl[$tpl_name]['mail']);
//	$headers ="MIME-Version: 1.0\r\nX-Priority: 1\r\nX-MSMail-Priority: High\r\n";
//	$headers.="Content-type: text/html; charset=utf-8\r\n";
//	$headers.="From: ".$mail_from."\r\n";
//	$subject='=?utf-8?B?'.chunk_split(base64_encode($mail_header)).'=?=';
		

// ####### SEND WITH PHP_MAILER

//error_reporting(E_STRICT);


//date_default_timezone_set('Europe/Moscow');


//require_once('phpmailer/class.phpmailer.php');
//require_once('phpmailer/class.smtp.php');

//require_once('phpmailer/src/PHPMailer.php');


//$mail             = new PHPMailer\PHPMailer\PHPMailer(true);

//$mail->ClearAddresses();
//$mail->IsSMTP(); // telling the class to use SMTP
//$mail->Host       = $SITE_VARS['SMTP_HOST']; // SMTP server
//$mail->SMTPDebug  = 2;                     // enables SMTP debug information (for testing)
                                           // 1 = errors and messages
                                           // 2 = messages only
//$mail->SMTPAuth   = true;                  // enable SMTP authentication
//;
//$mail->Port       = array_key_exists('SMTP_PORT',$SITE_VARS) && (int)$SITE_VARS['SMTP_PORT']>0 ? (int)$SITE_VARS['SMTP_PORT'] : 25;                   // set the SMTP port for the MAIL server
//$mail->Username   = $SITE_VARS['SMTP_USER'];  // MAIL username
//$mail->Password   = $SITE_VARS['SMTP_PASSWORD'];     // MAIL password
//$mail->SetFrom($mail_from, $SITE_VARS['SMTP_FROM']);
//$mail->AddReplyTo($mail_from); //,$tpl[$tpl_name]['alt_from']
//$mail->Subject    = $mail_header;
//$mail->AltBody    = "To view the message, please use an HTML compatible email viewer!"; // optional, comment out and test
//$mail->CharSet = "utf-8";
//$mail->MsgHTML($mail_body);

//foreach ($maillist as $mail_to)
//{	$mail->AddAddress(trim($mail_to), "");
//}					
//$cmd='sended';
		
//if ($mail->Send())
//{	$cmd='sended';	}
//else
//{	$cmd='notsended';
//}


//$mail->ClearAddresses();

//}	
	
// Сохранение данных в БД
if ($cmd=='send' || $cmd=='check'){
if ($sql_insert)
{	
//$mysqli_crm = mysqli_connect('promo.pk-mpei.ru', 'p638056_laravel_user', 'mV1cI8rB1n', 'p638056_laravel_app');

// Проверьте успешность подключения
//if (!$mysqli_crm) {
//    die("Ошибка подключения: " . mysqli_connect_error());
//}

// Устанавливаем кодировку соединения
//$mysqli_crm->set_charset('utf8mb4');

$query = preg_replace($vars['tpl'], $vars['value'], $sql_insert);

echo ($query);

// Выполнение запросов
$mysqli->query($query);
//$mysqli_crm->query($query);

if ($subtype == 2 && $_SESSION['debug']) echo '<hr>' . $query . '<hr>';

// Закрытие соединения
//$mysqli_crm->close();

}

}

// Построение тела страницы - форма

switch ($cmd)
{
case 'notsended':	// Сообщение об отправке
	{	$body=preg_replace($vars['tpl'],$vars['value'],$tpl[$tpl_name]['notsended']);
		break;
	}
case 'sended':	// Сообщение об отправке
	{	$body=preg_replace($vars['tpl'],$vars['value'],$tpl[$tpl_name]['sended']);
		break;
	}
case 'check':	// Проверка/подтверждение
	{	//$body=preg_replace($vars['tpl'],$vars['value'],$tpl[$tpl_name]['check']);
		$body = '<h1>Спасибо за регистрацию! <br>Данные успешно отправлены</h1> <br>
		<a href="https://dev.ehope.ru/index.html">Переход на главную страницу</a>';
		$_POST = array();
		break;
	}
default:
	{	// Форма резюме
		$body=preg_replace($vars['tpl'],$vars['value'],$tpl[$tpl_name]['form']);
		if (is_array($select_tpls)) $body=preg_replace($select_tpls,$select_options,$body);
		$body=preg_replace(array('/%error%/u'),array($error),$body);
	}
}

return $body;

}
?>