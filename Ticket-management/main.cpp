+----------------+              +--------------------+
|    Logger      |              |    AuthService     |
+----------------+              +--------------------+
| + now():string |              | - emailToId: map   |
| + info(msg)    |              | - tokenToId: map   |
| + warn(msg)    |              | - users: map       |
| + error(msg)   |              +--------------------+
+----------------+              | + registerUser(..) |
                                | + login(..)        |
                                | + auth(..)         |
                                +--------------------+
                                         |
                                         |
                                         v
                                +--------------------+
                                |       User         |
                                +--------------------+
                                | - id:int           |
                                | - name:string      |
                                | - email:string     |
                                | - password:string  |
                                +--------------------+

+--------------------+            1      *      +----------------+
|   ProjectService   |--------------------------|    Project     |
+--------------------+                          +----------------+
| - projects: map    |                          | - id:int       |
| - seq: map         |                          | - key:string   |
+--------------------+                          | - name:string  |
| + createProject()  |                          | - owner:int    |
| + getKey()         |                          +----------------+
| + nextSeq()        |
+--------------------+

+--------------------+             1     *      +----------------+
|   TicketService    |---------------------------|    Ticket     |
+--------------------+                           +----------------+
| - tickets: map     |                           | - id:int      |
| - ps: ProjectService&|                         | - projectId   |
+--------------------+                           | - reporterId  |
| + createTicket()   |                           | - assignee    |
| + assign()         |                           | - key         |
| + comment()        |                           | - title       |
| + markOpen()       |                           | - desc        |
| + markClosed()     |                           | - status      |
| + search()         |                           | - state       |
+--------------------+                           | - comments[]  |
                                                  +----------------+
                                                             |
                                                             | 1  *
                                                             |
                                                   +----------------+
                                                   |    Comment     |
                                                   +----------------+
                                                   | - id:int       |
                                                   | - author:int   |
                                                   | - text:string  |
                                                   +----------------+

Enums:
+-------------------+
|     Status        |
| TO_DO, IN_PROGRESS|
| DONE              |
+-------------------+

+-------------------+
|   TicketState     |
|   OPEN, CLOSED    |
+-------------------+


#include <bits/stdc++.h>
using namespace std;

// ---------------- Logger ----------------
class Logger {
public:
    static string now() {
        time_t t = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return string(buf);
    }

    static void info(const string& msg) {
        cout << "[" << now() << "] [INFO] " << msg << "\n";
    }

    static void warn(const string& msg) {
        cout << "[" << now() << "] [WARN] " << msg << "\n";
    }

    static void error(const string& msg) {
        cout << "[" << now() << "] [ERROR] " << msg << "\n";
    }
};

// simple id generator
int NEXT_ID = 1;
int genId() {
    int id = NEXT_ID++;
    Logger::info("Generated new ID: " + to_string(id));
    return id;
}

// ---------------- Models ----------------

struct User {
    int id;
    string name, email, password;

    User() : id(0) {}

    User(string n, string e, string p)
        : id(genId()), name(n), email(e), password(p) {}
};

struct Project {
    int id;
    string key, name;
    int owner;

    Project() : id(0), owner(0) {}

    Project(string k, string n, int o)
        : id(genId()), key(k), name(n), owner(o) {}
};

enum class Status { TO_DO, IN_PROGRESS, DONE };
enum class TicketState { OPEN, CLOSED };   // NEW STATE

struct Comment {
    int id;
    int author;
    string text;

    Comment() : id(0), author(0) {}

    Comment(int a, string t)
        : id(genId()), author(a), text(t) {}
};

struct Ticket {
    int id;
    int projectId;
    int reporterId;
    int assignee;
    string key, title, desc;
    Status status;
    TicketState state;              // NEW OPEN/CLOSED STATE
    vector<Comment> comments;

    Ticket()
        : id(0), projectId(0), reporterId(0),
          assignee(-1), status(Status::TO_DO),
          state(TicketState::OPEN) {}   // default OPEN

    Ticket(string k, int p, int r, string t, string d)
        : id(genId()), key(k), projectId(p), reporterId(r),
          assignee(-1), title(t), desc(d),
          status(Status::TO_DO),
          state(TicketState::OPEN) {}   // default OPEN
};

// ---------------- Auth Service ----------------
class AuthService {
    unordered_map<string, int> emailToId;
    unordered_map<string, int> tokenToId;
    unordered_map<int, User> users;

public:
    int registerUser(string n, string e, string p) {
        Logger::info("Registering user: " + e);

        if (emailToId.count(e))
            throw runtime_error("Email already exists");

        User u(n, e, p);
        users.emplace(u.id, u);
        emailToId[e] = u.id;

        Logger::info("User registered with ID: " + to_string(u.id));
        return u.id;
    }

