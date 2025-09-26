#pragma once
#include<iostream>
#include<string>
using namespace std;

class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() {};
};
