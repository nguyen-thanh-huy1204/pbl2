#ifndef SYSTEM_H
#define SYSTEM_H
#include "map.h"
#include "Vector.h"
#include "User.h"
#include "Student.h"
#include "Lecturer.h"
#include "admin.h"
#include "Course.h"
#include "Offering.h"
#include <string>
using namespace std;

class System {
private:
    string dataFolder;

    Map<string, User*> users;        // all users
    Map<string, Student> students;
    Map<string, Lecturer> lecturers;
    Map<string, Course> courses;
    Map<string, Offering> offerings;

    string nextUserId();
    string nextStudentId();
    string nextLecturerId();
    string nextCourseId();
    string nextOfferingId();

    void loadAll();
    void saveAll();

    void loadUsers();
    void loadStudents();
    void loadLecturers();
    void loadCourses();
    void loadOfferings();

    bool login(string& outId, string& outRole);
    void registerUser();

    void adminAddCourse();
    void adminAssignLecturer();
    void adminListStudentsInCourse();

    void studentEnroll(const string& sid);
    void studentWithdraw(const string& sid);
    void showStudentSchedule(const string& sid);
    void markCourseCompleted(const string& lid, const string& cid);
    void showCompletedCourses(const string& sid);

    void showLecturerCourses(const string& lid);
    void showCourseStudents(const string& cid);
    void showAllCourses() const;

public:
    System(const string& folder);
    ~System();
    void run();
};

#endif
