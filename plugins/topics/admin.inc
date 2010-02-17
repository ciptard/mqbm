<?php
if($_v['action'] == 'tpchelp') {
 e('<div align="center"><b>'.$plugin_name.'</b> (v. '.$plugin_version.', '.$plugin_author.')</div>'.$_l['topics_help']);
}
if($_v['action'] == 'tpcsett') { e('<div align="center"><b>'.$plugin_name.'</b> (v. '.$plugin_version.', '.$plugin_author.')</div>');

 $c='value="ON" checked="checked"';

 $c1=$c2=$c3='';

 if(@$_v['save']) {
 $_v['sett']=@$_v['tpcon'].':'.@$_v['tpclist'].':'.@$_v['tpctlist'];
 buffer($plugin_name,'sett');
 }

 $e=explode(':',buffer($plugin_name,'sett'));
 if(trim($e[0])) $c1=$c;
 if(trim($e[1])) $c2=$c;
 if(trim($e[2])) $c3=$c;

 e('<p align="center"><b>'.$_l['topics_sett'].'</b></p><form action="" method="post">
 <input name="save" type="hidden" value="on" />
 <input name="tpcon" type="checkbox" '.$c1.' /> - '.$_l['topics_off'].'
 <br /><input name="tpclist" type="checkbox" '.$c2.' /> - '.$_l['topics_aron'].'
 <br /><input name="tpctlist" type="checkbox" '.$c3.' /> - '.$_l['topics_list'].'
 <br /><br /><div align="center"><input type="submit" value="'.$_l['topics_send'].'" style="width:250px;font-size:20px;padding:10px" /></div></form>');

}
?>