#include<bits/stdc++.h>
using namespace std;

class Product {
private:
    int sku;
    string name;
    double price:
public:
    Product(int sku, string name, double price) {
        this->sku = sku;
        this->name = name;
        this->price = price;
    }
    int getSku() {
        return sku;
    }
    string getName() {
        return name;
    }
    double getPrice() {
        return price;
    }
};

class ProductFactory {
public:
    static Product* createProduct(int sku) {
        string name;
        double price;
        if(sku == "101"){
            name = "apple";
            price = 20;
        } else if(sku == "102") {
            name = "orange";
            price = 30;
        } else if(sku == "103") {
            name = "shirt";
            price = 400;
        } else if(sku == "104") {
            name = "remote";
            price = 100;
        }
    }
    return new Product(sku, name, price);
};

class InventoryStore {
public:
    virtual void addProduct() = 0;
    virtual void removeProduct() = 0;
    virtual void checkStock() = 0;
    virtual void listAllProduct() = 0;
    virtual ~InventoryStore() {};
};

class DbInventoryStore() : public InventoryStore {
private:
    map<int,int>* stock;
    map<int, Product*>* product;
public:
    DbInventoryStore() {
        stock = new map<int,int>();
        product = new map<int,Product*>();
    }
    ~DbInventoryStore() {
        for (auto it : *product) {
            delete it.second;
        }
        delete product;
        delete stock;
    }
    void addProduct(Product* prod, int qty) override {
        int sku = prod->getSku();
        if (product->count(sku) == 0) {
            (*product)[sku] = prod;
        } else {
            delete prod;
        }
        (*stock)[sku] += qty;
    } 
    void removeProduct(Product* p) override {
        int sku = p->getSku();
        if(stock.find(sku)==stock.end()){
            stock.erase(sku);
        } else {
            stock[sku]--;
        }
    }
    bool checkStock(int sku) override {
        if(stock.find(sku)!=stock.end()){
            return true;
        }
        return false;
    }
    vector<Product*> listAllProduct() {
        vector<Product*>prod;
        for(auto it: stock){
            prod.push_back(it.second);
        }
    }
};

int main() {
    cout<<"****** We are building zepto ******"<<endl;


    return 0;
}
