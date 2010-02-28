<?php
session_save_path($_s['include_blog'].'data/sess');
define('VERSION', 'Mosquito Bloody Mary 0.2');
// Директории
$_s['tpl_dir']=$_s['include_blog'].'templates';
$_s['lang_dir']=$_s['include_blog'].'lang';
$_s['data_dir']=$_s['include_blog'].'data';
$_s['attach_dir']=$_s['include_blog'].'upload/attach';
$_s['upload_dir']=$_s['include_blog'].'upload/post';
$_s['plugin_dir']=$_s['include_blog'].'plugins';
$_s['ids_dir']=$_s['data_dir'].'/ids';
$_s['act_dir']=$_s['data_dir'].'/actions';
$_s['posts_dir']=$_s['data_dir'].'/posts';
$_s['comment_dir']=$_s['data_dir'].'/comments';
$_s['blocks_dir']=$_s['data_dir'].'/blocks';
$_s['pages_dir']=$_s['blocks_dir'].'/pages';
// Шаблоны
$tpl['header']='head.html';
$tpl['day']='day.html';
$tpl['post']='post.html';
$tpl['post_topic']='post_topic.html';
$tpl['comment']='comment.html';
$tpl['comment_a']='comment_a.html';
$tpl['comment_last']='comment_last.html';
$tpl['comment_form']='comment_form.html';
$tpl['foother']='footer.html';
$tpl['post_list']='post_list.html';
$tpl['post_form']='post_form.html';
$tpl['login_form']='login_form.html';
// Системные страницы
$_s['admpage']='m.admin';

$self=$_SERVER['PHP_SELF'];
$_s['plugins'] = unserialize(file_get_contents($_s['data_dir'].'/plugins'));
$HOOKS = array();

// format functions
function prp($t) {
	return str_replace(array('\\','\''),array('\\\\','\\\''),$t);
}

function posttext($t) {
	$t = preg_replace_callback("#\<code\>(.*)\<\/code\>#is", "codepost", $t);
	$t = preg_replace_callback("#\<pre\>(.*)\<\/pre\>#is", "pre", $t);
	$t = str_replace("\n", "<br />\n", $t);
	$t = str_replace("\r", "\n", $t);
	$t = str_replace("tmpnbsp", "\n", $t);
	$t = @eregi_replace('<cut', '<cut', $t);
	$t = @eregi_replace('<lj-cut', '<cut', $t);
	return $t;
}

function pre($str) {
        $str[0] = str_replace(array("\r", "\n"), array('',"tmpnbsp"), $str[0]);
        return $str[0];
}
function codepost($str) {
        $str = str_replace(array('<', '>', '&lt;code&gt;', '&lt;/code&gt;' ), array('&lt;', '&gt;', '<pre><code>', '</code></pre>'), $str[0]);
        return $str;
}
function bbcode($t) {
	global $_s;
	$bk=explode(',',$_s['bb_comments']);
 	foreach ($bk as $b) { $b=trim($b); $t = str_replace(array('['.$b.']','[/'.$b.']'),array('<'.$b.'>','</'.$b.'>'), $t); }
 	$t = preg_replace('/(\[url\])(.+)(\[\/url\])/', '<a href="\\2">\\2</a>', $t);
 	return $t;
}

