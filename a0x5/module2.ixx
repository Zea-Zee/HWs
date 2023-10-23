export module module2;
//import <iostream>;
#include <iostream>

export namespace Module2
{
    std::string getMyName()
    {
        std::string name = "Name 2";
        return name;
    }
}
