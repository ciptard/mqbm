<?php
$_s['tags_dir']=$_s['data_dir'].'/tags';

function tag_translit($String, $detrans = FALSE) {
        global $_s;
        $String=strip_tags(trim($String));
		$String = str_replace(array('<','>','!','?','¦',"\t","\r","\n",'@','#','%','«','»','^','&','*','(',')','\\',',','№','~',':',';','[',']','=','"','\'','/'), '', $String);
        if($_s['lang']=='ru' && !strstr($String, 'm.') && $_s['tag_tag_translit']) {
        $Alpha=array('ё'=>'yo','й'=>'yi','ц'=>'c','у'=>'u','к'=>'k','е'=>'e','н'=>'n','г'=>'g','ш'=>'sh','щ'=>'she','з'=>'z','х'=>'x','ъ'=>'tz','ф'=>'f','ы'=>'y','в'=>'v','а'=>'a','п'=>'p','р'=>'r','о'=>'o','л'=>'l','д'=>'d','ж'=>'j','э'=>'e-','я'=>'ya','ч'=>'ch','с'=>'s','м'=>'m','и'=>'i','т'=>'t','ь'=>'mz','б'=>'b','ю'=>'ju','Ё'=>'Yo','Й'=>'YI','Ц'=>'C','У'=>'U','К'=>'K','Е'=>'E','Н'=>'N','Г'=>'G','Ш'=>'Sh','Щ'=>'She','З'=>'Z','Х'=>'X','Ъ'=>'Tz','Ф'=>'F','Ы'=>'Y','В'=>'V','А'=>'A','П'=>'P','Р'=>'R','О'=>'O','Л'=>'L','Д'=>'D','Ж'=>'J','Э'=>'E-','Я'=>'Ya','Ч'=>'Ch','С'=>'S','М'=>'M','И'=>'I','Т'=>'T','Ь'=>'Mz','Б'=>'B','Ю'=>'Ju',' '=>'_','\''=>'');
        $Beta=array('yo'=>'ё','yi'=>'й','c'=>'ц','u'=>'у','k'=>'к','e'=>'е','n'=>'н','g'=>'г','sh'=>'ш','she'=>'щ','z'=>'з','x'=>'х','tz'=>'ъ','f'=>'ф','y'=>'ы','v'=>'в','a'=>'а','p'=>'п','r'=>'р','o'=>'о','l'=>'л','d'=>'д','j'=>'ж','e-'=>'э','ya'=>'я','ch'=>'ч','s'=>'с','m'=>'м','i'=>'и','t'=>'т','mz'=>'ь','b'=>'б','ju'=>'ю','Yo'=>'Ё','YI'=>'Й','C'=>'Ц','U'=>'У','K'=>'К','E'=>'Е','N'=>'Н','G'=>'Г','Sh'=>'Ш','She'=>'Щ','Z'=>'З','X'=>'Х','TZ'=>'Ъ','F'=>'Ф','Y'=>'Ы','V'=>'В','A'=>'А','P'=>'П','R'=>'Р','O'=>'О','L'=>'Л','D'=>'Д','J'=>'Ж','E-'=>'Э','Ya'=>'Я','Ch'=>'Ч','S'=>'С','M'=>'М','I'=>'И','T'=>'Т','Mz'=>'Ь','B'=>'Б','Ju'=>'Ю','_'=>' ','\''=>'');
		if($detrans) $String=strtr($String, $Beta);
		else $String=strtr($String, $Alpha);
  		}

    return $String;
}

// tags functions by Bolzamo
function clean_str($str){
	return trim(str_replace(array("\n","\r","\t",'\\','/','.','%','&',':','<','>','"','\''),'',$str));
}

function dir2array($patch){
	$d = dir($patch);
	while (false !== ($entry = $d->read()))
	if (is_numeric($entry)) $entries[] = $entry;
	$d->close();
	return @$entries;
}

function ct($tag){//Кеширует отдельный тег
	global $_s;
	$tag_cache='';
	$posts=dir2array($_s['tags_dir']);
	if(isset($posts)){
	foreach($posts as $post){
		$tags=explode(',',file_get_contents($_s['tags_dir'].'/'.$post));
		if(in_array($tag,$tags))$tag_cache_array[]=$post;
	}
	}
	@$tag_cache=implode(',',$tag_cache_array);
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	$tt=tag_translit($tag);
	if(trim($tt)) fsave($_s['tags_dir'].'/cache/'.$tt,'w+',$tag_cache);
	if(filesize($_s['tags_dir'].'/cache/'.$tt)<1) @unlink($_s['tags_dir'].'/cache/'.$tt);
	return true;
}

