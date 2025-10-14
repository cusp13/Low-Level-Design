#include<bits/stdc++.h>
using namespace std;

class Product {
private:
    int sku;
    string name;
    double price;
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
        if(sku == 101){
            name = "apple";
            price = 20;
        } else if(sku == 102) {
            name = "orange";
            price = 30;
        } else if(sku == 103) {
            name = "shirt";
            price = 400;
        } else if(sku == 104) {
            name = "remote";
            price = 100;
        }
        return new Product(sku, name, price);
    }
};

class InventoryStore {
public:
    virtual void addProduct(Product* prod, int qty) = 0;
    virtual void removeProduct(int sku, int qty) = 0;
    virtual bool checkStock(int sku) = 0;
    virtual vector<Product*> listAllProduct() = 0;
    virtual ~InventoryStore() {};
};

class DbInventoryStore : public InventoryStore {
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
    void removeProduct(int sku, int qty) override {
        if (stock->count(sku) == 0) 
        return;

    int currentQuantity = (*stock)[sku];
    int remainingQuantity = currentQuantity - qty;
    if (remainingQuantity > 0) {
        (*stock)[sku] = remainingQuantity;
    } else {
        stock->erase(sku);
    }
    }

    bool checkStock(int sku) override {
        if (stock->count(sku) == 0) 
            return 0;
        return (*stock)[sku];
    }

    vector<Product*> listAllProduct() override {
        vector<Product*> prodAvailble;
        for(auto it: *stock){
            int sku = it.first;
            int qty = it.second;
            if(qty > 0 && product->count(sku)) {
                prodAvailble.push_back((*product)[sku]);
            }
        }
        return prodAvailble;
    }
};


class InventoryManager {
private:
    InventoryStore* store;
public:
    InventoryManager(InventoryStore* store) {
        this->store = store;
    }
    void addStock(int sku, int q) {
        Product* prod = ProductFactory::createProduct(sku);
        store->addProduct(prod, q);
        cout<<"[InventoryManager] Added SKU " << sku<<"with quantit: "<<q<<endl;
    }
    void removeStock(int sku, int q) {
        store->removeProduct(sku, q);
    }
    bool checkStock(int sku) {
        return store->checkStock(sku);
    }
    vector<Product*> getAvailableProduct() {
        return store->listAllProduct();
    }
};
//<abstract-class>
class ReplenishStrategy {
public:
    virtual void replenish(InventoryManager* mgr, map<int, int> itemsToReplenish) = 0;
    virtual ~ReplenishStrategy() {}; 
};

class ThresholdReplenishStrategy : public ReplenishStrategy {
private:
    int threshold;
public:
    ThresholdReplenishStrategy(int threshold) {
        this->threshold = threshold;
    }
    void replenish(InventoryManager* mgr, map<int, int> itemsToReplenish) override {
        for(auto it: itemsToReplenish) {
            int sku = it.first;
            int qtyToAdd = it.second;
            int currentStock = mgr->checkStock(sku);
            if(threshold > currentStock) {
                mgr->addStock(sku, qtyToAdd);
                cout << "  -> SKU " << sku << " was " << currentStock 
                << ", replenished by " << qtyToAdd << endl;
            }
        }
    }
};

class WeeklyReplenishStrategy : public ReplenishStrategy {
public: 
    WeeklyReplenishStrategy() {};
    void replenish(InventoryManager* mgr, map<int, int> itemsToReplenish) override {
        cout<<"we are going for weeking replenish strategy"<<endl;
    }
};

