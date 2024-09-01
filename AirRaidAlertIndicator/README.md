# AirRaidAlertIndicator

**AirRaidAlertIndicator** is an ESP32-based project designed to monitor air raid alerts in your region and indicate the current status using LEDs.

The device connects to Wi-Fi, fetches the latest alert status from a specified API endpoint, and displays the result with a red or green LED.

## Features

- **Wi-Fi Connectivity:** Connects to a Wi-Fi network to fetch alert status.
- **API Integration:** Makes an HTTP request to the `alerts.in.ua` API to get the current air raid alert status.
- **LED Indicators:**
  - **Green LED:** Indicates no active air raid alert.
  - **Red LED:** Indicates an active air raid alert.
- **Low Power Mode:** After fetching the alert status, the device enters deep sleep to conserve battery.
