<?php
// Личный кабинет. Профиль участника.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');

function build_page($pgid,$subtype)
{	global $SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO,$mysqli;

if ($USER_INFO['uid']<1) return $tpl['cabinet']['logon_form'];

if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) 
{   return $tpl['cabinet']['closed'];
}

$questions=array(
1=>'Дата рождения матери',
2=>'Дата рождения отца',
3=>'Серия и номер паспорта отца',
4=>'Серия и номер паспорта матери',
5=>'Любимый актер/актриса кино',
6=>'Любимый фильм',
7=>'Как вас называли в детстве',
8=>'Любимое блюдо');

$selected_tab=array_key_exists('selected_tab',$_REQUEST) ? trim($_REQUEST['selected_tab']) : '';
if (empty($selected_tab)) $selected_tab=0;
$step=array_key_exists('step',$_REQUEST) ? (int)$_REQUEST['step'] : 0;
if (isset($_POST['btnChangePassword']) && $step==1) $step=3;
if (isset($_POST['btnSelectSchool']) && $step==1) $step=3;
if (isset($_POST['btnBack']) && ($step==2)) $step=0;
if (isset($_POST['btnBack']) && ($selected_tab==3) && ($step==4)) $step=0;
if (isset($_POST['btnBack']) && ($selected_tab==3) && ($step==5)) $step=3; 
if (isset($_POST['btnApprove']) && ($selected_tab==2)) $step=0; 


$formdata=array_key_exists('formdata',$_REQUEST) ? (int)$_REQUEST['formdata'] : 0;

$error='';
$warning='';
$tabs='';

// Перерегистрация участников прошлого года
$message='';
include_once('../inc/reregistrate.inc');

$res=$mysqli->query('select count(stage_id) from olimp_actions as a left join olimp_stages as s on(s.id=a.stage_id) where s.year="'.$USER_INFO['year'].'"  AND person_id='.($USER_INFO['pid']+0));
$row=$res->fetch_row();
$USER_INFO['stage_regs']=$row[0];
$USER_INFO['block_edit']=(!empty($USER_INFO['stage_regs'])) || (!empty($SITE_VARS['block_edit'])) || ($USER_INFO['printed_z']=='L') || ($USER_INFO['printed_z']=='A');


if ($USER_INFO['uid']==1 || $USER_INFO['uid']==2 || $_SESSION['master_uid']>0) 
{
//echo 'регистраций='.$USER_INFO['stage_regs'];
//echo '<br>block_edit='.$USER_INFO['block_edit'];
$USER_INFO['block_edit']=0;

//print_r($USER_INFO);
}


// Действия

