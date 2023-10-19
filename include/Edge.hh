#pragma once
#include <Vortex.hh>
#include <iostream>
#include <string>

class Vortex;

using std::string;

class Edge {
    public:
        float distance;
        Vortex *vortex;

        // Constructors
        Edge(float distance, Vortex *vortex);
        Edge();

};
