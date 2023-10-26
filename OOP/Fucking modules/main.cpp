#include <iostream>
import module1;
import module2;

int main(int argc, char** argv)
{
    Module1::getMyName();
    Module2::getMyName();

    using namespace Module1;
    getMyName(); // (A)
    Module2::getMyName();
//
//    //using namespace Module2; // (B)
//    //std::cout << getMyName() << "\n"; // COMPILATION ERROR (C)
//
//    using Module2::getMyName;
//    std::cout << getMyName() << "\n"; // (D)

}
