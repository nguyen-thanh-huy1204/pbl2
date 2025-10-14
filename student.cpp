#include "Student.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
void tamDung() {
    cout << "Nhan Enter de tiep tuc...";
    cin.ignore();
    cin.get();
}
Student::Student(const string& tenDN, const string& matKhau,
                 const string& id, const string& hoTen,
                 int namHoc)
    : User(tenDN, matKhau), id(id), hoTen(hoTen),
      namHoc(namHoc) {}

// ========== Đọc danh sách học phần từ file ==========
vector<Course> docDanhSachHocPhan() {
    vector<Course> ds;
    ifstream in("Courses.txt");
    if (!in) {
        cout << "Khong mo duoc file Course.txt!\n";
        return ds;
    }
    Course c;
    while (c.loadFromFile(in)) {
        ds.push_back(c);
    }
    return ds;
}

// ========== Đăng ký học phần ==========
void Student::dangKyHocPhan() {
    vector<Course> ds = docDanhSachHocPhan();
    if (ds.empty()) {
        cout << "Chua co hoc phan nao trong he thong!\n";
        return;
    }

    cout << "\n===== DANH SACH HOC PHAN =====\n";
    for (const auto& hp : ds) {
        hp.display();
    }

    string idHP;
    cout << "\nNhap ma hoc phan muon dang ky: ";
    cin >> idHP;

    // Tim hoc phan co ma tuong ung
    for (const auto& hp : ds) {
        if (hp.getCourseID() == idHP) {

            // Kiem tra mon tien quyet
            string tienQuyet = hp.getPrerequisiteCourseID();
            bool daHocTienQuyet = true;
            if (!tienQuyet.empty()) {
                daHocTienQuyet = false;
                for (const auto& mon : danhSachDangKy) {
                    if (mon.getCourseID() == tienQuyet) {
                        daHocTienQuyet = true;
                        break;
                    }
                }
            }

            if (daHocTienQuyet || tienQuyet == "None") {
                danhSachDangKy.push_back(hp);
                cout << ">> Dang ky thanh cong!\n";
                luuVaoFile();
            } else {
                cout << ">> Chua hoc mon tien quyet " << tienQuyet << ", khong the dang ky!\n";
            }

            return;
        }
    }

    cout << ">> Khong tim thay hoc phan co ma " << idHP << "!\n";
}


// ========== Xem danh sách học phần đã đăng ký ==========
void Student::xemHocPhanDaDangKy() const {
    if (danhSachDangKy.empty()) {
        cout << "Ban chua dang ky hoc phan nao!\n";
        return;
    }
    cout << "\n===== HOC PHAN DA DANG KY =====\n";
    for (const auto& hp : danhSachDangKy) {
        hp.display();
    }
}

// ========== Hủy học phần ==========
void Student::huyHocPhan(const string& idHP) {
    for (auto it = danhSachDangKy.begin(); it != danhSachDangKy.end(); ++it) {
        if (it->getCourseID() == idHP) {
            danhSachDangKy.erase(it);
            cout << ">> Da huy hoc phan " << idHP << "!\n";
            luuVaoFile();
            return;
        }
    }
    cout << ">> Khong tim thay hoc phan " << idHP << "!\n";
}

// ========== Lưu danh sách đăng ký vào file ==========
void Student::luuVaoFile() const {
    string fileName = tenDN + "dangky.txt";
    ofstream out(fileName);
    if (!out) {
        cout << "Khong mo duoc file de luu!\n";
        return;
    }
    for (const auto& hp : danhSachDangKy) {
        hp.saveToFile(out);
    }
}

// ========== Tải danh sách đăng ký từ file ==========
void Student::taiTuFile() {
    string fileName = tenDN + "dangky.txt";
    ifstream in(fileName);
    if (!in) return;

    Course c;
    danhSachDangKy.clear();
    while (c.loadFromFile(in)) {
        danhSachDangKy.push_back(c);
    }
}

// ========== Menu ==========
void Student::Menu() {
    taiTuFile();
    int chon;
    do {
        cout << "\n===== MENU SINH VIEN =====\n";
        cout << "Xin chao " << hoTen << " (" << id << ")\n";
        cout << "1. Dang ky hoc phan\n";
        cout << "2. Xem hoc phan da dang ky\n";
        cout << "3. Huy hoc phan\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        cin >> chon;

        if (chon == 1) {
            dangKyHocPhan();
            tamDung();
        } else if (chon == 2) {
            xemHocPhanDaDangKy();
            tamDung();
        } else if (chon == 3) {
            string idHP;
            cout << "Nhap ma hoc phan muon huy: ";
            cin >> idHP;
            huyHocPhan(idHP);
            tamDung();
        }
    } while (chon != 0);
}
