#include<iostream>
using namespace std;

class Burger {
public:
    virtual void prepare() = 0;
    virtual ~Burger() {};
};

class SimpleBurger : public Burger {
public:
    void prepare() override {
         cout<<"Serving simple burger"<<endl;      
    }
};

class PremiumBurger : public Burger {
public:
    void prepare() override {
        cout<<"Serving premium burger"<<endl;
    }
};

class BurgerFactory {
public:
    Burger* createBurger(string type){
        if(type == "simple"){
            return new SimpleBurger();
        } else if(type == "premium") {
            return new PremiumBurger();
        } else {
            cout<<"invalid burger: ";
            return nullptr;
        }
    }
};

int main() {
    string type = "pre";
    BurgerFactory* burger = new BurgerFactory();
    Burger* myBurger = burger->createBurger(type);
    
    myBurger->prepare();
    
    return 0;
};
