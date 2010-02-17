<?php
	if (@trim($_v['mail'])!='') {
	$dt=buffer($_v['p'], 'maillist');
	if (eregi($ma=trim($_v['mail']),$dt)) {
	$dt = preg_replace("/".$ma."\n|\n?".$ma."$/", '', $dt);
	$_v['maillist']=$dt;
	buffer($_v['p'], 'maillist');
	$f=$_s['data_dir'].'/buffer/'.$_v['p'].'_maillist';
	e($_l['unsubscribe']);
	}
	}
?>