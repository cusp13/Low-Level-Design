#include<iostream>
using namespace std;

// This is example of interface segregation principle which the child class which is inheriting should not inheritance and implment those methods which are not in use for them.
// that is why we have separated in two different type even i have to implement the same method but in a different classs
// abstract class for 2D objects.
class TwoDimensional {
public:
    virtual double area(int side) = 0;
};

// abstract class for 3d objects
class ThreeDimensional {
public:
    virtual double volume(int side) = 0;
    virtual double surfaceArea(int side) = 0;
};

// inheriting the required property.
class Square: public TwoDimensional {
public:
    double area(int side) override {
        return side*side;
    }
};

class Cube: public ThreeDimensional {
public:
    double surfaceArea(int side) override {
        return side*side;
    }
    double volume(int side) override {
        return side*side*side;
    }
};

int main(){
    Square sq;
    cout<<sq.area(10)<<endl;
};
