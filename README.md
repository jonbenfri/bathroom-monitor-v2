# Description

Live monitoring a bathroom door's open or shut status.

Remixed version of Raspberry Pi Zero W bathroom monitor: <https://github.com/jonbenfri/bathroom-monitor>

Vast improvement over power consumption for door monitoring device. Raspberry Pi Zero W lasted about 8 hours on 2600mAh rechargeable battery. ESP8266-based door monitoring device power consumption too low to measure using inline USB multimeter, may last months.

# Overview

* A node.js server (e.g. Raspberry Pi) connects clients using websockets (socket.io).
* A WeMos D1 mini (ESP8266 board) has a magnetic reed switch soldered to its pins.
* When the bathroom door is opened or closed, an ESP8266 interrupt is triggered, sending the door status via websockets to the nodejs server.
* The nodejs server emits a signal to all connected clients informing of the new door status (open or closed).
* When users browse to node.js server (e.g. http://serverip:8080), door status is polled and displayed on a web page.
* When door status changes, door status is sent via websockets to all connected clients. Web pages are updated.

# Requirements

1. An always-on device that can act as a server (e.g. Raspberry Pi: <https://www.raspberrypi.org/>)
1. WeMos D1 mini: <https://docs.wemos.cc/en/latest/d1/d1_mini.html>
1. Magnetic reed door/window switch

# Installation

## WeMos D1 Mini (ESP8266 board)

1. Clone this repository: `git clone https://github.com/jonbenfri/bathroom-monitor-v2`
1. Solder magnetic reed switch to pin D3 and GND.
1. Set up the WeMos D1 mini as follows: <https://www.wemos.cc/en/latest/tutorials/d1/get_started_with_arduino_d1.html>
1. Edit the sketch `bathroom-monitor-v2/wemos-d1-mini-sketch/wemos-d1-mini-sketch.ino` with WiFi SSID and PASS information.
1. Upload the sketch `wemos-d1-mini-sketch.ino` to the D1 Mini.

## Node.js server

1. Log in to device that will run node server
1. Install node.js: <https://nodejs.org/en/>
1. Install bathroom monitor v2 node server:
    ```
    # Clone this repository
    git clone https://github.com/jonbenfri/bathroom-monitor-v2
    # Change directory
    cd bathroom-monitor-v2/node-server
    # Install bathroom monitor v2 node server
    npm install . # Don't forget the dot
    ```
1. Run: `node bathroom-monitor.js`
1. Note IP address or hostname:
    ```
    # IP address for Raspberry Pi:
    ip a | grep "inet "
    # Hostname for Raspberry Pi:
    cat /etc/hostname
    ```
1. Access node server from browser: <http://serverip:8080> (For Raspberry Pi using hostname: <http:hostname.local:8080>)

Note: If server reboots, may need to physically reset ESP8266 as websockets connection may stall.

### Useful functions

Run with pm2:
```
npm install pm2
# First time running:
pm2 start bathroom-monitor.js
# After second time running:
pm2 start bathroom-monitor
```

Check logs:
```
pm2 logs bathroom-monitor
```

Run on startup:
```
pm2 save
sudo env PATH=$PATH:/usr/local/bin pm2 startup systemd -u pi --hp /home/pi
```

# Licensing

All original work is released under GPLv3: https://www.gnu.org/licenses/gpl-3.0.en.html

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

Copyright 2020 Jonathan Friedman