function cache_tags($tag){//Функция для массового/одиночного кеширования тегов
	global $_s;
	if(@strstr($tag,',')) $tag=explode(',',$tag);
	if(is_array($tag)) foreach($tag as $t) ct($t);
	elseif($tag!='') ct($tag);
	else {
		$posts=dir2array($_s['tags_dir']);
		$last_post=array_pop($posts);
		$tags='';
		foreach($post as $post) $tags.=file_get_contents($_s['tags_dir'].'/'.$post).',';
		$tags.=file_get_contents($_s['tags_dir'].'/'.$last_post);
		$a_tags=split(',',$tags);
		foreach($a_tags as $tag) ct($tag);
	}
	return true;
}

function del_post_tags(){
	global $_s, $_v;
	if(file_exists($_s['tags_dir'].'/'.$_v['p']))$tags=file_get_contents($_s['tags_dir'].'/'.$_v['p']);
	@unlink($_s['tags_dir'].'/'.$_v['p']);
	if(@$tags)cache_tags($tags);
}

function posts_by_tag($tag, $num, $tpc=false){
    global $_s, $_l, $_v, $tpl, $buffer, $post, $cmnt;
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	$_v['t']=tag_translit($tag);
	$_s['ppp']=$num;
	$_s['topics']=$tpc;
	blog();
}

function getbytag($tag){
	global $_l, $_s, $posts;
	ct($tag);
	if(file_exists($_s['tags_dir'].'/cache/'.$tag)) $posts=explode(',',file_get_contents($_s['tags_dir'].'/cache/'.$tag));
	else return false;
	return $posts;
}

function string_to_tags($string){
	global $_l, $_v, $_s;
	$tags=explode(',',$string);
	$string='';
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	if(@$_v['t']&&!in_array(@$_v['t'],$tags)) cache_tags($_v['t']);
	$last_tag=array_pop($tags);
	//if(count($tags)) foreach($tags as $tag) $string.='<a href="?t='.$tag.'">'.tag_translit($tag,1).'</a>, ';
	//return $string.'<a href="?t='.$last_tag.'">'.tag_translit($last_tag,1).'</a>';
	if(count($tags)) foreach($tags as $tag) $string.='<a href="'.tags_link($tag).'">'.tag_translit($tag,1).'</a>, ';
	return $string.'<a href="'.tags_link($last_tag).'">'.tag_translit($last_tag,1).'</a>';
}

function tag_cloud($s) {
	global $_s, $tags;
	$d = dir($_s['tags_dir'].'/cache');
	while (false !== ($entry = $d->read()))
	if ($entry!='.' && $entry!='..') $tags[$entry] = sizeof(explode(',',file_get_contents($_s['tags_dir'].'/cache/'.$entry)));
	$d->close();
	hook('MQ_TAGS_SHOW_BEFORE');
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	if(isset($tags)) foreach($tags as $tag => $size) e(" <a href='".tags_link($tag)."' title='".$size."' style='font-size:".(($size*$s)+13)."px'>".tag_translit($tag,1)."</a> ");
}

function tag_list($tpl) {
	global $_s, $_v, $entries;
	$d = dir($_s['tags_dir'].'/cache');
	while (false !== ($entry = $d->read()))
	if ($entry!='.' && $entry!='..') $entries[] = $entry;
	$d->close();
	hook('MQ_TAGS_SHOW_BEFORE');
	$alltags='';
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	foreach($entries as $tag) {
	ct($tag);
	$altgs='';
	$altgs=str_replace('%tag%',tag_translit($tag,1),$tpl);
	$altgs=str_replace('%link%',tags_link($tag),$altgs);
	$altgs=str_replace('%num%',sizeof(explode(',',file_get_contents($_s['tags_dir'].'/cache/'.$tag))),$altgs);
	$alltags .= $altgs;
	}
	e($alltags);
}

function get_post_tags(){
	global $_s, $post, $_v, $_l;
	$post['tags']='';
	if(isset($_v['p'])) $p=$_v['p'];
	elseif(isset($post['id'])) $p=$post['id'];
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	if(isset($p)) {
	if (file_exists($_s['tags_dir'].'/'.$p) && filesize($_s['tags_dir'].'/'.$p)>0) {
	if(@$_v['action']=='edit_post') {
		$altg=split(',',file_get_contents($_s['tags_dir'].'/'.$p));
		$lt=array_pop($altg);
		foreach ($altg as $tg) $post['tags']=$post['tags'].tag_translit($tg,1).', ';
		$post['tags']=$post['tags'].tag_translit($lt,1);
	} else $post['tags']=string_to_tags(file_get_contents($_s['tags_dir'].'/'.$p));
	}
	}
	$_s['dwnpostface_']=$_l['tag_form'].'<br /><input type="text" name="tags" value="'.@$post['tags'].'" style="width:100%;"><br />'.$_s['dwnpostface_'];
        return $post;
}

