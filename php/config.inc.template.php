<?php

$username = "root";
$password = "your password here";

require_once "definitions.inc.php";
require_once "Connection.class.php";
require_once "LedMatrix.class.php";
require_once "control/Control.class.php";
require_once "plugins/Plugin.class.php";
require_once "plugins/LastfmPlugin.class.php";

$control = new Control(array(
  new LastfmPlugin("username", 5) // username, update interval in seconds
));
