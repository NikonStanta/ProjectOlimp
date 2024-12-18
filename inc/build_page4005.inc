<?php
// Личный кабинет. Загрузка работ.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');
include_once('../inc/cabinet2.inc');

function build_page($pgid,$subtype)
{    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;


if ($USER_INFO['uid']<1) return $tpl['cabinet']['logon_form'];


if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) 
{   return $tpl['cabinet']['closed'];
}

/*
if ($_SESSION['master_uid']<1)
{    //return '<br><br><div style="text-align:center; color:red; font-size:18px; padding:30px;">Доступ к странице закрыт до завершения работ по размещению результатов. <br><br>Следите за объявлениями на главной странице сайта.</div><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>';
        return '<br><br><div style="text-align:center; color:red; font-size:18px; padding:30px;">Загрузка работ творческих конкурсов и предметных олимпиад будет доступна на этой странице после 12 марта 2017г.</div><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>';
}
*/

// Перерегистрация участников прошлого года
$message='';
include_once('../inc/reregistrate.inc');
$error='';
$warning='';
$task_results='';

$_CURR_YEAR=(int)$SITE_VARS['current_year'];
$check_result=check_persons_data($USER_INFO['pid']);

$works_dir='../works';
$max_file_size=array(2=>52428800,4=>52428800,5=>52428800,6=>52428800);

// 'application/zip'=>1,
$allowed_file_types=array(
2=>array(/*'image/jpeg'=>1,*/'application/pdf'=>1/*,'application/x-rar-compressed'=>1,'application/zip'=>1,'application/x-rar'=>1*/),
4=>array(/*'image/jpeg'=>1,'application/pdf'=>1,*/'application/x-rar-compressed'=>1,'application/zip'=>1,'application/x-rar'=>1),
5=>array(/*'image/jpeg'=>1,*/'application/pdf'=>1/*,'application/x-rar-compressed'=>1,'application/zip'=>1,'application/x-rar'=>1*/),
6=>array('application/pdf'=>1/*,'application/x-rar-compressed'=>1,'application/zip'=>1,'application/x-rar'=>1*/)
);

$cmd='';

if (!empty($_POST['btnUploadWork'])) $cmd='UploadWork';
if (!empty($_POST['btnDeleteWork'])) $cmd='DeleteWork';

$olimp_exams=array();
$scales='';
$res=$mysqli->query('select * from olimp_exams');
while ($row=$res->fetch_assoc()) 
{    $olimp_exams[$row['id']]=$row;
    //$content[$row['id']]='';
}

if (!empty($_SESSION['debug'])) 
{    //echo '<hr>cmd= '.$cmd.'<hr>';
    //print_r($_POST);
}

