#pragma once
#include <iostream>
#include <string>
#include <list>

using std::string;

class Person{
    public:
        string gender;
        int age;
        string beginingRute;
        string endRute;
        std::list<string> activities;

        // Consturctors
        Person(string gender, int age, string beginingRute, string endRute);
        Person();

        friend std::ostream& operator<<(std::ostream& os, const Person& p){
            os << p.gender << " " << p.age << " " << p.beginingRute << " " << p.endRute << " ";
            return os;
        }

        friend std::istream& operator>>(std::istream& is, Person& p){
            is >> p.gender >> p.age >> p.beginingRute >> p.endRute;
            return is;
        }

};
