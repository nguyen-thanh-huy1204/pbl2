#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
using namespace std;

class User {
protected:
    string tenDN;
    string matKhau;

public:
    User(const string& tenDN, const string& matKhau);

    virtual void Menu() = 0;

    string getTenDN() const;
    
    bool kiemTraMatKhau(const string& mk) const;

    void doiMatKhau();
};

#endif
