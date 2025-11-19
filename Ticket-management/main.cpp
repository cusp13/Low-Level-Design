#include <bits/stdc++.h>
#include <atomic>
#include <mutex>
using namespace std;

// ----------------------- Utilities -----------------------
atomic<uint64_t> NEXT_ID{1};

uint64_t generateId() {
    return NEXT_ID.fetch_add(1, memory_order_relaxed);
}

string nowStr() {
    time_t t = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "%F %T", localtime(&t));
    return string(buf);
}

// ----------------------- Domain Models -----------------------
struct User {
    uint64_t id;
    string name;
    string email;
    string passwordHash; // placeholder

    User() = default;
    User(string n, string e, string p) : id(generateId()), name(n), email(e), passwordHash(p) {}
};

struct Project {
    uint64_t id;
    string key; // e.g., PROJ
    string name;
    uint64_t ownerId;

    Project() = default;
    Project(string k, string n, uint64_t owner) : id(generateId()), key(k), name(n), ownerId(owner) {}
};

enum class IssueStatus { TO_DO, IN_PROGRESS, DONE, CLOSED };

struct Comment {
    uint64_t id;
    uint64_t authorId;
    string body;
    string ts;

    Comment(uint64_t a, string b) : id(generateId()), authorId(a), body(b), ts(nowStr()) {}
};

struct Ticket {
    uint64_t id;
    string issueKey; // e.g., PROJ-1
    uint64_t projectId;
    uint64_t reporterId;
    optional<uint64_t> assigneeId;
    string title;
    string description;
    IssueStatus status;
    int priority; // 1 high .. 5 low
    string createdAt;
    string updatedAt;
    vector<Comment> comments;

    Ticket() = default;
    Ticket(string key, uint64_t projId, uint64_t reporter, string t, string desc, int prio)
        : id(generateId()), issueKey(key), projectId(projId), reporterId(reporter),
          title(t), description(desc), status(IssueStatus::TO_DO),
          priority(prio), createdAt(nowStr()), updatedAt(nowStr()) {}
};

// ----------------------- Notification Service -----------------------
class NotificationService {
public:
    void notifyUser(uint64_t userId, const string& message) {
        // In production: push to Kafka, send email/push, etc.
        lock_guard<mutex> l(m_);
        cout << "[Notification] to user " << userId << ": " << message << "\n";
    }
private:
    mutex m_;
};

// ----------------------- Auth Service (very simplified) -----------------------
class AuthService {
public:
    uint64_t registerUser(const string& name, const string& email, const string& password) {
        lock_guard<mutex> l(m_);
        // naive uniqueness check
        for (auto &p : users_) {
            if (p.second.email == email) {
                throw runtime_error("Email already registered");
            }
        }
        User u(name, email, hashPassword(password));
        users_[u.id] = u;
        emailToId_[email] = u.id;
        cout << "[Auth] registered user " << u.id << " (" << u.email << ")\n";
        return u.id;
    }

    // returns "token" (very naive)
    string login(const string& email, const string& password) {
        lock_guard<mutex> l(m_);
        auto it = emailToId_.find(email);
        if (it == emailToId_.end()) throw runtime_error("Invalid credentials");
        uint64_t uid = it->second;
        if (users_[uid].passwordHash != hashPassword(password)) throw runtime_error("Invalid credentials");
        // produce a simple token: "token:<userId>"
        string token = "token:" + to_string(uid);
        tokens_[token] = uid;
        cout << "[Auth] user " << uid << " logged in\n";
        return token;
    }

    uint64_t authenticate(const string& token) {
        lock_guard<mutex> l(m_);
        auto it = tokens_.find(token);
        if (it == tokens_.end()) throw runtime_error("Unauthorized");
        return it->second;
    }

private:
    string hashPassword(const string& p) {
        // placeholder - do NOT use in prod
        return "hash#" + p;
    }
    mutex m_;
    unordered_map<uint64_t, User> users_;
    unordered_map<string, uint64_t> emailToId_;
    unordered_map<string, uint64_t> tokens_;
};

