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
#include <algorithm>

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

void createMenu(const std::vector<string>& options) {
    int c = 1;
    for (const string& option : options) {
        std::cout << c << ". " << option << std::endl;
        c++;
    }
    std::cout << "0. Exit" << std::endl;
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

void uncheckGraph() {
    // This function unchecks the entire graph
    // Receive: nothing
    // Return: nothing

    for (Vortex& vortex : vortexList) {
        vortex.visited = false;
    }
}

void addEdge(Edge& edge, Vortex* vortex)  {
    //This method receive an edge and the pointer to the origin vortex, then add it to the vortexList, also add the bi-directional edge
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
}

void deleteEdge() {
    // This function deletes an edge from the list of Edges of the vortexes
    // Receive: nothing
    // Return: nothing

    string vortexName;
    string EdgeName;
    Vortex* destinationVortex = nullptr;

    std::cout << "Enter the name of the place you want to delete the edge: ";
    std::getline(std::cin, vortexName);

    std::cout << "Enter the destination of the edge you want to delete: ";
    std::getline(std::cin, EdgeName);

    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            destinationVortex = &vortex;
            auto edgeIt = vortex.edges.begin();
            while (edgeIt != vortex.edges.end()) {
                if (edgeIt->destination->name == EdgeName) {
                    vortex.edges.remove(*edgeIt); //If it founds the edge, it will delete it
                    std::cout << "Edge removed" << std::endl;
                    break;
                } else {
                    ++edgeIt;
                }
            }
            std::cout << "Invalid destination" << std::endl;
        }
    }
    
    // If the vortex was not found, print an error message and return
    if (destinationVortex == nullptr) {
        std::cout << "Invalid place" << std::endl;
        return;
    }
    
    //This loop delete the edge from the destination vortex
    for (Vortex& vortex : vortexList) {
        if (vortex.name == EdgeName) {
            auto edgeIt = vortex.edges.begin();
            while (edgeIt != vortex.edges.end()) {
                if (edgeIt->destination->name == vortexName) {
                    vortex.edges.remove(*edgeIt);
                    std::cout << "Edge removed" << std::endl;
                    return;
                } else {
                    ++edgeIt;
                }
            }
        }
    }
}

void modifyEdge() {
    // This function modifies an edge from the list of Edges of the vortexes
    // Receive: nothing
    // Return: nothing

    string vortexName;
    string EdgeName;
    Vortex* destinationVortex = nullptr;
    int newDistance;

    try {
        std::cout << "Enter the name of the place you want to modify the edge: ";
        std::getline(std::cin, vortexName);
    
        std::cout << "Those are the edges of this place: " << std::endl;
        for (Vortex& vortex : vortexList) {
            if (vortex.name == vortexName) {
                for (Edge& edge : vortex.edges) {
                    std::cout << edge << " ";
                }
            }
        }
        std::cout << std::endl;

        std::cout << "Enter the destination of the edge you want to modify: ";
        std::getline(std::cin, EdgeName);


        std::cout << "Enter the new destination distance: ";
        std::cin >> newDistance;
        std::cin.ignore();

        if (newDistance <= 0) {
            std::cout << "Invalid distance" << std::endl;
            return;
        }

        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input for distance. Please enter a valid numeric value." << std::endl;
            return;
    }

    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            destinationVortex = &vortex;
            auto edgeIt = vortex.edges.begin();
            while (edgeIt != vortex.edges.end()) {
                if (edgeIt->destination->name == EdgeName) {
                    edgeIt->distance = newDistance;
                    std::cout << "Edge modified" << std::endl;
                    break;
                } else {
                    ++edgeIt;
                }
            }
            std::cout << "Invalid destination" << std::endl;
        }
    }

    // If the vortex was not found, print an error message and return 
    if (destinationVortex == nullptr) {
        std::cout << "Invalid place" << std::endl;
        return;
    }

    //This loop modify the edge from the destination vortexes
    for (Vortex& vortex : vortexList) {
        if (vortex.name == EdgeName) {
            auto edgeIt = vortex.edges.begin();
            while (edgeIt != vortex.edges.end()) {
                if (edgeIt->destination->name == vortexName) {
                    edgeIt->distance = newDistance;
                    std::cout << "Edge modified" << std::endl;
                    return;
                } else {
                    ++edgeIt;
                }
            }
        }
    }
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

