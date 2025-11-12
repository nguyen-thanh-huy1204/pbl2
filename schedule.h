#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <string>
using namespace std;

class Schedule {
private:
    string day;      // e.g., "Mon", "Thu 2", ...
    int startMin;    // minutes from 00:00
    int endMin;
    string room;
public:
    Schedule();
    Schedule(const string& day, int sMin, int eMin, const string& room);

    static int parseTime(const string& hhmm); // "07:30" -> 450
    static string fmtTime(int m);

    bool conflictsWith(const Schedule& o) const;

    string getDay() const;
    int getStart() const;
    int getEnd() const;
    string getRoom() const;

    // serialization for saving multiple schedules in one field:
    // format: day,HH:MM,HH:MM,room  (commas inside)
    string toLine() const;
    void fromLine(const string& s); // parse the above format

    string toReadable() const;
};

#endif
