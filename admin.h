#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <vector>
#include <string>
using namespace std;

class Admin : public User {
private:
    string id;
    string ten;

public:
    Admin(const string& tenDN, const string& matKhau,
          const string& id, const string& ten);

    void Menu() override;
};

#endif
