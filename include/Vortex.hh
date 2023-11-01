#pragma once
#include <iostream>
#include <string>
#include <list>
#include <Edge.hh>

class Edge;

using std::string;

class Vortex {
    public:
        string name;
        bool visited;
        std::list<string*> activities;
        std::list<Edge> edges;
        
        // Constructors
        Vortex(string name);
        Vortex();

        friend std::ostream& operator<<(std::ostream& os, const Vortex& vortex);
        bool operator==(const Vortex& rhs) const {
            return name == rhs.name;
        }
};
