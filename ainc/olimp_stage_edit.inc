<?php

if (($role & 32) == 0) {
    return $tpl['noaccess'];
}

if (!$id = (int)$_GET['id']) {
    header('location:/olimp_stages.html');
    exit;
}

$query = "select * from olimp_stages where id = $id";
$res = $mysqli->query($query);
if (!$mystage = $res->fetch_assoc()) {
    header('location:/olimp_stages.html');
    exit;
}

if (isset($_POST['sent'])) {
    foreach ($_POST as $key => $value) {
        $$key = addslashes($value);
    }

    $query = "update olimp_stages set `exam`='$exam', `classes`='$classes', `place`='$place', `limit`='$limit', `regs`='$regs', `year`='$year', `stage`='$stage', `form`='$form', `date_breg`='$date_breg', `date_ereg`='$date_ereg', `date_exam`='$date_exam', `date_post`='$date_post', `grp_prefix`='$grp_prefix', `grp_limit`='$grp_limit', `next_group`='$next_group', `var_prefix`='$var_prefix', `min_var`='$min_var', `max_var`='$max_var', `descr`='$descr', `task_count`='$task_count', `results_ready`='$results_ready', `cipher`='$cipher', `task_weight`='$task_weight', `use_person_result_text`='$use_person_result_text', `stage_result_text`='$stage_result_text' where id = $id";
    $mysqli->query($query);
    header('location:/olimp_stages.html');
    exit;
}

$lists = array();
$res = $mysqli->query('select id, name from olimp_places order by name asc');
while ($row = $res->fetch_assoc()) {
    $lists['olimp_places'][$row['id']] = $row['name'];
}

$res = $mysqli->query('select id, name from olimp_exams order by name asc');
while ($row = $res->fetch_assoc()) {
    $lists['olimp_exams'][$row['id']] = $row['name'];
}

$lists['olimp_presence'][1] = 'Явившиеся (приславшие работу)';
$lists['olimp_presence'][2] = 'Не явившиеся (отказавшиеся)';
$lists['olimp_presence'][3] = 'Скачавшие вариант';
$lists['olimp_presence'][4] = 'Не скачавшие вариант';
$res = $mysqli->query('select id, name from olimp_forms order by name asc');
while ($row = $res->fetch_assoc()) {
    $lists['olimp_forms'][$row['id']] = $row['name'];
}

$res = $mysqli->query('select id, name from olimp_stage_types order by name asc');
while ($row = $res->fetch_assoc()) {
    $lists['olimp_stage_types'][$row['id']] = $row['name'];
}

$lists['classes'][5] = '5';
$lists['classes'][6] = '6';
$lists['classes'][7] = '7';
$lists['classes'][8] = '8';
$lists['classes'][9] = '9';
$lists['classes'][10] = '10';
$lists['classes'][11] = '11';
var_dump($lists);
exit;
$body = '
<div id="newPotok">
<form method="post">
    <input type="hidden" name="sent" value="1">
    
    <div class="line">
        <label>Предмет:</label>
        <select name="exam">';
