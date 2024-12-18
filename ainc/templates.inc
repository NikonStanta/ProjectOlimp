<?php

$tpl['page1']['main'] =
'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<html>
<head>
    <title>%page_title%</title>
    <meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
    <title>%page_title%</title>
    <link rel="stylesheet" type="text/css" href="css/ie.css">
    <link rel="stylesheet" type="text/css" href="ie.css">
    <link rel="stylesheet" type="text/css" href="nc.css">
<link rel=stylesheet href="/css/blue/jquery-ui-1.8.5.custom.css" type=text/css>    
<link href="/calendar/css/pickmeup.css" rel="stylesheet" type="text/css" />
     
<script src="/css/jquery-1.4.3.min.js" type="text/javascript"></script>
<script src="/css/jquery-ui-1.8.5.custom.min.js" type="text/javascript"></script>
<script src="/css/jquery.ui.datepicker-ru.js" type="text/javascript"></script>
<script src="/js/script.js" type="text/javascript"></script>
<script src="/calendar/js/pickmeup.js"></script>

</head>
<body bgcolor="#FFFFFF" leftmargin="0" topmargin="0" rightmargin="0" bottommargin="0" marginwidth="0" marginheight="0">
<table width=100% cellspacing=0 cellpadding=0 border=0>
<tr><td width=100% valign=top align=left background=aimg/top2_admin.gif><img src=aimg/top1_admin.gif width=601 height=156 border=0 align=top></td>
<td background=aimg/top3_admin.gif valign=bottom><img src=aimg/top3_admin.gif width=252 height=156 border=0 align=top></td></tr>
<tr><td colspan=2 align=right bgcolor=#cccccc><div style="margin: 0px 8px 8px 0px; color: Black; font-size: 12px; text-align:right;">%user%</span>&nbsp;&nbsp;&nbsp;<a href=%refpar%&cmd=logout>[Выйти]</a></td></tr>
</table>
<br>
<table width=100% cellspacing=0 cellpadding=0 border=0 bgcolor=white>
<tr><td valign=top background="aimg/bkg_menu.gif"><img src=aimg/top4_admin.gif width=200 height=30 border=0 align=top><br><img src=aimg/t.gif width=202 height=1 border=0 align=top></td>
<td valign=top width=100% background=aimg/bkg_ttl.gif><h1 class="ttl" style="margin: 0px 8px 0px 0px;">%title%</h1></td>
</tr>
</table><table width=100% cellspacing=0 cellpadding=0 border=0>
<tr>
    <td background="aimg/bkg_menu.gif" valign=top><table width=100% cellspacing=0 cellpadding=0 border=0 backgroung=aimg/t.gif>
    <tr><td background="aimg/t.gif">%menu2%</td></tr>
    <tr><td>%menu1%</td></tr>
    <tr><td>%pagemenu%</td></tr>
    </table></td>
    <td bgcolor=white valign=top width=100%><table width=100% cellspacing=0 cellpadding=0 border=0>
        <tr><td>&nbsp;</td><td><br>%body%<br><br></td><td>&nbsp;</td></tr>
    </table></td>
</tr>
<tr background=bkg_foot.gif bgcolor=#cccccc><td>&nbsp;</td><td>&nbsp;</td></tr>
</body>
</html>';

$tpl['page2']['main'] =
'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
    <title>%page_title%</title>
    <link rel="stylesheet" type="text/css" href="css/ie.css">
    <link rel="stylesheet" type="text/css" href="ie.css">
    <link rel="stylesheet" type="text/css" href="nc.css">
<link rel=stylesheet href="/css/blue/jquery-ui-1.8.5.custom.css" type=text/css>    
     
<script src="/css/jquery-1.4.3.min.js" type="text/javascript"></script>
<script src="/css/jquery-ui-1.8.5.custom.min.js" type="text/javascript"></script>
<script src="/css/jquery.ui.datepicker-ru.js" type="text/javascript"></script>

</head>
<body bgcolor="#FFFFFF" leftmargin="0" topmargin="0" rightmargin="0" bottommargin="0" marginwidth="0" marginheight="0">
<div align=center>
<tableheight=100% cellspacing=0 cellpadding=0 border=0>
<tr>
<td valign=center>%body%</td>
</tr>
</table>
</div>
</body>
</html>';

$tpl['logon'] =
'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//RU">
<html>
<head>
    <title>%page_title%</title>
    <meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
    <title>%page_title%</title>
    <link rel="stylesheet" type="text/css" href="css/ie.css">
    <link rel="stylesheet" type="text/css" href="ie.css">
    <link rel="stylesheet" type="text/css" href="nc.css">
</head>
<body bgcolor="#FFFFFF" leftmargin="0" topmargin="0" rightmargin="0" bottommargin="0" marginwidth="0" marginheight="0"><br><br><table width=600 height=400 cellpadding=0 cellspacing=0 border=0 background="aimg/logon.jpg" align=center>
<tr><td background=aimg/t.gif align=right valign=bottom style="text-align:right; vertical-align:bottom;">
<form method=post>%formrefpar%<input type=hidden name="cmd" value="logon">
Имя: <input type=text name="nic" style="width:140px;">&nbsp;&nbsp;&nbsp;<br><br>
Пароль: <input type=password name="pwd" style="width:140px;">&nbsp;&nbsp;&nbsp;<br><br>
<input type=submit value=" Войти " name=btnLogon>&nbsp;&nbsp;&nbsp;<br><br>
</form>
</td></tr>
</table>
</body>
</html>
';

$tpl['menu1']['main'] =
'<table cellspacing=0 cellpadding=0 border=0 background=aimg/t.gif>
<tr><td colspan=4 valign=top><img src=aimg/shade_admin.gif width=200 height=25 border=0></td></tr>
<tr><td colspan=4 valign=top><img src=aimg/t.gif width=202 height=4 border=0></td></tr>
%menu%
<tr><td colspan=4 valign=top><img src=aimg/t.gif width=202 height=16 border=0></td></tr>
</table>';
$tpl['menu1']['ttl'] =
'<tr><td valign=top><img src=aimg/t.gif width=10 height=11 border=0></td><td valign=top><img src=aimg/arr_menu.gif width=16 height=12 border=0></td><td valign=top colspan=2>%ttl%</td></tr>';
$tpl['menu1']['item'] =
'<tr><td valign=top><img src=aimg/t.gif width=10 height=11 border=0></td><td>&nbsp;</td><td valign=top><img src=aimg/dot_menu.gif width=9 height=12 border=0></td><td valign=top><a href=%ref%%refpar% class=menu1>%ttl%</a></td></tr>';
$tpl['menu1']['citem'] =
'<tr><td valign=top><img src=aimg/t.gif width=10 height=11 border=0></td><td>&nbsp;</td><td valign=top><img src=aimg/dot_menu.gif width=9 height=12 border=0></td><td valign=top><span class=menu1c>%ttl%</a></td></tr>';

$tpl['menu2']['main'] =
'<table cellspacing=0 cellpadding=0 border=0 background=aimg/t.gif><tr><td colspan=4 valign=top><img src=aimg/t.gif width=182 height=16 border=0></td></tr>%menu%<tr><td colspan=4 valign=top><img src=aimg/t.gif width=182 height=16 border=0></td></tr></table>';
$tpl['menu2']['item'] =
'<tr><td valign=top><img src=aimg/t.gif width=10 height=11 border=0></td><td valign=top><img src=aimg/arr_menu.gif width=16 height=12 border=0></td><td valign=top><a href=%ref%%refpar% class=menu1>%ttl%</a></td></tr>
<tr><td colspan=4 valign=top><img src=aimg/menu1_skip.gif width=182 height=7 border=0></td></tr>';
$tpl['menu2']['citem'] =
'<tr><td valign=top><img src=aimg/t.gif width=10 height=11 border=0></td><td valign=top><img src=aimg/t.gif width=16 height=12 border=0></td><td valign=top><span class=menu1>%ttl%</span></td></tr>
<tr><td colspan=4 valign=top><img src=aimg/menu1_skip.gif width=182 height=7 border=0></td></tr>';

// Вспомогательные шаблоны

$tpl['even'] = ' bgcolor=#e0e0f0';
$tpl['odd'] = ' bgcolor=#f0f0ff';
$tpl['noaccess'] =
'<h2>У Вас нет прав для доступа к этому разделу.</h2>';

// ======================== Содержание сайта

$tpl['pages']['main'] =
'<h2>Раздел: %div%</h2><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Текстовые страницы</span></td><td><span style="color:white"><nobr>Подтип страницы (макет)</nobr></span></td></tr>%list%</table><br><br>';

$tpl['pages']['pageline'] =
'<tr%bgcolor%><td width=100%><a href=pages.html%refpar%&pid=%id%>%ttl%</a></td><td><nobr>%maket%</nobr></td></tr>';

$tpl['pages']['nopage'] =
'<h2>Редактирование невозможно</h2>Запрошена несуществующая страница или тип страницы отличен от "Текстовая".';

$tpl['pages']['noaccess'] =
'<h2>У Вас нет прав для редактирования страниц.</h2>';

$tpl['pages']['edit'] =
'<form action="pages.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=pid value=%pid%><input type=hidden name=cmd value=doEdit>
%text%
%images%
%tables%
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['pages']['textfield'] =
'<h3>%title%</h3>
%text%
';

$tpl['pages']['imagefield'] =
'<h3>%title%</h3>
<table width=750 cellspacing=8 cellpadding=0 border=0>
<tr><td valign=top rowspan=2><img src=%image% width=100 height=70 border=0></td><td>Подпись: <input type=text name="images[%name%][alt]" value="%alt%" size=64></td></tr>
<tr><td>Ширина: <input type=text name="images[%name%][w]" value="%w%" size=16> Высота: <input type=text name="images[%name%][h]" value="%h%" size=16></td></tr>
<tr><td><input type=file size=4 name="imagefile_%name%"></td><td>Атрибуты: <input type=text name="images[%name%][attr]" value="%attr%" size=64></td></tr>
</table>
';

