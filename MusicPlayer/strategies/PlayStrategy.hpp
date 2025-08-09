#pragma once
#include<iostream>
#include "../models/Playlist.hpp";
#include "../models/Song.hpp";

using namespace std;

class PlayStrategy {
public:
    virtual ~PlayStrategy() {};
    virtual void setPlayList(Playlist* playlist) = 0;
    virtual Song* next() = 0;
    virtual bool hasNext() = 0;
    virtual Song* prev() = 0;
    virtual bool hasPrev() = 0;
    virtual void addToNext(Song* song){};
};
