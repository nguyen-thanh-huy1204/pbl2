#include "User.h"

User::User(): id(""), username(""), password(""), role("") {}
User::User(const string& id_, const string& username_, const string& password_, const string& role_)
    : id(id_), username(username_), password(password_), role(role_) {}
User::~User() {}

string User::getId() const { return id; }
string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
string User::getRole() const { return role; }
bool User::checkPassword(const string& pw) const { return password == pw; }

string User::toLine() const {
    return id + "|" + username + "|" + password + "|" + role + "|";
}