function translit($String, $detrans = FALSE) {
        global $_s;
        $String=strip_tags(trim($String));
		$String = str_replace(array('<','>','!','?','¦',"\t","\r","\n",'@','#','%','«','»','^','&','*','(',')','\\',',','№','~',':',';','[',']','=','"','\'','/'), '', $String);
        if($_s['lang']=='ru' && !strstr($String, 'm.') && $_s['translit']) {
        $Alpha=array('ё'=>'yo','й'=>'yi','ц'=>'c','у'=>'u','к'=>'k','е'=>'e','н'=>'n','г'=>'g','ш'=>'sh','щ'=>'she','з'=>'z','х'=>'x','ъ'=>'tz','ф'=>'f','ы'=>'y','в'=>'v','а'=>'a','п'=>'p','р'=>'r','о'=>'o','л'=>'l','д'=>'d','ж'=>'j','э'=>'e-','я'=>'ya','ч'=>'ch','с'=>'s','м'=>'m','и'=>'i','т'=>'t','ь'=>'mz','б'=>'b','ю'=>'ju','Ё'=>'Yo','Й'=>'YI','Ц'=>'C','У'=>'U','К'=>'K','Е'=>'E','Н'=>'N','Г'=>'G','Ш'=>'Sh','Щ'=>'She','З'=>'Z','Х'=>'X','Ъ'=>'Tz','Ф'=>'F','Ы'=>'Y','В'=>'V','А'=>'A','П'=>'P','Р'=>'R','О'=>'O','Л'=>'L','Д'=>'D','Ж'=>'J','Э'=>'E-','Я'=>'Ya','Ч'=>'Ch','С'=>'S','М'=>'M','И'=>'I','Т'=>'T','Ь'=>'Mz','Б'=>'B','Ю'=>'Ju',' '=>'_','\''=>'');
        $Beta=array('yo'=>'ё','yi'=>'й','c'=>'ц','u'=>'у','k'=>'к','e'=>'е','n'=>'н','g'=>'г','sh'=>'ш','she'=>'щ','z'=>'з','x'=>'х','tz'=>'ъ','f'=>'ф','y'=>'ы','v'=>'в','a'=>'а','p'=>'п','r'=>'р','o'=>'о','l'=>'л','d'=>'д','j'=>'ж','e-'=>'э','ya'=>'я','ch'=>'ч','s'=>'с','m'=>'м','i'=>'и','t'=>'т','mz'=>'ь','b'=>'б','ju'=>'ю','Yo'=>'Ё','YI'=>'Й','C'=>'Ц','U'=>'У','K'=>'К','E'=>'Е','N'=>'Н','G'=>'Г','Sh'=>'Ш','She'=>'Щ','Z'=>'З','X'=>'Х','TZ'=>'Ъ','F'=>'Ф','Y'=>'Ы','V'=>'В','A'=>'А','P'=>'П','R'=>'Р','O'=>'О','L'=>'Л','D'=>'Д','J'=>'Ж','E-'=>'Э','Ya'=>'Я','Ch'=>'Ч','S'=>'С','M'=>'М','I'=>'И','T'=>'Т','Mz'=>'Ь','B'=>'Б','Ju'=>'Ю','_'=>' ','\''=>'');
		if($detrans) $String=strtr($String, $Beta);
		else $String=strtr($String, $Alpha);
  		}

    return $String;
}

// plugins functions
function register_hook($hook_name, $function_name) {
        global $HOOKS;
        if ($hook_name and function_exists($function_name)) $HOOKS[$hook_name][] = $function_name;
}

function hook($hook_name) {
        global $HOOKS, $_s;
        if (!$_s['plugins_enable']) return 0;
        @rsort($posts);
        if (@sizeof($HOOKS[$hook_name])) foreach ($HOOKS[$hook_name] as $func) $func();
}

function load_plugins() {
        global $_s, $_l;
        if (!$_s['plugins_enable']) return 0;
        foreach ($_s['plugins'] as $plugin) {
                if(file_exists($lf=$_s['plugin_dir'].'/'.$plugin.'/lang_'.$_s['lang'].'.inc')) include_once $lf;
                else include_once $_s['plugin_dir'].'/'.$plugin.'/lang_def.inc';
                include_once $_s['plugin_dir'].'/'.$plugin.'/funcs.inc';
                include_once $_s['plugin_dir'].'/'.$plugin.'/info.inc';
        }
}

// get info functions
function blog() {
    global $_s, $_l, $_v, $_c, $tpl, $posts, $buffer, $post, $pages, $skip, $cmnt;	include($_s['act_dir'].'/blog.inc');
}

function get_posts() {
	global $_s;
	$d = dir($_s['posts_dir']);
	while (false !== ($entry = $d->read())) if (is_numeric($entry)) $posts[] = $entry;
	$d->close();
	if(isset($posts)) return $posts;
}

