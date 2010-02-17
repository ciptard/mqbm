<?php
if (is_admin()) {
		$post=post_info($_v['p']);
        hook('MQ_POST_UNTOPIC_BEFORE');
        $post['topic']='';
        $_v['topics']=buffer('','topics');
		if (eregi($post['id'],$_v['topics'])) {
			$_v['topics'] = preg_replace("/".$post['id']."\n|\n?".$post['id']."$/", '', $_v['topics']);
			buffer('', 'topics');
		}
        save_post($post['ida']);
        hook('MQ_POST_UNTOPIC_AFTER');
        redirect(generate_link(trim($post['id'])));
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>