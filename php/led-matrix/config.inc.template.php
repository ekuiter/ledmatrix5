<?php

// ========================== ADJUST ==========================

// insert the path to the led-matrix-data directory (no trailing slash)
$GLOBALS["dataPath"] = $dataPath = "/root/led-matrix-data";

// insert this in init.d/led-matrix too!
$GLOBALS["controlPass"] = "abcdefghijklmnopqrstuvwxyz0123456789";

// Arduino Yun credentials
$username = "root";
$password = "your password here";

date_default_timezone_set("Europe/Berlin");

// ============================================================

$GLOBALS["logFile"] = "$GLOBALS[dataPath]/control.log";
$GLOBALS["stateFile"] = "$GLOBALS[dataPath]/state.dat";

require_once "$dataPath/include/definitions.inc.php";
require_once "$dataPath/include/Connection.class.php";
require_once "$dataPath/include/LedMatrix.class.php";
require_once "$dataPath/include/Effect.class.php";
require_once "$dataPath/include/State.class.php";
require_once "$dataPath/include/Control.class.php";
require_once "$dataPath/include/Plugin.class.php";
require_once "$dataPath/include/plugins/IdlePlugin.class.php";
require_once "$dataPath/include/plugins/LastfmPlugin.class.php";
require_once "$dataPath/include/plugins/MailPlugin.class.php";

// ========================== ADJUST ==========================

$control = new Control(array(
  new LastfmPlugin("username", 5), // username, update interval in seconds
  new MailPlugin(60) // update interval in seconds, the credentials are stored in plugins/data/mail_config.lua
));

// ============================================================