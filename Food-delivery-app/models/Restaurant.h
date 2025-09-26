#pragma once
#include<string>
#include<vector>
#include<iostream>
#include "MenuItem.h"

class Restaurant {
private:
   static int nextRestaurantId;
   int restaurantId;
   string restaurantName;
   string restaurantAddress;
   string restaurantPhone;
   vector<MenuItem> menu;
public:
    Restaurant(string restaurantName, string restaurantAddress, string restaurantPhone){
        restaurantId = nextRestaurantId++;
        this->restaurantName = restaurantName;
        this->restaurantAddress = restaurantAddress;
        this->restaurantPhone = restaurantPhone;
    }
    void addMenuItem(MenuItem item){
        menu.push_back(item);
    }
    vector<MenuItem> getMenu() const {
        return menu;
    }
    string getRestaurantName() const {
        return restaurantName;
    }
    string getRestaurantAddress() const {
        return restaurantAddress;
    }
    string getRestaurantPhone() const {
        return restaurantPhone;
    }
    int getRestaurantId() const {
        return restaurantId;
    }

    void setRestaurantName(string name) {
        restaurantName = name;
    }
    void setRestaurantAddress(string address) {
        restaurantAddress = address;
    }
    void setRestaurantPhone(string phone) {
        restaurantPhone = phone;
    }
};

int Restaurant::nextRestaurantId = 0;