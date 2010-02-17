<?php
if (is_admin()) {
    if(isset($_v['text']) && isset($_v['p'])) {

        $e = explode(':',$_v['p']);
        $cmnts = file($_s['comment_dir'].'/'.$e[0]);
        $c = explode('¦¦', $cmnts[$e[1]]);
        $c[5]=$_v['title'];
        $cmnts[$e[1]]=$c[0].'¦¦'.$c[1].'¦¦'.$c[2].'¦¦'.$c[3].'¦¦'.$c[4].'¦¦'.$c[5].'¦¦'.$c[6].'¦¦'.$c[7].'¦¦'.$c[8].'¦¦'.$_v['text'].'¦¦'.$c[10];
        hook('MQ_COMMENT_EDIT_BEFORE');
        fsave($_s['comment_dir'].'/'.$e[0],'w+',implode('', $cmnts));
        hook('MQ_COMMENT_EDIT_AFTER');

        redirect(generate_link($e[0]).'#comment-'.$e[1]);
    } else {
        $cmnts = file($_s['comment_dir'].'/'.$_v['p']);
        $c = explode('¦¦', $cmnts[$_v['c']]);
        $post['text']=$c[9];
        $post['title']=$c[5];
        $_v['p']=$_v['p'].':'.$_v['c'];

        hook('MQ_COMMENT_EDIT_FORM_BEFORE');
        include_once $_s['tpl_dir'].'/'.$tpl['post_form'];
        hook('MQ_COMMENT_EDIT_FORM_AFTER');
    }
} else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}

?>
