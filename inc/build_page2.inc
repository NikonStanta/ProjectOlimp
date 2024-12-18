<?php

// Построение текстовых страниц

include_once('../inc/_text_page.inc');

function build_page($pgid, $subtype)
{
    global $SITE_VARS,$tpl,$_PAGE_VAR_VALUES;
    return text_page($pgid, $subtype, $SITE_VARS['Image dir'] . '/content');
}
