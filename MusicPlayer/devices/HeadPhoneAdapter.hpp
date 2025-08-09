#pragma once
#include "IAudioOutputDevice.hpp";
#include "../externals/HeadPhoneAPI.hpp";

using namespace std;

class HeadPhoneAdapter : public IAudioOutputDevice {
private:
    HeadPhoneAPI* headPhoneApi;
public:
    HeadPhoneAdapter(HeadPhoneAPI* api){
        headPhoneApi = api;
    }
    void playAudio(Song* song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        headPhoneApi -> playHeadPhone(payload); 
    }
};