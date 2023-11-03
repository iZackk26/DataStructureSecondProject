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
    
    system("clear");
    std::cout << "Graph: \n" << std::endl;

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

    system("clear");
    std::cout << "Deleting an edge \n" << std::endl;

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

    system("clear");
    std::cout << "Modifying an edge \n" << std::endl;

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

    system("clear");
    std::cout << "Creating an edge between two places \n" << std::endl;

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
    system("clear");
    bool exit = false;
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

    system("clear");
    std::cout << "Creating a place \n" << std::endl;

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

    system("clear");
    std::cout << "Deleting a place \n" << std::endl;

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

void modifyVortexName() {
    // This function modifies the name of a vortex 
    // Receive: nothing
    // Return: nothing
    
    system("clear");
    std::cout << "Modifying the name of a place \n" << std::endl;

    string vortexName;
    string newName;
    Vortex* vortexToModify = nullptr;
    
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
        if (iterator.name == vortexName) {
            iterator.name = newName;
            return;
        }
    }
}

void addActivity() {
    // This function adds an activity to a vortex 
    // Receive: nothing 
    // Return: nothing 

    system("clear");

    string activityName;
    string vortexName;
    Vortex* vortexToAdd = nullptr;

    std::cout << "Adding an activity to a place \n" << std::endl;
    
    std::cout << "Enter the name of the place: ";
    std::getline(std::cin, vortexName);
    
    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            vortexToAdd = &vortex;
            break;
        }
    }

    // If the vortex was not found, print an error message and return
    if (vortexToAdd == nullptr) {
        std::cout << "Invalid place" << std::endl;
        return;
    }
    

    std::cout << "Enter the name of the activity: ";
    std::getline(std::cin, activityName);

    std::list<string>::iterator it = std::find(activityList.begin(), activityList.end(), activityName); //This search the activity in the list and give the pointer
    
    //If the acivity is in the list, it will add it to the vortex
    if (it != activityList.end()) {

        // Check if the activity is already in the vortex
        for (string* activity : vortexToAdd->activities) {
            if (*activity == *it) {
                std::cout << "Activity " << activityName << " is already in " << vortexToAdd->name << std::endl;
                return;
            }
        }

        // Add the activity to the vortex 
        vortexToAdd->activities.push_back(&(*it));
        return;
    }
    
    std::cout << "Activity not found" << std::endl;
}

void removeActivity() {
    // This function removes an activity from a vortex 
    // Receive: nothing
    // Return: nothing

    system("clear");

    string vortexName;
    Vortex* vortexToRemove = nullptr;

    string activityName;

    std::cout << "Removing an activity from a place \n" << std::endl;

    std::cout << "Enter the name of the place: ";
    std::getline(std::cin, vortexName);

    // Find the vortex with the name entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == vortexName) {
            vortexToRemove = &vortex;
            break;
        }
    }

    // If the vortex was not found, print an error message and return 
    if (vortexToRemove == nullptr) {
        std::cout << "Invalid place" << std::endl;
        return;
    }

    std::cout << "Enter the name of the activity: ";
    std::getline(std::cin, activityName);

    // Check if the activity is in the vortex and if it is, remove it
    for (string* activity : vortexToRemove->activities) {
        if (*activity == activityName) {
            vortexToRemove->activities.remove(activity);
            std::cout << "Activity removed" << std::endl;
            return;
        }
    }
    
    std::cout << "Activity not found" << std::endl;
}

void modifyVortexActivities() {
    // This function modifies the activities of a vortex 
    // Receive: nothing
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
            addActivity();
            break;
        case 2:
            removeActivity();
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

    bool exit = false;
    system("clear");
    std::cout << "Notice that if you want to modify the edges, you have to go to the edge menu" << std::endl;

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
            modifyVortexName();
            break;
        case 2:
            modifyVortexActivities();
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
    
    system("clear");
    std::cout << "Amplitud (BFS) traversal: " << std::endl;

    for (const Vortex& vortex : vortexList) {
        std::cout << vortex << ", destinations: ";
        for (const Edge& edge : vortex.edges) {
            std::cout << "(" << edge.distance << " -> " << edge.destination->name << ") ";
        }
        std::cout << std::endl;
    }
}

