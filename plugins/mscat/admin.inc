<?php
if($_v['action'] == 'mscat_help') {
	e('<div align="center"><b>'.$plugin_name.'</b> (v. '.$plugin_version.', '.$plugin_author.')</div>'.$_l['mscat_help']);
}
if($_v['action'] == 'mscat_s') {
	mscat_save();
	$f=file_get_contents($_s['data_dir'].'/mscat/category');
	$mscat=explode('||',$f);
	e('<form action="" method="post">');
	foreach ($mscat as $k=>$v) {
		if ($v!="") {
		?>
		<input name="ms[<?=$k;?>]" value="<?=$v;?>" /><input type="checkbox" name="<?php e('del['.$k.']'); ?>"><br/>
		<?
		}
		
	}
	e('<input name="new" value="" /> - '.$_l['mscat_add']);
	e('<br /><input name="save" type="hidden" value="on" /><input type="submit" /></form>');
}

function mscat_save() {
	global $_s;
	if (isset($_POST['save'])) {
		
		foreach($_POST['ms'] as $k=>$v) {
			
			if ($_POST['del'][$k] == 'on'){
				unset($_POST['ms'][$k]);
			}
		}
	if ($_POST['new']!='') { $_POST['ms']['new'] =$_POST['new'];}
		$tmp=implode('||', $_POST['ms']);
	fsave($_s['data_dir'].'/mscat/category','w+',$tmp);
	}
}
?>