if ($step>1)
{
	switch ($selected_tab)
	{
	case 0:  // Изменение учетных данных
		{	if ($step==2)
			{	$question=trim($_POST['questionB']);
				if (empty($question)) $question=$_POST['questionA'];
				$answer=trim($_POST['answer']);
				if ($mysqli->query('update olimp_users set question="'.$question.'", answer="'.$answer.'" where id='.$USER_INFO['uid']))
				{	$warning='Учетные данные изменены';
				}
				else
				{	$error='Учетные данные не изменены';
				}
				$step=0;
				break;
			}
			if ($step==3)
			{	$pwd=gen_pwd(8);
				if ($mysqli->query('update olimp_users set pwd=UNHEX(SHA("'.$pwd.'")) where id='.$USER_INFO['uid']))
				{	$warning='Пароль изменен';
				}
				else
				{	$step=0;	
					$error='Пароль не изменен';
				}
				break;
			}
			break;
		}

	case 1:  // Изменение личных данных
		{	if ($USER_INFO['block_edit']) break;
                        $surename=trim($_POST['surename']);
			$name=trim($_POST['name']);
			$patronymic=trim($_POST['patronymic']);
			$birthday=trim($_POST['birthday']);
			$bplace=trim($_POST['bplace']);
			$gender=trim($_POST['gender']);
			$SNILS=trim($_POST['SNILS']);
			$citizenship=$_POST['citizenship']+0;
			$tel=trim($_POST['tel']);
			$email=trim($_POST['email']);

			//$email_agree=trim($_POST['email_agree']);
			$parent_fio=trim($_POST['parent_fio']);
			$parent_degree=trim($_POST['parent_degree']);
			$parent_tel=trim($_POST['parent_tel']);
			$parent_email=trim($_POST['parent_email']);

			$doc_type=$_POST['doc_type']+0;
			$doc_ser=trim($_POST['doc_ser']);
			$doc_num=trim($_POST['doc_num']);
			$doc_date=trim($_POST['doc_date']);
			$doc_where=trim($_POST['doc_where']);
			$doc_period=trim($_POST['doc_period']);
			$doc_code=trim($_POST['doc_code']);
						// Проверка корректности данных
						if (empty($name) || empty($surename)) 
						{	$error='Не введены фамилия и имя участника';
							break;
						}
						if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/',$birthday,$dts)) 
						{	$error='Неверный формат или отсутствует дата рождения';
							$birthday='';
							break;
						}
						if (empty($bplace)) 
						{	$error='Не введено место рождения участника';
							break;
						}
						$birthdayQ=$dts[3].'-'.$dts[2].'-'.$dts[1];
						if (($gender!='M') && ($gender!='F')) 
						{	$error='Неверно указан пол участника (нужно выбрать из списка)';
							$gender='';
							break;
						}
			/*		
						if (!preg_match('/\d{11}/', $SNILS) && $citizenship == 1)
						{	$error='Не введен номер СНИЛСа или введен в неверном формате';
							$SNILS='';
							break;
						}
			*/
			if ($citizenship==1)  // Проверки для граждан России
			{	$number = substr($SNILS, 0, 9);
				$checksum = (int)substr($SNILS, 9, 2);
				if (strlen($SNILS) != 11)
				{
    					$error.= 'СНИЛС должен состоять из 11 цифр';
					break;
				}
				else if ((int)$number > 1001998)
				{
   					$check = 0;
    					for ($i =0; $i < 9; $i++)
    					{
        					$check += $number[$i] * (9 - $i);
    					}
    					$check = $check % 101 % 100;
    					if ($check != $checksum)
    					{
        					$error= 'СНИЛС введен неверно. Проверьте ещё раз';
						break;
    					}
				}
			}

			if ($citizenship>0)
			{	$res=$mysqli->query('select id from olimp_citizenships where id='.$citizenship);
				if ($res->num_rows<1)
				{	$citizenship=0;
					$error='Неверно указано гражданство (нужно выбрать из списка)';
					break;
				}	
			}
			if (!empty($tel))
			{	if (!preg_match('/[0-9\(\)\-\+\ ]+/',$tel)) 
				{	$error='Неверный формат телефонного номера';
					break;
				}
			}
			$res=$mysqli->query('select DATE_FORMAT(p.birthday,"%d.%m.%Y") as $birthday, DATE_FORMAT(p.reg_date,"%d.%m.%Y") as reg_date, DATE_FORMAT(p.upd_date,"%d.%m.%Y") as upd_date from olimp_persons where p.uid='.$USER_INFO['uid']);
			$reg_date=($person['upd_date']=='00.00.0000' ? $person['reg_date'] : $person['upd_date']);

			if (strtotime($birthday) + 567993600 < strtotime($reg_date)){$adult=1;} else {$adult=0;}

			if ((empty($parent_fio) || empty($parent_degree) || empty($parent_tel) || empty($parent_email)) && ($adult==0)) 
			{	$error='Не введены данные родителя/законного представителя';
				break;
			}




			if ($doc_type>=0)
			{	$res=$mysqli->query('select req_fields,pattern_ser,pattern_num from olimp_doc_types where id='.$doc_type);
				if ($row=$res->fetch_assoc())
				{	if (!empty($doc_ser))
					{	if (!preg_match('/'.$row['pattern_ser'].'/',$doc_ser)) 
						{	$error='Неверный формат серии документа';
							break;
						}
					}
					else
					{	if (preg_match('/doc_ser/',$row['req_fields']))
						{	$error='Должна быть введена серия документа';
							break;
						}
					}
					
					if (!empty($doc_num))
					{	if (!preg_match('/'.$row['pattern_num'].'/',$doc_num)) 
						{	$error='Неверный формат номера документа';
							break;
						}
					}
					else
					{	if (preg_match('/doc_num/',$row['req_fields']))
						{	$error='Должен быть введен номер документа';
							break;
						}
					}

					if (empty($doc_code) && preg_match('/doc_code/',$row['req_fields']))
					{	$error='Должен быть введен номер документа';
						break;
					}
				
					if (!empty($doc_date))
					{	if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/',$doc_date,$dts)) 
						{	$error='Неверный формат даты выдачи документа';
							$doc_date='';
							break;
						}
						$doc_dateQ=$dts[3].'-'.$dts[2].'-'.$dts[1];
					}
					else
					{	if (preg_match('/doc_date/',$row['req_fields']))
						{	$error='Должна быть введена дата выдачи документа';
							break;
						}
					}
				
					if (empty($doc_where))
					{	if (preg_match('/doc_where/',$row['req_fields']))
						{	$error='Должно быть введено место выдачи документа';
							break;
						}
					}
				
					if (empty($doc_code))
					{	if (preg_match('/doc_code/',$row['req_fields']))
						{	$error='Должен быть введен код подразделения';
							break;
						}
					}
				
					if (!empty($doc_period))
					{	if (!preg_match('/(\d{1,2})\.(\d{1,2})\.(\d{2]|\d{4})/',$doc_period,$dts)) 
						{	$error='Неверный формат срока действия документа';
							$doc_period='';
							break;
						}
						$doc_periodQ=$dts[3].'-'.$dts[2].'-'.$dts[1];
					}
					else
					{	if (preg_match('/doc_period/',$row['req_fields']))
						{	$error='Должен быть введен срок действия документа';
							break;
						}
						else $doc_periodQ='0000-00-00';
					}
				
				}
				else
				{	$doc_type=0;
					$error='Неверно указан тип документа (нужно выбрать из списка)';
					break;
				}
			}
			
			
			
			$res=$mysqli->query('select id from olimp_persons where (surename like "'.$surename.'") AND (name like "'.$name.'") AND (patronymic like "'.$patronymic.'") AND (birthday="'.$birthdayQ.'")');
			$persons_count=$res->num_rows;
			if ($persons_count==1) 
			{ $row=$res->fetch_row(); } 
			
			if (($persons_count>1) || (($persons_count==1)&&($row[0]!=$USER_INFO['pid'])))
			{	$error='Изменения не внесены! Участник с именем '.$surename.' '.$name.' '.$patronymic.' (дата рождения '.$birthday.') уже зарегистрирован!';
				$step=0;
				break;
			}
			
			if ($doc_type>0)
			{	$res=$mysqli->query('select doc_ident from olimp_persons where uid='.$USER_INFO['uid']);
				if ($row=$res->fetch_row()){ $doc_ident=$row[0]; } else {$doc_ident=0;}
				if ($doc_ident>0)
				{	$mysqli->query('update olimp_docs_ic set uid='.$USER_INFO['uid'].','.
						'doc_type='.$doc_type.', '.
						'doc_ser="'.$doc_ser.'", '.
						'doc_num="'.$doc_num.'", '.
						'doc_date="'.$doc_dateQ.'", '.
						'doc_where="'.$mysqli->real_escape_string($doc_where).'", '.
						'doc_code="'.$doc_code.'", '.
						'doc_period="'.$doc_periodQ.'" '.
						'where id='.$doc_ident
					);
				}
				else
				{	$mysqli->query('insert into olimp_docs_ic set uid='.$USER_INFO['uid'].','.
						'doc_type='.$doc_type.', '.
						'doc_ser="'.$doc_ser.'", '.
						'doc_num="'.$doc_num.'", '.
						'doc_date="'.$doc_dateQ.'", '.
						'doc_where="'.$mysqli->real_escape_string($doc_where).'", '.
						'doc_code="'.$doc_code.'", '.
						'doc_period="'.$doc_periodQ.'" '
					);
					
					$doc_ident=$mysqli->insert_id;
				}
			}
			else { $doc_ident=0;}
			
			$mysqli->query('update olimp_persons set '.
				'surename="'.$surename.'", '.
				'name="'.$name.'", '.
				'patronymic="'.$patronymic.'", '.
				'birthday="'.$birthdayQ.'", '.
				'bplace="'.$bplace.'", '.
				'gender="'.$gender.'", '.
				'SNILS="'.$SNILS.'", '.
				'citizenship='.$citizenship.', '.
				'tel="'.$tel.'", '.
				'email="'.$email.'", '.


				'parent_fio="'.$parent_fio.'", '.
				'parent_degree="'.$parent_degree.'", '.
				'parent_tel="'.$parent_tel.'", '.
				'parent_email="'.$parent_email.'"'.

				($doc_ident>0 ? ', doc_ident='.$doc_ident : '').
				' where uid='.$USER_INFO['uid']
			);
		
			$step=0;
			break;
		} // case 1: Изменение личных данных
	
	
	case 2:  // Изменение адресов
		{	if ($USER_INFO['block_edit']) break;
                        $addr_reg_data=($_POST['addr_reg']);
			$addr_post_data=($_POST['addr_post']);
			$error=cabinet_address_check($addr_reg_data,'В адресе постоянной регистрации.');
			if ($error)	break;
			$error=cabinet_address_check($addr_post_data,'В почтовом адресе.',false);
			if ($error)	break;
			$res=$mysqli->query('select addr_reg, addr_post from olimp_persons where uid='.$USER_INFO['uid']);
			$row=$res->fetch_assoc();

			if ($addr_reg_data['country']>0) 
				if	(($addr_reg_id=cabinet_address_update($addr_reg_data,$row['addr_reg']))==0)
				{	$error='Непредвиденная ошибка при обновлении/добавлении адреса регистрации. Повторите попытку позже.';
					break;
				}
			if ($addr_post_data['country']>0)
			{
				if	(($addr_post_id=cabinet_address_update($addr_post_data,$row['addr_post']))==0)
				{	$error='Непредвиденная ошибка при обновлении/добавлении почтового адреса. Повторите попытку позже.';	
					break;
				}
			}
			else
			{	if	($row['addr_post']>0)
				{	$mysqli->query('update olimp_persons set addr_post=0 where uid='.$USER_INFO['uid']);
					break;
				}
			}
			
			if (($addr_reg_id>0)||($addr_post_id>0))
			{	$mysqli->query('update olimp_persons set '.
					($addr_reg_id>0  ? 'addr_reg='.$addr_reg_id : '').
					($addr_post_id>0 ? ($addr_reg_id>0 ? ', ' : '').'addr_post='.$addr_post_id : '').
					' where uid='.$USER_INFO['uid']);
			}
			$step=0;
			$_POST['addr_post'] = '';
			$_POST['addr_reg'] = '';
			break;
		}
		
	case 3:  // Изменение учебного заведения
		{	if ($USER_INFO['block_edit']) break;
                        switch ($step)
			{
			case 2:
				{	$school_class=$_POST['school_class']+0;
					$teacher_m=trim($_POST['teacher_m']);
					$teacher_f=trim($_POST['teacher_f']);
					$teacher_i=trim($_POST['teacher_i']);
					$class_m=$_POST['class_m']; if ($class_m==""){$class_m=0;}
					$class_f=$_POST['class_f']; if ($class_f==""){$class_f=0;}
					$class_i=$_POST['class_i']; if ($class_i==""){$class_i=0;}
					$class_k=$_POST['class_k']; if ($class_k==""){$class_k=0;}
					if (!empty($_POST['school_class']) && (($school_class<5) || ($school_class>11)))
					{	$error='Неверно задан класс. Должно быть число от 5 до 11.';
						$step=1;
						break;
					}
					if ( (!empty($_POST['class_m']) && (($class_m<$school_class) || ($class_m>11))) || (!empty($_POST['class_f']) && (($class_f<$school_class) || ($class_f>11))) || (!empty($_POST['class_i']) && (($class_i<$school_class) || ($class_i>11))) || (!empty($_POST['class_k']) && (($class_k<$school_class) || ($class_k>11))) )
					{	$error='Неверно задан класс по предметам. Класс должен быть больше учебного.';
						$step=1;
						break;
					}

					if ($mysqli->query('update olimp_persons set school_class='.$school_class.', '.
						'class_m='.$class_m.', '.
						'class_f='.$class_f.', '.
						'class_i='.$class_i.', '.
						'class_k='.$class_k.', '.
						'teacher_m="'.$teacher_m.'", '.
						'teacher_f="'.$teacher_f.'", '.
						'teacher_i="'.$teacher_i.'"'.
						' where uid='.$USER_INFO['uid']))
					{	$step=0;	}

					else
					{	$error='Непредвиденная ошибка при обновлении данных. Повторите попытку позже.';
						$step=1;
					}
					break;




				}
			case 5:
				{	$step=4;
					$school=$_POST['school']+0;
					if ($school>0)
					{	$res=$mysqli->query('select id from olimp_schools where id='.$school);
						if ($res->num_rows>0)
						{	if ($mysqli->query('update olimp_persons set school='.$school.' where uid='.$USER_INFO['uid']))
							{	$step=0;	}
							else
							{	$error='Непредвиденная ошибка при обновлении данных. Повторите попытку позже.';
							}
							break;
						}
					}
					$school_type=$_POST['school_type']+0;
					$school_type_ex=trim($_POST['school_type_ex']);
					$school_name=trim($_POST['school_name']);
					$director=trim($_POST['director']);
					$tel=trim($_POST['tel']);
					$email=trim($_POST['email']);
					$school_addr=$_POST['school_addr'];
					if (empty($school_type) && empty($school_type_ex))
					{	$error='Не задан тип образовательного учреждения';
						break;
					}
					if (empty($school_name))
					{	$error='Не задано наименование образовательного учреждения';
						break;
					}
					$error=cabinet_address_check($school_addr,'В адресе учебного заведения.');
					if ($error)	break;
					if	(($school_addr_id=cabinet_address_update($school_addr,0))==0)
					{	$error='Непредвиденная ошибка при обновлении/добавлении адреса образовательного учреждения. Повторите попытку позже.';	
						break;
					}
					if ($school_addr_id>0)
					{	if ($mysqli->query('insert into olimp_schools set '.
							'school_type='.$school_type.', '.
							'school_type_ex="'.$school_type_ex.'", '.
							'name="'.$school_name.'", '.
							'director="'.$director.'", '.
							'tel="'.$tel.'", '.
							'email="'.$email.'", '.
							'address='.$school_addr_id))
						{	$school=$mysqli->insert_id;
							if ($mysqli->query('update olimp_persons set school='.$school.' where uid='.$USER_INFO['uid']))
							{	$step=0;
								break;
							}
							
						}
					}
					$error='Непредвиденная ошибка при добавлении/обновлении данных. Повторите попытку позже.';
					break;
				}
			}
			break;
		}
	} // switch($selected_tab)
} // if ($step>1)

