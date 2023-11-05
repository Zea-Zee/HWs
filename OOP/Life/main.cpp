#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>

#ifdef WINDOWS
    #include<windows.h>
    #define pause(a) Sleep(a)

    #define ALIVE   '1'
    #define DEAD    '0'

    #define RESET   " "
    #define RED     " "
    #define GREEN   " "
    #define YELLOW  " "
    #define BLUE    " "
#else
    #include<unistd.h>
    #define pause(a) usleep(a * 1000)

    #define ALIVE   "■"
    #define DEAD    "□"

    #define RESET "\033[0m"
    #define RED "\033[31m"
    #define GREEN "\033[32m"
    #define YELLOW "\033[33m"
    #define BLUE "\033[34m"
#endif

void clear_screen(){
    #ifdef WINDOWS
        std::system("cls");
    #else
        // Assume POSIX
        std::system ("clear");
    #endif
}

using namespace std;

class MyError{
protected:
    const char *msg;
//    int exitNumber;
public:
    MyError(const char *str){
        this->msg = str;
//        this->exitNumber = exitNum;
    }
    virtual void throwException() const{
        perror("Unknown error.\n");
        exit(-1);
    }
};

class ArgumentError : public MyError{
public:
    using MyError::MyError;
//    explicit MyFatalError(const string str) : MyError(str){}
    void throwException() const override{
        throw invalid_argument(this->msg);
    }
};

class RangeError : public MyError{
public:
    using MyError::MyError;
//    explicit MyFatalError(const string str) : MyError(str){}
    void throwException() const override{
        throw out_of_range(this->msg);
    }
};

class MyPlainError : public MyError{
public:
    using MyError::MyError;
//    explicit MyFatalError(const string str) : MyError(str){}
    void throwException() const override{
        perror(this->msg);
    }
};

