#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include "../models/Restaurant.h"
using namespace std;

class RestaurantManager {
private:
    vector<Restaurant*> restaurants;
    static RestaurantManager* instance;
    RestaurantManager() {
        cout<<"Restaurants manager constructor initiated"<<endl;
    }
public:
    static RestaurantManager* getInstance() {
        if(instance == nullptr){
            instance = new RestaurantManager();
        }
        return instance;
    }

    void addRestaurants(Restaurant* r){
        restaurants.push_back(r);
    }

    vector<Restaurant*> searchByLoc(string loc) {
        vector<Restaurant*>result;
        transform(loc.begin(), loc.end(), loc.begin(), ::tolower);
        for (auto r : restaurants) {
            string rl = r->getRestaurantAddress();
            transform(rl.begin(), rl.end(), rl.begin(), ::tolower);
            if (rl == loc) {
                result.push_back(r);
            }
        }
        return result;
    }
};

RestaurantManager* RestaurantManager ::instance = nullptr;  


