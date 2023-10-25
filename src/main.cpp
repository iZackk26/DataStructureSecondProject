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
        std::cout << vortex << ", edges: ";
        
        if (vortex.edges.empty()) {
            std::cout << "No edges yet" << std::endl;
            continue;
        }

        for (Edge &edge : vortex.edges) {
            std::cout << edge << " " << std::endl;
        }
    }
}

void setEdge() {
    int distance;
    Vortex* originVortex = nullptr;
    string originVortexName;
    string destinationVortexName;
    
    // Obtiene y verifica el punto de partida
    std::cout << "Enter the starting point: ";
    std::getline(std::cin, originVortexName);

    for (Vortex& vortex : vortexList) {
        if (vortex.name == originVortexName) {
            originVortex = &vortex;
            break;
        }
    }

    if (originVortex == nullptr) {
        std::cout << "Invalid starting address" << std::endl;
        return;
    }
    
    // Obtiene el destino
    std::cout << "Enter the destination: ";
    std::getline(std::cin, destinationVortexName);

    for (Vortex& vortex : vortexList) {
        if (vortex.name == destinationVortexName) {
            try {
                std::cout << "Enter the distance: ";
                std::string distanceStr;
                std::cin >> distanceStr;
                std::cin.ignore();

                // Intenta convertir el valor de cadena a un entero
                distance = std::stoi(distanceStr);

                // Verifica la validez de la distancia
                if (distance <= 0) {
                    std::cout << "Invalid distance" << std::endl;
                    return;
                }

                Edge edge(distance, &vortex);
                originVortex->addEdge(edge);
                return;  // Sale de la función después de agregar la arista
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid input for distance. Please enter a valid numeric value." << std::endl;
                return;
            }
        }
    }
    std::cout << "Invalid destination" << std::endl;
}


int main(){
    Vortex vortex1("San Ramon");
    Vortex vortex2("Liberia");
    Vortex vortex3("Grecia");
    vortexList.push_back(vortex1);
    vortexList.push_back(vortex2);
    vortexList.push_back(vortex3);

    setEdge();


    printGraph();

    return 0;
}