function post_info($p) {
    global $_s, $_l, $buffer, $post, $_v;
    $post = array();
    $post['ida']='';
    $_s['denpost']=0;
    if(@$buffer[$p]['load']) foreach ($buffer[$p] as $k => $v) $post[$k] = $v;
    $inc=0;
    if(trim($p) && file_exists($_s['posts_dir'].'/'.$p)) {
    	$inc=1;
    	include($_s['posts_dir'].'/'.$p);
    	$p=@$post['id'];
    }
    if (!file_exists($_s['ids_dir'].'/'.$p) || !trim($p)) return 0;
    include($_s['ids_dir'].'/'.$p);;
    if (!file_exists($_s['posts_dir'].'/'.$post['ida'])) return 0;
    if(!$inc && isset($post['ida']) && trim($post['ida'])) include($_s['posts_dir'].'/'.$post['ida']);
	$buffer[$p]['load'] = 1;
	$pida = $post['time'] = $post['ida'];
    $dta = $post['date'] = date($_s['dateformat'], $post['ida'] + $_s['time_offset'] * 3600);
    if (!file_exists($_s['comment_dir'].'/'.$post['id'])) $cnm = $post['c_count'] = 0;
    else $cnm = $post['c_count'] = sizeof(file($_s['comment_dir'].'/'.$post['id']));

	// не считаем скрытые комментарии
    if ($post['c_count'] && $_s['hiddencmt'] && !is_admin()) {
    $cnm = $post['c_count'] = 0;
    $fc=file($_s['comment_dir'].'/'.$post['id']);
    	for ($i=0; $i<sizeof($fc); $i++) {
    	$c=explode('¦¦',$fc[$i]);
    	if($c[1]!=1) $post['c_count']++;
    	}
    	$cnm = $post['c_count'];
    }

	// суицид
	if(@$post['dietime'] && time()>=$post['dietime']) {
        hook('MQ_POST_DELETE_BEFORE');
		if(file_exists($_s['tags_dir'].'/'.$post['id']))$tags=file_get_contents($_s['tags_dir'].'/'.$post['id']);
		@unlink($_s['tags_dir'].'/'.$post['id']);
		if(@$tags)cache_tags($tags);
        @unlink($_s['ids_dir'].'/'.$post['id']);
        @unlink($_s['posts_dir'].'/'.$post['ida']);
        @unlink($_s['comment_dir'].'/'.$post['id']);
        hook('MQ_POST_DELETE_AFTER');
        return 0;
	}
	// дата назначения
	if($post['holdtime'] && !is_admin()) {
	if(time()<=$post['ida']) return 0;
	else {
	$post['holdtime']='';
	save_post($post['ida']);
	$post['c_count'] = $cnm;
	$post['ida'] = $pida;
	$post['time'] = $pida;
	$post['date'] = $dta;
	}
	}
	if($post['close'] && !is_admin()) return 0;
	if(!isset($_v['p']) && $post['hidden'] && !is_admin()) return 0;
    hook('MQ_POST_INFO');
    if($_s['denpost']==1) return 0;
	foreach ($post as $k => $v) $buffer[$p][$k] = $v;	
    return $post;
}

function cmt_prepare() {      global $cmnt, $_s, $c, $_l, $post;
      $cmnt['nick'] = $c[0];
      $cmnt['priv'] = $c[1];
      $cmnt['aauch'] = $c[2];
      $cmnt['link'] = generate_link($post['id']).'#comment-'.$cmnt['num'];
      if(is_admin()) $cmnt['ip'] = str_replace('%ip%',$c[3],$_l['ip']);
      $cmnt['date'] = date($_s['comment_dateformat'], $c[8]+$_s['time_offset']);
      $cmnt['www']='';
      if(trim($c[5])) {
      	$c[5]=str_replace('www.','',$c[5]);
      	$c[5]=str_replace('http://','',$c[5]);
      	$cmnt['www'] = 'http://'.$c[5];
      }
      $cmnt['mail'] = $c[6];
      $cmnt['uid'] = $c[7];
      $cmnt['text'] = $c[9];
      @$cmnt['attach']=str_replace("\n",'',$c[10]);

      if(@trim($cmnt['attach'])) {
      if((@filesize($_s['attach_dir'].'/'.trim($c[10]))/1024)>1024) @$cmnt['attach_size']=round(((filesize($_s['attach_dir'].'/'.trim($c[10]))/1024)/1024),3).' Mb';
	  else @$cmnt['attach_size']=round((filesize($_s['attach_dir'].'/'.trim($c[10]))/1024)).' Kb';
}
}

function genid() {
	global $_s;
        $id=sizeof(get_posts())+1;
		while(file_exists($_s['ids_dir'].'/'.$id)) $id++;
		return $id;
}

