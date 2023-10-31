#include "Tree.hh"
#include <Edge.hh>
#include <Person.hh>
#include <Vortex.hh>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

using std::string;
std::list<Vortex> vortexList;
std::list<string> activityList;
Tree *tree;


// This function opens a file and append information to it in binary mode
// Receives: a person object and the file name
// Returns: nothing
void writeToFile(Person* person, string fileName, size_t size) {

    std::ofstream file(fileName, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    // write the size of the person class
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(person), sizeof(Person) * size);
    file.close();

    if (!file.good()) {
        throw std::runtime_error("Error occurred at writing time");
    }
}

size_t load(Person** person, string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    size_t size = 0;
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    Person* p = new Person[size];
    file.read(reinterpret_cast<char*>(p), sizeof(Person) * size);
    file.close();
    *person = p;
    return size;
}
void printGraph() {
    // This function prints the entire graph
    // Receive: nothing
    // Return: nothing
    
    for (Vortex& vortex : vortexList) {
        std::cout << vortex << ", edges: ";

        if (vortex.edges.empty()) {
            std::cout << "No edges yet" << std::endl;
        } else {
            for (Edge& edge : vortex.edges) {
                std::cout << edge << " ";
            }
        }

        std::cout << vortex.name << " activities: ";

        if (vortex.activities.empty()) {
            std::cout << "No activities yet" << std::endl;
            std::cout << std::endl;
            continue;
        }

        for (string* activity : vortex.activities) {
            std::cout << *activity << " ";
        }

        std::cout << "\n" << std::endl;
    }
}

void createVortex() {
    // This function creates a vortex and adds it to the list of vortexes
    // Receive: nothing
    // Return: nothing
    string vortexName;

    std::cout << "Enter the name of the place: ";
    std::getline(std::cin, vortexName);

    // Check if the vortex already exists
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            std::cout << "This place already exists" << std::endl;
            return;
        }
    }

    Vortex vortex(vortexName); // Create the vortex if it doesn't exist and add it to the list
    vortexList.push_back(vortex);
}

void deleteVortex() {
    // This function deletes a vortex from the list of vortexes
    // Receive: nothing
    // Return: nothing

    string vortexName;
    Vortex* vortexToDelete = nullptr;

    std::cout << "Enter the name of the place you want to delete (notice that this will delete all the edges related to that vortex): ";
    std::getline(std::cin, vortexName);

    // Find the vortex with the name entered by the user and delete it
    auto it = vortexList.begin();
    while (it != vortexList.end()) {
        if (it->name == vortexName) {
            vortexToDelete = &(*it);
            it = vortexList.erase(it); // Elimina el vortex y actualiza el iterador
        } else {
            ++it;
        }
    }

    // If the vortex was not found, print an error message and return
    if (vortexToDelete == nullptr) {
        std::cout << "Invalid place" << std::endl;
        return;
    }

    // Delete all the edges that are related to the vortex
    for (Vortex& vortex : vortexList) {
        auto edgeIt = vortex.edges.begin();
        while (edgeIt != vortex.edges.end()) {
            if (edgeIt->destination == vortexToDelete) {
                edgeIt = vortex.edges.erase(edgeIt); // Elimina el edge y actualiza el iterador
            } else {
                ++edgeIt;
            }
        }
    }
}

void modifyVortexName(Vortex& vortex) {
    string newName;
    std::cout << "Enter the new name: ";
    std::getline(std::cin, newName);

    // Check if the new name already exists
    for (Vortex& vortex : vortexList) {
        if (vortex.name == newName) {
            std::cout << "There is a place that already has that name" << std::endl;
            return;
        }
    }

    // Change the name of the vortex
    for (Vortex& iterator : vortexList) {
        if (iterator.name == vortex.name) {
            iterator.name = newName;
            return;
        }
    }
}

void modifyVortex() {
    // This function modifies a vortex from the list of vortexes
    // Receive: nothing
    // Return: nothing

    int option;
    Vortex* vortexToModify = nullptr;
    string vortexName;
    std::cout << "Enter the name of the place you want to modify: ";
    std::getline(std::cin, vortexName);

    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            vortexToModify = &vortex;
            break;
        }
    }

    // If the vortex was not found, print an error message and return
    if (vortexToModify == nullptr) {
        std::cout << "Invalid place" << std::endl;
        return;
    }
    // Print the options to modify the vortex
    std::cout << "What do you want to modify?" << std::endl;
    std::cout << "1. Name" << std::endl;
    std::cout << "2. Edges" << std::endl;
    std::cout << "3. Activities" << std::endl;
    std::cin >> option;

    std::cin.ignore();

    // Execute the option selected by the user
    switch (option) {
    case 1:
        modifyVortexName(*vortexToModify);
        break;
    case 2:
        // Modify the edges of the vortex
        break;
    case 3:
        // Modify the activities of the vortex
    default:
        std::cout << "Invalid option" << std::endl;
        break;
    }
    /* switch (option) { */
    /*     case "1": */
    /*         //Modify the name of the vortex */
    /*         break; */
    /*     case "2": */
    /*         //Modify the edges of the vortex */
    /*         break; */
    /*     case "3": */
    /*         //Modify the activities of the vortex */
    /*     default: */
    /*         std::cout << "Invalid option" << std::endl; */
    /*         break; */
    /* } */
}

