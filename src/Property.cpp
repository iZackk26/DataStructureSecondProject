#include <Property.hh>
#include <string>

using std::string;

Property::Property(string name, int quantity, bool repeated) {
  this->name = name;
  this->quantity = quantity;
}

Property::Property() {
  this->name = "";
  this->quantity = 0;
}

void Property::addQuantity() {
  this->quantity++;
}
