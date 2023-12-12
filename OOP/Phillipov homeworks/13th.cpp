#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};


template <typename T>
class RealMatrix{
private:
    unordered_map<int, unordered_map<int, T>*> *table;
public:
    RealMatrix() {
        table = new unordered_map<int, unordered_map<int, T>*>;
        cout << "Infinity matrix have created.\n";
    }

    T operator()(int y, int x) {
        cout << "Getting cell (" << y << "; " << x << ") from RealMatrix.\n";
        auto Xes = table->at(y);
        T res = (*Xes).at(x);
        return res;
    }
    void setCell(int y, int x, T val){
        cout << "Setting cell (" << y << "; " << x << ") in real matrix.\n";
//        cout << ":" << table->size() << ":\n";
        auto outerIt = table->find(y);
//        cout << "bebra it is " << outerIt->second << "\n";
        if(outerIt == table->end())
            (*table)[y] = new unordered_map<int, T>;
        (*table)[y]->insert(make_pair(x, val));
    }
};

template <typename T, T defaultValue>
class MatrixProxy {
private:
    RealMatrix<T> *realMatrix;
    int sizeVar;
    unordered_map<int, unordered_set<int>*> cells;
    T def;
//    int y, x;
public:
    MatrixProxy() : def{defaultValue}, sizeVar{0}, realMatrix{nullptr} {};

//    T operator()(int y, int x) override {
//        if (size == 0) return def;
//        else return (*realMatrix)(y, x);
//    };

//    MatrixProxy& operator()(int a, int b) {
//        y = a;
//        x = b;
//        return *this;
//    }

    void operator()(int a, int b, T c) {
        auto outerIt = cells.find(a);
        if(outerIt == cells.end())
            outerIt = cells.emplace(a, new unordered_set<int>).first;
        if(outerIt->second->find(b) == outerIt->second->end())
            sizeVar++;
        outerIt->second->insert(b);
        if(realMatrix == nullptr)
            realMatrix = new RealMatrix<T>;
        realMatrix->setCell(a, b, c);
    }

    T operator()(int a, int b) const {
        auto outerIt = cells.find(a);
        if(outerIt == cells.end())
            return def;
        if(outerIt->second->find(b) == outerIt->second->end())
            return def;
        return (*realMatrix)(a, b);
    }

    int size(){
        return sizeVar;
    }

//    MatrixProxy& operator=(int c) {
//        auto outerIt = cells.find(y);
//        if(outerIt == cells.end())
//            outerIt = cells.emplace(y, new unordered_set<int>).first;
//        if(outerIt->second->find(x) == outerIt->second->end())
//            size++;
//        outerIt->second->insert(x);
//        return *this;
//    }
};

int main() {
    MatrixProxy<int, -1> matrix;
    cout << (matrix)(1000000000, 2000000000) << "\n";
    cout << matrix(0, 0) << "\n";
    cout << matrix.size() << "\n";
    matrix(2000000000, 2000000000, 10);
    cout << matrix(2000000000, 2000000000) << "\n";


    MatrixProxy<char, '_'> c_matrix;
    cout << (c_matrix)(1000000000, 2000000000) << "\n";
    cout << c_matrix(0, 0) << "\n";
    cout << c_matrix.size() << "\n";
    c_matrix(2000000000, 2000000000, 'b');
    cout << c_matrix(2000000000, 2000000000) << "\n";


    MatrixProxy<double, 10> d_matrix;
    cout << (d_matrix)(1000000000, 2000000000) << "\n";
    cout << d_matrix(0, 0) << "\n";
    cout << d_matrix.size() << "\n";
    d_matrix(2000000000, 2000000000, 10);
    cout << d_matrix(2000000000, 2000000000) << "\n";
}