$(function () {
  // console.log("cabinet_documents.js connected");
  $('#UploadSchoolDoc').click(function (e) { // Отправка справки школьника.
    e.preventDefault();
    const formData = new FormData();
    let file = $('#schooldocfile_form > #schooldocfile_load')[0].files[0];
    // let name = $(this).attr(name);
    console.log("name: ", file);
    formData.append('schooldocfile', file);
    console.log(formData);
    fetch("loadfiles.php?UploadSchoolDoc=1", {
      method: 'POST',
      body: formData,
    }).then((response) => {
      if (response.ok) {
        response.json().then(function (json) {
          if (json.error > 0) {
            $('#schoolDocMsg').removeClass('msg_success').removeClass('msg_simple').addClass('msg_warning').html(json.error_text);
            console.log("schoolDocMsg error");
          } else {
            $('#schoolDocMsg').removeClass('msg_success').removeClass('msg_warning').addClass('msg_simple').html("Файл загуржен на сервер и ожидает проверки.");
            console.log("schoolDocMsg error");
          }
          console.log("UploadSchoolDoc changed");
          console.log("response.error: " + json.error);
        })
      } else {
        console.log("Ошибка отправки данных schoolDocMsg");
        $('#schoolDocMsg').removeClass('msg_success').addClass('msg_warning').html("Ошибка отправки файла.");
      }
    }).catch(function (e) {
      console.log("Ошибка отправки данных schoolDocMsg");
      $('#schoolDocMsg').removeClass('msg_success').addClass('msg_warning').html("Ошибка отправки файла.");
    });
  });

  $('#UploadIdentDoc').click(function (e) { // Отправка документа, идентифицирующего участника.
    e.preventDefault();
    const formData = new FormData();
    let file = $('#identdocfile_load')[0].files[0];
    // let name = $(this).attr(name);
    console.log("name: ", file);
    formData.append('identdocfile', file);
    console.log(formData);
    fetch("loadfiles.php?UploadIdentDoc=1", {
      method: 'POST',
      body: formData,
    }).then((response) => {
      if (response.ok) {
        response.json().then(function (json) {
          if (json.error > 0) {
            $('#identDocMsg').removeClass('msg_success').removeClass('msg_simple').addClass('msg_warning').html(json.error_text);
            console.log("identDocMsg error");
          } else {
            $('#identDocMsg').removeClass('msg_success').removeClass('msg_warning').addClass('msg_simple').html("Файл загуржен на сервер и ожидает проверки.");
            console.log("identDocMsg error");
          }
          console.log("UploadIdentDoc changed");
          console.log("response.error: " + json.error);
        })
      } else {
        console.log("Ошибка отправки данных identDocMsg");
        $('#identDocMsg').removeClass('msg_success').addClass('msg_warning').html("Ошибка отправки файла.");
      }
    }).catch(function (e) {
      console.log("Ошибка отправки данных identDocMsg");
      $('#identDocMsg').removeClass('msg_success').addClass('msg_warning').html("Ошибка отправки файла.");
    });
  });

  $('#UploadIdentPhoto').click(function (e) { // Фотография участника олимпиады.
    e.preventDefault();
    const formData = new FormData();
    let file = $('#identphotofile_load')[0].files[0];
    // let name = $(this).attr(name);
    console.log("name: ", file);
    formData.append('identphotofile', file);
    console.log(formData);
    fetch("loadfiles.php?UploadIdentPhoto=1", {
      method: 'POST',
      body: formData,
    }).then((response) => {
      if (response.ok) {
        response.json().then(function (json) {
          if (json.error > 0) {
            $('#identPhotoMsg').removeClass('msg_success').removeClass('msg_simple').addClass('msg_warning').html(json.error_text);
            console.log("identPhotoMsg error");
          } else {
            $('#identPhotoMsg').removeClass('msg_success').removeClass('msg_warning').addClass('msg_simple').html("Файл загуржен на сервер и ожидает проверки.");
            console.log("identPhotoMsg error");
          }
          console.log("UploadIdentPhoto changed");
          console.log("response.error: " + json.error);
        })
      } else {
        console.log("Ошибка отправки данных identPhotoMsg");
        $('#identPhotoMsg').removeClass('msg_success').addClass('msg_warning').html("Ошибка отправки файла.");
      }
    }).catch(function (e) {
      console.log("Ошибка отправки данных identPhotoMsg");
      $('#identPhotoMsg').removeClass('msg_success').addClass('msg_warning').html("Ошибка отправки файла.");
    });
  });
});