$tpl['pages']['tablefield'] =
'<h3>%title%</h3>
Тип таблицы: <select name="tables[%name%][tp]">%tp%</select><br><br>
Cтрок: <input type=text name="tables[%name%][rows]" value="%rows%" size=8>  Cтолбцов: <input type=text name="tables[%name%][cols]" value="%cols%" size=8>&nbsp;&nbsp;&nbsp;
<input type=submit name="btnResize" value="  Изменить размер таблицы  ">
<br><br>
<table width=750 cellspacing=0 cellpadding=0 border=0>%cells%</table>
';

$tpl['pages']['row'] =
'<tr>%row%</tr>
';

$tpl['pages']['cell'] =
'<td><input type=text name="tables[%name%][cells][%rn%][%cn%]" value="%text%" style="width=100%;"></td>
';

$tpl['pages']['clone_text'] =
'<span style="color:blue; text-weight:bold;">клон</span>&nbsp;&nbsp;&nbsp;';

// Редактирование каталога

$tpl['catalog']['main'] =
'<h2>Каталог: %div%</h2><div align=right style="text-align:right;"><a href=catalog.html%refpar%&cmd=AddGroup&pgid=%pgid%><li> Добавить группу</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0>%list%</table><br><br>';

$tpl['catalog']['itemupline'] =
'<tr%bgcolor%><td><a href=catalog.html%refpar%&cmd=EditItem&iid=%iid%>%name%</a></td><td align=right style="text-align:right;"><a href=catalog.html%refpar%&cmd=EditDetail&iid=%iid%>[Подробное описание]</a>&nbsp;<a href=catalog.html%refpar%&cmd=DelItem&iid=%iid%>[Удалить элемент]</a>&nbsp;&nbsp;<a href=catalog.html%refpar%&iid=%iid%&upid=%upid%&cmd=UpItem><img src=/aimg/up_arrow.gif border=0></a></td></tr>';

$tpl['catalog']['itemline'] =
'<tr%bgcolor%><td><a href=catalog.html%refpar%&cmd=EditItem&iid=%iid%>%name%</a></td><td align=right style="text-align:right;"><a href=catalog.html%refpar%&cmd=EditDetail&iid=%iid%>[Подробное описание]</a>&nbsp;<a href=catalog.html%refpar%&cmd=DelItem&iid=%iid%>[Удалить элемент]</a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16 height=16></td></tr>';

$tpl['catalog']['groupupline'] =
'<tr bgcolor=#000099><td><a href=catalog.html%refpar%&cmd=EditGroup&gid=%gid% style="color:white; font-weight:bold;">%name%</a><br>
<a href=catalog.html%refpar%&cmd=CopyGroup&gid=%gid% style="color:white">[Скопировать группу]</a>&nbsp;&nbsp;&nbsp;
<a href=catalog.html%refpar%&cmd=GroupParams&gid=%gid% style="color:white">[Параметры группы]</a>
</td><td align=right style="text-align:right;" valign=bottom><a href=catalog.html%refpar%&cmd=SortItems&gid=%gid% style="color:white">[Сортировать по алфавиту]</a>&nbsp;<a href=catalog.html%refpar%&cmd=AddItem&gid=%gid% style="color:white">[Добавить элемент в группу]</a>&nbsp;<a href=catalog.html%refpar%&cmd=DelGroup&gid=%gid% style="color:white">[Удалить группу]</a>&nbsp;&nbsp;<a href=catalog.html%refpar%&gid=%gid%&upid=%upid%&cmd=UpGroup><img src=/aimg/up_arrow.gif border=0></a></td></tr>';

$tpl['catalog']['groupline'] =
'<tr bgcolor=#000099><td><a href=catalog.html%refpar%&cmd=EditGroup&gid=%gid% style="color:white; font-weight:bold;">%name%</a><br>
<a href=catalog.html%refpar%&cmd=CopyGroup&gid=%gid% style="color:white">[Скопировать группу]</a>&nbsp;&nbsp;&nbsp;
<a href=catalog.html%refpar%&cmd=GroupParams&gid=%gid% style="color:white">[Параметры группы]</a>
</td><td align=right style="text-align:right;" valign=bottom><a href=catalog.html%refpar%&cmd=SortItems&gid=%gid% style="color:white">[Сортировать по алфавиту]</a>&nbsp;<a href=catalog.html%refpar%&cmd=AddItem&gid=%gid% style="color:white">[Добавить элемент в группу]</a>&nbsp;<a href=catalog.html%refpar%&cmd=DelGroup&gid=%gid% style="color:white">[Удалить группу]</a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16 height=16></td></tr>';

$tpl['catalog']['detail'] =
'<form action="catalog.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=iid value=%iid%><input type=hidden name=cmd value=doDetail>
%text%
%images%
%tables%
<br><br><hr noshade>
<input type=submit name="btnDetail" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


$tpl['catalog']['editItem'] =
'<form action="catalog.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=iid value=%iid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>ФИО сотрудника:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Должность:</td><td><input type=text name="subttl1" value="%subttl1%" size=64 style="width:100%"></td></tr>
<tr><td>Группа:</td><td><select size=0 name=gid>%gids%</select></td></tr>

<tr><td colspan=2>Краткое описание:<br>
%descr%
</td></tr>
<tr><td><img src=%bimage% width=50 height=50 border=0></td><td>Картинка: <input type=file size=32 name="bimagefile">&nbsp;&nbsp;&nbsp;%bdelbtn%</td></tr>
<tr><td><img src=%image% width=50 height=50 border=0></td><td>Средняя: <input type=file size=32 name="imagefile">&nbsp;&nbsp;&nbsp;%delbtn%</td></tr>
<tr><td><img src=%simage% width=50 height=50 border=0></td><td>"Иконка": <input type=file size=32 name="simagefile">&nbsp;&nbsp;&nbsp;%sdelbtn%</td></tr>

<tr><td><nobr>Тип страницы описания:</nobr></td><td width=100%><select size=0 name=subtype>%subtypes%</select></td></tr>
<tr><td>Позиция в списке:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddItem" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalog']['extra_image_line'] = '<tr><td><img src=%simage% width=50 height=50 border=0></td><td>Иконка: <input type=file size=32 name="simagefile%num%">&nbsp;&nbsp;&nbsp;%sdelbtn%</td>
<td><img src=%bimage% width=50 height=50 border=0></td><td> Картинка: <input type=file size=32 name="bimagefile%num%">&nbsp;&nbsp;&nbsp;%bdelbtn%</td></tr>';

$tpl['catalog']['delImageBtn'] = '<input type=submit name="btnDelImage[%id%]" value=" Удалить ">';


$tpl['catalog']['delItem'] =
'<form action="catalog.html" method=post>%formrefpar%<input type=hidden name=iid value=%iid%>
<input type=hidden name=cmd value=doDelItem>
<h2>Подтвердите удаление элемента каталога</h2>
Все данные, связанные с удаляемым элементом будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Наименование элемента:</td><td>%name%</td></tr>
<tr><td>Описание:</td><td>%descr%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelItem" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalog']['delGroup'] =
'<form action="catalog.html" method=post>%formrefpar%<input type=hidden name=gid value=%gid%>
<input type=hidden name=cmd value=doDelGroup>
<h2>Подтвердите удаление группы</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Наименование группы:</td><td>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelGroup" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalog']['nodelGroup'] =
'<form action="catalog.html" method=post>%formrefpar%<input type=hidden name=gid value=%gid%>
<input type=hidden name=cmd value="">
<h2>Удаление группы невозможно</h2>
Группа содержит элементы, для удаления необходимо удалить все элементы группы либо перенести их в другую группу.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Наименование группы:</td><td>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnCancel" value="  Вернуться в каталог  ">
</form>
';

$tpl['catalog']['editGroup'] =
'<form action="catalog.html" method=post enctype="multipart/form-data"">%formrefpar%<input type=hidden name=gid value=%gid%>
<input type=hidden name=pass value=1>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название группы:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Раздел:</td><td><select size=0 name=pgid>%pgids%</select></td></tr>
<tr><td>Позиция в разделе:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
<tr><td>Префикс картинки:</td><td><input type=text name="img_prefix" value="%img_prefix%" size=16></td></tr>
<tr><td>Каталог картинок:</td><td><input type=text name="img_dir" value="%img_dir%" size=64 style="width:100%"></td></tr>
<tr><td>Количество картинок:</td><td><input type=text name="img_count" value="%img_count%" size=64></td></tr>
<tr><td>Префикс шаблона каталога:</td><td><input type=text name="tpl_prefix" value="%tpl_prefix%" size=64 style="width:100%"></td></tr>
<tr><td>Префикс шаблона администратора:</td><td><input type=text name="tpl_adm_prefix" value="%tpl_adm_prefix%" size=64 style="width:100%"></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddGroup" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalog']['editGroupParams'] =
'<form action="catalog.html" method=post enctype="multipart/form-data"">%formrefpar%<input type=hidden name=gid value=%gid%>
<h1>Параметры группы %name%</h1>
<table width=100% cellspacing=8 cellpadding=0 border=0>
%params%
</table>
<br><br><hr noshade>
<input type=submit name="btnGroupParams" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// -----------------------------  Для каталога недвижимости

$tpl['city_catalog']['editItem'] =
'<form action="catalog.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=iid value=%iid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название объекта:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Группа:</td><td><select size=0 name=gid>%gids%</select></td></tr>
<tr><td colspan=2>Краткое описание объекта:<br>
%descr%
</td></tr>
<tr><td><img src=%bimage% width=50 height=50 border=0></td><td>Картинка: <input type=file size=32 name="bimagefile">&nbsp;&nbsp;&nbsp;%bdelbtn%</td></tr>
<tr><td><img src=%image% width=50 height=50 border=0></td><td>Средняя: <input type=file size=32 name="imagefile">&nbsp;&nbsp;&nbsp;%delbtn%</td></tr>
<tr><td><img src=%simage% width=50 height=50 border=0></td><td>"Иконка": <input type=file size=32 name="simagefile">&nbsp;&nbsp;&nbsp;%sdelbtn%</td></tr>

