<?
if (($role & 32)==0)
{    return $tpl['noaccess'];
}
if(isset($_POST['sent'])){
    print_r($_POST);
    
    foreach($_POST['exam'] as $key => $x){
        $exam = (int)$_POST['exam'][$key];
        $classes = addslashes($_POST['classes'][$key]);
        $place = (int)$_POST['place'][$key];
        $limit = (int)$_POST['limit'][$key];
        $year = (int)$_POST['year'][$key];
        $stage = (int)$_POST['stage'][$key];
        $form = (int)$_POST['form'][$key];
        $date_breg = addslashes($_POST['date_breg'][$key]);
        $date_ereg = addslashes($_POST['date_ereg'][$key].' '.$_POST['date_ereg2'][$key]);
        $date_exam = addslashes($_POST['date_exam'][$key].' '.$_POST['date_exam2'][$key]);
        $date_post = addslashes($_POST['date_post'][$key].' '.$_POST['date_post2'][$key]);
        $grp_prefix = addslashes($_POST['grp_prefix'][$key]);
        $grp_limit = (int)$_POST['grp_limit'][$key];
        $next_group = (int)$_POST['next_group'][$key];
        $var_prefix = addslashes($_POST['var_prefix'][$key]);
        $min_var = (int)$_POST['min_var'][$key];
        $max_var = (int)$_POST['max_var'][$key];
        $descr = addslashes($_POST['descr'][$key]);
        $task_count = (int)$_POST['task_count'][$key];
        $results_ready = addslashes($_POST['results_ready'][$key]);
        $cipher = addslashes($_POST['cipher'][$key]);
        $task_weight = addslashes($_POST['task_weight'][$key]);
        $use_person_result_text = (int)$_POST['use_person_result_text'][$key];
        $stage_result_text = addslashes($_POST['stage_result_text'][$key]);
        
        $query = "update olimp_stages set `exam`='$exam', `classes`='$classes', `place`='$place', `limit`='$limit', `year`='$year', `stage`='$stage', `form`='$form', `date_breg`='$date_breg', `date_ereg`='$date_ereg', `date_exam`='$date_exam', `date_post`='$date_post', `grp_prefix`='$grp_prefix', `grp_limit`='$grp_limit', `next_group`='$next_group', `var_prefix`='$var_prefix', `min_var`='$min_var', `max_var`='$max_var', `descr`='$descr', `task_count`='$task_count', `results_ready`='$results_ready', `cipher`='$cipher', `task_weight`='$task_weight', `use_person_result_text`='$use_person_result_text', `stage_result_text`='$stage_result_text' where id=$key";
        $mysqli->query($query);
    }
    
    
    
    header('location:/olimp_stages.html');
    exit;
}

$lists = array();

$ids = array();
foreach($_GET['ids'] as $id){
    $ids[] = $id;
}
$ids = implode(',', $ids);
$res = $mysqli->query('select * from olimp_stages where id in('.$ids.')');
while ($row = $res->fetch_assoc()){    
    $lists['olimp_stages'][$row['id']] = $row;
}

$res = $mysqli->query('select id, name from olimp_places order by name asc');
while ($row = $res->fetch_assoc()){    
    $lists['olimp_places'][$row['id']] = $row['name'];
}

$res = $mysqli->query('select id, name from olimp_exams order by name asc');
while ($row = $res->fetch_assoc()){
    $lists['olimp_exams'][$row['id']] = $row['name'];
}

$lists['olimp_presence'][1] = 'Явившиеся (приславшие работу)';
$lists['olimp_presence'][2] = 'Не явившиеся (отказавшиеся)';
$lists['olimp_presence'][3] = 'Скачавшие вариант';
$lists['olimp_presence'][4] = 'Не скачавшие вариант';


$res = $mysqli->query('select id, name from olimp_forms order by name asc');
while ($row = $res->fetch_assoc()){
    $lists['olimp_forms'][$row['id']] = $row['name'];
}

