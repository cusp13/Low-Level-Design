#include<iostream>
#include<vector>
#include<string>
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
         this->startTime = startTime;
         this->endTime = endTime;
         this->capacity = cap;
    }
    bool isAvailable(MeetingRoom* mtr, User* user) {
        if(capacity < mtr->roomCapacity() && !mtr->isOccupied()) {
            cout<<"Meeting room booked by: "<< user->getUserName() <<endl;
            return true;
        }
        return false;
    }
};


class Notification {
private:
    string text;
    string startTime;
    string endTime;
public:
    Notification(const string& text, string startTime, string endTime) {
        this->text = text;
        this->startTime = startTime;
        this-> endTime = endTime;
    }
    string getContent() {
        return text + " " + startTime + " which ends at " + endTime; 
    }
};

class Invitee {
private:
    Notification* notification;
public:
    Invitee(Notification* ntf) {
        notification = ntf;
    }
    vector<User*> invitees;
    void addInvitees(User* user) {
        invitees.push_back(user);
    }
    
    string NotifyInvitee() {
        for(auto it: invitees) {
          cout<<"✅ Invitation send to " + it->getUserName() + " for " + notification->getContent()<<endl;
        }
        return "🚀Notified all users";
    }
};

int main() {
    User* user = new User("Divyansh");
    MeetingRoom* meetingRm = new MeetingRoom(101, 10, "10:30", "11:30");
    ScheduleMeeting *meet = new ScheduleMeeting("10:30", "11:30", 9);
    User *saumya = new User("Saumya");
    User *badal = new User("Badal");
    Notification* notification = new Notification("Sprint Planning", "10:30", "11:30");
    Invitee* invite = new Invitee(notification);
    invite->addInvitees(saumya);
    invite->addInvitees(badal);
    invite->addInvitees(user);
    if(meet->isAvailable(meetingRm, user)) {
        cout<<invite->NotifyInvitee()<<endl;
    } else {
        cout<<"Room not available for meeting"<<endl;
    }
    
    
    
    return 0;
};