<tr><td><nobr>Тип страницы описания:</nobr></td><td width=100%><select size=0 name=subtype>%subtypes%</select></td></tr>
<tr><td>Позиция в списке:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
<tr><td>Рейтинг:</td><td><input type=text name="rate" value="%rate%" size=8> (1-10)</td></tr>
</table>
<br><br><hr noshade>
<h2>Дополнительные картинки</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
%extra_images%
</table>
<h2>Дополнительные параметры</h2>
<style>
.extraparam input.txt {
    width:100%;
}
</style>
<table width=100% cellspacing=8 cellpadding=0 border=0 class=extraparam>
<tr><td>&nbsp;</td><td width=100%>&nbsp;</td></tr>
%extra_params%
</table>
<br><br><hr noshade><br><br>
<input type=submit name="btnAddItem" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalog']['extraParam1'] =
'<tr><td style="white-space : nowrap;">%title%:</td><td><input type=text name="param1[%pid%]" value="%value%" class=txt></td></tr>';
$tpl['catalog']['extraParam2'] =
'<tr><td style="white-space : nowrap;">%title%:</td><td><select name="param2[%pid%]" size=-1>%options%</select></td></tr>';
$tpl['catalog']['extraParam3'] =
'<tr><td style="white-space : nowrap;"><label for="param_%name%">%title%:</label></td><td><input type=checkbox name="param3[%pid%]" %value% id="param_%name%" value=1></td></tr>';
$tpl['catalog']['extraParam4'] =
'<tr><td style="white-space : nowrap;">%title%:</td><td><textarea name="param1[%pid%]" rows=%rows% cols=40 class=txt style="width:100%">%value%</textarea></td></tr>';


// -----------------------------  Для каталога зарубежной недвижимости

$tpl['fore_catalog']['editItem'] =
'<form action="catalog.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=iid value=%iid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название объекта:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Группа:</td><td><select size=0 name=gid>%gids%</select></td></tr>
<tr><td colspan=2>Краткое описание объекта:<br>
%descr%
</td></tr>
<tr><td><img src=%bimage% width=50 height=50 border=0></td><td>Картинка: <input type=file size=32 name="bimagefile">&nbsp;&nbsp;&nbsp;%bdelbtn%</td></tr>
<tr><td><img src=%image% width=50 height=50 border=0></td><td>Средняя: <input type=file size=32 name="imagefile">&nbsp;&nbsp;&nbsp;%delbtn%</td></tr>
<tr><td><img src=%simage% width=50 height=50 border=0></td><td>"Иконка": <input type=file size=32 name="simagefile">&nbsp;&nbsp;&nbsp;%sdelbtn%</td></tr>

<tr><td><nobr>Тип страницы описания:</nobr></td><td width=100%><select size=0 name=subtype>%subtypes%</select></td></tr>
<tr><td>Позиция в списке:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
</table>
<br><br><hr noshade>
<h2>Дополнительные картинки</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
%extra_images%
</table>
<h2>Дополнительные параметры</h2>
<style>
.extraparam input.txt {
    width:100%;
}
</style>
<table width=100% cellspacing=8 cellpadding=0 border=0 class=extraparam>
<tr><td>&nbsp;</td><td width=100%>&nbsp;</td></tr>
%extra_params%
</table>
<br><br><hr noshade><br><br>
<input type=submit name="btnAddItem" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


// Редактирование выборки из каталога

$tpl['catalogsub']['editItem'] =
'<h1>Свойства выборки</h1><form action="catalogsub.html">%formrefpar%<input type=hidden name=subid value=%subid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название выбрки:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Группа:</td><td><select size=0 name=gid>%gids%</select></td></tr>
<tr><td>Действует с:</td><td><input type=text name="bdt" value="%bdt%" size=64 style="width:100%"></td></tr>
<tr><td>Действует по:</td><td><input type=text name="edt" value="%edt%" size=64 style="width:100%"></td></tr>
<tr><td>Привязка к дню:</td><td>%days%</td></tr>
<tr><td>Позиция в списке:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddItem" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalogsub']['delItem'] =
'<form action="catalogsub.html" method=post>%formrefpar%<input type=hidden name=subid value=%subid%>
<input type=hidden name=cmd value=doDelItem>
<h2>Подтвердите удаление ds,jhrb</h2>
Все данные, связанные с удаляемой выборкой будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Наименование выборки:</td><td>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelItem" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalogsub']['delGroup'] =
'<form action="catalogsub.html" method=post>%formrefpar%<input type=hidden name=gid value=%gid%>
<input type=hidden name=cmd value=doDelGroup>
<h2>Подтвердите удаление группы</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Наименование группы:</td><td>%name%</td></tr>
<tr><td>Количество выборок:</td><td>%count%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelGroup" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalogsub']['nodelGroup'] =
'<form action="catalogsub.html" method=post>%formrefpar%<input type=hidden name=gid value=%gid%>
<input type=hidden name=cmd value="">
<h2>Удаление группы невозможно</h2>
Группа содержит выборки, для удаления необходимо удалить все выборки группы либо перенести их в другую группу.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Наименование группы:</td><td>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnCancel" value="  Вернуться  ">
</form>
';

$tpl['catalogsub']['editGroup'] =
'<h1>Свойства группы выборок</h1>
<form action="catalogsub.html" method=post>%formrefpar%<input type=hidden name=gid value=%gid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название группы:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Каталог:</td><td><select size=0 name=pgid>%pgids%</select></td></tr>
<tr><td>Позиция в каталоге:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddGroup" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// ---
$tpl['catalogsub']['main'] =
'<h2>Каталог: %div%</h2><div align=right style="text-align:right;"><a href=catalogsub.html%refpar%&cmd=AddGroup&pgid=%pgid%><li> Добавить группу</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0>%list%</table><br><br>';

$tpl['catalogsub']['itemupline'] =
'<tr%bgcolor%><td width=100%><a href=catalogsub.html%refpar%&cmd=EditItem&subid=%subid%>%name%</a></td>
<td><nobr>%dt%</nobr></td>
<td align=right style="text-align:right;"><nobr><a href=catalogsub.html%refpar%&cmd=EditSub&subid=%subid%>[Состав выборки]</a>&nbsp;<a href=catalog.html%refpar%&cmd=DelItem&subid=%subid%>[Удалить выборку]</a>&nbsp;&nbsp;</nobr></td>
<td><a href=catalog.html%refpar%&subid=%subid%&upid=%upid%&cmd=UpItem><img src=/aimg/up_arrow.gif border=0></a></td></tr>';

$tpl['catalogsub']['itemline'] =
'<tr%bgcolor%><td width=100%><a href=catalogsub.html%refpar%&cmd=EditItem&subid=%subid%>%name%</a></td>
<td><nobr>%dt%</nobr></td>
<td align=right style="text-align:right;"><nobr><a href=catalogsub.html%refpar%&cmd=EditSub&subid=%subid%>[Состав выборки]</a>&nbsp;<a href=catalogsub.html%refpar%&cmd=DelItem&subid=%subid%>[Удалить выборку]</a>&nbsp;&nbsp;</nobr></td>
<td><img src=/aimg/t.gif border=0 width=16 height=16></nobr></td></tr>';

$tpl['catalogsub']['groupupline'] =
'<tr bgcolor=#000099>
<td><a href=catalogsub.html%refpar%&cmd=EditGroup&gid=%gid%><span style="color:white; font-weight:bold">%name%</span></a></td>
<td><span style="color:white; font-weight:bold">Когда используется</span></td>
<td align=right style="text-align:right;"><nobr><a href=catalogsub.html%refpar%&cmd=AddItem&gid=%gid%><span style="color:white; font-weight:bold">[Добавить выборку]</span></a>&nbsp;<a href=catalogsub.html%refpar%&cmd=DelGroup&gid=%gid% style="color:white"><span style="color:white; font-weight:bold">[Удалить группу]</span></a>&nbsp;&nbsp;</td>
<td><a href=catalog.html%refpar%&gid=%gid%&upid=%upid%&cmd=UpGroup><img src=/aimg/up_arrow.gif  width=16 height=16 border=0></a></nobr></td></tr>
';

$tpl['catalogsub']['groupline'] =
'<tr bgcolor=#000099>
<td><a href=catalogsub.html%refpar%&cmd=EditGroup&gid=%gid% style="color:white"><span style="color:white; font-weight:bold">%name%</span></a></td>
<td><span style="color:white; font-weight:bold">Когда используется</span></td>
<td align=right style="text-align:right;"><nobr><a href=catalogsub.html%refpar%&cmd=AddItem&gid=%gid% style="color:white"><span style="color:white; font-weight:bold">[Добавить выборку]</span></a>&nbsp;<a href=catalogsub.html%refpar%&cmd=DelGroup&gid=%gid% style="color:white"><span style="color:white; font-weight:bold">[Удалить группу]</span></a>&nbsp;&nbsp;</td>
<td><img src=/aimg/t.gif border=0 width=16 height=16></nobr></td></tr>
';

$tpl['catalogsub']['editSub'] =
'<form action="catalogsub.html" method=post>%formrefpar%
<input type=hidden name=subid value=%subid%>
<input type=hidden name=cmd value=doEditSub>
<table width=100% cellspacing=0 cellpadding=2 border=0>
%items%
</table>
<br><br><hr noshade>
<input type=submit name="btnDoEditSub" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Вернуться к списку  ">
</form>
';

$tpl['catalogsub']['daylist'] =
'<select name=subDay[%id%] size=0>
<option value=0 %day_0%>Каждый день</option>
<option value=1 %day_1%>Понедельник</option>
<option value=2 %day_2%>Вторник</option>
<option value=3 %day_3%>Среда</option>
<option value=4 %day_4%>Четверг</option>
<option value=5 %day_5%>Пятница-Воскресенье</option>
</select>
';

$tpl['catalogsub']['daylist2'] =
'<select name=day size=0>
<option value=0 %day_0%>Каждый день</option>
<option value=1 %day_1%>Понедельник</option>
<option value=2 %day_2%>Вторник</option>
<option value=3 %day_3%>Среда</option>
<option value=4 %day_4%>Четверг</option>
<option value=5 %day_5%>Пятница-Воскресенье</option>
</select>
';

$tpl['catalogsub']['topcheck'] =
'<input type="checkbox" name="subTop[%id%]" value="1" %checked%>';

$tpl['catalogsub']['editSubLine'] =
'<tr %bgcolor%><td width=100%>%name%</td><td>%day%</td><td>%top%</td><td><a href="%refpar%&cmd=doDelSubItem&subid=%subid%&subiid=%id%"><span style="color:#FF3300;">X</span></a></td></tr>
';

