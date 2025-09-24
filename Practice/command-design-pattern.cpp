#include <iostream>
#include <memory>
using namespace std;

class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() {};
};

class Light {
public:
    void on() {
        cout << "Turning on light" << endl;
    }
    void off() {
        cout << "Turning off light" << endl;
    }
};

class Fan {
public:
    void on() {
        cout << "Turning on fan" << endl;
    }
    void off() {
        cout << "Turning off fan" << endl;
    }
};

// Command for Light
class LightCommand : public ICommand {
private:
    shared_ptr<Light> light;   // shared device
public:
    LightCommand(shared_ptr<Light> l) {
        light = l;
    }
    void execute() override {
        light->on();
    }
    void undo() override {
        light->off();
    }
};

// Command for Fan
class FanCommand : public ICommand {
private:
    shared_ptr<Fan> fan;   // shared device
public:
    FanCommand(shared_ptr<Fan> f) {
        fan = f;
    }
    void execute() override {
        fan->on();
    }
    void undo() override {
        fan->off();
    }
};

// Invoker
class RemoteController {
private:
    static const int numButtons = 4;
    unique_ptr<ICommand> buttons[numButtons];// each button owns its command
public:
    RemoteController() {
        for (int i = 0; i < numButtons; i++) {
            buttons[i] = nullptr;  // no command initially
        }
    }

    void setCommands(int idx, unique_ptr<ICommand> command) {
        if (idx >= 0 && idx < numButtons) {
            if (buttons[idx] == nullptr) {
                cout << "Command set at button " << idx << endl;
                buttons[idx] = move(command);   // transfer ownership
            } else {
                cout << "Button already set" << endl;
            }
        } else {
            cout << "Not a valid button" << endl;
        }
    }

    void pressButton(int idx) {
        if (idx >= 0 && idx < numButtons && buttons[idx] != nullptr) {
            buttons[idx]->execute();
        } else {
            cout << "No command set for button " << idx << endl;
        }
    }

    void undoButton(int idx) {
        if (idx >= 0 && idx < numButtons && buttons[idx] != nullptr) {
            buttons[idx]->undo();
        } else {
            cout << "No command to undo at button " << idx << endl;
        }
    }
};

int main() {
    cout << "No of buttons: 4" << endl;

    RemoteController controller;

    auto light = make_shared<Light>();
    auto fan = make_shared<Fan>();

    controller.setCommands(0, make_unique<LightCommand>(light));
    controller.setCommands(1, make_unique<FanCommand>(fan));

    controller.pressButton(0);   // Light ON
    controller.undoButton(0);    // Light OFF

    controller.pressButton(1);   // Fan ON
    controller.undoButton(1);    // Fan OFF
}
