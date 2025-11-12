#ifndef LECTURER_H
#define LECTURER_H

#include "User.h"
#include "vector.h"
#include <string>
using namespace std;

class Lecturer : public User {
private:
    string fullname;
    string department;
    Vector<string> teaching; // course ids
public:
    Lecturer();
    Lecturer(const string& id, const string& username, const string& password,
             const string& fullname, const string& department);

    void addTeaching(const string& cid);
    bool teaches(const string& cid) const;

    string getFullname() const { return fullname; }

    string toLine() const;
    void fromLine(const string& line);

    void displayShort() const;
};

#endif
