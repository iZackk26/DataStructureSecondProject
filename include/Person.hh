#pragma once
#include <iostream>
#include <string>
#include <list>

using std::string;

class Person{
    public:
        char gender[10];
        int age;
        char beginingRute[30];
        char endRute[30];
        char activity[20];

        // Consturctors
        Person(const char* gender, int age, const char* beginingRute, const char* endRute, const char* activity);
        Person();

        friend std::ostream& operator<<(std::ostream& os, const Person& p){
            os << p.gender << " " << p.age << " " << p.beginingRute << " " << p.endRute << " " << p.activity;
            return os;
        }

        friend std::istream& operator>>(std::istream& is, Person& p){
            is >> p.gender >> p.age >> p.beginingRute >> p.endRute >> p.activity;
            return is;
        }

};
