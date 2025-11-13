#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;
using Clock = chrono::system_clock;
using TimePoint = chrono::time_point<Clock>;

enum class VehicleType {
    CAR,
    TRUCK,
    SEDAN,
    BIKE,
    OTHER
};

static string vehicleTypeToString(VehicleType vt) {
    switch (vt) {
        case VehicleType::CAR: return "CAR";
        case VehicleType::TRUCK: return "TRUCK";
        case VehicleType::SEDAN: return "SEDAN";
        case VehicleType::BIKE: return "BIKE";
        default: return "OTHER";
    }
}

class Vehicle {
private:
    string vehicleNumber;
    VehicleType vehicleType;
    string parkingSlot; // optional: store assigned slot id

public:
    Vehicle(const string &vehicleNumber, VehicleType vehicleType)
        : vehicleNumber(vehicleNumber), vehicleType(vehicleType) {}

    void setVehicleNumber(const string &vn) { vehicleNumber = vn; }
    void setVehicleType(VehicleType vt) { vehicleType = vt; }
    void setParkingSlot(const string &slot) { parkingSlot = slot; }

    string getVehicleNumber() const { return vehicleNumber; }
    VehicleType getVehicleType() const { return vehicleType; }
    string getParkingSlot() const { return parkingSlot; }
};

// Forward declare
class Ticket;

class ParkingSlot {
public:
    string slotId;
    VehicleType slotType;
    bool isAvailable;
    shared_ptr<Vehicle> currentVehicle;

    ParkingSlot(string id, VehicleType t)
        : slotId(id), slotType(t), isAvailable(true), currentVehicle(nullptr) {}

    bool assignVehicle(shared_ptr<Vehicle> v) {
        if (!isAvailable) return false;
        currentVehicle = v;
        isAvailable = false;
        v->setParkingSlot(slotId);
        return true;
    }

    void removeVehicle() {
        if (currentVehicle) currentVehicle->setParkingSlot("");
        currentVehicle = nullptr;
        isAvailable = true;
    }
};

enum class PaymentType {
    CASH,
    UPI,
    CARD
};

class PaymentStrategy {
protected:
    PaymentType payType;
public:
    PaymentStrategy(PaymentType t) : payType(t) {}
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount) = 0;
    PaymentType getType() const { return payType; }
};

class CashPayment : public PaymentStrategy {
public:
    CashPayment(): PaymentStrategy(PaymentType::CASH) {}
    bool pay(double amount) override {
        cout << "[Payment] Paid ₹" << fixed << setprecision(2) << amount << " in cash.\n";
        return true;
    }
};

class UpiPayment : public PaymentStrategy {
    string upiId;
public:
    UpiPayment(const string &upi = "user@upi"): PaymentStrategy(PaymentType::UPI), upiId(upi) {}
    bool pay(double amount) override {
        cout << "[Payment] Paid ₹" << fixed << setprecision(2) << amount
             << " via UPI (" << upiId << ").\n";
        return true;
    }
};

class CardPayment : public PaymentStrategy {
    string cardNumber;
public:
    CardPayment(const string &card = "xxxx-xxxx-xxxx-1234"): PaymentStrategy(PaymentType::CARD), cardNumber(card) {}
    bool pay(double amount) override {
        cout << "[Payment] Paid ₹" << fixed << setprecision(2) << amount
             << " via Card (" << cardNumber << ").\n";
        return true;
    }
};

class Ticket {
private:
    string ticketId;
    shared_ptr<Vehicle> vehicle;
    shared_ptr<ParkingSlot> slot;
    TimePoint entryTime;
    TimePoint exitTime;
    double amount;
public:
    Ticket(const string &id, shared_ptr<Vehicle> v, shared_ptr<ParkingSlot> s, TimePoint inTime)
        : ticketId(id), vehicle(v), slot(s), entryTime(inTime), amount(0.0) {}

