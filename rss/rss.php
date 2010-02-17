<?php
// RSS модуль
Error_Reporting(E_ALL & ~E_NOTICE);
header('Pragma: no-cache');
header('Content-type: application/xml');
define('INCLUDED', TRUE);
$mask="%02s-%02s-%04s, %02s:%02s";
$_s['include_blog']='../';

include_once '../data/functions.inc';
include_once $_s['pages_dir'].'/m.settings';
include_once $_s['lang_dir'].'/'.$_s['lang'].'.inc';
$_s['dateformat']='d-m-Y, H:i';
$_s['comment_dateformat']='d-m-Y, H:i';
if(file_exists($_s['plugin_dir'].'/symlink/funcs.inc')) include_once $_s['plugin_dir'].'/symlink/funcs.inc';

load_plugins();

$_s['rss_title']       = $_s['blogname'];
$_s['rss_link']        = $_s['base_url'];
$_s['rss_description'] = $_s['blogdescription'];
$_s['rss_language']    = 'ru';

$_s['rss_logo_file']   = $_s['base_url'].'/img/logosmall.png';
$_s['rss_logo_title']  = $_s['blogname'];
$_s['rss_logo_link']   = $_s['base_url'];
$_s['rss_ppp']         = 10;

if ($p) $_s['rss_ppp'] = $p;

if(isset($_REQUEST['short'])) $_s['short']=$_REQUEST['short'];
else $_s['short']=0;

hook('MQ_RSS_BEFORE_ALL');

e('<?xml version="1.0" encoding="'.$_s['encoding'].'"?>');
?>
<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/">
<channel>
<title><?php e($_s['rss_title']); ?></title>
<link><?php e($_s['rss_link']); ?></link>
<description></description>
<language><?php e($_s['rss_language']); ?></language>
<image>
<url><?php e($_s['rss_logo_file']); ?></url>
<title><?php e($_s['rss_logo_title']); ?></title>
<link><?php e($_s['rss_logo_link']); ?></link>
</image>
<?php
if (isset($_GET['id']) && file_exists($_s['comment_dir'].'/'.$_GET['id'])) {

		hook('MQ_RSS_BEFORE_COMMENTS_PROCESS');
		$_v['p'] = $_GET['id']; // для топиков и скрытых
        if($post = post_info($_GET['id'])) {
  		$comment = file($_s['comment_dir'].'/'.$_GET['id']);
        for ($i=0; $i < $post['c_count']; $i++) {
                $c = explode('¦¦', $comment[$i]);
                unset($cmnt);
                $cmnt['num']=$i;
				cmt_prepare();

		if(@$cmnt['priv']!=1) {
        $nick = $date = $text = $title = '';
  		$nick = htmlspecialchars($cmnt['nick'], ENT_QUOTES);
  				$date = $post['date'];
				list($day,$month,$year, $h, $m) = sscanf($date, $mask);
				$date=date("r", mktime($h, $m, 0, $month, $day, $year));
  		$text = htmlspecialchars($cmnt['text'], ENT_QUOTES);
  		$title = mb_substr($text,0,30,'UTF-8');
		hook('MQ_RSS_COMMENT_SHOW_BEFORE');
?>
<item>
<title><?php e($nick); ?>: <?php e($title); ?></title>
<link><?=generate_link($_GET['id']);?></link>
<description><?php e($text); ?></description>
<author><?php e($nick); ?></author>
<pubDate><?php e($date); ?></pubDate>
</item>
<?php
	hook('MQ_RSS_COMMENT_SHOW_AFTER');
	     }
      unset($c);
    }
  }//!post
}
else if(isset($_GET['last_comments']) && file_exists($_s['data_dir'].'/lcomments')) {
        $time_offset = $_s['time_offset'] * 3600;
        $lc = file($_s['data_dir'].'/lcomments');
        $sz=sizeof($lc);
        if($hs!='') {
        if($hs>sizeof($lc)) $sz=sizeof($lc);
        else $sz=$hs;
        }
		for ($i=0; $i<$sz; $i++) {
	        $cline = explode('¦¦', $lc[$i]);
	        $comment = file($_s['comment_dir'].'/'.$cline[0]);
	        $c = ''; $cmnt['num']=$cline[1]=str_replace("\n",'',$cline[1]);
	        $post['id'] = $cline[0];
	        $c = explode('¦¦', $comment[$cline[1]]);
            cmt_prepare();
            $nick = $date = $text = $title = '';
            $nick = htmlspecialchars($cmnt['nick'], ENT_QUOTES);
				$date = $post['date'];
				list($day,$month,$year, $h, $m) = sscanf($date, $mask);
				$date=date("r", mktime($h, $m, 0, $month, $day, $year));
            $text = htmlspecialchars($cmnt['text'], ENT_QUOTES);
            $title = mb_substr($text,0,30,'UTF-8');
	        if($cmnt['priv']!=1) {
?>
<item>
<title><?php e($nick); ?>: <?php e($title); ?></title>
<link><?php e(generate_link($post['id'])); ?></link>
<description><?php e($text); ?></description>
<author><?php e($nick); ?></author>
<pubDate><?php e($date); ?></pubDate>
</item>
<?php
		}
  }
}
else if(isset($_GET['tags']) && file_exists($_s['data_dir'].'/tags/cache/'.$_GET['tags']) && file_exists($_s['plugin_dir'].'/tags/funcs.inc')) {
       include_once $_s['plugin_dir'].'/tags/funcs.inc';
		if(!$pub) {
		//muhas
		$posts = getbytag($_GET['tags']);
		//muhas
        //$posts = get_posts();
        } else {
        $posts[0]=$pub;
        $_s['short']=0;
        }

        if (empty($posts)) exit();
        @rsort($posts);
        $skip = (isset($_v['skip']) && is_numeric($_v['skip']) ? $_v['skip'] : 0);
        if (sizeof($posts) > $_s['rss_ppp']) $posts = array_slice($posts, $skip, $_s['rss_ppp']);

        for ($i = 0; $i< sizeof($posts); $i++) {

                if($post = post_info($posts[$i])) {
                $post_id = $post['id'];

                hook('MQ_RSS_ENTRY_PROCESS_BEFORE');
				$postovoy = "/<div class=\"postovoy\"(.*?)<\/div>/is";
				$post['text'] =  preg_replace($postovoy, "", $post['text']);
				$post['text']=posttext($post['text']);

                $text = htmlspecialchars($post['text'], ENT_QUOTES);

                if($_s['short']==1) {

                if(preg_match("#(.*)<cut text=\"(.*)\">#sU", $post['text'], $cut) or preg_match("#(.*)<cut>#sU", $post['text'], $cut))
                {
                        $origtxt=$post['text'];
                        $post['text'] = str_replace("</cut>", "", $post['text']);
                        if(isset($cut[2])) {
                        $post['text'] = $cut[1].'<a href="'.generate_link($post['id']).'">'.$cut[2].'</a>';
                        } else {
                        $post['text'] = $cut[1].'<a href="'.generate_link($post['id']).'">'.$_l['more'].'</a>';
                                }
                }
                $text = htmlspecialchars($post['text'], ENT_QUOTES);
				echo preg_replace("/<small>(.*?)<\/small>/i", "", $text);
                }

                if ($_s['short']==2) $text = "";
                $title = htmlspecialchars($post['title'], ENT_QUOTES);
				$date = $post['date'];
				list($day,$month,$year, $h, $m) = sscanf($date, $mask);
				$date=date("r", mktime($h, $m, 0, $month, $day, $year));
			hook('MQ_RSS_ENTRY_SHOW_BEFORE');
?>
<item>
<title><?php e($title); ?></title>
<link><?php e(generate_link($post['id'])); ?></link>
<description><?php e($text);  ?></description>
<author><?php e($nick); ?></author>
<pubDate><?php e($date); ?></pubDate>
</item>
<?
		}
  }
}


