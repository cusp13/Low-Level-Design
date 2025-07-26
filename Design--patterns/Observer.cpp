// 🧠 Why use const string& instead of string?
// Version	                       What Happens                                                  	Performance
// string videoTitle	         Makes a full copy of the string every time you call the function	⛔ Slower if string is large
// const string& videoTitle	     Just points to the original string — no copy	                    ✅ Much faster
// string& videoTitle	         Modifiable reference — could change original	                    ⚠️ Risky if you don’t want to change it

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// Interface for Subscriber
class ISubscriber {
public:
    virtual void update(const string& videoTitle) = 0;
    virtual ~ISubscriber() {};
};

// Interface for Channel
class IChannel {
public:
    virtual void subscribe(ISubscriber* subscriber) = 0;
    virtual void unsubscribe(ISubscriber* subscriber) = 0;
    virtual void notifySubscribers(const string& videoTitle) = 0;
    virtual ~IChannel() {};
};

// Concrete Channel
class Channel : public IChannel {
    string name;
    vector<ISubscriber*> subscribers;

public:
    Channel(const string& name) {
        this->name = name;
    }

    void subscribe(ISubscriber* subscriber) override {
        if (find(subscribers.begin(), subscribers.end(), subscriber) == subscribers.end()) {
            subscribers.push_back(subscriber);
        }
    }

    void unsubscribe(ISubscriber* subscriber) override {
        auto it = find(subscribers.begin(), subscribers.end(), subscriber);
        if (it != subscribers.end()) {
            subscribers.erase(it);
        }
    }

    void notifySubscribers(const string& videoTitle) override {
        for (ISubscriber* subscriber : subscribers) {
            subscriber->update(videoTitle);
        }
    }

    void uploadVideo(const string& videoTitle) {
        cout << "Channel " << name << " uploaded a new video: " << videoTitle << endl;
        notifySubscribers(videoTitle);
    }
};

// Concrete Subscriber
class User : public ISubscriber {
    string username;
public:
    User(const string& name) {
        username = name;
    }

    void update(const string& videoTitle) override {
        cout << username << " got notified about: " << videoTitle << endl;
    }
};

// Driver
int main() {
    Channel techChannel("TechVision");

    User alice("Alice");
    User bob("Bob");

    techChannel.subscribe(&alice);
    techChannel.subscribe(&bob);

    techChannel.uploadVideo("Large Cap fund Video");

    techChannel.unsubscribe(&bob);

    techChannel.uploadVideo("Mid Cap fund Video");

    return 0;
}
