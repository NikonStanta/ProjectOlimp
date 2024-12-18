<?php

// Чистка школ
//error_reporting(E_ALL);

include_once('olimp_common.inc');

function build_page()
{
    global $tpl,$refpar,$formrefpar,$sitemap,$uid,$role,$user,$pagemenu,$mysqli;

    if (($role & 8) == 0) {
        return $tpl['noaccess'];
    }

    if (isset($_REQUEST['cmd'])) {
        $cmd = $_REQUEST['cmd'];
    } else {
        $cmd = '';
    }

    if (isset($_REQUEST['btnMerge'])) {
        $cmd = 'Merge';
    }

    if (isset($_REQUEST['btnDelete'])) {
        $cmd = 'Delete';
    }

    if (isset($_REQUEST['btnCancel'])) {
        $cmd = '';
    }


// 50
// Фильтр
    if (isset($_REQUEST['filterCountry'])) {
        $filterCountry = $_REQUEST['filterCountry'];
        $_SESSION['filterCountry'] = $filterCountry;
    } elseif (isset($_SESSION['filterCountry'])) {
        $filterCountry = $_SESSION['filterCountry'];
    } else {
        $filterCountry = 0;
        $_SESSION['filterCountry'] = 0;
    }
    if (isset($_REQUEST['filterRegion'])) {
        $filterRegion = $_REQUEST['filterRegion'];
        $_SESSION['filterRegion'] = $filterRegion;
    } elseif (isset($_SESSION['filterRegion'])) {
        $filterRegion = $_SESSION['filterRegion'];
    } else {
        $filterRegion = 0;
        $_SESSION['filterRegion'] = 0;
    }
    if (isset($_REQUEST['filterSchool'])) {
        $filterSchool = $_REQUEST['filterSchool'];
        $_SESSION['filterSchool'] = $filterSchool;
    } elseif (isset($_SESSION['filterSchool'])) {
        $filterSchool = $_SESSION['filterSchool'];
    } else {
        $filterSchool = '';
        $_SESSION['School'] = '';
    }
    if (isset($_REQUEST['filterStreet'])) {
        $filterStreet = $_REQUEST['filterStreet'];
        $_SESSION['filterStreet'] = $filterStreet;
    } elseif (isset($_SESSION['filterStreet'])) {
        $filterStreet = $_SESSION['filterStreet'];
    } else {
        $filterStreet = '';
        $_SESSION['Street'] = '';
    }

    if (isset($_REQUEST['btnClearFilter'])) {
        $filterCountry = 0;
        $_SESSION['filterCountry'] = 0;
        $filterRegion = 0;
        $_SESSION['filterRegion'] = 0;
        $filterSchool = '';
        $_SESSION['School'] = '';
        $filterStreet = '';
        $_SESSION['Street'] = '';
    }

// Выполнение команд

    switch ($cmd) {
        case 'doDelete':
        {    $MergeIds = trim($_POST['MergeIds']);
            if ($MergeIds) {
                $mysqli->query('delete from olimp_schools where (confirmed="N") AND (id in (' . $MergeIds . '))');
            }
            $cmd = '';
            break;
        }
        case 'Merge':
        {
            $toMerge = $_POST['toMerge'];
            $MergeId = $_POST['MergeId'] + 0;
            $MergeAid = $_POST['MergeAid'] + 0;
            $w = trim($_POST['MergeForceDirector']);
            $MergeDirector = empty($w) ? trim($_POST['MergeDirector']) : $w;
            $w = trim($_POST['MergeForceTel']);
            $MergeTel = empty($w) ? trim($_POST['MergeTel']) : $w;
            $w = trim($_POST['MergeForceMail']);
            $MergeMail = empty($w) ? trim($_POST['MergeMail']) : $w;
            $MergeType = $_POST['MergeType'] + 0;
            $MergeName = trim($_POST['MergeName']);

            $AddressZip = trim($_POST['AddressZip']);
            $AddressRegion = $_POST['AddressRegion'] + 0;
            $AddressArea = trim($_POST['AddressArea']);
            $AddressCity = trim($_POST['AddressCity']);
            $AddressStreet = trim($_POST['AddressStreet']);
            $AddressHouse = trim($_POST['AddressHouse']);
            $AddressBuilding = trim($_POST['AddressBuilding']);

            $ConfirmSchool = $_POST['ConfirmSchool'] + 0;

            if ($MergeId > 0) {
                $MergeIds = '';
                $MergeCount = 0;
                foreach ($toMerge as $w) {
                    if ($w <> $MergeId) {
                                $MergeIds .= ($MergeIds ? ',' : '') . $w;
                                    $MergeCount++;
                    }
                }
                if ($MergeCount > 0) {
                    $mysqli->query('update olimp_persons set school=' . $MergeId . ' where school in (' . $MergeIds . ')');
                    $mysqli->query('delete from olimp_schools where id in (' . $MergeIds . ')');
    //echo 'update olimp_persons set school='.$MergeId.' where school in ('.$MergeIds.')<br><br>';
    //echo 'delete from olimp_schools where id in ('.$MergeIds.')<br><br>';
                }

                $update = '';
                if ($MergeAid) {
                    $update = 'address=' . $MergeAid;
                }
                if ($MergeDirector) {
                    $update .= ($update ? ', ' : '') . 'director="' . $MergeDirector . '"';
                }
                if ($MergeTel) {
                    $update .= ($update ? ' , ' : '') . 'tel="' . $MergeTel . '"';
                }
                if ($MergeMail) {
                    $update .= ($update ? ' , ' : '') . 'email="' . $MergeMail . '"';
                }
                if ($MergeType) {
                    $update .= ($update ? ', ' : '') . 'school_type=' . $MergeType . ', school_type_ex=""';
                }
                if ($MergeName) {
                    $update .= ($update ? ', ' : '') . 'name="' . $MergeName . '"';
                }
                if ($ConfirmSchool) {
                    $update .= ($update ? ', ' : '') . 'confirmed="Y"';
                }
                if ($update) {
                    $mysqli->query('update olimp_schools set ' . $update . ' where id=' . $MergeId);
                }

                if (empty($MergeAid)) {
                    $res = $mysqli->query('select address from olimp_schools where id=' . $MergeId);
                    $row = $res->fetch_row();
                    $MergeAid = $row[0];
                }

                $update = '';
                if ($AddressZip) {
                    $update = 'zip_code="' . $AddressZip . '"';
                }
                if ($AddressRegion) {
                    $update .= ($update ? ', ' : '') . 'region=' . $AddressRegion;
                }
                if ($AddressArea) {
                    $update .= ($update ? ', ' : '') . 'area="' . $AddressArea . '"';
                }
                if ($AddressCity) {
                    $update .= ($update ? ', ' : '') . 'city="' . $AddressCity . '"';
                }
                if ($AddressStreet) {
                    $update .= ($update ? ', ' : '') . 'street="' . $AddressStreet . '"';
                }
                if ($AddressHouse) {
                    $update .= ($update ? ', ' : '') . 'house="' . $AddressHouse . '"';
                }
                if ($AddressBuilding) {
                    $update .= ($update ? ', ' : '') . 'building="' . $AddressBuilding . '"';
                }
                if ($update) {
                    $mysqli->query('update olimp_address set ' . $update . ' where id=' . $MergeAid);
                }
    //echo 'update olimp_address set '.$update.' where id='.$MergeAid.'<br><br>';
            }
            $cmd = '';
            break;
        }
    }


// Формирование кода страницы

    switch ($cmd) {
        case 'Delete':
        {    $toMerge = $_POST['toMerge'];
            $MergeIds = '';
            $MergeCount = 0;
            foreach ($toMerge as $w) {
                $MergeIds .= ($MergeIds ? ',' : '') . $w;
                $MergeCount++;
            }
            if ($MergeCount > 0) {
                $list = '';

                $res = $mysqli->query(
                    'select
s.id,
IF(LENGTH(s.school_type_ex)>0, concat_ws(" ",IFNULL(st.name,""),"*",s.school_type_ex,"*"), IFNULL(st.name,"")) as 
school_type,s.name as name, concat_ws(" ",a.zip_code,r.name,a.area,a.city,a.street,a.house,a.building) as address
from olimp_schools as s 
left join olimp_school_types as st on (st.id=s.school_type)
left join olimp_address as a on (a.id=s.address)
left join kladr_regions as r on (r.id=a.region) where (s.id in (' . $MergeIds .
                    ')) AND (s.confirmed="N") order by a.region,a.street,school_type,s.name'
                );
                $list = '';
                while ($row = $res->fetch_assoc()) {
                    $list .= str_replace(
                        array('%id%','%type%','%name%','%address%'),
                        array($row['id'],$row['school_type'],$row['name'],$row['address']),
                        $tpl['cleanup_schools']['delete_line']
                    );
                }
                $body = str_replace(array('%list%','%MergeIds%'), array($list,$MergeIds), $tpl['cleanup_schools']['delete']);
                break;
            }
        }
        default:
        {    // Форма фильтра школы

            $countrys = '<option value="0"></option>';
            $res2 = $mysqli->query('select id,name from olimp_countrys');
            while ($row2 = $res2->fetch_row()) {
                $countrys .= '<option' . ($filterCountry == $row2[0] ? ' selected' : '') . ' value="' . $row2[0] . '">' . $row2[1] . '</option>';
            }

            $regions = '<option value="0"></option>';
            $res2 = $mysqli->query('select id,name,socr,ord from kladr_regions order by ord desc, name');
            while ($row2 = $res2->fetch_row()) {
                $regions .= '<option' . ($filterRegion == $row2[0] ? ' selected' : '') . ' value="' . $row2[0] . '">' . $row2[2] . ' ' . $row2[1] . '</option>';
            }

            $school_types = '<option value="0"></option>';
            $res2 = $mysqli->query('select id,name from olimp_school_types');
            while ($row2 = $res2->fetch_row()) {
                $school_types .= '<option value="' . $row2[0] . '">' . $row2[1] . '</option>';
            }


            if (!empty($filterSchool) or !empty($filterStreet) or ($filterRegion > 0)) {
                $where = '';
                if ($filterCountry > 0) {
                    $where = '(a.country=' . $filterCountry . ')';
                }
                if ($filterRegion > 0) {
                    $where .= ($where ? ' AND ' : '') . '(a.region=' . $filterRegion . ')';
                }
                if (!empty($filterSchool)) {
                    $where .= ($where ? ' AND ' : '') . '(s.name like "%' . $filterSchool . '%")';
                }
                if (!empty($filterStreet)) {
                    $where .= ($where ? ' AND ' : '') . '(a.street like "%' . $filterStreet . '%")';
                }

                $res = $mysqli->query(
                    'select
s.id,
IF(LENGTH(s.school_type_ex)>0, concat_ws(" ",IFNULL(st.name,""),"*",s.school_type_ex,"*"), IFNULL(st.name,"")) as 
school_type,s.name as name,s.director,s.tel,s.email, concat_ws(" ",a.zip_code,r.name,a.area,a.city,a.street,a.house,a.building) as address, 
s.address as aid, s.confirmed as confirmed
from olimp_schools as s 
left join olimp_school_types as st on (st.id=s.school_type)
left join olimp_address as a on (a.id=s.address)
left join kladr_regions as r on (r.id=a.region) where ' . $where .
                    'order by a.region,a.street,school_type,s.name'
                );

                $list = '';
                while ($row = $res->fetch_assoc()) {
                    $list .= str_replace(
                        array('%id%','%type%','%name%','%director%','%tel%','%email%','%address%','%aid%','%bkg%'),
                        array($row['id'],$row['school_type'],$row['name'],$row['director'],$row['tel'],$row['email'],$row['address'],$row['aid'], ($row['confirmed'] == 'Y' ? ' style="background:#E0FFE0"' : '') ),
                        $tpl['cleanup_schools']['line']
                    );
                }
            } else {
                $list = '';
            }

            $body = str_replace(
                array('%countrys%','%regions%','%filterSchool%','%filterStreet%','%list%','%school_types%'),
                array($countrys,$regions,$filterSchool,$filterStreet,$list,$school_types),
                $tpl['cleanup_schools']['list']
            );

        }
    }

    return str_replace('%error%', $error, $body);
}

$tpl['cleanup_schools']['list'] =
'<h1>Фильтр</h1>%error%
<form action="?" method=post>
<table cellspacing=4 border=0>
<tr><td>Страна:</td><td><select name="filterCountry" size=0>%countrys%</select></td></tr>
<tr><td>Регион:</td><td><select name="filterRegion" size=0>%regions%</select></td></tr>
<tr><td>Часть названия:</td><td><input type="text" name="filterSchool" size=64 value="%filterSchool%"></td></tr>
<tr><td>Улица:</td><td><input type="text" name="filterStreet" size=64 value="%filterStreet%"></td></tr>
</table>
<div style="text-align:center"><input type=submit name="btnFilter" value="  Применить фильтр  ">&nbsp;&nbsp;<input type=submit name="btnClearFilter" value=" Очистить фильтр  "></div>
<style>
.schools td
{  border-right:1px dotted gray;
   border-bottom:1px dotted gray;
}
table.schools
{  border:1px solid gray;
margin:20px 0px;
}
table.schools .title
{  background: #333399;
color:white;
font-weight:bold;
}
table.schools .title td
{    color:white;
}
</style>
<table cellspacing=0 cellpadding=4 width=100% border=0 class="schools">
<tr class="title">
<td>*</td>
<td colspan=2>Наименование образовательного учреждения</td>
<td colspan=2>Адрес</td>
<td colspan=2>Директор</td>
<td colspan=2>Телефон</td>
<td colspan=2>Почта</td>
</tr>
%list%
<tr><td colspan=11 style="background:#F0F0FF; font-weight:bold;">Заменить данные на следующие:</td></tr>
<tr>
<td>&nbsp;</td>
<td colspan=2><select name="MergeType" style="width:100%">%school_types%</select><input type="text" name="MergeName"  style="width:100%"></td>
<td colspan=2>
<input type="text" name="AddressZip" size=10> индекс<br>
<select name="AddressRegion" size=0>%regions%</select><br>
<input type="text" name="AddressArea" size=30> р-н<br>
<input type="text" name="AddressCity" size=30> гор<br>
<input type="text" name="AddressStreet" size=30> ул.<br>
д. <input type="text" name="AddressHouse" size=8> стр. <input type="text" name="AddressBuilding" size=8>
</td>
<td><input type="text" name="MergeForceDirector" style="width:100%"></td>
<td>&nbsp;</td>
<td><input type="text" name="MergeForceTel" style="width:100%"></td>
<td>&nbsp;</td>
<td><input type="text" name="MergeForceMail" style="width:100%"></td>
<td>&nbsp;</td></tr>
</table>
<br>
<input type="checkbox" name="ConfirmSchool" value=1 id="ConfirmSchool"><label for="ConfirmSchool"> Пометить школу как подтвержденную</label><br><br>
<div style="text-align:center"><input type=submit name="btnMerge" value="  Объединить школы  ">&nbsp;&nbsp;&nbsp;<input type=submit name="btnDelete" value="  Удалить выбранные  "></div>
</form>
<br><br><br><br>
';


$tpl['cleanup_schools']['line'] =
'
<tr%bkg%>
<td><input type="checkbox" name="toMerge[%id%]" value="%id%" id="toMerge%id%"></td>
<td><label for="toMerge%id%"><span style="font-weight:bold">%type%</span> %name% (%id%)</label></td><td><input type="radio" name="MergeId" value="%id%"></td>
<td>%address%</td><td><input type="radio" name="MergeAid" value="%aid%"></td>
<td>%director%</td><td><input type="radio" name="MergeDirector" value="%director%"></td>
<td>%tel%</td><td><input type="radio" name="MergeTel" value="%tel%"></td>
<td>%email%</td><td><input type="radio" name="MergeMail" value="%email%"></td>
</tr>
';


$tpl['cleanup_schools']['delete'] =
'<h1>Подтвердите удаление</h1>%error%
<form action="?" method=post>
<input type="hidden" name="cmd" value="doDelete">
<input type="hidden" name="MergeIds" value="%MergeIds%">
<style>
.schools td
{  border-right:1px dotted gray;
   border-bottom:1px dotted gray;
}
table.schools
{  border:1px solid gray;
margin:20px 0px;
}
table.schools .title
{  background: #333399;
color:white;
font-weight:bold;
}
table.schools .title td
{    color:white;
}
</style>
<table cellspacing=0 cellpadding=4 width=100% border=0 class="schools">
<tr class="title">
<td colspan=2>Наименование образовательного учреждения</td>
<td colspan=2>Адрес</td>
</tr>
%list%
</table>
<br>
<div style="text-align:center"><input type=submit name="btnDoDelete" value="  Удалить  ">&nbsp;&nbsp;<input type=submit name="btnCancel" value="  Отменить  "></div>
</form>
<br><br><br><br>
';

$tpl['cleanup_schools']['delete_line'] =
'<tr><td>%type% %name%</td><td>%address%</td></tr>
';

$body = build_page();
