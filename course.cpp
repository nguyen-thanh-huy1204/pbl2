#include "Course.h"
#include <sstream>

Course::Course(const string& id, const string& name, int cr)
    : courseID(id), courseName(name), credits(cr) {}

void Course::display() const {
    cout << courseID << " - " << courseName
         << " (" << credits << " tin chi)" << endl;
}

void Course::saveToFile(ostream& out) const {
    string tmp = courseName;
    for (auto& ch : tmp) {
        if (ch == ' ') ch = '_';
    }
    out << courseID << " " << tmp << " " << credits << "\n";
}

bool Course::loadFromFile(istream& in) {
    string id, name;
    int cr;
    if (!(in >> id >> name >> cr)) return false;

    for (auto& ch : name) {
        if (ch == '_') ch = ' ';
    }

    courseID = id;
    courseName = name;
    credits = cr;

    return true;
}
