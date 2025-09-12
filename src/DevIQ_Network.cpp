#include "DevIQ_Network.h"

using namespace DeviceIQ_Network;

Network::Network() {
    mUpdateMode = new DeviceIQ_DateTime::Timer(1000);
    mOnlineCheckingTimer = new DeviceIQ_DateTime::Timer(mOnlineCheckingMinutes * 60000);

    mUpdateMode->OnTimeout([&] { APMode t = ConnectionMode(); if (mFormerConnectionMode != t) { if (mOnModeChanged) mOnModeChanged(); mFormerConnectionMode = t; }});
    mOnlineCheckingTimer->OnTimeout([&] { if (ConnectionMode() != APMode::WifiClient) Connect(); });

    mUpdateMode->Start();
}

APMode Network::ConnectionMode() {
    switch (WiFi.getMode()) {
        case WIFI_MODE_STA: {
            if (WiFi.status() == WL_CONNECTED)
                return APMode::WifiClient;
            else
                return APMode::Offline;
        } break;
        case WIFI_MODE_AP: {
            return APMode::SoftAP;
        } break;
        default: {
            return APMode::Offline;
        } break;
    }
}

APMode Network::Connect() {
    if (mDHCP_Client) {
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    } else {
        WiFi.config(mIP_Address, mGateway, mNetmask, mDNS_Server[0], mDNS_Server[1]);
    }

    WiFi.mode(WIFI_STA);
    delay(50);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);
    WiFi.setSleep(false);
    WiFi.setHostname(mHostname.c_str());

    WiFi.begin(mSSID, mPassphrase);

    const uint32_t firstAttemptSec = max<uint16_t>(mConnectionTimeout, 25);
    for (uint16_t s = 0; s < firstAttemptSec && WiFi.status() != WL_CONNECTED; ++s) {
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_AP);
        delay(50);

        IPAddress apIp(192, 168, 77, 1);
        IPAddress apGw(192, 168, 77, 1);
        IPAddress apMask(255, 255, 255, 0);
        WiFi.softAPConfig(apIp, apGw, apMask);

        WiFi.softAPsetHostname(mHostname.c_str());

        const char* apSsid  = SoftAP_SSID().c_str();
        const char* apPass  = "";
        WiFi.softAP(apSsid, apPass);
    }

    if (mOnlineChecking && (mOnlineCheckingMinutes > 0)) {
        mOnlineCheckingTimer->SetTimeout(mOnlineCheckingMinutes * 60000UL);
        mOnlineCheckingTimer->Start();
    }

    return ConnectionMode();
}

void Network::Control() {
    mUpdateMode->Control();
    mOnlineCheckingTimer->Control();
}