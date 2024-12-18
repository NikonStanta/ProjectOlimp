<?php

function check_address($aid)
{   global $mysqli;
    $res=$mysqli->query('select zip_code,country,region,street,house,addr_text from olimp_address where id='.$aid);
	$row=$res->fetch_assoc();
	return cabinet_address_check($row,'');
}

function check_school($id)
{   global $mysqli;
    $res=$mysqli->query('select school_type, school_type_ex, name, address from olimp_schools where id='.$id);
	$row=$res->fetch_assoc();

$a=trim($row['school_type_ex']);
$b=$row['school_type']<1;
$c=trim($row['name']);
$d=($row['address']<0);

	if ((empty($a) && $b) || (empty($c)) || $d) 
	{	return 'Ошибка в имени или не введен адрес';
	}
	return check_address($row['address']);
}

function check_doc_ic($did)
{   global $mysqli;
    $res=$mysqli->query('select doc_type, doc_ser, doc_num, DATE_FORMAT(doc_date,"%d.%m.%Y") as doc_date, doc_where, DATE_FORMAT(doc_period,"%d.%m.%Y") as doc_period from olimp_docs_ic where id='.$did);

	$row=$res->fetch_assoc();
	if ($row['doc_type']<1)
	{	return 'Не задан тип документа';
	}
	$doc_type=$row['doc_type'];
	$doc_ser=$row['doc_ser'];
	$doc_num=$row['doc_num'];
	$doc_date=$row['doc_date'];
	$doc_where=$row['doc_where'];
	$doc_period=$row['doc_period'];
	$error='';
	$res=$mysqli->query('select req_fields,pattern_ser,pattern_num from olimp_doc_types where id='.$doc_type);
	if ($row=$res->fetch_assoc())
	{	if (!empty($doc_ser))
		{	if (!preg_match('/'.$row['pattern_ser'].'/',$doc_ser)) 
			{	$error.='Неверный формат серии документа';
			}
		}
		else
		{	if (preg_match('/doc_ser/',$row['req_fields']))
			{	$error.='Должна быть введена серия документа';
			}
		}
		
		if (!empty($doc_num))
		{	if (!preg_match('/'.$row['pattern_num'].'/',$doc_num)) 
			{	$error.='Неверный формат номера документа';
			}
		}
		else
		{	if (preg_match('/doc_num/',$row['req_fields']))
			{	$error.='Должен быть введен номер документа';
			}
		}
	
		if (!empty($doc_date))
		{	if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/',$doc_date,$dts)) 
			{	$error.='Неверный формат даты выдачи документа';
			}
		}
		else
		{	if (preg_match('/doc_date/',$row['req_fields']))
			{	$error.='Должна быть введена дата выдачи документа';
			}
		}
	
		if (empty($doc_where))
		{	if (preg_match('/doc_where/',$row['req_fields']))
			{	$error.='Должно быть введено место выдачи документа';
			}
		}
	
		if (empty($doc_code))
		{	if (preg_match('/doc_code/',$row['req_fields']))
			{	$error.='Должен быть введен код подразделения';
			}
		}
	
		if (!empty($doc_period))
		{	if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/',$doc_period,$dts)) 
			{	$error.='Неверный формат срока действия документа';
			}
		}
		else
		{	if (preg_match('/doc_period/',$row['req_fields']))
			{	$error.='Должен быть введен срок действия документа';
			}
		}
	}
	else
	{	return 'Неверный тип документа';
	}

	return $error;
}

