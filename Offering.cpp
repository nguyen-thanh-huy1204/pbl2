#include "Offering.h"
#include <sstream>

static string trimStr(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

Offering::Offering(): id(""), templateId(""), lecturerId("NULL"), maxStudents(0) {}
Offering::Offering(const string& id_, const string& templateId_, const string& lecturerId_, int max)
    : id(id_), templateId(templateId_), lecturerId(lecturerId_), maxStudents(max) {}

string Offering::getId() const { return id; }
string Offering::getTemplateId() const { return templateId; }
string Offering::getLecturerId() const { return lecturerId; }
int Offering::getMaxStudents() const { return maxStudents; }
Vector<string> Offering::getStudentIds() const { return students; }
const Vector<Schedule>& Offering::getSchedules() const { return schedules; }

void Offering::setLecturer(const string& lid) { lecturerId = lid; }
void Offering::addSchedule(const Schedule& s) { schedules.push_back(s); }
void Offering::addStudent(const string& sid) { if (students.findIndex(sid) == -1) students.push_back(sid); }
void Offering::removeStudent(const string& sid) { int idx = students.findIndex(sid); if (idx != -1) students.erase(idx); }
bool Offering::isFull() const { return students.size() >= maxStudents; }

string Offering::toLine() const {
    string out = id + "|" + templateId + "|" + lecturerId + "|";
    for (int i = 0; i < schedules.size(); ++i) {
        if (i) out += ";";
        out += schedules[i].toLine();
    }
    out += "|" + to_string(maxStudents) + "|";
    for (int i = 0; i < students.size(); ++i) {
        if (i) out += ";";
        out += students[i];
    }
    return out;
}

void Offering::fromLine(const string& line) {
    stringstream ss(line);
    if (!getline(ss, id, '|') || !getline(ss, templateId, '|') || !getline(ss, lecturerId, '|')) {
        throw runtime_error("Invalid offering line");
    }
    id = trimStr(id);
    templateId = trimStr(templateId);
    lecturerId = trimStr(lecturerId);

    string schedStr;
    if (getline(ss, schedStr, '|')) {
        stringstream sss(schedStr);
        string part;
        while (getline(sss, part, ';')) {
            part = trimStr(part);
            if (!part.empty()) {
                Schedule s; s.fromLine(part);
                schedules.push_back(s);
            }
        }
    }
    string maxStr;
    if (getline(ss, maxStr, '|')) maxStudents = stoi(trimStr(maxStr));
    string studentsStr;
    if (getline(ss, studentsStr)) {
        string cur;
        for (size_t i = 0; i < studentsStr.size(); ++i) {
            char ch = studentsStr[i];
            if (ch == ';') { if (!cur.empty()) { students.push_back(trimStr(cur)); cur.clear(); } }
            else cur.push_back(ch);
        }
        if (!cur.empty()) students.push_back(trimStr(cur));
    }
}
