#include <Vortex.hh>
#include <Edge.hh>
#include <list>
#include <string>

using namespace std;

static std::list<Vortex> vortexList; //Definition of the list declared in the header
static std::list<string> activityList; //Definition of the list declared in the header

Vortex::Vortex(string name) {
    //This is the constructor
    this->name = name;
    this->visited = false;
    this->edges = std::list<Edge>();
    this->activities = std::list<string*>();
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
