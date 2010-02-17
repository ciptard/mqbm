<?php
$plugin_name = 'PSearch';
$plugin_version = '0.5';
$plugin_description = $_l['psrch_desc'];
$plugin_author = 'zorg';

register_hook('MQ_POSTS_GET_BEFORE', 'search');
?>