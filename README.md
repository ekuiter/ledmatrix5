## 5x5 LED matrix

This is an Arduino Yun project I made some time ago.

### Getting started

The Arduino sketch is located in the folder ``ledmatrix5``, the code is commented in the important places.

The Linino PHP script is located in the ```php``` folder. You can use it to control the LED matrix remotely,
but it is optional. If you decide to use it, put it in the Yun's ``/www/`` directory and install the following
packages with ``opkg install``:
```
php5
php5-cgi
php5-mod-json
```

I found it useful to add ``openssh-sftp-server`` as well to upload the ``php`` folder quickly with SFTP,
but that's up to you.

You will need to rename ``config.inc.template.php`` to ``config.inc.php`` and insert your Yun password.

### Plugins

If you want to use the Last.fm plugin you need to call ``crontab -e`` and insert:
```
* * * * * php-cgi /path/to/plugins/lastfm.php > /dev/null
```
You also have to execute this in your terminal:
```
/etc/init.d/cron enable
etc/init.d/cron start
```

Feel free to ask questions about this project: [info@elias-kuiter.de](mailto:info@elias-kuiter.de)