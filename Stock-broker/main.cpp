// clean_order_service.cpp
// A minimal, clean design for an OrderService using
// - Domain model (Order)
// - Repository abstraction (Database interface)
// - Concrete repositories (PostgresRepository, MongoRepository) as strategies
// - Exchange adapter (external service stub)
// - Dependency Injection (OrderService receives Database & Exchange)
// This file is written for clarity and to teach LLD -> details are printed to console.

#include <bits/stdc++.h>
using namespace std;

// -------------------- Helpers --------------------
static string generateUUID() {
    // simple UUIDv4-like generator (not cryptographically perfect but fine for examples)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xffffffff);
    std::ostringstream ss;
    ss << hex << setfill('0');
    ss << setw(8) << dis(gen) << "-";
    ss << setw(4) << (dis(gen) & 0xffff) << "-";
    ss << setw(4) << ((dis(gen) & 0x0fff) | 0x4000) << "-"; // version 4
    ss << setw(4) << ((dis(gen) & 0x3fff) | 0x8000) << "-"; // variant
    ss << setw(12) << dis(gen);
    return ss.str();
}

// -------------------- Domain Model --------------------
enum class OrderStatus { ACTIVE, PENDING, CONFIRMED, FAILED };

static string statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::ACTIVE: return "ACTIVE";
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::CONFIRMED: return "CONFIRMED";
        case OrderStatus::FAILED: return "FAILED";
    }
    return "UNKNOWN";
}

static unordered_map<string, OrderStatus> stringToStatusMap = {
    {"ACTIVE", OrderStatus::ACTIVE},
    {"PENDING", OrderStatus::PENDING},
    {"CONFIRMED", OrderStatus::CONFIRMED},
    {"FAILED", OrderStatus::FAILED}
};

struct Order {
    string id;
    string userId;
    string symbol;
    int quantity;
    OrderStatus status;
    string exchangeId; // id returned by external exchange (if any)

    Order() = default;

    Order(const string &userId_, const string &symbol_, int qty)
        : id(generateUUID()), userId(userId_), symbol(symbol_), quantity(qty), status(OrderStatus::PENDING), exchangeId("") {}
};

// -------------------- Repository Abstraction --------------------
// Repository / Database interface - responsible only for persistence concerns
class Database {
public:
    virtual ~Database() = default;
    virtual void saveOrder(const Order &order) = 0;       // persist order
    virtual optional<Order> getOrder(const string &orderId) = 0; // fetch
};

// -------------------- Concrete Repositories (Stubs) --------------------
class PostgresRepository : public Database {
public:
    void saveOrder(const Order &order) override {
        // In a real implementation this would use SQL statements and connection pooling
        cout << "[Postgres] Saving order " << order.id << " for user " << order.userId
             << " symbol=" << order.symbol << " qty=" << order.quantity
             << " status=" << statusToString(order.status);
        if (!order.exchangeId.empty()) cout << " exchangeId=" << order.exchangeId;
        cout << "\n";
    }

    optional<Order> getOrder(const string &orderId) override {
        // stub - production code would query DB. We return nullopt here.
        cout << "[Postgres] getOrder(" << orderId << ") - stub returning nullopt\n";
        return nullopt;
    }
};

class MongoRepository : public Database {
public:
    void saveOrder(const Order &order) override {
        // In production this would use a MongoDB client and BSON documents
        cout << "[Mongo] Saving order " << order.id << " (mongo stub)\n";
    }

    optional<Order> getOrder(const string &orderId) override {
        cout << "[Mongo] getOrder(" << orderId << ") - stub returning nullopt\n";
        return nullopt;
    }
};

// -------------------- External Exchange Adapter (Stub) --------------------
// This represents the 3rd party exchange. The adapter isolates exchange-specific logic,
// retries, idempotency keys, and rate-limiting.
class Exchange {
private:
    unordered_map<string, int> priceMap;
public:
    Exchange() {
        // seed some sample prices
        priceMap = {{"INFY", 1000}, {"TCS", 2000}, {"GOOGL", 3000}, {"NIFTY", 4000}};
    }

    optional<int> getPrice(const string &symbol) {
        auto it = priceMap.find(symbol);
        if (it == priceMap.end()) return nullopt;
        return it->second;
    }

