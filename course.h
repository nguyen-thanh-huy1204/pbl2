#ifndef COURSE_H
#define COURSE_H

#include <string>
#include "vector.h"
#include "Schedule.h"
using namespace std;

class Course {
private:
    string id;
    string name;
    double credit;
    string lecturerId; // "NULL" if unassigned
    int maxStudents;
    Vector<string> studentIds; 
    Vector<Schedule> schedules; // multiple schedules for this course
    Vector<string> students;    // student IDs
    Vector<string> prerequisites; // prerequisite course ids

public:
    Course();
    Course(const string& id, const string& name, double credit, const string& lecturerId, int maxStu);

    string getId() const;
    string getName() const;
    double getCredit() const;
    Vector<string> getStudentIds() const;
    string getLecturerId() const;
    int getMaxStudents() const;

    void setLecturer(const string& lid);
    void setMaxStudents(int m);

    // schedules
    void addSchedule(const Schedule& s);
    const Vector<Schedule>& getSchedules() const;
    Vector<Schedule>& getSchedules();

    // students
    bool isFull() const;
    bool hasStudent(const string& sid) const;
    void addStudent(const string& sid);
    void removeStudent(const string& sid);

    // file IO
    // format: id|name|credit|lecturerId|sched1;sched2;...|max|s1;s2
    string toLine() const;
    void fromLine(const string& line);

    Vector<string> getPrerequisites() const { return prerequisites; }
    void addPrerequisite(const string& pid) { if (prerequisites.findIndex(pid) == -1) prerequisites.push_back(pid); }
    void setPrerequisites(const Vector<string>& p) { prerequisites = p; }

    string infoString() const;
};

#endif
