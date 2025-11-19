#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Forward declarations
class Stock;
class Portfolio;
class Order;

// ------------------ User Class ------------------
class User {
public:
    int userId;
    string name;
    string email;
    string passwordHash;

    vector<Portfolio*> portfolios;

    // Constructor
    User(int id, string n, string e, string p) : userId(id), name(n), email(e), passwordHash(p) {}

    void registerUser() {
        cout << name << " registered with email: " << email << endl;
    }

    void login() {
        cout << name << " logged in." << endl;
    }

    void addPortfolio(Portfolio* portfolio) {
        portfolios.push_back(portfolio);
    }
};

// ------------------ Stock Class ------------------
class Stock {
public:
    int stockId;
    string symbol;
    string name;
    double marketPrice;

    // Constructor
    Stock(int id, string s, string n, double price) : stockId(id), symbol(s), name(n), marketPrice(price) {}

    void updatePrice(double price) {
        marketPrice = price;
        cout << symbol << " price updated to " << marketPrice << endl;
    }
};

// ------------------ Order Class ------------------
class Order {
public:
    int orderId;
    int portfolioId;
    int stockId;
    string orderType;
    int quantity;
    string status;
    time_t createdAt;

    // Constructor
    Order(int oId, int pId, int sId, string type, int qty)
        : orderId(oId), portfolioId(pId), stockId(sId), orderType(type), quantity(qty) {
        status = "Pending";
        createdAt = time(nullptr);
    }

    void executeOrder() {
        status = "Executed";
        cout << "Order " << orderId << " executed." << endl;
    }
};

// ------------------ Portfolio Class ------------------
class Portfolio {
public:
    int portfolioId;
    string name;
    int userId;
    time_t creationDate;

    vector<Stock*> stocks;
    vector<Order*> orders;

    Portfolio(int id, string n, int uId) : portfolioId(id), name(n), userId(uId) {
        creationDate = time(nullptr);
    }

    void allocation(Stock* stock) {
        stocks.push_back(stock);
        cout << stock->symbol << " added to portfolio " << name << endl;
    }

    void removeStock(int stockId) {
        for (auto it = stocks.begin(); it != stocks.end(); ++it) {
            if ((*it)->stockId == stockId) {
                cout << (*it)->symbol << " removed from portfolio " << name << endl;
                stocks.erase(it);
                return;
            }
        }
        cout << "Stock not found in portfolio." << endl;
    }

    void rebalance() {
        cout << "Portfolio " << name << " rebalanced." << endl;
    }

    void placeOrder(Order* order) {
        orders.push_back(order);
    }
};

// ------------------ Main ------------------
int main() {
    // Create user
    User u1(1, "Divyansh", "divyansh@example.com", "hashedPassword123");
    u1.registerUser();
    u1.login();

    // Create stock
    Stock s1(101, "AAPL", "Apple Inc.", 150.0);
    Stock s2(102, "GOOGL", "Alphabet Inc.", 2800.0);

    // Create portfolio
    Portfolio p1(201, "Tech Portfolio", u1.userId);
    u1.addPortfolio(&p1);

    // Allocate stocks to portfolio
    p1.allocation(&s1);
    p1.allocation(&s2);

    // Place order
    Order o1(301, p1.portfolioId, s1.stockId, "BUY", 10);
    p1.placeOrder(&o1);

    // Execute order
    o1.executeOrder();

    // Rebalance portfolio
    p1.rebalance();

    return 0;
}
