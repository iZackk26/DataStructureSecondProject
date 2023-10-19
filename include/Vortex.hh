#pragma once
#include <iostream>
#include <string>
#include <list>
#include <Edge.hh>
#include <Activitie.hh>

class Edge;

using std::string;

class Vortex {
    public:
        string name;
        std::list<Activitie> activities;
        std::list<Edge> edges;
        
        // Constructors
        Vortex(string name);
        Vortex();
};
