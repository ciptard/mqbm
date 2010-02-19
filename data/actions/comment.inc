<?php
if(isset($_v['p']) && trim($_v['p'])) {
        $_v['nick'] = (@trim($_v['nick']) ? $_v['nick'] : $_l['anonymous']);

		if ($_s['capcha'] && !is_admin()) {
		if (isset($_v['capcha'])) { $_v['capcha'] = trim($_v['capcha']); }
		if (isset($_SESSION['capcha'])) { $_SESSION['capcha'] = trim($_SESSION['capcha']); }
		if(isset($_SESSION['capcha']) && $_v['capcha']!='' && isset($_v['capcha']) && is_numeric($_v['capcha']) && $_SESSION['capcha'] ==  $_v['capcha']){
		} else {
		$_s['nocomments'] = 1;
		$_l['cmnt_den'] = $_l['cmnt_wrong_capcha'];
		}
		}

        if (@trim($_v['text'])) {
        		$post = post_info($_v['p']);

		if(!@$post['nocomm'] && !$_s['nocomments']) {
        		$_s['prv'] = $_s['aut'] = 0;

				if(is_admin()) $_s['aut']=1;
        		if(@$_v['privat']) if(@$_s['allowhide'] || @$post['commpriv']) $_s['prv'] = 1;
        		if(@$post['commhide'] && !is_admin()) $_s['premod']=1;
        		if(@$_s['premod'] && !is_admin()) $_s['prv'] = 1;

				if(@$post['communhide'] || @$post['commnopriv']) $_s['prv'] = 0;

                $text = substr(trim($_v['text']), 0, $_s['max_comment']);
                //$nick = substr(trim($_v['nick']), 0, $_s['max_nick']);
				//include "func/utf8_substr.php";

				$nick = substr(trim($_v['nick']), 0, $_s['max_nick']);
				$nick = $_v['nick'];
                $f = array('<', '>', '¦');
                $t = array('&lt;', '&gt;', '&#166;');
                $text = str_replace("\r", '', $text);
                
                if(!is_admin()) $text = str_replace($f, $t, $text);
                if($_s['bb_comments'] && !is_admin()) $text = bbcode($text);
                $nick = str_replace($f, '', $nick);
                $cmnt_date = time();
				$text = str_replace("\n", '<br />', $text);
		        if ($nick==$_s['author'] && !is_admin()) {
		        e($_l['ncikden']);
		        redirect(generate_link($_v['p']).'#commentform');
		        } else {
                $comment_allow = true;

                hook('MQ_COMMENT_ADD_BEFORE');

				if(isset($_v['mmail']) && eregi("^[_\.0-9a-z-]+@([0-9a-z][0-9a-z-]+\.)+[a-z]{2,3}$",$_v['mmail'])) {
				if(($post['subs'] || $_s['subscribe']) && isset($_v['subsmy'])) {
				$mailadd=trim($_v['mmail']);
				$ml=@buffer($_v['p'], 'maillist');
				if(!ereg($mailadd,$ml)) {					$_v['maillist']=$ml.$mailadd."\n";
					buffer($_v['p'], 'maillist');
					e($_l['subscribe']);
				}
				}
				} else {				$_v['mmail']='';
				}

                if ($comment_allow) {

				if(@$post['attach']>0) $_s['attach']=$post['attach'];

                if(@$_s['attach']>0 || is_admin()) {

				if(@$_FILES['userfile']['size'] <= 1024*$_s['attach'] || is_admin()) {

				if(isset($_FILES['userfile']['name']) && trim($_FILES['userfile']['name'])) {

				$exx=explode('.',$_FILES['userfile']['name']);
				$exf=explode(' ',$_s['attype']);
				if(in_array(trim($exx[1]), $exf) || is_admin()) {

						if(file_exists($_s['attach_dir'].'/'.$_FILES['userfile']['name'])) $_FILES['userfile']['name']=time().'_'.$_FILES['userfile']['name'];
		                if (!move_uploaded_file($_FILES['userfile']['tmp_name'], $_s['attach_dir'].'/'.$_FILES['userfile']['name'])) {
		                        @copy($_FILES['userfile']['tmp_name'], $_FILES['userfile']['name']);

		                }
		        @chmod($_s['attach_dir'].'/'.$_FILES['userfile']['name'],0777);
		        $_s['cmt_attach'] = $_FILES['userfile']['name'];
		        }
          }
		}
    }           hook('MQ_COMMENT2MAIL_BEFORE');
    			if($_s['mail_comments'] && trim($_s['email'])) @mail($_s['email'], $_l['cmnto'].': '.$nick, $text."<br><br>".generate_link($_v['p'])."#comment <br><br>, "Content-Type: text/html; charset=".$_s['encoding']."\nFrom: "$_s['blogname'].$nick."<noreply-$_s[email]>\r\n");

				$ml=$_s['data_dir'].'/buffer/'.$_v['p'].'_maillist';
				if(file_exists($ml) && filesize($ml)<2) @unlink($ml);
				if(file_exists($ml)) {
				$maillist=file($ml);
				for ($i=0;$i<sizeof($maillist);$i++) {
				$mail = str_replace("\n",'',$maillist[$i]);
				hook('MQ_SUBSCRIBE_BEFORE');
				@mail($mail, $_l['cmnto'].': '.$nick, $text."<br><br><a href='".generate_link($_v['p'])."#comment'>".$_l['link']."</a><br><br><a href='".unsubs_link($_v['p'], $mail).">".$_l['unlink']."</a><br>", "Content-Type: text/html; charset=".$_s['encoding']."\nFrom: ".$_s['blogname'].$nick."<noreply-$_s[email]>\r\n");
					}
				}

                fsave($_s['comment_dir'].'/'.$_v['p'],'a+',$nick.'¦¦'.$_s['prv'].'¦¦'.$_s['aut'].'¦¦'.getenv("REMOTE_ADDR").'¦¦'.@$_s['cmt_more'].'¦¦'.@$_v['mwww'].'¦¦'.@$_v['mmail'].'¦¦'.@$_s['user_uid'].'¦¦'.$cmnt_date.'¦¦'.$text.'¦¦'.@$_s['cmt_attach']."\n");

				// добавим в последние комментарии
				hook('MQ_LASTCOMMADD_BEFORE');
		        $last_comments = file($_s['data_dir'].'/lcomments');
		        if (sizeof($last_comments) > $_s['last_comm_bd']) unset($last_comments[$_s['last_comm_bd']]);
		        $add_string = $_v['p'].'¦¦'.(sizeof(file($_s['comment_dir'].'/'.$_v['p']))-1)."\n";
		        array_unshift($last_comments, $add_string);
				fsave($_s['data_dir'].'/lcomments','w+', implode("", $last_comments));
                //

                hook('MQ_COMMENT_ADD_AFTER');

                        if($_s['prv']) e($_l['cmnt_addhide']);
                        else e($_l['cmnt_add']);
                }
        e($_l['cmnt_for_cmt']);
        redirect(generate_link($_v['p']).'#comment');
                }
        } else {
        e($_l['cmnt_den']);
        redirect(generate_link($_v['p']));
        }
        } else {
        e($_l['cmnt_notxt']);
        redirect(generate_link($_v['p']).'#comment');
        }
}
?>
