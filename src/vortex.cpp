#include <Globals.hh>
#include <Vortex.hh>
#include <Edge.hh>
#include <list>
#include <string>
#include <Activity.hh>
#include <iostream>

using namespace std;

std::list<Vortex> vortexList; //Definition of the list declared in the header

Vortex::Vortex(string name) {
    //This is the constructor
    this->name = name;
    this->edges = std::list<Edge>();
    this->activities = std::list<Activity*>();
}

Vortex::Vortex() {
    //This is the empty constructor
}

std::ostream& operator<<(ostream& os, const Vortex& vortex) {
    //This method is used to print the vortex
    //Receive: ostream and a vortex
    //Return: ostream
    os << vortex.name;
    return os;
}

void Vortex::addActivity(Activity* activity) {
    //This method receive an activity and add it to the vortexList
    //Receive: pointer to an Activity
    //Return: void
    this->activities.push_back(activity);
}

void Vortex::removeActivity(Activity* activity) {
    //This method receive an activity and remove it from the vortexList
    //Receive: pointer to an activity
    //Return: void
    this->activities.remove(activity);
}

void Vortex::addEdge(Edge& edge)  {
    //This method receive an edge and add it to the vortexList, also add the bi-directional edge
    //Receive: an edge 
    //Return: void
    
    this->edges.push_back(edge); //This add the edge to the origin vortex

    //This loop add the edge to the destination vortex but the destination will be the origin vortex
    for (Vortex& vortex : vortexList) {
        if (vortex.name == edge.destination->name) {
            Edge returnEdge(edge.distance, this);
            vortex.edges.push_back(returnEdge);
        }
    }

    //This loop udpate the list in order to storage the origin vortex
    for (Vortex& vortex: vortexList) {
        if (vortex.name == this->name) {
            vortex.edges = this->edges;
            return;
        }
    }


}
