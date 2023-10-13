#ifndef MYBITARRAY_H
#define MYBITARRAY_H

#include <iostream>
#include <cmath>
#include <string>

class BitArray{
private:
    unsigned int *array;
    int bitLen;
    BitArray& set(int n, bool val, bool isService);
    BitArray& reset(int n, bool isService);
public:
    /**
     * @brief Class constructor.
     * @return Empty BitArray obj, with zero len.
     */
    BitArray();
    /**
     * @brief Class constructor.
     * @param num_bits Len of new BitArray in bits.
     * @param value Value to initialize first 4 bytes (32 bits).
     * @return New BitArray obj.
     */
    explicit BitArray(int num_bits, unsigned long value = 0);
    /**
     * @brief Class constructor.
     * @param b BitArray obj to make copy by it.
     * @return New BitArray obj.
     */
    BitArray(const BitArray& b);
    /**
     * @brief Class desstructor.
     */
    ~BitArray();
    /**
     * @brief Swap bit string and lens between 2 BitArray objects.
     * @param b BitArray obj to swap this for it.
     */
    void swap(BitArray &b);
    /**
 * @brief Increase / decrease bit string len.
 * @param num_bits New len.
 * @param value If new len is bigger then prev, new bits will have value val.
 */
    void resize(int num_bits, bool value=false);
    /**
 * @brief Clear bit string and free it's mem.
 */
    void clear();
    /**
 * @brief Add new bit in bit string.
 * @param bit val of new bit.
 */
    void push_back(bool bit);
    /**
 * @brief Copy bit string in this from second BitArray obj.
 * @param b BitArray obj source to copy.
 * @return first BitArray obj pointer.
 */
    BitArray& operator=(const BitArray& b);
    /**
* @brief Make bit AND operation with second BitArray obj.
* @param b second BitArray obj.
* @return first BitArray obj pointer.
*/
    BitArray& operator&=(const BitArray& b);
    /**
* @brief Make bit OR operation with second BitArray obj.
* @param b second BitArray obj.
* @return first BitArray obj pointer.
*/
    BitArray& operator|=(const BitArray& b);
    /**
* @brief Make XOR operation with second BitArray obj.
* @param b second BitArray obj.
* @return first BitArray obj pointer.
*/
    BitArray& operator^=(const BitArray& b);
    /**
* @brief Make left shift and update bit string by new val.
* @param n size of shift.
* @return first BitArray obj pointer.
*/
    BitArray& operator<<=(int n);
    /**
* @brief Make right shift and update bit string by new val.
* @param n Size of shift.
* @return first BitArray obj pointer.
*/
    BitArray& operator>>=(int n);
    /**
* @brief Make left shift.
* @param n Size of shift.
* @return New BitArray obj with obtained bit string.
*/
    BitArray operator<<(int n) const;
    /**
* @brief Make right shift.
* @param n Size of shift.
* @return New BitArray obj with obtained bit string.
*/
    BitArray operator>>(int n) const;
    /**
* @brief Set val in bit string in particular pos.
* @param n Index of bit to set.
* @param val Val to set.
* @return this BitArray pointer.
*/
    BitArray& set(int n, bool val = true);
    /**
* @brief Set all bits in bit string with val true.
* @return this BitArray pointer.
*/
    BitArray& set();
    /**
* @brief Get val from bit string.
* @param i Index of bit to get.
* @return Val of i'th bit.
*/
    bool operator[](int i) const;
/**
* @brief Set false in bit string in particular pos.
* @param n Index of bit to reset.
* @return this BitArray pointer.
*/
    BitArray& reset(int n);
    /**
* @brief Set all bit in bit string by zero.
* @return this BitArray pointer.
*/
    BitArray& reset();
    /**
* @brief Check is any true bit in bit string.
* @return Result.
*/
    bool any() const;
    /**
* @brief Check is all bits are zero bit string.
* @return Result.
*/
    bool none() const;
    /**
* @brief Inverts all bits.
* @return New BitArray obj with inverted bits.
*/
    BitArray operator~() const;
    /**
* @brief Count all true bits in bit string.
* @return Result.
*/
    int count() const;
    /**
* @return Len of bit string.
*/
    int size() const;
    /**
* @brief Check is bit string empty.
* @return Result.
*/
    bool empty() const;
    /**
* @brief Convert bit string to C++ std::string.
* @return Result.
*/
    std::string to_string() const;
    /**
* @brief Check are first and second BitArray objects are equal.
* @return Result.
*/
    bool operator==(const BitArray &b) const;
    /**
* @brief Check AREN'T first and second BitArray objects are equal.
* @return Result.
*/
    bool operator!=(const BitArray & b) const;
    /**
* @brief Make bit AND operation between two objects.
* @param b2 Second BitArray obj fro operation;
* @return New BitArray with result.
*/
    BitArray operator&(const BitArray& b2) const;
    /**
* @brief Make bit AND operation between two objects.
* @param b2 Second BitArray obj fro operation;
* @return New BitArray with result.
*/
    BitArray operator|(const BitArray& b2) const;
    /**
* @brief Make bit OR operation between two objects.
* @param b2 Second BitArray obj fro operation;
* @return New BitArray with result.
*/
    BitArray operator^(const BitArray& b2) const;
    /**
* @brief Print bit string length, bit string, and int mapping of every 4byte block.
*/
    void print();
};

#endif //MYBITARRAY_H