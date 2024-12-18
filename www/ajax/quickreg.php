<?php
//mysql_connect('localhost',"webscript","tx8A63l_c0SXe") or die('Can\'t connect MySQL');        CHANGED TO LOWER
$mysql = new mysqli('localhost',"ehope_sql","Vk@FJHCd-y(IyJ)7M6rQ", 'ehope') or die('Can\'t connect DataBase'); //added  ", 'ehope'"  to  $mysql = new mysqli
//$result = $mysql->query('ehope');
//mysql_set_charset  ('cp1251');        -- Добавлением charset в строку соединения, например charset=utf8
$mysql->set_charset('utf8');
if (isset($_REQUEST['btnAdd']))
{
    /*
	if (mysql_query('insert into olimp_users set '.
	'nic=trim("'.$_REQUEST['nic'].'"), '.
	'pwd=UNHEX(SHA(trim("'.$_REQUEST['pwd'].'"))), '.
	'question="", answer="", role=1, dt=Now()'))
    */
    if ($mysql->query('insert into olimp_users set '.	'nic=trim("'.$_REQUEST['nic'].'"), '.	'pwd=UNHEX(SHA(trim("'.$_REQUEST['pwd'].'"))), '.	'question="", answer="", role=1, dt=Now()'))    // Trim(string) - удаляет
        //символы пробела из начала и конца строки string
	{
        //$uid=mysql_insert_id();
        $uid = $mysql->insert_id;         // Not sure. Should check
		if ($mysql->query('insert into olimp_persons set '.
		'surename="'.$_REQUEST['sname'].'", '.
		'name="'.$_REQUEST['name'].'", '.
		'patronymic="'.$_REQUEST['pname'].'", '.
		'birthday="'.$_REQUEST['bd'].'", '.
		'reg_num="'.$_REQUEST['regnum'].'", '.
		'uid='.$uid))
		{	echo 'Добавлен участник '.$_REQUEST['nic'].'<br><br><a href="?">Следующий...</a>';
		}
		else
		{	echo 'ОШИБКА! <a href="?">Следующий...</a>';
		}

	}
	else
	{	echo 'ОШИБКА! <a href="?">Следующий...</a>';
	}

}
else
{	echo ' 
<style>table{border:0;} td{padding:4px;}</style>
<table style="cellpadding:8px; ">
<form method="post">
<tr><td>Фамилия:</td><td><input type=text name=sname size=30></td></tr>
<tr><td>Имя:</td><td><input type="text" name="name" size=30></td></tr>
<tr><td>Отчество:</td><td><input type="text" name="pname" size=30></td></tr>
<tr><td>Дата рождения:</td><td><input type="text" name="bd" size=30> в формате ГГГГ-ММ-ДД</td></tr>
<tr><td>Рег.номер:</td><td><input type="text" name="regnum" size=30></td></tr>
<tr><td>Логин:</td><td><input type="text" name="nic" size=30></td></tr>
<tr><td>Пароль:</td><td><input type="text" name="pwd" size=30></td></tr>
</table>
<br><br>
<input name="btnAdd" type="submit" value="Добавить">
<br><br>
';
}
?>