switch ($cmd)
{
case 'UploadWork':
   {  $stage_id=$_POST['stage_id'];
       $cmd='';
       if ($_REQUEST['debug']>0) echo '--1--';
       $res=$mysqli->query('select a.presence as presence, s.form in (2,4,5,6) as form,  s.form as sform, s.results_ready as results_ready, IFNULL(w.status,"N") as status, UNIX_TIMESTAMP(s.date_post)-UNIX_TIMESTAMP(Now()) AS tshift from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_works as w ON ((w.stage_id=a.stage_id) AND (w.person_id=a.person_id)) where (a.stage_id='.$stage_id.') AND (a.person_id='.$USER_INFO['pid'].')');
       if ($row=$res->fetch_assoc())
       {   if ($_REQUEST['debug']>0) echo '--2--';
           if (($row['results_ready']!='N')&&($row['results_ready']!='P') || ($row['tshift']<0))
           {   $error='Прием работ завершен. Работа не загружена.'; 
               $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Прием работ завершен."');
               break;  }
           if ($_REQUEST['debug']>0) echo '--3--';
           if ($row['form']!=1)
           {   $error='Неверный поток. Работа не загружена.'; 
                $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Неверный поток."');
                break;  
            }
           if (($row['status']!='N') && ($row['status']!='L') && ($row['status']!='U') || ($row['tshift']<0))
           {   $error='Прием работ завершен. Работа не загружена.';
                $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Прием работ завершен."');
                break;  
            }

            if ($_REQUEST['debug']>0) echo '--4--';
           
           switch ($row['presence'])
           {
         case 'N':
         case 'P':
               {   $error='Прием работ завершен. Работа не загружена.'; 
                    $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Прием работ завершен."');
                    break;  
                }
         case 'A':
               {   $error='Работа аннулирована за нарушение порядка или регламента участия в олимпиаде. Работа не загружена'; 
                    $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Работа аннулирована."');
                    break;  
                }
         case 'R':
         case 'L':
         case 'W':
         case 'Y':
               {  if (is_uploaded_file($_FILES['workfile']['tmp_name']))
                   {       
                        $FileType=mime_content_type($_FILES['workfile']['tmp_name']);
                        
                        // if ($USER_INFO['uid']<100)  echo $FileType;
                        
                        if (!array_key_exists($FileType,$allowed_file_types[$row['sform']]))
                        {    $error='Недопустимый формат файла. Работа не загружена.'; 
                            $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Недопустимый формат файла."');
                            break; 
                        }
                        
                        if ($_FILES['workfile']['size']<=$max_file_size[$row['sform']])
                        {  $work_file=$works_dir.'/'.$stage_id.'_'.$USER_INFO['pid'];
                            if (($row['status']=='L') || ($row['status']=='U')) 
                            {   $NewStatus='U';
                                $work_file_old=$work_file.'_U'.date('YmdHis');
                                rename($work_file,$work_file_old);
                                //unlink($work_file);
                            }
                            else
                            {  $NewStatus='L'; } 
                            if (move_uploaded_file($_FILES['workfile']['tmp_name'], $work_file))
                            {  $mysqli->query('replace olimp_works set stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', status="'.$NewStatus.'"');
                                $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="'.$NewStatus.'"');
                                $message='Работа загружена.';
                            }
                            else
                            {  $mysqli->query('replace olimp_works set stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', status="N"');
                                if ( $NewStatus=='U') $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="D"');
                                $error='Ошибка при загрузке файла. Работа не загружена.';
                            }
                        }
                        else
                        {   $error='Слишком большой файл. Работа не загружена.'; 
                            $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Слишком большой файл."');
                            break;  
                        }
                    }
                   else
                   {   switch($_FILES['workfile']['error'])
                        {
                        case 1: //uploaded file exceeds the upload_max_filesize directive in php.ini
                        case 2: //uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the html form
                        {    $error='Слишком большой файл. Работа не загружена.'; 
                            $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Слишком большой файл."');
                            break;
                        }
                        default: //a default error, just in case!  :)
                        {    $error='Ошибка при загрузке файла. Работа не загружена.'; 
                            $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Ошибка при загрузке файла."');
                        }
                        }
                        break; 
                    }
                   break;  
               }
            default:
               {   $error='Неожиданная ошибка. Работа не загружена. Обратитесь в оргкомитет.'; 
                    $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="E", comment="Неверный статус."');
                    break;  
                }

           }
       }
       else
       {  $error='Неверный поток. Работа не загружена.';
           $cmd='';
       }
       break;
    }
case 'DeleteWork':
   {  $stage_id=$_POST['stage_id'];
       $cmd='';
       $res=$mysqli->query('select a.presence as presence, s.form in (2,4,5,6) as form,  s.results_ready as results_ready, IFNULL(w.status,"N") as 
    status from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_works as w ON ((w.stage_id=a.stage_id) AND 
                                                                                                                   (w.person_id=a.person_id)) where (a.stage_id='.$stage_id.') AND (a.person_id='.$USER_INFO['pid'].')');
       if ($row=$res->fetch_assoc())
       {  if (($row['results_ready']!='N')&&($row['results_ready']!='P'))
           {   $error='Прием работ завершен. Удаление работы невозможно.'; break;  }
           if ($row['form']!=1)
           {   $error='Неверный поток. Удаление работы невозможно.'; break;  }
           if (($row['status']!='N') && ($row['status']!='L') && ($row['status']!='U'))
           {   $error='Прием работ завершен. Удаление работы невозможно.'; break;  }
           switch ($row['presence'])
           {
         case 'N':
         case 'P':
               {   $error='Прием работ завершен. Удаление работы невозможно.'; break;  }
         case 'A':
               {   $error='Работа аннулирована за нарушение порядка или регламента участия в олимпиаде. Удаление работы невозможно'; break;  }
         case 'R':
         case 'L':
         case 'Y':
               {  $work_file=$works_dir.'/'.$stage_id.'_'.$USER_INFO['pid'];
                   if (($row['status']=='L') || ($row['status']=='U')) 
                   {   $work_file_old=$work_file.'_D'.date('YmdHis');
                       rename($work_file,$work_file_old);
                       //unlink($work_file);
                       $mysqli->query('replace olimp_works set stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', status="N"');
                       $mysqli->query('insert into olimp_works_log set dt=Now(), stage_id='.$stage_id.', person_id='.$USER_INFO['pid'].', action="D"');
                       $message='Работа удалена.';
                   }
                   else         
                   {   $error='Работа не была загружена. Удаление работы невозможно.'; break;  }
    
                   break;  
               }

           }
       }
       else
       {  $error='Неверный поток. Удаление работы невозможно.';
           $cmd='';
       }
       break;
    }
}

$olimp_places=array();
$res=$mysqli->query('select * from olimp_places');
while ($row=$res->fetch_assoc()) $olimp_places[$row['id']]=$row;

if (!empty($_SESSION['debug'])) 
{    //    echo '<hr>D => '.$error.'<hr>';
}

// Заключительный этап

$fcontent=array();

$res=$mysqli->query('select a.presence as presence, s.exam as exam, s.form as form, s.place as place, s.results_ready as results_ready, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as exam_sdt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_sdt, IFNULL(w.status,"N") as status, s.id as stage_id, IF(s.date_post < Now() ,1,IF(s.date_exam > Now() ,2,0)) as is_closed, Now() as Now from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_works as w ON ((w.stage_id=a.stage_id) AND (w.person_id=a.person_id)) where ((s.stage=2) AND (s.form in (1,2,6))) AND (a.person_id='.$USER_INFO['pid'].') AND (s.year='.$_CURR_YEAR.')  order by s.exam, s.date_exam');
while ($row=$res->fetch_assoc())
{    switch ($row['presence'])
    {
    case 'Y':
        {    $row['status']='A';
        }
    case 'R':
    case 'L':
    case 'W':
        {    switch ($row['results_ready'])
            {
            case 'Y':
                // Готовы результаты
                {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном'),
                            $tpl['cabinet_works']['ready']);
                    break;
                }
            case 'P':
            case 'N':    // Результаты не готовы
                {    $resW=$mysqli->query('select * from olimp_works_log where (stage_id='.$row['stage_id'].') AND (person_id='.$USER_INFO['pid'].') order by dt DESC limit 1');
                    $rowW=$resW->fetch_assoc();
                    switch ($row['status'])
                    {
                    case 'N':    // не загружена
                        {    $fmt = (array_key_exists('load_'.$row['form'],$tpl['cabinet_works']) && !empty($tpl['cabinet_works']['load_'.$row['form']])) ? $tpl['cabinet_works']['load_'.$row['form']] : $tpl['cabinet_works']['load'];
                            $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%stage_id%','%stage%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['stage_id'],'заключительном'),
                            $fmt);  // $tpl['cabinet_works']['load_'.$row['form']]
                            break;
                        }
                    case 'L':    // загружена
                    case 'U':    // изменена
                        {    
                            $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['stage_id']),
                            $tpl['cabinet_works']['update_'.$row['form']]);
                            break;
                        }
                    case 'A':    // принята
                        {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['accepted_'.$row['form']]);
                            break;
                        }
                    case 'R':    // отклонена
                        {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['rejected_'.$row['form']]);
                            break;
                        }
                    }
                    break;
                }

            }

            break;
        }
    case 'N':  // Срок загрузки прошел
        {    switch ($row['form'])
            {
            case 1:    // Неявка на очную форму
                {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_works']['unpresent_o']);
                    break;
                }
            case 2:    // Неявка на заочную форму
                {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_works']['unpresent_z']);
                    break;
                }
            }
            break;
        }

    case 'A':     // Работа аннулирована
        {    
        
        
            switch ($row['results_ready'])
            {
            case 'Y':    // Готовы окончательные результаты
            case 'P':    // Готовы предварительные результаты
                {
                    switch ($row['form'])
                    {
                    case 1:
                        {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['annulated_o']);
                            break;
                        }
                    case 2:
                        {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                            $tpl['cabinet_works']['annulated_z']);
                            break;
                        }
                    }
                    break;
                }
            case 'N':    // Результаты не готовы
                {    $resW=$mysqli->query('select * from olimp_works_log where (stage_id='.$row['stage_id'].') AND (person_id='.$USER_INFO['pid'].') order by dt DESC limit 1');
                    $rowW=$resW->fetch_assoc();
                    switch ($row['status'])
                    {
                    case 'N':    // не загружена
                        {    $fmt = (array_key_exists('load_'.$row['form'],$tpl['cabinet_works']) && !empty($tpl['cabinet_works']['load_'.$row['form']])) ? $tpl['cabinet_works']['load_'.$row['form']] : $tpl['cabinet_works']['load'];
                            $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%stage_id%','%stage%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['stage_id'],'заключительном'),
                            $fmt);  // $tpl['cabinet_works']['load_'.$row['form']]
                            break;
                        }
                    case 'L':    // загружена
                    case 'U':    // изменена
                        {    
                            $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['stage_id']),
                            $tpl['cabinet_works']['update_'.$row['form']]);
                            break;
                        }
                    case 'A':    // принята
                        {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['accepted_'.$row['form']]);
                            break;
                        }
                    case 'R':    // отклонена
                        {    $fcontent[$row['exam']]=(array_key_exists($row['exam'],$fcontent) ? $fcontent[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['rejected_'.$row['form']]);
                            break;
                        }
                    }
                    break;
                }
            }

            break;
        }
    }
}


