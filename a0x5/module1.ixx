export module module1;
//import <iostream>;
#include <iostream>

export namespace Module1{
    std::string getMyName()
    {
        std::string name = "Name 1";
        return name;
    }

}