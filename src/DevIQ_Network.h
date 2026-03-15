#ifndef DevIQ_Network_h
#define DevIQ_Network_h

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <DevIQ_DateTime.h>

namespace DeviceIQ_Network {
    using callback_t = std::function<void()>;
    enum APMode { Offline, WifiClient, SoftAP };

    class Network {
        private:
            uint16_t mConnectionTimeout = 30;
            bool mOnlineChecking = true;
            uint16_t mOnlineCheckingTimeout = 5 * 60;
            bool mDHCP_Client = true;

            String mSSID = "My Network";
            String mPassphrase = "NoConnection#123!";
            String mSoftAP_SSID = "DeviceIQ";
            String mSoftAP_Password = "";
            String mHostname = "deviceiq";

            IPAddress mIP_Address;
            IPAddress mNetmask;
            IPAddress mGateway;
            IPAddress mDNS_Server[2] = { IPAddress(8, 8, 8, 8), IPAddress(8, 8, 4, 4) };

            APMode mFormerConnectionMode = APMode::Offline;
            callback_t mOnModeChanged;

            DeviceIQ_DateTime::Timer* mUpdateMode = nullptr;
            DeviceIQ_DateTime::Timer* mOnlineCheckingTimer = nullptr;
        public:
            Network();

            APMode Connect();

            inline void ConnectionTimeout(uint16_t value) { mConnectionTimeout = value; };
            inline uint16_t ConnectionTimeout() { return mConnectionTimeout; };

            inline void OnlineChecking(bool value) { mOnlineChecking = value; };
            inline bool OnlineChecking() { return mOnlineChecking; };
            inline void OnlineCheckingTimeout(uint16_t value) { mOnlineCheckingTimeout = value; };
            inline uint16_t OnlineCheckingTimeout() { return mOnlineCheckingTimeout; };

            inline void DHCP_Client(bool value) { mDHCP_Client = value; }
            inline bool DHCP_Client() { return mDHCP_Client; }

            inline void SSID(String value) { mSSID = value.substring(0, 32); }
            inline String SSID() { return (ConnectionMode() == APMode::SoftAP) ? mSoftAP_SSID : mSSID; }

            inline void Passphrase(String value) { mPassphrase = value.substring(0, 63); }
            inline String Passphrase() { return (ConnectionMode() == APMode::SoftAP) ? mSoftAP_Password : mPassphrase; }

            inline void SoftAP_SSID(String value) { mSoftAP_SSID = value.substring(0, 32); }
            inline String SoftAP_SSID() { return mSoftAP_SSID; }

            inline void SoftAP_Password(String value) { mSoftAP_Password = value.substring(0, 63); }
            inline String SoftAP_Password() { return mSoftAP_Password; }

            inline void Hostname(String value) { mHostname = value.substring(0, 32); }
            inline String Hostname() { return mHostname; }

            inline void IP_Address(String value) { mIP_Address.fromString(value.c_str()); }
            inline void IP_Address(IPAddress value) { mIP_Address = value; }
            inline IPAddress IP_Address() { return mIP_Address; }

            inline void Netmask(String value) { mNetmask.fromString(value.c_str()); }
            inline void Netmask(IPAddress value) { mNetmask = value; }
            inline void Netmask(uint8_t cidr) { uint32_t subnetMaskValue = 0xFFFFFFFF << (32 - cidr); mNetmask = IPAddress(subnetMaskValue); }
            inline IPAddress Netmask() { return mNetmask; }

            inline void Gateway(String value) { mGateway.fromString(value.c_str()); }
            inline void Gateway(IPAddress value) { mGateway = value; }
            inline IPAddress Gateway() { return mGateway; }

            inline void DNS_Server(uint8_t index, IPAddress value) { if (index < 2) mDNS_Server[index] = value; }
            inline IPAddress DNS_Server(uint8_t index) { if (index < 2) return mDNS_Server[index]; }

            inline int8_t RSSI() { return WiFi.RSSI(); }

            APMode ConnectionMode();

            inline String MAC_Address() { return WiFi.macAddress(); }

            inline void OnModeChanged(callback_t callback) { mOnModeChanged = callback; }

            void Control();
    };
}

#endif