<?php
// Личный кабинет. Результаты.

include_once('../inc/common.inc');
include_once('../inc/cabinet.inc');
include_once('../inc/cabinet2.inc');

function build_page($pgid,$subtype)
{    global $mysqli,$SITE_VARS,$PAGE,$_PAGE_VAR_VALUES,$tpl,$USER_INFO;

$var_dir='/documents';
$var_file_prefix='ehope_variant';
$var_file_ext='rar';

if ($USER_INFO['uid']<1) return $tpl['cabinet']['logon_form'];


if (!$SITE_VARS['cabinet_opened'] && empty($_SESSION['debug'])) 
{   return $tpl['cabinet']['closed'];
}

//if ($_SESSION['master_uid']<1)
//{    return '<br><br><div style="text-align:center; color:red; font-size:18px; padding:30px;">Доступ к странице закрыт до завершения работ по размещению результатов. <br><br>Следите за объявлениями на главной странице сайта.</div><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>';
//}

// Перерегистрация участников прошлого года
$message='';
include_once('../inc/reregistrate.inc');
$error='';
$warning='';

$check_result=check_persons_data($USER_INFO['pid']);
$examLT=array(1=>'m',2=>'p',3=>'i',4=>'k',11=>'M',12=>'P',13=>'I',14=>'K');
$exams = array(1=>'Математика', 2=>'Физика', 3=>'Информатика', 4=>'Компьютерное моделирование');
$_CURR_YEAR=(int)$SITE_VARS['current_year'];

$olimp_exams=array();
$scales=array();
$res=$mysqli->query('select * from olimp_exams');
while ($row=$res->fetch_assoc()) 
{    $olimp_exams[$row['id']]=$row;
    $content[$row['id']]='';
    $training[$row['id']]='';
    $registered[$row['id']]=false;
    $w=explode(';',$row['scale']);
    foreach ($w as $v) 
    {    $ww=explode('=',$v);
        if (is_array($ww) && count($ww)>1)    $scales[$row['id']][$ww[0]]=$ww[1];
    }
    $w=explode(';',$row['scale2']);
    foreach ($w as $v) 
    {    $ww=explode('=',$v);
        if (is_array($ww) && count($ww)>1)    $scales2[$row['id']][$ww[0]]=$ww[1];
    }
}

$result_symbols=array(0=>'o',1=>'-',2=>'&divide;',3=>'&plusmn;',4=>'+');
$result_symbols2=array(0=>'нет',1=>'0',2=>'=',3=>'-',4=>'-/+',5=>'&plusmn;',6=>'+');

$ranks=array(0=>'',1=>'участник',2=>'призер (3 степень)',3=>'призер (2 степень)',4=>'победитель');
$exam_letter=array(1=>'m','p','i','k');

$olimp_places=array();
$res=$mysqli->query('select * from olimp_places');
while ($row=$res->fetch_assoc()) $olimp_places[$row['id']]=$row;

// Сертификаты участника
/*
$ParticipantCertText='';
if ($SITE_VARS['participant_cert_awail']==1 || ($_SESSION['debug']>0))
{    $res=$mysqli->query('select DISTINCT s.exam as exam from olimp_actions AS a LEFT JOIN olimp_stages as s ON (s.id=a.stage_id) where s.year='.$_CURR_YEAR.' AND a.rank IN ("1","2","3","4") AND a.person_id='.$USER_INFO['pid'].' order by s.exam');
    while ($row=$res->fetch_assoc())
    {    $ParticipantCertText.=str_replace(array('%exam%','%exam_txt%'),array($row['exam'], $exams[$row['exam']]),$tpl['cabinet_result']['participant_cert_line']);
    }
    if (!empty($ParticipantCertText))
        $ParticipantCertText = str_replace('%lines%',$ParticipantCertText,$tpl['cabinet_result']['participant_cert_block']);
}

$ParticipantCertText='';
if ($SITE_VARS['participant_cert_awail']==1 || ($_SESSION['debug']>0))
{    $res=$mysqli->query('select DISTINCT s.exam as exam from olimp_actions AS a LEFT JOIN olimp_stages as s ON (s.id=a.stage_id) where s.year!='.$_CURR_YEAR.' AND a.rank IN ("1","2","3","4") AND a.person_id='.$USER_INFO['pid'].' order by s.exam');
    while ($row=$res->fetch_assoc())
    {    $ParticipantCertText.=str_replace(array('%exam%','%exam_txt%'),array($row['exam'], $exams[$row['exam']]),$tpl['cabinet_result']['participant_cert_line']);
    }
    if (!empty($ParticipantCertText))
        $ParticipantCertText = str_replace('%lines%',$ParticipantCertText,$tpl['cabinet_result']['participant_cert_block_prev']);
}
*/

// Заключительный этап

$final_result='';
//$final_result.=$ParticipantCertText;
$res=$mysqli->query('select a.presence as presence, a.result as result, a.task_results as task_results, a.`rank` as rank, s.exam as exam, s.form as form, s.place as place, s.task_count as task_count, s.results_ready as results_ready, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as exam_sdt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_sdt, s.stage_result_text as srt, s.use_person_result_text as use_prt, rt.comment as prt, ex.result_text2 as ert, s.task_weight as task_weight  from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id)  left join olimp_result_comments as rt on ((rt.stage_id=a.stage_id) AND (rt.person_id=a.person_id))  left join olimp_exams as ex on (ex.id=s.exam)  where s.year='.$_CURR_YEAR.' AND (s.stage=2) AND (a.person_id='.$USER_INFO['pid'].') order by s.exam, s.date_exam');
while ($res && $row=$res->fetch_assoc())
{    switch ($row['presence'])
    {
    case 'R':
        {    $final_result.=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['2registered']);
            
            break;
        }

    case 'L':
        {    $final_result.=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['2registered']);
            
            break;
        }
    case 'N':
        {    // Неявка
            $final_result.=str_replace(
                array('%exam%','%place%','%exam_dt%'),
                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                $tpl['cabinet_result']['2unpresent']);
            break;
        }
    case 'A':     // Работа аннулирована
        {    $final_result.=str_replace(
                array('%exam%','%place%','%exam_dt%'),
                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                $tpl['cabinet_result']['2annulated']);
            break;
        }
    case 'Y':
        {    $task_results='';
            if ($row['task_results']!='')
            {    if (!empty($tpl['cabinet_result']['2result_table_'.$row['exam']])) { $tpl_result_table=$tpl['cabinet_result']['2result_table_'.$row['exam']]; }
                else if (!empty($tpl['cabinet_result']['2result_table'])) { $tpl_result_table=$tpl['cabinet_result']['2result_table']; }
                else if (!empty($tpl['cabinet_result']['result_table_'.$row['exam']])) { $tpl_result_table=$tpl['cabinet_result']['result_table_'.$row['exam']]; }
                else { $tpl_result_table=$tpl['cabinet_result']['result_table']; }


                $w=explode(',',$row['task_results']);
                $ww=explode(',',$row['task_weight']);
                $r1='';
                $r2='';
                $r3='';

                for ($i=1;$i<$row['task_count'];$i++)
                {    $wball=$w[$i-1]+0; //*($ww[$i-1]>0 ? $ww[$i-1] : 1)+0;
                    $r1.=str_replace('%num%',$i,$tpl['cabinet_result']['result_ttl_cell']);
                    $r2.=str_replace('%result%',$result_symbols2[$scales2[$row['exam']][$w[$i-1]+0]],$tpl['cabinet_result']['result_val_cell']);
                    $r3.=str_replace('%result%',$wball,$tpl['cabinet_result']['result_val_cell']);
                }

                $task_results=str_replace(array('%row1%','%row2%','%row3%'),array($r1,$r2,$r3),$tpl_result_table);

                $result_t=$w[$row['task_count']-1]+0;
                
                if ($result_t>0)
                {    $result_tt=str_replace('%ball%',$result_t,$tpl['cabinet_result']['2result_tt']);
                }
                else
                {$result_tt='';
                }

            }
            switch ($row['results_ready'])
            {
            case 'Y':    // Готовы окончательные результаты
                {    if ($row['result']>=0)
                    {    if (!empty($tpl['cabinet_result']['2ready_'.$row['exam']]))
                        {    $wtpl=$tpl['cabinet_result']['2ready_'.$row['exam']];    }
                        else
                        {    $wtpl=$tpl['cabinet_result']['2ready'];    }
                        
                        $result_final=$tpl['cabinet_result']['2result_final_'.$row['rank']];
                        
//if ($_SESSION['debug'] || $_REQUEST['debug']) print_r($row);
                        
                        $doc_link='';
                        $user_file=$_CURR_YEAR.'/'.$examLT[$row['exam']+10].$USER_INFO['pid'].'.pdf';
                        if ($row['rank']>1 && file_exists('../docs/diplomas'.$user_file))
                        {    $doc_link=str_replace('%link%','ehope_diploma'.$_CURR_YEAR.$examLT[$row['exam']].'.pdf',$tpl['cabinet_result']['diploma']);
                        }
                        if ($row['rank']==1 && file_exists('../docs/certificates'.$user_file))
                        {    $doc_link=str_replace('%link%','ehope_certificate'.$_CURR_YEAR.$examLT[$row['exam']].'.pdf',$tpl['cabinet_result']['certificate']);
                        }
                        
                        if (!empty($_SESSION['debug']))
                        {    //echo '../docs/diplomas'.$user_file;
                        }
                        
                        $result_final=str_replace('%doc_link%',$doc_link,$result_final);
                        
                        $final_result.=str_replace(
                            array('%pid%','%exam%','%place%','%exam_dt%','%task_results%','%result%','%result_tt%','%result_final%','%stage_result_text%','%personal_result_text%','%exam_result_text%'),
                            array($USER_INFO['pid'],$olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$task_results,$row['result'],$result_tt,$result_final,$row['srt'],$row['use_prt'] ? $row['prt'] : '',$row['ert']),$wtpl);
                        break;
                    }
                }
            case 'P':    // Готовы предварительные результаты
                {    if ($row['result']>=0)
                    {    if (!empty($tpl['cabinet_result']['2ready_p'.$row['exam']]))
                        {    $wtpl=$tpl['cabinet_result']['2ready_p'.$row['exam']];    }
                        else if(!empty($tpl['cabinet_result']['2ready_p']))
                        {    $wtpl=$tpl['cabinet_result']['2ready_p'];    }
                        else if (!empty($tpl['cabinet_result']['2ready_'.$row['exam']]))
                        {    $wtpl=$tpl['cabinet_result']['2ready_'.$row['exam']];    }
                        else
                        {    $wtpl=$tpl['cabinet_result']['2ready'];    }


                        $scan_ref=str_replace(array('%pid%','%exam%'),array($USER_INFO['pid'],$exam_letter[$row['exam']]),$tpl['cabinet_result']['2result_scan']);
                        $final_result.=str_replace(
                            array('%pid%','%exam%','%place%','%exam_dt%','%task_results%','%result%','%result_tt%','%result_final%','%stage_result_text%','%personal_result_text%','%exam_result_text%','%scan%','%apk%'),
                            array($USER_INFO['pid'],$olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$task_results,$row['result'],$result_tt,'',$row['srt'],$row['use_prt'] ? $row['prt'] : '',$row['ert'],$scan_ref,
                                $tpl['cabinet_result']['2result_apk']),$wtpl);
                        break;
                    }
                }
            case 'N':    // Результаты не готовы
                {    $final_result.=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['2progress']);
                    break;
                }
            }
            break;
        }
    }
}