// Вывод

if ($step>0)
{
	switch ($selected_tab)
	{
	case 0: //  Форма изменения учетных данных
		{	if ($step==1)
			{	$res=$mysqli->query('select nic,question,answer from olimp_users where id='.$USER_INFO['uid']);
				$row=$res->fetch_assoc();
				$questions_list='';
				$question=$row['question'];
				$questionB=$question;
				foreach ($questions as $w)
				{	$questions_list.='<option '.($w==$question ? ' selected' : '').'>'.$w.'</option>';
					if ($w==$question) $questionB='';
				}
				$content1=preg_replace(
					array('/%nic%/','/%questions_list%/','/%questionB%/','/%answer%/'),
					array($row['nic'],$questions_list,$questionB,$row['answer']),
					$tpl['cabinet_profile']['content1_step1']);
			}
			break;
		}
	case 1: //  Форма изменения личных данных
		{	$res=$mysqli->query('select p.surename,p.name,p.patronymic,DATE_FORMAT(p.birthday,"%d.%m.%Y") as birthday,p.gender,p.SNILS,p.doc_ident,p.citizenship,p.tel,p.email,p.bplace,p.parent_fio,p.parent_degree,p.parent_tel,p.parent_email from olimp_persons as p where p.uid='.$USER_INFO['uid']);
			$row=$res->fetch_assoc();
			if ($formdata>0)
			{	if (!empty($surename)) $row['surename']=$surename;
				if (!empty($name)) $row['name']=$name;
				if (!empty($patronymic)) $row['patronymic']=$patronymic;
				if (!empty($birthday)) $row['birthday']=$birthday;
				if (!empty($bplace)) $row['bplace']=$bplace;
				if (!empty($gender)) $row['gender']=$gender;
				if (!empty($SNILS)) $row['SNILS']=$SNILS;
				if (!empty($citizenship)) $row['citizenship']=$citizenship;
				if (isset($tel)) $row['tel']=$tel;
				if (isset($email)) $row['email']=$email;
				//if (isset($email_agree)) $row['email_agree']=$email_agree;


				if (isset($parent_fio)) $row['parent_fio']=$parent_fio;
				if (isset($parent_degree)) $row['parent_degree']=$parent_degree;
				if (isset($parent_tel)) $row['parent_tel']=$parent_tel;
				if (isset($parent_email)) $row['parent_email']=$parent_email;

				$doc_data=array('doc_type'=>$doc_type, 'doc_ser'=>$doc_ser, 'doc_num'=>$doc_num, 'doc_date'=>$doc_date, 'doc_where'=>$doc_where, 'doc_period'=>$doc_period, 'doc_code'=>$doc_code);
			}
			else $doc_data='';
			$citizenships='';
			$res2=$mysqli->query('select id,name from olimp_citizenships');
			while ($row2=$res2->fetch_assoc()) $citizenships.='<option value='.$row2['id'].' '.($row2['id']==$row['citizenship'] ? 'selected' : '').'>'.$row2['name'].'</option>';
			$genders='<option value="M" '.($row['gender']=='M' ? 'selected' : '').'>Мужской</option><option value="F" '.($row['gender']=='F' ? 'selected' : '').'>Женский</option>';
			$doc_form=cabinet_document_form($row['doc_ident'],$tpl['cabinet_profile'],$doc_data);
			$content2=preg_replace(
				array('/%surename%/','/%name%/','/%patronymic%/','/%birthday%/','/%bplace%/','/%genders%/','/%SNILS%/','/%citizenships%/','/%tel%/','/%email%/','/%parent_fio%/','/%parent_degree%/','/%parent_tel%/','/%parent_email%/','/%doc_form%/'),
				array($row['surename'],$row['name'],$row['patronymic'],$row['birthday'],$row['bplace'],$genders,$row['SNILS'],$citizenships,$row['tel'],$row['email'],$row['parent_fio'],$row['parent_degree'],$row['parent_tel'],$row['parent_email'],$doc_form),
				$tpl['cabinet_profile']['content2_step1']);
			break;
		}
	case 2: //  Форма изменения адресов
		{	$res=$mysqli->query('select addr_reg, addr_post from olimp_persons where uid='.$USER_INFO['uid']);
			$row=$res->fetch_assoc();
/*			if ($formdata>0)
			{	$addr_reg
			}
			else $doc_data='';
*/
			$addr_reg_form=cabinet_address_form($row['addr_reg'],$tpl['cabinet_profile'],'addr_reg',$addr_reg_data);
			$addr_post_form=cabinet_address_form($row['addr_post'],$tpl['cabinet_profile'],'addr_post',$addr_post_data);
			
			$content3=preg_replace(
				array('/%addr_reg_form%/','/%addr_post_form%/'),
				array($addr_reg_form,$addr_post_form),
				$tpl['cabinet_profile']['content3_step1']);
			break;
		}
	case 3: //  Форма изменения учебного заведения
		{	
			ident_old_addresses();
			switch ($step)
			{
			case 1:	// Форма изменения класса и учителей
				{	$res=$mysqli->query('select school_class,class_m,class_f,class_i,class_k,teacher_m,teacher_f,teacher_i from olimp_persons where uid='.$USER_INFO['uid']);
					$row=$res->fetch_assoc();
					if ($formdata>0)
					{	if (!empty($school_class)) $row['school_class']=$school_class;
						if (!empty($teacher_m)) $row['teacher_m']=$teacher_m;
						if (!empty($teacher_f)) $row['teacher_f']=$teacher_f;
						if (!empty($teacher_i)) $row['teacher_i']=$teacher_i;
						if ($class_m!=0) {$row['class_m']=$class_m;} else $row['class_m']='4';
						if ($class_f!=0) $row['class_f']=$class_f;
						if ($class_i!=0) $row['class_i']=$class_i;
						if ($class_k!=0) $row['class_k']=$class_k;

					}
					$content4=preg_replace(
						array('/%school_class%/','/%teacher_m%/','/%teacher_f%/','/%teacher_i%/','/%class_m%/','/%class_f%/','/%class_i%/','/%class_k%/'),
						array($row['school_class'],$row['teacher_m'],$row['teacher_f'],$row['teacher_i'],$row['class_m']==0 ? '' : $row['class_m'],$row['class_f']==0 ? '' : $row['class_f'],$row['class_i']==0 ? '' : $row['class_i'],$row['class_k']==0 ? '' : $row['class_k']),
						$tpl['cabinet_profile']['content4_step1']);
					break;
				}
			case 3:	// Форма выбора страны/региона/номера школы
				{	
					$res=$mysqli->query('select a.country,a.region,s.name from olimp_persons as p left join olimp_schools as s on (s.id=p.school) left join olimp_address as a on (a.id=s.address) where p.uid='.$USER_INFO['uid']);
					$row=$res->fetch_assoc();
					if (preg_match('/[0-9]+/',$row['name'],$w)) 
					{	$school_num=$w[0];	}
					else
					{	$school_num='';
					}
					if ($formdata>0)
					{	if (!empty($school_country)) $row['country']=$school_country;
						if (!empty($school_region)) $row['region']=$school_region;
						if (!empty($_POST['school_num']))
						{	$school_num=trim($_POST['school_num']);
							$school_num=preg_replace('/[^0-9]/','',$school_num);
						}
					}
					$countrys='';
					$res2=$mysqli->query('select id,name from olimp_countrys');
					while ($row2=$res2->fetch_assoc()) $countrys.='<option value='.$row2['id'].($row2['id']==$row['country'] ? ' selected' : '').'>'.
						$row2['name'].'</option>';
					$regions='';
					$res2=$mysqli->query('select id,name,socr,ord from kladr_regions order by ord desc, name');
					while ($row2=$res2->fetch_assoc()) $regions.='<option value='.$row2['id'].($row2['id']==$row['region'] ? ' selected' : '').($row2['ord']>0 ? 
					' style="font-weight:bold"' : '').'>'.
						$row2['name'].' '.$row2['socr'].'</option>';
					$content4=preg_replace(
						array('/%school_countrys%/','/%school_regions%/','/%school_num%/'),
						array($countrys,$regions,$school_num),
						$tpl['cabinet_profile']['content4_step3']);
					break;
				}
			case 4:	// Форма выбора/добавления школы
				{	$school_addr=$_POST['school_addr'];
					$school_num=trim($_POST['school_num']);
					$school_num=preg_replace('/[^0-9]/','',$school_num);
					$schools='';
					
					// Подтвержденные школы
					
					$res=$mysqli->query('select s.id,t.name as school_type, s.school_type_ex, s.name, concat(c.name,", ",r.name," ",r.socr,", ", a.city," ",a.street," ", a.house) as city  from olimp_schools as s left join olimp_school_types as t on (t.id=s.school_type) left join olimp_address as a on (a.id=s.address) left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where (s.confirmed="Y") AND (a.country='.$school_addr['country'].')'.($school_addr['region']>0 ? ' AND (a.region='.$school_addr['region'].')' : '').($school_num>0 ? ' AND (s.name like "%'.$school_num.'%")' : '').' order by a.country, a.region,s.school_type, s.name');
					while ($row=$res->fetch_assoc()) $schools.='<option value='.$row['id'].' style="font-weight:bold; color:#006600;">'.
						($row['school_type_ex'] ? $row['school_type_ex'] : $row['school_type']).' '.$row['name'].' ('.$row['city'].')</option>';
					
					// Остальные школы
					
					$res=$mysqli->query('select s.id,t.name as school_type, s.school_type_ex, s.name, concat(c.name,", ",r.name," ",r.socr,", ", a.city," ",a.street," ", a.house) as city  from olimp_schools as s left join olimp_school_types as t on (t.id=s.school_type) left join olimp_address as a on (a.id=s.address) left join olimp_countrys as c on (c.id=a.country) left join kladr_regions as r on (r.id=a.region) where (s.confirmed="N") AND (a.country='.$school_addr['country'].')'.($school_addr['region']>0 ? ' AND (a.region='.$school_addr['region'].')' : '').($school_num>0 ? ' AND (s.name like "%'.$school_num.'%")' : '').' order by a.country, a.region,s.school_type, s.name');
					while ($row=$res->fetch_assoc()) $schools.='<option value='.$row['id'].' style="font-weight:normal; color:#666666;">'.
						($row['school_type_ex'] ? $row['school_type_ex'] : $row['school_type']).' '.$row['name'].' ('.$row['city'].')</option>';
						
					$school_types='';
					$res2=$mysqli->query('select id,name,descr from olimp_school_types order by name');
					while ($row2=$res2->fetch_assoc()) $school_types.='<option value='.$row2['id'].($row2['id']==$school_type ? ' selected' : '').'>'.$row2['name'].'-'.$row2['descr'].'</option>';
					$school_addr_form=cabinet_address_form(0,$tpl['cabinet_profile']['org_address_form'],'school_addr',$school_addr);
					if (empty($school_name)&&($school_num>0)) $school_name='№'.$school_num;
					$content4=preg_replace(
						array('/%schools%/','/%school_types%/','/%school_addr_form%/','/%school_type_ex%/','/%school_name%/','/%director%/','/%tel%/','/%email%/'),
						array($schools,$school_types,$school_addr_form,$school_type_ex,$school_name,$director,$tel,$email),
						$tpl['cabinet_profile']['content4_step4']);
					break;
				}
			}
			

			
			
			break;
		}
	}
}

