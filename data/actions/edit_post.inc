<?php
if (is_admin()) {
        if (!isset($_v['title']) || !isset($_v['text']) || !isset($_v['p'])) {
                $post = post_info($_v['p']);
                chekers();
                $post['title'] = htmlspecialchars($post['title']);
                $post['text'] = htmlspecialchars($post['text']);
                back_my_time_edit_title();
                hook('MQ_POST_FORM_SHOW_BEFORE');
                include_once $_s['tpl_dir'].'/'.$tpl['post_form'];
                hook('MQ_POST_FORM_SHOW_AFTER');
        } else {
                $p = time();
                $post = post_info($_v['p']);
                $_v['text']  = preg_replace('/\[\[(.*)\]\]/U', $_s['upload_dir'].'\\1', $_v['text']);
                back_my_time();
                hook('MQ_POST_EDIT_BEFORE');
				post_sets();
				fsave($_s['ids_dir'].'/'.trim($_v['p']),'w+','<?php $post[\'ida\']=\''.$post['ida'].'\'; ?>');
                save_post($post['ida']);
                tagit();
                hook('MQ_POST_EDIT_AFTER');

                upload_files();
                redirect(generate_link($_v['p']));
        }
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>