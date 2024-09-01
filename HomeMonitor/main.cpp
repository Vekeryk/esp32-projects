#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h" // Adafruit DHT library

const char *ssid = "WIFI_NAME";
const char *password = "WIFI_PASSWORD";
const char *endpoint = "http://YOUR_SERVER_ADDRESS/api/measurements";

String apiKey = "YOUR_API_KEY";

const int maxRetries = 15;
const int blueLedPin = 27;

DHT dht(13, DHT11); // GPIO 13

void setup()
{
    pinMode(blueLedPin, OUTPUT);
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

        dht.begin();
        delay(2000);

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        Serial.print(temperature);
        Serial.print(" ");
        Serial.println(humidity);

        HTTPClient http;
        http.begin(endpoint);

        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-API-KEY", apiKey);

        String jsonData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";

        int httpResponseCode = http.POST(jsonData);
        Serial.println(httpResponseCode);

        if (httpResponseCode != 201)
        {
            Serial.println("Error on sending POST");
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

    Serial.println("Enter deep sleep mode...");
    esp_sleep_enable_timer_wakeup(30 * 60 * 1000000); // 30 minutes
    esp_deep_sleep_start();
}

void loop()
{
}
