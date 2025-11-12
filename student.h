#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "vector.h"
#include "Course.h"
#include "Schedule.h"
#include <string>
using namespace std;

class Student : public User {
private:
    string fullname;
    string major;

    // Parallel arrays: enrolledCourseIds[i] có schedule tương ứng enrolledSchedules[i]
    Vector<string> enrolledCourseIds;
    Vector<Schedule> enrolledSchedules;
    Vector<string> completedCourseIds; // courses student has completed

public:
    Student();
    Student(const string& id, const string& username, const string& password,
            const string& fullname, const string& major);

    // Kiểm tra đã đăng ký course chưa
    bool isEnrolled(const string& cid) const;

    // Đăng ký course với schedule
    void enroll(const string& cid, const Schedule& sch);

    // Hủy course (xóa cả schedule)
    void withdraw(const string& cid);

    // Kiểm tra trùng lịch với một schedule mới
    bool hasConflictWithCourseSchedule(const Schedule& newSch) const;

    // Lưu/truy xuất
    virtual string toLine() const override; // id|username|password|role|fullname|major|cid1,day,st,en,room;cid2,...
    void fromLine(const string& line);

    void displayShort() const;
    void displaySchedule() const;

    // Truy cập thông tin course đã đăng ký (chỉ ID)
    Vector<string> getEnrolledCourseIds() const { return enrolledCourseIds; }
    Vector<Schedule> getEnrolledSchedules() const;
    string getFullname() const { return fullname; }
    Vector<string> getCompletedCourseIds() const { return completedCourseIds; }
    bool hasCompleted(const string& cid) const;
    void completeCourse(const string& cid);
};

#endif
