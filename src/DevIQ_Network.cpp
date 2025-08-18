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
    if (mDHCP_Client == false) {
        WiFi.config(mIP_Address, mGateway, mNetmask, mDNS_Server[0], mDNS_Server[1]);
        WiFi.softAPConfig(mIP_Address, mIP_Address, mNetmask);
    }

    WiFi.setHostname(mHostname.c_str());
    WiFi.softAPsetHostname(mHostname.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(mSSID, mPassphrase);
    for (uint16_t timeoutCounter = 0; timeoutCounter < mConnectionTimeout && WiFi.status() != WL_CONNECTED; timeoutCounter++) delay(1000);

    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(SoftAP_SSID(), SoftAP_SSID());
    }

    if ((mOnlineChecking) && (mOnlineCheckingMinutes > 0)) {
        mOnlineCheckingTimer->SetTimeout(mOnlineCheckingMinutes * 60000);
        mOnlineCheckingTimer->Start();
    }

    return ConnectionMode();
}

void Network::Control() {
    mUpdateMode->Control();
    mOnlineCheckingTimer->Control();
}