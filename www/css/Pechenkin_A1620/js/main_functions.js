// Путь к php-скрипту, обрабатывающему ajax-запросы.
// let getPage_path = ""; // Путь на локальном компьютере. 
let getPage_path = "ajax/Pechenkin_A1620/"; // Путь на локальном сервере. 

let page_id = "page_id";

function get_section_and_page(pagehref) {
  let page_path = pagehref.split('/');
  let ob = {
    section: page_path[0],
    page: page_path[1]
  };
  return ob;
}
function save_last_update(section, page, content) {
  $cache[section][page + "_last_update"] = content;
}
function save_page_in_cache(section, page, content) {
  $cache[section][page] = content;
}
function is_page_in_cache(section, page) { // for debug: is_page_in_cache("about", "statement");.
  return !($cache[section][page] == undefined);
}
function load_page_from_website(page_identifyer) {
  let jsonData = "";
  $.ajax({
    type: 'POST',
    url: getPage_path + 'getPage.php',
    data: "page_identifyer=" + JSON.stringify(page_identifyer),
    success: function (response) {
      jsonData = JSON.parse(response)
      save_page_in_cache(page_identifyer.section, page_identifyer.page, jsonData.content);
      clearPageContent();
      buildPage(page_identifyer.section, page_identifyer.page);
      removeLoadingSpiner();
      console.log("removeLoadingSpiner");
    },
    error: function () {
      if (is_page_in_cache(page_identifyer.section, page_identifyer.page)) {
        let msg = "Не удалось обновить страницу с сервера.<br/>" +
          "Дата последнего обновления страницы на сервере:<br/>" +
          $cache[page_identifyer.section][page_identifyer.page + "_last_update"]
        // console.log("Ошибка заргузки данных с сервера. Данные получены из кэша.");
        clearPageContent();
        outWarningMsg(msg);
        buildPage(page_identifyer.section, page_identifyer.page);
      } else {
        clearPageContent();
        outWarningMsg("Ошибка загрузки!");
      }
      removeLoadingSpiner();
      console.log("removeLoadingSpiner");
    }
  })
}
function get_last_update(page_identifyer, prev_date) { // Получить от сервера дату последнего обновления страницы.
  let jsonData = "";  
  $.ajax({
    type: 'POST',
    url: getPage_path + 'getPage.php',
    data: "page_last_update=" + JSON.stringify(page_identifyer),
    success: function (response) {
      jsonData = JSON.parse(response);
      save_last_update(page_identifyer.section, page_identifyer.page, jsonData.date);
      if (prev_date != $cache[page_identifyer.section][page_identifyer.page + "_last_update"]  // Если страница была обновлена
        || $cache[page_identifyer.section][page_identifyer.page] == undefined                // или страница не была загружена.
      ) {
        // Загрузить страницу с сервера.        
        load_page_from_website(page_identifyer);
      } else if ($cache[page_identifyer.section][page_identifyer.page] != undefined) { // Иначе (если страница не обновлялась), если страница есть в кэше - загрузить страницу из кэша.
        clearPageContent();
        buildPage(page_identifyer.section, page_identifyer.page);
      } else { // Иначе (при первой загрузке страницы, (страницы нет в кэше, даты обновлений не сопадают)).
        load_page_from_website(page_identifyer);
      }

    },
    error: function () { // Если не удалось загрузить дату последнего обновления страницы.
      if (is_page_in_cache(page_identifyer.section, page_identifyer.page)) {
        // Загрузить страницу из кэша.
        clearPageContent();
        let msg = "Ошибка загрузки даты последнего обновления. Страница загружина из кэша.<br/>" +
          "Дата последнего обновления страницы на сервере: " + $cache[page_identifyer.section][page_identifyer.page + "_last_update"] + "<br/>";
        outWarningMsg(msg);
        buildPage(page_identifyer.section, page_identifyer.page);
      } else {
        // $("#page-main-content").html("Ошибка загрузки.").addClass("msg_warning");
        clearPageContent();
        outWarningMsg("Ошибка загрузки.");
      }
      removeLoadingSpiner();
      console.log("removeLoadingSpiner");
    }
  })
}
function load_page(page_href) {
  // Получить данные от сервера с помощью ajax.
  page_id = page_href;
  console.log("page_id: ", page_id);
  let page_identifyer = get_section_and_page(page_href);
  history.pushState({ page: page_id }, '');
  outLoadingSpiner();
  console.log("outLoadingSpiner");
  get_last_update(page_identifyer, $cache[page_identifyer.section][page_identifyer.page + "_last_update"]);  
  
  currentLocation = page_href;
  switch(currentLocation){
    case 'user/login':
      currentLocation = "cabinet/profile";
      break;
    case 'user/signup':
      currentLocation = "novice/reg_user";
      break;
    case 'contact/contact':
      currentLocation = "contact";
      break;  
  }
  $("#go-to-desktop").attr("href", "http://dev.ehope.ru/" + currentLocation + "?use_desktop=yes");
}

