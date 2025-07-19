#include<iostream>
#include<mutex>
using namespace std;

class Singleton {
    private:
    static Singleton* instance;
    static mutex mtx;
    Singleton () {
        cout<<"Singleton constructor\n";
    }
    public:
    static Singleton* getInstance(){
        if(instance == nullptr){
            lock_guard<mutex>lock(mtx);
            if(instance == nullptr){
              instance = new Singleton();
            }
        }
        return instance;
    }
};
Singleton* Singleton::instance = nullptr;
mutex Singleton::mtx;

int main(){
    Singleton *s1 = Singleton ::getInstance();
    Singleton *s2 = Singleton ::getInstance();
    cout<<(s1 == s2)<<endl;
}

// #include <iostream>
// #include <mutex>
// using namespace std;

// class Singleton {
// private:
//     // Step 1: Private constructor
//     Singleton() {
//         cout << "Singleton instance created\n";
//     }

//     // Step 2: Deleted copy constructor and assignment operator
//     Singleton(const Singleton&) = delete;
//     Singleton& operator=(const Singleton&) = delete;

// public:
//     // Step 3: Static method to access the instance
//     static Singleton& getInstance() {
//         static Singleton instance;  // Thread-safe in C++11 and beyond
//         return instance;
//     }

//     void showMessage() {
//         cout << "Hello from Singleton!\n";
//     }
// };

// // Step 4: Client code
// int main() {
//     Singleton& s1 = Singleton::getInstance();
//     s1.showMessage();

//     Singleton& s2 = Singleton::getInstance();
//     s2.showMessage();

//     cout << (&s1 == &s2 ? "Same instance\n" : "Different instances\n");
//     return 0;
// }



#include <iostream>
using namespace std;

class Singleton {
private:
    Singleton() {
        cout << "Singleton instance created\n";
    }

    // ❌ Prevent copying
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void showMessage() {
        cout << "Hello from Singleton!\n";
    }
};

int main() {
    // ✅ OK — proper way to get Singleton instance
    Singleton& s1 = Singleton::getInstance();
    s1.showMessage();

    // ❌ These will FAIL at compile time
    // Singleton s2 = s1;  // ❌ Copy constructor is deleted
    // Singleton s3;
    // s3 = s1;            // ❌ Assignment operator is deleted

    return 0;
}

