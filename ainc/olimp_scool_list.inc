<?php

if (($role & 32) == 0) {
    return $tpl['noaccess'];
}
$body = '<table class="scool_list">
<thead>
    <tr>
        <td>
            Аббревиатура
        </td>
        <td>
            Название
        </td>
        <td>
            Адрес
        </td>
        <td>
            org_id
        </td>
        <td>
            
        </td>
    </tr>
</thead>
<tbody>';
$query = "select * from olimp_places order by name asc";
$res = $mysqli->query($query);
while ($row = $res->fetch_assoc()) {
    $body .= '
        <tr>
            <td>
                ' . $row['abbr'] . '
            </td>
            <td>
                ' . $row['name'] . '
            </td>
            <td>
                ' . $row['address'] . '
            </td>
            <td>
                ' . $row['org_id'] . '
            </td>
            <td>
                <nobr><div class="edit" onclick="location.href=\'olimp_scool_edit.html?id=' . $row['id'] . '\'"></div> <div class="del" onclick="if(confirm(\'Вы действительно желаете удалить школу?\')) location.href=\'olimp_scool_del.html?id=' . $row['id'] . '\'"></div></nobr>
            </td>
        </tr>
    ';
}

$body .= '
</tbody>
</table>
';