    string login(string email, string pass) {
        Logger::info("Login attempt for email: " + email);

        if (!emailToId.count(email))
            throw runtime_error("Invalid login");

        int uid = emailToId[email];
        if (users[uid].password != pass)
            throw runtime_error("Invalid password");

        string token = "token_" + to_string(uid);
        tokenToId[token] = uid;

        Logger::info("Login successful. Token issued: " + token);
        return token;
    }

    int auth(string token) {
        Logger::info("Authenticating token: " + token);

        if (!tokenToId.count(token))
            throw runtime_error("Unauthorized");

        Logger::info("Token valid. User ID: " + to_string(tokenToId[token]));
        return tokenToId[token];
    }
};

// ---------------- Project Service ----------------
class ProjectService {
    unordered_map<int, Project> projects;
    unordered_map<int, int> seq;

public:
    int createProject(string key, string name, int owner) {
        Logger::info("Creating project: " + key);

        Project p(key, name, owner);
        projects.emplace(p.id, p);

        Logger::info("Project created with ID: " + to_string(p.id));
        return p.id;
    }

    string getKey(int pid) {
        Logger::info("Fetching project key for project ID: " + to_string(pid));
        return projects[pid].key;
    }

    int nextSeq(int pid) {
        seq[pid]++;
        Logger::info("Project " + to_string(pid) +
                     " next sequence: " + to_string(seq[pid]));
        return seq[pid];
    }
};

// ---------------- Ticket Service ----------------
class TicketService {
    unordered_map<int, Ticket> tickets;
    ProjectService& ps;

public:
    TicketService(ProjectService& p) : ps(p) {}

    int createTicket(int pid, int reporter, string title, string desc) {
        Logger::info("Creating ticket for project ID: " + to_string(pid));

        string key = ps.getKey(pid) + "-" + to_string(ps.nextSeq(pid));
        Ticket t(key, pid, reporter, title, desc);
        tickets.emplace(t.id, t);

        Logger::info("Ticket created with ID: " + to_string(t.id) +
                     " and key: " + key);
        return t.id;
    }

    void assign(int tid, int user) {
        Logger::info("Assigning ticket " + to_string(tid) +
                     " to user " + to_string(user));
        tickets[tid].assignee = user;
    }

    void comment(int tid, int author, string text) {
        Logger::info("Adding comment to ticket " + to_string(tid));
        tickets[tid].comments.emplace_back(author, text);
    }

    // ---- NEW FUNCTIONS ----
    void markOpen(int tid) {
        Logger::info("Marking ticket " + to_string(tid) + " as OPEN");

        if (!tickets.count(tid))
            throw runtime_error("Ticket not found");

        tickets[tid].state = TicketState::OPEN;
    }

    void markClosed(int tid) {
        Logger::info("Marking ticket " + to_string(tid) + " as CLOSED");

        if (!tickets.count(tid))
            throw runtime_error("Ticket not found");

        tickets[tid].state = TicketState::CLOSED;
    }

    vector<Ticket> search(string q) {
        Logger::info("Searching tickets with query: " + q);

        vector<Ticket> out;
        for (auto& p : tickets) {
            Ticket& t = p.second;
            if (t.title.find(q) != string::npos ||
                t.desc.find(q) != string::npos) {
                Logger::info("Matched ticket ID: " + to_string(t.id));
                out.push_back(t);
            }
        }

        if (out.empty())
            Logger::warn("No tickets matched search query.");

        return out;
    }
};

// ---------------- Demo ----------------
int main() {
    try {
        Logger::info("---- System Start ----");

        AuthService auth;
        ProjectService ps;
        TicketService ts(ps);

        int alice = auth.registerUser("Alice", "a@mail", "a1");
        int bob   = auth.registerUser("Bob", "b@mail", "b1");

        string token = auth.login("a@mail", "a1");

        int pid = ps.createProject("PRJ", "Project X", alice);

        int tid = ts.createTicket(pid, alice, "Login Bug", "Login returns 500");

        ts.assign(tid, bob);
        ts.comment(tid, bob, "Working on it...");

        // NEW FEATURE USAGE
        ts.markClosed(tid);
        ts.markOpen(tid);

        auto results = ts.search("login");
        cout << results.size() << " tickets found\n";

        Logger::info("---- System End ----");
    }
    catch (exception& e) {
        Logger::error("Exception: " + string(e.what()));
    }

    return 0;
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------
