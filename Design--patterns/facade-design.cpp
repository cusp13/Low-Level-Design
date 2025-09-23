#include<iostream>
using namespace std;
 
// subsystems
class PowerSupply {
public:
    void providePower() {
        cout<<"power supply provinding power"<<endl;
    }
};

class CoolingSystem {
public:
    void provideCooling() {
        cout<<"providing cooling"<<endl;
    }
};

class CPU {
public:
    void initialize() {
        cout<<"Initilizing CPU"<<endl;
    }
};

class Memory {
public:
    void selfTest() {
        cout<<"Providng memory"<<endl;
    }
};


class BIOS {
public:
    void boot(CPU& cpu, Memory& memory) {
        cpu.initialize();
        memory.selfTest();
    }
};

class OperatingSystem {
public:
    void load() {
         cout<<"Operaring system started"<<endl;
    }
};


class ComputerFacade {
private:
    PowerSupply powerSupply;
    CoolingSystem coolingSystem;
    CPU cpu;
    Memory memory;
    BIOS bios;
    OperatingSystem os;
public:
    void startComputer() {
        cout<<"computer starting"<<endl;
        powerSupply.providePower();
        coolingSystem.provideCooling();
        bios.boot(cpu, memory);
        os.load();
    }
};

int main() {
    ComputerFacade computerfacade;
    computerfacade.startComputer();

    return 0;
};
