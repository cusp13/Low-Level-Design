#pragma once
#include<iostream>
#include<vector>
#include "../models/Order.h"
#include "../models/MenuItem.h"
using namespace std;

class NotificationService {
public:
    static void notify(Order* ord) {
        cout << ord->getType() << " Order placed!" << endl;
        cout << "Order id: " << ord->getOrderId() << endl;
        cout << "Ordered by: " << ord->getUser()->getName() << endl;
        cout << "Ordered from: " << ord->getRestaurant()->getRestaurantName() << endl;

        const vector<MenuItem>& items = ord->getItems();
        for (const auto& item : items) {
            cout << "   - " << item.getName() << " (₹" << item.getPrice() << ")" << endl;
        }

        cout << "Total: ₹" << ord->getTotal() << endl;
        cout << "Scheduled For: " << ord->getScheduled() << endl;
        cout << "Payment: Done" << endl;
        cout << "---------------------------------------------" << endl;
    }
};
