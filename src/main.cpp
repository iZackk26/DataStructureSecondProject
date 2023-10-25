#include <Person.hh>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

void writeToFile(Person person, string fileName){
    //This function opens a file and append information to it in binary mode

    std::ofstream file(fileName, std::ios::out | std::ios::app | std::ios::binary);
    try {
        if (!file.is_open()) {
            throw std::runtime_error("File not found");
        }

        file.write(reinterpret_cast<char *>(&person), sizeof(person));
        file.close();

        if (!file.good()) {
            throw std::runtime_error("Error occurred at writing time");
        }
    } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
}

void readFromFile(const string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    try {
        if (!file) {
            throw std::runtime_error("File not found");
        }

        Person person;
        file.read(reinterpret_cast<char *>(&person), sizeof(person));

        while (!file.eof()) {
            std::cout << person << std::endl;
            file.read(reinterpret_cast<char *>(&person), sizeof(person));
        }

        file.close();
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
}

int main(){
    string dataFile = "Information/Data.bin";
    string gender = "Male";
    int age = 18;
    string beginingRute = "San Ramon";
    string endingRute = "Santa Clara";
    string hobby = "Patear la redonda";
    Person person(gender.c_str(), age, beginingRute.c_str(), endingRute.c_str(), hobby.c_str());
    writeToFile(person, dataFile);
    readFromFile(dataFile);

    return 0;
}
