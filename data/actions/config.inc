<?php
if (is_admin()) {
	if(isset($_POST['save'])) {
		save_mqconfig();
	} else {
		mqconfig();
	}

} else {
        e($_l['action_den']);
        redirect($_SERVER['PHP_SELF']);
}
?>
