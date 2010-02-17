<?php
if (is_admin()) {
		$post=post_info($_v['p']);
        hook('MQ_POST_TOPIC_BEFORE');
        $post['topic']='on';
        $_v['topics']=buffer('','topics');
        if(!ereg($_v['p'],$_v['topics'])) {
        $_v['topics']=$_v['topics'].$_v['p']."\n";
        buffer('','topics');
        }
        save_post($post['ida']);
        hook('MQ_POST_TOPIC_AFTER');
        redirect(generate_link(trim($post['id'])));
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>