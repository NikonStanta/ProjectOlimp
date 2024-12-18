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
        case 'doConfirmPerson':
        {
            break;
        }
    }

// Формирование кода страницы

    switch ($cmd) {
        default:
        {
            $body = str_replace('%admin_id%', $uid, $tpl['confirm_person']['mainform']);
        }
    }


    return str_replace('%error%', $error, $body);
}

$tpl['confirm_person']['mainform'] =
'
<table id="personsList">
<tr id="personsListHeader"><td>ID</td><td>ФИО</td><td width="100">Участие</td><td>Личность</td><td>Школа</td><td>Заявление</td><td>Справка из школы</td><td>Идент. документ</td><td>Фотография</td><td>Действия</td></tr>
</table>

<div id="personDialog" title="Подтверждение данных персоны">
<table id="PersonDocsTable">
</table>
</div>

<style>
.justApplyed td {    background:#CCFFCC; }
.Accepted td, td.Accepted, td.A {    background:#CCFFCC; }
.Rejected td, td.Rejected, td.R {    background:#FFCCCC; }
td.Loaded, td.Updated {     }
td.Not {    color:#CCCCCC; }
</style>

<script>
setTimeout(function () {
var lockTag={"IdPerson":"0","lock_edit_id":"%admin_id%","lock_edit_dt":""};
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
    
    var list = "";
    for (let person of persons) 
    {    rowClass="";
        
        if (person.SchoolDoc=="Accepted" && person.IdentDoc=="Accepted" && person.IdentPhoto=="Accepted")    rowClass=" Accepted";
        
        list = list + "<tr class=\'personsListRow"+rowClass+"\' id=\'Person"+person.pid+"\'><td>"+person.pid+"</td><td>"+person.fio+"</td><td>"+person.presence+"</td><td class=\'confirmed_person\'>"+person.confirmed_person+"</td><td class=\'confirmed_school\'>"+person.confirmed_school+"</td><td class=\'printed_z "+person.printed_z+"\'>"+person.printed_z+"</td><td class=\'SchoolDoc "+person.SchoolDoc+"\'>"+person.SchoolDoc+"</td><td class=\'IdentDoc\'>"+person.IdentDoc+"</td><td class=\'IdentPhoto\'>"+person.IdentPhoto+"</td><td>";
    if (person.lock_edit_id == 0)
    {    list = list + "<button class=\'btnPersonDialog\' IdPerson="+person.pid+">Проверить документы</button>";    }
    else
    {    list = list + "Заблокировано другим оператором";    }    
    list = list + "</td></tr>";
        
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
            res = $.getJSON("/ajax/olimp_confirm_person_lock.php", lockTag, lock_func);
            ev.preventDefault();
        });
}

function lock_func(data)
{    console.log("res");
    if (data.status == "ok")
    {    $( "#personDialog" ).dialog("open");    }
    else
    {    alert("Невозможно получить блокировку"+data.status); 
    }
    console.log(data);
}

function unlock_func(data)
{    if (data.status == "ok")
    {    lockTag.IdPerson=0;
    }
    else
    {    alert("Невозможно снять блокировку"+data.status); 
    }
    console.log(data);
}

function apply_func(data)
{       
    console.log("res");
    console.log(data);
    if (data.status == "ok")
    {    alert
        $("#Person"+lockTag.IdPerson + " td.confirmed_person").text(lockTag.confirmed_person);
        $("#Person"+lockTag.IdPerson + " td.confirmed_school").text(lockTag.confirmed_school);
        $("#Person"+lockTag.IdPerson + " td.printed_z").text(lockTag.printed_z);
        $("#Person"+lockTag.IdPerson + " td.SchoolDoc").text(lockTag.SchoolDoc);
        $("#Person"+lockTag.IdPerson + " td.IdentDoc").text(lockTag.IdentDoc);
        $("#Person"+lockTag.IdPerson + " td.IdentPhoto").text(lockTag.IdentPhoto);
        $("#Person"+lockTag.IdPerson).addClass("justApplyed");
    }
    else
    {    alert("Невозможно снять блокировку"+data.status); 
    }
}

function build_list()
{    $.getJSON("/ajax/olimp_confirm_person_list.php", build_list_func);
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
            text: "Внести изменения",
            click : function() {    
                // #################################################################################################################
                //lockTag.confirmed_person="N";
                //lockTag.confirmed_school="N";

                var application=$("input[name=\'application\']:checked").val();
                if ( application != "Skip") lockTag.printed_z=application;

                var schooldoc=$("input[name=\'schooldoc\']:checked").val();
                if ( schooldoc != "Skip") lockTag.SchoolDoc=schooldoc;
                
                var identdoc=$("input[name=\'identdoc\']:checked").val();
                if ( identdoc != "Skip") lockTag.IdentDoc=identdoc;
                
                var identphoto=$("input[name=\'identphoto\']:checked").val();
                if ( identphoto != "Skip") lockTag.IdentPhoto=identphoto;
                
                // #################################################################################################################                
                $.getJSON("/ajax/olimp_confirm_person_apply.php", lockTag, apply_func);
                needUnlock=false;
                $( this ).dialog( "close" );
                }
            }, {
            text: "Отменить",
            click: function() {
                $( this ).dialog( "close" );
             }}],
        close: function( event, ui ) 
                {    if (needUnlock) 
                    {    $.getJSON("/ajax/olimp_confirm_person_unlock.php", lockTag, unlock_func);
                    }
                    preventPageReload=false;  // ???
                    RefreshTimeout=setTimeout(refresh_list,RefreshPeriod);    
                },
        open: function( event, ui ) 
                {    needUnlock=true;
                    clearTimeout(RefreshTimeout);
                    $("#PersonDocsTable").load("/ajax/olimp_confirm_person_doc_list.php",lockTag);
                }
});
console.log("1");


$(function() {

refresh_list();

});
}, 1000);
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