    string getTicketId() const { return ticketId; }
    shared_ptr<Vehicle> getVehicle() const { return vehicle; }
    shared_ptr<ParkingSlot> getSlot() const { return slot; }
    TimePoint getEntryTime() const { return entryTime; }
    TimePoint getExitTime() const { return exitTime; }
    double getAmount() const { return amount; }

    void closeTicket(TimePoint outTime, double amt) {
        exitTime = outTime;
        amount = amt;
    }

    string timePointToString(TimePoint tp) const {
        time_t t = Clock::to_time_t(tp);
        std::tm tm = *localtime(&t);
        std::ostringstream oss;
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void printTicket() const {
        cout << "---- Ticket ----\n";
        cout << "TicketId : " << ticketId << "\n";
        cout << "Vehicle  : " << vehicle->getVehicleNumber() << " (" << vehicleTypeToString(vehicle->getVehicleType()) << ")\n";
        cout << "Slot     : " << (slot ? slot->slotId : "N/A") << "\n";
        cout << "Entry    : " << timePointToString(entryTime) << "\n";
        if (amount > 0.0) {
            cout << "Exit     : " << timePointToString(exitTime) << "\n";
            cout << "Amount   : ₹" << fixed << setprecision(2) << amount << "\n";
        } else {
            cout << "Exit     : Not exited yet\n";
        }
        cout << "----------------\n";
    }
};

class TicketManager {
private:
    map<string, shared_ptr<Ticket>> tickets;
    unsigned long counter = 0;

    // Simple rate per hour for different vehicle types
    map<VehicleType, double> ratesPerHour = {
        {VehicleType::CAR, 20.0},
        {VehicleType::SEDAN, 18.0},
        {VehicleType::TRUCK, 50.0},
        {VehicleType::BIKE, 5.0},
        {VehicleType::OTHER, 10.0}
    };

    string generateTicketId() {
        ++counter;
        ostringstream oss;
        oss << "TKT-" << setw(6) << setfill('0') << counter;
        return oss.str();
    }

    double computeAmount(shared_ptr<Ticket> ticket, TimePoint exitTime) {
        auto entry = ticket->getEntryTime();
        auto dur = chrono::duration_cast<chrono::minutes>(exitTime - entry).count();
        double hours = ceil(dur / 60.0); // charge by full hour
        VehicleType vt = ticket->getVehicle()->getVehicleType();
        double rate = ratesPerHour.count(vt) ? ratesPerHour[vt] : ratesPerHour[VehicleType::OTHER];
        return hours * rate;
    }

public:
    TicketManager() = default;

    shared_ptr<Ticket> createTicket(shared_ptr<Vehicle> vehicle, shared_ptr<ParkingSlot> slot) {
        string id = generateTicketId();
        TimePoint now = Clock::now();
        auto ticket = make_shared<Ticket>(id, vehicle, slot, now);
        tickets[id] = ticket;
        return ticket;
    }

    // returns amount charged, and processes payment (using a PaymentStrategy)
    bool closeTicket(const string &ticketId, TimePoint exitTime, PaymentStrategy &payment, double &outAmount) {
        if (tickets.find(ticketId) == tickets.end()) {
            cout << "[TicketManager] Ticket not found: " << ticketId << "\n";
            return false;
        }
        auto ticket = tickets[ticketId];
        if (ticket->getAmount() > 0.0) { // already closed
            cout << "[TicketManager] Ticket already closed: " << ticketId << "\n";
            outAmount = ticket->getAmount();
            return true;
        }

        double amount = computeAmount(ticket, exitTime);
        ticket->closeTicket(exitTime, amount);

        // Process payment
        bool paid = payment.pay(amount);
        if (paid) {
            // free the slot
            if (auto slotPtr = ticket->getSlot()) {
                slotPtr->removeVehicle();
            }
            outAmount = amount;
            // Optionally remove ticket from active list or keep for history
            // tickets.erase(ticketId);
            return true;
        } else {
            cout << "[TicketManager] Payment failed for ticket: " << ticketId << "\n";
            return false;
        }
    }

