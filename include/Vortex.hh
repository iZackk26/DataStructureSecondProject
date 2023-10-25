#pragma once
#include <iostream>
#include <string>
#include <list>
#include <Edge.hh>
#include <Activity.hh>

class Edge;

using std::string;

class Vortex {
    public:
        string name;
        std::list<Activity*> activities;
        std::list<Edge> edges;
        
        // Constructors
        Vortex(string name);
        Vortex();
        
        // Methods
        void addActivity(Activity* activity);
        void removeActivity(Activity* activity);

        void addEdge(Edge& edge);
        void removeEdge(Edge& edge);

        friend std::ostream& operator<<(std::ostream& os, const Vortex& vortex);
};
