#include<iostream>
using namespace std;

class User {
private:
    string name;
public:
    User(string user_name) {
        name = user_name;
    }
    string getUserName() {
        return name;
    }
};

class Room {
public:
    virtual int roomCapacity() = 0;
    virtual bool isOccupied() = 0;
};

class MeetingRoom : public Room {
private:
    int roomNo;
    int capacity;
    string startTime;
    string endTime;
public:
    MeetingRoom(int room, int cap, string srtTime, string edTime) {
        roomNo = room;
        capacity = cap;
        startTime = srtTime;
        endTime = edTime;
    }
    int roomCapacity() override {
        return capacity;
    }
    bool isOccupied() override {
        return false;
    }
};

class ScheduleMeeting {
private:
    string startTime;
    string endTime;
    int capacity;
    User* user;
    MeetingRoom* mtr;
public:
    ScheduleMeeting(string startTime, string endTime, int cap) {
         startTime = startTime;
         endTime = endTime;
         capacity = cap;
    }
    bool isAvailable(MeetingRoom* mtr, User* user) {
        if(capacity < mtr->roomCapacity() && !mtr->roomCapacity()) {
            cout<<"Meeting room booked by: "<< user->getUserName() <<endl;
            return true;
        }
        return false;
    }
};

class INotification {
public:
    virtual string getContent() const = 0;
    virtual ~INotification() {};    
};

class SimpleNotification : public INotification {
private:
    string text;
    string startTime;
    string endTime;
public:
    SimpleNotification(const string& text, string startTime, string endTime) {
        this->text = text;
        this->startTime = startTime;
        this-> endTime = endTime;
    }
    string getContent() const override {
        return text + " " + startTime + "which ends at " + endTime; 
    }
};

class IObserver {
public:
    virtual void update() = 0;
    virtual ~IObserver() {};
};

class IObservable {
public:
    virtual void addObserver(IObserver *observer) = 0;
    virtual void removeObserver(IObserver *observer) = 0;
    virtual void notifyObservers() = 0;
};







int main() {
    User* user = new User("Divyansh");
    MeetingRoom* meetingRm = new MeetingRoom(101, 10, "10:30", "11:30");
    ScheduleMeeting *meet = new ScheduleMeeting("10:30", "11:30", 9);
    cout<< meet->isAvailable(meetingRm, user) <<endl;
    // cout<<meetingRm->roomCapacity()<<endl;
    
    
    return 0;
}