// ----------------------- Project Service -----------------------
class ProjectService {
public:
    uint64_t createProject(const string& key, const string& name, uint64_t ownerId) {
        lock_guard<mutex> l(m_);
        Project p(key, name, ownerId);
        projects_[p.id] = p;
        projectsByKey_[p.key] = p.id;
        cout << "[Project] created " << p.key << " id=" << p.id << "\n";
        return p.id;
    }

    Project getProject(uint64_t projectId) {
        lock_guard<mutex> l(m_);
        if (!projects_.count(projectId)) throw runtime_error("Project not found");
        return projects_.at(projectId);
    }

    string keyForProject(uint64_t projectId) {
        return getProject(projectId).key;
    }

private:
    mutex m_;
    unordered_map<uint64_t, Project> projects_;
    unordered_map<string, uint64_t> projectsByKey_;
};

// ----------------------- Ticket Service -----------------------
class TicketService {
public:
    TicketService(ProjectService& ps, NotificationService& ns) : projSvc(ps), notifier(ns) {}

    // Create ticket, returns ticket id
    uint64_t createTicket(uint64_t projectId, uint64_t reporterId,
                          const string& title, const string& desc, int priority = 3) {
        lock_guard<mutex> l(m_);

        string issueKey = genIssueKey(projectId);
        Ticket t(issueKey, projectId, reporterId, title, desc, priority);
        tickets_[t.id] = t;

        // Notify watchers / assignee logic omitted here
        notifier.notifyUser(reporterId, "Ticket created: " + t.issueKey + " - " + t.title);

        cout << "[Ticket] created " << t.issueKey << " id=" << t.id << "\n";
        return t.id;
    }

    Ticket getTicket(uint64_t ticketId) {
        lock_guard<mutex> l(m_);
        if (!tickets_.count(ticketId)) throw runtime_error("Ticket not found");
        return tickets_.at(ticketId);
    }

    void assignTicket(uint64_t ticketId, uint64_t assigneeId, uint64_t actorId) {
        lock_guard<mutex> l(m_);
        auto it = tickets_.find(ticketId);
        if (it == tickets_.end()) throw runtime_error("Ticket not found");
        it->second.assigneeId = assigneeId;
        it->second.updatedAt = nowStr();
        notifier.notifyUser(assigneeId, "Assigned ticket: " + it->second.issueKey);
        cout << "[Ticket] " << it->second.issueKey << " assigned to " << assigneeId << "\n";
    }

    void changeStatus(uint64_t ticketId, IssueStatus newStatus, uint64_t actorId) {
        lock_guard<mutex> l(m_);
        auto it = tickets_.find(ticketId);
        if (it == tickets_.end()) throw runtime_error("Ticket not found");
        it->second.status = newStatus;
        it->second.updatedAt = nowStr();
        notifier.notifyUser(it->second.reporterId, "Status changed for " + it->second.issueKey);
    }

    void addComment(uint64_t ticketId, uint64_t authorId, const string& body) {
        lock_guard<mutex> l(m_);
        auto it = tickets_.find(ticketId);
        if (it == tickets_.end()) throw runtime_error("Ticket not found");
        Comment c(authorId, body);
        it->second.comments.push_back(c);
        it->second.updatedAt = nowStr();
        notifier.notifyUser(it->second.reporterId, "New comment on " + it->second.issueKey);
        cout << "[Ticket] comment added to " << it->second.issueKey << "\n";
    }

    vector<Ticket> listTicketsForProject(uint64_t projectId) {
        lock_guard<mutex> l(m_);
        vector<Ticket> out;
        for (auto &p : tickets_) {
            if (p.second.projectId == projectId) out.push_back(p.second);
        }
        return out;
    }

    // Very simple text search across title & description
    vector<Ticket> search(const string& q) {
        lock_guard<mutex> l(m_);
        vector<Ticket> out;
        string ql = toLower(q);
        for (auto &p : tickets_) {
            string t = toLower(p.second.title);
            string d = toLower(p.second.description);
            if (t.find(ql) != string::npos || d.find(ql) != string::npos) out.push_back(p.second);
        }
        return out;
    }

private:
    string genIssueKey(uint64_t projectId) {
        // Project key + numeric sequence
        string key = projSvc.keyForProject(projectId);
        uint64_t seq = ++projectSeq_[projectId];
        return key + "-" + to_string(seq);
    }

