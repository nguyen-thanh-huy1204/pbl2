#include <iostream>
#include <string.h>
#include <vector>
#include <Courses.h>
class student
{
private:
    string ID;
    string name;
    string password;
    int year;   
    vector <Courses*> list_of_courses;
public:
    student (string ma_sv,string ten_sv,string mat_khau,int nam_sv){};
    string getID();
    string getName();
    int getyear();
    vector<Course*>& getCourses();
    bool registerCourse(Course* course);
};