function is_admin() {
        global $_s, $_c;
        hook('MQ_IS_ADMIN');
        if (isset($_c['sid']) && $_c['sid'] == md5($_s['pass'])) return 1;
        return 0;
}

function last_comments($hs) {        global $_s, $tpl, $cmnt, $c, $post, $_l;
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
	        $cmnt['link'] = generate_link($cline[0]).'#comment-'.$cline[1];
	        if($cmnt['priv']!=1 || is_admin()) include $_s['tpl_dir'].'/'.$tpl['comment_last'];
        }
}
//

// time set functions
function back_my_time_edit_title () {
        global $_v, $p, $post, $dt;
if (!isset($_v['p'])) {
        $dt['hou']=date('H');
        $dt['imp']=date('i');
        $dt['sec']=date('s');
        $dt['dau']=date('d');
        $dt['monh']=date('m');
        $dt['yar']=date('Y');
} else {
	    $bdt=$post['ida'];
        $dt['hou']=date('H',$bdt);
        $dt['imp']=date('i',$bdt);
        $dt['sec']=date('i',$bdt);
        $dt['dau']=date('d',$bdt);
        $dt['monh']=date('m',$bdt);
        $dt['yar']=date('Y',$bdt);
}
}

function back_my_time () {
		global $_v, $_s, $p, $post;
    	$dt=mktime($_v['hou'], $_v['minu'], $_v['secu'], $_v['mon'], $_v['da'], $_v['yark']);
    	if(@!$_v['dietime']) {
    	if (@!$_v['p']) $_v['p']=$id;
    	if(isset($post['ida']) && $post['ida']!=$dt) {    	@unlink($_s['posts_dir'].'/'.$post['ida']);
    	$post['ida']=$dt;
    	}
    	else if(isset($p)) $p=$dt;
    	}
}

// save/set functions
function save_post($ts) {	 global $_s, $_v, $post;
	 $wd='';
	 unset($post['tags']);
	 unset($post['load']);
	 unset($post['date']);
	 unset($post['c_count']);
	 unset($post['ida']);
	 unset($post['time']);
     hook('MQ_SAVE_POST');
     foreach ($post as $k => $v) $wd .= '$post[\''.$k.'\']=\''.prp($v).'\';'."\n";
	 if(isset($wd)) fsave($_s['posts_dir'].'/'.$ts,'w+',"<?php\n".$wd."?>");
}

function post_sets() {	 global $_v, $post, $_s;
	hook('MQ_POST_SETS');
	$post['id']=trim($_v['p']);
	$post['title']=$_v['title'];
	$post['text']=$_v['text'];
	$post['hidden']=@$_v['hidden'];
	$post['subs']=@$_v['subscrab'];
	$post['holdtime']=@$_v['holdtime'];
	if(@$_v['dietime']) $post['dietime']=mktime($_v['hou'], $_v['minu'], $_v['secu'], $_v['mon'], $_v['da'], $_v['yark']);
	else $post['dietime']='';
	$post['close']=@$_v['close'];
	$post['nocomm']=@$_v['closecomm'];
	$post['commhide']=@$_v['commhide'];
	$post['attach']=@$_v['attach'];
	$post['commpriv']=@$_v['commpriv'];
	$post['nobr']=@$_v['nobr'];
}

function pages_list($t) {
	global $_s;
	 $d = dir($_s['pages_dir']);
	 while (false !== ($entry = $d->read())) if ($entry != '.' && $entry != '..') $_pgs[] = $entry;
	 $d->close();
//убрать лишние страницы
       foreach ($_pgs as $e) { 
	   if ((!strstr($e, '.off') && !strstr($e, 'm.') && $e!=$_s['page404']  ) || (!$_s['nosyspages']) && is_admin()) {
	   $tpl=str_replace('%page%',translit($e,1),$t);
	   $tpl=str_replace('%link%',page_link($e),$tpl);
	   e($tpl);	   }
	}
}

function cecx($c,$v) {
	global $_s, $_c;
	if(trim($v)) setcookie($c, $v, time()+6592000);
	else if(isset($_c[$c])) return $_c[$c];
	else return '';
}

function fsave($fle,$mod,$txt) {
         $fh = fopen($fle, $mod);
         flock($fh, LOCK_EX);
         fwrite($fh, $txt);
         flock($fh, LOCK_UN);
         fclose($fh);
         @chmod($fle, 0777);
}

