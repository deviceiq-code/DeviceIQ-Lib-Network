#include <Arduino.h>
#include <DevIQ_Network.h>

using namespace DeviceIQ_Network;

Network net;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("DeviceIQ Network Basic Usage Example");

  // Set Wi-Fi credentials
  net.SSID("YourSSID");
  net.Password("YourPassword");

  // Optional: set hostname
  net.Hostname("deviceiq-test");

  // Register callback for mode changes
  net.OnModeChanged([] {
    Serial.print("Mode changed to: ");
    switch (net.ConnectionMode()) {
      case APMode::Offline:   Serial.println("Offline"); break;
      case APMode::WifiClient: Serial.println("Wi-Fi Client"); break;
      case APMode::SoftAP:    Serial.println("SoftAP"); break;
    }
  });

  // Try to connect
  APMode mode = net.Connect();
  Serial.print("Initial mode: ");
  switch (mode) {
    case APMode::Offline:   Serial.println("Offline"); break;
    case APMode::WifiClient: Serial.println("Wi-Fi Client"); break;
    case APMode::SoftAP:    Serial.println("SoftAP"); break;
  }
}

void loop() {
  // Allow timers and state checking to run
  net.Control();

  // Example: print RSSI every 5 seconds if connected
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    lastPrint = millis();
    if (net.ConnectionMode() == APMode::WifiClient) {
      Serial.print("RSSI: ");
      Serial.println(net.RSSI());
    }
  }
}