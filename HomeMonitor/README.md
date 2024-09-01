# HomeMonitor

**HomeMonitor** is an ESP32-based project designed to collect temperature, humidity and other data and send this data to a server endpoint via Wi-Fi.

The device operates in low-power mode by entering deep sleep between data transmissions.

## Features

- **Wi-Fi Connectivity:** Connects to a Wi-Fi network to send data to a specified server endpoint.
- **Temperature and Humidity Monitoring:** Uses a DHT11 sensor to measure temperature and humidity.
- **HTTP POST Requests:** Sends the collected data to a server endpoint using an HTTP POST request.
- **Low Power Mode:** After sending data, the device enters deep sleep for 30 minutes to conserve battery.
