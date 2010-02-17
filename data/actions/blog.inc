<? 
        if(!isset($_s['posts_get_standart'])) $_s['posts_get_standart'] = 1;
        if(!isset($posts)) $posts = array();

        if(isset($_v['list'])) $_s['ppp']=$_s['list_ppp'];
        if(trim($_s['index']) && !trim($_SERVER['QUERY_STRING'])) page($_s['index']);
        else {

        hook('MQ_POSTS_GET_BEFORE');
        if(!$posts && $_s['posts_get_standart']==1) $posts=get_posts();
        hook('MQ_POSTS_GET_AFTER');

		if(isset($posts)) {
        @rsort($posts);
		$_s['tposts']=sizeof($posts);

        $pages = ceil(sizeof($posts) / $_s['ppp']);
        $skip = (isset($_v['skip']) && is_numeric($_v['skip']) ? $_v['skip'] : 0);


        if (sizeof($posts) > $_s['ppp']) $posts = array_slice($posts, $skip, $_s['ppp']);
        else if(isset($pages)) $pages = 0;

        $last_day = 0;

        for ($i = 0; $i<sizeof($posts); $i++) {
                if($post = post_info($posts[$i])) {
                $post_id = $post['ida'] + $_s['time_offset'];
                if ($last_day != date("d", $post_id)) {
                        $last_day = date("d", $post_id);
                        $month = date("n", $post_id);
                        $date = $last_day.' '.$month;
                        include $_s['tpl_dir'].'/'.$tpl['day'];
                }

				if(!$post['title']) $post['title']=$_l['notitle'];

                $post['text'] = posttext($post['text']);

                if(preg_match("#(.*)<cut text=\"(.*)\">#sU", $post['text'], $cut) or preg_match("#(.*)<cut>#sU", $post['text'], $cut))
                {
                        $post['text'] = str_replace('</cut>', '', $post['text']);
                        if(isset($cut[2])) {
                        $post['text'] = $cut[1].'<a href="'.generate_link($post['id']).'">'.$cut[2].'</a>';
                        } else {
                        $post['text'] = $cut[1].'<a href="'.generate_link($post['id']).'">'.$_l['more'].'</a>';
                        }
                }
                post_menu();

                hook('MQ_POST_SHOW_BEFORE');
                if(isset($_v['list'])) include $_s['tpl_dir'].'/'.$tpl['post_list'];
                else include $_s['tpl_dir'].'/'.$tpl['post'];
    	 }
    	}
        $_s['pages_link']='';

        if ($pages > 1) {
                $j=1;
                if(isset($_v['list'])) $_s['pt']='&list';
                else if(!isset($_s['pt'])) $_s['pt']='';
                if($_s['ppp_nav_type']==1) {
                while($j <= $pages) {
                        $pn = $j + 1;
                        if(($j*$_s['ppp'])<$_s['tposts']) {
                        if ($j*$_s['ppp']==$skip) $_s['pages_link'] .= '<a href="'.skip_link($j*$_s['ppp']).'"><font size="+2">'.$pn.'</font></a> ';
                        else $_s['pages_link'] .= '<a href="'.skip_link($j*$_s['ppp']).'">'.$pn.'</a> ';
                        }
                        $j++;
                }
                if($skip) $_s['pages_link'] = '<a href="'.$_SERVER['PHP_SELF'].'?'.$_s['pt'].'">1</a> '.$_s['pages_link'];
                }
                if($_s['ppp_nav_type']==2) {
                $pr = $nx = '';
                if($skip) $pr='<a href="'.skip_link($skip-$_s['ppp']).'">'.$_l['ppp_link_prev'].'</a>';
                if(($skip+$_s['ppp'])<$_s['tposts']) $nx='<a href="'.skip_link($skip+$_s['ppp']).'">'.$_l['ppp_link_next'].'</a>';
                $_s['pages_link'] = $pr.' '.$nx;
                }
        }

    }
  }
?>
