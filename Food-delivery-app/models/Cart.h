#pragma once
#include<iostream>
#include<vector>
#include <string>
#include "MenuItem.h"
#include "Restaurant.h"
using namespace std;

class Cart {
private:
    Restaurant* restaurant; 
    vector<MenuItem> items;
public:
    Cart() {
        restaurant = nullptr;
    }
    void addItem(const MenuItem& item) {
        if(restaurant == nullptr) {
            cout<<"Add a restaurants before items";
            return;
        }
        items.push_back(item);
    }

    double getTotalCost() {
        int totalPrice = 0;
        for(const auto& it: items){
            totalPrice += it.getPrice();
        }
        return totalPrice;
    }
    bool isEmpty() {
        return items.size() == 0;
    }

    void clear() {
        items.clear();
        restaurant = nullptr;
    }

    void setRestaurants(Restaurant* r) {
        restaurant = r;
    }

    Restaurant* getRestaurant() const {
        return restaurant;
    }

    const vector<MenuItem>& getItems() const {
        return items;
    }
};
