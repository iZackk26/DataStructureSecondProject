#pragma once
#include <Person.hh>
#include <iostream>
#include <list>
#include <string>

using std::string;

class Tree {
  public:
    string clasification;      // Tipo de clasificacion
    std::list<Tree*> children; // Lista de hijos
    int level;

    // List of people
    std::list<Person*> filePosition; // Lista de posiciones en el archivo

    // Constructor
    Tree(string clasification);
    Tree();

    // find a child with the given clasification
    Tree* findChild(string clasification) {
        for (Tree* child : this->children) {
            if (child->clasification == clasification) {
                return child;
            }
        }
        return nullptr;
    }
    // add a child with the given clasification if it doesn't exist
    Tree* addChildIfNotExist(const std::string& cls, Person& person) {
        Tree* child = findChild(cls);
        if (child != nullptr) {
            child->filePosition.push_back(new Person(person));
            return child;
        }
        Tree* newChild = new Tree(cls);
        this->children.push_back(newChild);
        newChild->filePosition.push_back(new Person(person));
        return newChild;
    }
};
