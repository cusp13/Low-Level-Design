#include<bits/stdc++.h>
using namespace std;

class WalkableRobot {
public:
    virtual void walk() = 0;
    virtual ~WalkableRobot() {};
};

class NormalWalk : public WalkableRobot {
public:
    void walk() override {
        cout<<"Normal walking"<<endl;
    }
};

class NoWalk :  public WalkableRobot {
public:
    void walk() override {
        cout<<"No walking"<<endl;
    }
};

class FlyableRobot {
public:
    virtual void fly() = 0;
    virtual ~FlyableRobot() {};
};

class NormalFly : public FlyableRobot {
public:
    void fly() override {
        cout<<"Normal Flying"<<endl;
    }
};

class NoFly : public FlyableRobot {
public:
    void fly() override {
        cout<<"No Fly"<<endl;
    }
};

class Robot {
protected:
    WalkableRobot *walkBehaviour; // composition
    FlyableRobot *flyBehaviour; // composition
public:
    Robot(WalkableRobot* w, FlyableRobot* f){
        this->walkBehaviour = w;
        this->flyBehaviour = f;
    }
    void walk() {
        walkBehaviour->walk();
    }
    void fly() {
        flyBehaviour->fly();
    }
    virtual void projection() = 0;
};

class CompanionRobot : public Robot {
public:
    CompanionRobot(WalkableRobot* w, FlyableRobot* f) : Robot(w, f) {};
    void projection() override {
        cout<<"Displaying companion"<<endl;
    }
};

class MachineRobot : public Robot {
public: 
    MachineRobot(WalkableRobot* w, FlyableRobot* f) : Robot(w,f){};
    void projection() override {
        cout<<"Diplaying machine"<<endl;
    }
};

int main() {
    Robot *robot = new CompanionRobot(new NormalWalk(), new NoFly());
    robot->walk();
    robot->fly();
    robot->projection();
    
    Robot *machine = new CompanionRobot(new NoWalk(), new NormalFly());
    machine->walk();
    machine->fly();
    machine->projection();
    
    return 0;
}
