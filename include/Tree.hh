#pragma once
#include <iostream>
#include <string>
#include <Person.hh>
#include <list>

using std::string;

class Tree{ 
    public:
        string clasification; // Tipo de clasificacion
        std::list<Tree*> children; // Lista de hijos

        // List of people
        std::list<int*> filePosition; // Lista de posiciones en el archivo

        // Constructor
        Tree(string clasification);
        Tree();

        // Add child
        void addChild(Tree* child);

        bool checkRepeteatedClasification(string clasification);

};
