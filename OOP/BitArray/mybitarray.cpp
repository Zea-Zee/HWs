#include <iostream>
#include <cmath>
#include <string>
#include "mybitarray.h"
#define UNSIGNED_INT 4294967295

using namespace std;

BitArray::BitArray(int num_bits, unsigned long value){
    if(num_bits < 0) throw length_error("Length must be not negative in constructor");
    this->bitLen = num_bits;
    if(num_bits == 0){
        this->array = (unsigned int*) calloc(0, 4);
        return;
    }
    int intLen = ceil((double) bitLen / 32);
    array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
    array[0] = value;
}
BitArray::BitArray(const BitArray& b){
    this->bitLen = b.bitLen;
    if(this->bitLen == 0){
        this->array = (unsigned  int*) calloc(0, 4);
        return;
    }
    int intLen = ceil((double) this->bitLen / 32);
    this->array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
    for(int i = 0; i < intLen; i++) this->array[i] = b.array[i];
}
BitArray::~BitArray(){
    if(this->bitLen == 0) return;
    free(this->array);
    this->bitLen == 0;
    this->array = nullptr;
}
void BitArray::swap(BitArray &b){           //works for BitArray with same sizes
//    if(this->bitLen == 0 and b.bitLen == 0) return;
//    if(ceil((double) this->bitLen / 32) != ceil((double) b.bitLen / 32)){
//        std::cerr << "BitArrays have different size!" << std::endl;
//        return;
//    }
    unsigned int *tmp = this->array;
    int tmpLen = this->bitLen;
    this->array = b.array;
    this->bitLen = b.bitLen;
    b.array = tmp;
    b.bitLen = tmpLen;
}
void BitArray::resize(int num_bits, bool value){
    if(num_bits < 0) throw invalid_argument("num_bits must be not negative in resize");
    int oldLen = this->bitLen;
    if(this->bitLen == 0)
        this->array = (unsigned int*) calloc((unsigned int) ceil((double) num_bits / 32), 4);
    else{
        auto *newArr= (unsigned int*) calloc((unsigned int) ceil((double) num_bits / 32), 4);
        for(int i = 0; i < (int) ceil((double) oldLen / 32); i++){
            unsigned int val = this->array[i];
            newArr[i] = this->array[i];
        }
        newArr[(int) oldLen / 32] = (this->array[(int) oldLen / 32] << (32 - oldLen)) >> (32 - oldLen);
        free(this->array);
        this->array = newArr;
    }
    this->bitLen = num_bits;
//    for(int i = ceil((double)num_bits / 32) * 32; i >= oldLen; i--) {
//        string tst = this->to_string();
//        this->reset(i, true);
//    }
    if(value)
        for(int i = num_bits - 1; i >= oldLen; i--)
            this->array[(int) i / 32] += (1 << (i % 32));
}
void BitArray::clear(){
    if(this->bitLen == 0){
        cout << "nothing to clear" << endl;
        return;
    }
    free(this->array);
    this->array = (unsigned int*) calloc(0, 4);
    this->bitLen = 0;
}
void BitArray::push_back(bool bit){
    if(this->bitLen % 32 == 0) {
        this->resize(this->bitLen + 1, false);
        this->bitLen -= 1;
    }
    unsigned int shift = (bit ? 1 : 0) << (this->bitLen);
    this->array[(int) this->bitLen / 32] += (shift);
    this->bitLen++;
}
BitArray& BitArray::operator=(const BitArray& b){
    if(this->bitLen == 0 and b.bitLen == 0) return *this;
    resize(b.bitLen, false);
    for(int i = 0; i < ceil((double) this->bitLen / 32); i++) this->array[i] = b.array[i];
    return *this;
}
BitArray& BitArray::operator&=(const BitArray& b){
    if(this->bitLen != b.bitLen) throw length_error("in bitwise operators both BitArray objects must have same length.");
    int intLen = ceil((double) this->bitLen / 32);
    for(int i = intLen - 1; i >= 0; i--){
        this->array[i] &= b.array[i];
    }
    return *this;
}
BitArray& BitArray::operator|=(const BitArray& b){
    if(this->bitLen != b.bitLen) throw length_error("in bitwise operators both BitArray objects must have same length.");
    int intLen = ceil((double) this->bitLen / 32);
    for(int i = intLen - 1; i >= 0; i--){
        this->array[i] |= b.array[i];
    }
    return *this;
}
BitArray& BitArray::operator^=(const BitArray& b){
    if(this->bitLen != b.bitLen) throw length_error("in bitwise operators both BitArray objects must have same length.");
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
BitArray& BitArray::operator<<=(int n){
    int intLen = ceil((double) this->bitLen / 32);
    for(int i = intLen - 1; i >= 0; i--){
        if(i == intLen - 1) this->array[i] = (this->array[i] << n);
        else{
            this->array[i + 1] += (this->array[i] >> (this->bitLen - n));
            this->array[i] = (this->array[i] << n);
        }
    }
    return *this;
}
BitArray& BitArray::operator>>=(int n){
    int intLen = ceil((double) this->bitLen / 32);
    for(int i = 0; i < intLen; i++){
        if(i == 0){
            this->array[i] = (this->array[i] >> n);
        }
        else{
            this->array[i - 1] += (this->array[i] << (this->bitLen - n));
            this->array[i] = (this->array[i] >> n);
        }
    }
    return *this;
}
BitArray BitArray::operator<<(int n) const{
    BitArray resBitArr(*this);
    int intLen = ceil((double) resBitArr.bitLen / 32);
    for(int i = intLen - 1; i >= 0; i--){
        if(i == intLen - 1){
            resBitArr.array[i] = (resBitArr.array[i] << n);
        }
        else{
            resBitArr.array[i + 1] += (resBitArr.array[i] >> (resBitArr.bitLen - n));
            resBitArr.array[i] = (resBitArr.array[i] << n);
        }
    }
    return resBitArr;
}
BitArray BitArray::operator>>(int n) const{
    BitArray resBitArr(*this);
    int intLen = ceil((double) resBitArr.bitLen / 32);
    for(int i = 0; i < intLen; i++){
        if(i == 0) resBitArr.array[i] = (resBitArr.array[i] >> n);
        else{
            resBitArr.array[i - 1] += (resBitArr.array[i] << (resBitArr.bitLen - n));
            resBitArr.array[i] = (resBitArr.array[i] >> n);
        }
    }
    return resBitArr;
}
BitArray& BitArray::set(int n, bool val){
    if(n < 0 or n >= this->bitLen) throw out_of_range("in set");
    int intIndex = (int) n / 32;
    n %= 32;
    unsigned int newInt = (this->array[intIndex] >> (n + 1)) << (n + 1);
    newInt += (val ? 1 : 0) << n;
    newInt += (this->array[intIndex] << (32 - n)) >> ((32 - n));
    this->array[intIndex] = newInt;
    return *this;
}
BitArray& BitArray::set(int n, bool val, bool isService){
    unsigned int nn =  (unsigned int) n;
    unsigned int intIndex = (int) nn / 32;
    n %= 32;
    string tst = this->to_string();
    unsigned int fstCopy = (unsigned int) ((unsigned int) this->array[intIndex] >> (nn + 1));
    unsigned int sndCopy = (unsigned int) ( (unsigned int) this->array[intIndex] << (32 - nn)) >> ((32 - nn));
    unsigned int newInt = fstCopy;
    newInt += sndCopy;
    this->array[intIndex] = newInt;
    return *this;
//    {
//        unsigned int intIndex = (int) n / 32;
//        n %= 32;
//        unsigned int fstCopy = 0;
//        fstCopy = (unsigned int) ((unsigned int) this->array[intIndex] >> n);
//        fstCopy = fstCopy >> 1;
//        fstCopy = fstCopy << 1;
//        fstCopy = fstCopy << n;
//        unsigned int sndCopy = 0;
//        sndCopy = (unsigned int) ((unsigned int) this->array[intIndex] << (32 - n - 1));
//        sndCopy = sndCopy << 1;
//        sndCopy = sndCopy >> 1;
//        sndCopy = sndCopy << (32 - n - 1);
//        unsigned int newInt = 0;
//        newInt += fstCopy;
//        newInt += sndCopy;
//        this->array[intIndex] = newInt;
//        return *this;
//    }
}
BitArray& BitArray::set(){
    int len = this->bitLen;
    this->clear();
    this->resize(len, true);
    return *this;
}
BitArray& BitArray::reset(int n){
    if(n < 0 or n >= this->bitLen) throw out_of_range("In reset");
    return this->set(n, false);
}
BitArray& BitArray::reset(int n, bool isService){
    return this->set(n, false, true);
}
BitArray& BitArray::reset(){
    int len = this->bitLen;
    this->clear();
    this->resize(len, false);
    return *this;
}
bool BitArray::any() const{
    for(int i = 0; i < ceil((double)this->bitLen / 32); i++) if(this->array[i] > 0) return true;
    return false;
}
bool BitArray::none() const{
    for(int i = 0; i < ceil((double)this->bitLen / 32); i++) if(this->array[i] > 0) return false;
    return true;
}
BitArray BitArray::operator~() const{
    int intLen = ceil((double) bitLen / 32);
    BitArray res(*this);
    for(int i = 0; i < intLen; i++)
        res.array[i] = ~res.array[i];
    return res;
}
int BitArray::count() const{
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
int BitArray::size() const{
//    return ceil((double) bitLen / 32);
    return this->bitLen;
}
bool BitArray::empty() const{
    return this->bitLen ==  0;
}
string BitArray::to_string() const{
    string res;
    if(this->bitLen == 0){
        cout << "bitarray is empty" << endl;
        return res;
    }
    int intLen = ceil((double) this->bitLen / 32);
    for(int i = this->bitLen % 32 - 1; i >= 0; i--) res += (((this->array[intLen - 1] >> (i)) & 1) ? '1' : '0');
    for(int i = (intLen - 1) * 32 - 1; i >= 0; i--) res += (((this->array[(int)i / 32] >> (i % 32)) & 1) ? '1' : '0');
    return res;
}
bool BitArray::operator==(const BitArray &b) const{
    if(this->bitLen != b.bitLen) return false;
    int intLen = ceil((double) bitLen / 32);
    for(int i = 0; i < intLen; i++) if(this->array[i] != b.array[i]) return false;
    return true;
}
bool BitArray::operator!=(const BitArray & b) const{
    if(this->bitLen != b.bitLen) return true;
    int intLen = ceil((double) bitLen / 32);
    for(int i = 0; i < intLen; i++) if(this->array[i] != b.array[i]) return true;
    return false;
}
BitArray BitArray::operator&(const BitArray& b2) const{
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
BitArray BitArray::operator|(const BitArray& b2) const{
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
BitArray BitArray::operator^(const BitArray& b2) const{
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
void BitArray::print(){
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
    }
    cout << endl;
}

bool BitArray::operator[](int n) const{
    if(n < 0 or n >= this->bitLen) throw out_of_range("in set");
    int intIndex = (int) n / 32;
    unsigned int aval = (this->array[intIndex] >> n) << n;
    unsigned int newInt = aval;
    unsigned int bval = (newInt << (31 - n));
    newInt = bval  >> ((31 - n));
    return newInt > 0;
}

BitArray::BitArray(): BitArray(0, 0){}
