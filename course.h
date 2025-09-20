#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <iostream>
using namespace std;

class Course {
private:
    string courseID;
    string courseName;
    int credits;

public:
    Course(const string& id = "", const string& name = "", int cr = 0);

    string getCourseID() const { return courseID; }
    string getCourseName() const { return courseName; }
    int getCredits() const { return credits; }

    void display() const;

    void saveToFile(ostream& out) const;
    bool loadFromFile(istream& in);
};

#endif
