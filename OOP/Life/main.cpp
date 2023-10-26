#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Board{
private:
    bool **board;
    const short mode;
    string name;
    char ruleBirth;
    char ruleSurvive;
    int size;
public:
    Board(): mode{0}{                                                                   //start game on default board
        return;
    }
    Board(const string path): mode{1}{                                                  //start game from file
        fillBoardFromFile(path);
        return;
    }
    Board(const string input_path, int iterations, const string output_path): mode{2}, ruleBirth{0}, ruleSurvive{0}, size{0}{  //offline from file
        return;
    }
    void fillBoardFromFile(const string path){
        ifstream in;
        in.open(path);
        string buf;
        for(int i = 0; i < 4; i++){
            getline(in, buf);
            if(not (buf[0] == '#' and buf[2] == ' ')){
//                cout << buf << ":" << buf.length() << endl;
                throw invalid_argument("Data isn't valid, in every string of file must be #mode (where mode is R / N / S / C)(Rules, Name, Size, Coordinates)\n"
                       "change data and try again");
//                i -= 1;
//                continue;
            }
            if(buf[1] == 'N'){
                if(this->name.length() == 0) {
                    name = buf.substr(3, this->name.length() - 3);
                    continue;
                } else throw invalid_argument("You are trying to set name second time, in doesn't allowed. Change your input file and try again.");
            }
            if(buf[1] == 'R'){
                if(ruleBirth != 0 or ruleSurvive != 0) {
                    this->ruleBirth = buf[4];
                    this->ruleSurvive = buf[7];
                    continue;
                } else throw invalid_argument("You are trying to set rules second time, in doesn't allowed. Change your input file and try again.");
            }
            if(buf[1] == 'S'){
                if(this->size != 0) {
                    this->size = stoi(buf.substr(3, buf.length() - 3));
                    this->board = (bool **) calloc(this->size, sizeof(bool*));
                    for(int j = 0; j < this->size; j++)
                        this->board[j] = (bool*) calloc(this->size, sizeof(bool));
                    continue;
                } else throw invalid_argument("You are trying to set size second time, in doesn't allowed. Change your input file and try again.");
            }
            if(buf[1] == 'C') {
                if (this->size > 0) {
                    int pointer = 2;
                    while(buf[pointer] != '\n' or buf[pointer] != '\000'){
                        if(buf[pointer] == ' '){
                            pointer += 1;
                            continue;
                        }
                        int x, y;
                        string cache;
                        int numPos = buf.find(',', pointer);
                        if(numPos == -1) break;
                        cache = buf.substr(pointer, numPos);
                        pointer += cache.length();
                        x = stoi(cache);
                        numPos = buf.find(' ', pointer);
                        if(numPos == -1) break;
                        cache = buf.substr(pointer, numPos);
                        pointer += cache.length();
                        y = stoi(cache);
                        if(x < 0 or y < 0 or x >= this->size or y >= this->size)
                            throw out_of_range("Coordinates of dots are out of board size range. Change your input file and try again.");
                        this->board[x][y] = true;
                    }
                } else throw invalid_argument("Firstly you must to set size of the board. Change your input file and try again.");
            }
        }
    }
    void printBoard(){
        for(int i = 0; i < this->size; i++){
//            cout << string(this->size + 2, '-') << '\n';
            for(int j = 0; j < this->size; j++) cout << ' ' << board[i][j];
            cout << " \n";
        }
    }
};

int main() {
    Board a("./zeromode.txt");
    a.printBoard();
//    cout << "work";
    return 0;
}
