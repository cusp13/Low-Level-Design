#pragma once
#include <iostream>
#include "../models/Playlist.hpp";
#include "PlayStrategy.hpp";

class CustomQueueStrategy : public PlayStrategy
{
private:
    Playlist *currentPlaylist;
    int currentIndex;
    queue<Song *> nextQueue;
    stack<Song *> prevStack;

    Song *nextSequential()
    {
        if (currentPlaylist->getSize() == 0)
        {
            throw runtime_error("Playlist is empty");
        }
        currentIndex = currentIndex + 1;
        return currentPlaylist->getSongs()[currentIndex];
    }
    Song *previousSequential()
    {
        if (currentPlaylist->getSize() == 0)
        {
            throw runtime_error("Playlist is empty");
        }
        currentIndex = currentIndex - 1;
        return currentPlaylist->getSongs()[currentIndex];
    }

public:
    CustomQueueStrategy()
    {
        currentPlaylist = nullptr;
        currentIndex = -1;
    }
    void setPlayList(Playlist *playlist) override
    {
        currentPlaylist = playlist;
        currentIndex = -1;
        while (!nextQueue.empty())
        {
            nextQueue.pop();
        }
        while (!prevStack.empty())
        {
            prevStack.pop();
        }
    }
    bool hasNext() override
    {
        return (currentIndex + 1 < currentPlaylist->getSize());
    }
    // when someone want explicitly do next.
    Song *next() override
    {
        if (!currentPlaylist || currentPlaylist->getSize() == 0)
        {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }
        if (!nextQueue.empty())
        {
            Song *s = nextQueue.front();
            nextQueue.pop();
            prevStack.push(s);
            auto& list = currentPlaylist->getSongs();
            for(int i = 0; i < (int)list.size(); i++){
                if(list[i] == s){
                    currentIndex = i;
                    break;
                }
            }
            return s;
        }
        return nextSequential();
    }
    bool hasPrev() override {
        return (currentIndex-1 > 0);
    }
    Song* prev() override {
        if(!currentPlaylist || currentPlaylist->getSize() == 0){
            throw runtime_error("No playlist loaded or exists");
        }
        if (!prevStack.empty()) {
            Song* s = prevStack.top();
            prevStack.pop();
            auto& list = currentPlaylist->getSongs();
            for (int i = 0; i < (int)list.size(); ++i) {
                if (list[i] == s) {
                    currentIndex = i;
                    break;
                }
            }
            return s;
        }
        return previousSequential();
    }
    void addToNext(Song* song) override {
        if(!song){
            throw runtime_error("Cannot queue null song");
        }
        nextQueue.push(song);
    }
};
