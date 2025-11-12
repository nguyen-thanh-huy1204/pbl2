#ifndef OFFERING_H
#define OFFERING_H

#include <string>
#include "vector.h"
#include "Schedule.h"
using namespace std;

class Offering {
private:
    string id; // offering id (we'll reuse old course id values)
    string templateId; // refers to Course template id
    string lecturerId;
    int maxStudents;
    Vector<string> students;
    Vector<Schedule> schedules;
public:
    Offering();
    Offering(const string& id, const string& templateId, const string& lecturerId, int max);

    string getId() const;
    string getTemplateId() const;
    string getLecturerId() const;
    int getMaxStudents() const;
    Vector<string> getStudentIds() const;
    const Vector<Schedule>& getSchedules() const;

    void setLecturer(const string& lid);
    void addSchedule(const Schedule& s);
    void addStudent(const string& sid);
    void removeStudent(const string& sid);
    bool isFull() const;

    // serialization: offeringId|templateId|lecturerId|sched1;sched2|max|s1;s2
    string toLine() const;
    void fromLine(const string& line);
};

#endif
