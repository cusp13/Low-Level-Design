#pragma once;
#include "IAudioOutputDevice.hpp";
#include "../externals/BluetoothSpeakerAPI.hpp";

using namespace std;

class BluetoothSpeakerAdapter: public IAudioOutputDevice {
private:
    BluetoothSpeakerAPI* bluetoothApi;
public:
    BluetoothSpeakerAdapter(BluetoothSpeakerAPI *api){
        bluetoothApi = api;
    }
    void playAudio(Song* song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        bluetoothApi->playSoundViaBluetooth(payload);
    }
};


