#include <Globals.hh>
#include <Person.hh>
#include <Vortex.hh>
#include <Activity.hh>
#include <Edge.hh>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

using std::string;

void writeToFile(Person person, string fileName){
    //This function opens a file and append information to it in binary mode

    std::ofstream file(fileName, std::ios::out | std::ios::app | std::ios::binary);
    try {
        if (!file.is_open()) {
            throw std::runtime_error("File not found");
        }

        file.write(reinterpret_cast<char *>(&person), sizeof(person));
        file.close();

        if (!file.good()) {
            throw std::runtime_error("Error occurred at writing time");
        }
    } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
}

void readFromFile(const string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    try {
        if (!file) {
            throw std::runtime_error("File not found");
        }

        Person person;
        file.read(reinterpret_cast<char *>(&person), sizeof(person));

        while (!file.eof()) {
            std::cout << person << std::endl;
            file.read(reinterpret_cast<char *>(&person), sizeof(person));
        }

        file.close();
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
}

void printGraph(){
    for (Vortex &vortex : vortexList) {
        std::cout << vortex << " :" << vortex.edges.size() << std::endl;
        
        for (Edge &edge : vortex.edges) {
            std::cout << edge << std::endl;
        }
    }
}

int main(){
    Vortex vortex1("Vortex1");
    Vortex vortex2("Vortex2");
    vortexList.push_back(vortex1);
    vortexList.push_back(vortex2);

    Edge edge(10, &vortex2);
    
    vortex1.addEdge(edge);
    
    printGraph();

    return 0;
}
