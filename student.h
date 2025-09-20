#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>
using namespace std;

class Student : public User {
private:
    string id;
    string hoTen;
    int namHoc;
    string nganhHoc;

public:
    Student(const string& tenDN, const string& matKhau,
            const string& id, const string& hoTen,
            int namHoc, const string& nganhHoc);

    void Menu() override;
};

#endif