class DarkStore {
private:
    string name;
    double x,y;
    InventoryManager* inventoryManager;
    ReplenishStrategy* replenishStrategy;
public:
    DarkStore(string n, double x_cord, double y_cord) {
        n = name;
        x = x_cord;
        y = y_cord;
        // every dark store ownd it own inventory manager.
        inventoryManager = new InventoryManager(new DbInventoryStore);
        // Why replenish Strategy is not initialize in this.
        // Because replenishStrategy is optional and replaceable — it depends on the strategy or policy you want to use later.
    }
    ~DarkStore() {
        delete inventoryManager;
        if (replenishStrategy) delete replenishStrategy;
    }
    double distanceTo(double ux, double uy) {
        return sqrt((x - ux)*(x - ux) + (y - uy)*(y - uy));
    }
    void runReplenishment(map<int,int> itemsToReplenish) {
        if (replenishStrategy) {
            replenishStrategy->replenish(inventoryManager, itemsToReplenish);
        }
    }
    vector<Product*> getAllProducts() {
        return inventoryManager->getAvailableProduct();
    }
    int checkStock(int sku) {
        return inventoryManager->checkStock(sku);
    }
    void removeStock(int sku, int qty) {
        inventoryManager->removeStock(sku, qty); 
    }
    void addStock(int sku, int qty) {
        Product* prod = ProductFactory::createProduct(sku);
        inventoryManager->addStock(sku, qty);
    }
    // Getters & Setters
    void setReplenishStrategy(ReplenishStrategy* strategy) {
        this->replenishStrategy = strategy;
    }
    string getName() {
        return this->name;
    }
    double getXCoordinate() {
        return this->x;
    }
    double getYCoordinate() {
        return this->y;
    }
    InventoryManager* getInventoryManager() {
        return this->inventoryManager;
    }
};

class DarkStoreManager {
private:
    vector<DarkStore*>* darkStores;
    static DarkStoreManager* instance;

    DarkStoreManager() {
        darkStores = new vector<DarkStore*>();
    }
public:
    static DarkStoreManager* getInstance() {
        if(instance == nullptr) {
            instance =  new DarkStoreManager();
        }
        return instance;
    }
    void registerDarkStore(DarkStore* ds) {
        darkStores->push_back(ds);
    }
    vector<DarkStore*> getNearByStore(double ux, double uy, double maxDistance) {
        vector<pair<double,DarkStore*>> distList;
        for (auto ds : *darkStores) {
            double d = ds->distanceTo(ux, uy);
            if (d <= maxDistance) {
                distList.push_back(make_pair(d, ds));
            }
        }
        sort(distList.begin(), distList.end(),
             [](auto &a, auto &b){ return a.first < b.first; });

        vector<DarkStore*> result;
        for (auto &p : distList) {
            result.push_back(p.second);
        }
        return result;
    }
    ~DarkStoreManager() {
        for (auto ds : *darkStores) {
            delete ds;
        }
        delete darkStores;
    }
};
DarkStoreManager* DarkStoreManager::instance = nullptr;

class Cart {
public: 
    vector<pair<Product* , int>> items;
    void addItems(int sku, int q) {
        Product* prod = ProductFactory::createProduct(sku);
        items.push_back({prod, q});
        cout<<"[Cart] Added SKU " << sku << "(" << prod->getName() << ") x" << q<<endl;
    }
    double getTotal() {
        double totalAmount = 0;
        for(auto &it: items) {
            totalAmount += it.first->getPrice()* it.second;
        }
        return totalAmount;
    }
    vector<pair<Product* , int>> getItems() {
        return items;
    }
    ~Cart() {
        for(auto &it: items) {
            delete it.first;
        }
    }
};

class User {
private:  
    string name;
    double x, y;
    Cart* cart;
public:
    User (string name, double x_cord, double y_cord) {
        this->name = name;
        x = x_cord;
        y = y_cord;
        cart = new Cart();
    }
    ~User() {
        delete cart;
    }
    string getName() const { return name; }
    double getX() const { return x; }
    double getY() const { return y; }
    // getter and setter
    Cart* getCart() {
        return cart;
    }
};

class DeliveryPartner {
public:
    string name;
    DeliveryPartner(string n) {
        name = n;
    }
};

class Order {
private:
    static int nextId;
    int orderId;
    User *user;
    vector<pair<Product*,int>> items;
    vector<DeliveryPartner*> partners;
    double totalAmount;
public:
    Order(User* u) {
        orderId = nextId++;
        totalAmount = 0.0;
        user = u;
    }
    
    // Public methods to access private members
    void addItem(Product* product, int quantity) {
        items.push_back({product, quantity});
    }
    
    void setTotalAmount(double amount) {
        totalAmount = amount;
    }
    
    void addPartner(DeliveryPartner* partner) {
        partners.push_back(partner);
    }
    
    int getOrderId() const {
        return orderId;
    }
    
    const vector<pair<Product*,int>>& getItems() const {
        return items;
    }
    
