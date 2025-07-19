// Abstract Factory is a design pattern that
// lets you create families of related objects, without specifying their exact classes.
#include <iostream>
using namespace std;

class Button
{
public:
    virtual void paint() = 0;
    virtual ~Button() {}
};

class Checkbox
{
public:
    virtual void paint() = 0;
    virtual ~Checkbox() {}
};

class WinButton : public Button
{
public:
    void paint() override
    {
        cout << "Painting Windows Button\n";
    }
};

class WinCheckbox : public Checkbox
{
public:
    void paint() override
    {
        cout << "Painting Windows Checkbox\n";
    }
};

class MacButton : public Button
{
public:
    void paint() override
    {
        cout << "Painting Mac Button\n";
    }
};

class MacCheckbox : public Checkbox
{
public:
    void paint() override
    {
        std::cout << "Painting Mac Checkbox\n";
    }
};

class GUIFactory
{
public:
    virtual Button *createButton() = 0;
    virtual Checkbox *createCheckbox() = 0;
    virtual ~GUIFactory() {}
};

class WinFactory : public GUIFactory
{
public:
    Button *createButton() override
    {
        return new WinButton();
    }

    Checkbox *createCheckbox() override
    {
        return new WinCheckbox();
    }
};

class MacFactory : public GUIFactory
{
public:
    Button *createButton() override
    {
        return new MacButton();
    }

    Checkbox *createCheckbox() override
    {
        return new MacCheckbox();
    }
};

class Application
{
private:
    Button *button;
    Checkbox *checkbox;

public:
    Application(GUIFactory *factory)
    {
        button = factory->createButton();
        checkbox = factory->createCheckbox();
    }

    void paintUI()
    {
        button->paint();
        checkbox->paint();
    }

    ~Application()
    {
        delete button;
        delete checkbox;
    }
};

int main()
{
    GUIFactory *factory1 = new WinFactory();
    Application app1(factory1);
    app1.paintUI(); // Output: Painting Windows Button + Checkbox
    delete factory1;

    GUIFactory *factory2 = new MacFactory();
    Application app2(factory2);
    app2.paintUI(); // Output: Painting Mac Button + Checkbox
    delete factory2;

    return 0;
}
