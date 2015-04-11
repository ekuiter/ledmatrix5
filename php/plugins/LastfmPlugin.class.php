<?php

class LastfmPlugin extends Plugin {
  private $username;

  function __construct($username, $interval) {
    parent::__construct("lastfm", $interval);
    $this->username = $username;
  }

  public function fetchData() {
    $data = json_decode(file_get_contents("http://ajax.last.fm/user/$this->username/now"));
    return array("playing" => $data->nowplaying,
                   "track" => (string) $data->track->name,
                  "artist" => (string) $data->track->artist->name);
  }

  public function isNewData($data) {
    return $data != $this->loadData();
  }

  public function isRelevantData($data) {
    return $data["playing"] ? true : false;
  }

  public function stringifyData($data) {
    if ($data["playing"])
      return "$this->username is listening to $data[track] by $data[artist].";
    else
      return "$this->username is not listening to music right now.";
  }

  public function displayData($data, $ledMatrix) {
    $ledMatrix->setText($data["track"] . "-" . $data["artist"]);
    $ledMatrix->run(EFFECT_TEXT);
  }
}