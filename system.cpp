#include "System.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <limits>
#include <vector>
using namespace std;

static string trimStr(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

// Simplified: assume input data is already Vietnamese without diacritics.
// Return the original string unchanged (minimal processing).
static string removeDiacritics(const string &s) {
    return s;
}

// Credit policy
static const int MIN_CREDITS_ALLOWED = 18; // informational
static const int MAX_CREDITS_ALLOWED = 24;

System::System(const string& folder) : dataFolder(folder) {
    loadAll();

    if (!users.contains("U000")) {
        Admin* a = new Admin("U000", "admin", "123");
        users.insert("U000", a);
    }
}


System::~System() {
    Vector<string> ukeys = users.keys();
    for (int i = 0; i < ukeys.size(); ++i)
        delete users.get(ukeys[i]);
}

string System::nextUserId() { int n = users.keys().size() + 1; char buf[8]; sprintf(buf, "U%03d", n); return buf; }
string System::nextStudentId() { int n = students.keys().size() + 1; char buf[8]; sprintf(buf, "S%03d", n); return buf; }
string System::nextLecturerId() { int n = lecturers.keys().size() + 1; char buf[8]; sprintf(buf, "L%03d", n); return buf; }
string System::nextCourseId() { int n = courses.keys().size() + 1; char buf[8]; sprintf(buf, "C%03d", n); return buf; }
string System::nextOfferingId() { int n = offerings.keys().size() + 1; char buf[12]; sprintf(buf, "O%04d", n); return buf; }

 void System::loadAll() {
    try {
        loadUsers();
        loadStudents();
        loadLecturers();
        loadCourses();
        loadOfferings();
    } catch (const exception& e) {
        cout << "Error loading data: " << e.what() << endl;
        throw;
    }
 }

void System::saveAll() {
    string userPath = dataFolder + "/users.txt";
    ofstream fu(userPath);
    if (!fu) {
        cout << "Failed to open file for writing: " << userPath << endl;
        return;
    }
    Vector<string> ukeys = users.keys();
    for (int i = 0; i < ukeys.size(); ++i) {
        User* u = users.get(ukeys[i]);
        // Persist the actual password stored in the User object so users can log in later.
        fu << u->getId() << "|" << u->getUsername() << "|" << u->getPassword() << "|" << u->getRole() << "|\n";
    }
    fu.close();

    ofstream fs("data/students.txt");
    Vector<string> skeys = students.keys();
    for (int i = 0; i < skeys.size(); ++i)
        fs << students.get(skeys[i]).toLine() << "\n";
    fs.close();

    ofstream fl("data/lecturers.txt");
    Vector<string> lkeys = lecturers.keys();
    for (int i = 0; i < lkeys.size(); ++i)
        fl << lecturers.get(lkeys[i]).toLine() << "\n";
    fl.close();

    ofstream fc("data/courses.txt");
    Vector<string> ckeys = courses.keys();
    for (int i = 0; i < ckeys.size(); ++i)
        fc << courses.get(ckeys[i]).toLine() << "\n";
    fc.close();
    
    ofstream fo("data/offerings.txt");
    Vector<string> okeys = offerings.keys();
    for (int i = 0; i < okeys.size(); ++i)
        fo << offerings.get(okeys[i]).toLine() << "\n";
    fo.close();
}

void System::loadUsers() {
    string path = dataFolder + "/users.txt";
    ifstream fin(path);
    if (!fin) {
        cout << "Failed to open users.txt" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        // Remove trailing CR
        if (!line.empty() && line.back() == '\r') line.pop_back();

        string id, uname, pw, role;
        stringstream ss(line);
        getline(ss, id, '|'); 
        getline(ss, uname, '|'); 
        getline(ss, pw, '|'); 
        getline(ss, role, '|');

        // normalize whitespace
        id = trimStr(id);
        uname = trimStr(uname);
        pw = trimStr(pw);
        role = trimStr(role);

        if (id.empty() || uname.empty() || pw.empty() || role.empty()) {
            cout << "Skipping invalid line: " << line << endl;
            continue;
        }

        users.insert(id, new User(id, uname, pw, role));
    }
}

void System::loadStudents() {
    string path = dataFolder + "/students.txt";
    ifstream fin(path);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Student s;
        try {
            s.fromLine(line);
            students.insert(s.getId(), s);
            if (!users.contains(s.getId()))
                users.insert(s.getId(), new User(s.getId(), s.getUsername(), "123", "STUDENT"));
        } catch (...) {
            cout << "Skipping invalid student line: " << line << endl;
        }
    }
}

