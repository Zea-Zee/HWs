module;

#include <iostream>

export module module1;

export void tst(){
    std::cout << "tst";
}

export namespace Module1{
    void getMyName(){
        std::cout << "1\n";
    }
}