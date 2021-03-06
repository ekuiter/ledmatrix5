<?php

class LedMatrix {
  private $connection, $host;

  public function __construct($host, $port) {
    $this->host = $host;
    $this->connection = new Connection($host, $port);
    $this->connection->connect();
  }
  
  private function readLedState() {
    return $this->connection->read(47);
  }

  public function getLedState() {
    $data = $this->readLedState();
    $json = json_decode($data);
    return $json->data;
  }

  public function run($effect, $color = COLOR_UNDEF, $argument = -1) {
    $this->connection->write("$effect,$color,$argument");
  }

  public function setText($text) {
    global $username, $password;
    $context = stream_context_create(array(
      "http" => array(
        "header"  => "Authorization: Basic " . base64_encode("$username:$password")
      )
    ));
    file_get_contents("http://$this->host/data/put/text/" . str_replace(" ", "_", $text), false, $context);
  }
}
