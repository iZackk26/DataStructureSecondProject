#include <Person.hh>
#include <list>
#include <string>

using std::string;

Person::Person(string gender, int age, string startRute, string endRute){
    this->gender = gender;
    this->age = age;
    this->beginingRute = startRute;
    this->endRute = endRute;
    /* this->activities = activities; */
}


Person::Person(){
    this->gender = "";
    this->age = 0;
    this->beginingRute = "";
    this->endRute = "";
    this->activities = {}; //*
}


