#include <Edge.hh>
#include <Vortex.hh>
#include <Globals.hh>

Edge::Edge(float distance, Vortex *destination) {
    this->distance = distance;
    this->destination = destination;
}

Edge::Edge() {
}

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << "Edge: " << edge.distance << " " << *edge.destination;
    return os;
}