function buffer($id,$var) {
	global $_v, $_s;
	$rec = $_s['data_dir'].'/buffer/'.$id.'_'.$var;
	if(isset($_v[$var])) fsave($rec,'w+',$_v[$var]);
	if(file_exists($rec) && filesize($rec)==0) @unlink($rec);
	if(file_exists($rec)) return file_get_contents($rec);
	else return false;
}

function chekers() {	global $_v, $_s, $post;
	 $_s['cmchk']=$_s['clchk']=$_s['hdchk']=$_s['prmchk']=$_s['prvchk']=$_s['sbschk']='';
	 $c='value="ON" checked="checked"';
	 if(@trim($post['nocomm'])) $_s['cmchk']=$c;
	 if(@trim($post['close'])) $_s['clchk']=$c;
	 if(@trim($post['holdtime'])) $_s['htmchk']=$c;
	 if(@trim($post['dietime'])) $_s['dtmchk']=$c;
	 if(@trim($post['subs'])) $_s['sbschk']=$c;
	 if(@trim($post['hidden'])) $_s['hdchk']=$c;
	 if(@trim($post['commhide'])) $_s['prmchk']=$c;
	 if(@trim($post['commpriv'])) $_s['prvchk']=$c;
}

// menu gen functions
function post_menu() {	global $post, $_l, $_s;
		$post['menu']=$post['del']=$post['edit']='';
		if(is_admin()) {		hook('MQ_POST_MENU');
 		$d='';
        if($_s['confirm_post']) $d = 'onclick="return confirm(\''.$_l['confirm'].' ['.$post['title'].']\')"';
        if(@$post['close']) $post['menu']='<a href="'.$_s['base_url'].'/?action=unlock_post&amp;p='.$post['id'].'">'.$_l['closepost_'].'</a>'.$post['menu'];
        else $post['menu']='<a href="'.$_s['base_url'].'/?action=lock_post&amp;p='.$post['id'].'">'.$_l['closepost'].'</a>'.$post['menu'];
        if(@$post['hidden']) $post['menu']='<a href="'.$_s['base_url'].'/?action=unhide_post&amp;p='.$post['id'].'">'.$_l['hidepost_'].'</a>'.$post['menu'];
        else $post['menu']='<a href="'.$_s['base_url'].'/?action=hide_post&amp;p='.$post['id'].'">'.$_l['hidepost'].'</a>'.$post['menu'];
 		if(@$post['holdtime']) $post['menu']=$post['menu'].str_replace('%date%',date($_s['dateformat'],$post['ida']+$_s['time_offset']*3600),$_l['holdtime']);
 		if(@$post['dietime']) $post['menu']=$post['menu'].str_replace('%date%',date($_s['dateformat'],$post['dietime']+$_s['time_offset']*3600),$_l['dietime']);
 		$post['edit'] = '<a href="'.$_s['base_url'].'/?action=edit_post&amp;p='.$post['id'].'">'.$_l['post_ed'].'</a>';
   		$post['del']  = '<a href="'.$_s['base_url'].'/?action=del_post&amp;p='.$post['id'].'" '.$d.'>'.$_l['post_del'].'</a>';
        $post['menu'] = $post['menu'].$post['edit'].$post['del'];
        }
}

function comment_menu() {
	global $post, $_l, $_v, $i, $_s, $cmnt;		$cmnt['menu']='';
        if (is_admin()) {
 		$d=''; if($_s['confirm_comment']) $d = 'onclick="return confirm(\''.$_l['confirm'].'\')"';
        $cmnt['del'] = '<a href="'.$_s['base_url'].'/?action=del_comment&amp;c='.$i.'&amp;p='.$_v['p'].'" '.$d.'>'.$_l['cmnt_del'].'</a>';
        $cmnt['ed'] = '<a href="'.$_s['base_url'].'/?action=edit_comment&amp;c='.$i.'&amp;p='.$_v['p'].'">'.$_l['cmnt_ed'].'</a>';
        $d='';  if($_s['confirm_attach']) $d = 'onclick="return confirm(\''.$_l['confirm'].'\')"';
        $cmnt['del_attach'] = '<a href="'.$_s['base_url'].'/?action=del_attach&amp;c='.$i.'&amp;p='.$_v['p'].'" '.$d.'>'.$_l['del_attach'].'</a>';
		if(@$cmnt['priv']==1) $cmnt['himnu']="<a href='".$_s['base_url']."'/?action=unhide_comment&amp;c=".$i."&amp;p=".$post['id']."'>".$_l['cmnt_op']."</a>";
		else
		$cmnt['himnu']="<a href='".$_s['base_url']."/?action=hide_comment&amp;c=".$i."&amp;p=".$post['id']."'>".$_l['cmnt_hi']."</a>";
        $cmnt['menu']=$cmnt['del'].$cmnt['ed'].$cmnt['himnu'];
        }
}
//

