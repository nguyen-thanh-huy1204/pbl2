#include "Schedule.h"
#include <sstream>
#include <cstdio>

// Helper function to convert day name to Vietnamese
static string dayToVietnamese(const string& day) {
    string dayLower = day;
    // Convert to lowercase
    for (size_t i = 0; i < dayLower.size(); ++i) {
        dayLower[i] = tolower((unsigned char)dayLower[i]);
    }
    
    // Return ASCII-only (no diacritics) day names
    if (dayLower.find("mon") != string::npos) return "Thu Hai";
    if (dayLower.find("tue") != string::npos) return "Thu Ba";
    if (dayLower.find("wed") != string::npos) return "Thu Tu";
    if (dayLower.find("thu") != string::npos) return "Thu Nam";
    if (dayLower.find("fri") != string::npos) return "Thu Sau";
    if (dayLower.find("sat") != string::npos) return "Thu Bay";
    if (dayLower.find("sun") != string::npos) return "Chu Nhat";
    return day;  // fallback if unknown
}

Schedule::Schedule(): day(""), startMin(0), endMin(0), room("") {}
Schedule::Schedule(const string& day_, int sMin, int eMin, const string& room_)
    : day(day_), startMin(sMin), endMin(eMin), room(room_) {}

int Schedule::parseTime(const string& hhmm) {
    int h=0,m=0;
    char c;
    stringstream ss(hhmm);
    ss >> h >> c >> m;
    return h*60 + m;
}
string Schedule::fmtTime(int m) {
    int h = m/60, mm = m%60;
    char buf[6];
    sprintf(buf, "%02d:%02d", h, mm);
    return string(buf);
}

bool Schedule::conflictsWith(const Schedule& o) const {
    if (day != o.day) return false;
    return !(endMin <= o.startMin || startMin >= o.endMin);
}

string Schedule::getDay() const { return day; }
int Schedule::getStart() const { return startMin; }
int Schedule::getEnd() const { return endMin; }
string Schedule::getRoom() const { return room; }

string Schedule::toLine() const {
    // escape-free small format: day,HH:MM,HH:MM,room
    return day + "," + fmtTime(startMin) + "," + fmtTime(endMin) + "," + room;
}

void Schedule::fromLine(const string& s) {
    // expect day,HH:MM,HH:MM,room
    stringstream ss(s);
    string day_, st, en, room_;
    if (!getline(ss, day_, ',')) { day=""; return; }
    if (!getline(ss, st, ',')) { day=""; return; }
    if (!getline(ss, en, ',')) { day=""; return; }
    if (!getline(ss, room_, ',')) { 
        // read rest of stream
        room_.clear();
        string rest;
        if (getline(ss, rest)) room_ = rest;
    }
    day = day_;
    startMin = parseTime(st);
    endMin = parseTime(en);
    room = room_;
}

string Schedule::toReadable() const {
    return dayToVietnamese(day) + " " + fmtTime(startMin) + "-" + fmtTime(endMin);
}
