<?php
$plugin_name = 'Tags';
$plugin_version = '0.6.4';
$plugin_description = $_l['tag_desc'];
$plugin_author = 'Bolzamo & Zorg & muhas';

register_hook('MQ_POST_ADD_AFTER', 'tagit');
register_hook('MQ_POST_SHOW_BEFORE', 'get_post_tags');
register_hook('MQ_POSTS_GET_BEFORE', 'tgposts');
register_hook('MQ_POST_FORM_SHOW_BEFORE', 'get_post_tags');
register_hook('MQ_POST_DELETE_BEFORE', 'del_post_tags');

$actions = array(
        'tgshelp'        => $_l['tag_help'],
        'tgssett'        => $_l['tag_sett']
);
?>