$tpl['catalogsub']['editSubGroup'] =
'<tr bgcolor=#000099><td width=100%><span style="color:white; font-weight:bold;">%groupname%</span></td><td colspan=3><nobr><a href="%refpar%&cmd=addSubItems&groupid=%groupid%&subid=%subid%"><span style="color:white; font-weight:bold;">[Добавить блюда]</span></a>&nbsp;<a href="%refpar%&cmd=doDelSubItems&groupid=%groupid%&subid=%subid%"><span style="color:white; font-weight:bold;">[Очистить]</span></a></nobr></td></tr>
<tr bgcolor="#3333CC">
<td width=100%><span style="color:#F0F0F0">Блюдо</span></td>
<td><span style="color:#F0F0F0">День</span></td>
<td><span style="color:#F0F0F0">ТОП</span></td>
<td><span style="color:#F0F0F0">X</span></td>
</tr>
%items%
<tr bgcolor="F9F9F9">
<td colspan=4 valign=top><img src=/img/t.gif width=10 height=4 border=0 align=top></td>
</tr>
';

$tpl['catalogsub']['addSubItems'] =
'<h1>Добавление/удаление блюд в выборку</h1>
<form action="catalogsub.html" method=post>%formrefpar%
<input type=hidden name=subid value=%subid%>
<input type=hidden name=cmd value=doAddSubItems>
<table width=100% cellspacing=0 cellpadding=1 border=0>
<tr bgcolor="#000099"><td colspan=2><span style="color:white; font-weight:bold;">%groupname%&nbsp;</span></td></tr>
<tr bgcolor="#3333CC"><td width=100%><span style="color:#F0F0F0">Блюдо</span></td><td><span style="color:#F0F0F0">Включено</span></td></tr>
%items%
</table>
<br><br><hr noshade>
<input type=submit name="btnDoAddSubItems" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['catalogsub']['addSubItemsLine'] =
'<tr %bgcolor%><td width=100%>%name%</td><td><input type="checkbox" name="itemCheck[%iid%]" value="1" %checked%><input type="hidden" name="itemPrev[%iid%]" value="%checked%"></td></tr>
';

// Редактирование формы

$tpl['forms']['main'] =
'<h2>Формы</h2><div align=right style="text-align:right;"><a href=forms.html%refpar%&cmd=AddForm><li> Добавить форму</a></div><br><table width=100% cellspacing=1 cellpadding=2 border=0 bgcolor=white>
<tr bgcolor=#000099><td style="color:white; font-weight:bold;">Название формы</td><td style="color:white; font-weight:bold; text-align:center;">Количество полей</td><td align=right style="text-align:right; color:white; font-weight:bold;">Действия</td></tr>
%list%</table><br><br>';


$tpl['forms']['itemline'] =
'<tr%bgcolor%><td><a href=forms.html%refpar%&cmd=EditForm&fid=%fid%>%name%</a></td><td align=right style="text-align:center;"><a href=forms.html%refpar%&cmd=ListFields&fid=%fid%>%fields%</a></td><td align=right style="text-align:right;"><a href=forms.html%refpar%&cmd=ListFields&fid=%fid%>[Поля формы]</a>&nbsp;<a href=forms.html%refpar%&cmd=DelForm&fid=%fid%>[Удалить форму]</a></td></tr>';

$tpl['forms']['FieldList'] =
'<h2>Формa: %name%</h2><form action="forms.html" method=post><input type=hidden name=fid value="%fid%"><input type=hidden name=cmd value="DelFields"><table cellspacing=0 cellpading=4 align=right style="text-align:right;"><tr><td width=100%>&nbsp;</td><td><nobr><li><input type="submit" name="btnDelFIelds" value="Удалить отмеченые" style="border: 0px; background:white; cursor: pointer;">&nbsp;&nbsp;&nbsp;&nbsp;</nobr></td><td><a href=forms.html%refpar%&cmd=AddField&fid=%fid%><nobr><li> Добавить поле&nbsp;&nbsp;&nbsp;&nbsp;</nobr></a></td><td><a href=forms.html%refpar%&cmd=&fid=0><nobr><li> К списку форм</nobr></a></td></tr></table><br><br><table width=100% cellspacing=1 cellpadding=2 border=0 bgcolor=white>
<tr bgcolor=#000099><td style="color:white; font-weight:bold;">*</td><td style="color:white; font-weight:bold;">Название поля</td><td style="color:white; font-weight:bold;">Поле</td><td style="color:white; font-weight:bold;">Тип</td><td align=right style="text-align:right; color:white; font-weight:bold;">Действия</td></tr>
%list%</table></form><br><br>';

$tpl['forms']['FieldLine'] =
'<tr%bgcolor%><td><input type=checkbox name="del_id[\'%iid%\']" value=%iid%></td><td><a href=forms.html%refpar%&cmd=EditField&iid=%iid%>%title%</a></td><td>%name%</a></td><td>%tp%</a></td><td align=right style="text-align:right;"><a href=forms.html%refpar%&cmd=DelFields&del_id["%iid%"]=%iid%>[Удалить поле]</a></td></tr>';

$tpl['forms']['editForm'] =
'<form action="forms.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=fid value=%fid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название формы:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Имя шаблона:</td><td><input type=text name="tplname" value="%tplname%" size=64 style="width:100%"></td></tr>
<tr><td>Адреса e-mail:<div style="font-size:smaller;">(разделитель ";")</div></td><td><textarea name="mail_list" cols=40 rows=4 style="width:100%">%mail_list%</textarea></td></tr>
<tr><td>Заголовок письма:</td><td><input type=text name="mail_header" value="%mail_header%" size=64 style="width:100%"></td></tr>
<tr><td>Письмо с адреса:</td><td><input type=text name="mail_from" value="%mail_from%" size=64 style="width:100%"></td></tr>
<tr><td>Использовать БД:</td><td><input type=checkbox name="usedb" value=1 size=64 %usedb_check%"></td></tr>
<tr><td>Insert:</td><td><input type=text name="sql_insert" value="%sql_insert%" size=64 style="width:100%"></td></tr>
<tr><td>Update:</td><td><input type=text name="sql_update" value="%sql_update%" size=64 style="width:100%"></td></tr>
<tr><td>Select:</td><td><input type=text name="sql_select" value="%sql_select%" size=64 style="width:100%"></td></tr>
<tr><td>Delete:</td><td><input type=text name="sql_delete" value="%sql_delete%" size=64 style="width:100%"></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddForm" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


$tpl['forms']['delForm'] =
'<form action="forms.html" method=post>%formrefpar%<input type=hidden name=fid value=%fid%>
<input type=hidden name=cmd value=doDelForm>
<h2>Подтвердите удаление формы</h2>
Все данные, связанные с удаляемой формой будут утеряны, восстановление невозможно.<br>
<table cellspacing=8 cellpadding=0 border=0>
<tr><td>Название формы:</td><td>%name%</td></tr>
<tr><td>Количество полей:</td><td>%fields%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelForm" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['forms']['delFields'] =
'<form action="forms.html" method=post>%formrefpar%<input type=hidden name=fid value=%fid%>
<input type=hidden name=cmd value=doDelFields><input type=hidden name=iids value="%iids%">
<h2>Подтвердите удаление полей формы %formname%</h2>
<table width=100% cellspacing=1 cellpadding=2 border=0>
<tr bgcolor=#000099><td style="color:white; font-weight:bold;">Поле</td><td style="color:white; font-weight:bold;">Имя</td><td style="color:white; font-weight:bold;">Тип</td></tr>
%fields%
</table>
<br><br><hr noshade>
<input type=submit name="btnDelFields" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancelF" value="  Отменить  ">
</form>
';

$tpl['forms']['delField'] =
'<tr%bgcolor%><td>%title%</td><td>%name%</td><td>%tp%</td></tr>
';

$tpl['forms']['nodelField'] =
'<tr><td align=center>Нет полей для удаления</td></tr>
';


$tpl['forms']['editField'] =
'<form action="forms.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=fid value=%fid%><input type=hidden name=iid value=%iid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название поля:</td><td><input type=text name="title" value="%title%" size=64 style="width:100%"></td></tr>
<tr><td>Имя поля:</td><td><input type=text name="name" value="%name%" size=32></td></tr>
<tr><td>Тип поля:</td><td><select size=0 name=tp>%tp_options%</select></td></tr>
<tr><td>Требуется/проверяется:</td><td><select size=0 name=req>%req_options%</select></td></tr>
<tr><td>Мах значение:</td><td><input type=text name="max" value="%max%" size=16></td></tr>
<tr><td>Мin значение:</td><td><input type=text name="min" value="%min%" size=16></td></tr>
<tr><td>Шаблон проверки:</td><td><input type=text name="pattern" value="%pattern%" size=64  style="width:100%"></td></tr>
<tr><td><nobr>Значение по умолчанию:</nobr></td><td width=100%><input type=text name="value" value="%value%" size=64  style="width:100%"></td></tr>
<tr><td>Элементы списка</td><td><textarea name="options" cols=40 rows=6 style="width:100%">%options%</textarea></td></tr>
<tr><td>SQL для списка:</td><td><input type=text name="sql_options" value="%sql_options%" size=64 style="width:100%"></td></tr>
<tr><td>Сообщениe об ошибке:</td><td><input type=text name="error" value="%error%" size=64  style="width:100%"></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddField" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancelF" value="  Отменить  ">
</form>
';

// Актуальные статьи

$tpl['actual']['main'] =
'<div align=right style="text-align:right;"><li><a href=actual.html%refpar%&cmd=Add>Добавить объявление</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Заголовок объявления</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['actual']['lineup'] =
'<tr%bgcolor%><td width=100%><a href=actual.html%refpar%&aid=%id%&cmd=Edit>%ttl%</a></td><td><nobr><a href=actual.html%refpar%&aid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<a href=actual.html%refpar%&aid=%id%&upid=%upid%&cmd=Up><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['actual']['line'] =
'<tr%bgcolor%><td width=100%><a href=actual.html%refpar%&aid=%id%&cmd=Edit>%ttl%</a></td><td><nobr><a href=actual.html%refpar%&aid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></nobr></td></tr>';

