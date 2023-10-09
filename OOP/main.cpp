#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
//#include <cstring>
#define UNSIGNED_INT 4294967295

using namespace std;

class BitArray{
private:
    unsigned int *array;
    int bitLen;
public:
//    BitArray(int bLen = 0){
//        int intLen = ceil((double) bitLen / 31);
//        array = new int[intLen];
//    }
    explicit BitArray(int num_bits, unsigned long value = 0){
        this->bitLen = num_bits;
        if(num_bits == 0){
            this->array = (unsigned int*) malloc(0);
            return;
        }
        int intLen = ceil((double) bitLen / 32);
        array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
        array[0] = value;
    }
    BitArray(const BitArray& b){
        this->bitLen = b.bitLen;
        if(this->bitLen == 0){
            this->array = (unsigned  int*) malloc(0);
            return;
        }
        int intLen = ceil((double) this->bitLen / 32);
        this->array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
//        memcpy(this->array, b.array);
//        copy(begin(b.array), end(b.array), begin(this->array));
//        copy(b.array, *(*b.array + (ceil((double) (double) b.bitLen / 31)), this->array));
        for(int i = 0; i < intLen; i++) this->array[i] = b.array[i];
    }
    ~BitArray(){
        if(this->bitLen == 0) return;
        free(this->array);
    }
    void swap(BitArray &b){           //works for BitArray with same sizes
        if(this->bitLen == 0 and b.bitLen == 0) return;
        if(ceil((double) this->bitLen / 32) != ceil((double) b.bitLen / 32)){
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
            this->array = (unsigned int*) calloc((unsigned int) ceil((double) num_bits / 32), 4);
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
        else {
            unsigned int shift = (bit ? 1 : 0) << (this->bitLen);
            this->array[(int) this->bitLen / 32] += (shift);
            this->bitLen++;
        }
    }
    BitArray& operator=(const BitArray& b){
        if(this->bitLen == 0 and b.bitLen == 0) return *this;
        resize(b.bitLen, false);
        for(int i = 0; i < ceil((double) this->bitLen / 32); i++) this->array[i] = b.array[i];
        return *this;
    }
    BitArray& operator&=(const BitArray& b){
        if(this->bitLen != b.bitLen){
            cout << "arrays must be same size" << endl;
            return *this;
        }
        int intLen = ceil((double) this->bitLen / 32);
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
        int intLen = ceil((double) this->bitLen / 32);
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
        int intLen = ceil((double) this->bitLen / 32);
        for(int i = intLen - 1; i >= 0; i--){
            this->array[i] ^= b.array[i];
        }
        return *this;
    }
    BitArray& operator<<=(int n){
        int intLen = ceil((double) this->bitLen / 32);
        for(int i = intLen - 1; i >= 0; i--){
            if(i == intLen - 1){
//                unsigned int num = this->array[i];
//                unsigned int shift = (this->array[i] << n);
                this->array[i] = (this->array[i] << n);
//                this->print();
            }
            else{
                this->array[i + 1] += (this->array[i] >> (this->bitLen - n));
                this->array[i] = (this->array[i] << n);
            }
        }
        return *this;
    }
    BitArray& operator>>=(int n){
        int intLen = ceil((double) this->bitLen / 32);
        for(int i = 0; i < intLen; i++){
            if(i == 0){
//                unsigned int num = this->array[i];
//                unsigned int shift = (this->array[i] << n);
                this->array[i] = (this->array[i] >> n);
//                this->print();
            }
            else{
                this->array[i - 1] += (this->array[i] << (this->bitLen - n));
                this->array[i] = (this->array[i] >> n);
            }
        }
        return *this;
    }
    BitArray operator<<(int n) const{
        BitArray resBitArr(*this);
        int intLen = ceil((double) resBitArr.bitLen / 32);
        for(int i = intLen - 1; i >= 0; i--){
            if(i == intLen - 1){
//                unsigned int num = resBitArr.array[i];
//                unsigned int shift = (resBitArr.array[i] << n);
                resBitArr.array[i] = (resBitArr.array[i] << n);
//                resBitArr.print();
            }
            else{
                resBitArr.array[i + 1] += (resBitArr.array[i] >> (resBitArr.bitLen - n));
                resBitArr.array[i] = (resBitArr.array[i] << n);
            }
        }
        return resBitArr;
    }
    BitArray operator>>(int n) const{
        BitArray resBitArr(*this);
        int intLen = ceil((double) resBitArr.bitLen / 32);
        for(int i = 0; i < intLen; i++){
            if(i == 0){
//                unsigned int num = resBitArr.array[i];
//                unsigned int shift = (resBitArr.array[i] << n);
                resBitArr.array[i] = (resBitArr.array[i] >> n);
//                resBitArr.print();
            }
            else{
                resBitArr.array[i - 1] += (resBitArr.array[i] << (resBitArr.bitLen - n));
                resBitArr.array[i] = (resBitArr.array[i] >> n);
            }
        }
        return resBitArr;
    }
    BitArray& set(int n, bool val = true){
        if(n >= this->bitLen){
            cout << "index out of range in set" << endl;
            return *this;
        }
        int intIndex = (int) n / 32;
        unsigned int newInt = 0;//(this->array[intIndex] >> (n + 1)) << (n + 1);
        newInt += (val ? 1 : 0) << n;
        newInt += (this->array[intIndex] << (32 - n)) >> ((32 - n));
        this->array[intIndex] = newInt;
        return *this;
    }
    BitArray& set(){
        int len = this->bitLen;
        this->clear();
        this->resize(len, true);
        return *this;
    }
    BitArray& reset(int n){
        return this->set(n, false);
    }
    BitArray& reset(){
        int len = this->bitLen;
        this->clear();
        this->resize(len, false);
        return *this;
    }
    bool any() const{
        for(int i = 0; i < ceil((double)this->bitLen / 32); i++) if(this->array[i] > 0) return true;
        return false;
    }
    bool none() const{
        for(int i = 0; i < ceil((double)this->bitLen / 32); i++) if(this->array[i] > 0) return false;
        return true;
    }
    BitArray operator~() const{
        int intLen = ceil((double) bitLen / 32);
        for(int i = 0; i < intLen; i++) this->array[i] = ~this->array[i];
    }
    int count() const{
        int n = 0;
        unsigned int one = 1;
        int intLen = ceil((double) this->bitLen / 32);
        for(int i = 0; i < intLen; i++){
            for(int j = 0; j < 32; j++){
                n += ((one << j) & this->array[i]) >> j;
            }
        }
        return n;
    }
    int size() const{
        return ceil((double) bitLen / 32);
    }
    bool empty() const{
        return this->bitLen ==  0;
    }
    string to_string() const{
//        int intLen = ceil((double) bitLen / 32);
//        char* res = (char*) calloc(ceil((double)this->bitLen / 8), 1);
//        int spb = 0;
//        bool lastFlag = false;
//        for(int i = intLen - 1; i>= 0; i--){
//            for(int j = 31; j >= 0; j--){
//                if(i == intLen - 1 and lastFlag == false){
//                    lastFlag = true;
//                    j = this->bitLen % 32;
//                }
//                res[spb] = () this->array[i] >> j & 1;
//                sp++;
//            }
//        }
//        return res;
        string res = "";
        if(this->bitLen == 0){
            cout << "bitarray is empty" << endl;
            return res;
        }
        int intLen = ceil((double) this->bitLen / 32);
        for(int i = intLen * 32 - 1; i >= 0; i--) res += (((this->array[(int)i / 32] >> (i % 32)) & 1) ? '1' : '0');
        return res;
    }
    bool operator==(const BitArray &b) const{
        if(this->bitLen != b.bitLen) return false;
        int intLen = ceil((double) bitLen / 32);
        for(int i = 0; i < intLen; i++) if(this->array[i] != b.array[i]) return false;
        return true;
    }
    bool operator!=(const BitArray & b) const{
        if(this->bitLen != b.bitLen) return true;
        int intLen = ceil((double) bitLen / 32);
        for(int i = 0; i < intLen; i++) if(this->array[i] != b.array[i]) return true;
        return false;
    }
    BitArray operator&(const BitArray& b2) const{
        BitArray new1(*this);
        BitArray new2(b2);
        if(new1.empty()) return new1;
        if(new2.empty()) return new2;
        if(new1.bitLen < new2.bitLen) new1.resize(new2.bitLen, false);
        else if(new1.bitLen > new2.bitLen) new2.resize(new1.bitLen, false);
        int intLen = ceil((double) bitLen / 32);
        for(int i = 0; i < intLen; i++) new1.array[i] &= new2.array[i];
        return new1;
    }
    BitArray operator|(const BitArray& b2) const{
        BitArray new1(*this);
        BitArray new2(b2);
        if(new1.empty()) return new1;
        if(new2.empty()) return new2;
        if(new1.bitLen < new2.bitLen) new1.resize(new2.bitLen, false);
        else if(new1.bitLen > new2.bitLen) new2.resize(new1.bitLen, false);
        int intLen = ceil((double) bitLen / 32);
        for(int i = 0; i < intLen; i++)
            new1.array[i] |= new2.array[i];
        return new1;
    }
    BitArray operator^(const BitArray& b2) const{
        BitArray new1(*this);
        BitArray new2(b2);
        if(new1.empty()) return new1;
        if(new2.empty()) return new2;
        if(new1.bitLen < new2.bitLen) new1.resize(new2.bitLen, false);
        else if(new1.bitLen > new2.bitLen) new2.resize(new1.bitLen, false);
        int intLen = ceil((double) bitLen / 32);
        for(int i = 0; i < intLen; i++) new1.array[i] ^= new2.array[i];
        return new1;
    }
    void print(){
        if(this->bitLen == 0){
            cout << "bitarray is empty" << endl;
            return;
        }
        cout << "bitLen is " << this->bitLen << ", intLen is " << ceil((double) this->bitLen / 32) << " representations of every 4 byte block is: " << endl;
        int intLen = ceil((double) this->bitLen / 32);
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



//    BitArray bitA(0, 0);
//    bitA.print();
//    bitA.push_back(true);
//    bitA.print();
//    bitA.push_back(false);
//    bitA.print();
//    bitA.push_back(true);
//    bitA.print();
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.print();
//    bitA.resize(64, true);
//    bitA.print();
//    bitA <<= 16;
//    bitA.print();



//    BitArray bitA(8, 255);
//    bitA.resize(8, 1)
//    bitA.print();
//    bitA.set(4, 0);
//    bitA.print();
//    bitA.push_back(true);
//    bitA.print();
//    bitA.push_back(false);
//    bitA.print();
//    bitA.push_back(true);
//    bitA.print();
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.push_back(true);
//    bitA.push_back(false);
//    bitA.print();
//    bitA.resize(64, true);
//    bitA.print();
//    (bitA >> 16).print();
//    bitA >>= 16;
//    bitA.print();

//    BitArray bitA(32, 0);
//    bitA.print();
//    bitA.set();
//    bitA.print();

//    BitArray bitA(32, 4294967295);
//    BitArray bitA(32, 4294967295);
//    bitA.print();
//    bitA.reset();
//    bitA.print();


//    BitArray bitA(64, UNSIGNED_INT);
//    bitA.print();
//    cout << bitA.count() << endl;
//    return 0;


//    BitArray bitA(8, 49);
//    bitA.print();
//    cout << bitA.to_string() << endl;

    BitArray bitA(80, 128);
    bitA.print();
    BitArray bitB(8, 127);
    bitB.print();
    cout << (bitA & bitB).to_string() << endl;
//    cout << bitA.to_string() << endl;


    return 0;
}