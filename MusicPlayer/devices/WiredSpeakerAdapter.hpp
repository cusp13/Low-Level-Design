#pragma once
#include "../externals/WiredSpeakerAPI.hpp";
#include "IAudioOutputDevice.hpp";
#include "../models/Song.hpp";

using namespace std;

class WiredSpeakerAdapter: public IAudioOutputDevice {
private: 
    WiredSpeakerAPI* wiredApi;
public:
    WiredSpeakerAdapter(WiredSpeakerAPI* api){
        wiredApi = api;
    }
    void playAudio(Song* song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        wiredApi -> playSoundViaWired(payload);
    }
};