    double getTotalAmount() const {
        return totalAmount;
    }
    
    const vector<DeliveryPartner*>& getPartners() const {
        return partners;
    }
};
int Order::nextId = 1;

class OrderManager {
    private:
        vector<Order*>* orders;
        static OrderManager* instance;
    
        OrderManager() {
            orders = new vector<Order*>();
        }
    
    public:
        static OrderManager* getInstance() {
            if(instance == nullptr) {
                instance = new OrderManager();
            }
            return instance;
        }
    
        void placeOrder(User* user, Cart* cart) {
            cout << "\n[OrderManager] Placing Order for: " << user->getName() << "\n";
    
            // product --> Qty
            vector<pair<Product*,int>> requestedItems = cart->getItems();
        
            // 1) Find nearby dark stores within 5 KM
            double maxDist = 5.0;
            vector<DarkStore*> nearbyDarkStores = DarkStoreManager::getInstance()->getNearByStore(user->getX(), user->getY(), maxDist);
            
            if (nearbyDarkStores.empty()) {
                cout << "  No dark stores within 5 KM. Cannot fulfill order.\n";
                return;
            }
        
            // 2) Check if closest store has all items
            DarkStore* firstStore = nearbyDarkStores.front();
    
            bool allInFirst = true;
            for (pair<Product*,int>& item : requestedItems) {
    
                int sku = item.first->getSku();
                int qty = item.second;
    
                if (firstStore->checkStock(sku) < qty) {
                    allInFirst = false;
                    break;
                }
            }
        
            Order* order = new Order(user);
    
            // One delivery partner required...
            if (allInFirst) {
    
                cout << "  All items at: " << firstStore->getName() << "\n";
    
                // Remove the products from store
                for (pair<Product*,int>& item : requestedItems) {
                    int sku = item.first->getSku();
                    int qty = item.second;
                    firstStore->removeStock(sku, qty);
                    order->addItem(item.first, qty);
                }
    
                order->setTotalAmount(cart->getTotal());
                order->addPartner(new DeliveryPartner("Partner1"));
                cout << "  Assigned Delivery Partner: Partner1\n";
            } 
    
            // Multiple delivery partners required
            else {
                cout << "  Splitting order across stores...\n";
    
                map<int,int> allItems; //SKU --> Qty
    
                for (pair<Product*,int>& item : requestedItems) {
                    allItems[item.first->getSku()] = item.second;
                }
        
                int partnerId = 1;
                for (DarkStore* store : nearbyDarkStores) {
    
                    // If allItems becomes empty, we break early (all SKUs have been assigned)
                    if (allItems.empty()) break;
    
                    cout << "   Checking: " << store->getName() << "\n";
        
                    bool assigned = false;
                    vector<int> toErase;
                    
                    for (auto& [sku, qtyNeeded] : allItems) {
    
                        int availableQty = store->checkStock(sku);
                        if (availableQty <= 0) continue;
        
                        //take whichever is smaller: available or qtyNeeded.
                        int takenQty = min(availableQty, qtyNeeded);
                        store->removeStock(sku, takenQty);
    
                        cout << "     " << store->getName() << " supplies SKU " << sku 
                             << " x" << takenQty << "\n";
    
                        order->addItem(ProductFactory::createProduct(sku), takenQty);
        
                        // Adjust the Quantity
                        if (qtyNeeded > takenQty) {
                            allItems[sku] = qtyNeeded - takenQty;
                        } else {
                            toErase.push_back(sku);
                        }
                        assigned = true;
                    }
    
                    // After iterating all SKUs in allItems, we erase 
                    // any fully‐satisfied SKUs from the allItems map.
                    for (int sku : toErase) allItems.erase(sku);
        
                    // If at least one SKU was taken from this store, we assign 
                    // a new DeliveryPartner.
                    if (assigned) {
                        string pname = "Partner" + to_string(partnerId++);
                        order->addPartner(new DeliveryPartner(pname));
                        cout << "     Assigned: " << pname << " for " << store->getName() << "\n";
                    }
                }
        
                //  if remaining still has entries, we print which SKUs/quantities could not be fulfilled.
                if (!allItems.empty()) {
                    cout << "  Could not fulfill:\n";
                    for (auto& [sku, qty] : allItems) {
                        cout << "    SKU " << sku << " x" << qty << "\n";
                    }
                }
    
                // recompute order->totalAmount
                double sum = 0;
                for (auto& item : order->getItems()) {
                    sum += item.first->getPrice() * item.second;
                }
                order->setTotalAmount(sum);
            }
        
            // Printing Order Summary
            cout << "\n[OrderManager] Order #" << order->getOrderId() << " Summary:\n";
            cout << "  User: " << user->getName() << "\n  Items:\n";
            for (auto& item : order->getItems()) {
                cout << "    SKU " << item.first->getSku()
                     << " (" << item.first->getName() << ") x" << item.second
                     << " @ ₹" << item.first->getPrice() << "\n";
            }
            cout << "  Total: ₹" << order->getTotalAmount() << "\n  Partners:\n";
            for (auto* dp : order->getPartners()) {
                cout << "    " << dp->name << "\n";
            }
            cout << endl;
        
            orders->push_back(order);
    
            // Cleanups
            for (auto* dp : order->getPartners()) delete dp;
            for (auto& item : order->getItems()) delete item.first;
        }    
    
