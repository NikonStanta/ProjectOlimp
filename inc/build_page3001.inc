<?php
// Восстановление регистрации

include_once('../inc/common.inc'); 

function build_page($pgid,$subtype)
{    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl;

if (isset($_POST['recStep'])&&($_POST['recStep']>0)&&($_POST['recStep']<5))  {$recStep=$_POST['recStep'];} else {$recStep=0;}

if (empty($_POST['hash']) || ($_POST['hash']!=$_SESSION['hash'])) {$recStep=0;}

$error='';
$dts='';
$nics='';

$sname=ucfirst_r(trim(strip_tags($_POST['recSurename'])));
$name=ucfirst_r(trim(strip_tags($_POST['recName'])));
$pname=ucfirst_r(trim(strip_tags($_POST['recPatronymic'])));
$birthday=trim(strip_tags($_POST['recBirthday']));

if ($recStep==0)
{    $_SESSION['recSurename']='';
    $_SESSION['recName']='';
    $_SESSION['recPatronymic']='';
    $_SESSION['recAnswer']='';
    $_SESSION['recQuestion']='';
    $_SESSION['recBirthday']='';
    $_SESSION['rec_nic']='';
    $_SESSION['hash']='';
    $_SESSION['just_rec_fio']='';
    $_SESSION['just_rec_nic']='';
    $_SESSION['just_rec_pwd']='';
}
else
{
    if (empty($sname)) 
    {    $sname=isset($_SESSION['recSurename']) ? $_SESSION['recSurename'] : ''; } 
    else 
    {    $_SESSION['recSurename']=$sname;    }

    
    if (empty($name)) { $name=isset($_SESSION['recName']) ? $_SESSION['recName'] : ''; } else { $_SESSION['recName']=$name;}

    
    if (empty($pname)) { $pname=isset($_SESSION['recPatronymic']) ? $_SESSION['recPatronymic'] : ''; } else { $_SESSION['recPatronymic']=$pname;}

    $answer=trim(strip_tags($_POST['recAnswer']));
    if (empty($answer)) { $answer=isset($_SESSION['recAnswer']) ? $_SESSION['recAnswer'] : ''; } else { $_SESSION['recAnswer']=$answer;}

    $doc_ser=trim(strip_tags($_POST['recDocSer']));     //are not used in build_page3001.inc
    $doc_num=trim(strip_tags($_POST['recDocNum']));

    
    if (empty($birthday)) { $birthday=isset($_SESSION['recBirthday']) ? $_SESSION['recBirthday'] : ''; } 
    else 
    { $_SESSION['recBirthday']=$birthday;}
//50
    $pregMatch_check_Symbols = 'абвгдеёжзийклмнсопрстуфхцчщшыьъюяэ\- ';
    if (empty($name) || empty($sname)) 
    {    $error='Не введены фамилия и имя участника.';
        $recStep=0;
    }
    elseif (preg_match("/[^$pregMatch_check_Symbols]/",mb_strtolower($sname)) || preg_match("/[^$pregMatch_check_Symbols]/",mb_strtolower($name)) || preg_match("/[^$pregMatch_check_Symbols]/",mb_strtolower($pname)))
    {    $error='Недопустимые символы в фамилии, имени или отчестве участника.';
        $recStep=0;
    }    
    if (($recStep>3) && empty($answer)) 
    {    $error.=($error ? ' ': '').'Необходимо ввести текст ответа.';
        $recStep=0;
    }
    if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/',$birthday,$dts)) 
    {    $error.=($error ? ' ': '').'Неверный формат или отсутствует дата рождения.';
        $recStep=0;
    }

    if (($recStep>0) && ($_SESSION['just_rec_fio']==($sname.' '.$name.' '.$pname))) // Защита от Refresh
    {    $recStep=5;
    }
    else
    {    $_SESSION['just_rec_fio']='';
        $_SESSION['just_rec_nic']='';
        $_SESSION['just_rec_pwd']='';
    }

}

// Защита от Refresh

switch($recStep)
{
case 1:  // Получим nic. Если  несколько, перейдем к формированию списка
    {    $res=$mysqli->query('select u.id,u.nic from olimp_persons as p left join olimp_users as u on (u.id=p.uid) where (p.surename="'.$mysqli->real_escape_string($sname).'") AND  (p.name="'.$mysqli->real_escape_string($name).'") AND (p.patronymic="'.$mysqli->real_escape_string($pname).
                    '") AND (p.birthday="'.$dts[3].'-'.$dts[2].'-'.$dts[1].'")' );
        $nic_count=$res->num_rows;
        if ($nic_count>1)
        {    while ($row=$res->fetch_row())    $nics[$row[0]]=$row[1];
            $recStep=2;
            break;
        }
        elseif ($nic_count==1)
        {    $row=$res->fetch_row();    
            $_SESSION['rec_nic']=$row[1];
            $recStep=3;
        }
        else
        {    $recStep=0;
            $error='Пользователь '.$sname.' '.$name.' '.$pname.' (дата рождения '.$birthday.') не зарегистрирован! ';  
            break;
        }
    }
case 3:  // Проверяем наличие паспорта и получаем вопрос
    {    if (!empty($_POST['nic'])) $_SESSION['rec_nic']=$_POST['nic'];

        $res=$mysqli->query('select p.doc_ident, u.question from olimp_persons as p left join olimp_users as u on (u.id=p.uid) where (p.surename="'.$mysqli->real_escape_string($sname).'") AND  (p.name="'.$mysqli->real_escape_string($name).'") AND (p.patronymic="'.$mysqli->real_escape_string($pname).
                    '") AND (p.birthday="'.$dts[3].'-'.$dts[2].'-'.$dts[1].'") AND (u.nic="'.$mysqli->real_escape_string($_SESSION['rec_nic']).'") ' );
        
        if ($row=$res->fetch_row())
        {    $question=$row[1];
            $passport=($row[0]>0);
        }
        else
        {    $recStep=0;
            $error='Пользователь '.$sname.' '.$name.' '.$pname.' ('.$_SESSION['rec_nic'].',  дата рождения '.$birthday.') не зарегистрирован! ';  
        }
        break;
    }
case 4:  // Проверяем всё
    {    

$res=$mysqli->query('select p.doc_ident from olimp_persons as p left join olimp_users as u on (u.id=p.uid) where (p.surename="'.$mysqli->real_escape_string($sname).'") AND  (p.name="'.$mysqli->real_escape_string($name).'") AND (p.patronymic="'.$mysqli->real_escape_string($pname).
                    '") AND (p.birthday="'.$dts[3].'-'.$dts[2].'-'.$dts[1].'") AND (u.nic="'.$mysqli->real_escape_string($_SESSION['rec_nic']).'") AND (u.answer="'.$mysqli->real_escape_string($_SESSION['recAnswer']).'") ' );
        if ($row=$res->fetch_row())
        {    if ($row[0]>0)
            {    $res2=$mysqli->query('select doc_ser,doc_num from olimp_docs_ic where id='.$row[0]); 
                $row2=$res2->fetch_row();
if ($_SESSION['debug']) {  print_r($_POST); echo '<hr>'; print_r($row2); echo '<hr>'; }
                if (($row2[0]!=$_POST['recDocSer'])||($row2[1]!=$_POST['recDocNum'])) 
                {    $recStep=0;
                    $error='Неверные серия и номер документа, подтверждающего личность и гражданство'; 
                    break;
                }
            }

            $pwd=gen_pwd(8);

            if ($mysqli->query('update olimp_users set pwd=UNHEX(SHA("'.$pwd.'")) where nic="'.$_SESSION['rec_nic'].'"'))
            {    $recStep=5;
                $_SESSION['just_rec_nic']=$_SESSION['rec_nic'];
                $_SESSION['just_rec_pwd']=$pwd;
                $_SESSION['just_rec_fio']=$sname.' '.$name.' '.$pname;
            }
            else
            {    $error="Непредвиденная ошибка при регистрации пользователя (Сервис временно недоступен), попробуйте попытку позже";
                $recStep=0;
                $_SESSION['just_reс_nic']='';
                $_SESSION['just_reс_pwd']='';
                $_SESSION['just_reс_fio']='';
                $_SESSION['hash']='';
            }

        }
        else
        {    $recStep=0;
            $error='Неверный ответ на секретный вопрос';  
        }

    }

}

// Вывод

switch ($recStep)
{
case 2: //  Выбор логина
    {    $nic_list=''; 
        if (!empty($error)) $error=preg_replace('/%error%/',$error,$tpl['rec_user']['error']);
        foreach ($nics as $w) $nic_list.=preg_replace('/%nic%/',$w,$tpl['rec_user']['nic']);
        $body=preg_replace(
            array('/%recSurename%/','/%recName%/','/%recPatronymic%/','/%recBirthday%/','/%recAnswer%/','/%nic_list%/','/%hash%/'),
            array($sname,$name,$pname,$birthday,$answer,$nic_list,$_SESSION['hash']),
            $tpl['rec_user']['step2']);
        $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX']=preg_replace(
            array('/%recSurename%/','/%recName%/','/%recPatronymic%/','/%recBirthday%/'),
            array($sname,$name,$pname,$birthday),
            $tpl['rec_user']['box2']);
        break;
    }
case 3: //  Форма ввода ответа и реквизитов документа
    {    if (!empty($error)) $error=preg_replace('/%error%/',$error,$tpl['rec_user']['error']);

        $body=preg_replace(
            array('/%recQuestion%/','/%reсAnswer%/','/%nic_list%/','/%hash%/'),
            array($question,$answer,$nic_list,$_SESSION['hash']),
            $passport ? $tpl['rec_user']['step3'] : preg_replace('%<passport>.*?</passport>%s','',$tpl['rec_user']['step3']));

        $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX']=$tpl['rec_user']['box3'];
        break;
    }

case 5: //  Результат регистрации
    {    $body=preg_replace(
            array('/%fio%/','/%nic%/','/%pwd%/'),
            array($_SESSION['just_rec_fio'],$_SESSION['just_rec_nic'],$_SESSION['just_rec_pwd']),
            $tpl['rec_user']['step4']);

        $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX']=$tpl['rec_user']['box4'];
        break;
    }

default: // Форма первичных данных
    {    if (!empty($error)) $error=preg_replace('/%error%/',preg_replace(array('/%title%/','/%text%/'),array('Ошибка!',$error),$tpl['common']['error']),$tpl['rec_user']['error']);
        if (empty($_SESSION['hash'])) $_SESSION['hash']=md5(microtime());
        $body=preg_replace(
            array('/%recSurename%/','/%recName%/','/%recPatronymic%/','/%recBirthday%/','/%recAnswer%/','/%error%/','/%hash%/'),
            array($sname,$name,$pname,$birthday,$answer,$error,$_SESSION['hash']),
            $tpl['rec_user']['step1']);
        $_PAGE_VAR_VALUES['PAGE_FIELD_UPPER_BOX']=$tpl['rec_user']['box1'];
    }


}

return $body;
}

?>