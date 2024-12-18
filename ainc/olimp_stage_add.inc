<?php

if (($role & 32) == 0) {
    return $tpl['noaccess'];
}

if (isset($_POST['sent'])) {
    foreach ($_POST as $key => $value) {
        $$key = addslashes($value);
    }

    $query = "insert into olimp_stages(`exam`, `classes`, `place`, `limit`, `regs`, `year`, `stage`, `form`, `date_breg`, `date_ereg`, `date_exam`, `date_post`, `grp_prefix`, `grp_limit`, `next_group`, `var_prefix`, `min_var`, `max_var`, `descr`, `task_count`, `results_ready`, `cipher`, `task_weight`, `use_person_result_text`, `stage_result_text` )
                values('$exam', '$classes', '$place', '$limit', '$regs', '$year', '$stage', '$form', '$date_breg', '$date_ereg', '$date_exam', '$date_post', '$grp_prefix', '$grp_limit', '$next_group', '$var_prefix', '$min_var', '$max_var', '$descr', '$task_count', '$results_ready', '$cipher', '$task_weight', '$use_person_result_text', '$stage_result_text')";
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
$body = '
<div id="newPotok">
<form method="post">
    <input type="hidden" name="sent" value="1">
    
    <div class="line">
        <label>Предмет:</label>
        <select name="exam">';
$body .= '<option value=0>Выберите предмет</option>';
foreach ($lists['olimp_exams'] as $id => $name) {
    $body .= '<option value=' . $id . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Классы:</label>
        <input type="text" name="classes">';
$body .= '
    </div>
    
    <div class="line">
        <label>Площадка:</label>
        <select name="place">';
$body .= '<option value=0>Выберите площадку</option>';
foreach ($lists['olimp_places'] as $id => $name) {
    $body .= "<option value=$id>$name</option>";
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Лимит:</label>
        <input type="number" name="limit" value="0" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>regs:</label>
        <input type="number" name="regs" value="0" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>Год:</label>
        <input type="number" name="year" value="' . date('Y') . '" min=' . date('Y') . '>';
$body .= '
    </div>
    
    <div class="line">
        <label>Этап:</label>
        <select name="stage">';
$body .= '<option value=0>Выберите этап</option>';
foreach ($lists['olimp_stage_types'] as $id => $name) {
    $body .= '<option value=' . $id . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>Форма:</label>
        <select name="form">';
$body .= '<option value=0>Выберите форму</option>';
foreach ($lists['olimp_forms'] as $id => $name) {
    $body .= '<option value=' . $id . '>' . $name . '</option>';
}

$body .= '
        </select>
    </div>
    
    <div class="line">
        <label>date_breg:</label>
        <input type="text" name="date_breg" placeholder="YYYY-mm-dd" value="' . date('Y-m-d') . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>date_ereg:</label>
        <input type="text" name="date_ereg" placeholder="YYYY-mm-dd HH:ii:ss" value="' . date('Y-m-d H:i:s') . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>date_exam:</label>
        <input type="text" name="date_exam" placeholder="YYYY-mm-dd HH:ii:ss" value="' . date('Y-m-d H:i:s') . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>date_post:</label>
        <input type="text" name="date_post" placeholder="YYYY-mm-dd HH:ii:ss" value="' . date('Y-m-d H:i:s') . '">';
$body .= '
    </div>
    
    <div class="line">
        <label>grp_prefix:</label>
        <input type="text" name="grp_prefix">';
$body .= '
    </div>
    
    <div class="line">
        <label>grp_limit:</label>
        <input type="number" name="grp_limit" value="25" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>next_group:</label>
        <input type="number" name="next_group" value="1" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>var_prefix:</label>
        <input type="text" name="var_prefix">';
$body .= '
    </div>
    
    <div class="line">
        <label>min_var:</label>
        <input type="number" name="min_var" value="0" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>max_var:</label>
        <input type="number" name="max_var" value="0" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>descr:</label>
        <input type="text" name="descr">';
$body .= '
    </div>
    
    <div class="line">
        <label>task_count:</label>
        <input type="number" name="task_count" value="0" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>results_ready:</label>
        <select name="results_ready">
            <option value="N">N</option>
            <option value="Y">Y</option>
        </select>';
$body .= '
    </div>
    
    <div class="line">
        <label>cipher:</label>
        <input type="text" name="cipher">';
$body .= '
    </div>
    
    <div class="line">
        <label>task_weight:</label>
        <input type="text" name="task_weight">';
$body .= '
    </div>
    
    <div class="line">
        <label>use_person_result_text:</label>
        <input type="number" name="use_person_result_text" value="0" min=0>';
$body .= '
    </div>
    
    <div class="line">
        <label>stage_result_text:</label>
        <input type="text" name="stage_result_text">';
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
