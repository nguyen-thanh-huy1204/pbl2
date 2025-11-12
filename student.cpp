#include "Student.h"
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

static string trimStr(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

Student::Student(): User(), fullname(""), major("") {}
Student::Student(const string& id, const string& username, const string& password,
                 const string& fullname, const string& major)
    : User(id, username, password, "STUDENT"), fullname(fullname), major(major) {}

bool Student::isEnrolled(const string& cid) const {
    for (int i = 0; i < enrolledCourseIds.size(); ++i)
        if (enrolledCourseIds[i] == cid) return true;
    return false;
}

void Student::enroll(const string& cid, const Schedule& sch) {
    if (!isEnrolled(cid)) {
        enrolledCourseIds.push_back(cid);
        enrolledSchedules.push_back(sch);
    }
}

void Student::withdraw(const string& cid) {
    for (int i = 0; i < enrolledCourseIds.size(); ++i) {
        if (enrolledCourseIds[i] == cid) {
            enrolledCourseIds.erase(i);
            enrolledSchedules.erase(i);
            return;
        }
    }
}

bool Student::hasCompleted(const string& cid) const {
    for (int i = 0; i < completedCourseIds.size(); ++i)
        if (completedCourseIds[i] == cid) return true;
    return false;
}

void Student::completeCourse(const string& cid) {
    if (!hasCompleted(cid)) completedCourseIds.push_back(cid);
}

bool Student::hasConflictWithCourseSchedule(const Schedule& newSch) const {
    for (int i = 0; i < enrolledSchedules.size(); ++i) {
        if (enrolledSchedules[i].conflictsWith(newSch)) return true;
    }
    return false;
}

string Student::toLine() const {
    // id|username|password|role|fullname|major|enrollPart|completed1;completed2
    string out = id + "|" + username + "|" + password + "|" + role + "|" + fullname + "|" + major + "|";
    for (int i = 0; i < enrolledCourseIds.size(); ++i) {
        if (i) out += ";";
        out += enrolledCourseIds[i] + "," + enrolledSchedules[i].toLine();
    }
    out += "|";
    for (int i = 0; i < completedCourseIds.size(); ++i) {
        if (i) out += ";";
        out += completedCourseIds[i];
    }
    return out;
}

void Student::fromLine(const string& line) {
    stringstream ss(line);
    
    // Format: id|username|password|role|fullname|major|enrollPart|completed
    if (!getline(ss, id, '|') || 
        !getline(ss, username, '|') ||
        !getline(ss, password, '|') ||
        !getline(ss, role, '|') ||
        !getline(ss, fullname, '|') ||
        !getline(ss, major, '|')) {
        throw runtime_error("Khong du truong du lieu cho student");
    }
    
    // Read enrolled list (until next '|')
    string rest;
    getline(ss, rest, '|');

    // normalize
    id = trimStr(id);
    username = trimStr(username);
    password = trimStr(password);
    role = trimStr(role);
    fullname = trimStr(fullname);
    major = trimStr(major);

    enrolledCourseIds.clear(); enrolledSchedules.clear();
    string cur;
    for (size_t i = 0; i < rest.size(); ++i) {
        char ch = rest[i];
        if (ch == ';') {
            if (!cur.empty()) {
                size_t p = cur.find(',');
                if (p != string::npos) {
                    string cid = cur.substr(0, p);
                    string schedPart = cur.substr(p+1);
                    Schedule s; s.fromLine(schedPart);
                    cid = trimStr(cid);
                    enrolledCourseIds.push_back(cid);
                    enrolledSchedules.push_back(s);
                }
                cur.clear();
            }
        } else cur.push_back(ch);
    }
    if (!cur.empty()) {
        size_t p = cur.find(',');
        if (p != string::npos) {
            string cid = cur.substr(0, p);
            string schedPart = cur.substr(p+1);
            Schedule s; s.fromLine(schedPart);
            cid = trimStr(cid);
            enrolledCourseIds.push_back(cid);
            enrolledSchedules.push_back(s);
        }
    }

    // Read completed list (optional)
    string completedStr;
    if (getline(ss, completedStr)) {
        completedCourseIds.clear();
        string cur2;
        for (size_t i = 0; i < completedStr.size(); ++i) {
            char ch = completedStr[i];
            if (ch == ';') { if (!cur2.empty()) { completedCourseIds.push_back(trimStr(cur2)); cur2.clear(); } }
            else cur2.push_back(ch);
        }
        if (!cur2.empty()) completedCourseIds.push_back(trimStr(cur2));
    }
}

void Student::displayShort() const {
    cout << "[" << id << "] " << fullname << " - " << major << "\n";
}

// return enrolled schedules (parallel to enrolledCourseIds)
Vector<Schedule> Student::getEnrolledSchedules() const {
    return enrolledSchedules;
}

void Student::displaySchedule() const {
    cout << "\n========================================\n";
    cout << "THOI KHOA BIEU CUA " << fullname << " (" << id << ")\n";
    cout << "========================================\n";
    
    if (enrolledCourseIds.size() == 0) {
        cout << "CHUA DANG KY MON HOC NAO.\n";
    } else {
        cout << left << setw(12) << "MA MON HOC" 
             << left << setw(30) << "TEN MON HOC"
             << left << setw(20) << "LICH HOC"
             << left << setw(10) << "PHONG HOC" << "\n";
        cout << "----------------------------------------"
             << "----------------------------------------\n";
        
        for (int i = 0; i < enrolledCourseIds.size(); ++i) {
            // Note: We only have course ID and schedule, not course name
            // In a better system, we would look up the course name from the course object
            cout << left << setw(12) << enrolledCourseIds[i]
                 << left << setw(30) << ""  // No course name available here
                 << left << setw(20) << enrolledSchedules[i].toReadable()
                 << left << setw(10) << enrolledSchedules[i].getRoom() << "\n";
        }
    }
    cout << "========================================\n";
}
