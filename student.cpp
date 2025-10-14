#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Course.h"
#include <vector>
#include <string>
using namespace std;

class Student : public User {
private:
    string id;                    
    string hoTen;                
    int namHoc;                    
    vector<Course> danhSachDangKy;  

public:
    Student(const string& tenDN, const string& matKhau,
            const string& id, const string& hoTen,
            int namHoc);

    void Menu() override;

    void dangKyHocPhan();

    void xemHocPhanDaDangKy() const;

    void huyHocPhan(const string& idHP);

    void luuVaoFile() const;

    void taiTuFile();
};

#endif
