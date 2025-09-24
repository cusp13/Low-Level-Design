// OPEN-CLOSE Principle.
// A class should be open for extension and closed for modification.

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

class Persistance {
private: 
   ShoppingCart* cart;
public:
   virtual void save(ShoppingCart* cart) = 0;
};

class SQLPersistance: public Persistance {
public:
    void save(ShoppingCart* cart) override {
        cout<<"Saving data in SQL Database"<<endl;
    }
};

class MongoPersistance: public Persistance {
public:
    void save(ShoppingCart* cart) override {
        cout<<"Saving data in Mongo Database"<<endl;
    }
};

class FilePersistance: public Persistance {
public:
     void save(ShoppingCart* cart) override {
         cout<<"Saving data in file persistence"<<endl;
     }
};



int main() {
    ShoppingCart * cart = new ShoppingCart();
    cart->addProduct(new Product("Laptop", 50000));
    cart->addProduct(new Product("Mouse", 2000));
    
    ShoppingCartPrinter* printer = new ShoppingCartPrinter(cart);
    printer->printInvoice();
    
    Persistance* sql = new SQLPersistance();
    Persistance* mongo = new MongoPersistance();
    Persistance* file = new FilePersistance();
    sql->save(cart);
    mongo->save(cart);
    file->save(cart);
};