void addEdge(Edge& edge, Vortex* vortex)  {
    //This method receive an edge and add it to the vortexList, also add the bi-directional edge
    //Receive: an edge 
    //Return: void
    
    vortex->edges.push_back(edge); //This add the edge to the origin vortex

    //This loop add the edge to the destination vortex but the destination will be the origin vortex
    for (Vortex& vortexIterator : vortexList) {
        if (vortexIterator.name == edge.destination->name) {
            Edge returnEdge(edge.distance, vortex);
            vortexIterator.edges.push_back(returnEdge);
        }
    }

    //This loop udpate the list in order to storage the origin vortex
    //for (Vortex& vortexIterator: vortexList) {
    //    if (vortexIterator.name == vortex.name) {
    //        vortexIterator.edges = vortex.edges;
    //        return;
    //    }
    //}
}

void setEdge() {
    // This function creates an edge between two vortexes
    // Receive: nothing
    // Return: nothing

    int distance;
    Vortex* originVortex = nullptr;
    string originVortexName;
    string destinationVortexName;

    std::cout << "Enter the starting point: ";
    std::getline(std::cin, originVortexName);

    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == originVortexName) {
            originVortex = &vortex;
            break;
        }
    }

    // If the vortex was not found, print an error message and return
    if (originVortex == nullptr) {
        std::cout << "Invalid starting address" << std::endl;
        return;
    }

    std::cout << "Enter the destination: ";
    std::getline(std::cin, destinationVortexName);

    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == destinationVortexName) {
            try {
                std::cout << "Enter the distance: ";
                std::string distanceStr;
                std::cin >> distanceStr;
                std::cin.ignore();

                distance = std::stoi(distanceStr); // Convert the string to an integer

                if (distance <= 0) {
                    std::cout << "Invalid distance" << std::endl;
                    return;
                }

                Edge edge(distance, &vortex);
                addEdge(edge, originVortex);
                return;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid input for distance. Please enter a valid numeric value." << std::endl;
                return;
            }
        }
    }
    std::cout << "Invalid destination" << std::endl;
}

void createMenu(const std::vector<string>& options) {
    int c = 1;
    for (const string& option : options) {
        std::cout << c << ". " << option << std::endl;
        c++;
    }
    std::cout << "0. Exit" << std::endl;
}

void setPeople(string fileName) {
    Person p1("Male", 18, "San Ramon", "Santa Clara", "Comer");
    Person p2("Female", 19, "Heredia", "Alajuela", "Comer");
    Person p3("Male", 20, "Palmares", "San Ramon", "Comer");
    Person p4("Male", 22, "Chachagua", "Alajuela", "Comer");
    Person p5("Female", 24, "San Jose", "San Ramon", "Comer");
    Person list[5] = {p1, p2, p3, p4, p5};
    writeToFile(list, fileName, 5);
}

void setVortex() {
    Vortex vortex1("San Ramon");
    Vortex vortex2("Liberia");
    Vortex vortex3("Grecia");
    Vortex vortex4("Palmares");
    Vortex vortex5("San Jose");
    Vortex vortex6("Alajuela");
    Vortex vortex7("Heredia");
    Vortex vortex8("Cartago");
    Vortex vortex9("Puntarenas");
    Vortex vortex10("Limon");
    Vortex vortex11("Santa Clara");
    vortexList.push_back(vortex1);
    vortexList.push_back(vortex2);
    vortexList.push_back(vortex3);
    vortexList.push_back(vortex4);
    vortexList.push_back(vortex5);
    vortexList.push_back(vortex6);
    vortexList.push_back(vortex7);
    vortexList.push_back(vortex8);
    vortexList.push_back(vortex9);
    vortexList.push_back(vortex10);
    vortexList.push_back(vortex11);
    setEdge();
}



int main() {
    string dataFile = "Information/Data.bin";
    Person* list = nullptr;
    bool exit = false;
    //while (!exit) {
    //    system("clear");
    //    std::vector<string> options = {"Calculate Route", "Print Graph", "Vortex Options", "Edge Options", "Activity Options"};
    //    createMenu(options);
    //    int option;
    //    std::cin >> option;
    //    std::cin.ignore();
    //    if (option < 0 || option > static_cast<int>(options.size())) {
    //        std::cout << "Invalid option" << std::endl;
    //        continue;
    //    }
    //    switch (option) {
    //    case 1:
    //        exit = true;
    //        break;
    //    case 2:
    //        printGraph();
    //        break;
    //    case 3:
    //       
    //        break;
    //    case 4:
            // Vortex Options
    //        break;
    //    case 5:
            // Edge Options
    //        break;
    //    case 6:
            // Activity Options
    //        break;
    //    case 0:
    //        exit = true;
    //        break;
    //    default:
    //        std::cout << "Invalid option" << std::endl;
    //    }

        
    //}
    //size_t size = load(&list, "Information/Data.bin");
    //for (size_t i = 0; i < size; i++) {
    //    std::cout << list[i].gender << std::endl;
    //}
    setVortex();
    printGraph();
    return 0;
}
