#include <exception>

class InvalidInputException : public std::exception {
  public:
    const char* what() const noexcept override {
        return "Error: you must enter a valid integer value";
    }
};
