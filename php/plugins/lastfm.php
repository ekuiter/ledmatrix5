<?php

require_once "../config.inc.php";
require_once "../LedMatrix.class.php";

$lastfmData = "lastfm.dat";
$ledMatrix = new LedMatrix("localhost", MATRIX_WRITE);

function updateNowPlaying() {
  global $lastfmData, $ledMatrix, $lastfmUsername;
  $data = json_decode(file_get_contents("http://ajax.last.fm/user/$lastfmUsername/now"));
  $newData = array("playing" => $data->nowplaying,
                     "track" => (string) $data->track->name,
                    "artist" => (string) $data->track->artist->name);
  if (file_exists($lastfmData))
    $oldData = unserialize(file_get_contents($lastfmData));
  else
    $oldData = array();

  if ($newData != $oldData) {
    if ($newData["playing"]) {
      $ledMatrix->setText(str_replace(" ", "_", $newData["track"]) . "-" .
                          str_replace(" ", "_", $newData["artist"]));
      $ledMatrix->run(EFFECT_TEXT);
    } else
      $ledMatrix->setText("-");
  }

  file_put_contents($lastfmData, serialize($newData));
}

updateNowPlaying();

$start = microtime(true);
for ($i = $lastfmInterval; ; $i += $lastfmInterval) {
  updateNowPlaying();
  time_sleep_until($start + $i);
}