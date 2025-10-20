#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// ==================== User Class ====================
class User {
private:
    string name;
public:
    User(const string& user_name) : name(user_name) {}
    string getUserName() const { return name; }
};

// ==================== Room Interface ====================
class Room {
public:
    virtual int roomCapacity() const = 0;
    virtual bool isOccupied() const = 0;
    virtual void bookRoom() = 0;  // Mark room as occupied
    virtual ~Room() = default;
};

// ==================== MeetingRoom Class ====================
class MeetingRoom : public Room {
private:
    int roomNo;
    int capacity;
    bool occupied;
    string startTime;
    string endTime;
public:
    MeetingRoom(int room, int cap, const string& srtTime, const string& edTime)
        : roomNo(room), capacity(cap), occupied(false), startTime(srtTime), endTime(edTime) {}

    int roomCapacity() const override { return capacity; }
    bool isOccupied() const override { return occupied; }
    void bookRoom() override { occupied = true; }

    string getTime() const { return startTime + " - " + endTime; }
};

// ==================== Notification Class ====================
class Notification {
private:
    string text;
    string startTime;
    string endTime;
public:
    Notification(const string& txt, const string& start, const string& end)
        : text(txt), startTime(start), endTime(end) {}

    string getContent() const {
        return text + " from " + startTime + " to " + endTime;
    }
};

// ==================== Invitee Class ====================
class Invitee {
private:
    vector<shared_ptr<User>> invitees;
    shared_ptr<Notification> notification;
public:
    Invitee(shared_ptr<Notification> ntf) : notification(ntf) {}

    void addInvitee(shared_ptr<User> user) {
        invitees.push_back(user);
    }

    void notifyAll() const {
        for (const auto& user : invitees) {
            cout << "✅ Invitation sent to " << user->getUserName()
                 << " for " << notification->getContent() << endl;
        }
    }
};

// ==================== ScheduleMeeting Class ====================
class ScheduleMeeting {
private:
    string startTime;
    string endTime;
    int capacity;
public:
    ScheduleMeeting(const string& start, const string& end, int cap)
        : startTime(start), endTime(end), capacity(cap) {}

    bool isAvailable(shared_ptr<MeetingRoom> room, shared_ptr<User> user) {
        if (capacity <= room->roomCapacity() && !room->isOccupied()) {
            room->bookRoom();
            cout << "Meeting room booked by: " << user->getUserName() << endl;
            return true;
        }
        return false;
    }
};

// ==================== Main ====================
int main() {
    auto divyansh = make_shared<User>("Divyansh");
    auto saumya = make_shared<User>("Saumya");
    auto badal = make_shared<User>("Badal");

    auto meetingRm = make_shared<MeetingRoom>(101, 10, "10:30", "11:30");
    ScheduleMeeting meeting("10:30", "11:30", 9);

    auto notification = make_shared<Notification>("Sprint Planning", "10:30", "11:30");
    Invitee invite(notification);

    invite.addInvitee(divyansh);
    invite.addInvitee(saumya);
    invite.addInvitee(badal);

    if (meeting.isAvailable(meetingRm, divyansh)) {
        invite.notifyAll();
    } else {
        cout << "Room not available for meeting" << endl;
    }

    return 0;
}
