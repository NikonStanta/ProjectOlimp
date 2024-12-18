<?php

// Подтверждение данных пользователя
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$error,$cmd;

    if (($role & 8) == 0) {
        return $tpl['noaccess'];
    }


// Выполнение команд

    switch ($cmd) {
        case 'doConfirmReculc':
        {
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        default:
        {
            $body = str_replace('%admin_id%', $uid, $tpl['confirm_reculc']['mainform']);
        }
    }


    return str_replace('%error%', $error, $body);
}

$tpl['confirm_reculc']['mainform'] =
'
<table id="personsList">
<tr id="personsListHeader"><td>ID</td><td>ФИО</td><td>Экзамен</td><td>Результат</td><td>Партнёрская олимпиада</td><td>Статус</td><td>Заявление</td><td>Сообщение пользователю</td><td></td><td></td></tr>
</table>

<div id="personDialog" title="Подтверждение перезачёта">
<style>
input::-webkit-outer-spin-button,
input::-webkit-inner-spin-button {
    -webkit-appearance: none;
    margin: 0;
}
</style>
<table id="PersonReculcsTable" style=\'width: 100%;\'>
</table>
</div>

<style>
.justAccepted td {    background:#CCFFCC; }
.justRejected td {    background:#FFCCCC; }
.Accepted td, td.Accepted, td.A {    background:#CCFFCC; }
.Rejected td, td.Rejected, td.R {    background:#FFCCCC; }
td.Loaded, td.Updated {     }
td.Not {    color:#CCCCCC; }
</style>

<script>
var lockTag={"IdPerson":"0","lock_edit_id":"%admin_id%","lock_edit_dt":"", "IdReculc":"0", "score":"0"};
var RefreshTimeout;
var RefreshPeriod = 60000;
var preventPageReload=false;
var persons;
var needUnlock=false;

function refresh_list()
{    $(".personsListRow").remove();
    build_list();
    RefreshTimeout=setTimeout(refresh_list,RefreshPeriod);
}
build_list_func = function(data) 
{    var persons = [];
    console.log(data);
    var rowClass="";
    persons = data.persons;
    console.log(persons);
    
    var list = "";
    for (let person of persons) 
    {    rowClass="";
        
    if (person.status=="Заявка принята")    rowClass=" Accepted";
    else if (person.status=="Заявка отклонена")    rowClass=" Rejected";
        
    list = list + "<tr class=\'personsListRow"+rowClass+"\' id=\'reculc"+person.rec_id+"\'><td>"+person.id+"</td><td>"+person.fio+"</td><td>"+person.exam+"</td><td class=\'score\'>"+person.score+"</td><td>"+person.partner+"</td><td class=\'status\'>"+person.status+"</td>";
    list = list + "<td>" + "<a href=\'/ajax/olimp_confirm_person_view_doc.php?IdPerson="+person.id+"&docType=reculc_application&exam_code=" + person.exam_code + "\' target=\'_blank\'>Просмотр заявления</a><br></td><td>"+person.msg+"</td>";
    
    if (person.lock_edit_id == 0)
    {    list = list + "<td><button class=\'btnAccept btnPersonDialog\' IdPerson="+person.id + " IdExam="+person.exam_code + " IdReculc="+person.rec_id + " IdScore="+person.score + " IdStageold="+person.old_stage + ">Одобрить</button></td>" +
    "<td><button class=\'btnReject btnPersonDialog\' IdPerson="+person.id + " IdExam="+person.exam_code + " IdReculc="+person.rec_id + " IdStageold="+person.old_stage + ">Отклонить</button></td></tr>";    }
    else
    {    list = list + "<td>Заблокировано другим оператором</td><td>Заблокировано другим оператором</td></tr>";    }
    }
    
    $("#personsList").append(list);
    
    $(".btnPersonDialog").button();
    
    $(".btnPersonDialog").click(function(ev) {
            if (RefreshTimeout) clearTimeout(RefreshTimeout);
            
            let currentDT = new Date(); 
            lockTag.lock_edit_dt = 
                currentDT.getFullYear() + "-" + 
                (currentDT.getMonth()<9 ? "0"+(currentDT.getMonth()+1) : (currentDT.getMonth()+1)) + "-" +
                (currentDT.getDate()<10 ? "0"+currentDT.getDate() : currentDT.getDate()) + "-" +
                (currentDT.getHours()<10 ? "0"+currentDT.getHours() : currentDT.getHours()) + ":" +
                (currentDT.getMinutes()<10 ? "0"+currentDT.getMinutes() : currentDT.getMinutes()) + ":" +
                (currentDT.getSeconds()<10 ? "0"+currentDT.getSeconds() : currentDT.getSeconds());
            lockTag.IdPerson=$(this).attr("IdPerson");
            lockTag.IdExam=$(this).attr("IdExam");
            lockTag.IdReculc=$(this).attr("IdReculc");
            lockTag.Score=$(this).attr("IdScore");
            lockTag.StageOld=$(this).attr("IdStageold");
            if (Array.from(ev.target.parentElement.classList).includes("btnAccept")) {
                lockTag.Action="accept";
            } else {
                lockTag.Action="reject";
            }
            console.log(lockTag);
            res = $.getJSON("/ajax/olimp_confirm_reculc_lock.php", lockTag, lock_func);
            ev.preventDefault();
        });
}

function lock_func(data)
{    console.log("res");
    if (data.status == "ok")
    {   
        $( "#personDialog" ).dialog("open");
    }
    else
    {    alert("Невозможно получить блокировку"+data.status); 
    }
    console.log(data);
}

function unlock_func(data)
{   
    console.log(data);
    if (data.status == "ok")
    {   lockTag.IdPerson=0;
        lockTag.IdExam=0;
        lockTag.IdReculc=0;
    }
    else
    {    alert("Невозможно снять блокировку"+data.status); 
    }
}

function apply_func(data)
{       
    console.log("res");
    console.log(data);
    color_class = "";
    color_class_old = "";
    if (data.type == "accepted")
    {   color_class = "Accepted";
        color_class_old = "Rejected";
        $("#reculc"+lockTag.IdReculc + " td.status").text("Заявка принята");
        $("#reculc"+lockTag.IdReculc + " td .btnAccept").attr("idstageold", data.stage);
        $("#reculc"+lockTag.IdReculc + " td .btnReject").attr("idstageold", data.stage);
        if (data.score) {
        console.log("res2");
        $("#reculc"+lockTag.IdReculc + " td.score").text(data.score);
        $("#reculc"+lockTag.IdReculc + " td .btnAccept").attr("IdScore", data.score);}
    }
    else if (data.type == "rejected")
    {   color_class = "Rejected";
        color_class_old = "Accepted";
        $("#reculc"+lockTag.IdReculc + " td.status").text("Заявка отклонена");
        console.log("res3");
    }
    if (data.status == "ok")
    {
        $("#reculc"+lockTag.IdReculc).removeClass(color_class_old);
        $("#reculc"+lockTag.IdReculc).addClass(color_class);
    }
    else
    {    alert("Невозможно снять блокировку"+data.status); 
    }
}

function build_list()
{    $.getJSON("/ajax/olimp_confirm_reculc_list.php", build_list_func);
}
console.log("2");

$( "#personDialog" ).dialog({
        modal: true,
        autoOpen: false,
        width: 750,
        height: "auto",
        appendTo: "#personsList",
        position: { my: "center", at: "center", of: window },
        buttons: [{
            text: "Принять",
            click : function() {
                if ( $("textarea#commentInput").val() != "") lockTag.comment = $("textarea#commentInput").val();
                if ( $("#stage").val() != "") lockTag.Stage = $("#stage").val();
                if ( $("#rank").val() != "") lockTag.Rank = $("#rank").val();
                if ( $("#score").val() != "") lockTag.Score = $("#score").val();
                $.getJSON("/ajax/olimp_confirm_reculc_response.php", lockTag, apply_func);
                console.log(lockTag);
                console.log("lockTag");
                needUnlock=false;
                $( this ).dialog( "close" );
                console.log("afterclose");
                }
            }, {
            text: "Отменить",
            click: function() {
                $( this ).dialog( "close" );
             }}],
        close: function( event, ui ) 
                {    if (needUnlock) 
                    {    $.getJSON("/ajax/olimp_confirm_reculc_unlock.php", lockTag, unlock_func);
                    }
                    preventPageReload=false;  // ???
                    RefreshTimeout=setTimeout(refresh_list,RefreshPeriod);    
                },
        open: function( event, ui ) 
                {    needUnlock=true;
                    clearTimeout(RefreshTimeout);
                    $("#PersonReculcsTable").load("/ajax/olimp_confirm_reculc_dialog.php",lockTag);
                    console.log(lockTag);
                }
});

console.log("1");


$(function() {

refresh_list();

});
</script>
';

$tpl['confirm_person']['tmp'] =
'

$( "#ConsentConfirmMagAsp" ).dialog({
        modal: true,
        autoOpen: false,
        width: 750,
        height: "auto",
        appendTo: "#center_column",
        position: { my: "center", at: "center", of: "#budget-consent-bar" },
        buttons: [{
            text: "Подтвердить",
            click : function() {    
                $( this ).dialog( "close" );
                $("#budget-consent-form").append( $("<input type=hidden name=btnApplyConsentMagAsp value=1>")  );
                res=$( "#consent-competitive-groups" ).sortable("toArray");
                res.forEach(function(v,idx) {$("#budget-consent-form").append("<input type=hidden name=\'consentGroups["+(idx+1)+"]\' value=\'"+v.substring(1)+"\' >");  });
                $("#budget-consent-form").submit();
                }
            }, {
            text: "Отменить",
            click: function() {
                $( this ).dialog( "close" );
                preventPageReload=false;  
             }}]
    });


 $("button#btnBudgetConsentAction").button({
        create: function(e) {
            $(e.target).click(function(ev) {
               preventPageReload=true;
               $( "#ConsentConfirmMagAsp" ).dialog("open");
               ev.preventDefault();
            })
        }
    });




';


$body = build_page();
