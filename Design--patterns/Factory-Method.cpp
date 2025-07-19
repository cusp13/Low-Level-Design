// Factory Method lets subclasses (child classes) decide which object to create.
// ✅ You still use a factory,
// ✅ But now each factory can be customized differently.

#include <iostream>
using namespace std;

class Transport {
public:
    virtual void deliver() = 0;
    virtual ~Transport() {}
};

class Truck : public Transport {
public:
    void deliver() override
    {
        cout << "Delivering by Truck\n";
    }
};

class Ship : public Transport {
public:
    void deliver() override
    {
        cout << "Delivering by Ship\n";
    }
};

class Flight: public Transport {
    public:
    void deliver() override {
        cout<<"Delivering by flight\n";
    }
};

class Logistics {
public:
    // This is the "factory method"
    virtual Transport *createTransport() = 0;
    void planDelivery() {
        Transport *transport = createTransport(); // 👈 Factory method used
        transport->deliver();
        delete transport;
    }
    virtual ~Logistics() {}
};

class RoadLogistics : public Logistics {
public:
    Transport *createTransport() override {
        return new Truck(); // 👈 RoadLogistics chooses Truck
    }
};

class SeaLogistics : public Logistics {
public:
    Transport *createTransport() override {
        return new Ship(); // 👈 SeaLogistics chooses Ship
    }
};

class AirLogistics : public Logistics {
    public:
    Transport *createTransport() override {
        return new Flight();
    }
};

int main()
{
    Logistics *logistics1 = new RoadLogistics();
    logistics1->planDelivery(); // Output: Delivering by Truck
    delete logistics1;

    Logistics *logistics2 = new SeaLogistics();
    logistics2->planDelivery(); // Output: Delivering by Ship
    delete logistics2;

    Logistics *logistics3 = new AirLogistics();
    logistics3->planDelivery(); // Output: Delivering by Ship
    delete logistics3;

    return 0;
}
