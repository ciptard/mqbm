<?php
$plugin_name = 'mscat';
$plugin_version = '0.2.1';
$plugin_description = $_l['mscat_desc'];
$plugin_author = '<a href="http://muhas.ru">muhas</a>';

register_hook('MQ_TAGS_SHOW_BEFORE', 'mscat_simple');


$actions = array(
        'mscat_help'        => $_l['mscat_h'],
        'mscat_s'        => $_l['mscat_s']
);
?>