$body .= '<option value=0>Выберите предмет</option>';
foreach ($lists['olimp_exams'] as $id => $name) {
    if ($mystage['exam'] == $id) {
        $selected = 'selected="true"';
    } else {
        $selected = '';
    }
    $body .= '<option value=' . $id . ' ' . $selected . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Классы:</label>
        <input type="text" name="classes" value="' . $mystage['classes'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>Площадка:</label>
        <select name="place">';
$body .= '<option value=0>Выберите площадку</option>';
foreach ($lists['olimp_places'] as $id => $name) {
    if ($mystage['place'] == $id) {
        $selected = 'selected="true"';
    } else {
        $selected = '';
    }
    $body .= '<option value=' . $id . ' ' . $selected . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Лимит:</label>
        <input type="number" name="limit"  value="' . $mystage['limit'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>regs:</label>
        <input type="number" name="regs"  value="' . $mystage['regs'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>Год:</label>
        <input type="number" name="year"  value="' . $mystage['year'] . '" min=' . date('Y') . '>';
$body .= '
    </div>
    
    <div class="line">
        <label>Этап:</label>
        <select name="stage">';
$body .= '<option value=0>Выберите этап</option>';
foreach ($lists['olimp_stage_types'] as $id => $name) {
    if ($mystage['stage'] == $id) {
        $selected = 'selected="true"';
    } else {
        $selected = '';
    }
    $body .= '<option value=' . $id . ' ' . $selected . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Форма:</label>
        <select name="form">';
$body .= '<option value=0>Выберите форму</option>';
foreach ($lists['olimp_forms'] as $id => $name) {
    if ($mystage['form'] == $id) {
        $selected = 'selected="true"';
    } else {
        $selected = '';
    }
    $body .= '<option value=' . $id . ' ' . $selected . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>date_breg:</label>
        <input type="text" name="date_breg" placeholder="YYYY-mm-dd"  value="' . $mystage['date_breg'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>date_ereg:</label>
        <input type="text" name="date_ereg" placeholder="YYYY-mm-dd HH:ii:ss"  value="' . $mystage['date_ereg'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>date_exam:</label>
        <input type="text" name="date_exam" placeholder="YYYY-mm-dd HH:ii:ss"  value="' . $mystage['date_exam'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>date_post:</label>
        <input type="text" name="date_post" placeholder="YYYY-mm-dd HH:ii:ss"  value="' . $mystage['date_post'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>grp_prefix:</label>
        <input type="text" name="grp_prefix" value="' . $mystage['grp_prefix'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>grp_limit:</label>
        <input type="number" name="grp_limit"  value="' . $mystage['grp_limit'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>next_group:</label>
        <input type="number" name="next_group"  value="' . $mystage['next_group'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>var_prefix:</label>
        <input type="text" name="var_prefix" value="' . $mystage['var_prefix'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>min_var:</label>
        <input type="number" name="min_var" value="' . $mystage['min_var'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>max_var:</label>
        <input type="number" name="max_var"  value="' . $mystage['max_var'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>descr:</label>
        <input type="text" name="descr" value="' . $mystage['descr'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>task_count:</label>
        <input type="number" name="task_count"  value="' . $mystage['task_count'] . '" min=0>';
if ($mystage['results_ready'] == 'N') {
    $selected1 = ' selected';
    $selected2 = '';
    $selected3 = '';
}
if ($mystage['results_ready'] == 'Y') {
    $selected1 = '';
    $selected2 = ' selected';
    $selected3 = '';
}
if ($mystage['results_ready'] == 'P') {
    $selected1 = '';
    $selected2 = '';
    $selected3 = ' selected';
}
$body .= '
    </div>
    
    <div class="line">
        <label>results_ready:</label>
        <select name="results_ready">
            <option value="N" ' . $selected1 . '>N</option>
            <option value="Y" ' . $selected2 . '>Y</option>
            <option value="P" ' . $selected3 . '>P</option>
        </select>';
$body .= '
    </div>
    
    <div class="line">
        <label>cipher:</label>
        <input type="text" name="cipher" value="' . $mystage['cipher'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>task_weight:</label>
        <input type="text" name="task_weight" value="' . $mystage['task_weight'] . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>use_person_result_text:</label>
        <input type="number" name="use_person_result_text"  value="' . $mystage['use_person_result_text'] . '" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>stage_result_text:</label>
        <input type="text" name="stage_result_text" value="' . $mystage['stage_result_text'] . '">';
$body .= '<!--
    </div>
    
    <div class="line">
        <label>Участники:</label>
        <select>';
$body .= '<option value=0>Выберите участников</option>';
foreach ($lists['olimp_presence'] as $id => $name) {
    $body .= '<option value=' . $id . '>' . $name . '</option>';
}



$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Дата проведения:</label>
        <input type="text" />
    </div>-->
    
    <div class="line">
        <label></label>
        <button>Сохранить</button>
    </div>


</form>
</div>';
