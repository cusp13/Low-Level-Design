#pragma once
#include<iostream>
#include "../devices/IAudioOutputDevice.hpp";
#include "../enums/DeviceType.hpp";
#include "../factories/DeviceFactory.hpp";

using namespace std;

class DeviceManager {
private: 
    static DeviceManager* instance;
    IAudioOutputDevice* currentOutputDevice;
    DeviceManager() {
        currentOutputDevice = nullptr;
    }
public:
    static DeviceManager* getInstance(){
        if(instance == nullptr){
            instance = new DeviceManager();
        }
        return instance;
    }
    void connect(DeviceType deviceType){
        if(currentOutputDevice){
            delete currentOutputDevice;
        }
        currentOutputDevice = DeviceFactory::createDevice(deviceType);

        switch(deviceType){
            case DeviceType::BLUETOOTH:
            cout<<"Bluetooth Device connected\n";
            break;
            case DeviceType::HEADPHONES:
            cout<<"Headphone Device connected\n";
            break;
            case DeviceType::WIRED:
            cout<<"Wired Device connected\n";
        }
    }
    IAudioOutputDevice* getOutputDevice() {
        if(!currentOutputDevice){
            throw runtime_error("No device connected");
        }
        return currentOutputDevice;
    }
    bool hasOutputDevice(){
        return currentOutputDevice != nullptr;
    }
};

DeviceManager* DeviceManager:: instance = nullptr;
