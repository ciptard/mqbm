<?php
if (is_admin()) {
$post=post_info($_v['p']);
        hook('MQ_POST_UNLOCK_BEFORE');
        $post['close']='';
        save_post($post['ida']);
        hook('MQ_POST_UNLOCK_AFTER');
        redirect(generate_link(trim($post['id'])));
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>