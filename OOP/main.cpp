#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <vector>
//#include <cstring>

using namespace std;

class BitArray{
private:
    unsigned int *array;
    int bitLen;
public:
//    BitArray(int bLen = 0){
//        int intLen = ceil(bitLen / 31);
//        array = new int[intLen];
//    }
    explicit BitArray(int num_bits, unsigned long value = 0){
        this->bitLen = num_bits;
        if(num_bits == 0){
            this->array = (unsigned int*) malloc(0);
            return;
        }
        int intLen = (int) (bitLen / 32) + 1;
        array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
        array[0] = value;
    }
    BitArray(const BitArray& b){
        this->bitLen = b.bitLen;
        if(this->bitLen == 0){
            this->array = (unsigned  int*) malloc(0);
            return;
        }
        int intLen = (int) (bitLen / 32) + 1;
        this->array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
//        memcpy(this->array, b.array);
//        copy(begin(b.array), end(b.array), begin(this->array));
//        copy(b.array, *(*b.array + (ceil((double) b.bitLen / 31)), this->array));
        for(int i = 0; i < intLen; i++) this->array[i] = b.array[i];
    }
    ~BitArray(){
        if(this->bitLen == 0) return;
        free(this->array);
    }
    void swap(BitArray &b){           //works for BitArray with same sizes
        if(this->bitLen == 0 and b.bitLen == 0) return;
        if((int) (this->bitLen / 32) + 1 != (int) (b.bitLen / 32) + 1){
            std::cerr << "BitArrays have different size!" << std::endl;
            return;
        }
        unsigned int *tmp = this->array;
        unsigned int tmpLen = this->bitLen;
        this->array = b.array;
        b.array = tmp;
        this->bitLen = b.bitLen;
        b.bitLen = tmpLen;
    }
    void resize(int num_bits, bool value=false){
//        this->bitLen = num_bits;
//        int newLen = (unsigned int) (num_bits / 8) + 1;
        if(this->bitLen == 0)
            this->array = (unsigned int*) calloc((unsigned int) (num_bits / 32) + 1, 4);
        else
            this->array = (unsigned int*) realloc(this->array, (unsigned int) (num_bits / 8) + 1);
//        cout << this->array[(int) (num_bits / 32) + 1]
        if(value){
            for(int i = num_bits - 1; i >= this->bitLen; i--){
//            cout << (1 << i) << " " << this->array[(int) (i / 32)] << " " << (this->array[(int) (i / 32)] + (1 << i)) << endl;
                this->array[(int) (i / 32)] += (1 << i);
            }
        }
        this->bitLen = num_bits;
    }
    void clear(){
        if(this->bitLen == 0){
            cout << "nothing to clear" << endl;
            return;
        }
//        memset(this->array, 0, sizeof(unsigned int) * ((int) this->bitLen / 32 + 1));
        free(this->array);
        this->array = (unsigned int*) malloc(0);
        this->bitLen = 0;
    }
    void push_back(bool bit){
        if(this->bitLen % 32 == 0)
            this->resize(this->bitLen + 1, bit);
        else
            this->bitLen++;
        this->array[(int) this->bitLen / 32] += (1 << (this->bitLen - 1));
    }
    BitArray& operator=(const BitArray& b){
        if(this->bitLen == 0 and b.bitLen == 0) return *this;
        resize(b.bitLen, false);
        for(int i = 0; i < (int) this->bitLen / 32 + 1; i++) this->array[i] = b.array[i];
        return *this;
    }
    BitArray& operator&=(const BitArray& b){
        if(this->bitLen != b.bitLen){
            cout << "arrays must be same size" << endl;
            return *this;
        }
        int intLen = (int) (this->bitLen / 32) + 1;
        for(int i = intLen - 1; i >= 0; i--){
            this->array[i] &= b.array[i];
        }
        return *this;
    }
    BitArray& operator|=(const BitArray& b){
        if(this->bitLen != b.bitLen){
            cout << "arrays must be same size" << endl;
            return *this;
        }
        int intLen = (int) (this->bitLen / 32) + 1;
        for(int i = intLen - 1; i >= 0; i--){
            this->array[i] |= b.array[i];
        }
        return *this;
    }
    BitArray& operator^=(const BitArray& b){
        if(this->bitLen != b.bitLen){
            cout << "arrays must be same size" << endl;
            return *this;
        }
        int intLen = (int) (this->bitLen / 32) + 1;
        for(int i = intLen - 1; i >= 0; i--){
            this->array[i] ^= b.array[i];
        }
        return *this;
    }
    BitArray& operator<<=(int n){
        int intLen = (int) (this->bitLen / 32) + 1;
        for(int i = intLen - 1; i >= 0; i--){
            if(i == intLen - 1) this->array[i] = (this->array[i] << n);
            else{
                this->array[i + 1] = (this->array[i] >> (this->bitLen - n));
                this->array[i] = (this->array[i] << n);
            }
        }
        return *this;
    }
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;
    void print(){
        if(this->bitLen == 0){
            cout << "bitarray is empty" << endl;
            return;
        }
        cout << "bitLen is " << this->bitLen << ", intLen is " << (int) this->bitLen / 32 + 1 << " representations of every 4 byte block is: " << endl;
        int intLen = (int) (this->bitLen / 32) + 1;
        for (int i = intLen - 1; i >= 0; i--){
            cout << i << "th block is " << this->array[i] << " ";
        }
        cout << endl;
        for(int i = intLen * 32 - 1; i >= 0; i--){
            if(i == this->bitLen - 1) cout << "|";
            if((i - intLen * 32 + 1) % 8 == 0 and i < intLen * 32 - 1) cout << " ";
            if((i - intLen * 32 + 1) % 32 == 0 and i < intLen * 32 - 1) cout << "_ ";
            cout << ((this->array[(int)i / 32] >> (i % 32)) & 1);
//            cout << ((array[0] >> i) & 1);
//            cout << ((array[(int) bitLen / 8] >> (i % 8)) & 1);
        }
        cout << endl;
    }
};

int main(){
//    BitArray bitA(4, false);
//    bitA.print();
//    bitA.resize(80, true);
//    bitA.print();
//    bitA.clear();
//    bitA.print();
//    bitA.resize(3, true);
//    bitA.print();
//    bitA.push_back(true);
//    bitA.print();
//
//    BitArray bitB(4, 5);
//    cout << "A ";
//    bitA.print();
//    cout << "B ";
//    bitB.print();
//    bitA &= bitB;
//    cout << "new A ";
//    bitA.print();
//    cout << "new B ";
//    bitB.print();
//    bitB.resize(0, true);
//    bitB.resize(2, true);
//    bitA = bitB;
//    bitA.print();
    BitArray bitA(0, 0);
    bitA.print();
    bitA.push_back(true);
    bitA.print();
    bitA.push_back(false);
    bitA.print();
    bitA.push_back(true);
    bitA.print();
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.push_back(true);
    bitA.push_back(false);
    bitA.print();
//    bitA.resize(64, true);
//    bitA.print();
//    bitA <<= 10;
//    bitA.print();
    return 0;
}