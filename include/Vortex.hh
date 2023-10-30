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
        std::list<string*> activities;
        std::list<Edge> edges;
        
        // Constructors
        Vortex(string name);
        Vortex();
        
        // Methods
        void addActivity(string activity);
        void removeActivity(string activity);

        void addEdge(Edge& edge);
        void removeEdge(Edge& edge);

        friend std::ostream& operator<<(std::ostream& os, const Vortex& vortex);
        bool operator==(const Vortex& rhs) const {
            return name == rhs.name;
        }
};
