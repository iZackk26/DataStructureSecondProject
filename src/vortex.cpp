#include <Vortex.hh>
#include <Edge.hh>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

static std::list<Vortex> vortexList; //Definition of the list declared in the header
static std::list<string> activityList; //Definition of the list declared in the header

Vortex::Vortex(string name) {
    //This is the constructor
    this->name = name;
    this->edges = std::list<Edge>();
    this->activities = std::list<string*>();
}

Vortex::Vortex() {
    //This is the empty constructor
}

std::ostream& operator<<(ostream& os, const Vortex& vortex) {
    //This method is used to print the vortex
    //Receive: ostream and a vortex
    //Return: ostream
    os << vortex.name;
    return os;
}

void Vortex::addActivity(string activity) {
    //This method receive an activity, checks if it is in the main activities list and add it to the vortex activities list
    //Receive: the name of the activity
    //Return: void
    
    std::list<string>::iterator it = std::find(activityList.begin(), activityList.end(), activity); //This search the activity in the list and give the pointer
    
    if (it != activityList.end()) {
        //This loop checks if the activity is already in the vortex 
        for (string* activity : this->activities) {
            if (*activity == *it) {
                std::cout << "Activity " << activity << " is already in " << this->name << std::endl;
                return;
            }
        }
        
        this->activities.push_back(&(*it)); //This add the activity to the vortexList

        //This loop udpate the list 
        for (Vortex& vortex: vortexList) {
            if (vortex.name == this->name) {
                vortex.activities = this->activities;
                std::cout << "Activity " << activity << " added to " << vortex.name << std::endl;
                return;
            }
        }
    }

    std::cout << "Activity " << activity << " not found" << std::endl;

}

void Vortex::removeActivity(string activity) {
    //This method receive an activity and remove it from the vortexList
    //Receive: pointer to an activity
    //Return: void

    std::list<string>::iterator it = std::find(activityList.begin(), activityList.end(), activity); //This search the activity in the list and give the pointer
    
    if (it != activityList.end()) {
        for (string* activity : this->activities) {
            if (*activity == *it) {
                this->activities.remove(activity); //This remove the activity from the vortex activity list
                std::cout << "Activity " << *activity << " removed from " << this->name << std::endl;
                break;
            }
        }

        //This loop updates the vortex 
        for (Vortex& vortex: vortexList) {
            if (vortex.name == this->name) {
                vortex.activities = this->activities;
                return;
            }
        }

        std::cout << "The place does not have the activity " << activity << std::endl;
    }

    std::cout << "Activity " << activity << " not found" << std::endl;

}