$fcontent_all='';

foreach ($fcontent as $k=>$w) $fcontent_all.=str_replace(array('%exam%','%result%'),array($olimp_exams[$k]['name'],$w),$tpl['cabinet_works']['exam_result']);

$fcontent_all=str_replace('%results%',$fcontent_all,$tpl['cabinet_works']['final']);

if (!empty($_SESSION['debug']))
{    // echo '<hr>C => '.$error.'<hr>';
}

// Отборочный этап

$content=array();

$query='select a.presence as presence, s.exam as exam, s.form as form,  s.place as place, s.results_ready as results_ready, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as exam_sdt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_sdt, IFNULL(w.status,"N") as status, s.id as stage_id, IF(s.date_post < Now() ,1,IF(s.date_exam > Now() ,2,0)) as is_closed, Now() as Now from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_works as w ON ((w.stage_id=a.stage_id) AND (w.person_id=a.person_id)) where s.year='.$_CURR_YEAR.' AND (s.stage=1) AND (a.person_id='.$USER_INFO['pid'].') AND (s.form in (1,2,4)) order by s.exam, s.date_exam';

$res=$mysqli->query($query);

if (!empty($_SESSION['debug']))
{    // echo $query;
}

while ($row=$res->fetch_assoc())
{
    switch ($row['presence'])
    {
    case 'Y':
        {    $row['status']='A';
        }
    case 'R':
    case 'L':
    case 'W':
        {    switch ($row['results_ready'])
            {
            case 'Y':
            // Готовы результаты
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном'),
                            $tpl['cabinet_works']['ready']);
                    break;
                }
            case 'P':
            case 'N':    // Результаты не готовы
                {    $resW=$mysqli->query('select * from olimp_works_log where (stage_id='.$row['stage_id'].') AND (person_id='.$USER_INFO['pid'].') order by dt DESC limit 1');
                    $rowW=$resW->fetch_assoc();
                    switch ($row['status'])
                    {
                    case 'N':    // не загружена
                        {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%Now%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['Now'],$row['stage_id']),
                            $row['is_closed']==2 ? $tpl['cabinet_works']['wait_'.$row['form']] : ($row['is_closed']==1 ? $tpl['cabinet_works']['closed_'.$row['form']] : $tpl['cabinet_works']['load_'.$row['form']]));
                            
                            break;
                        }
                    case 'L':    // загружена
                    case 'U':    // изменена
                        {    
                            $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%Now%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['Now'],$row['stage_id']),
                            $row['is_closed']==1 ? $tpl['cabinet_works']['accepted_'.$row['form']] : $tpl['cabinet_works']['update_'.$row['form']]);
                            break;
                        }
                    case 'A':    // принята 
                        {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['accepted_'.$row['form']]);
                            break;
                        }
                    case 'R':    // отклонена
                        {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['rejected_'.$row['form']]);
                            break;
                        }
                    }
                    break;
                }

            }

            break;
        }
    case 'N':  // Срок загрузки прошел
        {    switch ($row['form'])
            {
            case 1:    // Неявка на очную форму
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_works']['unpresent_o']);
                    break;
                }
            case 2:    // Неявка на заочную форму
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_works']['unpresent_z']);
                    break;
                }
            }
            break;
        }

    case 'P': // Срок загрузки прошел, техническая неявка
        {    switch ($row['form'])
            {
            case 1:    // Неявка на очную форму
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_works']['tech_unpresent_o']);
                    break;
                }
            case 2:    // Неявка на заочную форму
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_works']['tech_unpresent_z']);
                    break;
                }
            }
            break;
        }

    case 'A':     // Работа аннулирована
        {    
        switch ($row['results_ready'])
        {
        case 'Y':
        // Готовы результаты
{
            switch ($row['form'])
            {
            case 1:
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                    array('%exam%','%place%','%exam_dt%'),
                    array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                    $tpl['cabinet_works']['annulated_o']);
                    break;
                }
            case 2:
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                    array('%exam%','%place%','%exam_dt%','%post_dt%'),
                    array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                    $tpl['cabinet_works']['annulated_z']);
                    break;
                }
            }
            break;
        }
        case 'P':
        case 'N':    // Результаты не готовы
{                            $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%Now%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['Now'],$row['stage_id']),
                            $row['is_closed']==1 ? $tpl['cabinet_works']['accepted_'.$row['form']] : $tpl['cabinet_works']['update_'.$row['form']]);
                            break;
}
        }
    }
}
}


