#pragma once
#include<iostream>
#include<string>
#include<vector>
#include "Order.h"
using namespace std;

class DeliveryOrder : public Order {
private:
    string userAddress;
public:
    DeliveryOrder(){
        userAddress = "";
    }

    string getType() const override {
        return "Delivery";
    }

    void setUserAddress(const string& addr) {
        userAddress = addr;
    }

    string getUserAddress() const {
        return userAddress;
    }
};