    // placeOrder returns exchangeOrderId on success or nullopt on failure
    optional<string> placeOrder(const Order &order) {
        // Simulate success for supported symbols and failure otherwise
        if (priceMap.find(order.symbol) == priceMap.end()) return nullopt;
        // simulate network latency small
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return string("ex-") + order.id.substr(0, 8); // simple exchange id
    }
};

// -------------------- Services --------------------
// PriceService demonstrates composition/aggregation usage
class PriceService {
private:
    Exchange *exchange; // aggregation: PriceService does not own Exchange
public:
    PriceService(Exchange *ex) : exchange(ex) {}

    optional<int> getPrice(const string &symbol) {
        return exchange->getPrice(symbol);
    }
};

// OrderService: core business logic lives here. It depends on abstractions (Database & Exchange)
class OrderService {
private:
    Database *db;        // injected repository (strategy)
    Exchange *exchange;  // injected exchange adapter

public:
    // OrderService does NOT own db/exchange. Caller controls lifetimes.
    OrderService(Database *db_, Exchange *exchange_) : db(db_), exchange(exchange_) {}

    // createOrder builds a domain Order, asks exchange to place it (async in real system), and persists.
    // Here we do a synchronous flow for demo and then persist the resulting order.
    optional<Order> createAndSaveOrder(const string &userId, const string &symbol, int qty) {
        Order order(userId, symbol, qty);

        // 1) Validate
        if (qty <= 0) {
            cout << "[OrderService] invalid quantity" << endl;
            order.status = OrderStatus::FAILED;
            db->saveOrder(order);
            return nullopt;
        }

        // 2) Check price freshness / existence
        auto price = exchange->getPrice(symbol);
        if (!price.has_value()) {
            cout << "[OrderService] symbol not supported by exchange: " << symbol << endl;
            order.status = OrderStatus::FAILED;
            db->saveOrder(order);
            return nullopt;
        }

        // 3) Place order at exchange (in production this would be queued + async worker)
        auto exOrderId = exchange->placeOrder(order);
        if (!exOrderId.has_value()) {
            cout << "[OrderService] exchange rejected order" << endl;
            order.status = OrderStatus::FAILED;
            db->saveOrder(order);
            return nullopt;
        }

        // 4) On successful placement, update order and persist
        order.exchangeId = *exOrderId;
        order.status = OrderStatus::CONFIRMED; // simplified: we treat "placed" as CONFIRMED here

        db->saveOrder(order);
        return order;
    }

    // fetch is delegated to repository
    // we can keep some redis pub/sub to fetch the latest price as we can not directly delegate all the request to exchange it involve costing and we
    // can also rate limit from backend.  
    optional<Order> getOrder(const string &orderId) {
        return db->getOrder(orderId);
    }
};

// -------------------- Demo / Main --------------------
int main() {
    // Create shared external dependencies
    Exchange exchange;                        // real service would be a singleton / client
    PostgresRepository pgRepo;                // concrete strategy
    MongoRepository mongoRepo;                // another strategy

    // Choose repository at runtime (Strategy pattern)
    Database *repo = &pgRepo; // we can switch to &mongoRepo without changing OrderService

    // Inject dependencies into services
    PriceService priceService(&exchange);
    OrderService orderService(repo, &exchange);

    // Use flow
    string userId = "user-100";
    auto currentPrice = priceService.getPrice("INFY");
    if (currentPrice.has_value()) cout << "Current INFY price: " << *currentPrice << "\n";

    auto maybeOrder = orderService.createAndSaveOrder(userId, "INFY", 5);
    if (maybeOrder.has_value()) {
        cout << "Order created & saved. id=" << maybeOrder->id << " status=" << statusToString(maybeOrder->status)
             << " exchangeId=" << maybeOrder->exchangeId << "\n";
    } else {
        cout << "Order creation failed\n";
    }

    cout << "--- switch repository at runtime to Mongo and place another order ---\n";
    repo = &mongoRepo; // runtime switch
    OrderService orderService2(repo, &exchange);
    orderService2.createAndSaveOrder(userId, "TCS", 2);

    return 0;
}
