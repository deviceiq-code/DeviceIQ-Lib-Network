# DeviceIQ Lib Network

`DeviceIQ Lib Network` is a network management library for ESP32, providing Wi-Fi STA/AP switching, DHCP/static IP configuration, hostname control, mDNS support, and periodic online checking.

It simplifies handling Wi-Fi connections in automation and IoT projects, automatically falling back to SoftAP mode if STA mode fails, while offering event callbacks for connection mode changes.

## Features

-   Automatic Wi-Fi Station connection with fallback to SoftAP.
-   Support for DHCP or static IP configuration.
-   Hostname and mDNS service support.
-   Periodic online checking with automatic reconnection.
-   Callback support for connection mode changes.
-   Simple API for querying status and RSSI.
-   Uses `DeviceIQ_DateTime::Timer` for internal timing.

## Usage Example

``` cpp
#include <DevIQ_Network.h>

using namespace DeviceIQ_Network;

Network net;

void setup() {
    Serial.begin(115200);
    delay(1000);

    net.SSID("YourSSID");
    net.Password("YourPassword");
    net.Hostname("deviceiq-test");

    net.OnModeChanged([] {
        Serial.print("Mode changed to: ");
        switch (net.ConnectionMode()) {
            case APMode::Offline:   Serial.println("Offline"); break;
            case APMode::WifiClient: Serial.println("Wi-Fi Client"); break;
            case APMode::SoftAP:    Serial.println("SoftAP"); break;
        }
    });

    net.Connect();
}

void loop() {
    net.Control();
}
```

## API Summary

### Main Classes

  -----------------------------------------------------------------------
  Class                          Description
  ------------------------------ ----------------------------------------
  `Network`                      Manages Wi-Fi Station/SoftAP modes,
                                 connection checking, hostname, and IP
                                 configuration.

  -----------------------------------------------------------------------

### Key Methods

  ------------------------------------------------------------------------------------------
  Method                                         Description
  ---------------------------------------------- -------------------------------------------
  `Connect()`                                    Attempts STA connection; falls back to
                                                 SoftAP if unavailable.

  `Control()`                                    Must be called in loop to update timers and
                                                 check connectivity.

  `ConnectionMode()`                             Returns current mode (`Offline`,
                                                 `WifiClient`, or `SoftAP`).

  `SSID(String)` / `Password(String)`            Set Wi-Fi credentials.

  `SoftAP_SSID(String)` /                        Set fallback SoftAP credentials.
  `SoftAP_Password(String)`                      

  `Hostname(String)`                             Define the device hostname.

  `DHCP_Client(bool)`                            Enable/disable DHCP (static IP otherwise).

  `IP() / Subnet() / Gateway() / DNS_Server()`   Configure static IP settings.

  `OnModeChanged(callback)`                      Register callback for connection mode
                                                 changes.

  `RSSI()`                                       Get current Wi-Fi signal strength.
  ------------------------------------------------------------------------------------------

## Notes

## Dependencies

-   [DeviceIQ Lib
    DateTime](https://github.com/deviceiq-code/DeviceIQ-Lib-DateTime)\
-   Core ESP32 libraries (`WiFi`, `ESPmDNS`) are required but come with
    the ESP32 Arduino core.

## License

This library is released under the MIT License.
