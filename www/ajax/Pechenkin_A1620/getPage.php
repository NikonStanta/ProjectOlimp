<?php
session_start();
// Подключиться к базе данных.
// На сервере.
$mysqli = new mysqli('localhost', 'p638056_ehope_sql', 'qW6tZ7(N0_hY8eY%wD2q', 'p638056_ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
$mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
$mysqli->query('SET SESSION sql_mode = ""');
// Локально
// $mysqli = new mysqli('localhost', 'root', 'mysql', 'ehope') or die('Сервер перегружен запросами. Пожалуйста, повторите попытку позже.');
// $mysqli->set_charset(!empty($_connection_charset) ? $_connection_charset : 'utf8');
// $mysqli->query('SET SESSION sql_mode = ""');

function string6($str)
{
  for ($i = 0; $i < 6; $i++) {
    $str .= $str;
  }
  return $str;
}

$last_updates = array( // Зполнить полностью.
  "cabinet" => array(
    "profile" => "01-01-0000",
    "documents" => "01-01-0001",
    "stages" => "01-01-0002",
    "distance" => "01-01-0003_",
    "works" => "01-01-0004",
    "results" => "01-01-0005",
  ),
  "main" => array(
    "main" => "01-01-0000",
  ),
  "about" => array( // Section # 1. 
    "statement" => "01-01-10011", // Page # 1.
    "reglament" => "01-01-1002", // Page # 2.
    "istoriya_olimpiadi" => "01-01-1003", // Page # 3.
    "organizatori" => "01-01-1004", // Page # 4.
    "partner" => "01-01-1005", // Page # 5.
  ),
  "actual" => array( // Section # 2.
    "grafik" => "01-01-2001", // Page # 1.
    "result" => "01-01-2002", // Page # 2.
    "criterii" => "01-01-2003", // Page # 3.
    "tasks" => "01-01-2004", // Page # 4.
  ),
  "olymp" => array( // Section # 3.
    "nextnear" => "01-01-3001", // Page # 1.
    "metodic" => "01-01-3002", // Page # 2.
    "liter" => "01-01-3003", // Page # 3.
    "liter_p" => "01-02-3003", // Page # 3.1
    "liter_m" => "01-02-3003", // Page # 3.2
    "liter_i" => "01-02-3003", // Page # 3.3

    "tasks" => "01-01-3004", // Page # 4.
    "orgadd" => "01-01-3007", // Page # 7.
    "obrazci" => "01-01-3009", // Page # 9.
    "pdf" => "01-01-3010", // Page # 10.
  ),
  "novice" => array( // Section # 4.
    "kak_prinyat_uchastie" => "01-01-4001", // Page # 1.
    "otveti_na_voprosi" => "01-01-4002", // Page # 2.
  ),
  "lastyear" => array( // Section # 5.
    "fisgia" => "01-01-5001", // Page # 1.
    "actionplan_20" => "01-01-5002", // Page # 2.
  ),
  "contact" => array( // Section # 7.
    "contact" => "01-01-7001", // Page # 1.
  ),
  "user" => array( // Формы регистрации и входа.
    "login" => "login 01-01-2000",
    "signup" => "signup 01-01-2001",
  )
);
if (isset($_POST['page_last_update'])) { // Получение последнего обновления страницы.
  $param = json_decode($_POST['page_last_update']);
  $date = array("date" => $last_updates[$param->section][$param->page]);
  echo json_encode($date);
}

if (isset($_POST['page_identifyer'])) { // Получение содержимого страницы.
  // sleep(100); // Для отладки: для имитации неудачи загрузки содержимого страницы с сервера.
  $param = json_decode($_POST['page_identifyer']);
  switch ($param->section) {
    // Меню личного кабинета.
    case "cabinet":
      switch ($param->page) {
        case "left_menu":          
          $strRespond = "
            <li><a href=\"cabinet/profile\" class=\"submenuTrigger  inner-a\" class=\"signupTrigger\">Профиль</a>
            <li><a href=\"cabinet/documents\" class=\"submenuTrigger  inner-a\" class=\"signupTrigger\">Документы</a>
            <li><a href=\"cabinet/stages\" class=\"submenuTrigger  inner-a\" class=\"signupTrigger\">Регистрация</a>
            <li><a href=\"cabinet/distance\" class=\"submenuTrigger  inner-a\" class=\"signupTrigger\">Участие</a>
            <li><a href=\"cabinet/works\" class=\"submenuTrigger  inner-a\" class=\"signupTrigger\">Загрузка</a>
            <li><a href=\"cabinet/results\" class=\"submenuTrigger  inner-a\" class=\"signupTrigger\">Результаты</a>
            <li><a href=\"\" class=\"submenuTrigger\" class=\"signupTrigger\" id=\"outbtn\">Выход</a>
            ";
          
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "profile":          
          $strRespond = "
            <h2 class=\"page-header\">Участие в этапах олимпиады</h2>
              <h2 class=\"page-subheader\">Учетные данные</h2>
              <div class=\"tab-stages\" id=\"tab-1\"></div>
              <h2 class=\"page-subheader\">Личные данные</h2>
              <div class=\"tab-stages\" id=\"tab-2\"></div>
              <h2 class=\"page-subheader\">Адреса</h2>
              <div class=\"tab-stages\" id=\"tab-3\"></div>
              <h2 class=\"page-subheader\">Учебное заведение</h2>
              <div class=\"tab-stages\" id=\"tab-4\"></div>
          ";

          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          /*
          $strRespond = "
            <h2 class=\"page-header\">Профиль</h2>
            <h2 class=\"page-subheader\">Учетные данные</h2>
            <span class=\"bold profile-text\">Логин:</span> %nic%  <br/>
            <span class=\"bold profile-text\">Секретный вопрос:</span> %question%  <br/>
            <span class=\"bold profile-text\">Секретный ответ:</span> %answer% <br/>
            </br>
            <a class=\"download_btn\" target=\"\" href=\"\">Изменить учётные данные</a>
            <a class=\"download_btn\" target=\"\" href=\"\">Изменить пароль</a>
            
            <!-- <a class=\"download_btn_half\" target=\"_blank\" href=\"/docs/Obrazec_zajavlenija_na_prozhivanie_bez_soprovozhdajushhego.pdf\">Изменить уч. данные</a>
            <a class=\"download_btn_half\" target=\"_blank\" href=\"/docs/Obrazec_zajavlenija_na_prozhivanie_s_soprovozhdajushhim.pdf\">Изменить пароль</a>            
            <br>
            -->

            <h2 class=\"page-subheader\">Личные данные</h2>
            <span class=\"bold profile-text\">ФИО участника:</span> %fio% <br/>
            <span class=\"bold profile-text\">Дата рождения:</span> %birthday% <br/>
            <span class=\"bold profile-text\">Место рождения:</span> %bplace% <br/>
            <span class=\"bold profile-text\">Пол:</span> %gender% <br/>
            <span class=\"bold profile-text\">СНИЛС:</span> %SNILS% <br/>
            <span class=\"bold profile-text\">Документ:</span> %document_ic% <br/>
            <span class=\"bold profile-text\">Гражданство:</span> %citizenship% <br/>
            <span class=\"bold profile-text\">Телефон:</span> %tel% <br/>
            <span class=\"bold profile-text\">e-mail:</span> %email% <br/>
            <h2 class=\"page-subheader\">Информация о родителе (или законном представителе)</h2>
            <span class=\"bold profile-text\">ФИО:</span> %parent_fio% <br/>
            <span class=\"bold profile-text\">Степень родства:</span> %parent_degree% <br/>
            <span class=\"bold profile-text\">Телефон:</span> %parent_tel% <br/>
            <span class=\"bold profile-text\">e-mail:</span> %parent_email% <br/>            
            <a class=\"download_btn\" target=\"\" href=\"\">Перейти к изменению данных</a>

            <h2 class=\"page-subheader\">Адреса</h2>
            <span class=\"bold profile-text\">Адрес постоянной регистрации: %addr_reg%</span> <br/>
            <span class=\"bold profile-text\">Почтовый адрес: %addr_post%</span> <br/>
            <a class=\"download_btn\" target=\"\" href=\"\">Перейти к изменению данных</a>

            <h2 class=\"page-subheader\">Учебное заведение</h2>
            <span class=\"bold profile-text\">Класс: %school_class%</span> <br/>
            <span class=\"bold profile-text\">Учебное заведение: %school%</span> <br/>
            <span class=\"bold profile-text\">Учитель математики: %teacher_m%</span> <br/>
            <span class=\"bold profile-text\">Учитель физики: %teacher_f%</span> <br/>
            <span class=\"bold profile-text\">Учитель информатики: %teacher_i%</span> <br/>
            <a class=\"download_btn\" target=\"\" href=\"\">Перейти к изменению данных</a>
            <a class=\"download_btn\" target=\"\" href=\"\">Выбрать/ввести учебное заведение</a>
          ";

          // $_SESSION['uid'] = 78508; // DELETE - for debug. 
          if($_SESSION['uid'] > 0){
            // Учетные данные.
            $res = $mysqli->query('select nic,question,answer from olimp_users where id='.$_SESSION['uid'].';');
            while ($row = $res->fetch_assoc()) {
                $strRespond = preg_replace(
                  array('/%nic%/','/%question%/','/%answer%/'),
                  array($row['nic'],$row['question'],$row['answer']),
                  $strRespond
                ); // pwd
              }
            // Личные данные.
            $res=$mysqli->query('select concat_ws(" ",p.surename,p.name,p.patronymic) as fio,DATE_FORMAT(p.birthday,"%d.%m.%Y") 
              as birthday,p.gender,p.SNILS,p.doc_ident,c.name 
              as citizenship,p.tel,p.email,p.parent_fio,p.parent_degree,p.parent_tel,p.parent_email, p.bplace as bplace
              from olimp_persons as p left join olimp_citizenships as c on (c.id=p.citizenship) where p.uid='.$_SESSION['uid']);
            // $strRespond = mysqli_error($mysqli);
            $row=$res->fetch_assoc();
            if ($row['doc_ident']>0){
              $document_ic=cabinet_document_text($row['doc_ident'], $mysqli);
            } else{
              $document_ic=preg_replace('/%text%/','Документ, удостоверяющий личночть и гражданство не введен', '<span class="msg_warning">%text%</span>');
            }
            //if (empty($row['tel'])) $row['tel']=preg_replace(array('/%text%/','/%id%/','/%target%/'),array('Телефон не введен','btnTel','btnNext2'),$tpl['cabinet']['NoDataErrorBtn']);
            if (empty($row['bplace'])) $row['bplace']=preg_replace('/%text%/','Не введено место рождения', '<span class="msg_warning">%text%</span>');
            if (empty($row['tel'])) $row['tel']=preg_replace('/%text%/','Телефон не введен', '<span class="msg_warning">%text%</span>');
            if (empty($row['SNILS'])) $row['SNILS']=preg_replace('/%text%/','СНИЛС не введен', '<span class="msg_warning">%text%</span>');
            if (empty($row['email'])) $row['email']=preg_replace('/%text%/','Адрес электронной почты не введен', '<span class="msg_warning">%text%</span>');
            
            if (empty($row['parent_fio'])) $row['parent_fio']=preg_replace('/%text%/','Не введено.', '<span class="msg_warning">%text%</span>');
            if (empty($row['parent_degree'])) $row['parent_degree']=preg_replace('/%text%/','Не введено.', '<span class="msg_warning">%text%</span>');
            if (empty($row['parent_tel'])) $row['parent_tel']=preg_replace('/%text%/','Не введено.', '<span class="msg_warning">%text%</span>');
            if (empty($row['parent_email'])) $row['parent_email']=preg_replace('/%text%/','Не введено.', '<span class="msg_warning">%text%</span>');
            
            $strRespond=preg_replace(
              array('/%fio%/','/%birthday%/','/%bplace%/','/%gender%/','/%SNILS%/','/%document_ic%/','/%citizenship%/','/%tel%/','/%email%/','/%parent_fio%/','/%parent_degree%/','/%parent_tel%/','/%parent_email%/'),
              array($row['fio'],$row['birthday'],$row['bplace'],($row['gender']=='M' ? 'Мужской' : 'Женский'),$row['SNILS'],$document_ic,$row['citizenship'],$row['tel'],$row['email'],$row['parent_fio'],$row['parent_degree'],$row['parent_tel'],$row['parent_email']),
              $strRespond);

            // Адреса. addr_reg addr_post
            $res=$mysqli->query('select addr_reg,addr_post from olimp_persons where uid='.$_SESSION['uid']);
	          $row=$res->fetch_assoc();

            if ($row['addr_reg']>0) {
              if (isset($_POST['btnApprove'])){
                cabinet_address_approve_kladr($row['addr_reg'], $mysqli);
              }
              
              $addr_reg=cabinet_address_text($row['addr_reg'], $mysqli);
              $addr_reg_kladr=cabinet_address_text_kladr($row['addr_reg'], $mysqli); // $dadata
            } else {
              $addr_reg=preg_replace('/%text%/','Адрес постоянной регистрации не введен', '<span class="msg_warning">%text%</span>');
              $addr_reg_kladr='';
            }
            if ($row['addr_post']>0) {	
              if (isset($_POST['btnApprove'])){
                cabinet_address_approve_kladr($row['addr_post'], $mysqli);
              }
              $addr_post=cabinet_address_text($row['addr_post'], $mysqli);
              $addr_post_kladr=cabinet_address_text_kladr($row['addr_post'], $mysqli); // $dadata
            } else {
              $addr_post=preg_replace('/%text%/','Почтовый адрес не введен. Требуется только если отличается от адреса постоянной регистрации', '<span class="msg_warning">%text%</span>');
              $addr_post_kladr='';
            }

            $strRespond=preg_replace(
              array('/%addr_reg%/','/%addr_post%/'),
              array($addr_reg,$addr_post),
              $strRespond);
          }

          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          */
          break;
          case "documents": // Кабинет/Документы
            $strRespond = "
              <h2 class=\"page-header\">Документы участника</h2>              
              Документы формируются в формате PDF. Для просмотра и печати используйте Adobe Reader, который бесплатно доступен по адресу
              <a href=\"http://get.adobe.com/reader/\" target=\"_blank\">http://get.adobe.com/reader/</a>
              <br/><br/>
              <div class=\"documents-attention-msg\">
                <span class=\"bold\">ВНИМАНИЕ!</span>
                Формирование документов требует некоторого времени (от 30 секунд до нескольких минут). До завершения формирования документа
                не нажимайте повторно кнопку \"Вывести/напечатать документ\". При скачивании документов не поддерживаются менеджеры загрузки.
                Если при попытке скачать документ вы получаете сообщение об отсутствующей странице или получаете пустую страницу, отключите
                менеджер загрузки.
              </div>

              <div class=\"documents-nodata-msg\">
                <h2 class=\"page-subheader\">Заявление на участие в олимпиаде и согласие на обработку персональных данных
                </h2>
                Введены не все необходимые персональные данные! Пожалуйста, перейдите на страницу \"Профиль участника\" и введите недостающие данные.
                <br/>
                <span class=\"bold\">Внимание!</span> Не введены следующие данные:</br>
                  <ul>
                    <li>Не введено место рождения участника</li>
                    <li>Не введен контактный телефон</li>
                    <li>Не введен документ, подтверждающий личность и гражданство</li>
                    <li>Не введен адрес постоянной регистрации</li>
                    <li>Не введен или неверно введен класс</li>
                  </ul>
              </div>

              <div class=\"document-upload-msg\">
                <h2 class=\"page-subheader\">Справка из школы
                </h2>                
                Для подтверждения данных о классе обучения участника заключительного этапа необходимо загрузить справку из школы с указанием ФИО
                и класса обучения. Справку можно загрузить и позже (после регистрации в потоки заключительного этапа), но при подведении итогов
                заключительного этапа отсутствие справки из школы не позволит присудить участнику призовую ступень. Поэтому оргкомитет рекомендует
                не откладывать загрузку этого документа. 
                <br/><br/>
                Размер загружаемого файла - не более 5 Mb. 
                <br/><br/>
                Формат загружаемого файла – PDF или JPEG. 
              </div>
              <div class=\"\" id=\"schoolDocMsg\"></div>
              <form id = \"schooldocfile_form\" enctype=\"multipart/form-data\">
                Выберите файл: <input id = \"schooldocfile_load\" type=\"file\" class=\"download_btn\" name=\"schooldocfile\"></input>
                <input type=\"button\" class=\"upload_btn\" name=\"\" id=\"UploadSchoolDoc\"value=\"Загрузить справку\"></input>
              </form>
              <!-- <a class=\"download_btn\" href=\"\" target=\"\">Выбрать файл</a> -->
              <!-- <a class=\"download_btn\" href=\"\" target=\"\">Загрузить справку</a> -->
              <br/>

              <div class=\"document-upload-msg\">
                <h2 class=\"page-subheader\">Документ, идентифицирующий участника олимпиады
                </h2>                                
                    Для регистрации в потоки необходимо загрузить документ, идентифицирующий участника, содержащий фотографию участника, в том числе: справку из школы
                    с фотографией (заверенной печатью школы), страницу паспорта с фото, страницу загранпаспорта с фото, водительские права с фото, социальную карту с
                    фото, пропуск в школу с фото. 
                <br/><br/>
                Размер загружаемого файла - не более 5 Mb. 
                <br/><br/>
                Формат загружаемого файла – PDF или JPEG. 
              </div>
              <div class=\"\" id=\"identDocMsg\"></div>
              <form id = \"identdocfile_form\" enctype=\"multipart/form-data\">
                Выберите файл: <input id = \"identdocfile_load\" type=\"file\" class=\"download_btn\" name=\"identdocfile\"></input>
                <input type=\"button\" class=\"upload_btn\" name=\"\" id=\"UploadIdentDoc\"value=\"Загрузить документ\"></input>
              </form>
              <!-- <a class=\"download_btn\" href=\"\" target=\"\">Выбрать файл</a>
              <a class=\"download_btn\" href=\"\" target=\"\">Загрузить документ</a> -->
              <br/>

              <div class=\"document-upload-msg\">
                <h2 class=\"page-subheader\">Фотография участника олимпиады
                </h2>                    
                Для идентификации участника на заключительном этапе олимпиады необходимо загрузить фотографию участника (загрузка фото также необходима для доступа к регистрации в потоки заключительного этапа). 
                <br/><br/>
                <span class=\"bold\">Требования к фотографии: </span>
                <br/><br/>

                Участник на фотографии должен соответствовать своему возрасту на момент участия в олимпиаде.
                <br/>
                Лицо — анфас, в фокусе.  70 – 80 % от фотографии должен занимать овал лица. Выражение лица — нейтральное, закрытый рот и открытые глаза. Смотреть нужно прямо на камеру. Ничего не должно закрывать лица. 
                <br/>
                Лицо — анфас, в фокусе.  70 – 80 % от фотографии должен занимать овал лица. Выражение лица — нейтральное, закрытый рот и открытые глаза. Смотреть нужно прямо на камеру. Ничего не должно закрывать лица. 
                <br/>
                Лицо — анфас, в фокусе.  70 – 80 % от фотографии должен занимать овал лица. Выражение лица — нейтральное, закрытый рот и открытые глаза. Смотреть нужно прямо на камеру. Ничего не должно закрывать лица. 
                <br/><br/>
                Размер загружаемого файла - не более 5 Mb. 
                <br/><br/>
                Формат загружаемого файла – PDF или JPEG. 
              </div>

              <div class=\"\" id=\"identPhotoMsg\"></div>
              <form id = \"identphotofile_form\" enctype=\"multipart/form-data\">
                Выберите файл: <input id = \"identphotofile_load\" type=\"file\" class=\"download_btn\" name=\"identphotofile\"></input>
                <input type=\"button\" class=\"upload_btn\" name=\"\" id=\"UploadIdentPhoto\"value=\"Загрузить документ\"></input>
              </form>

              <!-- <a class=\"download_btn\" href=\"\" target=\"\">Выбрать файл</a>
              <a class=\"download_btn\" href=\"\" target=\"\">Загрузить документ</a> -->
            ";

            $page_content = array("content" => $strRespond);
            echo json_encode($page_content);
            break;
          case "stages":
            $strRespond = "
              <h2 class=\"page-header\" >Участие в этапах олимпиады</h2>              
              <h2 class=\"page-subheader\" id=\"qualif_stages\">Отборочный этап <div class=\"\" id=\"tab-2-arrow\"></div></h2>
              <div class=\"tab-stages\" id=\"tab-2\"></div>
              <h2 class=\"page-subheader\" id=\"final_stages\">Заключительный этап <div class=\"\" id=\"tab-3-arrow\"></div></h2>
              <div class=\"tab-stages\" id=\"tab-3\"></div>
              <h2 class=\"page-subheader\" id=\"training_stage\">Тренировочный этап <div class=\"\" id=\"tab-4-arrow\"></div></h2>
              <div class=\"tab-stages\" id=\"tab-4\"></div>
			  <h2 class=\"page-subheader\" id=\"completed_stages\">Завершенные этапы <div class=\"\" id=\"tab-1-arrow\"></div></h2>
              <div class=\"tab-stages\" id=\"tab-1\"></div>
              <br/>
              <!-- 
              <h2 class=\"page-header\">Перезачёт результатов олимпиад-партнёров и призёрства прошлого года</h2>
              <h2 class=\"page-subheader\">Математика</h2>
              <div class=\"document-upload-msg\">
                <span class=\"bold\">Внимание!</span>                 
                Вы не можете зарегистрироваться на участие в этапе так как введены не все необходимые персональные данные!
                Пожалуйста, перейдите на страницу \"Профиль участника\" и введите недостающие данные.
              </div>
              <h2 class=\"page-subheader\">Физика</h2>
              <h2 class=\"page-subheader\">Информатика</h2>
              <h2 class=\"page-subheader\">Компьютерное моделирование</h2> -->
            ";
            $page_content = array("content" => $strRespond);
            echo json_encode($page_content);
            break;
          case "distance":
            $strRespond = "
              <h2 class=\"page-header\">Материалы для участия в олимпиаде в заочной форме</h2>              
              Некоторые документы сформированы в формате PDF. Для просмотра и печати используйте Adobe Reader,
              который бесплатно доступен по адресу 
              <a href=\"http://get.adobe.com/reader/\" target=\"_blank\">http://get.adobe.com/reader/</a>
              <br/><br/>
              <span class=\"bold\">ВНИМАНИЕ! </span> 
              При скачивании вариантов не поддерживаются менеджеры загрузки. Если при попытке скачать вариант вы получаете сообщение об отсутствующей странице, отключите менеджер загрузки.
              <br/><br/>
              <div id=\"var-ref\" class=\"distance-msg\"></div>

              <div class=\"distance-msg\">
              <h2 class=\"page-subheader\">Бланки для выполнения работы</h2>
              Все бланки, необходимые для выполнения и отправки работы в оргкомитет олимпиады, помещены в архив формата RAR. Вы можете скачать архив нажав кнопку  \"Получить бланки\" ниже. Рекомендуем Вам сделать это заблаговременно, не откладывая на последний день перед началом этапа. 
              </div>

              <a class=\"download_btn\" href=\"\" target=\"\">Получить бланки</a>
            ";
            $page_content = array("content" => $strRespond);
            echo json_encode($page_content);
            break;
          case "works":
            $strRespond = "
            <h2 class=\"page-subheader\">Загрузка и просмотр работ</h2>
            <h2 class=\"page-subheader\">Заключительный этап</h2>
            <h2 class=\"page-subheader\">Отборочный этап</h2>
            <h2 class=\"page-subheader\">Тренировочный этап</h2>
            ";
            $page_content = array("content" => $strRespond);
            echo json_encode($page_content);
            break;
          case "results":
            $strRespond = "
            <h2 class=\"page-header\">Результаты участника</h2>
            <h2 class=\"page-subheader\">Отборочный этап</h2>

            <h2 class=\"page-subheader\">Математика</h2>
            <div class=\"documents-nodata-msg\"> 
              Вы не принимали участия и не зарегистрированы на участие ни в одном потоке отборочного этапа олимпиады по предмету Математика.
              Для того, чтобы принять участие в отборочном этапе по этому предмету перейдите на страницу «Участие в олимпиаде» и зарегистрируйтесь
              на участие в одном из потоков. Внимательно выбирайте площадку, форму (очную/заочную) и дату проведения потока.
            </div>

            <h2 class=\"page-subheader\">Физика</h2>
            <div class=\"documents-nodata-msg\">               
              Вы не принимали участия и не зарегистрированы на участие ни в одном потоке отборочного этапа олимпиады по предмету Физика.
              Для того, чтобы принять участие в отборочном этапе по этому предмету перейдите на страницу «Участие в олимпиаде» и зарегистрируйтесь
              на участие в одном из потоков. Внимательно выбирайте площадку, форму (очную/заочную) и дату проведения потока.
            </div>

            <h2 class=\"page-subheader\">Информатика</h2>
            <div class=\"documents-nodata-msg\">               
              Вы не принимали участия и не зарегистрированы на участие ни в одном потоке отборочного этапа олимпиады по предмету Информатика.
              Для того, чтобы принять участие в отборочном этапе по этому предмету перейдите на страницу «Участие в олимпиаде» и зарегистрируйтесь
              на участие в одном из потоков. Внимательно выбирайте площадку, форму (очную/заочную) и дату проведения потока.
            </div>

            <h2 class=\"page-subheader\">Компьютерное моделирование</h2>
            <div class=\"documents-nodata-msg\">
              Вы не принимали участия и не зарегистрированы на участие ни в одном потоке отборочного этапа олимпиады по предмету Компьютерное моделирование.
              Для того, чтобы принять участие в отборочном этапе по этому предмету перейдите на страницу «Участие в олимпиаде» и зарегистрируйтесь
              на участие в одном из потоков. Внимательно выбирайте площадку, форму (очную/заочную) и дату проведения потока.
            </div>
            ";
            $page_content = array("content" => $strRespond);
            echo json_encode($page_content);
            break;
      }
      break;
    // Об олимпиаде.
    case "main":
      switch ($param->page) {
        case "main":
          // Софрмировать содержимое страницы страницу.
          $strRespond = string6("Главная страница. ");
          $page_content = array("content" => $strRespond);
          // Отправить страницу
          echo json_encode($page_content);
          break;
      }
      break;
    case "about": // Section # 1.
      switch ($param->page) {
        case "statement": // Page # 1.
          // Софрмировать содержимое страницы страницу.          
          // $strRespond = "Положение об олимпиаде:</br><a target=\"_blank\" href = \"docs/EHOPE_statement.pdf\" class=\"download_btn\">Скачать файл PDF</a>";
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'about' and `page` = 'statement'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          // Отправить страницу
          echo json_encode($page_content);
          break;
        case "reglament": // Page # 2.
          // $strRespond = string6("Регламент проведения. ");
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'about' and `page` = 'reglament'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода регламента олимпиады";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "istoriya_olimpiadi": // Page # 3.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'about' and page = 'istoriya_olimpiadi'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода истории олимпиады";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "organizatori": // Page # 4.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'about' and page = 'organizatori'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"организаторы\"";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "partner": // Page # 5.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'about' and page = 'partner'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Партнеры\"";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
      }
      break;

    case "actual": { // Section # 2.
      switch ($param->page) {        
        case "grafik": // Page # .
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'actual' and page = 'grafik'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"График мероприятий.\"";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "result":
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'actual' and page = 'result'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Результаты олимпиады.\"";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "criterii":
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'actual' and page = 'criterii'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Критерии Олимпиады.\"";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "tasks":
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'actual' and page = 'tasks'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Материалы заданий\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
      }
    }
      break;
    case "olymp": { // Section # 3.
      switch ($param->page) {
        case "nextnear": // Page # 1.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'nextnear'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Ближайшие мероприятия\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;

        case "metodic": // Page # 2.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'metodic'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Методические материалы\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "liter": // Page # 3.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'liter'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Перечень рекомендуемой литературы\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "liter_p":
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'liter_p'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Перечень рекомендуемой литературы по физике\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "liter_m":
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'liter_m'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Перечень рекомендуемой литературы по математике\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "liter_i":
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'liter_i'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Перечень рекомендуемой литературы по информатике\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "tasks": // Page # 4.
          $strRespond = string6("Материалы заданий прошлых лет. ");
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "orgadd": // Page # 7.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'orgadd'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Организационная поддержка участников\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "obrazci": // Page # 9.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'obrazci'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Бланки и образцы документов\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "pdf": // Page # 10.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'olymp' and page = 'pdf'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Работа с форматом файла PDF\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
      }
    }
      break;

    case "novice": { // Section # 4.
      switch ($param->page) {
        case "kak_prinyat_uchastie": // Page # 1.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'novice' and page = 'kak_prinyat_uchastie'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Как принять участие\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "otveti_na_voprosi": // Page # 2.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'novice' and page = 'otveti_na_voprosi'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Часто задаваемые вопросы\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
      }
    }
      break;

    case "lastyear": { // Section # 5.
      switch ($param->page) {
        case "fisgia": // Page # 1.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'lastyear' and page = 'fisgia'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Внесение сведений в ФИС ГИА и приема\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
        case "actionplan_20": // Page # 2.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'lastyear' and page = 'actionplan_20'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Как засчитать призовое место прошлого года\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
      }
    }
      break;
    case "contact": { // Section # 7.
      switch ($param->page) {
        case "contact": // Page # 1.
          $result = $mysqli->query("SELECT content FROM `mobile_pages` WHERE section = 'contact' and page = 'contact'");
          if ($result->num_rows == 0) {
            $strRespond = "Ошибка вывода страницы \"Контакты\".";
          } else {
            $row = $result->fetch_assoc();
            $strRespond = $row["content"];
          }
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          break;
      }
    }
      break;
    case "user": { # Вход в личный кабинет.
      switch ($param->page) {
        case "login":
          // Передать форму входа в личный кабинет.
          $strRespond = getLoginPage();
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          // ~Check login and password.
          break;
        case "signup":
          // Передать форму регистрации пользователя.
          $strRespond = getSignupPage();
          $page_content = array("content" => $strRespond);
          echo json_encode($page_content);
          // ~Get data of user; validate user data?add_user&return_result:rewrite_data; 
          break;
      }
    }
      break;
  }
}


function getLoginPage()
{
  return "<div class=\"login_and_signup_label page-header\">Вход на сайт</div>
  <div class=\"login_form_msg msg\" id=\"login_form_msg\"></div>
  <form action=\"\" method=\"get\" class=\"login_and_signup_form login_form\" id=\"login_form\">
    <!--
    <label>Статус:</label>
    <select id=\"login_status\">
      <option value=\"1\" id=\"\">участник</option>
      <option value=\"2\" id=\"\">партнер</option>
      <option value=\"3\" id=\"\">организатор</option>
    </select></br>
    -->
    <label>Логин:</label>
    <input type=\"text\" name=\"login_login\" id=\"login_login\"></input></br>
    <label>Пароль:</label>
    <input type=\"password\" name=\"login_pass\" id=\"login_password\"></input></br>
    <input type=\"button\" value=\"Вход\" id=\"login_btn\"></input>
    <input type=\"reset\" value=\"Сбросить\" id=\"login_reset_btn\"></input>
  </form>
          ";
}
function getSignupPage()
{
  $_SESSION['hash'] = md5(microtime());
  $reg_html =  "
  <div class=\"login_and_signup_label page-header\">Регистрация. Шаг 1.</div>
  <div class=\"signup_form_msg msg\" id=\"signup_form_msg\"></div>
  <form action=\"\" method=\"get\" class=\"login_and_signup_form signup_form\" id=\"signup_form\">
    <input type=\"hidden\" name=\"regStep\" value=\"1\" id=\"regStep\">
    <input type=\"hidden\" name=\"hash\" value=\"%hash%\" id=\"hash\">
    <label>Фамилия:</label>
    <input type=\"text\" name=\"regSurename\" id=\"regSurename\"></input></br>
    <label>Имя:</label>
    <input type=\"text\" name=\"regName\" id=\"regName\"></input></br>
    <label>Отчество:</label>
    <input type=\"text\" name=\"regPatronymic\" id=\"regPatronymic\"></input></br>
    <label>Дата рождения в формате dd.mm.yyyy (например 20.01.2001):</label>
    <input type=\"text\" name=\"regBirthday\" id=\"regBirthday\"></input></br>
    <!-- <input type=\"date\" name=\"signup_birthday\" id=\"signup_birthday\" value=\"01.01.2000\" min=\"01.01.1900\" max=\"22.04.2024\"></input></br> -->
    <label>e-mail:</label>
    <input type=\"text\" name=\"regEmail\" id=\"regEmail\"></input></br>
    <label>Выберите вопрос:</label> 
    <select id=\"regQuestionA\">
      <option value=\"1\">Дата рождения матери</option>
      <option value=\"2\">Дата рождения отца</option>
      <option value=\"3\">Серия и номер паспорта отца</option>
      <option value=\"4\">Серия и номер паспорта матери</option>
      <option value=\"5\">Любимый актер/актриса кино</option>
      <option value=\"6\">Любимый фильм</option>
      <option value=\"7\">Как вас называли в детстве</option>
      <option value=\"8\">Любимое блюдо</option>
    </select></br>
    <label>или введите свой:</label>
    <input type=\"text\" name=\"regQuestionB\" id=\"regQuestionB\"></input></br>
    <label>Введите ответ:</label>
    <input type=\"text\" name=\"regAnswer\" id=\"regAnswer\"></input></br>

    <input type=\"button\" value=\"Дальше\" id=\"signup_btn\"></input>
    <input type=\"reset\" value=\"Сбросить\" id=\"signup_reset_btn\"></input>
  </form>
  ";

  $reg_html = preg_replace("/%hash%/", $_SESSION['hash'], $reg_html);
  return $reg_html;
}

function cabinet_document_text($id, $mysqli)
{
	$res = $mysqli->query('select d.doc_ser,d.doc_num, DATE_FORMAT(d.doc_date,"%d.%m.%Y")
    as doc_date,d.doc_where,d.doc_code,DATE_FORMAT(d.doc_period,"%d.%m.%Y")
    as doc_period,t.name
    from olimp_docs_ic as d left join olimp_doc_types as t on (t.id=d.doc_type) where d.id=' . $id);
	$row = $res->fetch_assoc();
	$document_ic = $row['name'] .
		($row['doc_ser'] ? ' серия: ' . $row['doc_ser'] : '') .
		($row['doc_num'] ? ' номер: ' . $row['doc_num'] : '') .
		($row['doc_where'] ? ' выдан: ' . $row['doc_where'] : '') .
		($row['doc_date'] ? ' дата выдачи: ' . $row['doc_date'] : '') .
		($row['doc_code'] ? ' код подразделения: ' . $row['doc_code'] : '') .
		($row['doc_period'] != '00.00.0000' ? ' действительно до: ' . $row['doc_period'] : '');
	return $document_ic;
}
function cabinet_address_form($id, $tpl, $varname, $data, $mysqli)
{
	$ok = false;
	if ($id > 0) {
		$res = $mysqli->query('select country,zip_code,region,area,city,street,house,building,apartament,addr_text,kladr_street
      from olimp_address where id=' . $id);
		if ($row = $res->fetch_assoc()) $ok = true;
	}
	if (!$ok) {
		$row['country'] = 0;
		$row['zip_code'] = '';
		$row['region'] = '';
		$row['area'] = '';
		$row['city'] = '';
		$row['street'] = '';
		$row['house'] = '';
		$row['building'] = '';
		$row['apartament'] = '';
		$row['addr_text'] = '';
		$row['kladr_street'] = '';
	}
	if (is_array($data)) {
		$row['country'] = $data['country'];
		$row['zip_code'] = $data['zip_code'];
		$row['region'] = $data['region'];
		$row['area'] = $data['area'];
		$row['city'] = $data['city'];
		$row['street'] = $data['street'];
		$row['house'] = $data['house'];
		$row['building'] = $data['building'];
		$row['apartament'] = $data['apartament'];
		$row['addr_text'] = $data['addr_text'];
		$row['kladr_street'] = $data['kladr_street'];
	}
	$countrys = '';
	$res2 = $mysqli->query('select id,name from olimp_countrys');
	while ($row2 = $res2->fetch_assoc()) $countrys .= '<option value=' . $row2['id'] . ($row2['id'] == $row['country'] ? ' selected' : '') . '>' . $row2['name'] . '</option>';
	
	$regionName = '';
	$res2 = $mysqli->query('select id,name,socr from kladr_regions where id = "'. $row['region'] .'"');
	if ($row2 = $res2->fetch_assoc()) {
		$regionName = $row2['name'] . ' ' . $row2['socr'];
	}

	$address_form = preg_replace(
		array('/%countrys%/', '/%zip_code%/', '/%region%/', '/%regionId%/', '/%area%/', '/%city%/', '/%street%/', '/%house%/', '/%building%/', '/%apartament%/', '/%addr_text%/', '/%kladr_street%/', '/%varname%/'),
		array($countrys, $row['zip_code'], $regionName, $row['region'], $row['area'], $row['city'], $row['street'], $row['house'], $row['building'], $row['apartament'], $row['addr_text'], $row['kladr_street'], $varname),
		is_array($tpl) ? $tpl['address_form'] : $tpl
	);

	return $address_form;
}

function cabinet_address_check($data, $message, $req = true)
{
	if (empty($data['country'])) {
		if ($req) {
			return  $message . ' Не введена страна';
		} else {
			return  '';
		}
	}
	if (!preg_match('/\d{6,}/', $data['zip_code'])) {
		return $message . ' Неверный формат индекса или индекс не введен.';
	}
	if ($data['country'] == 1) {
		return '';
		//      if (($data['region']<1) || ($data['region']>99))
		//		{	return $message.' Неверный регион (нужно выбрать из списка).';		}
		//		if (empty($data['street']))
		//		{	return $message.' Не введено название улицы.';		}
		//		if (empty($data['house']))
		//		{	return $message.' Не введен номер дома.';		}
	} else {
		if (empty($data['addr_text'])) {
			return $message . ' Не введен текст адреса.';
		}
	}
	return '';
}

function cabinet_address_approve_kladr($id, $mysqli){
    $data = cabinet_convert_dadata($id, $mysqli);
    if(empty($data)) return 0;
    
    $_SESSION['dadata_address' . $id] = '';

    if ($id > 0) {
		if ($mysqli->query('update olimp_address set ' .
			'country=1,' .
			'zip_code="' . $data['zip_code'] . '", ' .
			'region=' . ($data['region'] + 0) . ', ' .
			'area="' . $data['area'] . '", ' .
			'city="' . $data['city'] . '", ' .
			'street="' . $data['street'] . '", ' .
			'house="' . $data['house']  . '", ' .
			'isKladrApproved="Y"' .
			' where id=' . $id)) return $id;
	}        
}

function cabinet_convert_dadata($id, $mysqli) {
  $data = $_SESSION['dadata_address'. $id];
  if(empty($data)) {
      return '';
  }

$data = $data['data'];

  $res = $mysqli->query('select id from kladr_regions where locate(name,"'. $data['region'] .'") > 0');
if ($row = $res->fetch_assoc()) {
  $region = $row['id'];
}

  if(empty($data['city_with_type'])) {
      $city = $data['settlement_with_type'];
  } else {
      $city = $data['city_with_type'];
  }

  $country=1;
  $postal_code = $data['postal_code'];
  $area = $data['area_with_type'];
  $street = $data['street_with_type'];

  $house = $data['house_type'] . ' ' . $data['house'];
  if($data['block']) {
      $house .= ' ' .$data['block_type'] . ' ' . $data['block'];
  }

  $fields = array(
      "country" => $country,
      "region" => $region,
      "zip_code" => $postal_code,
      "area" => $area,
      "city" => $city,
      "street" => $street,
      "house" => $house,
  );
  return $fields;
}

function cabinet_address_text($id, $mysqli){
	$res = $mysqli->query('select  a.country as country,c.name as country_name,a.zip_code,concat_ws(" ",r.name,r.socr) as region,a.area,a.city,a.street,a.house,a.building,a.apartament,a.addr_text,a.kladr_street from olimp_address as a left join olimp_countrys as c on (c.id=a.country) left outer join kladr_regions as r on (r.id=a.region) where a.id=' . $id);

	$row = $res->fetch_assoc();
	$w = $row['country_name'];
	if ($row['zip_code']) $w .= ($w ? ', ' : '') . $row['zip_code'];
	if ($row['country'] == 1) {
		if ($row['region']) $w .= ($w ? ', ' : '') . $row['region'];
		if ($row['area']) $w .= ($w ? ', ' : '') . $row['area'];
		if ($row['city']) $w .= ($w ? ', ' : '') . $row['city'];
		if ($row['street']) $w .= ($w ? ', ' : '') . $row['street'];
		if ($row['house']) $w .= ($w ? ', ' : '') . $row['house'];
		if ($row['building']) $w .= ($w ? ', корп.' : '') . $row['building'];
		if ($row['apartament']) $w .= ($w ? ', кв.' : '') . $row['apartament'];
	}
	if ($row['addr_text']) $w .= ($w ? ', ' : '') . $row['addr_text'];

	return $w;
}

function cabinet_address_text_kladr($id, $mysqli){
  global $dadata;
	$w = '';
	$res = $mysqli->query('select kladr_street, isKladrApproved from olimp_address where id=' . $id);
	$row = $res->fetch_assoc();

	// не предлагаем менять адрес, если кладр не найден, либо адрес из ФИАС уже одобрен пользователен
	if (empty($row['kladr_street'])) {
		return $w;
	} else if ($row['isKladrApproved'] == 'Y') {
		return $w;
	}

	$fields = array("query" => $row['kladr_street'], "count" => 1);
	$result = $dadata->findById("address", $fields);
	$data = $result['suggestions'][0];
    $_SESSION['dadata_address' . $id] = $data;
	$country = $data['data']['country'];
	$address = $data['unrestricted_value'];

	if ($country == 'Россия') {
		$w = $country;
		if ($address) $w .= ($w ? ', ' : '') . $address;
	}
	return $w;
}