function check_persons_data($pid)
{   global $mysqli;
    $result['All is entered']=false;
	$result['Person confirmed']=false;
	$result['School confirmed']=false;
	$result['Permission confirmed']=false;
	$result['Pass confirmed']=false;
	$result['printed_z']='N';
	$result['Idents_confirmed']=false;
	$result['Idents_loaded']=false;
	
	$res=$mysqli->query('select p.surename as sname, p.name, DATE_FORMAT(p.birthday,"%Y%d%m") as birthday,p.addr_reg,p.addr_post,p.doc_ident,p.school,p.tel,p.email,p.citizenship,p.confirmed_person,p.confirmed_school,p.confirmed_permission,p.confirmed_pass, p.bplace as bplace, p.school_class, p.printed_z, p.IdentDoc, p.IdentPhoto, p.SchoolDoc from olimp_persons as p where id='.$pid);
	if ($row=$res->fetch_assoc())
	{	$result['Person confirmed']=$row['confirmed_person']=='Y';
		$result['School confirmed']=$row['confirmed_school']=='Y';
		$result['Permission confirmed']=$row['confirmed_permission']=='Y';
		$result['Pass confirmed']=$row['confirmed_pass']=='Y';
		
		$result['printed_z']=$row['printed_z'];
		$result['IdentDoc']=$row['IdentDoc'];
		$result['IdentPhoto']=$row['IdentPhoto'];
		$result['SchoolDoc']=$row['SchoolDoc'];
		
		$result['Idents_loaded']=
		(($result['IdentDoc']=='Accepted') || ($result['IdentDoc']=='Loaded') || ($result['IdentDoc']=='Updated')) &&
		(($result['IdentPhoto']=='Accepted') || ($result['IdentPhoto']=='Loaded') || ($result['IdentPhoto']=='Updated'));
		
		$result['Idents_confirmed']=($result['IdentDoc']=='Accepted') && ($result['IdentPhoto']=='Accepted') && ($result['SchoolDoc']=='Accepted');
		
		$all_entered=true;

$a=trim($row['sname']);
$b=trim($row['name']);
$p=trim($row['bplace']);

		if (empty($a) || empty($b)  )
		{	$result['errors'][]='Не введены фамилия и имя участника'; $all_entered=false;
		}
		
		$cdate=date('Y').'0101'; 
		if (($row['birthday']>($cdate-80000)) || ($row['birthday']<($cdate-700000)))
		{	$result['errors'][]='Не введена или ошибочная дата рождения*'; $all_entered=false;
		}

		if (empty($p))
		{	$result['errors'][]='Не введено место рождения участника'; $all_entered=false;
		}

		if (empty($row['citizenship']))
		{	$result['errors'][]='Не введено гражданство участника'; $all_entered=false;
		}

		if (empty($row['tel']))
		{	$result['errors'][]='Не введен контактный телефон'; $all_entered=false;
		}

		if ($row['doc_ident']<1)
		{	$result['errors'][]='Не введен документ, подтверждающий личность и гражданство'; $all_entered=false;
		}
		else
		{	if ($w=check_doc_ic($row['doc_ident']))
			{	$result['errors'][]='Ошибка в реквизитах документа, подтверждающего личность и гражданство'; $all_entered=false;
			}
//echo $w;
		}


		if ($row['addr_reg']<1)
		{	$result['errors'][]='Не введен адрес постоянной регистрации'; $all_entered=false;
		}
		else
		{	if (check_address($row['addr_reg']))
			{	$result['errors'][]='Ошибка в адресе постоянной регистрации'; $all_entered=false;
			}
		}


		if ($row['addr_post']>1)
		{	if (check_address($row['addr_reg']))
			{	$result['errors'][]='Ошибка в адресе временной регистрации (почтовом адресе)'; $all_entered=false;
			}
		}

		if (($row['school_class']<3) || ($row['school_class']>11))
		{	$result['errors'][]='Не введен или неверно введен класс'; $all_entered=false;
		}
		
		if ($row['school']<1)
		{	$result['errors'][]='Не введена или неверно введена информация о школе'; $all_entered=false;
		}
		else
		{	if (check_school($row['school']))
			{	$result['errors'][]='Ошибка в названии или адресе школы'; $all_entered=false;
			}
		}
		$result['All is entered']=$all_entered;

	}

	return $result;
}