$res = $mysqli->query('select id, name from olimp_stage_types order by name asc');
while ($row = $res->fetch_assoc()){
    $lists['olimp_stage_types'][$row['id']] = $row['name'];
}

$lists['classes'][5] = '5';
$lists['classes'][6] = '6';
$lists['classes'][7] = '7';
$lists['classes'][8] = '8';
$lists['classes'][9] = '9';
$lists['classes'][10] = '10';
$lists['classes'][11] = '11';

$body = '
    <form method="post">
    <input type="hidden" name="sent" value="1">
    <table class="scool_list">
                <thead>
                    <tr>
                        <td>
                            Предмет
                        </td>
                        <td>
                            Классы
                        </td>
                        <td>
                            Площадка
                        </td>
                        <td>
                            Лимит
                        </td>
                        <td>
                            Год
                        </td>
                        <td>
                            Этап
                        </td>
                        <td>
                            Форма
                        </td>
                        <td>
                            Дата начала рег-ции
                        </td>
                        <td>
                            Дата завершения рег-ции
                        </td>
                        <td>
                            Время завершения рег-ции
                        </td>
                        <td>
                            Дата экзамена
                        </td>
                        <td>
                            Время экзамена
                        </td>
                        <td>
                            Дата публикации
                        </td>
                        <td>
                            Время публикации
                        </td>
                        <td>
                            Префикс
                        </td>
                        <td>
                            Лимит
                        </td>
                        <td>
                            next group
                        </td>
                        <td>
                            var prefix
                        </td>
                        <td>
                            min var
                        </td>
                        <td>
                            max var
                        </td>
                        <td>
                            Описание
                        </td>
                        <td>
                            Кол-во задач
                        </td>
                        <td>
                            results_ready
                        </td>
                        <td>
                            cipher
                        </td>
                        <td>
                            task weight
                        </td>
                        <td>
                            use person result text
                        </td>
                        <td>
                            stage result text
                        </td>
                        
                    </tr>
                </thead>
            <tbody>';
    
    
    foreach($lists['olimp_stages'] as $stageId=>$stageData){
        $body .= '
                    <tr>
                        <td>
                            <select data-name="exam" name="exam['.$stageId.']">';
                            
        foreach($lists['olimp_exams'] as $id => $name){
            if($stageData['exam'] == $id) $sel = 'selected="true"'; else $sel = '';
            $body .= '<option value="'.$id.'" '.$sel.'>'.$name.'</option>';
        }
        
        $body .= '
                            </select><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="classes" name="classes['.$stageId.']" value="'.$stageData['classes'].'" style="width:90px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <select data-name="place" name="place['.$stageId.']" style="width:150px;">';
                            
        foreach($lists['olimp_places'] as $id => $name){
            if($stageData['place'] == $id) $sel = 'selected="true"'; else $sel = '';
            $body .= '<option value="'.$id.'" '.$sel.'>'.$name.'</option>';
        }
        
        $body .= '
                            </select><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="limit" name="limit['.$stageId.']" value="'.$stageData['limit'].'" style="width:50px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="year" name="year['.$stageId.']" value="'.$stageData['year'].'" style="width:55px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <select data-name="stage" name="stage['.$stageId.']" style="width:90px;">';
                            
        foreach($lists['olimp_stage_types'] as $id => $name){
            if($stageData['stage'] == $id) $sel = 'selected="true"'; else $sel = '';
            $body .= '<option value="'.$id.'" '.$sel.'>'.$name.'</option>';
        }
        
        $body .= '
                            </select><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <select data-name="form" name="form['.$stageId.']" style="width:90px;">';
        $date_ereg = explode(' ', $stageData['date_ereg']);
        $date_exam = explode(' ', $stageData['date_exam']);
        if($stageData['date_post'] == '0000-00-00 00:00:00') $stageData['date_post'] = date('Y-m-d H:i:s');
        $date_post = explode(' ', $stageData['date_post']);
        $Y = ''; $N = '';
        $$stageData['results_ready']= 'selected="true"';  
                        
        foreach($lists['olimp_forms'] as $id => $name){
            if($stageData['form'] == $id) $sel = 'selected="true"'; else $sel = '';
            $body .= '<option value="'.$id.'" '.$sel.'>'.$name.'</option>';
        }
        
        $body .= '
                            </select><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td><input type="text" data-name="date_breg" name="date_breg['.$stageId.']" data-date="'.$stageData['date_breg'].'" style="width:80px;" class="calendar1_'.$stageId.'" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="date_ereg" name="date_ereg['.$stageId.']" data-date="'.$date_ereg[0].'" style="width:80px;" class="calendar2_'.$stageId.'" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="date_ereg2" name="date_ereg2['.$stageId.']" value="'.$date_ereg[1].'" style="width:60px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="date_exam" name="date_exam['.$stageId.']" data-date="'.$date_exam[0].'" style="width:80px;" class="calendar3_'.$stageId.'" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="date_exam2" name="date_exam2['.$stageId.']" value="'.$date_exam[1].'" style="width:60px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="date_post" name="date_post['.$stageId.']" data-date="'.$date_post[0].'" style="width:80px;" class="calendar4_'.$stageId.'" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="date_post2" name="date_post2['.$stageId.']" value="'.$date_post[1].'" style="width:60px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="grp_prefix" name="grp_prefix['.$stageId.']" value="'.$stageData['grp_prefix'].'" style="width:60px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="grp_limit" name="grp_limit['.$stageId.']" value="'.$stageData['grp_limit'].'" style="width:40px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="next_group" name="next_group['.$stageId.']" value="'.$stageData['next_group'].'" style="width:40px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="var_prefix" name="var_prefix['.$stageId.']" value="'.$stageData['var_prefix'].'" style="width:60px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="min_var" name="min_var['.$stageId.']" value="'.$stageData['min_var'].'" style="width:40px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="max_var" name="max_var['.$stageId.']" value="'.$stageData['max_var'].'" style="width:40px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="descr" name="descr['.$stageId.']" value="'.$stageData['descr'].'" style="width:100px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="task_count" name="task_count['.$stageId.']" value="'.$stageData['task_count'].'" style="width:40px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <select data-name="results_ready" name="results_ready['.$stageId.']" style="width:90px;">
                                <option value="Y" '.$Y.'>Y</option>
                                <option value="N" '.$N.'>N</option>
                            </select><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="cipher" name="cipher['.$stageId.']" value="'.$stageData['cipher'].'" style="width:70px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="task_weight" name="task_weight['.$stageId.']" value="'.$stageData['task_weight'].'" style="width:70px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="number" data-name="use_person_result_text" name="use_person_result_text['.$stageId.']" value="'.$stageData['use_person_result_text'].'" style="width:40px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        <td>
                            <input type="text" data-name="stage_result_text" name="stage_result_text['.$stageId.']" value="'.$stageData['stage_result_text'].'" style="width:70px;" ><img src="/copy.png" class="copy" style="height:20px; margin-left:3px; display:inline-block; vertical-align:top;">
                        </td>
                        
                    </tr>
                    <script>
                        addEventListener(\'DOMContentLoaded\', function () {
                            pickmeup(\'.calendar1_'.$stageId.'\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar1_'.$stageId.'\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar2_'.$stageId.'\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar2_'.$stageId.'\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar3_'.$stageId.'\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar3_'.$stageId.'\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar4_'.$stageId.'\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar4_'.$stageId.'\').attr(\'data-date\')),
                                ],
                            });
                        });
                    </script>
        ';
    }
    
    
    $body .= '</tbody></table><br /><button>Сохранить</button></form><style>tbody tr td{white-space:nowrap}</style>';
    