// Вывод для шага 0


// Учетные данные
if (($selected_tab!=0)||($step==0))
{	$res=$mysqli->query('select nic,question,answer from olimp_users where id='.$USER_INFO['uid']);
	$row=$res->fetch_assoc();
	$content1=preg_replace(
		array('/%nic%/','/%question%/','/%answer%/'),
		array($row['nic'],$row['question'],$row['answer']),
		$tpl['cabinet_profile']['content1_step0']);
}
if (($selected_tab==0)&&($step==3))
{	$res=$mysqli->query('select nic,question,answer from olimp_users where id='.$USER_INFO['uid']);
	$row=$res->fetch_assoc();
	$content1=preg_replace(
		array('/%nic%/','/%question%/','/%answer%/','/%pwd%/'),
		array($row['nic'],$row['question'],$row['answer'],$pwd),
		$tpl['cabinet_profile']['content1_step3']);
}
//p.email_argee,
// Личные данные
if (($selected_tab!=1)||($step==0))
{	$res=$mysqli->query('select concat_ws(" ",p.surename,p.name,p.patronymic) as fio,DATE_FORMAT(p.birthday,"%d.%m.%Y") as birthday,p.gender,p.SNILS,p.doc_ident,c.name as citizenship,p.tel,p.email,p.parent_fio,p.parent_degree,p.parent_tel,p.parent_email, p.bplace as bplace from olimp_persons as p left join olimp_citizenships as c on (c.id=p.citizenship) where p.uid='.$USER_INFO['uid']);
	$row=$res->fetch_assoc();
	if ($row['doc_ident']>0)
	{	$document_ic=cabinet_document_text($row['doc_ident']);
	}
	else
	{	$document_ic=preg_replace('/%text%/','Документ, удостоверяющий личночть и гражданство не введен',$tpl['cabinet']['NoDataError']);
	}
	
	//if (empty($row['tel'])) $row['tel']=preg_replace(array('/%text%/','/%id%/','/%target%/'),array('Телефон не введен','btnTel','btnNext2'),$tpl['cabinet']['NoDataErrorBtn']);
	if (empty($row['bplace'])) $row['bplace']=preg_replace('/%text%/','Не введено место рождения',$tpl['cabinet']['NoDataError']);
	if (empty($row['tel'])) $row['tel']=preg_replace('/%text%/','Телефон не введен',$tpl['cabinet']['NoDataError']);
	if (empty($row['SNILS'])) $row['SNILS']=preg_replace('/%text%/','СНИЛС не введен',$tpl['cabinet']['NoDataError']);
	if (empty($row['email'])) $row['email']=preg_replace('/%text%/','Адрес электронной почты не введен',$tpl['cabinet']['NoDataWarning']);
	
	$content2=preg_replace(
		array('/%fio%/','/%birthday%/','/%bplace%/','/%gender%/','/%SNILS%/','/%document_ic%/','/%citizenship%/','/%tel%/','/%email%/','/%parent_fio%/','/%parent_degree%/','/%parent_tel%/','/%parent_email%/'),
		array($row['fio'],$row['birthday'],$row['bplace'],($row['gender']=='M' ? 'Мужской' : 'Женский'),$row['SNILS'],$document_ic,$row['citizenship'],$row['tel'],$row['email'],$row['parent_fio'],$row['parent_degree'],$row['parent_tel'],$row['parent_email']),
		$tpl['cabinet_profile']['content2_step0']);
}

