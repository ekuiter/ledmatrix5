## 5x5 LED matrix

This is an Arduino Yun project I made some time ago.

### Getting started

The Arduino sketch is located in the folder ``ledmatrix5``, the code is commented in the important places.

The Linino PHP script is located in the ```php``` folder. You can use it to control the LED matrix remotely,
but it is optional. If you decide to use it, put it in the Yun's ``/www/`` directory and run:
```
opkg install php5 php5-cgi php5-mod-json
uci set uhttpd.main.index_page=index.php
uci commit 
/etc/init.d/uhttpd restart
```

I found it useful to add ``openssh-sftp-server`` as well to upload the ``php`` folder quickly with SFTP,
but that's up to you.

You will need to rename ``config.inc.template.php`` to ``config.inc.php`` and insert your Yun password.

### Plugins

If you want to use plugins you need to copy ``init.d/led-matrix`` to ``/etc/init.d/``.
(I'll assume you installed the PHP script to ``/www/led-matrix``. If you did not, change the script accordingly.)
Then make it executable and set it up:
```
chmod +x /etc/init.d/led-matrix
/etc/init.d/led-matrix enable
/etc/init.d/led-matrix start
```
You can add this to ``/etc/profile``:
```
PATH=$PATH:/etc/init.d
export PATH
```
in order to make the ``led-matrix`` command available everywhere.

#### Last.fm

Insert your username in ``config.inc.php`` and make sure your recent scrobbels on Last.fm are public.

#### IMAP mail

Rename ``plugins/data/mail_config.template.lua`` to ``plugins/data/mail_config.lua`` and insert your server credentials.
Run this in the PHP script's root directory:
```
opkg install imapfilter
imapfilter -c plugins/data/mail_config.lua
```
Then press ``p`` to add your mail server's certificate permanently.

Feel free to ask questions about this project: [info@elias-kuiter.de](mailto:info@elias-kuiter.de)