#pragma once
#include<string>
#include<iostream>

using namespace std;

class HeadPhoneAPI {
public:
    void playHeadPhone(const string& data){
        cout<<"[Headphone] playing" << data <<endl;
    }
};