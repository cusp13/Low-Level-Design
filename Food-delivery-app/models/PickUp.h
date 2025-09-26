#pragma once
#include<iostream>
#include<string>
#include<vector>
#include "Order.h"

class PickUpOrder : public Order {
private:
    string restaurantAddress;
public:
    PickUpOrder() {
        restaurantAddress = "";
    }

    string getType() const override {
        return "PickUpOrder";
    }

    void setRestaurantAddress(const string& addr){
        restaurantAddress = addr;
    }
    
    string getRestaurantAddress() {
       return restaurantAddress;
    }
};