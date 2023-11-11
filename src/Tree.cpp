#include <Tree.hh>
#include <Person.hh>
#include <list>
#include <string>

using std::string;

// Constructor
Tree::Tree(string clasification){
    this->clasification = clasification;
    this->children = std::list<Tree*>();
    this->filePosition = std::list<int*>();
}

void Tree::addChild(Tree* child){
    this->children.push_back(child);
}

/* Constructor */
Tree::Tree(){
    this->clasification = "";
    this->children = std::list<Tree*>();
    this->filePosition = std::list<int*>();
}

bool Tree::checkRepeteatedClasification(string clasification){
    for (Tree* child : this->children){
        std::cout << child->clasification << std::endl;
        if (child->clasification.compare(clasification) == 0 || child->clasification.empty()){
            return true;
        }
    }
    return false;
}
