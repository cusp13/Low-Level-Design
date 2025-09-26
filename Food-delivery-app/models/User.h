#pragma once
#include<iostream>
#include<string>
#include "Cart.h"
using namespace std;

class User {
private:
    int userId;
    string name;
    string address;
    Cart* cart;
public:
    User(const string& name, const string& address, int userId) {
        this->userId = userId;
        this->name = name;
        this->address = address;
        cart = new Cart();
    }
    ~User() {
        delete cart;
    }
    // getter and setter
    void setUserId(int id) {
        userId = id;
    }
    int getUserId() {
        return userId;
    }
    void setName(const string& n) {
        name = n;
    }
    string getName() const {
        return name;
    }
    void setAddress(const string& add) {
        address = add;
    }
    string getAddress() const {
        return address;
    }
    Cart* getCart() const {
        return cart;
    }
};