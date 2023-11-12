#include <Person.hh>
#include <Tree.hh>
#include <list>
#include <string>

using std::string;

// Constructor
Tree::Tree(string clasification) {
    this->clasification = clasification;
    this->children = std::list<Tree*>();
    this->filePosition = std::list<int*>();
}

/* Constructor */
Tree::Tree() {
    this->clasification = "";
    this->children = std::list<Tree*>();
    this->filePosition = std::list<int*>();
}