$tpl['actual']['edit'] =
'
<form action="actual.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=aid value=%aid%><input type=hidden name=cmd value=doEdit>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Заголовок:</td><td><input type=text name="ttl" value="%ttl%" size=64 style="width:100%"></td></tr>
<tr><td colspan=2>Аннотация:<br>
%txt1%
</td></tr>
<tr><td colspan=2>Текст:<br>
%txt2%
</td></tr>
<tr><td>Позиция в списке:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
<tr><td><nobr>Дата (ГГГГ-ММ-ДД):</nobr></td><td width=100%><input type=text name="dt" value="%dt%" size=16></td></tr>
<tr><td><nobr>Видимо в разделах:</nobr></td><td width=100%>%actual_divs%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['actual']['del'] =
'<form action="actual.html" method=post>%formrefpar%<input type=hidden name=aid value=%aid%>
<input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление объявления</h2>
Все данные, связанные с удаляемым объявлением будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Заголовок:</nobr></td><td width=100%>%ttl%</td></tr>
<tr><td><nobr>Аннотация:</nobr></td><td>%txt1%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// Новости

$tpl['news']['main'] =
'<div align=right style="text-align:right;"><li><a href=news.html%refpar%&cmd=Add>Добавить новость</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Дата</span></td><td><span style="color:white">Заголовок новости</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['news']['line'] =
'<tr%bgcolor%><td><nobr>&nbsp;%dt%&nbsp;</nobr></td><td width=100%><a href=news.html%refpar%&nid=%id%&cmd=Edit>%ttl%</a></td><td><a href=news.html%refpar%&nid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a></td></tr>';

$tpl['news']['edit'] =
'
<form action="news.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=nid value=%nid%><input type=hidden name=cmd value=doEdit>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Дата (ГГГГ-ММ-ДД):</nobr></td><td width=100%><input type=text name="dt" value="%dt%" size=16></td></tr>
<tr><td>Заголовок:</td><td><input type=text name="ttl" value="%ttl%" size=64 style="width:100%"></td></tr>
<tr><td colspan=2>Аннотация:<br>
%txt1%
</td></tr>
<tr><td colspan=2>Текст:<br>
%txt2%
</td></tr>
<tr><td colspan=2>Изображение (полное название файла из перечня ниже без пути):<br>
%img%
</td></tr>
<tr><td colspan=2>Изображения (пока добавляются только через FTP):<br>
<div style="display: flex;
    flex-wrap: wrap;
    align-items: flex-end;">
%imgs%
</div>
</td></tr>

<tr><td colspan=2><input type=checkbox name="top" value="1" %top% id=top><lafel for=top>Показывать на главной странице</label></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['news']['del'] =
'<form action="news.html" method=post>%formrefpar%<input type=hidden name=nid value=%nid%>
<input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление новости</h2>
Все данные, связанные с удаляемой новостью будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Дата:</td><td>%dt%</td></tr>
<tr><td><nobr>Заголовок:</nobr></td><td width=100%>%ttl%</td></tr>
<tr><td><nobr>Аннотация:</nobr></td><td>%txt1%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// Вопросы и ответы

$tpl['faq']['main'] =
'<div align=right style="text-align:right;"><a href=faq.html%refpar%&cmd=AddGroup><li> Добавить тему</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0>%list%</table><br><br>';

$tpl['faq']['itemline'] =
'<tr%bgcolor%><td><a href=faq.html%refpar%&cmd=EditItem&fid=%fid%>%question%</a></td><td align=right style="text-align:right;"><nobr><a href=faq.html%refpar%&cmd=DelItem&fid=%fid%>[Удалить вопрос]</a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></nobr></td></tr>';
$tpl['faq']['itemlineup'] =
'<tr%bgcolor%><td><a href=faq.html%refpar%&cmd=EditItem&fid=%fid%>%question%</a></td><td align=right style="text-align:right;"><nobr><a href=faq.html%refpar%&cmd=DelItem&fid=%fid%>[Удалить вопрос]</a>&nbsp;&nbsp;<a href=faq.html%refpar%&fid=%fid%&upid=%upid%&cmd=UpItem><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['faq']['groupline'] =
'<tr bgcolor=#000099><td><a href=faq.html%refpar%&cmd=EditGroup&tid=%tid% style="color:white">%name%</a></td><td align=right style="text-align:right;"><nobr><a href=faq.html%refpar%&cmd=AddItem&tid=%tid% style="color:white">[Добавить вопрос]</a>&nbsp;<a href=faq.html%refpar%&cmd=DelGroup&tid=%tid% style="color:white">[Удалить тему]</a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></nobr></td></tr>';

$tpl['faq']['grouplineup'] =
'<tr bgcolor=#000099><td><a href=faq.html%refpar%&cmd=EditGroup&tid=%tid% style="color:white">%name%</a></td><td align=right style="text-align:right;"><nobr><a href=faq.html%refpar%&cmd=AddItem&tid=%tid% style="color:white">[Добавить вопрос]</a>&nbsp;<a href=faq.html%refpar%&cmd=DelGroup&tid=%tid% style="color:white">[Удалить тему]</a>&nbsp;&nbsp;<a href=faq.html%refpar%&tid=%tid%&upid=%upid%&cmd=UpGroup><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['faq']['editItem'] =
'<form action="faq.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=fid value=%fid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Тема:</td><td><select size=0 name=tid>%tids%</select></td></tr>
<tr><td colspan=2>Вопрос:<br>
<input type="text" name=question size=40 style="width:100%" value="%question%">
</td></tr>
<tr><td colspan=2>Аннотация ответа:<br>
%abstract%
</td></tr>
<tr><td colspan=2>Ответ:<br>
%answer%
</td></tr>
<tr><td><nobr>Позиция в списке:</nobr></td><td width=100%><input type=text name="ord" value="%ord%" size=8></td></tr>

</table>
<br><br><hr noshade>
<input type=submit name="btnAddItem" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


$tpl['faq']['delItem'] =
'<form action="faq.html" method=post>%formrefpar%<input type=hidden name=fid value=%fid%>
<input type=hidden name=cmd value=doDelItem>
<h2>Подтвердите удаление вопроса</h2>
Все данные, связанные с удаляемым вопросом будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Вопрос:</td><td width=100%>%question%</td></tr>
<tr><td>Ответ:</td><td>%answer%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelItem" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>';

$tpl['faq']['delGroup'] =
'<form action="faq.html" method=post>%formrefpar%<input type=hidden name=tid value=%tid%>
<input type=hidden name=cmd value=doDelGroup>
<h2>Подтвердите удаление темы</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название темы:</nobr></td><td width=100%>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelGroup" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['faq']['nodelGroup'] =
'<form action="faq.html" method=post>%formrefpar%<input type=hidden name=tid value=%tid%>
<input type=hidden name=cmd value="">
<h2>Удаление темы невозможно</h2>
Тема содержит вопросы, для удаления необходимо удалить все вопросы темы либо перенести их в другую тему.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название темы:</td><td>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnCancel" value="  Вернуться в каталог  ">
</form>
';

$tpl['faq']['editGroup'] =
'<form action="faq.html" method=post enctype="multipart/form-data"">%formrefpar%<input type=hidden name=tid value=%tid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название темы:</nobr></td><td width=100%><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Позиция:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddGroup" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';




// Советы

$tpl['advice']['main'] =
'<div align=right style="text-align:right;"><a href=advice.html%refpar%&cmd=AddGroup><li> Добавить тему</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0>%list%</table><br><br>';

$tpl['advice']['itemline'] =
'<tr%bgcolor%><td><a href=advice.html%refpar%&cmd=EditItem&fid=%fid%>%title%</a></td><td align=right style="text-align:right;"><nobr><a href=advice.html%refpar%&cmd=DelItem&fid=%fid%>[Удалить совет]</a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></nobr></td></tr>';
$tpl['advice']['itemlineup'] =
'<tr%bgcolor%><td><a href=advice.html%refpar%&cmd=EditItem&fid=%fid%>%title%</a></td><td align=right style="text-align:right;"><nobr><a href=advice.html%refpar%&cmd=DelItem&fid=%fid%>[Удалить совет]</a>&nbsp;&nbsp;<a href=advice.html%refpar%&fid=%fid%&upid=%upid%&cmd=UpItem><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['advice']['groupline'] =
'<tr bgcolor=#000099><td><a href=advice.html%refpar%&cmd=EditGroup&tid=%tid% style="color:white">%name%</a></td><td align=right style="text-align:right;"><nobr><a href=advice.html%refpar%&cmd=AddItem&tid=%tid% style="color:white">[Добавить совет]</a>&nbsp;<a href=advice.html%refpar%&cmd=DelGroup&tid=%tid% style="color:white">[Удалить тему]</a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></nobr></td></tr>';

$tpl['advice']['grouplineup'] =
'<tr bgcolor=#000099><td><a href=advice.html%refpar%&cmd=EditGroup&tid=%tid% style="color:white">%name%</a></td><td align=right style="text-align:right;"><nobr><a href=advice.html%refpar%&cmd=AddItem&tid=%tid% style="color:white">[Добавить совет]</a>&nbsp;<a href=advice.html%refpar%&cmd=DelGroup&tid=%tid% style="color:white">[Удалить тему]</a>&nbsp;&nbsp;<a href=advice.html%refpar%&tid=%tid%&upid=%upid%&cmd=UpGroup><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['advice']['editItem'] =
'<form action="advice.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=fid value=%fid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Тема:</td><td><select size=0 name=tid>%tids%</select></td></tr>
<tr><td colspan=2>Заголовок:<br>
<input type="text" name=title size=40 style="width:100%" value="%title%">
</td></tr>
<tr><td colspan=2>Аннотация:<br>
%abstract%
</td></tr>
<tr><td colspan=2>Подробный текст:<br>
%txt%
</td></tr>
<tr><td colspan=2><input type=checkbox name="top" value="1" %top% id=top><lafel for=top>Топовый совет/публикация</label></td></tr>
<tr><td><nobr>Позиция в списке:</nobr></td><td width=100%><input type=text name="ord" value="%ord%" size=8></td></tr>