$content_all='';
foreach ($content as $k=>$w) $content_all.=str_replace(array('%exam%','%result%'),array($olimp_exams[$k]['name'],$w),$tpl['cabinet_works']['exam_result']);

if (!empty($_SESSION['debug']))
{    // echo '<hr>B => '.$error.'<hr>';
}

// Тренировочный этап

$content=array();


$res=$mysqli->query('select a.presence as presence, s.exam as exam, s.form as form,  s.place as place, s.results_ready as results_ready, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as exam_sdt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_sdt, IFNULL(w.status,"N") as status, s.id as stage_id, IF(s.date_post < Now() ,1,IF(s.date_exam > Now() ,2,0)) as is_closed, Now() as Now from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) left join olimp_works as w ON ((w.stage_id=a.stage_id) AND (w.person_id=a.person_id)) where (s.year='.$_CURR_YEAR.') AND (s.stage=3) AND (a.person_id='.$USER_INFO['pid'].') AND (s.form in (1,2)) order by s.exam, s.date_exam');
while ($row=$res->fetch_assoc())
{      switch ($row['presence'])
    {
    case 'Y':
        {    $row['status']='A';
        }
    case 'R':
    case 'L':
    case 'W':
        {    
            switch ($row['results_ready'])
            {
            case 'Y':
            // Готовы результаты
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном'),
                            $tpl['cabinet_works']['ready']);
                    break;
                }
            case 'P':
            case 'N':    // Результаты не готовы
                {    $resW=$mysqli->query('select * from olimp_works_log where (stage_id='.$row['stage_id'].') AND (person_id='.$USER_INFO['pid'].') order by dt DESC limit 1');
                    $rowW=$resW->fetch_assoc();
                    switch ($row['status'])
                    {
                    case 'N':    // не загружена
                        {    
                            $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%Now%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['Now'],$row['stage_id']),
                            $row['is_closed']==2 ? $tpl['cabinet_works']['rwait_'.$row['form']] : ($row['is_closed']==1 ? $tpl['cabinet_works']['rclosed_'.$row['form']] : $tpl['cabinet_works']['rload_'.$row['form']]));
                            

                            break;
                        }
                    case 'L':    // загружена
                    case 'U':    // изменена
                        {     $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%','%Now%','%stage_id%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt'],$row['Now'],$row['stage_id']),
                            $row['is_closed']==1 ? $tpl['cabinet_works']['raccepted_'.$row['form']] : $tpl['cabinet_works']['rupdate_'.$row['form']]);  //['rload_'.$row['form']]); 12.11.2021
                            break;
                        }
                    case 'A':    // принята 
                        {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['accepted_'.$row['form']]);
                            break;
                        }
                    case 'R':    // отклонена
                        {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_works']['rejected_'.$row['form']]);
                            break;
                        }
                    }
                    break;
                }

            }

            break;
        }
    case 'N':  // Срок загрузки прошел
        {    switch ($row['form'])
            {
            case 1:    // Неявка на очную форму
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_works']['unpresent_o']);
                    break;
                }
            case 2:    // Неявка на заочную форму
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_works']['unpresent_z']);
                    break;
                }
            }
            break;
        }

    case 'A':     // Работа аннулирована
        {    switch ($row['form'])
            {
            case 1:
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                    array('%exam%','%place%','%exam_dt%'),
                    array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                    $tpl['cabinet_works']['annulated_o']);
                    break;
                }
            case 2:
                {    $content[$row['exam']]=(array_key_exists($row['exam'],$content) ? $content[$row['exam']] : '').str_replace(
                    array('%exam%','%place%','%exam_dt%','%post_dt%'),
                    array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                    $tpl['cabinet_works']['annulated_z']);
                    break;
                }
            }
            break;
        }
    }
}


