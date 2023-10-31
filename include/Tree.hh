#pragma once
#include <iostream>
#include <string>
#include <Person.hh>
#include <list>

using std::string;

class Tree{
    public:
        int quantity; // Cantidad
        string clasification; // Tipo de clasificacion
        int age; // Edad
        std::list<Tree*> children; // Lista de hijos

        // List of people
        std::list<int*> filePosition; // Lista de posiciones en el archivo

        // Constructor
        Tree(int quantity, string clasification);

        // Add child
        void addChild(Tree* child);

};
