#pragma once
#include<iostream>
#include<string>
#include "OrderFactory.h"
#include "../models/DeliveryOrder.h"
#include "../models/PickUp.h"
#include "../utils/TimeUtils.h"
using namespace std;

class ScheduledOrderFactory : public OrderFactory {
private:
   string scheduleTime;
public:
   ScheduledOrderFactory(string scheduleTime){
    this->scheduleTime = scheduleTime;
   }
   Order* createOrder(User* user, Cart* cart, Restaurant* restaurant, const vector<MenuItem>& menuItems, 
        PaymentStrategy* paymentStrategy, double totalCost, const string& orderType) override {
        Order* order = nullptr;
        if(orderType == "Delivery") {
            auto deliveryOrder = new DeliveryOrder();
            deliveryOrder->setUserAddress(user->getAddress());
            order = deliveryOrder;
        } else if(orderType == "PickUpOrder") {
            auto pickUp = new PickUpOrder();
            pickUp->setRestaurantAddress(user->getAddress());
            order = pickUp;
        }
        order->setUser(user);
        order->setRestaurant(restaurant);
        order->setItems(menuItems);
        order->setPaymentStrategy(paymentStrategy);
        order->setScheduled(scheduleTime);
        order->setTotal(totalCost);

        return order;
    }
};