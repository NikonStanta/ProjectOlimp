<?php

include_once('olimp_common.inc');

    if (($role & 32) == 0) {
        return $tpl['noaccess'];
    }

    if (isset($_POST['sent']) and $_POST['sent'] == 1) {
        $exams = array();
        $query = "select * from olimp_exams order by name asc";
        $res = $mysqli->query($query);
        while ($row = $res->fetch_assoc()) {
            $exams[$row['id']] = $row['name'];
        }

        $places = array();
        $query = "select * from olimp_places order by name asc";
        $res = $mysqli->query($query);
        while ($row = $res->fetch_assoc()) {
            $places[$row['id']] = $row['name'];
        }

        $stages = array();
        $query = "select * from olimp_stage_types order by name asc";
        $res = $mysqli->query($query);
        while ($row = $res->fetch_assoc()) {
            $stages[$row['id']] = $row['name'];
            if ($row['note']) {
                $stages[$row['id']] .= ' - ' . $row['note'];
            }
        }

        $forms = array();
        $query = "select * from olimp_forms order by name asc";
        $res = $mysqli->query($query);
        while ($row = $res->fetch_assoc()) {
            $forms[$row['id']] = $row['name'];
        }


        $date_exam = addslashes($_POST['date_exam']);
        if (!is_numeric(strtotime($date_exam))) {
            header('location:/olimp_stages.html');
            exit;
        }

        $date_ereg = addslashes($_POST['date_ereg']);
        if (!is_numeric(strtotime($date_ereg))) {
            header('location:/olimp_stages.html');
            exit;
        }

        $form = (int)($_POST['form']);
        if (!$form) {
            header('location:/olimp_stages.html');
            exit;
        }

        if (!isset($_POST['class']) or !is_array($_POST['class']) or !count($_POST['class'])) {
            header('location:/olimp_stages.html');
            exit;
        }

        $data = array();
        foreach ($_POST['class'] as $one) {
            $one = explode('-', trim($one));
            $data[$one[0]][$one[1]][] = $one[2];
        }

        $body = '
    <form method="post">
    <input type="hidden" name="sent" value="2">
    <input type="hidden" name="form" value="' . $form . '">
    <table class="scool_list">
                <thead>
                    <tr>
                        <td>
                            Дата экзамена
                        </td>
                        <td>
                            Время экзамена
                        </td>
                        <td>
                            Дата завершения рег-ции
                        </td>
                        <td>
                            Время завершения рег-ции
                        </td>
                        <td>
                            Предмет
                        </td>
                        <td>
                            Школа
                        </td>
                        <td>
                            Класс
                        </td>
                        <td>
                            Год
                        </td>
                        <td>
                            Этап
                        </td>
                        <td>
                            Начало р-ции
                        </td>
    ';
        if ($form != 1) {
            $body .= '
                        <td>
                            Дата выведения р-тов
                        </td>
                        <td>
                            Время выведения р-тов
                        </td>
        ';
        }
        if ($form == 2) {
            $body .= '
                        <td>
                            var_prefix
                        </td>
                        <td>
                            min_var
                        </td>
                        <td>
                            max_var
                        </td>
        ';
        }
        $body .= '
                        <td>
                            Код группы
                        </td>
                        <td>
                            Кол-во человек
                        </td>
                        <td>
                            След. групп
                        </td>
                        <td>
                            Кол-во задач
                        </td>
                        <td>
                            Готовность р-тов
                        </td>
                        <td>
                            Вес задачи
                        </td>
                        <td>
                            Use person result text
                        </td>
                        <td>
                            Stage result text
                        </td>
                    </tr>
                </thead>
            <tbody>';
        $n = 0;
        foreach ($data as $exam => $data2) {
            foreach ($data2 as $place => $classes) {
                foreach ($classes as $class) {
                    $exam = addslashes($exam);
                    $place = addslashes($place);

                    $body .= '<tr>
                            <td>
                                <input name="date_exam[]" style="width:80px;" class="calendar4_' . $n . '" data-date="' . $date_exam . '">
                            </td>
                            <td>
                                <input name="date_exam2[]" style="width:60px;" value="10:00:00">
                            </td>
                            <td>
                                <input name="date_ereg[]" style="width:80px;" class="calendar5_' . $n . '" data-date="' . $date_ereg . '">
                            </td>
                            <td>
                                <input name="date_ereg2[]" style="width:60px;" value="09:00:00">
                            </td>
                            <td>
                                ' . $exams[$exam] . '
                                <input type="hidden" name="exam[]" value="' . $exam . '">
                            </td>
                            <td>
                                ' . $places[$place] . '
                                <input type="hidden" name="place[]" value="' . $place . '">
                            </td>
                            <td>
                                ' . $class . '
                                <input type="hidden" name="class[]" value="' . $class . '">
                            </td>
                            <td>
                                <input type="text" name="year[]" style="width:50px;" value=' . date('Y') . '>
                            </td>
                            <td>
                                <select name="stage[]" style="width:100px;">';
                    foreach ($stages as $id => $stage) {
                        if ($id == 1) {
                            $sel = 'selected="true"';
                        } else {
                            $sel = '';
                        }
                        $body .= '<option value="' . $id . '" ' . $sel . '>' . $stage . '</option>';
                    }
                    $body .= '      </select>
                            </td>
                            <td>
                                <input name="date_breg[]" style="width:80px;" class="calendar1_' . $n . '" data-date="' . date('Y-m-d') . '">
                            </td>
                            ';
                    if ($form != 1) {
                        $body .= '
                        <td>
                            <input name="date_post[]" style="width:80px;" class="calendar3_' . $n . '" data-date="' . date('Y-m-d') . '">
                        </td>
                        <td>
                            <input name="date_post2[]" style="width:60px;" value="10:00:00">
                        </td>
        ';
                    }
                    if ($form == 2) {
                        $body .= '
                        <td>
                            <input name="var_prefix[]" style="width:100px;">
                        </td>
                        <td>
                            <input name="min_var[]" type=number style="width:40px;" value="0">
                        </td>
                        <td>
                            <input name="max_var[]" type=number style="width:40px;" value="0">
                        </td>
        ';
                    }
                    $body .= '
                            <td>
                                <input name="grp_prefix[]" style="width:100px;">
                            </td>
                            <td>
                                <input name="grp_limit[]" type=number style="width:40px;" value="25">
                            </td>
                            <td>
                                <input name="next_group[]" type=number style="width:40px;" value="1">
                            </td>
                            <td>
                                <input name="task_count[]" type=number style="width:40px;" value="5">
                            </td>
                            <td>
                                <select name="results_ready[]" style="width:50px;">
                                    <option value="N">N</option>
                                    <option value="Y">Y</option>
                                </select>
                            </td>
                            <td>
                                <input name="task_weight[]" style="width:100px;">
                            </td>
                            <td>
                                <input name="use_person_result_text[]" type=number style="width:40px;" value="0">
                            </td>
                            <td>
                                <input name="stage_result_text[]" style="width:100px;">
                            </td>
                        </tr>
                        <script>
                            console.log(111);
                        addEventListener(\'DOMContentLoaded\', function () {
                            pickmeup(\'.calendar1_' . $n . '\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar1_' . $n . '\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar2_' . $n . '\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar2_' . $n . '\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar3_' . $n . '\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar3_' . $n . '\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar4_' . $n . '\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar4_' . $n . '\').attr(\'data-date\')),
                                ],
                            });
                            pickmeup(\'.calendar5_' . $n . '\', {
                                position       : \'bottom\',
                                hide_on_select : true,
                                format    : \'Y-m-d\',
                                date      : [
                                    new Date($(\'.calendar5_' . $n . '\').attr(\'data-date\')),
                                ],
                            });
                        });
                        </script>
                        
                        ';
                    $n++;
                }
            }
        }

        $body .= '</tbody></table><br /><button>Сохранить</button></form>';
    } elseif (isset($_POST['sent']) and $_POST['sent'] == 2) {
        $form = (int)($_POST['form']);
        if (!$form) {
            header('location:/olimp_stages.html');
            exit;
        }

        foreach ($_POST['exam'] as $key => $x) {
            $date_exam = addslashes($_POST['date_exam'][$key] . ' ' . $_POST['date_exam2'][$key]);
            $date_ereg = addslashes($_POST['date_ereg'][$key] . ' ' . $_POST['date_ereg2'][$key]);
            $exam = (int)$_POST['exam'][$key];
            $place = (int)$_POST['place'][$key];
            $class = (int)$_POST['class'][$key];
            $year = (int)$_POST['year'][$key];
            $stage = (int)$_POST['stage'][$key];
            $date_breg = addslashes($_POST['date_breg'][$key]);
            $date_post = addslashes($_POST['date_post'][$key] . ' ' . $_POST['date_post2'][$key]);
            $grp_prefix = addslashes($_POST['grp_prefix'][$key]);
            $grp_limit = (int)$_POST['grp_limit'][$key];
            $next_group = (int)$_POST['next_group'][$key];
            $task_count = (int)$_POST['task_count'][$key];
            $results_ready = addslashes($_POST['results_ready'][$key]);
            $task_weight = addslashes($_POST['task_weight'][$key]);
            $use_person_result_text = (int)$_POST['use_person_result_text'][$key];
            $stage_result_text = addslashes($_POST['stage_result_text'][$key]);
            $var_prefix = addslashes($_POST['var_prefix'][$key]);
            $min_var = addslashes($_POST['min_var'][$key]);
            $max_var = addslashes($_POST['max_var'][$key]);

            $grp_prefix = '';
            if ($exam == 1) {
                $grp_prefix .= 'М';
            }
            if ($exam == 2) {
                $grp_prefix .= 'P';
            }
            if ($exam == 3) {
                $grp_prefix .= 'I';
            }
            if ($exam == 4) {
                $grp_prefix .= 'K';
            }

            $grp_prefix .= $class;

            if ($stage == 0) {
                $grp_prefix .= 'D';
            }
            if ($stage == 1) {
                $grp_prefix .= 'S';
            }
            if ($stage == 2) {
                $grp_prefix .= 'F';
            }
            if ($stage == 3 or $stage == 4) {
                $grp_prefix .= 'Т';
            }


            $query = "insert into olimp_stages(`var_prefix`, `min_var`, `max_var`, `date_exam`, `exam`, `place`, `classes`, `year`, `stage`, `form`, `date_breg`, `date_ereg`, `date_post`, `grp_prefix`, `grp_limit`, `next_group`, `task_count`, `results_ready`, `task_weight`, `use_person_result_text`, `stage_result_text`) 
                                        values('$var_prefix', '$min_var', '$max_var', '$date_exam', '$exam', '$place', '$class', '$year', '$stage', '$form', '$date_breg', '$date_ereg', '$date_post', '$grp_prefix', '$grp_limit', '$next_group', '$task_count', '$results_ready', '$task_weight', '$use_person_result_text', '$stage_result_text');";
            $mysqli->query($query);
        }

        header('location:/olimp_stages.html');
        exit;
    } else {
        $forms = array();
        $query = "select * from olimp_forms order by name asc";
        $res = $mysqli->query($query);
        while ($row = $res->fetch_assoc()) {
            $forms[$row['id']] = $row['name'];
        }

        $lists = array();

        // Предметы
        $res = $mysqli->query('select id, name from olimp_exams order by name asc');
        while ($row = $res->fetch_assoc()) {
            $lists['exams'][$row['id']] = $row['name'];
        }

        // Площадки
        $res = $mysqli->query('select id, name from olimp_places order by name asc');
        while ($row = $res->fetch_assoc()) {
            $lists['places'][$row['id']] = $row['name'];
        }

        $body = '
    <form method="post" id="newPotok">
        <input type="hidden" name="sent" value="1">
    
        <div class="line">
            <label>Укажите дату экзамена:</label>
            <input type="text" name="date_exam" class="calendar" data-date="' . date('Y-m-d') . '">
        </div>
        
        <div class="line">
            <label>Укажите дату завершения рег-ции:</label>
            <input type="text" name="date_ereg" class="calendar2" data-date="' . date('Y-m-d') . '">
        </div>
        
        <div class="line">
            <label>Укажите форму:</label>
            <select name="form" style="width:100px;">';
        foreach ($forms as $id => $form) {
            if ($id == 1) {
                $sel = 'selected="true"';
            } else {
                $sel = '';
            }
            $body .= '<option value="' . $id . '" ' . $sel . '>' . $form . '</option>';
        }
        $n++;
        $body .= '</select>
        </div>
    ';

        foreach ($lists['exams'] as $id => $name) {
            $body .= '<div class="line">
            <input type="checkbox" name="exam" /> ' . $name . '
            <div style="display:none;">
        ';

            foreach ($lists['places'] as $id2 => $name2) {
                $sel5 = '';
                $sel6 = '';
                $sel7 = '';
                $sel8 = '';
                $sel9 = '';
                $sel10 = '';
                $sel11 = '';

                if ($id == 3 or $id == 4) {
                    $sel9 = 'checked="true"';
                    $sel10 = 'checked="true"';
                    $sel11 = 'checked="true"';
                }
                if ($id == 1 or $id == 2) {
                    $sel7 = 'checked="true"';
                    $sel8 = 'checked="true"';
                    $sel9 = 'checked="true"';
                    $sel10 = 'checked="true"';
                    $sel11 = 'checked="true"';
                }

                $body .= '<div class="line">
                <input style="margin-left:30px;" type="checkbox" name="place" /> ' . $name2 . '
                <div style="display:none;">
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-5" ' . $sel5 . ' />5<br />
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-6" ' . $sel6 . ' />6<br />
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-7" ' . $sel7 . ' />7<br />
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-8" ' . $sel8 . ' />8<br />
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-9" ' . $sel9 . ' />9<br />
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-10" ' . $sel10 . ' />10<br />
                    <input style="margin-left:60px;" type="checkbox" name="class[]" value="' . $id . '-' . $id2 . '-11" ' . $sel11 . ' />11
                </div>
            </div>';
            }

            $body .= '
            </div>
        </div>';
        }

        $body .= '    
        <div class="line">
            <label></label>
            <button class="step1">Далее</button>
        </div>
    </form>
    ';
    }