class Board{
private:
    bool **board;
    bool mode;
    string name;
    vector<short> birthRule;
    vector<short> surviveRule;
    int size;
    int alive;  //number of alive cells to corr sleep time with it
    int mi(int i){  //make correct index from -1 size and other
        if(i < 0) return i + this->size;
        if(i == this->size) return 0;
        return i;
    }
public:
    Board(){    //start game on default board
        srand(time(0));
        int defPath = rand() % 3;
        if(defPath == 0) Board("./triplet.txt");
        else if(defPath == 1) Board("./crystal.txt");
        else if(defPath == 2) Board("./gun.txt");
    }
    Board(const string path): mode{true}, size{0},alive{0}{                                                  //start game from file
        fillBoardFromFile(path);
        cout << "You've filled board with this shape:\n";
        askForAction();
        return;
    }
    Board(const string input_path, int iterations, const string output_path): mode{false}, size{0},alive{0}{  //offline from file
        fillBoardFromFile(input_path);
        for(int i = 0; i < iterations; i++) tic();
        ofstream out;
        out.open(output_path);
        printBoard(&out);
    }
    void fillBoardFromFile(const string path){
        ifstream in;
        in.open(path);
        string buf;
        for(int i = 0; i < 4; i++){
            getline(in, buf);
//            cout << "fillBuff buf:" << buf << ":\n";
            if(not (buf[0] == '#' and buf[2] == ' ')){
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
                if(birthRule.empty() and surviveRule.empty()) {
                    int startPos = buf.find('B', 0) + 1;
                    int slashPos = buf.find('/', 0);
                    if(startPos == -1 or slashPos == startPos) {
                        ArgumentError aerr(
                                "Your rules are wrong, enter them by example (#R Bx/Sy) (where x is number of neighbotr to birth"
                                "and y is number of neighbors to survive");
                        aerr.throwException();
                        throw invalid_argument(
                                "Your rules are wrong, enter them by example (#R Bx/Sy) (where x is number of neighbotr to birth"
                                "and y is number of neighbors to survive");
                    }
                    for(int j = 0; j < (slashPos - startPos); j++){
                        string tmp = buf.substr(startPos + j, 1);
                        birthRule.push_back(stoi(tmp));
                    }
                    startPos = buf.find('S', slashPos) + 1;
                    slashPos = buf.length();
                    if(startPos == -1 or slashPos == startPos) {
                        ArgumentError aerr(
                                "Your rules are wrong, enter them by example (#R Bx/Sy) (where x is number of neighbotr to birth"
                                "and y is number of neighbors to survive");
                        aerr.throwException();
//                        throw invalid_argument(
//                                "Your rules are wrong, enter them by example (#R Bx/Sy) (where x is number of neighbotr to birth"
//                                "and y is number of neighbors to survive");
                    }
                    for(int j = 0; j < (slashPos - startPos); j++){
                        surviveRule.push_back(stoi(buf.substr(startPos + j, 1)));
                    }
                    continue;
                } else{
                    ArgumentError aerr("You are trying to set rules second time, in doesn't allowed. Change your input file and try again.");
                    aerr.throwException();
//                    throw invalid_argument("You are trying to set rules second time, in doesn't allowed. Change your input file and try again.");
                }
            }
            if(buf[1] == 'S'){
//                cout << "\n" << ":" << this->size << ":\n";
                if(this->size == 0) {
                    this->size = stoi(buf.substr(3, buf.length() - 3));
                    this->board = (bool **) calloc(this->size, sizeof(bool*));
                    for(int j = 0; j < this->size; j++)
                        this->board[j] = (bool*) calloc(this->size, sizeof(bool));
                    continue;
                } else{
                    ArgumentError aerr("You are trying to set size second time, in doesn't allowed. Change your input file and try again.");
                    aerr.throwException();
//                    throw invalid_argument("You are trying to set size second time, in doesn't allowed. Change your input file and try again.");
                }
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
                        cache = buf.substr(pointer, numPos - pointer);
                        pointer += cache.length() + 1;
                        x = stoi(cache);
                        numPos = buf.find(' ', pointer);
                        if(numPos == -1) numPos = buf.length();
                        cache = buf.substr(pointer, numPos - pointer);
                        pointer += cache.length();
                        y = stoi(cache);
                        if(x < 0 or y < 0 or x >= this->size or y >= this->size){
                            RangeError rerr("Coordinates of dots are out of board size range. Change your input file and try again.");
                            rerr.throwException();
                        }
//                            throw out_of_range("Coordinates of dots are out of board size range. Change your input file and try again.");
                        this->board[x][y] = true;
                    }
                } else{
                    RangeError rerr("Firstly you must to set size of the board. Change your input file and try again.");
                    rerr.throwException();
//                    throw invalid_argument("Firstly you must to set size of the board. Change your input file and try again.");
                }
            }
        }
        if(this->mode){
            cout << "You've entered universal with name:\n  " << BLUE << this->name << RESET << "\n";
            cout << "Size:\n  " << BLUE << this->size << RESET << "\n";
            cout << "Birth rules:\n  " << BLUE;
            for(int j = 0; j < this->birthRule.size(); j++) cout << " " << this->birthRule[j];
            cout << RESET << "\n";
            cout << "Survival rules:\n  " << BLUE;
            for(int j = 0; j < this->surviveRule.size(); j++) cout << " " << this->surviveRule[j];
            cout << RESET << "\n";
            cout << "Mode:\n  " << BLUE << (this->mode == 0 ? "Random board -> game" : (this->mode == 1 ? "Your board from file -> game" :
                                                                                        "Offline mode")) << "\n" << RESET;
            cout << YELLOW << "Field is:\n" << RESET;
            printBoard();
        }
    }
    void askForAction(){
        while(true){
            cout << "Enter command below:\n";
            string buf;
            getline(cin, buf);
//            if(buf[0] == '\n') buf = buf.substr(1, buf.length() - 1);
            stringstream ss(buf);
            string buf2;
            getline(ss, buf2, ' ');
            cout << "Command is:" << buf << endl;
//        cin >> action;
            if(buf == "help"){
                cout << "dump <file.txt> - write current board's state in your file\n"
                        "tic <n> - make n tics, if you type just tic, then will made 1 tic\n"
                        "help - show all actions (it will show this text)\n"
                        "exit - end game\n";
                continue;
            }
            if(buf2 == "dump"){
                getline(ss, buf2, ' ');
                ofstream out;
                try{
                    out.open(buf2, std::ios::out);
                } catch (const char* err){
                    cout << "An error ocurred with opening the file : "
                         << buf2 << " type action dump with filename again\n";
                    continue;
                }
                printBoard(&out);
                cout << "Dump was created.\n";
                out.close();
                continue;
            }
//            getline(ss, buf)
            if(buf == "exit"){
                cout << YELLOW << "You've exit the game.\n" << RESET;
                exit(0);
            }
            if(buf == "tic"){
                cout << "TIC once\n";
                tic();
                continue;
            }
            if(buf2 == "tic") {
                buf2 = "";
                getline(ss, buf2);
                if(buf2.length() > 0){
                    int n;
                    try {
                        n = stoi(buf2);
                    } catch (const char* err){
//                        perror("After tic you must enter integer or nothing!!! Try it again\n");
                        MyPlainError perr("After tic you must enter integer or nothing!!! Try it again.\\n");
                        perr.throwException();
                        continue;
                    }
                    cout << "TIC " << n << " times\n";
                    for(int j = 0; j < n; j++){
                        tic();
                        cout << string(size * 2 + 2, '-') << '\n';
                        int sleepTime = this->alive < 10 ? 50 : this->alive * 7;
                        pause(size * 20);
                        clear_screen();
                    }
                }
                continue;
            }
//            perror("You must type one command from help list!!!\n");
            MyPlainError perr("You must type one command from help list!!!\n");
            perr.throwException();
            continue;
        }
    }
    void tic(){
        //check for birth
        int curAlive = 0;
        bool **newBoard = (bool**) calloc(this->size, sizeof(bool*));
        for(int i = 0; i < this->size; i++){
            newBoard[i] = (bool*) calloc(this->size, sizeof(bool));
            for(int j = 0; j < this->size; j++){
                int aliveNeighbors = 0;
                aliveNeighbors += board[mi(i + 1)][mi(j - 1)];        //***
                aliveNeighbors += board[mi(i + 1)][j];                  //---
                aliveNeighbors += board[mi(i + 1)][mi(j + 1)];       //---
                aliveNeighbors += board[i][mi(j - 1)];                 //---
                aliveNeighbors += board[i][mi(j + 1)];                 //*-*
                                                                         //---
                aliveNeighbors += board[mi(i - 1)][mi(j - 1)];      //---
                aliveNeighbors += board[mi(i - 1)][j];                //---
                aliveNeighbors += board[mi(i - 1)][mi(j + 1)];     //***
                if(board[i][j]){
                    for(auto el: this->surviveRule){
                        if(el == aliveNeighbors){
                            newBoard[i][j] = true;
                            curAlive++;
                            break;
                        }
                    }
                } else{
                    for(auto el: this->birthRule){
                        if(el == aliveNeighbors){
                            newBoard[i][j] = true;
                            curAlive++;
                            break;
                        }
                    }
                }
            }
        }
        for(int i = 0; i < this->size; i++){
            free(board[i]);
        }
        free(board);
        this->board = newBoard;
        this->alive = curAlive;
        if(this->mode) printBoard();
    }

    void printBoard(ofstream *stream){
//        string outputBuf;
        for(int i = 0; i < this->size; i++){
//            outputBuf += (string(2 * this->size, '-'));
//            *stream << string(2 * size, '-');
            for(int j = 0; j < this->size; j++) {
//                outputBuf.push_back(' ');
//                outputBuf.push_back((char) board[i][j]);
                *stream << ' ' << board[i][j];
            }
//            outputBuf.push_back('\n');
            *stream << '\n';
        }
//        *stream << outputBuf;
    }
    void printBoard(){
        for(int i = 0; i < this->size; i++){
//            cout << string(this->size + 2, '-') << '\n';
            for(int j = 0; j < this->size; j++){
                if(j == 0){
                    cout << "|";
                }
                if(board[i][j])
                    cout << ' ' << GREEN << ALIVE << RESET;
                else {
                    cout << ' ' << RED << DEAD << RESET;
//                    cout << ' ' << RED << " " << RESET;
                }
            }
            cout << "|";
            cout << " \n";
        }
    }
};

