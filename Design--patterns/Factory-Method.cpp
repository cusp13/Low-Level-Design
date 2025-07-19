#include<iostream>
using namespace std;

class Theme {
    public:
    virtual void setTheme () = 0;
    virtual ~Theme() {}
};

class Darktheme : public Theme {
    public:
    void setTheme() override {
        cout<<"Enabling dark mode\n";
    }
};

class Lighttheme : public Theme {
    public:
    void setTheme() override {
        cout<<"Enabling light mode\n";
    }
};

class ThemeFactory {
    public:
    static Theme* manageTheme(const string& type) {
        if (type == "dark") return new Darktheme();
        if (type == "light") return new Lighttheme();
        return nullptr;
        }
    };

int main(){
    Theme* t1 = ThemeFactory::manageTheme("dark");
    if(t1){
        t1->setTheme();
        delete t1;
    } 
    Theme* t2 = ThemeFactory::manageTheme("light");
    if(t2){
        t2->setTheme();
        delete t2;
    }

    return 0;
};