function check_vcl_param($stage_id,&$vclink,&$varpwd,&$delay)
{   global $mysqli;
    global $SITE_VARS,$tpl,$USER_INFO;
	$vclink=trim($tpl['cabinet']['info_novclink_before']);
	$varpwd=trim($tpl['cabinet']['info_novarpwd_before']);
	$delay=300;

	$query='select s.form, a.audit as audit1, g.audit as audit2, a.presence, s.var_pwd as varpwd, UNIX_TIMESTAMP(s.date_exam)-UNIX_TIMESTAMP(Now()) AS tshift, a.varnum, IF(ISNULL(a.startDT),0,1) as started, IF(ISNULL(a.endDT),0,1) ended from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_groups as g ON (g.id=a.group_id) where (s.date_post>Now()) AND (a.person_id='.$USER_INFO['pid'].') AND (a.stage_id='.$stage_id.')';

	$res=$mysqli->query($query);
	if ($row=$res->fetch_assoc())
	{	// Если осталось не больше 10 минут до публикации ссылки, устанавливаем время обновления 1 мин
		if ($row['tshift']<=($SITE_VARS['vclink_advance']+10)*60)  
		{	$delay=60;
		}
		// Если до публикации ссылки больше часа, заканчиваем (ссылка и пароль = no_before)
		if ($row['tshift']>$SITE_VARS['vclink_advance']*60) 
		{	return;
		}
		// Определяем комнату участника
		$audit=($row['audit1']>0 ? $row['audit1'] : $row['audit2']);
		if ($audit<0) 
		{	$vclink.='!!! no audit';
			return;
		}
		// Получаем параметры комнаты
		$query='select IF(ISNULL(sa.startDT),0,1) as started, IF(ISNULL(sa.endDT),0,1) as ended, sa.link as link from olymp_stream_audits as sa where id='.$audit;
		
		$res2=$mysqli->query($query);
		if ($row2=$res2->fetch_assoc())
		{	// Если олимпиада завершена или участник сдал работу, прекращаем публикацию пароля и ссылки
			if ($row['ended'] || $row2['ended']) 
			{	$vclink=trim($tpl['cabinet']['info_novclink_after']);
				$varpwd=trim($tpl['cabinet']['info_novarpwd_after']);
			}
			else
			{	// Публикуем ссылку
				if ($row2['link']) $vclink=trim($row2['link']);
				// Если олимпиада начата и участник приступил к работе, публикуем пароль и  устанавливаем время обновления 5 мин
				if ($row['varpwd'] && $row2['started'] && $row['started'])	
				{	$varpwd=trim($row['varpwd']);
					$delay=300;
				}
				// иначе, если прошло плановое время начала олимпиады, устанавливаем время обновления 10 сек., чтобы участник быстро получил пароль
				else if ($row['tshift']<0)  
				{	$delay=10;
				}
				// если плановое время начала олимпиады ещё не наступило, время обновления 1 мин.
			}
		}
	}
}

function check_pwd_param($stage_id,&$varpwd,&$delay)
{   global $mysqli;
    global $SITE_VARS,$tpl,$USER_INFO;
	$varpwd=trim($tpl['cabinet']['info_novarpwd_before']);
	$delay=300;

	$query='select s.form, a.audit as audit1, g.audit as audit2, a.presence, s.var_pwd as varpwd, UNIX_TIMESTAMP(s.date_exam)-UNIX_TIMESTAMP(Now()) AS tshift, UNIX_TIMESTAMP(s.date_post)-UNIX_TIMESTAMP(Now()) AS tpost, a.varnum, IF(ISNULL(a.startDT),0,1) as started, IF(ISNULL(a.endDT),0,1) ended from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_groups as g ON (g.id=a.group_id) where (s.date_post>Now()) AND (a.person_id='.$USER_INFO['pid'].') AND (a.stage_id='.$stage_id.')';

	$res=$mysqli->query($query);
	if ($row=$res->fetch_assoc())
	{	// Если осталось не больше 10 минут до публикации пароля, устанавливаем время обновления 1 мин
		if ($row['tshift']<=10*60)  
		{	$delay=60;
		}
		// Если до публикации пароля больше часа, заканчиваем (ссылка и пароль = no_before)
		if ($row['tshift']>60*60) 
		{	return;
		}
		
			// Если олимпиада завершена или участник сдал работу, прекращаем публикацию пароля
			if ($row['tpost']<0) //добавить проверку на сдал работу
			{	
				$varpwd=trim($tpl['cabinet']['info_novarpwd_after']);
			}
			else
			{	
				// Если олимпиада начата, публикуем пароль и  устанавливаем время обновления 5 мин
				if ($row['varpwd'] && $row['tshift']<0 /* && $row2['started'] && $row['started']*/)	
				{	$varpwd=trim($row['varpwd']);
					$delay=300;
				}
				// иначе, если прошло плановое время начала олимпиады, устанавливаем время обновления 10 сек., чтобы участник быстро получил пароль
				else if ($row['tshift']<0)  
				{	$delay=10;
				}
				// если плановое время начала олимпиады ещё не наступило, время обновления 1 мин.
			}
		
	}
}
?>