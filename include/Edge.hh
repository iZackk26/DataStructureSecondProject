#pragma once
#include <Vortex.hh>
#include <iostream>
#include <string>

class Vortex;

using std::string;

class Edge {
    public:
        float distance;
        Vortex *destination;

        // Constructors
        Edge(float distance, Vortex *destination);
        Edge();
        
        friend std::ostream& operator<<(std::ostream& os, const Edge& edge);

        bool operator==(const Edge& rhs) {
            return destination == rhs.destination;
        }


};