// link functions
function generate_link($post_id) {
        global $_s, $post;
        if($_s['config_chpu'] == "1") return $_s['base_url'].'/p/'.$post_id;
        else return $_s['base_url'].'/?p='.$post_id;
}

function page_link($pg_id) {
        global $_s;
        if($_s['config_chpu'] == "1") return $_s['base_url'].'/page/'.$pg_id;
        else return $_s['base_url'].'/?pg='.$pg_id;
}

function act_link($ac_id) {
        global $_s;
        if($_s['base_url']) return $_s['base_url'].'/?action='.$ac_id;
        return $_s['base_url'].$_SERVER['PHP_SELF'].'?action='.$ac_id;
}

function unsubs_link($p, $m) {
		global $_s;        return $_s['base_url'].'/?action=unsubscribe&amp;p='.$p.'&amp;mail='.$m;
}

function skip_link($sp) {
        global $_s;
       if($_s['config_chpu'] == "1") return $_s['base_url'].'/skip/'.$sp.$_s['pt'];
       else return ''.$_s['base_url'].'/?skip='.$sp.$_s['pt'];
}

function rss_link() {
        global $_s;
        if($_s['base_url']) return $_s['base_url'].'/rss';
        return $_s['base_url'].$_SERVER['PHP_SELF'].'/rss';
}

function page($pg) {
		global $_l, $_s, $tpl, $_v;
  		if((eregi("^([A-Za-z0-9А])", $pg) && !ereg('.off',$_s['pages_dir'].'/'.$pg) && !is_admin() && file_exists($_s['pages_dir'].'/'.$_s['page404']) && !file_exists($_s['pages_dir'].'/'.$pg)) || (strstr($pg, 'm.') && !is_admin())) $pg=$_s['page404'];
		$d='';
        if($_s['confirm_page']) $d = 'onclick="return confirm(\''.$_l['confirm'].' ['.$pg.']\')"';
		if(is_admin()) e('<a href="'.$_s['base_url'].'/?action=edit_page&amp;pg='.$pg.'">'.$_l['ed'].'</a> <a href="'.$_s['base_url'].'/?action=del_page&amp;pg='.$pg.'" '.$d.'>'.$_l['del'].'</a><br />');
		if(file_exists($_s['pages_dir'].'/'.$pg)) {
			hook('MQ_PAGE_SHOW_BEFORE');
			include_once($_s['pages_dir'].'/'.$pg);
			hook('MQ_PAGE_SHOW_AFTER');
		} elseif(is_admin()) {
			$_v['action']='add_page';
			$post['title']=trim($pg);
		    hook('MQ_POST_FORM_SHOW_BEFORE');
			include_once $_s['tpl_dir'].'/'.$tpl['post_form'];
		    hook('MQ_POST_FORM_SHOW_AFTER');
		} else {
			e($_l['not_found']);
		}
}

function redirect($u) {
	global $_l;
	$_l['redirect']=str_replace('%link%',$u,$_l['redirect']);
	e('<meta http-equiv="refresh" content="4;url='.$u.'"> '.$_l['redirect']);
}

