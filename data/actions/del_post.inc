<?php
if (is_admin() && isset($_v['p']) && trim($_v['p'])) {
	$post=post_info($_v['p']);
	if($post) {
		$lc = file($_s['data_dir'].'/lcomments');
		foreach($lc as $k=>$v) {
			$cline = explode('¦¦', $v);
			if ($cline[0]==$_v['p']) {
				unset($lc[$k]);
				  
			}
		}
        fsave($_s['data_dir'].'/lcomments','w+',implode('',$lc));		
			
        hook('MQ_POST_DELETE_BEFORE');
		@unlink($_s['ids_dir'].'/'.$_v['p']);
        @unlink($_s['posts_dir'].'/'.$post['ida']);
        @unlink($_s['comment_dir'].'/'.$_v['p']);
        hook('MQ_POST_DELETE_AFTER');
        redirect($_SERVER['PHP_SELF']);
     } else {        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
    }
} else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>
