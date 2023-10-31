#include <Tree.hh>
#include <Person.hh>
#include <list>
#include <string>

using std::string;

// Constructor
Tree::Tree(int quantity, string clasification){
    this->quantity = quantity;
    this->clasification = clasification;
}

void Tree::addChild(Tree* child){
    this->children.push_back(child);
}