function tgposts() {
	global $_s, $_v, $posts, $pages, $skip;
	@$tag=clean_str(@$_v['t']);
	if(@$tag) {
	unset($_v['list']);
	$_s['pt']='&t='.$tag;
	if($_s['config_chpu'] == "1") $_s['pt'] = '/tag/'.$tag;
	$_s['posts_get_standart'] = 0;
	$posts=getbytag($tag);
	}
}

function tagit() {
	global $_v, $_s;
		if(@trim($_v['tags'])){
			$e=explode(':',buffer('Tags','sett'));
			if(@trim($e[0])) $_s['tag_tag_translit']=1;
			else $_s['tag_tag_translit']=0;
			$altg=split(',',$_v['tags']);
			$lt=array_pop($altg);
			foreach ($altg as $tag) if(trim($tag)) $tags[]=tag_translit($tag);
			if(trim($lt)) $tags[]=tag_translit($lt);
			if(@sizeof($tags)>0) {
			$tags_s=implode(',',$tags);
			@$ptf=explode(',',file_get_contents($_s['tags_dir'].'/'.$_v['p']));
			if(@$adr=array_diff($ptf,$tags)) foreach($adr as $tg) ct($tg);
			fsave($_s['tags_dir'].'/'.$_v['p'],'w',$tags_s);
			cache_tags($tags);
			}
		} else if(@$ptf=explode(',',file_get_contents($_s['tags_dir'].'/'.$_v['p']))) {
			@unlink($_s['tags_dir'].'/'.$_v['p']);
			foreach($ptf as $tg) ct($tg);
		}
}
//

//muhas
function ShuffleAcArray($arBla)
{
    $num = count($arBla);
    $arResult = array();
    while($arBla)
    {
        $rkey = array_rand($arBla);
        $arResult[$rkey] = $arBla[$rkey];
        unset($arBla[$rkey]);
    }
    
    return $arResult;
}


function top_tag_cloud($s, $top) {
	global $_s, $tags;
	$d = dir($_s['tags_dir'].'/cache');
	while (false !== ($entry = $d->read()))
	if ($entry!='.' && $entry!='..') $tags[$entry] = sizeof(explode(',',file_get_contents($_s['tags_dir'].'/cache/'.$entry)));
	$d->close();
	hook('MQ_TAGS_SHOW_BEFORE');
	@arsort($tags);
	$tags = ShuffleAcArray(@array_slice($tags, 0, $top));
	$e=explode(':',buffer('Tags','sett'));
	if(@trim($e[0])) $_s['tag_tag_translit']=1;
	else $_s['tag_tag_translit']=0;
	if(isset($tags)) foreach($tags as $tag => $size) e(" <a href='".tags_link($tag)."' title='".$size."' style='font-size:".(($size*$s)+13)."px'>".tag_translit($tag,1)."</a> ");
}
function show_near_post($number) {
	global $_v, $_s;
	if(is_numeric($_v['p'])){
	 echo '<h6>Смотрите также</h6><div class="padd c">';
	 $altg=split(',',@file_get_contents($_s['tags_dir'].'/'.$_v['p']));
	 $othposts=array();
	 foreach($altg as $tg)$othposts=array_merge($othposts,getbytag($tg));
	 $othposts=array_count_values($othposts);
	 arsort($othposts,SORT_NUMERIC);
	 $othposts=array_keys($othposts);
	 $del=array_search($_v['p'],$othposts);
	 unset($othposts[$del]);
	 $othposts=array_slice($othposts,0,7);
	 $string='';
	 foreach($othposts as $p){
	  $post=post_info($p);
	  $string.='<a href="'.generate_link($p).'">'.$post['title'].'</a><br />';
	 }
	 if($string=="") { echo "нет похожих записей</div><br />";} else {echo ''.$string.'</div>';};
	}
}
function  tags_link($v) {
	global $_s;
    if($_s['config_chpu'] == "1") return $_s['base_url'].'/tag/'.$v;
    else return $_s['base_url'].'/?t='.$v;
}
?>
