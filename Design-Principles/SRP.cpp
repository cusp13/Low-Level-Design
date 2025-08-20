// In this code, the single responsibility principle is followed
#include <iostream>
#include<vector>
using namespace std;

class Product {
public: 
    string name;
    double price;
    
    Product(string name, double price){
        this->name = name;
        this->price = price;
    }
};

class ShoppingCart {
private:
    vector<Product*> products;
public:
    void addProduct(Product *p){
        products.push_back(p);
    }
    
    const vector<Product*> getProducts() {
        return products;
    }
    
    double calculatePrice(){
        double total = 0;
        for(auto it: products) {
            total += it->price;
        }
        return total;
    }
};


class ShoppingCartPrinter {
private:
    ShoppingCart* cart;
public:
    ShoppingCartPrinter(ShoppingCart* cart){
        this->cart = cart;
    }
    void printInvoice() {
        for(auto it: cart->getProducts()){
            cout<< it->name<<endl;
        }
        cout << "Total Price: " << cart->calculatePrice()<<endl;
    }
};

class ShoppingCartStorage {
private:
    ShoppingCart* cart;
public:
    ShoppingCartStorage(ShoppingCart* cart){
        this->cart = cart;
    }
    void saveInDB() {
        for(auto it: cart->getProducts()){
            cout<<"Saving data in db for: "<<it->name<<endl;
        }
    }
};

int main() {
    ShoppingCart * cart = new ShoppingCart();
    cart->addProduct(new Product("Laptop", 50000));
    cart->addProduct(new Product("Mouse", 2000));
    
    ShoppingCartPrinter* printer = new ShoppingCartPrinter(cart);
    printer->printInvoice();
    
    ShoppingCartStorage* storage = new ShoppingCartStorage(cart);
    storage -> saveInDB();
};
