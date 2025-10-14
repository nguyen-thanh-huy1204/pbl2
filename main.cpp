#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Admin.h"
#include "Student.h"
#include "Course.h"
using namespace std;

int main() {
    vector<User*> dsUser;
    vector<Course> courses;

    ifstream fin("Users.txt");
    if (!fin) {
        cout << "Khong tim thay file Users.txt\n";
        return 1;
    }

    string line;
    while (getline(fin, line)) {
        istringstream iss(line);
        string tenDN, mk, loaiTK, id, ten;
        iss >> tenDN >> mk >> loaiTK >> id >> ten;

        if (loaiTK == "admin") {
            dsUser.push_back(new Admin(tenDN, mk, id, ten));
        } else if (loaiTK == "sv") {
            int nam;
            string nganh;
            iss >> nam >> nganh;
            dsUser.push_back(new Student(tenDN, mk, id, ten, nam));
        }
    }
    fin.close();

    ifstream finCourse("Courses.txt");
    while (true) {
        Course c("", "", 0);
        if (!c.loadFromFile(finCourse)) break;
        courses.push_back(c);
    }
    finCourse.close();

    string tenDangNhap, matKhau;
    cout << "===== DANG NHAP =====\n";
    cout << "Ten dang nhap: ";
    cin >> tenDangNhap;
    cout << "Mat khau: ";
    cin >> matKhau;

    User* u = nullptr;
    for (int i = 0; i < dsUser.size(); i++) {
        if (dsUser[i]->getTenDN() == tenDangNhap && 
            dsUser[i]->kiemTraMatKhau(matKhau)) 
        {
            u = dsUser[i];
            break; 
        }
    }

    if (u) {
        cout << "Dang nhap thanh cong!\n";
        u->Menu(); 
    } else {
        cout << "Sai ten dang nhap hoac mat khau!\n";
    }

    for (User* x : dsUser) delete x;

    return 0;
}
