<?php
if (is_admin()) {
		$post=post_info($_v['p']);
        hook('MQ_POST_UNHIDE_BEFORE');
        $post['hidden']='';
        save_post($post['ida']);
        hook('MQ_POST_UNHIDE_AFTER');
  	    redirect(generate_link(trim($post['id'])));
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>