function container($container) {
		global $blogedit, $_s, $_c, $_l;
        $blocks = array();
        $d_blocks = dir($_s['blocks_dir'].'/'.$container);
        while (false !== ($file = $d_blocks->read())) if ($file != '.' && $file != '..' && !ereg('.off',$file)) $blocks[] = $file;
        asort($blocks);
        foreach ($blocks as $block) {
        		$d='';
        		if($_s['confirm_block']) $d = 'onclick="return confirm(\''.$_l['confirm'].' ['.$block.']\')"';
                if(is_admin() && $_s['blockmenu']) {
                $_s['blockmenu']='<small><a href="'.$_s['base_url'].'?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=edit&amp;container='.$container.'&amp;block='.$block.'">'.$_l['ed'].'</a><a href="'.$_s['base_url'].'?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=del&amp;container='.$container.'&amp;block='.$block.'" '.$d.'>'.$_l['del'].'</a></small> ';
                e($_s['blockmenu']);
                }
                hook('MQ_BLOCK_INCLUDE_BEFORE');
                include($_s['blocks_dir'].'/'.$container.'/'.$block);
        }
        $d_blocks->close();
}

function upload_files() {
        global $_s;
        $upload_dir = $_s['upload_dir'];
		if(isset($_FILES['userfile']['name'])) {
        for ($i=0; $i<sizeof($_FILES['userfile']['name']); $i++) if(trim($_FILES['userfile']['tmp_name'][$i])!='') @move_uploaded_file($_FILES['userfile']['tmp_name'][$i], $upload_dir.'/'.strtolower(basename($_FILES['userfile']['name'][$i])));
        }
}
//выбор языка
function chose_lang() {	
	global $_s, $_POST;
	if (isset($_POST['lang'])) {$_s['lang']=$_POST['lang'];}
	e('<select name="lang" style="width:100%;" class="input">');
	$h=@opendir($_s['lang_dir']);
	while($file	=readdir($h)) {
		if($file != ".." && $file !="."){
			$file =explode('.',$file);
			if($_s['lang'] == $file[0]) {$select=" selected";}
			else {$select="";}
			e('<option value="'.$file[0].'"'.$select.'>'.$file[0].'</option>');	
		}
		
	}
?>
	</select>
<?
}

