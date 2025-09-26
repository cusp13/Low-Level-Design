#pragma once
#include<iostream>
#include<vector>
#include "../models/Order.h"
using namespace std;

class OrderManager {
private:
    vector<Order*> orders;
    static OrderManager* instance;
    OrderManager() {
        cout<<"Constructor invoked"<<endl;
    }
public:
    static OrderManager* getInstance() {
        if(instance==nullptr){
            instance = new OrderManager();
        }
        return instance;
    }

    void addOrder(Order* ord){
        orders.push_back(ord);
    }

    void listOrder() {
        cout<<"Printing all orders"<<endl;
        for(auto order: orders){
            cout<<order->getType()<<" ordered for "<< order->getUser()->getName()
            <<" | Total: " << order->getTotal()
            <<" | At: "<<order->getScheduled()<<endl;
        }
    }
};

OrderManager* OrderManager :: instance = nullptr;
