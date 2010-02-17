<?php
if (is_admin() && isset($_v['p']) && isset($_v['c'])) {
        $cmnts = file($_s['comment_dir'].'/'.$_v['p']);
        $c = explode('¦¦', $cmnts[$_v['c']]);

        hook('MQ_COMMENT_DELETE_BEFORE');
		// удаляем из последних комментариев
        $lc = file($_s['data_dir'].'/lcomments');
        for ($i=0; $i<sizeof($lc); $i++) {
        $cline = explode('¦¦', $lc[$i]);
           $cline[1]=str_replace("\n",'',$cline[1]);
           if ($cline[0]==$_v['p']) {
              unset($lc[$i]);
           }
           if($cline[0]==$_v['p'] && $cline[1]!=$_v['c'] && $cline[1]>$_v['c']) {           	  $lc[$i]=$_v['p'].'¦¦'.($cline[1]-1)."\n";
           }
        }
        fsave($_s['data_dir'].'/lcomments','w+',implode('',$lc));
		//
        @unlink($_s['attach_dir'].'/'.str_replace("\n",'',$c[10]));
        unset($cmnts[$_v['c']]);
		fsave($_s['comment_dir'].'/'.$_v['p'],'w+',implode('', $cmnts));
        hook('MQ_COMMENT_DELETE_AFTER');

        redirect(generate_link($_v['p']).'#comment');
  } else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>
