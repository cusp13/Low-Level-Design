#pragma once
#include<iostream>
#include<string>
#include "../models/Order.h"
#include "../models/Cart.h"
#include "../models/MenuItem.h"
#include "../models/Restaurant.h"
#include "../strategy/PaymentStrategy.h"
#include "../models/User.h"
using namespace std;

class OrderFactory {
public:
    virtual Order* createOrder(User* user, Cart* cart, Restaurant* restaurant, const vector<MenuItem>& menuItems,
        PaymentStrategy* paymentStrategy, double totalCost, const string& orderType) = 0;
    virtual ~OrderFactory() {};
};
