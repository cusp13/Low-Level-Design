#pragma once
#include<string>
#include<iostream>

using namespace std;

class WiredSpeakerAPI {
public:
    void playSoundViaWired(const string& data) {
        cout << "[Wired Speaker] Playing: " << data << "\n";
    }
};