#include<iostream>
using namespace std;

class Text {
public:
    virtual string getText() = 0;
    virtual string lang() = 0;
    virtual ~Text() {}
};

class TextStyle: public Text {
public:
    string getText() override {
        return "This is {styled} text";
    }
    string lang() override {
        return "English";
    }
};

class TextDecorator : public Text {
protected:
    Text* text;
public:
// Initialize the member variable, not recursively call the constructor
    TextDecorator(Text* txt) : text(txt) {}
    virtual ~TextDecorator() {
        delete text;
    }
};

class TextBold : public TextDecorator {
public:
    TextBold(Text *txt): TextDecorator(txt) {}
    string getText() override {
        return "This is Bold text";
    }
    string lang() override {
        return "English Bold";
    }
};
class TextItalic : public TextDecorator {
public:
    TextItalic(Text *txt): TextDecorator(txt) {}
    string getText() override {
        return "This is Italic text";
    }
    string lang() override {
        return "English Italic";
    }
};

int main(){
    Text* newText = new TextStyle();
    cout<<newText->getText()<<endl;
    newText = new TextBold(newText);
    cout<<newText->getText()<<endl;
    newText = new TextItalic(newText);
    cout<<newText->getText()<<endl;
    cout<<newText->lang()<<endl;
    
}
