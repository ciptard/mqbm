<?php
if($_v['action'] == 'tgshelp') {
 e('<div align="center"><b>'.$plugin_name.'</b> (v. '.$plugin_version.', '.$plugin_author.')</div>'.$_l['tag_helps']);
}
if($_v['action'] == 'tgssett') {
 e('<div align="center"><b>'.$plugin_name.'</b> (v. '.$plugin_version.', '.$plugin_author.')</div>');

 $c='value="ON" checked="checked"';

 $c1='';

 if(@$_v['save']) {
 $_v['sett']=@$_v['tgstr'].':';
 buffer($plugin_name,'sett');
 }

 $e=explode(':',buffer($plugin_name,'sett'));
 if(trim($e[0])) $c1=$c;

 e('<p align="center"><b>'.$_l['tag_sett'].'</b></p><form action="" method="post">
 <input name="save" type="hidden" value="on" />
 <input name="tgstr" type="checkbox" '.$c1.' /> - '.$_l['tags_tr'].'
 <br /><br /><div align="center"><input type="submit" value="'.$_l['tags_send'].'" style="width:250px;font-size:20px;padding:10px" /></div></form>');
}
?>