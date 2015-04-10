<?php

class Connection {
  private $host;
  private $port;
  private $socket;
  private $timeout = 400;

  public function __construct($host, $port) {
    $this->host = $host;
    $this->port = $port;
  }

  public function connect() {
    $socket = @pfsockopen($this->host, $this->port, $errno, $errstr);
    if (!$socket || $errno !== 0)
      throw new Exception("Connection Error #$errno: $errstr");
    $this->socket = $socket;
    @stream_set_timeout($this->socket, 0, $this->timeout * 1000);
  } 

  public function write($cmd) {
    if (!$this->socket)
      throw new Exception("Not yet connected");
    @fputs($this->socket, "$cmd\n");
  }

  public function read($length) {
    if (!$this->socket)
      throw new Exception("Not yet connected");
    return fread($this->socket, $length);
  }

  public function __destruct() {
    if ($this->socket)
      @fclose($this->socket);
  }
}
