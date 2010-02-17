<?php
if (is_admin()) {
		$post=post_info($_v['p']);
        hook('MQ_POST_LOCK_BEFORE');
        $post['close']='on';
        save_post($post['ida']);
        hook('MQ_POST_LOCK_AFTER');
        redirect(generate_link(trim($post['id'])));
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>