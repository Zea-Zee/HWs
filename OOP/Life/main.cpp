#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Board{
private:
    bool **board;
    const short mode;
public:
    Board(): mode{0}{

    }
    Board(const string path): mode{1}{

    }
    Board(const string input_path, int iterations, const string output_path): mode{2}{

    }
    void fillBoardFromFile(const string path){
        ifstream in;
        in.open(path);
        
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
