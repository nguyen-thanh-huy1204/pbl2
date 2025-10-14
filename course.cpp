#include "Course.h"
#include <iostream>
using namespace std;

Course::Course(const string& id, const string& name, int credits, const string& mon_tien_quyet)
    : courseID(id), courseName(name), creditCourse(credits), ID_mon_tien_quyet(mon_tien_quyet) {}

string Course::getCourseID() const { return courseID; }
string Course::getCourseName() const { return courseName; }
int Course::getCreditCount() const { return creditCourse; }
string Course::getPrerequisiteCourseID() const { return ID_mon_tien_quyet; }

void Course::display() const {
    cout << "Ma mon hoc: " << courseID
         << " | Ten mon hoc: " << courseName
         << " | So tin chi: " << creditCourse;
    if (!ID_mon_tien_quyet.empty())
        cout << " | Mon tien quyet: " << ID_mon_tien_quyet;
    cout << endl;
}

void Course::saveToFile(ostream& out) const {
    out << courseID << " " << courseName << " " << creditCourse << " " << ID_mon_tien_quyet << endl;
}

bool Course::loadFromFile(istream& in) {
    string id, name, tien_quyet;
    int credits;

    if (!(in >> id >> name >> credits >> tien_quyet))
        return false;

    courseID = id;
    courseName = name;
    creditCourse = credits;
    ID_mon_tien_quyet = tien_quyet;

    return true;
}
