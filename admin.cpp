#include "Admin.h"

Admin::Admin(): User() {}
Admin::Admin(const string& id, const string& username, const string& password)
    : User(id, username, password, "ADMIN") {}

string Admin::toLine() const {
    return id + "|" + username + "|" + password + "|" + role + "|";
}