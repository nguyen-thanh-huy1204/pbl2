#include "Student.h"
#include <iostream>
using namespace std;

Student::Student(const string& tenDN, const string& matKhau,
                 const string& id, const string& hoTen,
                 int namHoc, const string& nganhHoc)
    : User(tenDN, matKhau), id(id), hoTen(hoTen),
      namHoc(namHoc), nganhHoc(nganhHoc) {}

void Student::Menu() {
    cout << "\n===== MENU SINH VIEN =====\n";
    cout << "Xin chao " << hoTen << " (" << id << ")\n";
    cout << "Nam hoc: " << namHoc << ", Nganh: " << nganhHoc << "\n";
    cout << "1. Dang ky hoc phan\n";
    cout << "2. Xem hoc phan da dang ky\n";
    cout << "0. Thoat\n";
    int chon;
    cout << "Chon: ";
    cin >> chon;
    if(chon == 1) {
        cout << ">> Chuc nang dang ky hoc phan (chua lam)\n";
    } else if(chon == 2) {
        cout << ">> Chuc nang xem hoc phan (chua lam)\n";
    }
}
