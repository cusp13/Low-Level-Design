#pragma once
#include<iostream>
#include "../models/Playlist.hpp";
#include "PlayStrategy.hpp";

using namespace std;

class RandomPlayStrategy: public PlayStrategy {
private:
    Playlist* currentPlaylist;
    vector<Song*>remainingSongs;
    stack<Song*> history;
public:
    RandomPlayStrategy(){
        currentPlaylist = nullptr;
        srand((unsigned)time(nullptr));
    }
    void setPlayList(Playlist* playlist) override {
        currentPlaylist = playlist;
        if(!currentPlaylist || currentPlaylist->getSize() == 0) return;

        remainingSongs = currentPlaylist->getSongs();
        history = stack<Song*>();
    }
    bool hasNext() override {
        return (currentPlaylist && remainingSongs.size() != 0);
    }

    Song* next() override {
        if(!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist  loaded  or playlist is empty");
        }
        if(remainingSongs.empty()){
            throw runtime_error("no song lets to play");
        }

        int idx = rand() % remainingSongs.size();
        Song* selectedSong = remainingSongs[idx];

        swap(remainingSongs[idx], remainingSongs.back());
        remainingSongs.pop_back();

        history.push(selectedSong);
        return selectedSong;
    }
    bool hasPrev() override {
        return history.size() > 0;
    }
    Song* prev() override {
        if(history.empty()){
            runtime_error("No song exist in history");
        }
        Song* song = history.top();
        history.pop();
        return song;
    }
};