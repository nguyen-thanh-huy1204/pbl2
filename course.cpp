#include "Course.h"
#include <sstream>

static string trimStr(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

Course::Course(): credit(0), lecturerId("NULL"), maxStudents(0) {}
Course::Course(const string& id_, const string& name_, double credit_, const string& lecturerId_, int maxStu)
    : id(id_), name(name_), credit(credit_), lecturerId(lecturerId_), maxStudents(maxStu) {}

string Course::getId() const { return id; }
string Course::getName() const { return name; }
double Course::getCredit() const { return credit; }
Vector<string> Course::getStudentIds() const {return studentIds;}  
string Course::getLecturerId() const { return lecturerId; }
int Course::getMaxStudents() const { return maxStudents; }

void Course::setLecturer(const string& lid) { lecturerId = lid; }
void Course::setMaxStudents(int m) { maxStudents = m; }

void Course::addSchedule(const Schedule& s) { schedules.push_back(s); }
const Vector<Schedule>& Course::getSchedules() const { return schedules; }
Vector<Schedule>& Course::getSchedules() { return schedules; }

bool Course::isFull() const { return students.size() >= maxStudents; }
bool Course::hasStudent(const string& sid) const { return students.findIndex(sid) != -1; }
void Course::addStudent(const string& sid) { if (!hasStudent(sid)) students.push_back(sid); }
void Course::removeStudent(const string& sid) { int idx = students.findIndex(sid); if (idx!=-1) students.erase(idx); }

string Course::toLine() const {
    // template format: id|name|credit|prereq1;prereq2
    string out = id + "|" + name + "|" + to_string(credit) + "|";
    for (int i = 0; i < prerequisites.size(); ++i) {
        if (i) out += ";";
        out += prerequisites[i];
    }
    return out;
}

void Course::fromLine(const string& line) {
    stringstream ss(line);
    if (!getline(ss, id, '|') || !getline(ss, name, '|')) {
        throw runtime_error("Invalid course template line");
    }
    string creditStr;
    if (!getline(ss, creditStr, '|')) creditStr = "0";
    id = trimStr(id);
    name = trimStr(name);
    credit = stod(trimStr(creditStr));

    string prereqStr;
    if (getline(ss, prereqStr)) {
        stringstream pss(prereqStr);
        string prereq;
        while (getline(pss, prereq, ';')) {
            prereq = trimStr(prereq);
            if (!prereq.empty() && prereq != "NULL") prerequisites.push_back(prereq);
        }
    }
}

string Course::infoString() const {
    string out = "[" + id + "] " + name + " (" + to_string((int)credit) + "tc)";
    if (prerequisites.size() > 0) {
        out += " | Pre: ";
        for (int i = 0; i < prerequisites.size(); ++i) {
            if (i) out += ",";
            out += prerequisites[i];
        }
    }
    return out;
}
