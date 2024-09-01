#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "WIFI_NAME";
const char *password = "WIFI_PASSWORD";

const char *endpoint = "https://api.alerts.in.ua/v1/iot/active_air_raid_alerts/13.json?token=your_token";

const int blueLedPin = 27;

// RTC GPIO
const int redLedPin = 26;
const int greenLedPin = 25;

int maxRetries = 15;

void setup()
{
    pinMode(blueLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);

    // reset held pins
    gpio_hold_dis(GPIO_NUM_26); // redLedPin
    gpio_hold_dis(GPIO_NUM_25); // greenLedPin

    digitalWrite(blueLedPin, HIGH);

    Serial.begin(115200);

    WiFi.begin(ssid, password);
    int retryCount = 0;

    // handle WiFi connection
    while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries)
    {
        delay(500);
        Serial.println("Connecting to WiFi...");
        retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WiFi");

        // make API request
        HTTPClient http;
        http.begin(endpoint);
        int httpResponseCode = http.GET();
        Serial.println(httpResponseCode);

        if (httpResponseCode == 200)
        {
            String payload = http.getString();
            Serial.println(payload);

            if (payload == "\"N\"")
            {
                // no alarm, turn on green LED
                digitalWrite(greenLedPin, HIGH);
                digitalWrite(redLedPin, LOW);
                gpio_hold_en(GPIO_NUM_25); // enable hold for greenLedPin
            }
            else
            {
                // there is an alarm, turn on red LED
                digitalWrite(greenLedPin, LOW);
                digitalWrite(redLedPin, HIGH);
                gpio_hold_en(GPIO_NUM_26); // enable hold for redLedPin
            }
        }
        else
        {
            Serial.println("Error in HTTP request");
        }

        http.end();
    }
    else
    {
        Serial.println("Failed to connect to WiFi...");
    }

    // prepare to sleep
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.flush();

    // hold active LED and enter deep sleep mode
    gpio_deep_sleep_hold_en();
    esp_sleep_enable_timer_wakeup(90 * 1000000); // 90 seconds
    esp_deep_sleep_start();
}

void loop()
{
}