    static string toLower(const string& s) {
        string r = s;
        transform(r.begin(), r.end(), r.begin(), ::tolower);
        return r;
    }

    mutex m_;
    unordered_map<uint64_t, Ticket> tickets_;
    unordered_map<uint64_t, uint64_t> projectSeq_; // per-project sequence
    ProjectService& projSvc;
    NotificationService& notifier;
};

// ----------------------- API Gateway (simulated) -----------------------
class ApiGateway {
public:
    ApiGateway(AuthService& a, ProjectService& p, TicketService& t) : auth(a), projSvc(p), ticketSvc(t) {}

    // wrappers that simulate endpoints
    string registerUser(const string& name, const string& email, const string& pass) {
        uint64_t uid = auth.registerUser(name, email, pass);
        return to_string(uid);
    }

    string login(const string& email, const string& pass) {
        return auth.login(email, pass);
    }

    uint64_t createProject(const string& token, const string& key, const string& name) {
        uint64_t uid = auth.authenticate(token);
        return projSvc.createProject(key, name, uid);
    }

    uint64_t createTicket(const string& token, uint64_t projectId, const string& title, const string& desc, int priority = 3) {
        uint64_t uid = auth.authenticate(token);
        return ticketSvc.createTicket(projectId, uid, title, desc, priority);
    }

    void assignTicket(const string& token, uint64_t ticketId, uint64_t assigneeId) {
        uint64_t uid = auth.authenticate(token);
        ticketSvc.assignTicket(ticketId, assigneeId, uid);
    }

    void addComment(const string& token, uint64_t ticketId, const string& body) {
        uint64_t uid = auth.authenticate(token);
        ticketSvc.addComment(ticketId, uid, body);
    }

    vector<Ticket> searchTickets(const string& token, const string& q) {
        auth.authenticate(token);
        return ticketSvc.search(q);
    }

private:
    AuthService& auth;
    ProjectService& projSvc;
    TicketService& ticketSvc;
};

// ----------------------- Demo / Main -----------------------
int main() {
    NotificationService notifier;
    AuthService authSvc;
    ProjectService projSvc;
    // TicketService depends on project & notifier
    TicketService ticketSvc(projSvc, notifier);
    ApiGateway gw(authSvc, projSvc, ticketSvc);

    try {
        // Register users
        uint64_t aliceId = authSvc.registerUser("Alice", "alice@example.com", "alicepass");
        uint64_t bobId = authSvc.registerUser("Bob", "bob@example.com", "bobpass");

        // Login
        string aliceToken = authSvc.login("alice@example.com", "alicepass");
        string bobToken = authSvc.login("bob@example.com", "bobpass");

        // Create project
        uint64_t projId = gw.createProject(aliceToken, "PROJ", "Project X");

        // Alice creates ticket
        uint64_t t1 = gw.createTicket(aliceToken, projId, "Bug: login fails", "Login fails with 500 when password empty", 1);

        // Bob searches
        auto results = gw.searchTickets(bobToken, "login");
        cout << "Search results for 'login': " << results.size() << "\n";
        for (auto &tk : results) {
            cout << " - " << tk.issueKey << " : " << tk.title << " status=" << (int)tk.status << "\n";
        }

        // Assign ticket to Bob
        gw.assignTicket(aliceToken, t1, bobId);

        // Bob comments
        gw.addComment(bobToken, t1, "I am investigating this.");

        // Change status (direct call, could be exposed via API)
        ticketSvc.changeStatus(t1, IssueStatus::IN_PROGRESS, bobId);

        // List tickets in project
        auto list = ticketSvc.listTicketsForProject(projId);
        cout << "Tickets in project " << projId << ": " << list.size() << "\n";
        for (auto &tk : list) {
            cout << " * " << tk.issueKey << " - " << tk.title << " (assignee: " 
                 << (tk.assigneeId ? to_string(*tk.assigneeId) : string("none")) << ")\n";
        }

    } catch (const exception& ex) {
        cerr << "ERROR: " << ex.what() << "\n";
    }

    return 0;
}
