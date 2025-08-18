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
            uint16_t mConnectionTimeout = 30; // seconds
            bool mOnlineChecking = true;
            uint16_t mOnlineCheckingMinutes = 5;
            bool mDHCP_Client = true;

            String mSSID = "My Network";
            String mPassphrase = "NoConnection#123!";
            String mSoftAP_SSID = "DeviceIQ";
            String mSoftAP_Password = "DeviceIQ";
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
            inline void OnlineCheckingMinutes(uint16_t value) { mOnlineCheckingMinutes = value; };
            inline uint16_t OnlineCheckingMinutes() { return mOnlineCheckingMinutes; };

            inline void DHCP_Client(bool value) { mDHCP_Client = value; }
            inline bool DHCP_Client() { return mDHCP_Client; }

            inline void SSID(String value) { mSSID = value.substring(0, 32); }
            inline String SSID() { return (ConnectionMode() == APMode::SoftAP) ? mSoftAP_SSID : mSSID; }

            inline void Password(String value) { mPassword = value.substring(0, 63); }
            inline String Password() { return (ConnectionMode() == APMode::SoftAP) ? mSoftAP_Password : mPassword; }

            inline void SoftAP_SSID(String value) { mSoftAP_SSID = value.substring(0, 32); }
            inline String SoftAP_SSID() { return mSoftAP_SSID; }

            inline void SoftAP_Password(String value) { mSoftAP_Password = value.substring(0, 63); }
            inline String SoftAP_Password() { return mSoftAP_Password; }

            inline void Hostname(String value) { mHostname = value.substring(0, 32); }
            inline String Hostname() { return mHostname; }

            inline void IP(IPAddress value) { mIP = value; }
            inline IPAddress IP() { return mIP; }

            inline void Subnet(IPAddress value) { mSubnet = value; }
            inline IPAddress Subnet() { return mSubnet; }

            inline void Gateway(IPAddress value) { mGateway = value; }
            inline IPAddress Gateway() { return mGateway; }

            inline void DNS_Server(uint8_t index, IPAddress value) { if (index < 2) mDNS_Server[index] = value; }
            inline IPAddress DNS_Server(uint8_t index) { if (index < 2) return mDNS_Server[index]; return IPAddress(); }

            inline int8_t RSSI() { return WiFi.RSSI(); }

            APMode ConnectionMode();

            inline String MAC_Address() { return WiFi.macAddress(); }

            inline void OnModeChanged(callback_t callback) { mOnModeChanged = callback; }

            void Control();
    };
}