else {
        if(!$pub) {
        $posts = get_posts();
        } else {
        $posts[0]=$pub;
        $_s['short']=0;
        }
        if (empty($posts)) exit();
        @rsort($posts);
        $skip = (isset($_v['skip']) && is_numeric($_v['skip']) ? $_v['skip'] : 0);
        if (sizeof($posts) > $_s['rss_ppp']) $posts = array_slice($posts, $skip, $_s['rss_ppp']);

        for ($i = 0; $i< sizeof($posts); $i++) {

                if($post = post_info($posts[$i])) {
                $post_id = $post['id'];

                hook('MQ_RSS_ENTRY_PROCESS_BEFORE');
				$postovoy = "/<div class=\"postovoy\"(.*?)<\/div>/is";
				$post['text'] =  preg_replace($postovoy, "", $post['text']);
                $post['text']=posttext($post['text']);
                $text = htmlspecialchars($post['text'], ENT_QUOTES);

                if($_s['short']==1) {

                if(preg_match("#(.*)<cut text=\"(.*)\">#sU", $post['text'], $cut) or preg_match("#(.*)<cut>#sU", $post['text'], $cut))
                {
                        $origtxt=$post['text'];
                        $post['text'] = str_replace("</cut>", "", $post['text']);
                        if(isset($cut[2])) {
                        $post['text'] = $cut[1].'<a href="'.generate_link($post['id']).'">'.$cut[2].'</a>';
                        } else {
                        $post['text'] = $cut[1].'<a href="'.generate_link($post['id']).'">'.$_l['more'].'</a>';
                                }
                }
                $text = htmlspecialchars($post['text'], ENT_QUOTES);
                }

                if ($_s['short']==2) $text = "";
                $title = htmlspecialchars($post['title'], ENT_QUOTES);
				$date = $post['date'];
				list($day,$month,$year, $h, $m) = sscanf($date, $mask);
				$date=date("r", mktime($h, $m, 0, $month, $day, $year));
	hook('MQ_RSS_ENTRY_SHOW_BEFORE');
?>
<item>

<title><?php e($title); ?></title>
<link><?php e(generate_link($post_id)); ?></link>
<description><?php e($text); ?></description>
<pubDate><?php e($date); ?></pubDate>
</item>
<?php
		hook('MQ_RSS_ENTRY_SHOW_AFTER');
				}
        }

}
?>
</channel>
</rss>
