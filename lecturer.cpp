#include "Lecturer.h"
#include <sstream>
#include <iostream>

static string trimStr(const string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

Lecturer::Lecturer(): User(), fullname(""), department("") {}
Lecturer::Lecturer(const string& id, const string& username, const string& password,
                   const string& fullname, const string& department)
    : User(id, username, password, "LECTURER"), fullname(fullname), department(department) {}

void Lecturer::addTeaching(const string& cid) { if (teaching.findIndex(cid) == -1) teaching.push_back(cid); }
bool Lecturer::teaches(const string& cid) const { return teaching.findIndex(cid) != -1; }

string Lecturer::toLine() const {
    string out = id + "|" + username + "|" + password + "|" + role + "|" + fullname + "|" + department + "|";
    for (int i = 0; i < teaching.size(); ++i) {
        if (i) out += ";";
        out += teaching[i];
    }
    return out;
}

void Lecturer::fromLine(const string& line) {
    stringstream ss(line);
    
    // Doc fields according to format: id|username|password|role|fullname|department|
    string roleField;
    if (!getline(ss, id, '|') || 
        !getline(ss, username, '|') ||
        !getline(ss, password, '|') ||
        !getline(ss, roleField, '|') ||
        !getline(ss, fullname, '|') ||
        !getline(ss, department, '|')) {
        throw runtime_error("Thieu truong du lieu cho lecturer");
    }
    
    // normalize
    id = trimStr(id);
    username = trimStr(username);
    password = trimStr(password);
    roleField = trimStr(roleField);
    fullname = trimStr(fullname);
    department = trimStr(department);

    // set role from file
    role = roleField;
    
    string rest;
    getline(ss, rest, '|');
    string cur;
    for (size_t i=0;i<rest.size();++i) {
        char ch = rest[i];
        if (ch==';') { if (!cur.empty()){ teaching.push_back(cur); cur.clear(); } }
        else cur.push_back(ch);
    }
    if (!cur.empty()) teaching.push_back(cur);
}

void Lecturer::displayShort() const { cout << "[" << id << "] " << fullname << " - " << department << "\n"; }
