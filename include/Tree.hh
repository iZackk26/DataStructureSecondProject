#pragma once
#include <iostream>
#include <string>
#include <Person.hh>
#include <list>

using std::string;

class Tree{
    public:
        int TotalPeople;
        string name;
        std::list<Tree> *children;

};
