#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <iostream>
using namespace std;

class Course {
private:
    string courseID;        
    string courseName;         
    int creditCourse;               
    string ID_mon_tien_quyet;   

public:
    Course(const string& id = "", const string& name = "", int credits = 0, const string& mon_tien_quyet = "");
    
    string getCourseID() const;
    string getCourseName() const;
    int getCreditCount() const;
    string getPrerequisiteCourseID() const;

    void display() const;
    void saveToFile(ostream& out) const;
    bool loadFromFile(istream& in);
};

#endif
