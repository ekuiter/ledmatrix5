<?php

$username = "root";
$password = "your password here";

require_once "definitions.inc.php";
require_once "Connection.class.php";
require_once "LedMatrix.class.php";
require_once "Effect.class.php";
require_once "Control.class.php";
require_once "Plugin.class.php";
require_once "plugins/LastfmPlugin.class.php";
require_once "plugins/MailPlugin.class.php";

$control = new Control(array(
  new LastfmPlugin("username", 5), // username, update interval in seconds
  new MailPlugin(60) // update interval in seconds, the credentials are stored in plugins/data/mail_config.lua
));
