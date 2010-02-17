<?php
if (is_admin() && isset($_v['pg']) && trim($_v['pg'])) {
        if(file_exists($_s['pages_dir'].'/'.$_v['pg'])) {
        hook('MQ_PAGE_DELETE_BEFORE');
        @unlink($_s['pages_dir'].'/'.$_v['pg']);
        hook('MQ_PAGE_DELETE_AFTER');
        } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
        }
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>