#include <Person.hh>
#include <list>
#include <string>
#include <cstring>

using std::string;

Person::Person(const char* gender, int age, const char* beginingRute, const char* endRute, const char* activity) {
    strncpy(this->gender, gender, sizeof(this->gender));
    this->age = age; 
    strncpy(this->beginingRute, beginingRute, sizeof(this->beginingRute));
    strncpy(this->endRute, endRute, sizeof(this->endRute));
    strncpy(this->activity, activity, sizeof(this->activity));
}

Person::Person() {
}
