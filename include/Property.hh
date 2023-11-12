#pragma once
#include <string>

using std::string;

class Property {
  public:
    string name;
    int quantity;
    bool repeated;

    Property(string name, int quantity, bool repeated);
    Property();

    void addQuantity();

};
