#pragma once
#include<iostream>
#include "../devices/IAudioOutputDevice.hpp";
#include "../devices/BluetoothSpeakerAdapter.hpp";
#include "../devices/WiredSpeakerAdapter.hpp";
#include "../devices/HeadPhoneAdapter.hpp";
#include "../enums/DeviceType.hpp";

using namespace std;

class DeviceFactory {
public:
    static IAudioOutputDevice* createDevice(DeviceType deviceType) {
        if (deviceType == DeviceType::BLUETOOTH) {
            return new BluetoothSpeakerAdapter(new BluetoothSpeakerAPI());
        } else if (deviceType == DeviceType::WIRED) {
            return new WiredSpeakerAdapter(new WiredSpeakerAPI());
        } else {
            return new HeadPhoneAdapter(new HeadPhoneAPI());
        }
    }
};