// проверяем конфиг
function chec_conf($c) {
	global $_s;
	if($_s[$c]=="1") e('checked="checked"');
}
//сохраняем конфиг
function save_mqconfig() {
	global $_s, $_l;
	$_mq['encoding']='utf-8';
	$_mq['plugins_enable']=1;
	$_mq['moremeta']='';
	foreach($_POST as $k => $v) {
		$_mq[$k] = $_POST[$k];	
	}
	$checks=array('list_tags','translit','nosyspages','blockmenu','confirm_block','confirm_post','confirm_page','confirm_comment','confirm_attach','mail_comments','capcha','subscribe','nocomments','premod','allowhide','attach','hiddencmt', 'config_chpu');
	foreach($checks as $k => $v) {
		if ($_POST[$v] == "on") $_mq[$v]=1;else $_mq[$v]=0;
	}	
	foreach ($_mq as $k => $v) $wd .= '$_s[\''.$k.'\']=\''.prp($v).'\';'."\n";
	if(isset($wd)) fsave($_s['pages_dir'].'/m.settings','w+',"<?php\n".$wd."?>");
	
	if ($_l['install'] != "done") redirect($_s['base_url'].'/?action=config');
}
//выводим форму редактирования
function mqconfig() {
	global $_s, $_l;
?>
<form method="post">
<input type="hidden" name="save" value="finish">
<table border="0" width="100%" cellspacing="0" id="mqconfig">
	<tr>
		<td width="250px"><?=$_l['base_url'];?></td>
		<td> <input type="text" name="base_url" value="<?=$_s['base_url'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['author'];?></td>
		<td> <input type="text" name="author" value="<?=$_s['author'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['lang'];?></td>
		<td><?php chose_lang(); ?></td>
	</tr>
	<tr>
		<td><?=$_l['pass'];?></td>
		<td> <input type="text" name="pass" value="<?=$_s['pass'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['email'];?></td>
		<td> <input type="text" name="email" value="<?=$_s['email'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['blogname'];?></td>
		<td> <input type="text" name="blogname" value="<?=$_s['blogname'];?>" class="input" /></td>
	</tr>	
	<tr>
		<td><?=$_l['blogdescription'];?></td>
		<td> <input type="text" name="blogdescription" value="<?=$_s['blogdescription'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['metakeywords'];?></td>
		<td> <input type="text" name="metakeywords" value="<?=$_s['metakeywords'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['config_chpu'];?></td>
		<td> <input type="checkbox" name="config_chpu" <?php chec_conf('config_chpu');?>  class="checkbox"/> </td>
	</tr> 
	<tr>
		<td><?=$_l['time_offset'];?></td>
		<td> <input type="text" name="time_offset" value="<?=$_s['time_offset'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['ppp'];?></td>
		<td> <input type="text" name="ppp" value="<?=$_s['ppp'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['ppp_nav_type'];?></td>
		<td>
			<select name="ppp_nav_type">
				<option value="2" <? if($_s['ppp_nav_type']=="2"){?>selected<?}?>>&lt;&lt;Раньше  Позже&gt;&gt;</option>
				<option value="1"<? if($_s['ppp_nav_type']=="1"){?>selected<?}?>>Номера страниц</option>
			</select>
		</td>
	</tr>
	<tr>
		<td><?=$_l['list_ppp'];?></td>
		<td> <input type="text" name="list_ppp" value="<?=$_s['list_ppp'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['list_tags'];?></td>
		<td><input type="checkbox" name="list_tags" <?php chec_conf('list_tags');?>  class="checkbox" /> </td>
	</tr>
	<tr>
		<td><?=$_l['translit'];?></td>
		<td><input type="checkbox" name="translit" <?php chec_conf('translit');?>  class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['nosyspages'];?></td>
		<td><input type="checkbox" name="nosyspages" <?php chec_conf('nosyspages');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['index'];?></td>
		<td> <input type="text" name="index" value="<?=$_s['index'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['page404'];?></td>
		<td> <input type="text" name="page404" value="<?=$_s['page404'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['blockmenu'];?></td>
		<td><input type="checkbox" name="blockmenu" <?php if($_s['blockmenu'] && $_s['blockmenu'] != "0")e('checked="checked"'); ?>  class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['confirm_block'];?></td>
		<td><input type="checkbox" name="confirm_block" <?php chec_conf('confirm_block');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['confirm_post'];?></td>
		<td><input type="checkbox" name="confirm_post" <?php chec_conf('confirm_post');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['confirm_page'];?></td>
		<td><input type="checkbox" name="confirm_page" <?php chec_conf('confirm_page');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['confirm_comment'];?></td>
		<td><input type="checkbox" name="confirm_comment" <?php chec_conf('confirm_comment');?>class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['confirm_attach'];?></td>
		<td><input type="checkbox" name="confirm_attach" <?php chec_conf('confirm_attach');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['mail_comments'];?></td>
		<td><input type="checkbox" name="mail_comments" <?php chec_conf('mail_comments');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['last_comm_bd'];?></td>
		<td> <input type="text" name="last_comm_bd" value="<?=$_s['last_comm_bd'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['capcha'];?></td>
		<td><input type="checkbox" name="capcha" <?php chec_conf('capcha');?>  class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['subscribes'];?></td>
		<td><input type="checkbox" name="subscribe" <?php chec_conf('subscribe');?>  class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['nocomments'];?></td>
		<td><input type="checkbox" name="nocomments" <?php chec_conf('nocomments');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['max_comment'];?></td>
		<td> <input type="text" name="max_comment" value="<?=$_s['max_comment'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['max_nick'];?></td>
		<td> <input type="text" name="max_nick" value="<?=$_s['max_nick'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['premod'];?></td>
		<td><input type="checkbox" name="premod" <?php chec_conf('premod');?>  class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['allowhide'];?></td>
		<td><input type="checkbox" name="allowhide" <?php chec_conf('allowhide');?>  class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['hiddencmt'];?></td>
		<td><input type="checkbox" name="hiddencmt" <?php chec_conf('hiddencmt');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['attach'];?></td>
		<td><input type="checkbox" name="attach" <?php chec_conf('attach');?> class="checkbox"/> </td>
	</tr>
	<tr>
		<td><?=$_l['attype'];?></td>
		<td> <input type="text" name="attype" value="<?=$_s['attype'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['dateformat'];?></td>
		<td> <input type="text" name="dateformat" value="<?=$_s['dateformat'];?>" class="input" /></td>
	</tr>
	<tr>
		<td><?=$_l['comment_dateformat'];?></td>
		<td> <input type="text" name="comment_dateformat" value="<?=$_s['comment_dateformat'];?>" class="input" /></td>
	</tr>
</table><br/>
<input type="submit" value="<? e($_l['pform_send']); ?>" style="width:100%" />
</form>
<?
}

function e($e) {
	echo $e;
}
?>