void profundidad(Vortex* originVortex) {
    // This function does a Depth-First Search (DFS) traversal of the vertices in the Graph 
    // Receive: a pointer to a vortex 
    // Return: nothing 
    // (For the sake of clarity, it is called 'profundidad' and not 'Depth-First Search')

    if (originVortex->visited) {
        return;
    }

    for (Vortex& vortex : vortexList) {
        if (vortex.name == originVortex->name) {
            vortex.visited = true;
        }
    }

    for (Edge& edge : originVortex->edges) {
        std::cout << originVortex->name << std::endl;
        std::cout << "Now visiting: " << edge << std::endl;
        Vortex* neighbor = edge.destination;
        if (!neighbor->visited) {
            profundidad(neighbor);
        }
    }
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
    addActivity();
    //modifyVortex();
    //findShortestPath("San Ramon", "Limon");
    //amplitud();
    //std::cout << std::endl;
    //uncheckGraph();
    //std::vector<Vortex*> visitedVertices;
    //Vortex firstVortex = vortexList.front();
    //std::cout << "Profundidad (DFS) traversal: " << std::endl;
    //profundidad(&firstVortex);
    //uncheckGraph();
}

void calculateRoute() {
    // This function calculates the shortest path between two vortexes if the activity is available in the destination 
    // Receive: nothing
    // Return: nothing

    system("clear");
    std::cout << "Calculating the route \n" << std::endl;

    string startPoint;
    string destination;
    string activityToDo;

    Vortex* originVortex = nullptr;
    Vortex* targetVortex = nullptr;

    bool activityInList = false;
    bool activityInDestination = false;


    std::cout << "Enter the starting point: ";
    std::getline(std::cin, startPoint);

    std::cout << "Enter the destination: ";
    std::getline(std::cin, destination);

    // Find the vortexes with the names entered by the user
    for (Vortex& vortex : vortexList) {
        if (vortex.name == startPoint) {
            originVortex = &vortex;
        }
        if (vortex.name == destination) {
            targetVortex = &vortex;
        }
    }

    // If the vortexes were not found, print an error message and return
    if (originVortex == nullptr || targetVortex == nullptr) {
        std::cout << "Invalid starting point or destination" << std::endl;
        return;
    }

    std::cout << "Enter the activity you want to do: ";
    std::getline(std::cin, activityToDo);

    // Check if the activity is in the list of activities
    for (string activity : activityList) {
        if (activity == activityToDo) {
            activityInList = true;
            break;
        }
    }

    // If the activity is not in the list, print an error message and return
    if (!activityInList) {
        std::cout << "Invalid activity" << std::endl;
        return;
    }

    // Check if the activity is in the destination
    for (string* activity : targetVortex->activities) {
        if (*activity == activityToDo) {
            activityInDestination = true;
            break;
        }
    }

    // If the activity is not in the destination, print an error message (gives and advice) and return
    if (!activityInDestination) {
        std::cout << "The destination doesn't have that activity, maybe you can look for another place" << std::endl;
        return;
    }

    // Call to findShortestPath function in order to find the shortest path between the vortexes
    findShortestPath(startPoint, destination);
    
}

void vortexMenu() {
    // This function prints the vortex menu 
    // Receive: nothing
    // Return: nothing
    
    bool exit = false;
    system("clear");
    while (!exit) {
    std::vector<string> options = {"Create Vortex", "Delete Vortex", "Modify Vortex"};
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
        createVortex();
        break;
    case 2:
        deleteVortex();
        break;
    case 3:
        modifyVortex();
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

void activityMenu() {
    // This function prints the activity menu 
    // Receive: nothing 
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
            addActivity();
            break;
        case 2:
            removeActivity();
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

void printGraphMenu() {
    // This function prints the graph menu
    // Receive: nothing
    // Return: nothing

    bool exit = false;
    system("clear");
    while (!exit) {
        std::vector<string> options = {"Amplitud", "Profundidad", "Print Whole Graph"};
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
            uncheckGraph();
            amplitud();
            uncheckGraph();
            break;
        case 2:
            uncheckGraph();
            profundidad(&vortexList.front());
            uncheckGraph();
            break;
        case 3:
            printGraph();
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

int main() {
    setVortex();
    string dataFile = "Information/Data.bin";
    Person* list = nullptr;
    bool exit = false;
    while (!exit) {
        std::vector<string> options = {"Calculate Route", "Print Graph", "Vortex Options", "Edge Options", "Activity Options"};
        createMenu(options);
        int option;
        std::cin >> option;
        std::cin.ignore();
        if (option < 0 || option > static_cast<int>(options.size())) {
            std::cout << "Invalid option" << std::endl;
            continue;
        }
        switch (option) {
        case 1:
            calculateRoute();
            break;
        case 2:
            printGraphMenu();
            break;
        case 3:
            vortexMenu();
            break;
        case 4:
            edgeMenu();
            break;
        case 5:
            activityMenu();
            break;
        case 0:
            exit = true;
            break;
        default:
            std::cout << "Invalid option" << std::endl;
        }

        
    }
    size_t size = load(&list, "Information/Data.bin");
    for (size_t i = 0; i < size; i++) {
        std::cout << list[i].gender << std::endl;
    }

    return 0;
}
