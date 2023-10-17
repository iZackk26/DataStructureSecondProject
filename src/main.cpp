#include <Person.hh>
#include <list>
#include <iostream>
#include <fstream>


void writeToFile(Person x, string fileName){
    std::ofstream file(fileName, std::ios::binary | std::ios::app);
    try {
        if (!file.is_open()) {
            throw std::runtime_error("File not found");
        }
        /* file.write(reinterpret_cast<char *>(&x), sizeof(x)); */
        file.write((char *)&x, sizeof(x));
        file.close();

        if (!file.good()) {
            throw std::runtime_error("Error occurred at writing time");
        }
    } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
}

void readFile(string filename){
    std::ifstream file(filename, std::ios::out | std::ios::binary);
    try {
        Person p;
        if (!file) {
            throw std::runtime_error("File not found");
        }
        file.read((char *)&p, sizeof(p));
        file.close();
        if (!file.good()) {
            throw std::runtime_error("Error occurred at reading time");
        }
    } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }
}


int main(){
    string dataFile = "Information/Data.bin";
    Person p("Male",18, "San Ramon", "Santa Clara");
    Person p2("Female", 19, "Heredia", "Alajuela");
    /* writeToFile(p, dataFile); */
    writeToFile(p2, dataFile);
    readFile(dataFile);

    return 0;
}
