## 5x5 LED matrix

This is an Arduino Yun project I made some time ago.

[![LED matrix front](https://raw.githubusercontent.com/ekuiter/ledmatrix5/img/front_small.jpg)](https://raw.githubusercontent.com/ekuiter/ledmatrix5/img/front.jpg)
[![LED matrix front](https://raw.githubusercontent.com/ekuiter/ledmatrix5/img/back_small.jpg)](https://raw.githubusercontent.com/ekuiter/ledmatrix5/img/back.jpg)

### Getting started

The Arduino sketch is located in the folder `ledmatrix5`, the code is commented in the important places.
To set it up, check out `ledmatrix5/ledmatrix5.ino` and adjust the settings to your needs.
You might want to adjust the LED map in `ledmatrix5/Leds.h` (line 103).

The Linino PHP script is located in the `php` folder. You can use it to control the LED matrix remotely,
but it is optional. If you decide to use it run the following in a SSH shell:
    
    opkg install php5 php5-cgi php5-mod-json
    uci set uhttpd.main.index_page=index.php
    uci commit 
    /etc/init.d/uhttpd restart

Copy the `php/led-matrix` folder to the Yun's `/www` folder - it contains the HTTP user interface.
Then copy the `php/led-matrix-data` folder to `/root` - this folder contains the private (and sensitive) PHP code and data.

I found it useful to run `opkg install openssh-sftp-server` as well to upload the `php/led-matrix` and `php/led-matrix-data`
folders quickly with SFTP, but that's up to you.

You will need to rename both `/www/led-matrix/config.inc.template.php` and `/root/led-matrix-data/include/config.inc.template.php`
to `config.inc.php` and, in the latter, insert your Yun's root password (and modify your plugin settings).

(You *can* install the PHP script to other directories than `/www` and `/root` (though it is not recommended).
To do this, you need to adjust the following files: `php/led-matrix/config.inc.php` and `init.d/led-matrix`.)

### Plugins

To use plugins you need to copy `init.d/led-matrix` to `/etc/init.d/`.
(I'll assume you copied the `led-matrix-data` folder to `/root/led-matrix-data`. If you did not, change the script accordingly.)
Then make it executable and set it up:

    chmod +x /etc/init.d/led-matrix
    /etc/init.d/led-matrix enable
    /etc/init.d/led-matrix start

You can add this to ``/etc/profile``:

    PATH=$PATH:/etc/init.d
    export PATH

in order to make the `led-matrix` command available everywhere, or just use `/etc/init.d/led-matrix` to start or stop the script.

### Available plugins

Make sure to adjust `/root/led-matrix-data/include/config.inc.php` to include your plugins of choice!

#### Last.fm

This plugin displays your recently played tracks on the matrix.
Insert your username in `/root/led-matrix-data/include/config.inc.php` and make sure your recent scrobbels on Last.fm are public.

#### IMAP mail

The mail plugin tracks how many unread mails are in your IMAP inbox.
Rename `/root/led-matrix-data/mail_config.template.lua` to `mail_config.lua` and insert your server credentials.
Run this in the `/root/led-matrix-data` directory:

    opkg install imapfilter
    imapfilter -c mail_config.lua

Then press `p` to add your mail server's certificate permanently.

Feel free to ask questions about this project: [info@elias-kuiter.de](mailto:info@elias-kuiter.de)