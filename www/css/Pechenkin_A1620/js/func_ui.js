let right_menu_opened = 0;
function rightMenuPopoupClose($rightMenu) { // При открытии меню.
  if (right_menu_opened == 0) {
    $rightMenu.show(0);
    // $('body').addClass('body_pointer');
    $(".right-menu-trigger").addClass('menu-trigger-opened');
    $(".right-menu-trigger").removeClass('menu-trigger');
  }
  $rightMenu.animate(
    { left: parseInt($rightMenu.css('left'), 10) == 0 ? + $rightMenu.outerWidth() : 0 }
    , {
      duration: 200,
      complete: function () {
        if (right_menu_opened == 0) {
          right_menu_opened = 1;
        } else {
          right_menu_opened = 0;
        }
      }
    });
  if (right_menu_opened == 1) { // При закрытии меню.
    $rightMenu.hide(0);
    closeSumbenu();
    // $('body').removeClass('body_pointer');
    $(".right-menu-trigger").removeClass('menu-trigger-opened');
    $(".right-menu-trigger").addClass('menu-trigger');
  }
  return false;
}
function rightMenuClose($rightMenu) {
  if (right_menu_opened == 1) { // Если правое меню открыто
    rightMenuPopoupClose($rightMenu);
  }
  return false;
}
let left_menu_opened = 0;
function leftMenuPopoupClose($leftMenu) {
  if (left_menu_opened == 0) {
    $leftMenu.show(0);
    // $('body').addClass('body_pointer');
    $(".left-menu-trigger").addClass('menu-trigger-opened');
    $(".left-menu-trigger").removeClass('menu-trigger');
  }
  $leftMenu.animate(
    { left: parseInt($leftMenu.css('left'), 10) == 0 ? - $leftMenu.outerWidth() : 0 }
    , {
      duration: 200,
      complete: function () {
        if (left_menu_opened == 0) {
          left_menu_opened = 1;
        } else {
          left_menu_opened = 0;
        }
      }
    }
  );
  if (left_menu_opened == 1) { // При закрытии меню.
    $leftMenu.hide(0);
    closeSumbenu();
    // $('body').removeClass('body_pointer');
    $(".left-menu-trigger").removeClass('menu-trigger-opened');
    $(".left-menu-trigger").addClass('menu-trigger');
  }
  return false;
}
function leftMenuClose($leftMenu) {
  if (left_menu_opened == 1) { // Если правое меню открыто
    leftMenuPopoupClose($leftMenu);
  }
  return false;
}
function closeSumbenu() { // Закрытие всех подменю.
  $('ul[class="submenu"]').each(function () {
    $(this).css("display", "none");
  });
}
function clearPageContent() {
  $("#page-content-wrap").html(" \
  <div id=\"page-content-wrap\"> \
  <div id=\"page-msg\"> \
  </div> \
  <div id=\"page-main-content\"> \
    <div class=\"login-and-signup-btns\" id=\"login-and-signup-btns\"> \
    </div> \
  </div> \
</div> \
  ");
}
function outWarningMsg(msg) {
  $("#page-msg").removeClass('msg_success').addClass('msg_warning').html(msg);
}
function outSuccessMsg(msg) {
  $("#page-msg").removeClass('msg_warning').addClass('msg_success').html(msg);
}
function appendPageMainContent(content) {
  $("#page-main-content").append(content);
}
function buildPage(section, page) { // Строит страницу, данные о которой загружены в кэш.
  switch (section) {
    case "cabinet":
      switch (page) {
        case "documents": // Документы.
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          $.getScript('css/Pechenkin_A1620/js/cabinet_documents.js'); // 
          break;
        case "stages": // Регистрация.
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          // Получить данные об этапах.
          getStages().then(function () {
            console.log("after getStages");
          });
          break;
        case "profile":
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          getProfile();
          break;
		case "distance":
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          getDistance();
          
          break;
        default:
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
      }
      break;
    case "main":
      switch (page) {
        case "main":
          if (!is_login) {
            show_signup_login_btns();
          }
          appendPageMainContent($cache[section][page]);
          // $("#page-main-content").append($cache[section][page]);

          break;
      }
      break;
    case 'olymp':
      switch (page) {
        case 'nextnear':
        case 'liter':
          hide_signup_login_btns();
          // Инициализация ссылок с помощью Promise (для последловательного выполенения функций).
          outContent(section, page).then(function () {
            return initListener('#page-main-content a[class= \"inner-a\"]');
          }).then(function () {
            console.log('Promise done!');
          });
          break;

        default:
          hide_signup_login_btns();
          appendPageMainContent($cache[section][page]);
          // $("#page-main-content").append($cache[section][page]);
          break;
      }
      break;
    case 'novice':
      switch (page) {
        case 'kak_prinyat_uchastie':
          hide_signup_login_btns();
          // Инициализация ссылко с помощью Promise (для последловательного выполенения функций).
          outContent(section, page).then(function () {
            return initListener('#page-main-content a[class= \"inner-a\"]');
          }).then(function () {
            console.log('Promise done!');
          });
          break;
        default:
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          break;
      }
      break;
    case 'lastyear':
      switch (page) {
        case 'actionplan_20':
          hide_signup_login_btns();
          // Инициализация ссылко с помощью Promise (для последловательного выполенения функций).
          outContent(section, page).then(function () {
            return initListener('#page-main-content a[class= \"inner-a\"]');
          }).then(function () {
            console.log('Promise done!');
          });
          break;
        default:
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          break;
      }
      break;
    case 'user':
      switch (page) {
        case 'login':
          hide_signup_login_btns();
          $("#page-main-content").append($cache[section][page]);
          // Инициалзизировать класс для валидации.
          initLoginForm();
          break;
        case 'signup':
          // hide_signup_login_btns();
          // $("#page-main-content").append($cache[section][page]);
          outPageMainContentProm($cache[section][page]).then(function () {
            initSignupForm();
          });
          // Инициалзизировать класс для валидации.
          // initSignupForm();
          break;
      }
      break;
    default:
      hide_signup_login_btns();
      $("#page-main-content").append($cache[section][page]);
  }
}
function outContent(section, page) {
  return new Promise(function (resolve) {
    $("#page-main-content").append($cache[section][page]);
    resolve();
  });
}
function initListener(elem) {
  return new Promise(function (resolve) {
    $(elem).click(function (e) {
      e.preventDefault();
      load_page($(this).attr("href"));
    });
    resolve();
  });
}
function initListenerMenu(elem) { // Для инициализации группы
  return new Promise(function (resolve) {
    let $left_menu_popup = $('.left-menu-popup');
    $(elem).each(function (indx, element) {
      $(element).click(function (e) {
        e.preventDefault();
        leftMenuPopoupClose($left_menu_popup);
        load_page($(this).attr("href"));
      });
    });
    resolve();
  });
}
function initLogout(elem) {
  return new Promise(function (resolve) {
    let $left_menu_popup = $('.left-menu-popup');
    $(elem).click(function (e) {
      e.preventDefault();
      leftMenuPopoupClose($left_menu_popup);
      completeLogout();
    })
    resolve()
  });
}
function completeLogout() {
  $.ajax({
    type: 'POST',
    url: getPage_path + 'login.php',
    data: "logout",
    success: function (response) {
      jsonData = JSON.parse(response);
      if (jsonData['result'] == 'ok') {
        $("#page-main-content").html("<div>Выход произошел успешно</div>");
        is_login = false;
        window.location.href = "http://dev.ehope.ru";
      }
    },
    error: function () {
      $("#page-main-content").html("<div>Выход произошел успешно</div>");
    }
  });
}
function hide_signup_login_btns() {
  // console.log("sginup_login_butns: ", $cache.login_and_signup_btns.allContent);
  if(currentLocation != "main/main"){
    loginAndSignupBtns = $cache.login_and_signup_btns.allContent.detach();
    // loginBtn = $("#login-inv").detach();
    // signupBtn = $("#signup-inv").detach();
    // loginAndSignupBtns = $("#login-and-signup-btns").detach();
    console.log("loginAndSignupBtns");
  }
  $('#page-main-content').addClass("text-content");
}
function show_signup_login_btns() {
  $('#page-main-content').removeClass("text-content");
  $('#page-main-content').append($cache.login_and_signup_btns.allContent);
  // console.log("show sginup_login_btns: ", $cache.login_and_signup_btns.allContent);
  $('.loginTrigger').click(function (e) {
		e.preventDefault();
		load_page($(this).attr("href"));
	});
	$('.signupTrigger').click(function (e) {
		e.preventDefault();
		is_login = false;
		load_page($(this).attr("href"));
	});
}
function changeLeftMenu() {
  // Загрузить меню личного кабинета.
  $.ajax({
    type: 'POST',
    url: getPage_path + 'getPage.php',
    data: "page_identifyer=" + JSON.stringify({
      section: "cabinet",
      page: "left_menu"
    }),
    success: function (response) {
      jsonData = JSON.parse(response);
      outLeftMenu(jsonData.content).then(function () {
        return initListenerMenu('#left-menu-popup .inner-a');
      }).then(function () {
        return initLogout("#outbtn");
      });
    },
    error: function () {
      $(".left-menu-popup > ul").html("Перезагрузить меню.");
    }
  });
  // Инициализировать новые сслыки.
}

