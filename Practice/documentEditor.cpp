#include<iostream>
#include<vector>
#include<string>

using namespace std;

class DocumentElement {
public:
    virtual void render(string text) = 0;
};

class TextElement : public DocumentElement {
private:
    string text;
public:
    TextElement(string text){
        this->text = text;
    }
    void render(string text) override {
        cout<<"Rendering text file"<<endl;
    }
};

class ImageElement : public DocumentElement {
private:
    string imagePath;
public:
    ImageElement(string imagePath) {
        this->imagePath = imagePath;
    }
    void render(string imagesPath) override {
        cout<<"Rendering [image.png]"<<endl;
    }
};

class Document {
private:
    vector<DocumentElement*> elements;
public:
    void addElement(DocumentElement* text) {
        elements.push_back(text);
    }
    vector<DocumentElement*> getElement(string text) {
        return elements;
    }
};

class Persistance {
public:
    virtual void save(string data) = 0;
};

class SQLDatabase : public Persistance {
public:
    void save(string data) override {
        cout<<"Saving data in SQL: "<<data<<endl;
    } 
};



int main(){

};