</table>
<br><br><hr noshade>
<input type=submit name="btnAddItem" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';



$tpl['advice']['delItem'] =
'<form action="advice.html" method=post>%formrefpar%<input type=hidden name=fid value=%fid%>
<input type=hidden name=cmd value=doDelItem>
<h2>Подтвердите удаление совета</h2>
Все данные, связанные с удаляемым советом будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Вопрос:</td><td width=100%>%title%</td></tr>
<tr><td>Ответ:</td><td>%abstract%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelItem" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['advice']['delGroup'] =
'<form action="advice.html" method=post>%formrefpar%<input type=hidden name=tid value=%tid%>
<input type=hidden name=cmd value=doDelGroup>
<h2>Подтвердите удаление темы</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название темы:</nobr></td><td width=100%>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDelGroup" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['advice']['nodelGroup'] =
'<form action="advice.html" method=post>%formrefpar%<input type=hidden name=tid value=%tid%>
<input type=hidden name=cmd value="">
<h2>Удаление темы невозможно</h2>
Тема содержит советы, для удаления необходимо удалить все советы темы либо перенести их в другую тему.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название темы:</td><td>%name%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnCancel" value="  Вернуться к списку  ">
</form>
';

$tpl['advice']['editGroup'] =
'<form action="advice.html" method=post enctype="multipart/form-data"">%formrefpar%<input type=hidden name=tid value=%tid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название совета:</nobr></td><td width=100%><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Позиция:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddGroup" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';







// ========================================== Структура сайта

// Разделы

$tpl['divdefs']['main'] =
'<div align=right style="text-align:right;"><a href=divdefs.html%refpar%&cmd=Edit&pid=0><li> Добавить раздел</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Раздел</span></td><td><span style="color:white">Страница по умолчанию</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['divdefs']['divlineup'] =
'<tr%bgcolor%><td><a href=divdefs.html%refpar%&did=%id%>%name%</a></td><td>%def_page%</td><td><a href=divdefs.html%refpar%&did=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<a href=divdefs.html%refpar%&did=%id%&upid=%upid%&cmd=Up><img src=/aimg/up_arrow.gif border=0></a></td></tr>';

$tpl['divdefs']['divline'] =
'<tr%bgcolor%><td><a href=divdefs.html%refpar%&did=%id%>%name%</a></td><td>%def_page%</td><td><a href=divdefs.html%refpar%&did=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0></td></tr>';

$tpl['divdefs']['edit'] =
'<form action="divdefs.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=did value=%did%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название раздела:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Страница по умолчанию:</td><td><select size=0 name=def_page>%def_page%</select></td></tr>
<tr><td>Префикс раздела:</td><td><input type=text name="prefix" value="%prefix%" size=64  style="width:100%"></td></tr>
<tr><td>Позиция разделa:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
<tr><td colspan=2><input type=checkbox name="menu1" value="1" id=pgmenu1 %pgmenu1%><label for=pgmenu1>Показывать ссылку в списке разделов</label></td></tr>
<tr><td>e-mail для рассылки:</td><td><input type=text name="email" value="%email%" size=64  style="width:100%"></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['divdefs']['del'] =
'<form action="divdefs.html" method=post>%formrefpar%<input type=hidden name=did value=%did%>
<input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление раздела &laquo;%ttl%&raquo;</h2>
Все данные, связанные с удаляемым разделом будут утеряны, восстановление невозможно.<br>
<br><br>
<div align=center><input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  "></div>
</form>
';

$tpl['divdefs']['nodel'] =
'<form action="divdefs.html" method=post>%formrefpar%<input type=hidden name=did value=%did%>
<input type=hidden name=cmd value=doDel>
<h2>Невозможно удаление раздела &laquo;%ttl%&raquo;</h2>
Раздел содержит %count% страниц. Перед удалением раздела необходимр удалить страницы<br>
<br><br>
<div align=center><input type=submit name="btnCancel" value="  Отменить  "></div>
</form>
';

// --- Страницы

$tpl['pagedefs']['main'] =
'<div align=right style="text-align:right;"><a href=pagedefs.html%refpar%&cmd=Edit&pid=0><li> Добавить страницу</a></div><br><table width=100% cellspacing=0 cellpadding=3 border=0><tr bgcolor=#000099><td><span style="color:white">Страница</span></td><td><span style="color:white">Ссылка</span></td><td><span style="color:white">Тип страницы</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['pagedefs']['divline'] =
'<tr bgcolor=#0000CC><td colspan=4><span style="color:white">%ttl%</span></td></tr>';

$tpl['pagedefs']['pagelineup'] =
'<tr%bgcolor%><td><a href=pagedefs.html%refpar%&pid=%id%>%ttl%</a></td><td>%name%</td><td>%tp%</td><td align=right style="text-align:right;"><nobr>%clone%%content%<a href=pagedefs.html%refpar%&pid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<a href=pagedefs.html%refpar%&pid=%id%&upid=%upid%&cmd=Up><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['pagedefs']['pageline'] =
'<tr%bgcolor%><td><a href=pagedefs.html%refpar%&pid=%id%>%ttl%</a></td><td>%name%</td><td>%tp%</td><td align=right style="text-align:right;">%clone%%content%<a href=pagedefs.html%refpar%&pid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></td></tr>';

$tpl['pagedefs']['pagelineblank'] =
'<tr%bgcolor%><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td align=right style="text-align:right;">&nbsp;</td></tr>';

$tpl['pagedefs']['content_text'] =
'<a href=pages.html%refpar%&pid=%id%><img src=/aimg/edit.gif border=0></a>&nbsp;&nbsp;&nbsp;';

$tpl['pagedefs']['clone_btn'] =
'<a href=pagedefs.html%refpar%&pid=%id%&cmd=Clone><img src=/aimg/clone.gif border=0></a>&nbsp;&nbsp;&nbsp;';

$tpl['pagedefs']['clone_text'] =
'<span style="color:blue; text-weight:bold;">клон</span>&nbsp;&nbsp;&nbsp;';

$tpl['pagedefs']['nopage'] =
'<h2>Редактирование невозможно</h2>Запрошена несуществующая страница.';

$tpl['pagedefs']['edit'] =
'<form action="pagedefs.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=pid value=%pid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Заголовок страницы:</td><td><input type=text name="ttl" value="%ttl%" size=64 style="width:100%"></td></tr>
<tr><td>Физическая страница:</td><td><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Раздел:</td><td><select size=0 name=did>%dids%</select></td></tr>
%page_groups%
<tr><td>Позиция в разделе:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
<tr><td>Пункт меню:</td><td><input type=text name="page_ttlm" value="%page_ttlm%" style="width:100%"></td></tr>
<tr><td>Шапка:</td><td>%themes%</td></tr>
<tr><td>Тип страницы:</td><td><select size=0 name=page_tp>%page_tps%</select></td></tr>
%subtype%
<tr><td colspan=2><input type=checkbox name="menu1" value="1" id=pgmenu1 %pgmenu1%><label for=pgmenu1>Показывать ссылку на главной странице</label></td></tr>
<tr><td colspan=2><input type=checkbox name="locked" value="1" id=locked %locked%><label for=locked>Заблокировать страницу</label></td></tr>
<tr><td colspan=2>Для поисковиков<br><hr noshade></td></tr>
<tr><td>Дополнение к заголовку:</td><td><input type=text name="page_ttla" value="%page_ttla%" style="width:100%"></td></tr>
<tr><td>Ключевые слова:</td><td><textarea name="page_keywords" cols=64 rows=8 style="width:100%">%page_keywords%</textarea></td></tr>
<tr><td>Описание:</td><td><textarea name="page_descr" cols=64 rows=8 style="width:100%">%page_descr%</textarea></td></tr>
<tr><td>Дополнительные META:</td><td><textarea name="page_meta" cols=64 rows=8 style="width:100%">%page_meta%</textarea></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['pagedefs']['editclone'] =
'<form action="pagedefs.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=pid value=%pid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Заголовок страницы:</td><td><input type=text name="ttl" value="%ttl%" size=64 style="width:100%"></td></tr>
<tr><td>Физическая страница:</td><td>%name% (редактирование запрещено - клон)</td></tr>
<input type=hidden name=did value=%did%>
<input type=hidden name=gid value=%gid%>
<input type=hidden name=ord value=%ord%>
<input type=hidden name=page_ttlm value=%page_ttlm%>
<tr><td>Шапка:</td><td>%themes%</td></tr>
<tr><td>Тип страницы:</td><td>Текст (редактирование запрещено - клон)</td></tr>
<input type=hidden name=page_tp value=2>
%subtype%
<input type=hidden name=pgmenu1 value=0>
<tr><td colspan=2><input type=checkbox name="locked" value="1" id=locked %locked%><label for=locked>Заблокировать страницу</label></td></tr>
<tr><td colspan=2>Для поисковиков<br><hr noshade></td></tr>
<tr><td>Дополнение к заголовку:</td><td><input type=text name="page_ttla" value="%page_ttla%" style="width:100%"></td></tr>
<tr><td>Ключевые слова:</td><td><textarea name="page_keywords" cols=64 rows=8 style="width:100%">%page_keywords%</textarea></td></tr>
<tr><td>Описание:</td><td><textarea name="page_descr" cols=64 rows=8 style="width:100%">%page_descr%</textarea></td></tr>
<tr><td>Дополнительные META:</td><td><textarea name="page_meta" cols=64 rows=8 style="width:100%">%page_meta%</textarea></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


$tpl['pagedefs']['page_groups'] =
'<tr><td>Группа страниц:</td><td><select size=0 name=page_group>%list%</select></td></tr>
';

$tpl['pagedefs']['del'] =
'<form action="pagedefs.html" method=post>%formrefpar%<input type=hidden name=pid value=%pid%>
<input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление страницы</h2>
Все данные, связанные с удаляемой страницей будут утеряны, восстановление невозможно.<br>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Заголовок страницы:</td><td>%ttl%</td></tr>
<tr><td>Физическая страница:</td><td>%name%</td></tr>
<tr><td>Раздел:</td><td>%divname%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


$tpl['pagedefs']['themes'] =
'<table width=100% cellspacing=8 cellpadding=0 border=0>%list%</table>
';