console.log("functions.js");
let $cache = { // For debug : $cache[about][statement];.
  cabinet: {
    profile_last_update: undefined,
    profile: undefined,

    documents_last_update: undefined,
    documents: undefined,
    stages_last_update: undefined,
    stages: undefined,
    distance_last_update: undefined,
    distance: undefined,
    works_last_update: undefined,
    works: undefined,
    results_last_update: undefined,
    results: undefined,

  },
  main: { // Главная страница.
    main_last_update: undefined,
    main: undefined,
  },
  about: { // Section # 1.
    statement_last_update: undefined, // Page # 1.      
    statement: undefined,
    reglament_last_update: undefined, // Page # 2.
    reglament: undefined,
    istoriya_olimpiadi_last_update: undefined,  // Page # 3.
    istoriya_olimpiadi: undefined,
    organizatori_last_update: undefined,  // Page # 4.
    organizatori: undefined,
    partner_last_update: undefined, // Page # 5.
    partner: undefined,
    gallery_last_update: undefined, // Page # 6.
    gallery: undefined,
  },
  actual: { // Section # 2.
    grafik_last_update: undefined,
    grafik: undefined,
  },
  olymp: { // Section # 3.
    nextnear_last_update: undefined, // Page # 1.
    nextnear: undefined,
    metodic_last_update: undefined, // Page # 2.
    metodic: undefined,
    liter_last_update: undefined, // Page # 3.
    liter: undefined,
    tasks_last_update: undefined, // Page # 4.
    tasks: undefined,
    energy_school_last_update: undefined, // Page # 5.
    energy_school: undefined,
    idea_last_update: undefined, // Page # 6.
    idea: undefined,
    orgadd_last_update: undefined, // Page # 7.
    orgadd: undefined,
    health_last_update: undefined, // Page # 8.
    health: undefined,
    obrazci_last_update: undefined, // Page # 9.
    obrazci: undefined,
    pdf_last_update: undefined, // Page # 10.
    pdf: undefined,
  },
  novice: { // Section # 4.
    kak_prinyat_uchastie_last_update: undefined,
    kak_prinyat_uchastie: undefined,
    otveti_na_voprosi_last_update: undefined,
    otveti_na_voprosi: undefined,
  },
  lastyear: { // Section # 5.
    fisgia_last_update: undefined,
    fisgia: undefined,
    actionplan_20_last_update: undefined,
    actionplan_20: undefined,
  },
  arch: { // Section # 6.
    arch_xxi_last_update: undefined, // Page # 1.
    arch_xxi: undefined,
    arch_xx_last_update: undefined, // Page # 2.
    arch_xx: undefined,
    arch_xix_last_update: undefined, // Page # 3.
    arch_xix: undefined,
    arch_xviii_last_update: undefined, // Page # 4.
    arch_xviii: undefined,
    yeartwokseventeen_last_update: undefined, // Page # 5.
    yeartwokseventeen: undefined,
    yeartwoksixteen_last_update: undefined, // Page # 6.
    yeartwoksixteen: undefined,
    yeartwokfifteen_last_update: undefined, // Page # 7.
    yeartwokfifteen: undefined,
    yeartwokfourteen_last_update: undefined, // Page # 8.
    yeartwokfourteen: undefined,
    yeartwokthirteen_last_update: undefined, // Page # 9.
    yeartwokthirteen: undefined,
    yeartwoktwelve_last_update: undefined, // Page # 10.
    yeartwoktwelve: undefined,
    yeartwokeleven_last_update: undefined, // Page # 11.
    yeartwokeleven: undefined,
    yeartwokten_last_update: undefined, // Page # 12.
    yeartwokten: undefined,
    yeartwoknine_last_update: undefined, // Page # 13.
    yeartwoknine: undefined,
    yeartwokeight_last_update: undefined, // Page # 14.
    yeartwokeight: undefined,
  },
  contact: { // Section # 7.
    contact_last_update: undefined,
    contact: undefined,
  },
  // Кэш меню.
  left_menu_trigger: undefined,
  left_menu_popup: undefined,
  // Кэш кнопок-приглашений регистрации и входа.

  login_and_signup_btns: { // Кнопки приглашения к регистрации и входу.
    allContent_last_update: undefined,
    allContent: undefined,
  },
  // Кэш форм регистрации и входа. FEXME: нужно ли их кэшировать?
  user: {
    login_last_update: undefined,
    login: undefined,
    signup_last_update: undefined,
    signup: undefined,
  },
}
// Кнопка назад. TODO.
function popState(e) {
  console.log("e.state: ", e.state);
  console.log("e.state.page: ", e.state.page);
  console.log("config.mainPage: ", $cache.main);
  let page = (e.state && e.state.page) || $cache.main;
  // console.log("page = ", page);
  load_page(page);
}
class loginFormClass {
  login_form = undefined
  login_status = undefined
  login_login = undefined
  login_password = undefined
  login_btn = undefined
  login_reset_btn = undefined
  constructor() {
    this.login_form = $('#login_form');
    // this.login_status = $('#login_form select :selected');
    this.login_login = $('#login_login');
    this.login_password = $('#login_password');
    this.login_btn = $('#login_btn');
    this.login_reset_btn = $('#login_reset_btn');

    this.login_login.val("PechenkinYS");
    this.login_password.val("LY-fV51y");
    // this.login_btn.click(this.validateLoginForm, );
    this.login_btn.click((e) => { // Валидация данных в форме логина, TODO в случае успешного ввода - отправить их на сервер, получить ответ...
      let self = this;
      // self.login_status = $('#login_form select :selected');
      // let status_num = self.login_status.val().trim();
      let login_text = self.login_login.val().trim();
      let password_text = self.login_password.val().trim();
      // TODO Обрабатывать все поля формы.

      // alert("login_login: ",$('#login_login').val());
      if (self.login_login.val().trim() === "" || self.login_password.val().trim() === "") {
        // alert("Поле логин должно быть заполнено!");
        $("#login_form_msg").removeClass("msg_success");
        $("#login_form_msg").addClass("msg_warning");
        $("#login_form_msg").text("Данные введены некорректно!");
      } else {
        // alert("login_login: ", self.login_login.val());
        // alert("login_login: ", login_login.val());
        // Тестовый вывод данных.
        $("#login_form_msg").removeClass("msg_warning");
        $("#login_form_msg").addClass("msg_success");
        // Отправить данные на сервер.
        $.ajax({
          type: 'POST',
          url: getPage_path + 'login.php',
          data: "login=" + JSON.stringify(login_text) + "&password=" + JSON.stringify(password_text),
          success: function (response) {
            let jsonData = JSON.parse(response);
            if (jsonData.login_status == 1) { // Успешный вход на сайт.
              clearPageContent();
              // $("#page-main-content").append("<div id=\"login_status\">Добро пожаловать, <br/>" + jsonData.fio + "!<br/> id: " + jsonData.uid + ".</div>");
              // $("#login_status").addClass("msg_success");
              outSuccessMsg("<div id=\"login_status\">Добро пожаловать, <br/>" + jsonData.fio + "!<br/> id: " + jsonData.uid + ".</div>");
              changeLeftMenu();
              is_login = true;
            } else { // Ошибка входа на сайт.
              $("#login_form_msg").removeClass("msg_success");
              $("#login_form_msg").addClass("msg_warning");
              $("#login_form_msg").text("Ошибка входа на сайт.");
            }
          },
          error: function () {
            $("#login_form_msg").removeClass("msg_success");
            $("#login_form_msg").addClass("msg_warning");
            $("#login_form_msg").text("Ошибка входа на сайт. Не удалось установить подключение.");
          }
        });
      }
    });
    this.login_reset_btn.click((e) => {
      if ($('#login_form_msg') != undefined) {
        $('#login_form_msg').text('');
        $("#login_form_msg").removeClass("msg_warning");
        $("#login_form_msg").removeClass("msg_success");
      }
    });
  }
}
class signupFormClass {
  signup_form = undefined
  signup_surname = undefined
  signup_name = undefined
  signup_patronymic = undefined
  signup_birthday = undefined
  signup_email = undefined
  signup_questionA = undefined
  signup_questionB = undefined
  signup_answer = undefined
  signup_btn = undefined
  signup_reset_btn = undefined
  constructor() {
    this.signup_form = $('#signup_form');
    this.signup_regstep = $('#regStep');
    this.signup_surname = $('#regSurename');
    this.signup_name = $('#regName');
    this.signup_patronymic = $('#regPatronymic');
    this.signup_birthday = $('#regBirthday');
    this.signup_email = $('#regEmail');
    this.signup_questionA = $('#signup_form select :selected').val();
    this.signup_questionB = $('#regQuestionB');
    this.signup_answer = $('#regAnswer');
    this.hash = $('#hash');
    this.signup_btn = $('#signup_btn');
    this.signup_reset_btn = $('#signup_reset_btn');

    this.signup_surname.val("Печенкин");
    this.signup_name.val("Ярослав");
    this.signup_patronymic.val("Станиславович");
    this.signup_birthday.val("25.03.1999");
    this.signup_email.val("email@yandex.ru");
    // this.signup_questionA.val("1"); !!!
    this.signup_questionB.val("abcd");
    this.signup_answer.val("1234");

    this.signup_btn.click((e) => { // Обработка регистрации.
      let self = this;
      // Проверить, что поля не пусты.
      let regstep = self.signup_regstep.val();
      let surname = self.signup_surname.val().trim();
      let name = self.signup_name.val().trim();
      let patronymic = self.signup_patronymic.val().trim();
      let birthday = self.signup_birthday.val().trim();
      let email = self.signup_email.val().trim();
      let questionA = $('#signup_form select :selected').val();
      let questionB = self.signup_questionB.val().trim();
      let answer = self.signup_answer.val().trim();
      let hash = self.hash.val().trim();
      // Тестовый вывод данных.
      if (surname === "" || name === "" || patronymic === "" || birthday === "" || email === "" ||
        questionA === "" && questionB === "" || answer === "") {
        $("#signup_form_msg").removeClass("msg_success");
        $("#signup_form_msg").addClass("msg_warning");
        $("#signup_form_msg").text("Данные введены некорректно!");
      } else {
        $("#signup_form_msg").removeClass("msg_warning");
        $("#signup_form_msg").addClass("msg_success");
        let msg = `Фамилия : "${surname}"; Имя: "${name}"; Отчество: "${patronymic}";
        Дата рождения : "${birthday}"; e-mail: "${email}"; вопрос: "${questionA}"; свой: "${questionB}"; ответ: "${answer}";`;
        // $("#signup_form_msg").text(msg);

        // Отправить данные на сервер.
        $.ajax({
          type: 'POST',
          url: getPage_path + 'registration.php',
          data: "regStep=" + regstep + "&regSurename=" + surname + "&regName=" + name +
            "&regPatronymic=" + patronymic + "&regBirthday=" + birthday +
            "&regEmail=" + email + "&regQuestionA=" + questionA +
            "&regQuestionB=" + questionB + "&regAnswer=" + answer + "&hash=" + hash,
          success: function (response) {
            let jsonData = JSON.parse(response);

            if (jsonData.regStep == 2) {
              // Сформировать страницу для выбора логинов.
              // $("#signup_form_msg").text("Стадия регистрации: " + jsonData.regstep);  
              outContentRegStepTwo(jsonData).then(function () {
                return initRegStepTwo('#signup_btn_stepTwo');
              });
              // Инициализировать обработчик формы.
            } else if (jsonData.regStep == 3) {
              // Сформировать страницу результатов регистрации (успешная регистрация).
              $("#signup_form_msg").text("Стадия регистрации: " + jsonData.regStep);
              outContentRegStepThree(jsonData);
            } else {
              // Ошибка регистрации.
              $("#signup_form_msg").removeClass("msg_success");
              $("#signup_form_msg").addClass("msg_warning");
              // $("#signup_form_msg").text("Ошибка регистрации. " + jsonData.error + ", " + jsonData.error_text);
              $("#signup_form_msg").text("Ошибка регистрации. " + jsonData.error_text);
            }

            /*
            if(jsonData.signup_status == 1){ // Успешная регистрация.
              $("#signup_form_msg").removeClass("msg_warning");
              $("#signup_form_msg").addClass("msg_success");
              $("#signup_form_msg").html("Полученные данные:<br/>"
              + "reg_opened: " + jsonData.reg_opened + ", "
              + "<br/>regstep: " + jsonData.regstep + ","
              + "<br/>surname: " + jsonData.surname + ","
              + "<br/>name: " + jsonData.name + ","
              + "<br/>patronymic: " + jsonData.patronymic + ","
              + "<br/>birthday: " + jsonData.birthday + ","
              + "<br/>email: " + jsonData.email + ","
              + "<br/>questionA: " + jsonData.questionA + ","
              + "<br/>questionB: " + jsonData.questionB + ","
              + "<br/>answer: " + jsonData.answer + "."
              ); 
            } else if (jsonData.signup_status == 0){ // Ошибка - регистрация участников закрыта.
              $("#signup_form_msg").removeClass("msg_success");
              $("#signup_form_msg").addClass("msg_warning");
              $("#signup_form_msg").text("Ошибка регистрации. Регистрация закрыта.");  
            } else { // Ошибка регистрации.
              $("#signup_form_msg").removeClass("msg_success");
              $("#signup_form_msg").addClass("msg_warning");
              $("#signup_form_msg").text("Ошибка регистрации. " + jsonData.error);  
            }*/
          },
          error: function () {
            $("#signup_form_msg").removeClass("msg_success");
            $("#signup_form_msg").addClass("msg_warning");
            $("#signup_form_msg").text("Ошибка регистрации. Не удалось установить подключение.");
          }
        });
      }
    })
    this.signup_reset_btn.click((e) => {
      if ($('#signup_form_msg') != undefined) {
        $('#signup_form_msg').text('');
        $("#signup_form_msg").removeClass("msg_warning");
        $("#signup_form_msg").removeClass("msg_success");
      }
    })
  }
}
function initLoginForm() {
  loginForm = new loginFormClass();
}
function initSignupForm() {
  signupForm = new signupFormClass();
}
function outContentRegStepTwo(jsonData) {
  return new Promise(function (resolve) {
    // Сформировать select для выбора логинов.
    let nics_select = "\
      <input class = \"regStepTwo\" type=\"radio\" name=\"nic\" value=\"" + jsonData.nics[0] + "\" id=\"" + jsonData.nics[0] + "\" checked>\
      <label class = \"regStepTwo\" for=\"nic\">" + jsonData.nics[0] + "</label>\
      <br/>\
    ";
    for (let i = 1; i < jsonData.nics.length; i++) {
      nics_select += "\
        <input class = \"regStepTwo\" type=\"radio\" name=\"nic\" value=\"" + jsonData.nics[i] + "\" id=\"" + jsonData.nics[i] + "\">\
        <label class = \"regStepTwo\" for=\"nic\">" + jsonData.nics[i] + "</label>\
        <br/>\
      ";
    }
    // Вывести форму.
    $("#page-main-content").html(" \
      <div class=\"login_and_signup_label page-header\">Регистрация. Шаг 2.</div>\
      <div class=\"page-header3\">Выберите логин пользователя</div>\
      <form action=\"\" method=\"get\" class=\"login_and_signup_form signup_form\" id=\"signup_form_regStep2\"> \
        <input type=\"hidden\" name=\"regStep\" id=\"regStep\" value=\"" + jsonData.regStep + "\">  \
        <input type=\"hidden\" name=\"regSurename\" id=\"regSurename\" value= " + jsonData.sname + "> \
        <input type=\"hidden\" name=\"regName\" id=\"regName\" value=" + jsonData.name + "> \
        <input type=\"hidden\" name=\"regPatronymic\" id=\"regPatronymic\" value=" + jsonData.pname + "> \
        <input type=\"hidden\" name=\"regBirthday\" id=\"regBirthday\" value=" + jsonData.birthday + "> \
        <input type=\"hidden\" name=\"regEmail\" id=\"regEmail\" value=" + jsonData.email + "> \
        <input type=\"hidden\" name=\"regQuestionA\" id=\"regQuestionA\" value=" + jsonData.questionA + "> \
        <input type=\"hidden\" name=\"regQuestionB\" id=\"regQuestionB\" value=" + jsonData.questionB + "> \
        <input type=\"hidden\" name=\"regAnswer\" id=\"regAnswer\" value=" + jsonData.answer + "> \
        <input type=\"hidden\" name=\"hash\" id=\"hash\" value=" + jsonData.hash + "> \
        " + nics_select + "\
        <input type=\"button\" value=\"Дальше\" id=\"signup_btn_stepTwo\"></input>\
      </form> \
      error_text" + jsonData.error_text + "<br/>\
    ");
    // $("#page-main-content").html(jsonData.surname);
    resolve();
  });
}