$final_results='';
if (!empty($final_result)) $final_results=str_replace('%results%',$final_result,$tpl['cabinet_result']['final']);

// Отборочный этап
//a.rank as rank,
$res=$mysqli->query('select a.presence as presence, a.result as result, a.task_results as task_results, s.exam as exam, s.form as form, s.place as place, s.task_count as task_count, s.results_ready as results_ready, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as exam_sdt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_sdt,s.task_weight as task_weight, p.printed_z, a.rank AS "rank" from olimp_persons as p left join olimp_actions as a on (a.person_id=p.id) left join olimp_stages as s on (s.id=a.stage_id)  where (s.stage=1) AND (a.person_id='.$USER_INFO['pid'].') and s.year='.$_CURR_YEAR.' order by s.exam, s.date_exam');
while ($res && $row=$res->fetch_assoc())
{    switch ($row['presence'])
    {
    case 'R':
    case 'L':
        {    switch ($row['form'])
            {
            case 1:    // Зарегистрирован на очную форму
                {    $content[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['registered_o']);
                    $registered[$row['exam']]=true;
                    break;
                }
            case 2:    // Зарегистрирован на заочную форму
                {    $content[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['address'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_result']['registered_z']);
                    $registered[$row['exam']]=true;
                    break;
                }
            }
            break;
        }
    case 'N':
        {    switch ($row['form'])
            {
            case 1:    // Неявка на очную форму
                {    $content[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['unpresent_o']);
                    break;
                }
            case 2:    // Неявка на заочную форму
                {    $content[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_result']['unpresent_z']);
                    break;
                }
            }
            break;
        }
    case 'P':
        {    switch ($row['form'])
            {
            case 1:    // Техническая неявка на очную форму
                {    $content[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['tech_unpresent_o']);
                    break;
                }
            case 2:    // Техническая неявка на заочную форму
                {    $content[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_result']['tech_unpresent_z']);
                    break;
                }
            }
            break;
        }

    case 'W':     // Ожидание письма
        {    $content[$row['exam']].=str_replace(
                array('%exam%','%place%','%exam_dt%','%post_dt%'),
                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                $tpl['cabinet_result']['waiting']);
            $registered[$row['exam']]=true;
            break;
        }
    case 'E':     // Ошибка! Неявка или Ожидание письма
        {    $content[$row['exam']].=str_replace(
                array('%exam%','%place%','%exam_dt%','%post_dt%'),
                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                $tpl['cabinet_result']['result_error']);
            //$registered[$row['exam']]=true;
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
                    case 1:    // Очная форма
                        {    $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_result']['annulated_o']);
                            break;
                        }
                    case 2:    // Заочная форма
                        {    $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                            $tpl['cabinet_result']['annulated_z']);
                            break;
                        }
                    }
                    break;
                }
            case 'N':    // Результаты не готовы
                {    switch ($row['form'])
                    {
                    case 1:    // Очная форма
                        {    $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_result']['progress_o']);
                        break;
                        }
                    case 2:    // Заочная форма
                        {    $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                            $tpl['cabinet_result']['progress_z']);
                        break;
                        }
                    }
                    break;
                }
            }
            
            
        
            
            $registered[$row['exam']]=true;
            break;
        }
    case 'Y':
        {    $task_results='';
            if ($row['task_results']!='')
            {    $w=explode(',',$row['task_results']);
                $ww=explode(',',$row['task_weight']);
                $r1='';
                $r2='';
                $r3='';
                for ($i=1;$i<=$row['task_count'];$i++)
                {    
                    $wball=($w[$i-1]+0)*($ww[$i-1]>0 ? $ww[$i-1] : 1);
                    $r1.=str_replace('%num%',$i,$tpl['cabinet_result']['result_ttl_cell']);
                    $r2.=str_replace('%result%',$w[$i-1]+0,$tpl['cabinet_result']['result_val_cell']);
                    //$r2.=str_replace('%result%',$result_symbols2[$scales[$row['exam']][$w[$i-1]]],$tpl['cabinet_result']['result_val_cell']);
                    $r3.=str_replace('%result%',$wball,$tpl['cabinet_result']['result_val_cell']);
                }
                $task_results=str_replace(array('%row1%','%row2%','%row3%'),array($r1,$r2,$r3),$tpl['cabinet_result']['result_table_1'] ? $tpl['cabinet_result']['result_table_1'] : $tpl['cabinet_result']['result_table']);
            }

            switch ($row['results_ready'])
            {
            case 'Y':    // Готовы окончательные результаты
                {    
                    if ($row['printed_z']=='A') 
                    {
                        if ($row['result']>0)
                        {    $content[$row['exam']].=str_replace(
                                array('%exam%','%place%','%exam_dt%','%task_results%','%form%','%result%'),
                                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном',$row['result']),
                                $tpl['cabinet_result']['ready_'.$row['rank'].'_'.$row['exam']] ? $tpl['cabinet_result']['ready_'.$row['rank'].'_'.$row['exam']] : $tpl['cabinet_result']['ready_'.$row['rank']]);
                            
                            //echo $content[$row['exam']].= 'ready_'.$row['rank'].'_'.$row['exam'].' || '.'ready_'.$row['rank'];
                            
                            break;
                        }
                    }


                    else{
                        $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном'),
                            $tpl['cabinet_result']['noaccess_p']);
                        break;
                    }
                }
            case 'P':    // Готовы предварительные результаты
                {    
                    if ($row['printed_z']=='A') 
                    {
                        if ($row['task_results']!='')
                        {    
                            $content_apk=str_replace(array(),array(),$tpl['cabinet_result']['result_apk']);
                            $scan_ref=str_replace(array('%pid%','%exam%'),array($USER_INFO['pid'],$exam_letter[$row['exam']]),$tpl['cabinet_result']['2result_scan']);
                            $content[$row['exam']].=str_replace(
                                array('%exam%','%place%','%exam_dt%','%task_results%','%form%','%result%','%scan%'),
                                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном',$row['result'], $scan_ref),
                                $tpl['cabinet_result']['ready_p_'.$row['exam']] ? $tpl['cabinet_result']['ready_p_'.$row['exam']] : $tpl['cabinet_result']['ready_p']);
                                
                            break;
                                                
                        }
                    }


                    else{
                        $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном'),
                            $tpl['cabinet_result']['noaccess_p']);
                        break;
                    }
                }
            case 'N':    // Результаты не готовы
                {    switch ($row['form'])
                    {
                    case 1:    // Очная форма
                        {    $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_result']['progress_o']);
                        break;
                        }
                    case 2:    // Заочная форма
                        {    $content[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                            $tpl['cabinet_result']['progress_z']);
                        break;
                        }
                    }
                    break;
                }
            }
            $registered[$row['exam']]=true;
            break;
        }
    }
}

