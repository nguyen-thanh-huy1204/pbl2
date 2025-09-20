#include "User.h"

User::User(const string& tenDN, const string& matKhau)
    : tenDN(tenDN), matKhau(matKhau) {}

string User::getTenDN() const {
    return tenDN;
}

bool User::kiemTraMatKhau(const string& mk) const {
    return mk == matKhau;
}

void User::doiMatKhau() {
    string mkCu, mkMoi;
    cout << "Nhap mat khau hien tai: ";
    cin >> mkCu;
    if(mkCu != matKhau) {
        cout << "Mat khau khong dung!\n";
        return;
    }
    cout << "Nhap mat khau moi: ";
    cin >> mkMoi;
    matKhau = mkMoi;
    cout << "Doi mat khau thanh cong!\n";
}