void edgeMenu() {
    // This function prints the edge menu
    // Receive: nothing
    // Return: nothing

    bool exit = false;
    system("clear");
    while (!exit) {
        std::vector<string> options = {"Create Edge", "Delete Edge", "Modify Edge"};
        createMenu(options);
        int option;
        try {
        std::cin >> option;
        std::cin.ignore();
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input for option. Please enter a valid numeric value." << std::endl;
            return;
        }
        if (option < 0 || option > static_cast<int>(options.size())) {
        std::cout << "Invalid option" << std::endl;
        return;
        }
        switch (option) {
        case 1:
            setEdge();
            break;
        case 2:
            deleteEdge();
            break;
        case 3:
            modifyEdge();
            break;
        case 0:
            exit = true;
            break;
        default:
            std::cout << "Invalid option" << std::endl;
            break;
        }
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
    // This function modifies the name of a vortex 
    // Receive: a pointer to a vortex 
    // Return: nothing

    string newName;
    std::cout << "Enter the new name: ";
    std::getline(std::cin, newName);

    // Check if the new name already exists
    for (Vortex& vortexIterator : vortexList) {
        if (vortexIterator.name == newName) {
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

void addActivity(Vortex& vortex) {
    // This function adds an activity to a vortex 
    // Receive: a vortex 
    // Return: nothing 

    string activityName;
    std::cout << "Enter the name of the activity: ";
    std::getline(std::cin, activityName);

    std::list<string>::iterator it = std::find(activityList.begin(), activityList.end(), activityName); //This search the activity in the list and give the pointer
    
    //If the acivity is in the list, it will add it to the vortex
    if (it != activityList.end()) {

        // Check if the activity is already in the vortex
        for (string* activity : vortex.activities) {
            if (*activity == *it) {
                std::cout << "Activity " << activityName << " is already in " << vortex.name << std::endl;
                return;
            }
        }


        // Add the activity to the vortex 
        vortex.activities.push_back(&(*it));
        return;
    }
    
    std::cout << "Activity not found" << std::endl;
}

void removeActivity(Vortex& vortex) {
    // This function removes an activity from a vortex 
    // Receive: a vortex 
    // Return: nothing

    string activityName;
    std::cout << "Enter the name of the activity: ";
    std::getline(std::cin, activityName);

    for (string* activity : vortex.activities) {
        if (*activity == activityName) {
            vortex.activities.remove(activity);
            std::cout << "Activity removed" << std::endl;
            return;
        }
    }
    
    std::cout << "Activity not found" << std::endl;
}

void modifyVortexActivities(Vortex& vortex) {
    // This function modifies the activities of a vortex 
    // Receive: a vortex 
    // Return: nothing
    
    bool exit = false; 
    system("clear");

    while (!exit) {
        std::vector<string> options = {"Add activity", "Delete activity"};
        createMenu(options);
        int option;
        try {
        std::cin >> option;
        std::cin.ignore();
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input for option. Please enter a valid numeric value." << std::endl;
            return;
        }
        if (option < 0 || option > static_cast<int>(options.size())) {
            std::cout << "Invalid option" << std::endl;
            return;
        }
        switch (option) {
        case 1:
            addActivity(vortex);
            break;
        case 2:
            removeActivity(vortex);
            break;
        case 0:
            exit = true;
            break;
        default:
            std::cout << "Invalid option" << std::endl;
            break;
        }

    }

}

void modifyVortex() {
    // This function modifies a vortex from the list of vortexes
    // Receive: nothing
    // Return: nothing

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

    std::cout << "Notice that if you want to modify vortex edges, you have to select Edges Menu" << std::endl;

    bool exit = false;
    system("clear");

    // Print the options to modify the vortex
    while (!exit) {
        int option;
        std::vector<string> options = {"Modify name", "Modify activities"};
        createMenu(options);
        try {
        std::cin >> option;
        std::cin.ignore();
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input for option. Please enter a valid numeric value." << std::endl;
            return;
        }
        // Execute the option selected by the user
        switch (option) {
        case 1:
            modifyVortexName(*vortexToModify);
            break;
        case 2:
            modifyVortexActivities(*vortexToModify);
            break;
        case 0:
            exit = true;
            break;
        default:
            std::cout << "Invalid option" << std::endl;
            break;
        }
    }
}

void findPaths(Vortex& originVortex, Vortex& targetVortex, std::vector<Vortex*>& path, std::vector<std::vector<Vortex*>>& allPaths, std::vector<float>& distances, float currentDistance) {
    // This function finds all the paths between two vortexes
    // Receive: two vortexes, a vector of vortexes, a vector of vectors of vortexes, a vector of distances and a float
    // Return: nothing

    // If the vortex was already visited, return
    if (originVortex.visited) {
        return;
    }

    originVortex.visited = true;
    path.push_back(&originVortex);

    // If the origin vortex is the same as the target vortex, add the path to the list of paths
    if (&originVortex == &targetVortex) {
        allPaths.push_back(path);
        distances.push_back(currentDistance);  
    }

    // Find the paths between the origin vortex and the target vortex by visiting all the edges of the origin vortex
    for (Edge& edge : originVortex.edges) {
        findPaths(*edge.destination, targetVortex, path, allPaths, distances, currentDistance + edge.distance);
    }

    // Mark the vortex as not visited and remove it from the path
    originVortex.visited = false;
    path.pop_back();
}

void findShortestPath(string originVortexName, string targetVortexName) {
    // This function finds the shortest path between two vortexes by using the findPaths function
    // Receive: two strings (the names of the vortexes) 
    // Return: nothing

    Vortex* originVortex = nullptr;
    Vortex* targetVortex = nullptr;

    std::vector<Vortex*> path;
    std::vector<std::vector<Vortex*>> allPaths;
    std::vector<float> distances;
    
    // Find the vortexes with the names entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == originVortexName) {
            originVortex = &vortex;
        }
        if (vortex.name == targetVortexName) {
            targetVortex = &vortex;
        }
    }

    // Call to findPaths function
    findPaths(*originVortex, *targetVortex, path, allPaths, distances, 0);
    
    // If there are no paths between the vortexes, print an error message and return
    if (allPaths.empty()) {
        std::cout << "There is no path between " << originVortexName << " and " << targetVortexName << std::endl;
        return;
    }

    // Find the shortest path
    auto minDistance = std::min_element(distances.begin(), distances.end());

    // If the shortest path was found, print it
    if (minDistance != distances.end()) {
        int minIndex = std::distance(distances.begin(), minDistance);
        std::cout << "The shortest path is: " << std::endl;

        for (size_t i = 0; i < allPaths[minIndex].size(); i++) {
            std::cout << allPaths[minIndex][i]->name;
            if (i < allPaths[minIndex].size() - 1) {
                std::cout << " -> ";
            }
        }
        
        std::cout << ". The distance is: " << distances[minIndex] << std::endl;

    } else {
        std::cout << "No minimum distance found." << std::endl;
    }

}

void amplitud() {
    // This function does a Breadth-First Search (BFS) traversal of the vertices in the Graph
    // Receive: nothing
    // Return: nothing
    // (For the sake of clarity, it is called 'amplitud' and not 'Breadth-First Search')

    std::cout << "Amplitud (BFS) traversal: " << std::endl;

    for (const Vortex& vortex : vortexList) {
        std::cout << vortex << ", destinations: ";
        for (const Edge& edge : vortex.edges) {
            std::cout << "(" << edge.distance << " -> " << edge.destination->name << ") ";
        }
        std::cout << std::endl;
    }
}

void profundidad(Vortex* originVortex, std::vector<Vortex*>& visitedVertices) {
    // This function does a Depth-First Search (DFS) traversal of the vertices in the Graph
    // Receive: a pointer to a vortex and a vector of visited vertices
    // Return: nothing
    // (For the sake of clarity, it is called 'profundidad' and not 'Depth-First Search')

    // If the vortex was already visited on this path, return
    if (originVortex == nullptr || std::find(visitedVertices.begin(), visitedVertices.end(), originVortex) != visitedVertices.end()) {
        return;
    }

    std::cout << "Profundidad (DFS) traversal: " << std::endl;
    std::cout << originVortex->name << "\t";
    visitedVertices.push_back(originVortex);

    for (const Edge& edge : originVortex->edges) {
        std::cout << " (" << edge.distance << " -> " << edge.destination->name << ") ";
        profundidad(edge.destination, visitedVertices);
    }

    // After visiting all adjacent vertices, remove the current vortex from the vector 
    originVortex->visited = false;
    visitedVertices.pop_back();

    std::cout << std::endl;
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
    string activity1 = "Comer";
    string activity2 = "Dormir";
    string activity3 = "Estudiar";

    activityList.push_back(activity1);
    activityList.push_back(activity2);
    activityList.push_back(activity3);
    setEdge();
    setEdge();
    setEdge();
    //setEdge();
    //setEdge();
    //setEdge();
    //edgeMenu();
    //modifyVortexName(vortex1);
    //addActivity(vortex1);
    //modifyVortex();
    //findShortestPath("San Ramon", "Limon");
    amplitud();
    std::cout << std::endl;
    uncheckGraph();
    std::vector<Vortex*> visitedVertices;
    profundidad(&vortex1, visitedVertices);
    uncheckGraph();
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
    //printGraph();
    return 0;
}
