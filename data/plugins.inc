<?php
if (!defined('INCLUDED')) exit();

$_s['plugins'] = unserialize(file_get_contents($_s['data_dir'].'/plugins'));
$HOOKS = array();

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
        global $_s;

        if (!$_s['plugins_enable']) return 0;

        foreach ($_s['plugins'] as $plugin) {
                if(file_exists($lf=$_s['plugin_dir'].'/'.$plugin.'/lang_'.$_s['lang'].'.inc')) include_once $lf;
                else include_once $_s['plugin_dir'].'/'.$plugin.'/lang_def.inc';
                include_once $_s['plugin_dir'].'/'.$plugin.'/funcs.inc';
                include_once $_s['plugin_dir'].'/'.$plugin.'/info.inc';
        }
}
?>