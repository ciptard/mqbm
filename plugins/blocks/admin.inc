<?php
// Get containers
$d = dir($_s['blocks_dir']);
while (false !== ($entry = $d->read())) if ($entry != '.' && $entry != '..' && is_dir($_s['blocks_dir'].'/'.$entry)) $containers[] = $entry;
$d->close();

switch ($_v['action']) {
        case 'list':
                $d = dir($_s['blocks_dir']);
                while (false !== ($entry = $d->read())) {
                        if ($entry != '.' && $entry != '..') {
                                if (is_dir($_s['blocks_dir'].'/'.$entry)) {
                                        print '<table border="0"  ><tr> <td><b>'.$entry.'</b></td> </tr>';

                                        $bd = dir($_s['blocks_dir'].'/'.$entry);
                                        $blocks_list = array();
                                        while (false !== ($block = $bd->read())) if ($block != '.' && $block != '..') $blocks_list[] = $block;
                                        asort($blocks_list);
                                        foreach ($blocks_list as $block) {
                                        		$dblock=$block;

                                        		if(ereg('.off',$block)) { $dblock = str_replace('.off','',$block); $dblock='<font color="red">'.translit($dblock,1).'</font>'; } else { $dblock='<font color="green">'.translit($block,1).'</font>'; }
                                        		if($entry=='pages') { $dblock = "<a href='index.php?pg=".$block."'>".translit($dblock,1)."</a>"; }
                                                print '<tr> <td>&raquo;'.$dblock.'</td> <td> <a href="?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=edit&amp;container='.$entry.'&amp;block='.$block.'">'.$_l['ed'].'</a> </td> <td> <a href="?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=del&amp;container='.$entry.'&amp;block='.$block.'" onclick="return confirm(\'Удалить '.translit($block,1).' ?\')">'.$_l['del'].'</a></td> </tr>';
                                        }
                                        $blocks_list = '';
                                        print '</table>';
                                }
                        }
                }
                $d->close();

                break;

        case 'edit':
                if (!isset($_v['block_content'])) {

                        print '<form action="'.$PHP_SELF.'" method="post">';
                        print '<input type="hidden" name="plugin" value="'.$_v['plugin'].'" />';
                        print '<input type="hidden" name="action" value="'.$_v['action'].'" />';
                        print '<input type="hidden" name="block" value="'.$_v['block'].'" />';
                        print '<input type="hidden" name="old_container" value="'.$_v['container'].'" />';
                        $disbl=$_v['block'];
						if (ereg(".off",$_v['block'])) { $_v['onof'] = 'off'; $disbl = str_replace('.off','',$_v['block']); } else { $_v['onof'] = "on"; }                    print '<input type="hidden" name="old_onoff" value="'.$_v['onof'].'" />';
                        print '<input type="text" name="file" value="'.translit($disbl,1).'" />';
                        print ' <select name="container">';
                        for ($i = 0; $i < sizeof($containers); $i++) {
                                print '<option value="'.$containers[$i].'" '.($_v['container'] == $containers[$i] ? 'selected="selected"' : '').'>'.$containers[$i].'</option>';
                        }
                        print '</select> ';

                        print '<select name="onof">';
                        if ($_v['onof'] == "off") { $sel="selected=\"selected\" style=\"background-color:#FF7777\""; } else { $selo="selected=\"selected\" style=\"background-color:#B4EBAF\""; }
  						print '<option value="on" '.$selo.'>'.$_l['blockon'].'</option>';
                        print '<option value="off" '.$sel.'>'.$_l['blockoff'].'</option>';
                        print '</select><br />';

                        print '<textarea name="block_content" cols="70" rows="20" style="width:100%;">'.htmlspecialchars(file_get_contents('data/blocks/'.$_v['container'].'/'.$_v['block'])).'</textarea><br />';
                        print '<div align="center"><input type="submit" value="'.$_l['blocksend'].'" /></div></form>';
                }
                else
                {       $_v['file']=translit($_v['file']);
                        // if block name or container changed then delete old file and write to new
                        if(@trim($_v['old_onoff'])=='off' && @trim($_v['onof'])=='off') $_v['file']=$_v['file'].'.off';

                        if(@trim($_v['onof'])!=@trim($_v['old_onoff'])) {
                        if(@trim($_v['onof'])=='off'):
                        $_v['file']=$_v['file'].'.off';
                        endif;
                        }

                        if (($_v['block'] != $_v['file']) || ($_v['old_container'] != $_v['container']))
                        {
                                unlink($_s['blocks_dir'].'/'.$_v['old_container'].'/'.$_v['block']);
                                $_v['block'] = $_v['file'];
                        }

                        $f = fopen($_s['blocks_dir'].'/'.$_v['container'].'/'.$_v['block'], 'w+');
                        fwrite($f, $_v['block_content']);
                        fclose($f);

						 redirect('?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=list');

                }
                break;

        case 'del':
                if (file_exists($_s['blocks_dir'].'/'.$_v['container'].'/'.$_v['block'])) {
                        if (unlink($_s['blocks_dir'].'/'.$_v['container'].'/'.$_v['block'])) {
                                redirect('?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=list');
                        }
                }
                break;

        case 'new_block':
                if (!isset($_v['block_content'])) {
                        print '<form action="'.$PHP_SELF.'" method="post">';
                        print '<input type="hidden" name="plugin" value="'.$_v['plugin'].'" />';
                        print '<input type="hidden" name="action" value="'.$_v['action'].'" />';
                        print '<input type="text" name="file" />';
                        print '<select name="container">';
                        for ($i = 0; $i < sizeof($containers); $i++) e('<option value="'.$containers[$i].'">'.$containers[$i].'</option>');
                        print '</select><br />';
                        print '<textarea name="block_content" cols="70" rows="20" style="width:100%;"></textarea><br />';
                        print '<div align="center"><input type="submit" value="'.$_l['blocksend'].'"></div></form>';
                } else {
                		fsave($_s['blocks_dir'].'/'.$_v['container'].'/'.$_v['file'], 'w+', $_v['block_content']);
                        redirect('?pg='.$_s['admpage'].'&amp;plugin=blocks&amp;action=list');
                }
                break;

}

?>