// Адреса
if (($selected_tab!=2)||($step==0))
{	$res=$mysqli->query('select addr_reg,addr_post from olimp_persons where uid='.$USER_INFO['uid']);
	$row=$res->fetch_assoc();

	

	if ($row['addr_reg']>0) {
		if (isset($_POST['btnApprove'])){
			cabinet_address_approve_kladr($row['addr_reg']);
		}
		
		$addr_reg=cabinet_address_text($row['addr_reg']);
		$addr_reg_kladr=cabinet_address_text_kladr($row['addr_reg']);
	}
	else
	{	$addr_reg=preg_replace('/%text%/','Адрес постоянной регистрации не введен',$tpl['cabinet']['NoDataError']);
		$addr_reg_kladr='';
	}
	if ($row['addr_post']>0)
	{	
		if (isset($_POST['btnApprove'])){
			cabinet_address_approve_kladr($row['addr_post']);
		}
		$addr_post=cabinet_address_text($row['addr_post']);
		$addr_post_kladr=cabinet_address_text_kladr($row['addr_post']);
	}
	else
	{	$addr_post=preg_replace('/%text%/','Почтовый адрес не введен. Требуется только если отличается от адреса постоянной регистрации',$tpl['cabinet']['NoDataWarning']);
		$addr_post_kladr='';
	}
	
	$kladr_addr_warn = preg_replace('/%text%/','Проверьте правильно ли найден Ваш адрес в ФИАС!',$tpl['cabinet']['NoDataWarning']);
	
	$content3=preg_replace(
		array('/%addr_reg%/','/%addr_post%/','/%kladr_addr_warn%/','/%kladr_addr_reg%/','/%kladr_addr_post%/'),
		array($addr_reg,$addr_post,$kladr_addr_warn,$addr_reg_kladr,$addr_post_kladr),
		$tpl['cabinet_profile']['content3_step0']);

	if (empty($addr_reg_kladr))	{	
		$content3=preg_replace('/<%KLADR_REG%>.*?<\/%KLADR_REG%>/s','',$content3);
	}
	else {
		$content3=str_replace(array('<%KLADR_REG%>','</%KLADR_REG%>'),'',$content3);
	}

	if (empty($addr_post_kladr))	{	
		$content3=preg_replace('/<%KLADR_POST%>.*?<\/%KLADR_POST%>/s','',$content3);
	}
	else {
		$content3=str_replace(array('<%KLADR_POST%>','</%KLADR_POST%>'),'',$content3);
	}

	if (empty($addr_reg_kladr) && empty($addr_post_kladr)) 	{	
		$content3=preg_replace('/<%KLADR%>.*?<\/%KLADR%>/s','',$content3);
		$content3=preg_replace('/<%APPROVE_FIAS%>.*?<\/%APPROVE_FIAS%>/s','',$content3);
	}
	else {	
		$content3=str_replace(array('<%KLADR%>','</%KLADR%>'),'',$content3);
		$content3=str_replace(array('<%APPROVE_FIAS%>','</%APPROVE_FIAS%>'),'',$content3);
	}
	

}

