#pragma once
#include<iostream>
#include "../models/Playlist.hpp"
#include "PlayStrategy.hpp"

using namespace std;

class SequentialPlayStrategy : public PlayStrategy {
private:
    Playlist* currentPlaylist;
    int currentIndex;
public:
    SequentialPlayStrategy() {
        currentPlaylist = nullptr;
        currentIndex = -1;
    }

    void setPlayList(Playlist* playlist) override {
        currentPlaylist = playlist;
        currentIndex = -1;
    }

    bool hasNext() override {
        return ((currentIndex + 1) < currentPlaylist->getSize());
    }

    Song* next() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }
        currentIndex = currentIndex + 1;
        return currentPlaylist->getSongs()[currentIndex];
    }

    bool hasPrev() override {
        return (currentIndex - 1 > 0);
    }

    Song* prev() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }
        currentIndex = currentIndex - 1;
        return currentPlaylist->getSongs()[currentIndex];
    }
};