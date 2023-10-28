#include <Globals.hh>
#include <Person.hh>
#include <Vortex.hh>
#include <Edge.hh>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

using std::string;

//Prototypes
void createVortex();
void deleteVortex();
void modifyVortex();
void modifyVortexName(Vortex& vortex);
void setEdge();
void printGraph();
void writeToFile(Person person, string fileName);
void readFromFile(const string& filename);

void writeToFile(Person person, string fileName){
    //This function opens a file and append information to it in binary mode
    //Receives: a person object and the file name
    //Returns: nothing

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
    //This function opens a file and reads the information in binary mode 
    //Receives: the file name 
    //Returns: nothing

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
    //This function prints the entire graph 
    //Receive: nothing
    //Return: nothing
    
    for (Vortex &vortex : vortexList) {
        std::cout << vortex << ", edges: ";
        
        if (vortex.edges.empty()) {
            std::cout << "No edges yet" << std::endl;
        } else {
            for (Edge &edge : vortex.edges) {
                std::cout << edge << " ";
            }
        }

        std::cout << std::endl;
        std::cout << "Activities of " << vortex.name << ": ";

        if (vortex.activities.empty()) {
            std::cout << "No activities yet" << std::endl;
            std::cout << std::endl;
            continue;
        }

        for (string *activity : vortex.activities) {
            std::cout << *activity << " ";
        }

        std::cout << "\n" << std::endl;
    }
}

void createVortex() {
    //This function creates a vortex and adds it to the list of vortexes
    //Receive: nothing
    //Return: nothing
    string vortexName;
    
    std::cout << "Enter the name of the place: ";
    std::getline(std::cin, vortexName);

    //Check if the vortex already exists
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            std::cout << "This place already exists" << std::endl;
            return;
        }
    }

    Vortex vortex(vortexName); //Create the vortex if it doesn't exist and add it to the list
    vortexList.push_back(vortex);
}

void deleteVortex() {
    //This function deletes a vortex from the list of vortexes
    //Receive: nothing
    //Return: nothing


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


void modifyVortex() {
    //This function modifies a vortex from the list of vortexes
    //Receive: nothing
    //Return: nothing
    
    int option;
    Vortex* vortexToModify = nullptr;
    string vortexName;
    std::cout << "Enter the name of the place you want to modify: ";
    std::getline(std::cin, vortexName);

    //Find the vortex with the name entered by the user
    for(Vortex& vortex: vortexList){
        if(vortex.name == vortexName){
            vortexToModify = &vortex;
            break;
        }
    }

    //If the vortex was not found, print an error message and return
    if(vortexToModify == nullptr){
        std::cout << "Invalid place" << std::endl;
        return;
    }

    //Print the options to modify the vortex
    std::cout << "What do you want to modify?" << std::endl;
    std::cout << "1. Name" << std::endl;
    std::cout << "2. Edges" << std::endl;
    std::cout << "3. Activities" << std::endl;
    std::cin >> option;

    std::cin.ignore();

    //Execute the option selected by the user
    switch (option) {
        case 1:
            modifyVortexName(*vortexToModify);
            break;
        case 2:
            //Modify the edges of the vortex
            break;
        case 3:
            //Modify the activities of the vortex
        default:
            std::cout << "Invalid option" << std::endl;
            break;
    }
}

void modifyVortexName(Vortex& vortex) {
    string newName;
    std::cout << "Enter the new name: ";
    std::getline(std::cin, newName);
    
    //Check if the new name already exists
    for (Vortex& vortex : vortexList) {
        if (vortex.name == newName) {
            std::cout << "There is a place that already has that name" << std::endl;
            return;
        }
    }
    
    //Change the name of the vortex
    for (Vortex& iterator : vortexList) {
        if (iterator.name == vortex.name) {
            iterator.name = newName;
            return;
        }
    }
}

void setEdge() {
    //This function creates an edge between two vortexes
    //Receive: nothing
    //Return: nothing
    
    int distance;
    Vortex* originVortex = nullptr;
    string originVortexName;
    string destinationVortexName;
    
    std::cout << "Enter the starting point: ";
    std::getline(std::cin, originVortexName);
    
    //Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == originVortexName) {
            originVortex = &vortex;
            break;
        }
    }
    
    //If the vortex was not found, print an error message and return
    if (originVortex == nullptr) {
        std::cout << "Invalid starting address" << std::endl;
        return;
    }
    
    std::cout << "Enter the destination: ";
    std::getline(std::cin, destinationVortexName);

    //Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == destinationVortexName) {
            try {
                std::cout << "Enter the distance: ";
                std::string distanceStr;
                std::cin >> distanceStr;
                std::cin.ignore();
    
                distance = std::stoi(distanceStr); //Convert the string to an integer

                if (distance <= 0) {
                    std::cout << "Invalid distance" << std::endl;
                    return;
                }

                Edge edge(distance, &vortex);
                originVortex->addEdge(edge);
                return;  
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
    activityList.push_back("Comer");

    setEdge();
    setEdge();

    vortex1.addActivity("Comer");

    //deleteVortex();
    //modifyVortex();
    printGraph();

    return 0;
}
