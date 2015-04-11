<?php

class MailPlugin extends Plugin {

  function __construct($interval) {
    parent::__construct("mail", $interval);
  }

  public function fetchData() {
    return array("unread" => (int) `imapfilter -c ../plugins/mail_config.lua`);
  }

  public function isNewData($data) {
    return $data != $this->loadData();
  }

  public function isRelevantData($data) {
    return $data["unread"] > 0;
  }

  public function stringifyData($data) {
    if ($data["unread"] === 1)
      return "There is 1 unread mail in your inbox.";
    else
      return "There are $data[unread] unread mails in your inbox.";
  }

  public function displayData($data, $ledMatrix) {
    $ledMatrix->setText($data["unread"] . ($data["unread"] == 1 ? " mail" : " mails"));
    $ledMatrix->run(EFFECT_TEXT);
  }
}