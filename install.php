<? error_reporting(E_ALL & ~E_NOTICE); ?>
<?
if (isset($_POST['lang'])) {
	include_once 'lang/'.$_POST['lang'].'.inc';
} else {
include_once 'lang/ru.inc';
}
include_once 'data/functions.inc';
include_once 'data/blocks/pages/m.settings';
$mqlock=$_s['pages_dir'].'/m.lock';
if(!file_exists($mqlock)){header('Location: index.php');}



function creat_file_list($s){
	if ($h=@opendir($s)) {
		while($file=readdir($h)){
			$file=$s.'/'.$file;
			$p="/[.]/i";
			if(!preg_match($p,$file)) {	
				if(is_dir($file)){
					$ff[]=$file;
					if($file!='data/sess'){
						$dir=creat_file_list($file);
						if (!empty($dir)){
							foreach($dir as $k => $v ){ 
								$ff[]=$dir[$k];
							}
						}
					}
				} else {
					$ff[]=$file;
				}
			}
		}
	}	
	return $ff; 
}
function check_perm() {
	global $_s; ?> 
	<table border="0" width="100%" cellspacing="0" id="mqconfig">
	<? 
	
	$files = creat_file_list('data');
	$files[]="data/blocks/pages/m.settings";
	$files[]="data/blocks/pages/m.error404";
	$files[]="data/sess";
	$files[]="upload";
	$files[]="upload/attach";
	$files[]="upload/post";
	foreach ($files as $k => $i) {
		e('<tr><td>'.$i.'</td>');
		@chmod($i,'777');
		$perms[$k]=substr(sprintf('%o',fileperms("$i")),-3);
		if($perms[$k] != "777") { $style='color:red;'; } else { $style='color:green;';}
		e('<td style='.$style.'>'.$perms[$k].'</td></tr>');
	}
	?></table><?
	$good=array_diff($perms,array("777"));

	if ( $good == Array() ) {
	?>
		<form action="install.php" method="post">
			<input type="hidden" name="conf" value="">
			<input type="hidden" name="lang" value="<?=$_s['lang'];?>" /><br/>
			<input type="submit" value="Next" style="width:100%;">
		</form>
	<?
	} else {
		e('check permissons');
	}
	
}
if(isset($_POST['save'])) {
	$title="step 3";
} elseif(isset($_POST['conf'])) {
	$title="instalation: configure";
} elseif(isset($_POST['perm'])) {
	$title="instalation: check permissions";
} else {
	$title="instalation: chose languages";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ru" lang="ru">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<link href="templates/style.css" rel="stylesheet" type="text/css" media="screen" />
<title><? e($title); ?></title>


</head>
<body>
<div id="wrapper"> 
 
	<div id="header"> 
		<div class="logo">
			<div class="head">
				<?  @container('header'); ?>
			</div>
		</div>
		
	</div>
	

	
	<div id="middle"> 
 
		<div id="container"> 
			<div id="content" style="padding:0 150px 0 150px;"> 

<b class="pl">&nbsp;</b><b class="pr">&nbsp;</b><h2 class="title"><? e($title); ?></a></h2>
		<div style="padding:15px;">
<?

if(isset($_POST['save'])) {
	save_mqconfig();
	unlink($mqlock);
} elseif(isset($_POST['conf']) ||isset($_GET['conf'])) {
	$lang=$_POST['lang'];
	mqconfig();
} elseif(isset($_POST['perm'])) {
	$_s['lang']=$_POST['lang'];
	check_perm();
}else {
?>
<form action="install.php" method="post">
<?
	chose_lang();
?>
	<input type="hidden" name="perm" value=""><br/><br/>
	<input type="submit" value="Next" style="width:100%;">
</form>
<?
}

?>
				</div>
		  			</div><!-- #content--> 
		</div><!-- #container--> 
 
	</div><!-- #middle--> 
 
</div><!-- #wrapper --> 
 
<div id="footer"> 
<?  @container('footer'); ?>
</div><!-- #footer --> 

</body>
</html>
