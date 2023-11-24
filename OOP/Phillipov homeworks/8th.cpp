#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

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
            last->changeLed(man);
            man->changeLeader(last);
            last = man;
            row.insert(make_pair(num, man));
        }
    }
    Sportsman *getTop(){
        return top;
    }
};


int main(){
    ifstream in("./8inp.txt");
    if(not in.is_open()){
        cerr << "Error opening file: ./8inp.txt.\n";
        return 1;
    }
    int n, num, leaderNum;
    in >> n;
    Row *row = new Row();
    for(int i = 0; i < n; i++){
        in >> num >> leaderNum;
        row->addElem(num, leaderNum);
    }
    Sportsman *top = row->getTop();
    while(top != nullptr){
        cout << top->getNumber() << "\n";
        top = top->getLed();
    }
}