$rcontent_all='';
foreach ($content as $k=>$w) $rcontent_all.=str_replace(array('%exam%','%result%'),array($olimp_exams[$k]['name'],$w),$tpl['cabinet_works']['exam_result']);


if (!empty($_SESSION['debug']))
{    echo '<hr>A => '.$error.'<hr>';
}

// Формирование страницы

$message='';
$msgs=build_message_list($USER_INFO['pid'],1,true); 
foreach ($msgs AS $msg)
{    $message.=str_replace(array('%id%','%dt%','%from%','%title%','%body%'),$msg,$tpl['cabinet']['message_line']);
}    
if (!empty($message)) $message=str_replace('%message%',$message,$tpl['cabinet']['message']);


if (!empty($error)) $error=preg_replace('/%error%/',preg_replace(array('/%title%/','/%text%/'),array('Ошибка!',$error),$tpl['common']['error']),$tpl['cabinet']['error']);
if (!empty($warning)) $warning=preg_replace('/%warning%/',preg_replace(array('/%title%/','/%text%/'),array('Внимание!',$warning),
        $tpl['common']['highlight']),$tpl['cabinet']['warning']);
        


$body=preg_replace(
            array('/%error%/','/%warning%/','/%message%/','/%results%/','/%final%/','/%repetition%/'),
            array($error,$warning,$message,$content_all,$fcontent_all,$rcontent_all),
            $tpl['cabinet_works']['main']);

return $body;
}

?>