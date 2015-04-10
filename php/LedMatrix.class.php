<?php

require_once "Connection.class.php";

class LedMatrix {
  private $connection;

  public function __construct($host, $port) {
    $this->connection = new Connection($host, $port);
    $this->connection->connect();
  }
  
  private function readState() {
    return $this->connection->read(47);
  }

  public function getState() {
    $data = $this->readState();
    $json = json_decode($data);
    return $json->data;
  }

  public function run($effect, $color = NULL) {
    if ($color === NULL)
      $this->connection->write("$effect");
    else
      $this->connection->write("$effect,$color");
  }
}
