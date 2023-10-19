#pragma once
#include <string>

using std::string;

class Activitie {
    public:
        string name;
        Activitie *next;

        // Constructors
        Activitie(string name);
        Activitie();
};
