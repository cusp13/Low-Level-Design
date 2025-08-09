#pragma once
#include "../models/Song.hpp";
#include "../devices/IAudioOutputDevice.hpp";
using namespace std;

class AudioEngine {
private:
    Song* currentSong;
    bool songIsPaused;
public:
    AudioEngine(){
        currentSong = nullptr;
        songIsPaused = false;
    }
    string getCurrentSong() const {
        if(currentSong == nullptr){
            return "";
        }
        return currentSong->getTitle();
    }
    bool isPaused() const {
        return songIsPaused;
    }
    void play(IAudioOutputDevice* aod, Song* song) {
        if(song == nullptr){
            throw runtime_error("Can not play any song");
        }
        if(songIsPaused && song == currentSong){
            songIsPaused = false;
            cout<< "Resuming song: " << song->getTitle()<<endl;
            aod ->playAudio(song);
            return;
        }
        currentSong = song;
        songIsPaused = false;
        cout<<"Playing song: "<< song->getTitle()<<endl;
        aod->playAudio(song);
    }
    void pause(){
        if(currentSong == nullptr){
            throw runtime_error("No song is currently playing to play");
        }
        if(songIsPaused == false){
            throw runtime_error("Song is already paused");
        }
        songIsPaused = true;
        cout<<"Pausing song: " << currentSong->getTitle()<<endl;
    }
};
