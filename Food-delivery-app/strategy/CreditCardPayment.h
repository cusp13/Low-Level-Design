#pragma once
#include<iostream>
#include<string>
#include "PaymentStrategy.h"
using namespace std;

class CreditCartPayment: public PaymentStartegy {
private:
    string cardNumber;
public:
    CreditCartPayment(const string& cardNo) {
        this->cardNumber = cardNo;
    }
    void pay(double amount) override {
        cout<<"Paid ₹" <<amount<<"via credit card"<<endl;
    }
};