        vector<Order*> getAllOrders() {
            return *orders;
        }
    
        ~OrderManager() {
            for (auto ord : *orders) {
                delete ord;
            }
            delete orders;
        }
    };
OrderManager* OrderManager::instance = nullptr;

class ZeptoHelper {
public:  
    static void showAllItems(User* user) {
        cout << "\n[Zepto] All Available products within 5 KM for " << user->getName() << ":\n";
        DarkStoreManager* dsManager = DarkStoreManager::getInstance();
        vector<DarkStore*> nearbyStores = dsManager->getNearByStore(user->getX(), user->getY(), 5.0);

        map<int, double> skuToPrice;
        map<int, string> skuToName;

        for (DarkStore* darkStore : nearbyStores) {
            vector<Product*> products = darkStore->getAllProducts();

            for (Product* product : products) {
                int sku = product->getSku();

                if (skuToPrice.count(sku) == 0) {
                    skuToPrice[sku] = product->getPrice();
                    skuToName[sku]  = product->getName();
                }
            }
        }
        for (auto& entry : skuToPrice) {
            int sku = entry.first;
            double price = entry.second;
            cout << "  SKU " << sku << " - " << skuToName[sku] << " @ ₹" << price << "\n";
        }
    }

    static void initialize() {
        auto dsManager  = DarkStoreManager::getInstance();
        // dark store 1
        DarkStore* darkStoreA = new DarkStore("DarkStoreA", 0.0, 0.0);
        darkStoreA->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout << "\nAdding stocks in DarkStoreA...." << endl;  
        darkStoreA->addStock(101, 5); // Apple
        darkStoreA->addStock(102, 2); // Banana

        // dark store 2
        DarkStore* darkStoreB = new DarkStore("DarkStoreB", 4.0, 1.0);
        darkStoreB->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout << "\nAdding stocks in DarkStoreB...." << endl; 
        darkStoreB->addStock(101, 3); // Apple
        darkStoreB->addStock(103, 10); // Chocolate

        DarkStore* darkStoreC = new DarkStore("DarkStoreC", 2.0, 3.0);
        darkStoreC->setReplenishStrategy(new ThresholdReplenishStrategy(3));

        cout << "\nAdding stocks in DarkStoreC...." << endl; 
        darkStoreC->addStock(102, 5); // Banana
        darkStoreC->addStock(201, 7); // T-Shirt

        dsManager->registerDarkStore(darkStoreA);
        dsManager->registerDarkStore(darkStoreB);
        dsManager->registerDarkStore(darkStoreC);
    }
};

// high level
int main() {
    cout<<"****** We are building zepto ******"<<endl;
    User* divyansh = new User("Divyansh", 0.0, 0.0);
    ZeptoHelper::initialize();
    ZeptoHelper::showAllItems(divyansh);
    Cart *cart = new Cart;
    cart->addItems(101, 1);
    OrderManager* odmgr = OrderManager::getInstance();
    odmgr->placeOrder(divyansh, cart);
    // Don't use new when using factory product because it is already returning new pointer as a product
    // Product* product = ProductFactory::createProduct(101);
    // cout<<"product name:  "<<product->getName();
    return 0;
}