$tpl['pagedefs']['themeline'] =
'<tr><td width=20 valign=center align=left><input type=radio name="theme" value="%id%" %sel%></td><td ><img src=%ref% width=300 height=100 border=0></td><td align=left width=100%><span style="font-size:larger">%ttl%</span></td></tr>
';

$tpl['pagedefs']['edit_text'] =
'<tr><td>Макет страницы:</td><td><select size=0 name=page_subtp>%list%</select></td></tr>
';

// Макеты текстовых страниц

$tpl['patterns']['main'] =
'
<div align=right style="text-align:right;"><li><a href=patterns.html%refpar%&cmd=Add>Добавить макет</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Название</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>
';

$tpl['patterns']['line'] =
'<tr%bgcolor%><td><nobr>&nbsp;<a href=patterns.html%refpar%&pid=%id%&cmd=Edit>%name%</a>&nbsp;</nobr></td><td><a href=patterns.html%refpar%&pid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a></td></tr>';

$tpl['patterns']['edit'] =
'<form action="patterns.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name="pid" value="%id%"><input type=hidden name=cmd value=doEdit>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название:</nobr></td><td width=100%><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Макет:</td><td><textarea cols="60" rows="16" name="maket" style="width:100%">%maket%</textarea></td></tr>
<tr><td>Параметры:</td><td><textarea cols="60" rows="12" name="params" style="width:100%">%params%</textarea></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['patterns']['del'] =
'<form action="patterns.html" method=post>%formrefpar%<input type=hidden name="pid" value="%id%"><input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление макета</h2>
Удаление макета необратимая операция.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название макета:</nobr></td><td width=100%><strong>%name%</strong></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['patterns']['nodel'] =
'<form action="patterns.html" method=post>%formrefpar%<input type=hidden name="pid" value="%id%"><input type=hidden name=cmd value=doDel>
<h2>Удаление макета невозможно</h2>
Макет используетмся для формирования %c1% страниц сайта и %c2% страниц каталога. Для удаления макета сначала удалите эти страницы или используйте для их формирования другой макет.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название макета</nobr>:</td><td width=100%><strong>%name%</strong></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// Макеты таблиц

$tpl['tables']['main'] =
'
<div align=right style="text-align:right;"><li><a href=tables.html%refpar%&cmd=Add>Добавить макет</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Название</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>
';

$tpl['tables']['line'] =
'<tr%bgcolor%><td><nobr>&nbsp;<a href=tables.html%refpar%&pid=%id%&cmd=Edit>%name%</a>&nbsp;</nobr></td><td><a href=tables.html%refpar%&pid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a></td></tr>';

$tpl['tables']['edit'] =
'<form action="tables.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name="pid" value="%id%"><input type=hidden name=cmd value=doEdit>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название:</nobr></td><td width=100%><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td>Макет таблицы:</td><td><textarea cols="60" rows="8" name="main" style="width:100%">%main%</textarea></td></tr>
<tr><td>Строка заголовка:</td><td><textarea cols="60" rows="4" name="ttl_row" style="width:100%">%ttl_row%</textarea></td></tr>
<tr><td>Ячейки строки заголовка:</td><td><textarea cols="60" rows="4" name="ttl_cells" style="width:100%">%ttl_cells%</textarea></td></tr>
<tr><td>Четные строки:</td><td><textarea cols="60" rows="4" name="even_row" style="width:100%">%even_row%</textarea></td></tr>
<tr><td>Нечетные строки:</td><td><textarea cols="60" rows="4" name="odd_row" style="width:100%">%odd_row%</textarea></td></tr>
<tr><td>Ячейки четных строк:</td><td><textarea cols="60" rows="8" name="even_cells" style="width:100%">%even_cells%</textarea></td></tr>
<tr><td>Ячейки нечетных строк:</td><td><textarea cols="60" rows="8" name="odd_cells" style="width:100%">%odd_cells%</textarea></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['tables']['del'] =
'<form action="tables.html" method=post>%formrefpar%<input type=hidden name="pid" value="%id%"><input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление макета</h2>
Удаление макета необратимая операция.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название макета:</nobr></td><td width=100%><strong>%name%</strong></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['tables']['nodel'] =
'<form action="tables.html" method=post>%formrefpar%<input type=hidden name="pid" value="%id%"><input type=hidden name=cmd value=doDel>
<h2>Удаление макета невозможно</h2>
Макет используетмся для формирования %c1% таблиц на страницах сайта и %c2% таблиц на страницах каталога. Для удаления макета сначала удалите эти страницы или используйте для их формирования другой макет.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название макета</nobr>:</td><td width=100%><strong>%name%</strong></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// Меню

$tpl['menus']['main'] =
'<div align=right style="text-align:right;"><li><a href=menus.html%refpar%&cmd=Add>Добавить меню</a></div>
<table width=100% cellspacing=0 cellpadding=3 border=0><tr bgcolor=#000099><td><span style="color:white">Заголовок меню</span></td><td><span style="color:white">Имя&nbsp;меню</span></td><td><span style="color:white">Раздел</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['menus']['line'] =
'<tr%bgcolor%><td width=100%><a href=menus.html%refpar%&mid=%id%&cmd=Edit>%ttl%</a></td><td><nobr>%menu_name%</nobr></td><td><nobr>%div%</nobr></td><td><nobr><a href=menus.html%refpar%&mid=%id%&cmd=ListItem>[Пункты меню]</a></nobr></td></tr>';

$tpl['menus']['edit'] =
'<form action="menus.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=mid value=%id%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Заголовок меню:</nobr></td><td width=100%><input type=text name="title" value="%title%" size=64 style="width:100%"></td></tr>
<tr><td><nobr>Имя меню:</nobr></td><td width=100%><input type=text name="menu_name" value="%menu_name%" size=64 style="width:100%"></td></tr>
<tr><td>Раздел:</td><td><select size=0 name=menu_did>%dids%</select></td></tr>
<tr><td><nobr>Имя шаблона:</nobr></td><td width=100%><input type=text name="menu_tpl" value="%menu_tpl%" size=64 style="width:100%"></td></tr>

</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['menus']['itemlist'] =
'<h1>Меню: %menu_name%</h1><table width=100% border=0><tr><td align=left><a href=menus.html%refpar%&cmd=>К списку меню</a></td><td align=right style="text-align:right;"><li><a href=menus.html%refpar%&cmd=AddItem&mid=%mid%>Добавить пункт меню</a></td></tr></table><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Пункт меню</span></td><td><span style="color:white"><nobr>Страница или ссылка</nobr></span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['menus']['itemlineup'] =
'<tr%bgcolor%><td width=100%><a href=menus.html%refpar%&iid=%id%&cmd=EditItem>%ttl%</a></td><td><nobr>%page%</nobr></td><td><nobr><a href=menus.html%refpar%&iid=%id%&cmd=DelItem><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<a href=menus.html%refpar%&iid=%id%&upid=%upid%&cmd=Up><img src=/aimg/up_arrow.gif border=0></a></nobr></td></tr>';

$tpl['menus']['itemline'] =
'<tr%bgcolor%><td width=100%><a href=menus.html%refpar%&iid=%id%&cmd=EditItem>%ttl%</a></td><td><nobr>%page%</nobr></td><td><nobr><a href=menus.html%refpar%&iid=%id%&cmd=DelItem><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<img src=/aimg/t.gif border=0 width=16></nobr></td></tr>';

$tpl['menus']['delitem'] =
'<form action="menus.html" method=post>%formrefpar%<input type=hidden name="iid" value="%id%"><input type=hidden name=cmd value=doDelItem>
<h2>Подтвердите удаление пункта меню</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название:</nobr></td><td width=100%><strong>%ttl%</strong></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancelI" value="  Отменить  ">
</form>
';

$tpl['menus']['edititem'] =
'
<h2>Пункт меню</h2><form action="menus.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=iid value=%iid%><input type=hidden name=mid value=%mid%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Заголовок:</td><td><input type=text name="title" value="%title%" size=64 style="width:100%"></td></tr>
<tr><td><nobr>Уровень вложенности (1-2)</nobr>:</td><td><input type=text name="level" value="%level%" size=8></td></tr>
<tr><td>Позиция в списке:</td><td><input type=text name="ord" value="%ord%" size=8></td></tr>
<tr><td colspan=2>Использовать как:</td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb0 value="0" %sel0%><label for=rb0>Заголовок:</label></td><td width=100%>&nbsp;</td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb1 value="1" %sel1%><label for=rb1>Внешняя ссылка:</label></td><td width=100%><input type=text name="txt1" value="%txt1%" size=64 style="width:100%"></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb2 value="2" %sel2%><label for=rb2>Вложенное меню:</label></td><td width=100%><select name="txt2" size=0>%menus2%</td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb3 value="3" %sel3%><label for=rb3>Ссылка на страницу:</label></td><td width=100%><select name="hid3" size=0>%pages%</select>&nbsp;<input type=text name="txt3" value="%txt3%" size=64></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb5 value="5" %sel5%><label for=rb5>Список разделов видимых:</label></td><td width=100%>&nbsp;</td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb6 value="6" %sel6%><label for=rb6>Список разделов видимых:</label></td><td width=100%>&nbsp;</td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb4 value="4" %sel4%><label for=rb4>Ссылка на раздел:</label></td><td width=100%><select name="hid4" size=0>%divs4%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb7 value="7" %sel7%><label for=rb7>Страницы раздела видимые:</label></td><td width=100%><select name="hid7" size=0>%divs7%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb8 value="8" %sel8%><label for=rb8>Страницы раздела вcе:</label></td><td width=100%><select name="hid8" size=0>%divs8%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb9 value="9" %sel9%><label for=rb9>Для видимых разделов меню</label></td><td width=100%><select name="txt9" size=0>%menus9%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb10 value="10" %sel10%><label for=rb10>Для вcех разделов меню</label></td><td width=100%><select name="txt10" size=0>%menus10%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb11 value="11" %sel11%><label for=rb11>Список групп раздела:</label></td><td width=100%><select name="hid11" size=0>%divs11%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb12 value="12" %sel12%><label for=rb12>Страницы группы видимые:</label></td><td width=100%><select name="hid12" size=0>%groups12%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb13 value="13" %sel13%><label for=rb13>Страницы группы все:</label></td><td width=100%><select name="hid13" size=0>%groups13%</select></td></tr>

