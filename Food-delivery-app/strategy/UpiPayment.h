#pragma once
#include<iostream>
#include "PaymentStrategy.h"
using namespace std;

class UpiPayment : public PaymentStrategy {
private:
    string mobile;
public:
    UpiPayment(const string& mob){
        this->mobile = mob;
    }
    void pay(double amount) override {
        cout<<"Paid ₹"<<amount<<" via Upi"<<endl;
    }
};