void System::loadLecturers() {
    string path = dataFolder + "/lecturers.txt";
    ifstream fin(path);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Lecturer l; l.fromLine(line);
        lecturers.insert(l.getId(), l);
        if (!users.contains(l.getId()))
            users.insert(l.getId(), new User(l.getId(), l.getUsername(), "123", "LECTURER"));
    }
}

void System::loadCourses() {
    string path = dataFolder + "/courses.txt";
    ifstream fin(path);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Course c; c.fromLine(line);
        courses.insert(c.getId(), c);
    }
}

void System::loadOfferings() {
    string path = dataFolder + "/offerings.txt";
    ifstream fin(path);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        // remove trailing CR if present
        if (!line.empty() && line.back() == '\r') line.pop_back();
        Offering o; 
        try {
            o.fromLine(line);
            offerings.insert(o.getId(), o);

            // ensure students listed in offering are enrolled in their Student records
            Vector<string> sids = o.getStudentIds();
            const Vector<Schedule>& scheds = o.getSchedules();
            for (int i = 0; i < sids.size(); ++i) {
                string sid = sids[i];
                if (students.contains(sid)) {
                    Student &s = students.get(sid);
                    if (!s.isEnrolled(o.getId())) {
                        for (int j = 0; j < scheds.size(); ++j) {
                            s.enroll(o.getId(), scheds[j]);
                        }
                    }
                }
            }
        } catch (...) {
            cout << "Skipping invalid offering line: " << line << "\n";
        }
    }
}

