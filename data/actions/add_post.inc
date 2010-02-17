<?php
        if (is_admin()) {
 		$post = array();
        chekers();
        if (!isset($_v['title']) || !isset($_v['text'])) {
                $post['title'] = $post['text'] = $p = '';
                back_my_time_edit_title();
                hook('MQ_POST_FORM_SHOW_BEFORE');
                include $_s['tpl_dir'].'/'.$tpl['post_form'];
                hook('MQ_POST_FORM_SHOW_AFTER');
        } elseif (!@trim($_v['title']) && !@trim($_v['text'])) {
        		e($_l['post_notxt']);
                redirect(act_link('add_post'));
        } else {
                $p = time();
        		$_v['p']=$post['id']=genid();
        		back_my_time();
                hook('MQ_POST_ADD_BEFORE');
                if (isset($_s['upload_dir'])) $_v['text'] = preg_replace('/\[\[(.*)\]\]/U', $_s['upload_dir'].'\\1', $_v['text']);
                fsave($_s['ids_dir'].'/'.trim($_v['p']),'w+','<?php $post[\'ida\']=\''.$p.'\'; ?>');
				post_sets();
                save_post($p);
                fsave($_s['comment_dir'].'/'.trim($_v['p']),"w+",'');
                upload_files();
                hook('MQ_POST_ADD_AFTER');

                redirect(generate_link(trim($_v['p'])));
        }
        } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
        }
?>
