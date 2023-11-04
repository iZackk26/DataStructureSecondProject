#include <Tree.hh>
#include <Person.hh>
#include <list>
#include <string>

using std::string;

// Constructor
Tree::Tree(int quantity, string clasification){
    this->clasification = clasification;
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
        std::cout << "clasification inside Check" <<clasification << std::endl;
        std::cout << "Child clasification" << child->clasification << std::endl;
        std::cout << "COMPARE" << clasification.compare(child->clasification) << std::endl;
        if (child->clasification.compare(clasification) == 0){
            return true;
        }
    }
    return false;
}
