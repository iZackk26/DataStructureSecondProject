#include <Edge.hh>
#include <Vortex.hh>

Edge::Edge(float distance, Vortex *destination) {
    this->distance = distance;
    this->destination = destination;
}

Edge::Edge() {
}

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << "(" << "Distance: " << edge.distance << " Destination: " << *edge.destination << ")";
    return os;
}