/*
$body = '
<div id="newPotok">
<form method="post">
    <input type="hidden" name="sent" value="1">
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="exam"> Предмет:</label>
        <select name="exam">';
        $body .= '<option value=0>Выберите предмет</option>';
        foreach($lists['olimp_exams'] as $id => $name){
            $body .= '<option value='.$id.'>'.$name.'</option>';
        }
        
$body .= '
        </select>
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="classes"> Классы:</label>
        <input type="text" name="classes">';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="place"> Площадка:</label>
        <select name="place">';
        $body .= '<option value=0>Выберите площадку</option>';
        foreach($lists['olimp_places'] as $id => $name){
            $body .= '<option value='.$id.'>'.$name.'</option>';
        }
        
$body .= '
        </select>
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="limit"> Лимит:</label>
        <input type="number" name="limit" min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="regs"> regs:</label>
        <input type="number" name="regs" min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="year"> Год:</label>
        <input type="number" name="year" min='.date('Y').'>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="stage"> Этап:</label>
        <select name="stage">';
        $body .= '<option value=0>Выберите этап</option>';
        foreach($lists['olimp_stage_types'] as $id => $name){
            $body .= '<option value='.$id.'>'.$name.'</option>';
        }
        
$body .= '
        </select>
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="exam"> form:</label>
        <select name="form">';
        $body .= '<option value=0>Выберите форму</option>';
        foreach($lists['olimp_forms'] as $id => $name){
            $body .= '<option value='.$id.'>'.$name.'</option>';
        }
        
$body .= '
        </select>
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="date_breg"> date_breg:</label>
        <input type="text" name="date_breg" placeholder="YYYY-mm-dd" >';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="date_ereg"> date_ereg:</label>
        <input type="text" name="date_ereg" placeholder="YYYY-mm-dd HH:ii:ss">';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="date_exam"> date_exam:</label>
        <input type="text" name="date_exam" placeholder="YYYY-mm-dd HH:ii:ss">';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="date_post"> date_post:</label>
        <input type="text" name="date_post" placeholder="YYYY-mm-dd HH:ii:ss" >';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="grp_prefix"> grp_prefix:</label>
        <input type="text" name="grp_prefix">';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="grp_limit"> grp_limit:</label>
        <input type="number" name="grp_limit"  min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="next_group"> next_group:</label>
        <input type="number" name="next_group"  min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="var_prefix"> var_prefix:</label>
        <input type="text" name="var_prefix" >';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="min_var"> min_var:</label>
        <input type="number" name="min_var" min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="max_var"> max_var:</label>
        <input type="number" name="max_var" min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="descr"> descr:</label>
        <input type="text" name="descr" >';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="task_count"> task_count:</label>
        <input type="number" name="task_count" min=0>';

$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="results_ready"> results_ready:</label>
        <select name="results_ready">
            <option value="N">N</option>
            <option value="Y">Y</option>
        </select>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="cipher"> cipher:</label>
        <input type="text" name="cipher"';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="task_weight"> task_weight:</label>
        <input type="text" name="task_weight">';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="use_person_result_text"> use_person_result_text:</label>
        <input type="number" name="use_person_result_text"  min=0>';
        
$body .= '
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="stage_result_text"> stage_result_text:</label>
        <input type="text" name="stage_result_text" >';
        
        
$body .= '<!--
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="exam"> Участники:</label>
        <select>';
        $body .= '<option value=0>Выберите участников</option>';
        foreach($lists['olimp_presence'] as $id => $name){
            $body .= '<option value='.$id.'>'.$name.'</option>';
        }



$body .= '
        </select>
    </div>
    
    <div class="line">
        <label><input type="checkbox" name="sel[]" value="exam"> Дата проведения:</label>
        <input type="text" />
    </div>-->
    
    <div class="line">
        <label></label>
        <button>Сохранить</button>
    </div>


</form>
</div>';

*/