// Учебное заведение
if (($selected_tab!=3)||($step==0))
{	$res=$mysqli->query('select school,school_class,teacher_m,teacher_f,teacher_i,class_m,class_f,class_i,class_k from olimp_persons where uid='.$USER_INFO['uid']);
	$row=$res->fetch_assoc();
	if ($row['school']>0)
	{	$school=cabinet_school_text($row['school']);
	}
	else
	{	$school=preg_replace('/%text%/','Учебное заведение не введено',$tpl['cabinet']['NoDataError']);
	}
	if (empty($row['school_class'])) $row['school_class']=preg_replace('/%text%/','Не введен класс, в котором обучается участник',$tpl['cabinet']['NoDataError']);
	if (empty($row['teacher_m'])) $row['teacher_m']=preg_replace('/%text%/','Не введены ФИО учителя математики',$tpl['cabinet']['NoDataWarning']);
	if (empty($row['teacher_f'])) $row['teacher_f']=preg_replace('/%text%/','Не введены ФИО учителя физики',$tpl['cabinet']['NoDataWarning']);
	if (empty($row['teacher_i'])) $row['teacher_i']=preg_replace('/%text%/','Не введены ФИО учителя информатики',$tpl['cabinet']['NoDataWarning']);

	$content4=preg_replace(
		array('/%school%/','/%school_class%/','/%teacher_m%/','/%teacher_f%/','/%teacher_i%/','/%class_m%/','/%class_f%/','/%class_i%/','/%class_k%/'),
		array($school,$row['school_class']!=20 ? $row['school_class'] : 'закончил обучение' ,$row['teacher_m'],$row['teacher_f'],$row['teacher_i'],$row['class_m']==0 ? '' : '<tr><td class="parName">Класс участия (математика):</td><td>'.$row['class_m'].'</td></tr>', $row['class_f']==0 ? '' : '<tr><td class="parName">Класс участия (физика):</td><td>'.$row['class_f'].'</td></tr>',$row['class_i']==0 ? '' : '<tr><td class="parName">Класс участия (информатика):</td><td>'.$row['class_i'].'</td></tr>',$row['class_k']==0 ? '' : '<tr><td class="parName">Класс участия (компьютерное моделирование):</td><td>'.$row['class_k'].'</td></tr>'),
		$tpl['cabinet_profile']['content4_step0']);
}


