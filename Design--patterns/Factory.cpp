#include <bits/stdc++.h>
using namespace std;

// Step 1: Base class
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() {}
};

// Step 2: Concrete classes
class Circle : public Shape {
public:
    void draw() override {
        cout <<"Drawing Circle\n";
    }
};

class Square : public Shape {
public:
    void draw() override {
        cout <<"Drawing Square\n";
    }
};

class Rectangle : public Shape {
public:
    void draw() override {
        cout <<"Drawing Rectangle\n";
    }
};
class Triangle : public Shape {
public: 
    void draw() override {
        cout<<"Drawing triangle\n";
    }
};

// Step 3: Factory class
class ShapeFactory {
public:
    static unique_ptr<Shape> createShape(const string& type) {
        if (type == "circle") {
            return make_unique<Circle>();
        } else if (type == "square") {
            return make_unique<Square>();
        } else if (type == "rectangle") {
            return make_unique<Rectangle>();
        } else if(type == "triangle"){
            return make_unique<Triangle>();
        } else {
            return nullptr;
        }
    }
};
// Step 4: Client code
int main() {
    unique_ptr<Shape> shape1 = ShapeFactory::createShape("circle");
    unique_ptr<Shape> shape2 = ShapeFactory::createShape("square");
    unique_ptr<Shape> shape3 = ShapeFactory::createShape("rectangle");
    unique_ptr<Shape> shape4 = ShapeFactory::createShape("triangle");

    if (shape1) shape1->draw();
    if (shape2) shape2->draw();
    if (shape3) shape3->draw();
    if (shape4) shape4->draw();
    return 0;
}