function initRegStepTwo(elem) {
  return new Promise(function (resolve) {
    // Считать данные из новой формы.
    let regstep = $('#regStep').val();
    let surname = $('#regSurename').val().trim();
    let name = $('#regName').val().trim();
    let patronymic = $('#regPatronymic').val().trim();
    let birthday = $('#regBirthday').val().trim();
    let email = $('#regEmail').val().trim();
    let questionA = $('#regQuestionA').val().trim();
    let questionB = $('#regQuestionB').val().trim();
    let answer = $('#regAnswer').val().trim();
    let hash = $('#hash').val().trim();

    let nick = $('input[name=nic]:checked').val().trim();
    let btn = $('#signup_btn_stepTwo');

    this.signup_form = $('#signup_form');
    $(elem).click(function () {
      $.ajax({
        type: 'POST',
        url: getPage_path + 'registration.php',
        data: "regStep=" + regstep + "&regSurename=" + surname + "&regName=" + name +
          "&regPatronymic=" + patronymic + "&regBirthday=" + birthday +
          "&regEmail=" + email + "&regQuestionA=" + questionA +
          "&regQuestionB=" + questionB + "&regAnswer=" + answer + "&hash=" + hash
          +"&nic=" + nick,
        success: function (response) {
          // При успешной регистрации вывести результаты.
          let jsonData = JSON.parse(response);
          // $("#page-main-content").html(" \
          //   <div class=\"login_and_signup_label page-header\">Регистрация. Шаг 3.</div>\
          // ");
          outContentRegStepThree(jsonData);
        },
        error: function () {
          $("#page-main-content").html("Ошибка соединения.");
        }
      });
    });
    resolve();
  });
}

function outContentRegStepThree(jsonData){
  $('#page-main-content').html(" \
    <div class=\"page-header3\">Регистрация успешно завершена</div>\
    Ф.И.О.: " + jsonData.sname + " " + jsonData.name + " " + jsonData.pname + "<br/>\
    Регистрационный номер: " + jsonData.regnum + "<br/>\
    Логин: " + jsonData.nic + "<br/>\
    Пароль: " + jsonData.pwd + "<br/>\
    regStep: " + jsonData.regStep + "<br/>\
    error_text" + jsonData.error_text + "<br/>\
  ");
}