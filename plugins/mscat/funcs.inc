<?php
function mscat_simple() {
	global $_s, $tags;
	$f=file_get_contents($_s['data_dir'].'/mscat/category');
	$mscat=explode('||',$f);
	foreach ($mscat as $k=>$v) {
		unset($tags[$v]);
	}
}
function mscat_show($tpl) {
	global $_s, $tags;
	print_r($tags);
	if($_s['config_chpu'] == "1") $tchpu=$_s['base_url'].'/tag/';
        else $tchpu=$_s['base_url'].'/?t=';
    $f=file_get_contents($_s['data_dir'].'/mscat/category');
	$mscat=explode('||',$f);
	$alltags="";
	foreach ($mscat as $k=>$v) {
		if ($v!="") {	
			$altgs=str_replace('%name%',tag_translit($v,1),$tpl);
			$altgs=str_replace('%link%',tags_link($v),$altgs);
			$alltags .= $altgs;
		}
	}
	e($alltags);
}
?>
