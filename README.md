##5x5 LED matrix

This is an Arduino Yun project I made some time ago.

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

You will need to rename ``auth.inc.template.php`` to ``auth.inc.php`` and insert your Yun password.

Feel free to ask questions about this project: [info@elias-kuiter.de](mailto:info@elias-kuiter.de)