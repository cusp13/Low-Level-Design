#include<iostream>
#include<string>
using namespace std;

// we can intiatite any abstract class only derived class can be insitiatited.
class Engine {
public:
    virtual void start() = 0;
    virtual ~Engine(){};
};

class PetrolEngine : public Engine {
public:
    void start() override {
        cout<<" with petrol"<<endl;
    }
};

class DieselEngine : public Engine {
public:
    void start() override {
        cout<<" with diesel"<<endl;
    }
};

class Car {
// if you want to directly access anything member then use protected member 
// not private it can only be access by getter and setter
// protected:
//    Engine* engine;
// for priavte member function need getter funtion to access
private: 
    Engine* engine; //'has-a' relation with Engine class
public:
    Car(Engine* e) {
        engine  = e;
    }
    virtual void drive() = 0;
    virtual ~Car() {};
    Engine* getEngine() {
        return engine;
    }
};

class SUV: public Car {
public:
    SUV(Engine* e): Car(e) {};
    void drive() override {
        cout<<"Driving suv car";
        getEngine()->start();
    }
};

class Sedan: public Car {
public:
    Sedan(Engine* e): Car(e) {};
    void drive() override {
        cout<<"Driving sedan car";
        getEngine()->start();
    }
};

int main() {
    Engine* petrol = new PetrolEngine();
    Engine* diesel = new DieselEngine();
    Car* car = new SUV(petrol);
    Car* car1 = new Sedan(diesel);
    car->drive();
    car1->drive();

    delete petrol;
    delete car;
    return 0;
};
