#include "Admin.h"
#include <iostream>
using namespace std;

Admin::Admin(const string& tenDN, const string& matKhau,
             const string& id, const string& ten)
    : User(tenDN, matKhau), id(id), ten(ten) {}

void Admin::Menu() {
    cout << "\n===== MENU ADMIN =====\n";
    cout << "Xin chao Admin " << ten << " (" << id << ")\n";
    cout << "1. Quan ly hoc phan\n";
    cout << "2. Quan ly tai khoan\n";
    cout << "0. Thoat\n";
    int chon;
    cout << "Chon: ";
    cin >> chon;
    if(chon == 1) {
        cout << ">> Chuc nang quan ly hoc phan (chua lam)\n";
    } else if(chon == 2) {
        cout << ">> Chuc nang quan ly tai khoan (chua lam)\n";
    }
}
