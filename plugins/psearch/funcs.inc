<?php
function search() {
	global $_v, $_s, $_l, $posts;
	if(isset($_v['scword']) && trim($_v['scword'])) {
	$_s['posts_get_standart']=0;
    $psts=get_posts();
    $_s['ppp']=999999;
	for($i=0;$i<sizeof($psts);$i++) if(eregi($_v['scword'],file_get_contents($_s['posts_dir'].'/'.$psts[$i]))) $posts[]=$psts[$i];
	if(sizeof($posts)<1) e($_l['search_not_found']);
	}
}
?>
