#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
public:
    Admin();
    Admin(const string& id, const string& username, const string& password);
    string toLine() const;
};

#endif