// Формирование страницы

if ($USER_INFO['block_edit']) 
{	$content2=preg_replace('/<%EDIT%>.*?<\/%EDIT%>/s','',$content2);$content2.='Вы не можете редактировать личные данные, т.к. уже загрузили заявление участника. В случае необходимости внесения изменений (если Вы еще не регистрировались в этап) удалите загруженное заявление на странице <Документы>, внесите необходимые изменения, после чего напечатайте и загрузите новое заявление. Если Вы уже зарегистрировались в какой-либо этап, свяжитесь с оргкомитетом Олимпиады.';
	$content3=preg_replace('/<%EDIT%>.*?<\/%EDIT%>/s','',$content3);$content3.='Вы не можете редактировать адреса, т.к. уже загрузили заявление участника. В случае необходимости внесения изменений (если Вы еще не регистрировались в этап) удалите загруженное заявление на странице <Документы>, внесите необходимые изменения, после чего напечатайте и загрузите новое заявление. Если Вы уже зарегистрировались в какой-либо этап, свяжитесь с оргкомитетом Олимпиады.';
	$content4=preg_replace('/<%EDIT%>.*?<\/%EDIT%>/s','',$content4);$content4.='Вы не можете редактировать данный раздел, т.к. уже загрузили заявление участника. В случае необходимости внесения изменений (если Вы еще не регистрировались в этап) удалите загруженное заявление на странице <Документы>, внесите необходимые изменения, после чего напечатайте и загрузите новое заявление. Если Вы уже зарегистрировались в какой-либо этап, свяжитесь с оргкомитетом Олимпиады.';
}
else
{	$content2=str_replace(array('<%EDIT%>','</%EDIT%>'),'',$content2);
	$content3=str_replace(array('<%EDIT%>','</%EDIT%>'),'',$content3);
	$content4=str_replace(array('<%EDIT%>','</%EDIT%>'),'',$content4);
}
$message='';
$msgs=build_message_list($USER_INFO['pid'],1,true); 
foreach ($msgs AS $msg)
{	$message.=str_replace(array('%id%','%dt%','%from%','%title%','%body%'),$msg,$tpl['cabinet']['message_line']);
}	
if (!empty($message)) $message=str_replace('%message%',$message,$tpl['cabinet']['message']);


if (!empty($error)) $error=preg_replace('/%error%/',preg_replace(array('/%title%/','/%text%/'),array('Ошибка!',$error),$tpl['common']['error']),$tpl['cabinet']['error']);
if (!empty($warning)) $warning=preg_replace('/%warning%/',preg_replace(array('/%title%/','/%text%/'),array('Внимание!',$warning),
		$tpl['common']['highlight']),$tpl['cabinet']['warning']);

$body=preg_replace(
			array('/%error%/','/%warning%/','/%message%/','/%tabs%/','/%selected_tab%/','/%content1%/','/%content2%/','/%content3%/','/%content4%/'),
			array($error,$warning,$message,$tabs,$selected_tab,$content1,$content2,$content3,$content4),
			$tpl['cabinet_profile']['main']);

return $body;
}

?>