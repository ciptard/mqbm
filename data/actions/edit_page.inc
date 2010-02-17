<?php
if (is_admin()) {
        if (!isset($_v['text']) || !isset($_v['pg'])) {
                $post['text']=htmlspecialchars(@file_get_contents($_s['pages_dir'].'/'.trim($_v['pg'])));
                $_v['action']='add_page';
                $post['title']=translit($_v['pg'],1);
                $_v['p']=$_v['pg'];

                hook('MQ_POST_FORM_SHOW_BEFORE');
				include_once $_s['tpl_dir'].'/'.$tpl['post_form'];
                hook('MQ_POST_FORM_SHOW_AFTER');
        }
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>