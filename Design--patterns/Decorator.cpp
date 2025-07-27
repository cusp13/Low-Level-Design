#include <iostream>
#include <memory>
using namespace std;

// Base interface
class Coffee {
public:
    virtual string getDescription() = 0;
    virtual int getCost() = 0;
    virtual ~Coffee() {}
};

// Concrete component
class BasicCoffee: public Coffee {
public:
    string getDescription() override {
        return "Basic Coffee";
    }
    int getCost() override {
        return 50;
    }
};

// Decorator base
class CoffeeDecorator: public Coffee {
protected:
    shared_ptr<Coffee> coffee; // strong pointer
public:
// When someone creates a CoffeeDecorator and passes a shared_ptr<Coffee> to it, store that pointer in the member variable coffee.
    CoffeeDecorator(shared_ptr<Coffee> c) : coffee(c) {}
};

// Concrete Decorators
class MilkDecorator: public CoffeeDecorator {
public:
    MilkDecorator(shared_ptr<Coffee> c) : CoffeeDecorator(c) {}
    string getDescription() override {
        return coffee->getDescription() + ", Milk";
    }
    int getCost() override {
        return coffee->getCost() + 20;
    }
};

class SugarDecorator: public CoffeeDecorator {
public:
    SugarDecorator(shared_ptr<Coffee> c) : CoffeeDecorator(c) {}
    string getDescription() override {
        return coffee->getDescription() + ", Sugar";
    }
    int getCost() override {
        return coffee->getCost() + 10;
    }
};

int main() {
    shared_ptr<Coffee> myCoffee = make_shared<BasicCoffee>();
    myCoffee = make_shared<MilkDecorator>(myCoffee);
    myCoffee = make_shared<SugarDecorator>(myCoffee);

    cout << "Order: " << myCoffee->getDescription() << endl;
    cout << "Total cost: ₹" << myCoffee->getCost() << endl;
}
