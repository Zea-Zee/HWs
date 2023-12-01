#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <ctime>

using namespace std;


class Sportsman{
public:
    Sportsman(int num) : number{num}, leader{nullptr}, led{nullptr}{};
    void changeLeader(Sportsman *newLeader){
        leader = newLeader;
    }
    void changeLed(Sportsman *newLed){
        led = newLed;
    }
    int getNumber(){
        return number;
    }
    Sportsman* getLed(){
        return led;
    }
private:
    Sportsman *leader;
    Sportsman *led;
    int number;
};

//template <typename T>
class Row{
private:
    unordered_map<int, Sportsman*> row;
    Sportsman *top;
    Sportsman *last;
public:
    Row() : top{nullptr}, last{nullptr}{};
    void addElem(int num, int lead){
        if(not row.size()){
            top = new Sportsman(num);
            last = top;
            row.insert(make_pair(num, top));
        } else if(row.count(lead)){
            Sportsman *leader = row.at(lead);
            Sportsman *man = new Sportsman(num);
            if(leader->getLed() != nullptr){
                man->changeLed(leader->getLed());
                leader->getLed()->changeLeader(man);
            } else{
                last = man;
            }
            leader->changeLed(man);
            man->changeLeader(leader);
            row.insert(make_pair(num, man));
        } else{
            Sportsman *man = new Sportsman(num);
            man->changeLed(top);
            top = man;
            row.insert(make_pair(num, man));
        }
    }
    Sportsman *getTop(){
        return top;
    }
};


int main(){
   ifstream in("./8inp.txt");
    // ifstream in("./gendata.txt");
    if(not in.is_open()){
        cerr << "Error opening file: ./8inp.txt.\n";
        return 1;
    }
    int n, num, leaderNum;
    in >> n;
    Row *row = new Row();

    auto start_time = std::chrono::high_resolution_clock::now();
    // time_t start = time.

    for(int i = n - 1; i >= 0; i--){
        in >> num >> leaderNum;
        row->addElem(num, leaderNum);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    // Output the duration in milliseconds
    // std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
    cout << "For " << n << " elements it took " << duration.count() << "ms\n";

   Sportsman *top = row->getTop();
   while(top != nullptr){
       cout << top->getNumber() << "\n";
       top = top->getLed();
   }
}