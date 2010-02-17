<?php
if (is_admin()) {
        $cmnts = file($_s['comment_dir'].'/'.$_v['p']);

        $c = explode('¦¦', $cmnts[$_v['c']]);
        $cmnts[$_v['c']]=$c[0].'¦¦0¦¦'.$c[2].'¦¦'.$c[3].'¦¦'.$c[4].'¦¦'.$c[5].'¦¦'.$c[6].'¦¦'.$c[7].'¦¦'.$c[8].'¦¦'.$c[9].'¦¦'.$c[10];

        hook('MQ_COMMENT_UNHIDE_BEFORE');
        fsave($_s['comment_dir'].'/'.$_v['p'],'w+',implode('', $cmnts));
        hook('MQ_COMMENT_UNHIDE_AFTER');

        redirect(generate_link($_v['p']).'#comment-'.$_v['c']);
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>