<tr><td><nobr><input type="radio" name="tp" id=rb14 value="14" %sel14%><label for=rb14>Для всех групп раздела меню</label></td><td width=100%><select name="hid14" size=0>%divs14%</select>&nbsp;<select name="txt14" size=0>%menus14%</select></td></tr>
<tr><td>Дополнительно 1:</td><td><input type=text name="ext1" value="%ext1%" size=40></td></tr>
<tr><td>Дополнительно 2:</td><td><input type=text name="ext2" value="%ext2%" size=40></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAddI" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancelI" value="  Отменить  ">
</form>

';

// ==============  Настройки

// Параметры

$tpl['params']['main'] =
'<h1>Параметры-строки</h1>
<div align=right style="text-align:right;"><li><a href=params.html%refpar%&cmd=Add&tp=1>Добавить параметр-строку</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Название</span></td><td><span style="color:white">Описание</span></td><td><span style="color:white">Действия</span></td></tr>%list1%</table><br><br>
<h1>Параметры-текст</h1>
<div align=right style="text-align:right;"><li><a href=params.html%refpar%&cmd=Add&tp=2>Добавить параметр-текст</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Название</span></td><td><span style="color:white">Описание</span></td><td><span style="color:white">Действия</span></td></tr>%list2%</table><br><br>';

$tpl['params']['line'] =
'<tr%bgcolor%><td><nobr>&nbsp;<a href=params.html%refpar%&name=%nameref%&tp=%tp%&cmd=Edit>%name%</a>&nbsp;</nobr></td><td width=100%>%descr%</td><td><a href=params.html%refpar%&name=%nameref%&tp=%tp%&cmd=Del><img src=/aimg/del.gif border=0></a></td></tr>';

$tpl['params']['add'] =
'<form action="params.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=tp value=%tp%><input type=hidden name=cmd value=doAdd>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название:</nobr></td><td width=100%><input type=text name="name" value="%name%" size=16  style="width:100%"></td></tr>
<tr><td>Описание:</td><td><textarea cols="60" rows="8" name="descr" style="width:100%">%descr%</textarea></td></tr>
<tr><td>Значение:</td><td>%value%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['params']['edit'] =
'<form action="params.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=tp value=%tp%><input type=hidden name="name" value="%name%"><input type=hidden name=cmd value=doEdit>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Название:</nobr></td><td width=100%><strong>%name%</strong></td></tr>
<tr><td>Описание:</td><td><textarea cols="60" rows="8" name="descr" style="width:100%">%descr%</textarea></td></tr>
<tr><td>Значение:</td><td>%value%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['params']['del'] =
'<form action="params.html" method=post>%formrefpar%<input type=hidden name=tp value=%tp%><input type=hidden name="name" value="%name%">
<input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление параметра</h2>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Название:</td><td>%name%</td></tr>
<tr><td>Описание:</td><td>%descr%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

// Шаблоны

$tpl['tpls']['main'] =
'
<div align=right style="text-align:right;"><li><a href=tpls.html%refpar%&cmd=Add&tp=1>Добавить шаблон</a></div><br><table width=100% cellspacing=0 cellpadding=2 border=0><tr bgcolor=#000099><td><span style="color:white">Имя</span></td><td><span style="color:white">Название</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>
';

$tpl['tpls']['line'] =
'<tr%bgcolor%><td><nobr>&nbsp;<a href=tpls.html%refpar%&tid=%id%&cmd=Edit>%name%</a>&nbsp;</nobr></td><td width=100%>%title%</td><td><a href=tpls.html%refpar%&tid=%id%&cmd=Links>[Привязка]</a>&nbsp;&nbsp;<a href=tpls.html%refpar%&tid=%id%&cmd=Del><img src=/aimg/del.gif border=0></a></td></tr>';

$tpl['tpls']['edit'] =
'<form action="tpls.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name="tid" value="%id%"><input type=hidden name=cmd value=doEdit>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Имя:</nobr></td><td width=100%><input type=text name="name" value="%name%" size=64 style="width:100%"></td></tr>
<tr><td><nobr>Название:</nobr></td><td><input type=text name="title" value="%title%" size=64 style="width:100%"></td></tr>
<tr><td><nobr>Тип:</nobr></td><td><select name="tp" size=0>%tps%</select></td></tr>
<tr><td>Значение:</td><td><textarea cols="60" rows="24" name="txt" style="width:100%">%txt%</textarea></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['tpls']['del'] =
'<form action="tpls.html" method=post>%formrefpar%<input type=hidden name="tid" value="%id%"><input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление шаблона</h2>
Удаление шаблона необратимая операция и может привести к неработоспособности сайта.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Имя:</td><td>%name%</td></tr>
<tr><td>Название:</td><td width=100%>%title%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['tpls']['links'] =
'<form action="tpls.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name="tid" value="%tid%"><input type=hidden name=cmd value=doLink><table width=100% border=0 cellspacing=4>%list%</table><br><br><hr noshade>
<input type=submit name="btnLink" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>';

$tpl['tpls']['divlink'] =
'<tr><td colspan=2><input type="checkbox" name="divlink[%did%]" value="1" id=divlink%did% %sel%><link for=divlink%did%><strong>%name%</strong></link></td></tr>';

$tpl['tpls']['pagelink'] =
'<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;</td><td width=100%><input type="checkbox" name="pagelink[%pgid%]" value="1" id=pagelink%did% %sel%><link for=pagelink%pgid%>%name%</link></td></tr>';

// ==============  Администрование

// Пользователи

$tpl['users']['main'] =
'<div align=right style="text-align:right;"><a href=users.html%refpar%&cmd=Add><li> Добавить пользователя</a></div><br><table width=100% cellspacing=0 cellpadding=3 border=0><tr bgcolor=#000099><td><span style="color:white">Имя</span></td><td><span style="color:white">Реальное имя</span></td><td><span style="color:white">Роли</span></td><td><span style="color:white">Действия</span></td></tr>%list%</table><br><br>';

$tpl['users']['line'] =
'<tr%bgcolor%><td><a href=users.html%refpar%&cmd=Edit&user_id=%id%>%nic%</a></td><td>%fio%</td><td>%role%</td><td><a href=users.html%refpar%&user_id=%id%&cmd=Del><img src=/aimg/del.gif border=0></a>&nbsp;&nbsp;<a href=users.html%refpar%&user_id=%id%&cmd=Places>[Площадки]</a></td></tr>';

$tpl['users']['edit'] =
'<form action="users.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=user_id value=%id%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Имя пользователя:</nobr></td><td width=100%><input type=text name="user_nic" value="%user_nic%" size=64></td></tr>

<tr><td><nobr>Пароль:</nobr></td><td width=100%><input type=password name="user_pwd1" size=64></td></tr>
<tr><td><nobr>Ещё раз:</nobr></td><td width=100%><input type=password name="user_pwd2" size=64></td></tr>
<tr><td>Реальное имя:</td><td><input type=text name="user_fio" value="%user_fio%" size=64 style="width:100%"></td></tr>
<tr><td>Телефон:</td><td><input type=text name="user_phone" value="%user_phone%" size=24></td></tr>
<tr><td><nobr>Электронная почта:</nobr></td><td><input type=text name="user_email" value="%user_email%" size=64 style="width:100%"></td></tr>

<tr style="background: #FFE0E0"><td>Роли пользователя:</td><td>%user_roles%</td></tr>

<tr><td>Доступ к разделам:</td><td>%user_divs%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnAdd" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


$tpl['users']['places'] =
'
<style>
tr.even, tr.even td{background-color:F0F0FF !important;}
tr.odd, tr.odd td{background-color:F0F0F0 !important;}
</style>

<form action="users.html" method=post enctype="multipart/form-data">%formrefpar%<input type=hidden name=user_id value=%user_id%>
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td><nobr>Имя пользователя:</nobr></td><td width=100%>%user_nic% (%user_fio%)</tr>
<!--<tr><td>Роль пользователя:</td><td><select size=0 name=user_role>%user_roles%</select></td></tr>-->
<tr><td>Площадки:</td><td><table width=100% cellspacing=8 cellpadding=0 border=0>%user_places%</table></td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnPlaces" value="  Принять изменения  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['users']['places_line'] =
'<tr class=%rowclass%><td>%place%</td><td><select name="user_rights[%place_id%]"><option value="All" %SelectedAll%>Полный доступ</option><option value="Read" %SelectedRead%>Только чтение</option><option value="None" %SelectedNone%>Нет доступа</option></select></td></tr>
';

$tpl['users']['del'] =
'<form action="users.html" method=post>%formrefpar%<input type=hidden name="user_id" value="%id%"><input type=hidden name=cmd value=doDel>
<h2>Подтвердите удаление пользователя</h2>
Удаление пользователя необратимая операция и может привести к неработоспособности сайта.
<table width=100% cellspacing=8 cellpadding=0 border=0>
<tr><td>Имя:</td><td>%user_nic%</td></tr>
<tr><td>Реальное имя:</td><td width=100%>%user_fio%</td></tr>
<tr><td>Полномочия:</td><td width=100%>%user_role%</td></tr>
</table>
<br><br><hr noshade>
<input type=submit name="btnDel" value="  Подтвердить удаление  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';

$tpl['users']['nodel'] =
'<form action="users.html" method=post>%formrefpar%
<h2>Нельзя удалить пользователя admin</h2>
Удаление встоенной учетной записи администратора приведет к неработоспособности сайта, удаление невозможно.<br><br>
<input type=submit name="btnCancel" value="  Отменить  ">
</form>
';


// +++++++++++++++++++++++++++++++++++

$tpl['common']['error'] = '
<div class="ui-widget">
<div class="ui-state-error ui-corner-all" style="padding: 1em;"> 
<span class="ui-icon ui-icon-alert" style="float: left; margin-right: .3em;"></span> 
<strong>%title%</strong> %text%
</div></div>';

$tpl['common']['highlight'] = '
<div class="ui-widget">
<div class="ui-state-highlight ui-corner-all" style="padding: 0 .7em;"> 
<p><span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>
<strong>%title%</strong> %text%</p>
</div></div>';