foreach ($registered as $k=>$w)
{    if (!$w)    $content[$k].=str_replace(
                            array('%exam%'),
                            array($olimp_exams[$k]['name']),
                            $tpl['cabinet_result']['unregistered']);
}

$content_all='';

foreach ($content as $k=>$w) $content_all.=str_replace(array('%exam%','%result%'),array($olimp_exams[$k]['name'],$w),$tpl['cabinet_result']['exam_result']);
$content_all.=$content_apk;


// Тренировочный этап

$res=$mysqli->query('select a.presence as presence, a.result as result, a.task_results as task_results, a.`rank` as rank, s.exam as exam, s.form as form, s.place as place, s.task_count as task_count, s.results_ready as results_ready, DATE_FORMAT(s.date_exam,"%d.%m.%Y %H:%i") as exam_dt, DATE_FORMAT(s.date_post,"%d.%m.%Y %H:%i") as post_dt, DATE_FORMAT(s.date_exam,"%d.%m.%Y") as exam_sdt, DATE_FORMAT(s.date_post,"%d.%m.%Y") as post_sdt from olimp_actions as a left join olimp_stages as s on (s.id=a.stage_id) where (s.stage=3) AND (a.person_id='.$USER_INFO['pid'].') order by s.exam, s.date_exam');
while ($res && $row=$res->fetch_assoc())
{    switch ($row['presence'])
    {
    case 'R':
    case 'L':
        {    switch ($row['form'])
            {
            case 1:    // Зарегистрирован на очную форму
                {    $training[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['3registered_o']);
                    $registered[$row['exam']]=true;
                    break;
                }
            case 2:    // Зарегистрирован на заочную форму
                {    $training[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['address'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_result']['3registered_z']);
                    $registered[$row['exam']]=true;
                    break;
                }
            }
            break;
        }
    case 'N':
        {    switch ($row['form'])
            {
            case 1:    // Неявка на очную форму
                {    $training[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                        $tpl['cabinet_result']['3unpresent_o']);
                    break;
                }
            case 2:    // Неявка на заочную форму
                {    $training[$row['exam']].=str_replace(
                        array('%exam%','%place%','%exam_dt%','%post_dt%'),
                        array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                        $tpl['cabinet_result']['3unpresent_z']);
                    break;
                }
            }
            break;
        }
    case 'W':     // Ожидание письма
        {    $training[$row['exam']].=str_replace(
                array('%exam%','%place%','%exam_dt%','%post_dt%'),
                array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                $tpl['cabinet_result']['3waiting']);
            $registered[$row['exam']]=true;
            break;
        }
    case 'A':     // Работа аннулирована
        {    switch ($row['form'])
            {
            case 1:
                {    $training[$row['exam']].=str_replace(
                    array('%exam%','%place%','%exam_dt%'),
                    array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                    $tpl['cabinet_result']['3annulated_o']);
                    break;
                }
            case 2:
                {    $training[$row['exam']].=str_replace(
                    array('%exam%','%place%','%exam_dt%','%post_dt%'),
                    array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                    $tpl['cabinet_result']['3annulated_z']);
                    break;
                }
            }
            $registered[$row['exam']]=true;
            break;
        }
    case 'Y':
        {    $task_results='';
            if ($row['task_results']!='')
            {    $w=explode(',',$row['task_results']);
                $r1='';
                $r2='';
                for ($i=1;$i<=$row['task_count'];$i++)
                {    $r1.=str_replace('%num%',$i,$tpl['cabinet_result']['result_ttl_cell']);
                    $r2.=str_replace('%result%',$result_symbols[$scales[$row['exam']][$w[$i-1]]],$tpl['cabinet_result']['result_val_cell']);
                }
                $task_results=str_replace(array('%row1%','%row2%'),array($r1,$r2),$tpl['cabinet_result']['result_table']);
            }
            switch ($row['results_ready'])
            {
            case 'Y':    // Готовы окончательные результаты
                {    if ($row['result']>0)
                    {    $training[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%','%result%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном',$row['result']),
                            $tpl['cabinet_result']['3ready_'.$row['rank']]);
                        break;
                    }
                }
            case 'P':    // Готовы предварительные результаты
                {    if ($row['task_results']!='')
                    {    $training[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%task_results%','%form%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['form']==1 ? $row['exam_dt'] : $row['exam_sdt'].' - '.$row['post_sdt'],$task_results,$row['form']==1 ? 'очном' : 'заочном'),
                            $tpl['cabinet_result']['3ready_p']);
                        break;
                    }
                }
            case 'N':    // Результаты не готовы
                {    switch ($row['form'])
                    {
                    case 1:    // Очная форма
                        {    $training[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt']),
                            $tpl['cabinet_result']['3progress_o']);
                        break;
                        }
                    case 2:    // Заочная форма
                        {    $training[$row['exam']].=str_replace(
                            array('%exam%','%place%','%exam_dt%','%post_dt%'),
                            array($olimp_exams[$row['exam']]['name'],$olimp_places[$row['place']]['name'],$row['exam_dt'],$row['post_dt']),
                            $tpl['cabinet_result']['3progress_z']);
                        break;
                        }
                    }
                    break;
                }
            }
            $registered[$row['exam']]=true;
            break;
        }
    }
}

$training_all='';
foreach ($training as $k=>$w) if ($w) $training_all.=str_replace(array('%exam%','%result%'),array($olimp_exams[$k]['name'],$w),$tpl['cabinet_result']['exam_result']);
if (!empty($training_all)) $training_all=str_replace('%results%',$training_all,$tpl['cabinet_result']['training']);

// Формирование страницы

if (!empty($error)) $error=preg_replace('/%error%/',preg_replace(array('/%title%/','/%text%/'),array('Ошибка!',$error),$tpl['common']['error']),$tpl['cabinet']['error']);
if (!empty($warning)) $warning=preg_replace('/%warning%/',preg_replace(array('/%title%/','/%text%/'),array('Внимание!',$warning),
        $tpl['common']['highlight']),$tpl['cabinet']['warning']);

$body=preg_replace(
            array('/%error%/','/%warning%/','/%message%/','/%results%/','/%final%/','/%training%/'),
            array($error,$warning,$message,$content_all,$final_results,$training_all),
            $tpl['cabinet_result']['main']);

return $body;
}

?>