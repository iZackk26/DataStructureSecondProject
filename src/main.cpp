#include <Edge.hh>
#include <InvalidInputException.hh>
#include <Person.hh>
#include <Tree.hh>
#include <Vortex.hh>
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits> // Limpiar el buffer de entrada
#include <list>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using std::string;
std::list<Vortex> vortexList;
std::list<string> activityList;
Tree* peopleTree = new Tree();
Person* peopleList = nullptr;
size_t currentPeopleSize = 0;

int getIntegerInput() {
    int result;
    std::cin >> result;

    if (std::cin.fail()) {
        // La entrada no es un entero válido
        std::cin.clear();                                                   // Limpiar el indicador de error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpiar el buffer de entrada
        throw InvalidInputException();
    }

    return result;
}

// This function opens a file and append information to it in binary mode
// Receives: a person object and the file name
// Returns: nothing
void writeToFile(Person* person, const string& fileName, size_t size) {
    std::ofstream file(fileName, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    // write the size of the person class
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    for (size_t i = 0; i < size; i++) {
        file.write(reinterpret_cast<const char*>(&person[i]), sizeof(Person));
    }
    // file.write(reinterpret_cast<const char*>(person), sizeof(Person) * size);
    file.close();

    if (!file.good()) {
        throw std::runtime_error("Error occurred at writing time");
    }
}
void load(Person** person, string filename) {
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
    currentPeopleSize = size;
}

void addPerson(const Person& newPerson, const string& fileName) {
    Person* existingPeople = nullptr;
    load(&existingPeople, fileName);

    size_t newSize = currentPeopleSize + 1;
    Person* updatedPeople = new Person[newSize];

    for (size_t i = 0; i < currentPeopleSize; ++i) {
        updatedPeople[i] = existingPeople[i];
    }

    updatedPeople[currentPeopleSize] = newPerson;

    writeToFile(updatedPeople, fileName, newSize);

    // Liberar la memoria de las listas temporales
    delete[] existingPeople;
    delete[] updatedPeople;
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
    std::cout << "Graph: \n"
              << std::endl;

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

        std::cout << "\n"
                  << std::endl;
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

void addEdge(Edge& edge, Vortex* vortex) {
    // This method receive an edge and the pointer to the origin vortex, then add it to the vortexList, also add the bi-directional edge
    // Receive: an edge
    // Return: void

    vortex->edges.push_back(edge); // This add the edge to the origin vortex

    // This loop add the edge to the destination vortex but the destination will be the origin vortex
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
    std::cout << "Deleting an edge \n"
              << std::endl;

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
                    vortex.edges.remove(*edgeIt); // If it founds the edge, it will delete it
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

    // This loop delete the edge from the destination vortex
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
    std::cout << "Modifying an edge \n"
              << std::endl;

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

    // This loop modify the edge from the destination vortexes
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
    std::cout << "Creating an edge between two places \n"
              << std::endl;

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

void setTrunkEdge(const std::string& originVortexName, const std::string& destinationVortexName, int distance) {
    // This function creates an edge between two vortexes based on their names
    // Receive: originVortexName (name of the origin vortex), destinationVortexName (name of the destination vortex), distance
    // Return: nothing

    Vortex* originVortex = nullptr;

    // Find the vortex with the name entered as the origin vortex
    for (Vortex& vortex : vortexList) {
        if (vortex.name == originVortexName) {
            originVortex = &vortex;
            break;
        }
    }

    // If the origin vortex was not found, print an error message and return
    if (originVortex == nullptr) {
        std::cout << "Invalid starting address" << std::endl;
        return;
    }

    Vortex* destinationVortex = nullptr;

    // Find the vortex with the name entered as the destination vortex
    for (Vortex& vortex : vortexList) {
        if (vortex.name == destinationVortexName) {
            destinationVortex = &vortex;
            break;
        }
    }

    // If the destination vortex was not found, print an error message and return
    if (destinationVortex == nullptr) {
        std::cout << "Invalid destination" << std::endl;
        return;
    }

    // Ensure that the distance is valid
    if (distance <= 0) {
        std::cout << "Invalid distance" << std::endl;
        return;
    }

    Edge edge(distance, destinationVortex);
    addEdge(edge, originVortex);
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
    std::cout << "Creating a place \n"
              << std::endl;

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
    std::cout << "Deleting a place \n"
              << std::endl;

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
    std::cout << "Modifying the name of a place \n"
              << std::endl;

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

    std::cout << "Adding an activity to a place \n"
              << std::endl;

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

    std::list<string>::iterator it = std::find(activityList.begin(), activityList.end(), activityName); // This search the activity in the list and give the pointer

    // If the acivity is in the list, it will add it to the vortex
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

    std::cout << "Removing an activity from a place \n"
              << std::endl;

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

void printTreeDepth(Tree* root, int depth) {
    if (root == nullptr) {
        return;
    }

    // Print the current node with its depth
    for (int i = 0; i < depth; ++i) {
        std::cout << "  "; // Adjust the spacing for better visualization
    }
    std::cout << "|-- " << root->clasification << std::endl;

    // Recursively print children
    for (Tree* child : root->children) {
        printTreeDepth(child, depth + 1);
    }
}

// This function adds a person to the tree
// Receive: nothing
// Return: nothing
//
enum Option {
    GENDER = 1,
    AGE = 2,
    PLACE_OF_RESIDENCE = 3,
    ACTIVITY = 4
};

void createDecisionTree(std::vector<Option> choices, Person* person, Tree* root) {
    if (choices.empty() || root == nullptr) {
        return;
    }
    Option choice = choices[0];
    // Identify the feature based on the choice
    string feature;
    switch (choice) {
    case GENDER:
        feature = person->gender;
        break;
    case AGE:
        feature = std::to_string(person->age);
        break;
    case PLACE_OF_RESIDENCE:
        feature = person->endRute;
        break;
    case ACTIVITY:
        feature = person->activity;
        break;
    default:
        // Handle invalid choice
        return;
    }
    Tree* child = root->addChildIfNotExist(feature);
    // Recursively call for the next feature in the order
    createDecisionTree(std::vector<Option>(choices.begin() + 1, choices.end()), person, child);
}

void setOrder() {
    std::vector<string> options = {"Gender", "Age", "Place of Residence", "Activity"};
    std::vector<Option> order;
    createMenu(options);
    int option;
    for (size_t i = 0; i < options.size(); i++) {
        std::cin >> option;
        std::cin.ignore();
        if (option > 4 || option < 0) {
            std::cout << "Invalid option" << std::endl;
            i--;
            continue;
        }
        order.push_back((Option)option);
    }
    for (size_t i = 0; i < currentPeopleSize; i++) {
        createDecisionTree(order, &peopleList[i], peopleTree);
    }
    std::cout << "Tree successfully sorted" << std::endl;
}

void printPeopleInformation() {
    load(&peopleList, "Information/Data.bin");
    for (size_t i = 0; i < currentPeopleSize; i++) {
        std::cout << peopleList[i] << std::endl;
    }

}

void treeMenu() {
    std::vector<string> options = {"Sort Tree", "Print Tree", "Print People Information"};
    load(&peopleList, "Information/Data.bin");
    createMenu(options);
    int option;
    std::cin >> option;
    std::cin.ignore();
    if (option > 2 || option < 0) {
        std::cout << "Invalid option" << std::endl;
        return;
    }
    switch (option) {
    case 0:
        break;
    case 1:
        setOrder();
        break;
    case 2:
        std::cout << "====Printing Tree====" << std::endl;
        printTreeDepth(peopleTree, 0);
        std::cout << "=====================" << std::endl;
        break;
    default:
        std::cout << "Invalid option" << std::endl;
        break;
    }
}

void registerUser() {
    // This function is called due to a petition to search the best route for a person
    // It will ask for the information of the person and will save it in a file
    // Receive: nothing
    // Return: nothing
    bool error = false;
    char gender[10];
    int age;
    char beginingRoute[30];
    char endRute[30];
    char activity[20];
    std::cout << "Registering a new user \n"
              << std::endl;
    std::cout << "Type your gender: Male/Female";
    std::cin >> gender;

    while (!error) {
        try {
            std::cout << "Type your age: ";
            age = getIntegerInput();
            if (age < 0) {
                std::cout << "Invalid age" << std::endl;
            } else {
                error = true;
            }
        } catch (const InvalidInputException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    std::cout << "Type your place of residence: ";
    std::cin >> beginingRoute;
    std::cout << "Type your destination: ";
    std::cin >> endRute;
    std::cout << "Type your activity: ";
    std::cin >> activity;
    Person newPerson(gender, age, beginingRoute, endRute, activity);
    addPerson(newPerson, "Information/Data.bin");
}

void setPeople(string fileName) {
    Person p1("Male", 18, "San Ramon", "Santa Clara", "Comer");
    Person p2("Female", 18, "Heredia", "Alajuela", "Comer");
    Person p3("Male", 20, "Palmares", "San Ramon", "Comer");
    Person p4("Male", 20, "Chachagua", "Alajuela", "Comer");
    Person p5("Female", 24, "San Jose", "San Ramon", "Comer");
    Person p6("Female", 24, "Ciudad Quesada", "San Ramon", "Comer");
    Person p7("Male", 23, "Grecia", "Palmares", "Comer");
    Person p8("Male", 23, "Naranjo", "Alajuela", "Comer");
    Person p9("Female", 27, "San Ramon", "Heredia", "Comer");
    Person p10("Male", 27, "Liberia", "Puntarenas", "Comer");
    Person p11("Female", 32, "Palmares", "Naranjo", "Comer");
    Person p12("Male", 32, "San Jose", "Ciudad Quesada", "Comer");
    Person p13("Female", 18, "Ciudad Quesada", "Fortuna", "Comer");
    Person p14("Male", 22, "Liberia", "Alajuela", "Comer");
    Person p15("Female", 22, "Grecia", "San Ramon", "Comer");
    Person p16("Male", 30, "Fortuna", "Grecia", "Comer");
    Person p17("Male", 30, "Naranjo", "Palmares", "Comer");
    Person p18("Female", 20, "Ciudad Quesada", "Grecia", "Comer");
    Person p19("Male", 23, "San Ramon", "Alajuela", "Comer");
    Person p20("Female", 26, "Heredia", "San Jose", "Comer");
    Person p21("Male", 27, "Palmares", "Ciudad Quesada", "Comer");
    Person p22("Female", 29, "Naranjo", "Liberia", "Comer");
    Person p23("Male", 35, "San Jose", "Heredia", "Comer");
    Person p24("Male", 35, "Grecia", "Palmares", "Comer");
    Person p25("Female", 18, "Ciudad Quesada", "Liberia", "Comer");
    Person p26("Male", 22, "Naranjo", "Fortuna", "Comer");
    Person p27("Female", 35, "Palmares", "San Jose", "Comer");
    Person p28("Male", 26, "Ciudad Quesada", "Grecia", "Comer");
    Person p29("Male", 30, "San Ramon", "Naranjo", "Comer");
    Person p30("Female", 30, "Grecia", "Liberia", "Comer");

    Person list[30] = {p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29, p30};
    writeToFile(list, fileName, 30);
}

void appendGlobalActivity() {
    // This function appends an activity to the global list of activities
    // Receive: nothing
    // Return: nothing

    system("clear");
    std::cout << "Appending an activity to the global list of activities \n"
              << std::endl;

    string activityName;

    std::cout << "Enter the name of the activity: ";
    std::getline(std::cin, activityName);

    // Check if the activity is already in the list
    for (string activity : activityList) {
        if (activity == activityName) {
            std::cout << "This activity is already in the list" << std::endl;
            return;
        }
    }

    // Add the activity to the list
    activityList.push_back(activityName);
}

void deleteGlobalActivity() {
    // This function deletes an activity from the global list of activities
    // Receive: nothing
    // Return: nothing
    system("clear");
    std::cout << "Deleting an activity from the global list of activities \n"
              << std::endl;

    string activityName;

    std::cout << "Enter the name of the activity: ";
    std::getline(std::cin, activityName);

    // Check if the activity is in the list and if it is, delete it
    for (string activity : activityList) {
        if (activity == activityName) {
            activityList.remove(activity);
            std::cout << "Activity removed" << std::endl;
            return;
        }
    }

    std::cout << "Activity not found" << std::endl;
}

void setGraph() {
    // This function creates the graph
    // Receive: nothing
    // Return: nothing

    Vortex moncho("San Ramon");
    Vortex palmares("Palmares");
    Vortex naranjo("Naranjo");
    Vortex grecia("Grecia");
    Vortex alajuela("Alajuela");
    Vortex poas("Poas");
    Vortex cq("Ciudad Quesada");
    Vortex fortuna("Fortuna");
    Vortex liberia("Liberia");
    Vortex puntarenas("Puntarenas");

    vortexList.push_back(moncho);
    vortexList.push_back(palmares);
    vortexList.push_back(naranjo);
    vortexList.push_back(grecia);
    vortexList.push_back(alajuela);
    vortexList.push_back(poas);
    vortexList.push_back(cq);
    vortexList.push_back(fortuna);
    vortexList.push_back(liberia);
    vortexList.push_back(puntarenas);

    setTrunkEdge("Liberia", "Puntarenas", 136);      // 1
    setTrunkEdge("Liberia", "Fortuna", 135);         // 2
    setTrunkEdge("Liberia", "San Ramon", 162);       // 3
    setTrunkEdge("San Ramon", "Fortuna", 71);        // 4
    setTrunkEdge("San Ramon", "Puntarenas", 54);     // 5
    setTrunkEdge("San Ramon", "Palmares", 8);        // 6
    setTrunkEdge("San Ramon", "Naranjo", 22);        // 7
    setTrunkEdge("San Ramon", "Ciudad Quesada", 37); // 8
    setTrunkEdge("Palmares", "Puntarenas", 59);      // 9
    setTrunkEdge("Palmares", "Naranjo", 19);         // 10
    setTrunkEdge("Palmares", "Grecia", 27);          // 11
    setTrunkEdge("Palmares", "Alajuela", 40);        // 12
    setTrunkEdge("Fortuna", "Ciudad Quesada", 45);   // 13
    setTrunkEdge("Ciudad Quesada", "Naranjo", 40);   // 14
    setTrunkEdge("Ciudad Quesada", "Poas", 80);      // 15
    setTrunkEdge("Naranjo", "Grecia", 16);           // 16
    setTrunkEdge("Naranjo", "Poas", 27);             // 17
    setTrunkEdge("Grecia", "Poas", 15);              // 18
    setTrunkEdge("Grecia", "Alajuela", 20);          // 19
    setTrunkEdge("Poas", "Alajuela", 18);            // 20
}

void calculateRoute() {
    // This function calculates the shortest path between two vortexes if the activity is available in the destination
    // Receive: nothing
    // Return: nothing

    system("clear");
    std::cout << "Calculating the route \n"
              << std::endl;

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
    // ** CALL HERE FOR THE CREATION OF THE PERSON INFORMATION **
    registerUser();
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

void globalActivityMenu() {
    // This function prints the global activity menu
    // Receive: nothing
    // Return: nothing

    bool exit = false;
    system("clear");
    while (!exit) {
        std::vector<string> options = {"Append activity", "Delete activity", "Print activity list"};
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
        case 0:
            exit = true;
            break;
        case 1:
            appendGlobalActivity();
            break;
        case 2:
            deleteGlobalActivity();
            break;
        case 3:
            std::cout << "Global activity list: " << std::endl;
            for (string activity : activityList) {
                std::cout << activity << std::endl;
            }
            break;
        default:
            std::cout << "Invalid option" << std::endl;
            break;
        }
    }
    return;
}

int main() {
    setGraph();
    string dataFile = "Information/Data.bin";
    bool exit = false;
    setPeople(dataFile);
    while (!exit) {
        std::vector<string> options = {"Calculate Route", "Print Graph", "Vortex Options", "Edge Options", "Activity Options", "Global Activity Options", "Order Tree and People Information"};
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
        case 6:
            globalActivityMenu();
        case 7:
            treeMenu();
            break;
        case 0:
            exit = true;
            break;
        default:
            std::cout << "Invalid option" << std::endl;
        }
    }
    return 0;
}
