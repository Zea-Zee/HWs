#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

class BitArray{
private:
    unsigned int *array;
    unsigned int bitLen;
public:
//    BitArray(int bLen = 0){
//        int intLen = ceil(bitLen / 31);
//        array = new int[intLen];
//    }
    explicit BitArray(int num_bits, unsigned long value = 0){
        this->bitLen = num_bits;
        int intLen = (int) (bitLen / 32) + 1;
        array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
        array[0] = value;
    }
    BitArray(const BitArray& b){
        bitLen = b.bitLen;
        int intLen = (int) (bitLen / 32) + 1;
        array = (unsigned int*) calloc(intLen, sizeof(unsigned int));
//        memcpy(this->array, b.array);
//        copy(begin(b.array), end(b.array), begin(this->array));
//        copy(b.array, *(*b.array + (ceil((double) b.bitLen / 31)), this->array));
        for(int i = 0; i < intLen; i++){
            this->array[i] = b.array[i];
        }
    }
    ~BitArray(){
        delete array;
    }
    void swap(BitArray &b){           //works for BitArray with same sizes
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
//        unsigned int bBitLen = this->bitLen;
//        unsigned int aBitLen = b.bitLen;
//        int aIntLen = (int) (aBitLen / 32) + 1;
//        int bIntLen = (int) (bBitLen / 32) + 1;
//        auto *aArr = new unsigned int[aIntLen];
//        auto *bArr = new unsigned int[bIntLen];
//        for(int i = 0; i < aIntLen; i++) aArr[i] = b.array[i];
    }
    void resize(int num_bits, bool value = false){
        bool isBigger = (num_bits > this->bitLen ? true : false);
        this->bitLen = num_bits;
        int intLen = (int) (bitLen / 32) + 1;
        realloc(array, intLen);
        if(isBigger){
            for(int i = this->bitLen - 1; i >= 0; i--){
                cout << ((array[(int)i / 32] >> (i % 32)) & 1);
                array[(int)i / 32] >> (i % 32)
        }
    }
    void print(){
//        unsigned long long bitness = bitLen;//(unsigned long long) pow(2, bitLen);
//        for (; bitness > 0; bitness--) {
//            cout << (((1 << bitness) & array[0]) ? '1' : '0');
//        }
        cout << "bitLen is " << this->bitLen << ", intLen is " << (int) bitLen / 32 + 1 << " int representation of first 32 bits is " << this->array[0] << " ";
        for(int i = bitLen - 1; i >= 0; i--){
            cout << ((array[(int)i / 32] >> (i % 32)) & 1);
//            cout << ((array[0] >> i) & 1);
//            cout << ((array[(int) bitLen / 8] >> (i % 8)) & 1);
        }
        cout << endl;
    }
};

int main(){
    BitArray bitA(8, 255);
    bitA.print();
    BitArray bitB(6, 63);
    bitB.print();
    bitA.swap(bitB);
    bitA.print();
    bitB.print();
    return 0;
}