void System::run() {
    cout << endl;
    while (true) {
        system("cls");
        cout << "=== PBL2 University System ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register (student/lecturer)" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose: " << flush;
        
        int ch;
        while (!(cin >> ch)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number (0-2): ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ch == 0) {
            saveAll();
            break;
        }
        else if (ch == 1) {
            string uid, role;
            if (login(uid, role)) {
                cout << "Login successful as " << role << endl;
                if (role == "ADMIN") {
                    int cmd;
                    do {
                        cout << "\n=== Admin Menu ===\n"
                             << "1. Add course\n2. Assign lecturer\n"
                             << "3. List students in course\n4. Show all courses\n"
                             << "0. Logout\nChoice: ";
                        cin >> cmd;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        
                        if (cmd == 1) adminAddCourse();
                        else if (cmd == 2) adminAssignLecturer();
                        else if (cmd == 3) adminListStudentsInCourse();
                        else if (cmd == 4) showAllCourses();
                        
                        if (cmd != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.get();
                        }
                    } while (cmd != 0);
                }
                else if (role == "LECTURER") {
                    string lid = uid;
                    int cmd;
                    do {
                        cout << "\n=== Lecturer Menu ===\n"
                             << "1. Show my courses\n2. Show students of course\n"
                             << "3. Mark student course completion\n"
                             << "0. Logout\nChoice: ";
                        cin >> cmd;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        
                        if (cmd == 1) showLecturerCourses(lid);
                        else if (cmd == 2) {
                            cout << "Course id: ";
                            string cid;
                            getline(cin, cid);
                            showCourseStudents(cid);
                        }
                        else if (cmd == 3) {
                            cout << "Offering id: ";
                            string oid;
                            getline(cin, oid);
                            if (!offerings.contains(oid)) {
                                cout << "Invalid offering ID\n";
                            } else if (offerings.get(oid).getLecturerId() != lid) {
                                cout << "You are not teaching this offering\n";
                            } else {
                                markCourseCompleted(lid, oid);
                            }
                        }
                        
                        if (cmd != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.get();
                        }
                    } while (cmd != 0);
                }
                else if (role == "STUDENT") {
                    string sid = uid;
                    int cmd;
                    do {
                        cout << "\n=== Student Menu ===\n"
                             << "1. Show offered courses\n2. Enroll course\n"
                             << "3. Withdraw course\n4. Show my schedule\n"
                             << "5. View completed courses\n"
                             << "0. Logout\nChoice: ";
                        cin >> cmd;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        
                        if (cmd == 1) showAllCourses();
                        else if (cmd == 2) studentEnroll(sid);
                        else if (cmd == 3) studentWithdraw(sid);
                        else if (cmd == 4) showStudentSchedule(sid);
                        else if (cmd == 5) showCompletedCourses(sid);
                        
                        if (cmd != 0) {
                            cout << "\nPress Enter to continue...";
                            cin.get();
                        }
                    } while (cmd != 0);
                }
            }
        }
        else if (ch == 2) {
            registerUser();
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

bool System::login(string& outId, string& outRole) {
    cout << "Username: "; 
    string uname; 
    cin.clear();
    cin >> uname;
    
    cout << "Password: "; 
    string pw; 
    cin.clear();
    cin >> pw;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline from input buffer

    Vector<string> keys = users.keys();
    for (int i = 0; i < keys.size(); ++i) {
        User* u = users.get(keys[i]);
        string storedUsername = u->getUsername();
        string storedPassword = u->getPassword();
        
        if (!storedUsername.empty() && storedUsername.back() == '\r') storedUsername.pop_back();
        if (!storedPassword.empty() && storedPassword.back() == '\r') storedPassword.pop_back();

        if (storedUsername == uname && storedPassword == pw) {
            outId = u->getId();
            outRole = u->getRole();
            cout << "\nLogin success: " << outId << " (" << outRole << ")\n";
            cout << "=== Loading menu for " << outRole << " ===\n";
            return true;
        }
    }

    cout << "\nLogin failed - Invalid username or password\n";
    return false;
}


void System::registerUser() {
    cout << "Register as (student/lecturer): ";
    string role; cin >> role;
    if (role != "student" && role != "lecturer") { 
        cout << "Invalid role\n"; 
        return; 
    }

    cout << "Username: "; string uname; cin >> uname;
    cout << "Password: "; string pw; cin >> pw;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Full name: "; string fullname; getline(cin, fullname);
    cout << (role=="student" ? "Major: " : "Department: "); string extra; getline(cin, extra);

    if (role == "student") {
        string sid = nextStudentId();
        Student s(sid, uname, pw, fullname, extra);
        students.insert(sid, s);
        users.insert(sid, new User(sid, uname, pw, "STUDENT"));
        cout << "Registered student id: " << sid << "\n";
    } else {
        string lid = nextLecturerId();
        Lecturer l(lid, uname, pw, fullname, extra);
        lecturers.insert(lid, l);
        users.insert(lid, new User(lid, uname, pw, "LECTURER"));
        cout << "Registered lecturer id: " << lid << "\n";
    }

    saveAll();
}

void System::adminAddCourse() {
    cin.ignore();
    cout << "Course name: "; string name; getline(cin, name);
    cout << "Credits: "; int cr; cin >> cr;
    cout << "Max students: "; int maxs; cin >> maxs;
    string cid = nextCourseId();
    Course c(cid, name, cr, "NULL", maxs);

    // Prerequisites input
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Prerequisite course IDs (separated by ';', leave blank if none): ";
    string prereqLine; getline(cin, prereqLine);
    if (!prereqLine.empty()) {
        string cur;
        for (size_t i = 0; i < prereqLine.size(); ++i) {
            char ch = prereqLine[i];
            if (ch == ';') { if (!cur.empty()) { c.addPrerequisite(cur); cur.clear(); } }
            else cur.push_back(ch);
        }
        if (!cur.empty()) c.addPrerequisite(cur);
    }

    cout << "Number of schedules for this course: ";
    int n; cin >> n;
    for (int i=0;i<n;i++) {
        cout << "Schedule " << i+1 << " - Day: "; string day; cin >> day;
        cout << "Start HH:MM: "; string st; cin >> st;
        cout << "End HH:MM: "; string en; cin >> en;
        cout << "Room: "; string room; cin >> room;
        c.addSchedule(Schedule(day, Schedule::parseTime(st), Schedule::parseTime(en), room));
    }

    courses.insert(cid, c);
    cout << "Added course: " << cid << "\n";
    saveAll();
}

void System::adminAssignLecturer() {
    showAllCourses();
    cout << "Offering id: "; string oid; cin >> oid;
    if (!offerings.contains(oid)) { cout << "Not found\n"; return; }
    cout << "Lecturers:\n";
    Vector<string> lkeys = lecturers.keys();
    for (int i=0;i<lkeys.size();++i) lecturers.get(lkeys[i]).displayShort();
    cout << "Enter lecturer id to assign: "; string lid; cin >> lid;
    if (!lecturers.contains(lid)) { cout<<"No lecturer\n"; return; }
    Offering& o = offerings.get(oid);
    o.setLecturer(lid);
    lecturers.get(lid).addTeaching(oid);
    cout << "Assigned\n";
    saveAll();
}

void System::adminListStudentsInCourse() {
    cout << "Offering id: "; string oid; cin >> oid;
    if (!offerings.contains(oid)) { cout << "No offering\n"; return; }
    Offering& o = offerings.get(oid);
    
    // Get template name for display
    string templateId = o.getTemplateId();
    string courseName = "-";
    if (courses.contains(templateId)) {
        courseName = courses.getConst(templateId).getName();
    }
    
    cout << "Students in offering " << oid << " (" << courseName << "):\n";
    Vector<string> sids = o.getStudentIds();
    for (int i = 0; i < sids.size(); ++i) {
        if (students.contains(sids[i])) {
            students.get(sids[i]).displayShort();
        }
    }
}

// studentEnroll: student enrolls in an offering (by offering ID)
// Check prerequisites against the offering's template course
void System::studentEnroll(const string& sid) {
    cout << "Offering id to enroll: "; string inputOid; cin >> inputOid;
    // trim possible CR and whitespace from input
    if (!inputOid.empty() && inputOid.back() == '\r') inputOid.pop_back();
    string oid = trimStr(inputOid);

    if (!students.contains(sid)) { 
        cout << "Invalid student (not found in map)\n"; 
        return; 
    }

    // Try to find offering by ID, or by searching templates
    if (!offerings.contains(oid)) {
        // Try to match against offering IDs or template names
        auto toUpper = [&](string t) {
            for (size_t i = 0; i < t.size(); ++i) t[i] = toupper((unsigned char)t[i]);
            return t;
        };
        string typedU = toUpper(oid);
        Vector<string> okeys = offerings.keys();
        vector<string> matches;
        
        for (int i = 0; i < okeys.size(); ++i) {
            const Offering &o = offerings.getConst(okeys[i]);
            string idU = toUpper(o.getId());
            string templateId = o.getTemplateId();
            
            // Match against offering ID or template course name
            if (idU == typedU || idU.find(typedU) != string::npos) {
                matches.push_back(o.getId());
            } else if (courses.contains(templateId)) {
                string nameU = toUpper(courses.getConst(templateId).getName());
                if (nameU.find(typedU) != string::npos) {
                    matches.push_back(o.getId());
                }
            }
        }

        if (matches.size() == 1) {
            oid = matches[0];
            cout << "Auto-selected offering: " << oid << "\n";
        } else {
            cout << "Invalid offering ID: '" << oid << "'\n";
            if (matches.size() > 0) {
                cout << "Multiple potential matches found:\n";
                for (size_t i = 0; i < matches.size(); ++i) {
                    if (offerings.contains(matches[i])) {
                        string templateId = offerings.getConst(matches[i]).getTemplateId();
                        if (courses.contains(templateId)) {
                            cout << " - " << matches[i] << " : " << courses.getConst(templateId).getName() << "\n";
                        }
                    }
                }
                cout << "Please enter the exact offering ID.\n";
            } else {
                cout << "Available offerings:\n";
                for (int i = 0; i < okeys.size(); ++i) {
                    const Offering &o = offerings.getConst(okeys[i]);
                    string templateId = o.getTemplateId();
                    if (courses.contains(templateId)) {
                        cout << " - " << o.getId() << " : " << courses.getConst(templateId).getName() << "\n";
                    }
                }
            }
            return;
        }
    }

    Student& s = students.get(sid);
    Offering& o = offerings.get(oid);
    
    // Get the template course to check prerequisites and credit
    string templateId = o.getTemplateId();
    if (!courses.contains(templateId)) {
        cout << "Course template not found\n";
        return;
    }
    
    Course& c = courses.get(templateId);
    
    // Check if full
    if (o.isFull()) { 
        cout << "Offering full\n"; 
        return; 
    }
    
    // Check if already enrolled
    if (s.isEnrolled(oid)) { 
        cout << "Already enrolled\n"; 
        return; 
    }

    // Credit limit check: sum current enrolled credits using template credits
    double currentCredits = 0.0;
    Vector<string> enrolled = s.getEnrolledCourseIds();
    for (int ei = 0; ei < enrolled.size(); ++ei) {
        string enrOid = enrolled[ei];
        // Look up template for this offering to get its credit
        if (offerings.contains(enrOid)) {
            string enrTemplateId = offerings.getConst(enrOid).getTemplateId();
            if (courses.contains(enrTemplateId)) {
                currentCredits += courses.getConst(enrTemplateId).getCredit();
            }
        }
    }
    double newTotal = currentCredits + c.getCredit();
    if (newTotal > MAX_CREDITS_ALLOWED) {
        cout << "Cannot enroll: total credits would be " << newTotal << " > " << MAX_CREDITS_ALLOWED << "\n";
        cout << "Current credits: " << currentCredits << ", course credits: " << c.getCredit() << "\n";
        cout << "Allowed credit range: " << MIN_CREDITS_ALLOWED << " - " << MAX_CREDITS_ALLOWED << "\n";
        return;
    }

    // prerequisite check: student must have completed all prerequisite courses (from template)
    Vector<string> prereqs = c.getPrerequisites();
    for (int pi = 0; pi < prereqs.size(); ++pi) {
        string pre = prereqs[pi];
        if (!s.hasCompleted(pre)) {
            cout << "Missing prerequisite: " << pre << " - cannot enroll\n";
            return;
        }
    }

    // conflict check with offering schedules
    const Vector<Schedule>& oscheds = o.getSchedules();
    for (size_t i = 0; i < oscheds.size(); ++i) {
        if (s.hasConflictWithCourseSchedule(oscheds[i])) { 
            cout << "Schedule conflict!\n"; 
            return; 
        }
    }

    // enroll all offering schedules
    for (size_t i = 0; i < oscheds.size(); ++i) {
        s.enroll(oid, oscheds[i]);
    }

    // add student to offering
    o.addStudent(sid);
    cout << "Enrolled successfully\n";
    saveAll();
}

void System::studentWithdraw(const string& sid) {
    cout << "Offering id to withdraw: "; string oid; cin >> oid;
    if (!students.contains(sid) || !offerings.contains(oid)) { 
        cout << "Invalid\n"; 
        return; 
    }
    Student& s = students.get(sid);
    Offering& o = offerings.get(oid);
    if (!s.isEnrolled(oid)) { 
        cout << "Not enrolled\n"; 
        return; 
    }
    s.withdraw(oid);
    o.removeStudent(sid);
    cout << "Withdrawn\n";
    saveAll();
}

void System::showStudentSchedule(const string& sid) {
    if (!students.contains(sid)) return;
    Student& s = students.get(sid);

    Vector<string> oids = s.getEnrolledCourseIds();  // Actually offering IDs
    Vector<Schedule> scheds = s.getEnrolledSchedules();

    cout << "\n========================================\n";
    cout << "THOI KHOA BIEU CUA " << s.getFullname() << " (" << s.getId() << ")\n";
    cout << "========================================\n";
    if (oids.size() == 0) {
        cout << "CHUA DANG KY MON HOC NAO.\n";
        cout << "========================================\n";
        return;
    }

    const int creditColW = 6;
    cout << left << setw(12) << "MA MON HOC" 
        << left << setw(30) << "TEN MON HOC"
        << right << setw(creditColW) << "TC" << " "
        << left << setw(20) << "LICH HOC"
        << left << setw(10) << "PHONG HOC" << "\n";
    cout << string(90, '-') << "\n";

    for (int i = 0; i < oids.size() && i < scheds.size(); ++i) {
        string oid = oids[i];  // offering ID (in current migration, same as template ID)
        string cname = "-";
        double credit = 0.0;
        string room = scheds[i].getRoom();
        
        // Look up course template by offering ID (they're equal in current migration)
        if (courses.contains(oid)) {
            const Course &cc = courses.getConst(oid);
            cname = cc.getName();
            credit = cc.getCredit();
        }
        
        // format credit with one decimal if needed
        ostringstream cs;
        cs << fixed << setprecision(1) << credit;
        cout << left << setw(12) << oid
             << left << setw(30) << cname
             << right << setw(creditColW) << cs.str() << " "
             << left << setw(20) << scheds[i].toReadable()
             << left << setw(10) << room << "\n";
    }
    cout << "========================================\n";
}

void System::markCourseCompleted(const string& lid, const string& oid) {
    // Find the offering to mark students as completed
    if (!offerings.contains(oid)) {
        cout << "Offering not found\n";
        return;
    }
    
    Offering& o = offerings.get(oid);
    
    // Verify this lecturer teaches this offering
    if (o.getLecturerId() != lid) {
        cout << "You do not teach this offering\n";
        return;
    }
    
    // Get the template course to determine what course ID to mark as completed
    string templateId = o.getTemplateId();
    if (!courses.contains(templateId)) {
        cout << "Course template not found\n";
        return;
    }

    // Show enrolled students for this offering
    cout << "\nEnrolled students in offering " << oid << ":\n";
    Vector<string> enrolledStudents = o.getStudentIds();
    for (int i = 0; i < enrolledStudents.size(); ++i) {
        string sid = enrolledStudents[i];
        if (students.contains(sid)) {
            const Student& s = students.get(sid);
            cout << "[" << s.getId() << "] " << s.getUsername() << "\n";
        }
    }

    cout << "\nEnter student ID to mark as completed (or press Enter to cancel): ";
    string sid;
    getline(cin, sid);
    if (sid.empty()) return;

    if (!students.contains(sid)) {
        cout << "Invalid student ID\n";
        return;
    }

    Student& s = students.get(sid);
    if (!s.isEnrolled(oid)) {
        cout << "This student is not enrolled in this offering\n";
        return;
    }

    // Mark the template course as completed (not the offering)
    s.completeCourse(templateId);
    
    // Withdraw from the offering
    s.withdraw(oid);
    o.removeStudent(sid);
    cout << "Course marked as completed. Student withdrawn from offering.\n";
    
    saveAll();
}

void System::showCompletedCourses(const string& sid) {
    if (!students.contains(sid)) return;
    const Student& s = students.get(sid);
    Vector<string> completed = s.getCompletedCourseIds();
    
    if (completed.size() == 0) {
        cout << "\nNo completed courses yet.\n";
        return;
    }

    cout << "\nCompleted courses:\n";
    cout << "+--------+--------------------------------+-------+\n";
    cout << "| ID     | Course Name                    | Grade |\n";
    cout << "+--------+--------------------------------+-------+\n";
    
    for (int i = 0; i < completed.size(); ++i) {
        string cid = completed[i];
        if (courses.contains(cid)) {
            const Course& c = courses.getConst(cid);
            cout << "| " << left << setw(6) << c.getId() << " | "
                 << left << setw(30) << c.getName() << " | Pass  |\n";
        }
    }
    cout << "+--------+--------------------------------+-------+\n";
}

void System::showLecturerCourses(const string& lid) {
    cout << "Courses taught by " << lid << ":\n";
    Vector<string> okeys = offerings.keys();
    for (int i = 0; i < okeys.size(); ++i) {
        const Offering &o = offerings.getConst(okeys[i]);
        if (o.getLecturerId() == lid) {
            string templateId = o.getTemplateId();
            if (courses.contains(templateId)) {
                const Course &c = courses.getConst(templateId);
                cout << "[" << o.getId() << "] " << c.getName() 
                     << " (" << fixed << setprecision(1) << c.getCredit() << " tc)\n";
            }
        }
    }
}

void System::showCourseStudents(const string& cid) {
    if (!courses.contains(cid)) { cout<<"No course\n"; return; }
    Vector<string> sids = courses.get(cid).getStudentIds();
    cout<<"Students:\n";
    for (int i=0;i<sids.size();++i)
        if (students.contains(sids[i]))
            students.get(sids[i]).displayShort();
}

void System::showAllCourses() const {
    Vector<string> okeys = offerings.keys();  // Iterate offerings, not courses

    // column width defaults (can be tuned)
    const int idW = 10;
    const int creditW = 4; // e.g. "2.5"
    const int roomW = 6;
    const int timeW = 16;
    const int lecturerW = 16;
    const int nameMaxAllowed = 24;

    // compute maximum name length in data
    int realNameMax = 10; // minimal
    for (int i = 0; i < okeys.size(); ++i) {
        const Offering &o = offerings.getConst(okeys[i]);
        string templateId = o.getTemplateId();
        if (courses.contains(templateId)) {
            int ln = (int)courses.getConst(templateId).getName().size();
            if (ln > realNameMax) realNameMax = ln;
        }
    }
    int nameW = min(realNameMax, nameMaxAllowed);

    // helper to wrap text into multiple lines of given width
    auto wrapText = [](const string &s, int w) -> vector<string> {
        vector<string> lines;
        string remaining = s;
        while (!remaining.empty()) {
            if ((int)remaining.size() <= w) {
                lines.push_back(remaining + string(w - remaining.size(), ' '));
                break;
            }
            // Find the last space within the width limit
            int spacePos = w;
            while (spacePos > 0 && remaining[spacePos] != ' ') spacePos--;
            if (spacePos == 0) spacePos = w; // If no space found, force break at width

            lines.push_back(remaining.substr(0, spacePos) + string(w - spacePos, ' '));
            remaining = remaining.substr(spacePos == w ? w : spacePos + 1);
        }
        return lines;
    };

    // helper to fit string into width without wrapping
    auto fit = [&](const string &s, int w) {
        if ((int)s.size() <= w) return s + string(w - (int)s.size(), ' ');
        if (w <= 3) return string(w, '.');
        return s.substr(0, w-3) + "...";
    };

    // build header row into string to compute exact border length
    ostringstream hdr;
    hdr << "| " << left << setw(idW) << "MA" 
        << " " << left << setw(nameW) << "TEN MON HOC"
        << " " << left << setw(lecturerW) << "GIANG VIEN"
        << " " << right << setw(creditW) << "TC"
        << " " << left << setw(roomW) << "PHONG"
        << " " << left << setw(timeW) << "LICH HOC" << " |";
    string hdrStr = hdr.str();
    string border = "+" + string(hdrStr.length()-2, '-') + "+";

    cout << "\n" << border << "\n";
    cout << hdrStr << "\n";
    cout << border << "\n";

    for (int i = 0; i < okeys.size(); ++i) {
        const Offering &o = offerings.getConst(okeys[i]);
        string templateId = o.getTemplateId();
        
        // Get course template info
        string courseName = "-";
        double credit = 0.0;
        if (courses.contains(templateId)) {
            const Course &c = courses.getConst(templateId);
            courseName = c.getName();
            credit = c.getCredit();
        }

        // Get offering details (lecturer, schedules)
        string lecturerName = "-";
        if (o.getLecturerId() != "NULL" && !o.getLecturerId().empty()) {
            if (lecturers.contains(o.getLecturerId())) {
                lecturerName = lecturers.getConst(o.getLecturerId()).getFullname();
            } else lecturerName = o.getLecturerId();
        }

        const Vector<Schedule>& scheds = o.getSchedules();

        // first (main) row: offering id, course name, lecturer, credit, then first schedule
        string room = "-", times = "-";
        if (scheds.size() > 0) {
            room = scheds[0].getRoom();
            times = scheds[0].toReadable();
        }

        // Wrap the course name into multiple lines
        vector<string> nameLines = wrapText(courseName, nameW);

        // First row with first line of name and first schedule (if any)
        ostringstream row;
        row << "| " << left << setw(idW) << o.getId()
            << " " << left << nameLines[0]
            << " " << left << setw(lecturerW) << fit(lecturerName, lecturerW)
            << " " << right << setw(creditW) << fixed << setprecision(1) << credit
            << " " << left << setw(roomW) << room
            << " " << left << setw(timeW) << times << " |";
        cout << row.str() << "\n";

        // Additional rows for wrapped name lines
        for (size_t ni = 1; ni < nameLines.size(); ++ni) {
            ostringstream nameLine;
            nameLine << "| " << left << setw(idW) << ""
                     << " " << left << nameLines[ni]
                     << " " << left << setw(lecturerW) << ""
                     << " " << right << setw(creditW) << ""
                     << " " << left << setw(roomW) << ""
                     << " " << left << setw(timeW) << "" << " |";
            cout << nameLine.str() << "\n";
        }

        // Additional schedules printed in extra rows with empty id/name/credit
        for (size_t j = 1; j < scheds.size(); ++j) {
            ostringstream ex;
            ex << "| " << left << setw(idW) << "" 
               << " " << left << setw(nameW) << ""
               << " " << left << setw(lecturerW) << ""
               << " " << right << setw(creditW) << ""
               << " " << left << setw(roomW) << scheds[j].getRoom()
               << " " << left << setw(timeW) << scheds[j].toReadable() << " |";
            cout << ex.str() << "\n";
        }

        if (i < okeys.size() - 1) cout << border << "\n";
    }

    if (okeys.size() == 0) cout << "|" << string(hdrStr.length()-2, ' ') << "|\n";
    cout << "\n" << border << "\n";
    cout << "Tong so mon hoc: " << okeys.size() << "\n";
}
