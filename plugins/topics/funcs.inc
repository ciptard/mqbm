<?php
function topics($tctp=FALSE) {
	global $_v, $post, $_s, $tpl, $_l;
 	$e=explode(':',buffer('Topics','sett'));
	$_v['topics']=buffer('','topics');
	$_s['stat']='tpc';
	$t=explode("\n",$_v['topics']);
	if(isset($_v['list']) && !trim($e[1])) $e[0]=1;
	if((!trim($e[0]) && @$_s['posts_get_standart']==1) || !isset($_s['posts_get_standart']) || (isset($_v['list']) && trim($e[1])) || @$_s['topics']) {
	hook('MQ_BEFORE_ALL_TOPICS');
	$_s['stat']='tpc';
	for($i=0; $i<sizeof($t); $i++) {
		$post = post_info($t[$i]);
		if($post) {
		post_menu();
		$post['text'] = posttext($post['text']);
		hook('MQ_BEFORE_TOPIC');
		hook('MQ_POST_SHOW_BEFORE');
		if(!isset($tpl['post_topic'])) $tpl['post_topic']=$tpl['post'];
		if($tctp || trim($e[2])) include $_s['tpl_dir'].'/'.$tpl['post_list'];
		else include $_s['tpl_dir'].'/'.$tpl['post_topic'];
		}
	}
	}
	$_s['stat']='';
}

function tpcstat() {
	global $post, $_l, $_s, $_v;

	if(@$post['topic'] && @$_s['stat']!='tpc' && !isset($_v['p'])) $_s['denpost']=1;
}

function tpcmenu() {
	global $post, $_l;

	if(@$post['topic']) $post['menu']='<a href="?action=untopic_post&p='.$post['id'].'">'.$_l['topic_'].'</a>'.$post['menu'];
	else $post['menu']='<a href="?action=topic_post&amp;p='.$post['id'].'">'.$_l['topic'].'</a>'.$post['menu'];
}

function tpcsave() {
	global $post, $_s, $_v;

	$post['topic']=@$_v['topic'];
	if(@$_v['topic']) {
        $_v['topics']=buffer('','topics');
        if(!@ereg($post['id'],$_v['topics'])) {
	        $_v['topics'].=$post['id']."\n";
	        buffer('','topics');
        }
	} else {
	$_v['topics']=buffer('', 'topics');
		if (@eregi($post['id'],$_v['topics'])) {
			$_v['topics'] = preg_replace("/".$post['id']."\n|\n?".$post['id']."$/", '', $_v['topics']);
			buffer('', 'topics');
		}
	}
}

function tpcform() {
	global $post, $_s, $_l;

	$_s['tpcchk']='';
	$c='value="ON" checked="checked"';
	if(@trim($post['topic'])) $_s['tpcchk']=$c;
	$_s['dwnpostface']='<input type="checkbox" name="topic" '.$_s['tpcchk'].'>'.$_l['pform_topic'].'<br />';
}

?>