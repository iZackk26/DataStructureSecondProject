#include <Person.hh>
#include <list>
#include <string>
#include <cstring>

using std::string;

Person::Person(const char* gender, int age, const char* beginingRoute, const char* endRute, const char* activity) {
    strncpy(this->gender, gender, sizeof(this->gender));
    this->age = age; 
    strncpy(this->beginingRoute, beginingRoute, sizeof(this->beginingRoute));
    strncpy(this->endRute, endRute, sizeof(this->endRute));
    strncpy(this->activity, activity, sizeof(this->activity));
}

Person::Person() {
}