    void printActiveTickets() const {
        cout << "[TicketManager] Active Tickets:\n";
        for (const auto &p : tickets) {
            p.second->printTicket();
        }
    }
};

class ParkingLot {
private:
    map<VehicleType, vector<shared_ptr<ParkingSlot>>> slots;
    shared_ptr<TicketManager> ticketManager;

public:
    ParkingLot(shared_ptr<TicketManager> tm) : ticketManager(tm) {}

    void addSlot(const string &slotId, VehicleType vt) {
        auto s = make_shared<ParkingSlot>(slotId, vt);
        slots[vt].push_back(s);
    }

    // find first available slot for vehicle type
    shared_ptr<ParkingSlot> findAvailableSlot(VehicleType vt) {
        if (slots.find(vt) == slots.end()) return nullptr;
        for (auto &s : slots[vt]) {
            if (s->isAvailable) return s;
        }
        // try OTHER type if specific not found
        if (slots.find(VehicleType::OTHER) != slots.end()) {
            for (auto &s : slots[VehicleType::OTHER]) if (s->isAvailable) return s;
        }
        return nullptr;
    }

    shared_ptr<Ticket> parkVehicle(shared_ptr<Vehicle> v) {
        auto slot = findAvailableSlot(v->getVehicleType());
        if (!slot) {
            cout << "[ParkingLot] No available slot for type: " << vehicleTypeToString(v->getVehicleType()) << "\n";
            return nullptr;
        }
        bool assigned = slot->assignVehicle(v);
        if (!assigned) {
            cout << "[ParkingLot] Failed assigning vehicle to slot\n";
            return nullptr;
        }
        auto ticket = ticketManager->createTicket(v, slot);
        cout << "[ParkingLot] Vehicle parked. TicketId: " << ticket->getTicketId() << "\n";
        return ticket;
    }

    void printSlots() const {
        cout << "--- Parking Slots ---\n";
        for (const auto &kv : slots) {
            cout << vehicleTypeToString(kv.first) << " slots:\n";
            for (const auto &s : kv.second) {
                cout << "  " << s->slotId << " - " << (s->isAvailable ? "Available" : "Occupied") << "\n";
            }
        }
    }
};

// Demo main
int main() {
    cout << "Designing parking lot system (demo)\n";

    auto ticketManager = make_shared<TicketManager>();
    ParkingLot lot(ticketManager);

    // create some slots
    lot.addSlot("C-01", VehicleType::CAR);
    lot.addSlot("C-02", VehicleType::CAR);
    lot.addSlot("B-01", VehicleType::BIKE);
    lot.addSlot("T-01", VehicleType::TRUCK);
    lot.addSlot("O-01", VehicleType::OTHER);

    lot.printSlots();

    // create a vehicle
    auto vehicle = make_shared<Vehicle>("UP60M1213", VehicleType::CAR);

    // park vehicle
    auto ticket = lot.parkVehicle(vehicle);
    if (!ticket) {
        cout << "Parking failed.\n";
        return 0;
    }

    ticket->printTicket();

    // simulate time pass: create exit time 90 minutes later
    TimePoint exitTime = ticket->getEntryTime() + chrono::minutes(90);

    // choose payment strategy (UPI)
    UpiPayment upi("demo@upi");

    double chargedAmount = 0.0;
    bool closed = ticketManager->closeTicket(ticket->getTicketId(), exitTime, upi, chargedAmount);
    if (closed) {
        cout << "[Main] Ticket closed. Charged: ₹" << fixed << setprecision(2) << chargedAmount << "\n";
    } else {
        cout << "[Main] Failed to close ticket.\n";
    }

    cout << "\nFinal slot status:\n";
    lot.printSlots();

    cout << "\nAll tickets:\n";
    ticketManager->printActiveTickets();

    return 0;
}
