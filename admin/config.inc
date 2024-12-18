<?php

// Дополнительные установки для редактора

// language 
SpawConfig::setStaticConfigItem('default_lang','ru');
// output charset (empty strings means charset specified in language file)
SpawConfig::setStaticConfigItem('default_output_charset','cp1251');
// theme 
SpawConfig::setStaticConfigItem('default_theme','spaw2');
// toolbarset 
SpawConfig::setStaticConfigItem('default_toolbarset','standard');
// stylesheet
SpawConfig::setStaticConfigItem('default_stylesheet','/css/ie.css');
// width 
SpawConfig::setStaticConfigItem('default_width','100%');
// height 
SpawConfig::setStaticConfigItem('default_height','200px');

// specifies in which directions resizing is allowed (values: none, horizontal, vertical, both)
SpawConfig::setStaticConfigItem('resizing_directions', 'vertical', SPAW_CFG_TRANSFER_JS);
// specifies that special characters should be converted to the respective html entities
SpawConfig::setStaticConfigItem('convert_html_entities', false, SPAW_CFG_TRANSFER_JS);

// data for style (css class) dropdown list
SpawConfig::setStaticConfigItem("dropdown_data_core_style",
  array(
    '' => 'Normal',
    'blue' => 'Синий текст',
    'menu11' => 'Пункт меню',
    'menu11с' => 'Пункт меню (текущий)'
  )
);
// data for style (css class) dropdown in table properties dialog
SpawConfig::setStaticConfigItem("table_styles",
  array(
    '' => 'Normal',
    'blue' => 'Синий текст',
    'menu11' => 'Пункт меню',
    'menu11с' => 'Пункт меню (текущий)'
  )
);
// data for style (css class) dropdown in table cell properties dialog
SpawConfig::setStaticConfigItem("table_cell_styles",
  array(
    '' => 'Normal',
    'blue' => 'Синий текст',
    'menu11' => 'Пункт меню',
    'menu11с' => 'Пункт меню (текущий)'
  )
);
// data for fonts dropdown list
SpawConfig::setStaticConfigItem("dropdown_data_core_fontname",
  array(
    'Arial' => 'Arial',
    'Courier' => 'Courier',
    'Tahoma' => 'Tahoma',
    'Times New Roman' => 'Times',
    'Verdana' => 'Verdana'
  )
);
// data for fontsize dropdown list
SpawConfig::setStaticConfigItem("dropdown_data_core_fontsize",
  array(
    '1' => '1',
    '2' => '2',
    '3' => '3',
    '4' => '4',
    '5' => '5',
    '6' => '6'
  )
);
// data for paragraph dropdown list
SpawConfig::setStaticConfigItem("dropdown_data_core_formatBlock",
  array(
    'Normal' => 'Normal',
    '<H1>' => 'Заголовок 1',
    '<H2>' => 'Заголовок 2',
    '<H3>' => 'Заголовок 3',
  )
);
// data for link targets drodown list in hyperlink dialog
SpawConfig::setStaticConfigItem("a_targets",
  array(
    '_self' => 'Self',
    '_blank' => 'Blank',
    '_top' => 'Top',
    '_parent' => 'Parent'
  )
);


// toolbar sets (should start with "toolbarset_"
// standard core toolbars
SpawConfig::setStaticConfigItem('toolbarset_standard',
  array(
    "format" => "format",
    "style" => "style",
    "edit" => "edit",
    "table" => "table",
    "plugins" => "plugins",
    "insert" => "insert",
    "tools" => "tools"
  ) 
);
// all core toolbars
SpawConfig::setStaticConfigItem('toolbarset_all',
  array(
    "format" => "format",
    "style" => "style",
    "edit" => "edit",
    "table" => "table",
    "plugins" => "plugins",
    "insert" => "insert",
    "tools" => "tools",
    "font" => "font"   
  ) 
);
// mini core toolbars
SpawConfig::setStaticConfigItem('toolbarset_mini',
  array(
    "format" => "format_mini",
    "edit" => "edit",
    "tools" => "tools"
  ) 
);

// colorpicker config
SpawConfig::setStaticConfigItem('colorpicker_predefined_colors',
  array(
    'black',
    'silver',
    'gray',
    'white',
    'maroon',
    'red',
    'purple',
    'fuchsia',
    'green',
    'lime',
    'olive',
    'yellow',
    'navy',
    'blue',
    '#fedcba',
    'aqua'
  ),
  SPAW_CFG_TRANSFER_SECURE
);

// SpawFm plugin config:

// global filemanager settings
SpawConfig::setStaticConfigItem(
  'PG_SPAWFM_SETTINGS',
  array(
    'allowed_filetypes'   => array('any'),  // allowed filetypes groups/extensions
    'allow_modify'        => true,         // allow edit filenames/delete files in directory
    'allow_upload'        => true,         // allow uploading new files in directory
    'chmod_to'            => 0777,          // change the permissions of an uploaded file if allowed
                                            // (see PHP chmod() function description for details), or comment out to leave default
    'max_upload_filesize' => 0,             // max upload file size allowed in bytes, or 0 to ignore
    'max_img_width'       => 0,             // max uploaded image width allowed, or 0 to ignore
    'max_img_height'      => 0,             // max uploaded image height allowed, or 0 to ignore
    'recursive'           => true,         // allow using subdirectories
    'allow_modify_subdirectories' => true, // allow renaming/deleting subdirectories
    'allow_create_subdirectories' => true, // allow creating subdirectories
    'forbid_extensions'   => array('php','inc','js'),  // disallow uploading files with specified extensions
    'forbid_extensions_strict' => true,     // disallow specified extensions in the middle of the filename
  ),
  SPAW_CFG_TRANSFER_SECURE
);

// directories
SpawConfig::setStaticConfigItem(
  'PG_SPAWFM_DIRECTORIES',
  array(
    array(
      'dir'     => 'img/gallery',
      'caption' => 'Галерея',
      'params'  => array(
      'allowed_filetypes' => array('images','flash','swf')
      )
    ),
    array(
      'dir'     => 'img/examples',
      'caption' => 'Примеры заданий', 
      'params'  => array(
      'allowed_filetypes' => array('images','flash','swf','any','pdf','ppt','xls')
      )
    ),
    array(
      'dir'     => 'docs',
      'caption' => 'Документы', 
      'params'  => array(
      'allowed_filetypes' => array('images','flash','swf','any','pdf','ppt','xls','rar','zip')
      )
    ),
    array(
      'dir'     => 'css',
      'caption' => 'Стили', 
      'params'  => array(
      'allowed_filetypes' => array('any','css')
      )
    ),
    array(
      'dir'     => 'img/content',
      'caption' => 'Картинки содержания страниц', 
      'params'  => array(
      'allowed_filetypes' => array('any','flash','swf')
      )
    ),
  ),
  SPAW_CFG_TRANSFER_SECURE
);

?>