function outLeftMenu(content) {
  return new Promise(function (resolve) {
    $("#left-menu-login").remove();
    $("#left-menu-signup").remove();
    $("#left-menu-popup > ul").prepend(content);
    console.log("Promise 1");
    resolve();
  })
}

function outLoadingSpiner() {
  let code = "<div class=\"loading-overlay\">\
    <div class=\"loading-spinner\"> \
    </div> \
  </div>";
  clearPageContent();
  appendPageMainContent(code);
}
function removeLoadingSpiner() {
  $('.loading-overlay').remove();
}
function outPageMainContentProm(content) {
  return new Promise(function (resolve) {
    clearPageContent();
    appendPageMainContent(content);
    resolve();
  })
}


function getStages() { // Получить данные о стадиях олимпиады.
  return new Promise(function (resolve) {
    $.ajax({
      type: "POST",
      url: "ajax/Pechenkin_A1620/adaptedToAjax/engine_mobile.php",
      data: "page=cabinet/stages", // page_type=4001
      success: function (response) {
        let jsonData = JSON.parse(response);

        if (jsonData['error'] == 0) {
          console.log("error == " + 0);
          outCabinetStages(jsonData).then(function () {
            initCabinetStagesListeners(jsonData);
          }).then(function(){ // Инициализация раскрытия информации о стадиях.
            $('#completed_stages').click(function(){
              $('#tab-1').slideToggle(500);
              $('#tab-1-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });
            $('#qualif_stages').click(function(){
              $('#tab-2').slideToggle(500);
              $('#tab-2-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });
            $('#final_stages').click(function(){
              $('#tab-3').slideToggle(500);
              $('#tab-3-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });
            $('#training_stage').click(function(){
              $('#tab-4').slideToggle(500);
              $('#tab-4-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });

          });;

        } else if (jsonData['error'] > 0) {
          console.log("error > " + 0);
          $('.tab-stages').text(jsonData['error_text']).addClass('msg_warning').removeClass('msg_success');
        }
      },
      error: function () {
        console.log("page_type_4001 error");
        $('#tab-1').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-2').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-3').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-4').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
      }
    });
    resolve();
  });
}

function outCabinetStages(jsonData) {
  return new Promise(function (resolve) {
    if (jsonData['completed_stages'] !== ""){
      $('#tab-1').html(jsonData['completed_stages']);
      $('#tab-1').slideUp(0);
      $('#tab-1-arrow').addClass('arrow-down');
    } else {
      $('#tab-1').html('<div class="msg_warning">Информация отсутствует.</div>');
      $('#tab-1-arrow').addClass('arrow-up');
    }
    if (jsonData['qualifying_stages'] !== "") {
      $('#tab-2').html(jsonData['qualifying_stages']);
      $('#tab-2').slideUp(0);
      $('#tab-2-arrow').addClass('arrow-down');
    } else {
      $('#tab-2').html('<div class="msg_warning">Информация отсутствует.</div>');
      $('#tab-2-arrow').addClass('arrow-up');
    }
    if (jsonData['final_stages'] !== "") {      
      $('#tab-3').html(jsonData['final_stages']);
      $('#tab-3').slideUp(0);
      $('#tab-3-arrow').addClass('arrow-down');
    } else {
      $('#tab-3').html('<div class="msg_warning">Информация отсутствует.</div>');
      $('#tab-3-arrow').addClass('arrow-up');
    }
    if (jsonData['traning_stages'] !== "") {
      $('#tab-4').html(jsonData['traning_stages']);
      $('#tab-4').slideUp(0);
      $('#tab-4-arrow').addClass('arrow-down');
    } else {
      $('#tab-4').html('<div class="msg_warning">Информация отсутствует.</div>');
      $('#tab-4-arrow').addClass('arrow-up');
    }
    resolve();
  })
}

function initCabinetStagesListeners(jsonData) {
  return new Promise(function (resolve) {
  console.log('before for');
  for (let i = 0; i < jsonData['Listeners']['count']; i++) {
    console.log('first in for');
    let listener_type = jsonData['Listeners'][i]['type'];
    if (listener_type == 'registration') {
      console.log('in if');
      let stage_id = jsonData['Listeners'][i]['stage_id'];
      let form_id = "#formStageAction" + stage_id;
      let cmd = $(form_id + "> input[name=\"cmd\"]").val();
      let active_stage = $(form_id + "> input[name=\"active_stage\"]").val();
      $('#' + jsonData['Listeners'][i]['elem_id']).click(function (e) {
        e.preventDefault();
        // alert("#" + jsonData['Listeners'][i]['elem_id']);
        let request = "page=cabinet/stages&active_stage=" + active_stage + "&stage_id=" +
          + stage_id + "&cmd=" + cmd;
        console.log("registration");
        $.ajax({
          type: "POST",
          url: "ajax/Pechenkin_A1620/adaptedToAjax/engine_mobile.php",
          data: request, // page_type=4001
          success: function (response) {
            let jsonData = JSON.parse(response);
            if (jsonData['error'] == 0) {
              console.log("error == " + 0);
              outCabinetStages(jsonData).then(function () {
                initCabinetStagesListeners(jsonData);
              });

            } else if (jsonData['error'] > 0) {
              console.log("error > " + 0);
              $('.tab-stages').text(jsonData['error_text']).addClass('msg_warning').removeClass('msg_success');
            }
          },
          error: function () {
            $('#tab-1').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
            $('#tab-2').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
            $('#tab-3').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
            $('#tab-4').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
          }
        });
      });

      //.click(registrationListener, e);
    } else if (listener_type == "cancel_registration") {
      let stage_id = jsonData['Listeners'][i]['stage_id'];
      let form_id = "#formStageAction" + stage_id;
      let cmd = $(form_id + "> input[name=\"cmd\"]").val();
      let active_stage = $(form_id + "> input[name=\"active_stage\"]").val();
      $('#' + jsonData['Listeners'][i]['elem_id']).click(function (e) {
        e.preventDefault();
        // alert("#" + jsonData['Listeners'][i]['elem_id']);
        let request = "page=cabinet/stages&active_stage=" + active_stage + "&stage_id=" +
          + stage_id + "&cmd=" + cmd;
        $.ajax({
          type: "POST",
          url: "ajax/Pechenkin_A1620/adaptedToAjax/engine_mobile.php",
          data: request, // page_type=4001
          success: function (response) {
            let jsonData = JSON.parse(response);
            if (jsonData['error'] == 0) {
              console.log("error == " + 0);
              outCabinetStages(jsonData).then(function () {
                initCabinetStagesListeners(jsonData);
              });

            } else if (jsonData['error'] > 0) {
              console.log("error > " + 0);
              $('.tab-stages').text(jsonData['error_text']).addClass('msg_warning').removeClass('msg_success');
            }
          },
          error: function () {
            $('#tab-1').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
            $('#tab-2').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
            $('#tab-3').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
            $('#tab-4').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
          }
        });
      });
    }
  }
  resolve();
  });
}
function registrationListener(e) {
  console.log('in registrationListener 1');
  e.preventDefault();
  console.log('in registrationListener 2');
  alert("btnStageAction3102");
  console.log('in registrationListener 3');
}


function getProfile(){
  return new Promise(function (resolve) {
    $.ajax({
      type: "POST",
      url: "ajax/Pechenkin_A1620/adaptedToAjax/engine_mobile.php",
      data: "page=cabinet/profile", // page_type=4001
      success: function (response) {
        let jsonData = JSON.parse(response);

        if (jsonData['error'] == 0) {
          console.log("error == " + 0);
          outCabinetProfile(jsonData).then(function () {
            //initCabinetProfileListeners(jsonData);
            console.log("заглушка");
          });

        } else if (jsonData['error'] > 0) {
          console.log("error > " + 0);
          $('.tab-stages').text(jsonData['error_text']).addClass('msg_warning').removeClass('msg_success');
        }
      },
      error: function () {
        console.log("page_type_4001 error");
        $('#tab-1').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-2').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-3').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-4').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
      }
    });
    resolve();
  });
}

function outCabinetProfile(jsonData) {
  return new Promise(function (resolve) {
    $('#tab-1').html(jsonData['uch_data']);
    $('#tab-2').html(jsonData['pers_data']);
    $('#tab-3').html(jsonData['addr_data']);
    $('#tab-4').html(jsonData['sch_data']);
    resolve();
  })
}

/* ********************* getDistance ********************************* */
function getDistance() { // Получить данные о стадиях олимпиады.
  return new Promise(function (resolve) {
    $.ajax({
      type: "POST",
      url: getPage_path + "adaptedToAjax/engine_mobile.php",
      data: "page=cabinet/distance", // page_type=4001
      success: function (response) {
        let jsonData = JSON.parse(response);

        if (jsonData['error'] == 0) {
          console.log("error == " + 0);
          outCabinetDistance(jsonData);
          /*
          outCabinetDistance(jsonData).then(function () {
            initCabinetDistanceListeners(jsonData);
          }).then(function(){ // Инициализация раскрытия информации о стадиях.
            $('#completed_stages').click(function(){
              $('#tab-1').slideToggle(500);
              $('#tab-1-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });
            $('#qualif_stages').click(function(){
              $('#tab-2').slideToggle(500);
              $('#tab-2-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });
            $('#final_stages').click(function(){
              $('#tab-3').slideToggle(500);
              $('#tab-3-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });
            $('#training_stage').click(function(){
              $('#tab-4').slideToggle(500);
              $('#tab-4-arrow').toggleClass('arrow-down').toggleClass('arrow-up');
            });

          });*/

        } else if (jsonData['error'] > 0) {
          console.log("error > " + 0);
          $('.tab-stages').text(jsonData['error_text']).addClass('msg_warning').removeClass('msg_success');
        }
  
      },
      error: function () {
        console.log("page_type_4003 error");
/*
        $('#tab-1').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-2').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-3').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
        $('#tab-4').html("<span class=\"msg_warning\">Ошибка получения данных</span>");
*/
      }
    });
    resolve();
  });
}

function outCabinetDistance(jsonData) {
  return new Promise(function (resolve) {
    console.log("outCabinetDistance");
    console.log("outCabinetDistance get_var_ref !== \"\"" + jsonData['get_var_ref'] !== "");
    if (jsonData['get_var_ref'] !== ""){
      $('#var-ref').html(jsonData['get_var_ref']);
      // $('#tab-1').slideUp(0);
      // $('#tab-1-arrow').addClass('arrow-down');
    } else {
      if(jsonData['debug'] === 1){
        $('#var-ref').html('<div class="msg_warning">Информация отсутствует.</div>');
        // $('#tab-1-arrow').addClass('arrow-up');
      }
    }
    resolve();
  })
}