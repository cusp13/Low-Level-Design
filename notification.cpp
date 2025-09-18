#include<iostream>
#include<string>
using namespace std;

class INotification {
public:
    virtual string getContent() = 0;
};

class SimpleNotification : public INotification {
public:
    string text;
    string getContent() override {
        return "Sending simple notification";
    }
};

class INotificationDecorator : public INotification {
protected:
   INotification* notification;
public:
   INotificationDecorator(INotification* n) {
       this->notification = n;
   }
};

class TimestampDecorator : public INotificationDecorator {
public:
    TimestampDecorator(INotification* n) : INotificationDecorator(n){};
    string getContent() override {
        return notification -> getContent() + " with timestamp";
    }
};

class SignatureDecorator : public INotificationDecorator {
public:
    SignatureDecorator(INotification* n) : INotificationDecorator(n){};
    string getContent() override {
        return notification->getContent() + " with signature";
    }
};

class INotificationService {
public:
    vector<INotification*> notification;
    virtual void sendNotification() = 0;
};

class IObservable {
    vector<IObserver*> observers;
    virtual void add(IObserver* obs)  = 0;
    virtual void remove(IObserver* obs)  = 0;
    virtual void notify() = 0;
   virtual ~IObservable() {};
};

class IObserver {
    virtual void update() = 0;
   virtual ~IObserver() {};
};

// class NotificationObser

int main() {
    cout<<"Running notification service"<<endl;
    
    SimpleNotification *notify = new SimpleNotification;
    cout<<notify->getContent()<<endl;
    
    INotification *signature = new SignatureDecorator(notify);
    cout<<signature->getContent()<<endl;
    
    INotification* timestamp = new TimestampDecorator(notify);
    cout<<timestamp->getContent()<<endl;
    
    return 0;
}