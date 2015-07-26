<?php

class LastfmPlugin extends Plugin {
  private $username;

  function __construct($username, $interval) {
    parent::__construct("lastfm", $interval);
    $this->username = $username;
  }

  public function fetchData() {
    $rawData = @file_get_contents("http://ajax.last.fm/user/$this->username/now");
    if (!$rawData)
      throw new PluginException("Could not connect to last.fm.");
    $data = json_decode($rawData);
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

  protected function displayDataInternal($data) {
    (new Effect("text"))->run(COLOR_UNDEF, $data["track"] . "-" . $data["artist"]);
  }
}