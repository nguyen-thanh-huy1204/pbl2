#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    string id;
    string username;
    string password;
    string role; // "ADMIN","LECTURER","STUDENT"
public:
    User();
    User(const string& id, const string& username, const string& password, const string& role);
    virtual ~User();
    string getId() const;
    string getUsername() const;
    string getPassword() const;
    string getRole() const;
    bool checkPassword(const string& pw) const;
    // toLine/fromLine basic (derived classes will extend)
    virtual string toLine() const;
};

#endif
