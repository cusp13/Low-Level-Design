#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

class MenuItem {
private:
    string code;
    string name;
    int price;
public:
    MenuItem(string c, string n, int p) {
        this->code = c;
        this->name = n;
        this->price = p;
    }
    string getCode() const {
        return code;
    }
    void setCode(string c) {
        code = c;
    }
    string getName() const {
        return name;
    }
    void setName(string n) {
        name = n;
    }
    int getPrice() const {
        return price;
    }
    void setPrice(int p) {
        price = p;
    }
};