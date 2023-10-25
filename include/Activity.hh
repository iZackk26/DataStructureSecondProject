#pragma once
#include <string>

using std::string;

class Activity {
    public:
        string name;
        Activity *next;

        // Constructors
        Activity(string name);
        Activity();
};
