Remixed version of Raspberry Pi Zero W version: https://github.com/jonbenfri/bathroom-monitor

# Description

Live monitoring a bathroom door's open or shut status.

# Overview

A node.js server (e.g. Raspberry Pi) connects clients using websockets (socket.io).

A WeMos D1 mini (ESP8266 board) has a magnetic reed switch soldered to its pins.

When the bathroom door is opened or closed, an ESP8266 interrupt is triggered, sending the door status via websockets to the nodejs server.

The nodejs server emits a signal to all connected clients informing of the new door status (open or cosed).

When users browse to node.js server (e.g. localhost:8080), door status is polled and displayed on a web page.

When door status changes, door status is sent via websockets to all connected clients. Web pages are updated.

# Installation

## WeMos D1 Mini

Set up the WeMos D1 mini as follows:
    <https://www.wemos.cc/en/latest/tutorials/d1/get_started_with_arduino_d1.html>

Edit the sketch `wemos-d1-mini-sketch.ino` with WiFi SSID and PASS information.

Upload the sketch `wemos-d1-mini-sketch.ino` to the D1 Mini.

## Node.js server

Install node.js

Run server: node monitor.js

### Useful functions

Run with pm2:
```
pm2 start monitor.js
```

Check logs:
```
pm2 logs monitor
```

# Licensing


## Images

Images of Audrey Hepburn are licensed under the Pixabay License:

>License for Content – Pixabay License
>
>Content on Pixabay is made available to you on the following terms ("Pixabay License"). Under the Pixabay License you are granted an irrevocable, worldwide, non-exclusive and royalty free right to use, download, copy, modify or adapt the Content for commercial or non-commercial purposes. Attribution of the photographer, videographer, musician or Pixabay is not required but is always appreciated.
>
>The Pixabay License does not allow:
>
>    Sale or distribution of Content as digital Content or as digital wallpapers (such as on stock media websites);
>    Sale or distribution of Content e.g. as a posters, digital prints, music files or physical products, without adding any additional elements or otherwise adding value
>    Depiction of identifiable persons in an offensive, pornographic, obscene, immoral, defamatory or libelous way; or
>    Any suggestion that there is an endorsement of products and services by depicted persons, brands, vocalists and organisations, unless permission was granted.
>
>Please be aware that while all Content on Pixabay is free to use for commercial and non-commercial purposes, items in the Content, such as identifiable people, logos, brands, audio samples etc. may be subject to additional copyrights, property rights, privacy rights, trademarks etc. and may require the consent of a third party or the license of these rights - particularly for commercial applications. Pixabay does not represent or warrant that such consents or licenses have been obtained, and expressly disclaims any liability in this respect.
>
><https://pixabay.com/service/terms/#license>

## Code

All libraries and library examples are governed by their respective licenses.

All original work is released under GPLv3: https://www.gnu.org/licenses/gpl-3.0.en.html

Copyright 2020 Jonathan Friedman
