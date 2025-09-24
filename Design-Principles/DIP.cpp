#include<iostream>
using namespace std;

class Database {
public:
    virtual void save (string data) = 0;
};
// Low level module
class SQLDatabase: public Database {
public:
    void save(string data) override {
        cout<<"Executing SQL Query: "<<data<<endl;
    }
};
// Low level module
class MongoDatabase : public Database {
public:
    void save(string data) override {
        cout<<"Executing Mongo Query: " <<data<<endl;
    }
};


// High level module with loose coupling.
class UserService {
private:
    Database* db; // (dependency injection);
public:
    UserService(Database * database) {
        db = database;
    }
    void storeUser(string user){
        db->save(user);
    }
};

int main(){
    SQLDatabase sql;
    MongoDatabase mongo;
    
    UserService service1(&sql);
    UserService service2(&mongo);
    
    service1.storeUser("Divyansh");
    service2.storeUser("Sharma");
};
