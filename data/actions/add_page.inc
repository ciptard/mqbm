<?php
if (is_admin()) {
        if (!isset($_v['title']) || !isset($_v['text'])) {
                $post['title'] = $post['text'] = $p = "";
                hook('MQ_POST_FORM_SHOW_BEFORE');
                include_once $_s['tpl_dir'].'/'.$tpl['post_form'];
                hook('MQ_POST_FORM_SHOW_AFTER');
        } elseif(isset($_v['title']) && !trim($_v['title'])) {
        	    e($_l['pagename']);
        		redirect(act_link('add_page'));
        } else {
                hook('MQ_PAGE_ADD_BEFORE');
                $tf=translit($_v['title']);
                if(@$tr!=$_v['p']) {		        hook('MQ_PAGE_DELETE_BEFORE');
		        @unlink($_s['pages_dir'].'/'.$_v['pg']);
		        hook('MQ_PAGE_DELETE_AFTER');
                }
                fsave($_s['pages_dir'].'/'.$tf,'w+',$_v['text']);
                hook('MQ_PAGE_ADD_AFTER');
                redirect(page_link($tf));
        }
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
  }
?>