# Urequests/usage.py
This folder contains a modifyed version of `urequests` called `piwrequests`.  
This can be used together with the `esp_socket` to make regular http(s) requests.  
Check the "usage.py" file, on how to use the `piwrequests` module.



<br>

# pppwhttp.py
This is the old version of making servers and clients.  
It has been fittet to the updated version of `Pico-Wireless` and should still work.  
This code can be used to make a WebServer.  
If regular GET/POST requests are needed, then just use the [Urequests](Urequests/usage.py) example.  


<br>

# demo_http.py
This is a demo web server, using the [pppwhttp](#pppwhttppy) module.  
This can also be modifyed, if you want to make your own web server.  

<br>

# plasma_ws2812_http.py
This example uses the Plasma WS2812 LED library to drive a string of LEDs.  
You should wire your LEDs to VBUS/GND and connect the data pin to pin 27 (unused by Pico Wireless).  