int main(int argc, char *argv[]){
    if(argc >= 3){
        int iters;
        string path;
        bool iterFlag = false, outputFlag = false;
        for(int i = 1; i < argc; i++){
            string arg = argv[i];
            if(arg == "-i"){
                if(i >= argc){
//                    perror("After -i you must type number of iterations.\n");
                    ArgumentError aerr("After -i you must type number of iterations.\n");
                    aerr.throwException();
                }
                if(iterFlag){
                    perror("You are trying two set number of iterations second time.\n");
                    ArgumentError aerr("You are trying two set number of iterations second time.\n");
                    aerr.throwException();
                }
                try{
                    iters = stoi(argv[i + 1]);
                } catch(const char* err){
//                    perror("Iterations must be integer.\n");
                    ArgumentError aerr("Iterations must be integer.\n");
                    aerr.throwException();
//                    exit(1);
                }
                iterFlag = true;
                i++;
            } else if(arg.find("--iterations=") == 0){
                try{
                    iters = stoi(arg.substr(13));
                } catch(const char* err){
//                    perror("Iterations must be integer.\n");
//                    exit(1);
                    ArgumentError aerr("After -i you must type number of iterations.\n");
                    aerr.throwException();
                }
            } else if(arg == "-o"){
                if(i >= argc){
//                    perror("After -o you must type output path.\n");
//                    exit(1);
                    ArgumentError aerr("After -o you must type output path.\n");
                    aerr.throwException();
                }
                if(outputFlag){
//                    perror("You are trying two set output path second time.\n");
//                    exit(1);
                    ArgumentError aerr("You are trying two set output path second time.\n");
                    aerr.throwException();
                }
                path = argv[i + 1];
                outputFlag = true;
                i++;
            } else if(arg.find("--output=") == 0){
                path = arg.substr(9);
            }
        }
        Board a("./triplet.txt", iters, path);
        exit(0);
    }
    Board a;
    return 0;
}
