//#include "googletest/googletest/include/gtest/gtest.h"
//#include <gtest/gtest.h>
#include "mybitarray.h"
#include <string>
#include <gtest/gtest.h>

//V new params
// new copy
// swap

//TEST(firstTest, HandlesPositiveInput) {
//    BitArray bitArr1(8, 127);
//    EXPECT_EQ(bitArr1.to_string(), "01111111");
//    BitArray bitArr2(80, 2222222222);
//    EXPECT_EQ(bitArr2.to_string(), "00000000000000000000000000000000000000000000000010000100011101000110101110001110");
//    bitArr2.resize(40, true);
//    EXPECT_EQ(bitArr2.to_string(), "0000000010000100011101000110101110001110");
//    bitArr1.resize(120, true);
//    bitArr1.print();
//    std::cout << "\n\n\n\n\n\n\n\n\n\nBRBREARARSE";
//    EXPECT_EQ(bitArr1.to_string(), "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111101111111");
//    bitArr1.clear();
//    bitArr1.push_back(true);
//    bitArr1.push_back(false);
//    bitArr1.push_back(true);
//    EXPECT_EQ(bitArr1.to_string(), "101");
////    EXPECT_THROW({
////        try{
////            bitArr1.swap(bitArr2);
////            EXPECT_EQ(bitArr2.to_string(), "101");
////            EXPECT_EQ(bitArr1.to_string(), "0000000010000100011101000110101110001110");
////        } catch (const e)
////    })
////    bitArr1.swap(bitArr2);
////    EXPECT_EQ(bitArr1.to_string(), "101");
////    EXPECT_EQ(bitArr1.to_string(), "0000000010000100011101000110101110001110");
//}

#define UNSIGNED 4294967295

TEST(BitArrayTest, DefaultConstructor) {
    BitArray bits;
    EXPECT_EQ(bits.size(), 0);
    EXPECT_TRUE(bits.empty());
}

TEST(BitArrayTest, ConstructorWithValue) {
    BitArray bits(8, 255);
    EXPECT_EQ(bits.size(), 8);
    for (int i = 0; i < 8; i++) {
        EXPECT_TRUE(bits[i]);
    }
}

TEST(BitArrayTest, CopyConstructor) {
    BitArray bits(8, 4);
    BitArray copy(bits);
    EXPECT_EQ(copy.size(), bits.size());
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_EQ(copy[i], bits[i]);
    }
}

TEST(BitArrayTest, AssignmentOperator) {
    BitArray bits(8, 4);
    BitArray copy;
    copy = bits;
    EXPECT_EQ(copy.size(), bits.size());
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_EQ(copy[i], bits[i]);
    }
}

TEST(BitArrayTest, Swap) {
    BitArray bits1(8, 8);
    BitArray bits2(50, 1000);
    int len1 = bits1.size();
    int len2 = bits2.size();
    std::string b1 = bits1.to_string();
    std::string b2 = bits2.to_string();
    bits1.swap(bits2);
    for (int i = 0; i < len1; i++) EXPECT_EQ(bits2[i], b1.substr(len1 - i - 1, 1) == "1");
    for (int i = 0; i < len2; i++) EXPECT_EQ(bits1[i], b2.substr(len2 - i - 1, 1) == "1");
}

TEST(BitArrayTest, Resize) {
    BitArray bits(8, UNSIGNED);
    std::cout << bits.to_string() << std::endl;
    bits.resize(4);
    EXPECT_EQ(bits.size(), 4);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_TRUE(bits[i]);
    }
    std::cout << bits.to_string() << std::endl;
    bits.resize(16, false);
    std::cout << bits.to_string() << std::endl;
    EXPECT_EQ(bits.size(), 16);
    for (int i = 0; i < 4; i++) {
        EXPECT_TRUE(bits[i]);
    }
    for (int i = 4; i < bits.size(); i++) {
        EXPECT_FALSE(bits[i]);
    }
}
//
TEST(BitArrayTest, Clear) {
    BitArray bits(8, true);
    bits.clear();
    EXPECT_EQ(bits.size(), 0);
    EXPECT_TRUE(bits.empty());
}
//
TEST(BitArrayTest, PushBack) {
    BitArray bits;
    bits.push_back(true);
    EXPECT_EQ(bits.size(), 1);
    EXPECT_TRUE(bits[0]);

    bits.push_back(false);
    EXPECT_EQ(bits.size(), 2);
    EXPECT_FALSE(bits[1]);
}
//
TEST(BitArrayTest, BitwiseAnd1st) {
    BitArray bits1(4, 15);  //1111
    BitArray bits2(4, 3);   //0011
    BitArray result = bits1 & bits2;
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], true);
    EXPECT_EQ(result[1], true);
    EXPECT_EQ(result[2], false);
    EXPECT_EQ(result[3], false);
}

TEST(BitArrayTest, BitwiseAnd2st){
    BitArray bits1(4, 7);  //0111
    BitArray bits2(4, 8);   //1000
    BitArray result = bits1 & bits2;
//    std::cout << bits1.to_string() << "_" << bits2.to_string() << "_" << result.to_string() << std::endl;
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], false);
    EXPECT_EQ(result[1], false);
    EXPECT_EQ(result[2], false);
    EXPECT_EQ(result[3], false);
}
//
TEST(BitArrayTest, BitwiseOr1) {
    BitArray bits1(4, 15);  //1111
    BitArray bits2(4, 0);   //0000
    BitArray result = bits1 | bits2;
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], true);
    EXPECT_EQ(result[1], true);
    EXPECT_EQ(result[2], true);
    EXPECT_EQ(result[3], true);
}

TEST(BitArrayTest, BitwiseOr2) {
    BitArray bits1(4, 3);   //0011
    BitArray bits2(4, 12);  //1100
    BitArray result = bits1 | bits2;
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], true);
    EXPECT_EQ(result[1], true);
    EXPECT_EQ(result[2], true);
    EXPECT_EQ(result[3], true);
}

TEST(BitArrayTest, BitwiseOr3) {
    BitArray bits1(4, 0);   //0000
    BitArray bits2(4, 0);   //0000
    BitArray result = bits1 | bits2;
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], false);
    EXPECT_EQ(result[1], false);
    EXPECT_EQ(result[2], false);
    EXPECT_EQ(result[3], false);
}
//
TEST(BitArrayTest, BitwiseXor) {
    BitArray bits1(4, 15);  //1111
    BitArray bits2(4, 3);   //0011
    BitArray result = bits1 ^ bits2;      //1100
    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], false);
    EXPECT_EQ(result[1], false);
    EXPECT_EQ(result[2], true);
    EXPECT_EQ(result[3], true);
}
//
TEST(BitArrayTest, BitwiseNot) {
    BitArray bits(8, 15);//00001111
    BitArray result = ~bits;          //11110000
    EXPECT_EQ(result.size(), 8);
    EXPECT_EQ(result[0], false);
    EXPECT_EQ(result[1], false);
    EXPECT_EQ(result[2], false);
    EXPECT_EQ(result[3], false);
    EXPECT_EQ(result[4], true);
    EXPECT_EQ(result[5], true);
    EXPECT_EQ(result[6], true);
    EXPECT_EQ(result[7], true);
}
//
TEST(BitArrayTest, IndexOperator) {
    BitArray bits(8, 255);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_EQ(bits[i], true);
    }
}
//
//
TEST(BitArrayTest, AnyTest1) {
    const BitArray bits(8, 1);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_TRUE(bits.any());
    }
}
TEST(BitArrayTest, AnyTest2) {
    const BitArray bits(8, 126);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_TRUE(bits.any());
    }
}
TEST(BitArrayTest, AnyTest3) {
    const BitArray bits(8, 0);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_FALSE(bits.any());
    }
}
//
TEST(BitArrayTest, NoneTest1) {
    const BitArray bits(8, 0);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_TRUE(bits.none());
    }
}
TEST(BitArrayTest, NoneTest2) {
    const BitArray bits(8, 37);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_FALSE(bits.none());
    }
}
//
TEST(BitArrayTest, CountTest1) {
    const BitArray bits(8, 255);
    EXPECT_EQ(bits.count(), 8);
}
TEST(BitArrayTest, CountTest2) {
    const BitArray bits(8, 0);
    EXPECT_EQ(bits.count(), 0);
}
TEST(BitArrayTest, CountTest3) {
    BitArray bits(32, UNSIGNED);
    bits.push_back(true);
    bits.push_back(true);
    bits.push_back(false);
    bits.push_back(false);
    bits.push_back(true);
    EXPECT_EQ(bits.count(), 35);
}
TEST(BitArrayTest, CountTest_ResizeTest) {
    BitArray bits(7, 5);
    bits.print();
    std::cout << "NUM OF true BITS IS " << bits.count() << std::endl;
    bits.resize(100, true);
    bits.print();
    std::cout << "NUM OF true BITS IS " << bits.count() << std::endl;
    bits.resize(200, false);
    bits.print();
    std::cout << "NUM OF true BITS IS " << bits.count() << std::endl;
    bits.resize(300, true);
    bits.print();
    std::cout << "NUM OF true BITS IS " << bits.count() << std::endl;
    bits.resize(400, false);
    bits.print();
    std::cout << "NUM OF true BITS IS " << bits.count() << std::endl;
    EXPECT_EQ(bits.count(), 195);
}

TEST(BitArrayTest, LeftShiftAssignmentTest) {
    BitArray bits(8, 255);
    bits<<=(2);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_EQ(bits.to_string(), "11111100");
    }
}
//
TEST(BitArrayTest, RightShiftAssignmentTest) {
    BitArray bits(8, 255);
    bits>>=(2);
    for (int i = 0; i < bits.size(); i++) {
        EXPECT_EQ(bits.to_string(), "00111111");
    }
}
//
TEST(BitArrayTest, RightShiftTest) {
    BitArray bits(8, 255);
    BitArray b2 = bits>>(2);
    for (int i = 0; i < b2.size(); i++) {
        EXPECT_EQ(b2.to_string(), "00111111");
    }
}
TEST(BitArrayTest, LeftShiftTest) {
    BitArray bits(8, 255);
    BitArray b2 = bits<<(2);
    for (int i = 0; i < b2.size(); i++) {
        EXPECT_EQ(b2.to_string(), "11111100");
    }
}
//
//
TEST(BitArrayTest, ToStringTest) {
    BitArray bits(100, 4);
    EXPECT_EQ(bits.to_string(), bits.to_string());  //OOO YEEEah
}
////
TEST(BitArrayTest, SetTest) {
    BitArray bits(8, 15);
    bits.set();
    EXPECT_EQ(bits.to_string(), "11111111");
}
//
TEST(BitArrayTest, SetWithArgsTest1) {
    BitArray bits(50, UNSIGNED);
    bits.set(45, true);
    EXPECT_TRUE(bits[45]);
    EXPECT_EQ(bits.to_string(), "00001000000000000011111111111111111111111111111111");
}
TEST(BitArrayTest, SetWithArgsTest2) {
    BitArray bits(32, UNSIGNED);
    bits.resize(50, true);
    bits.set(45, false);
    EXPECT_FALSE(bits[45]);
    EXPECT_EQ(bits.to_string(), "11110111111111111111111111111111111111111111111111");
}
TEST(BitArrayTest, SetWithArgsTest3) {
    BitArray bits(32, UNSIGNED);
    bits.set(5, false);
    EXPECT_FALSE(bits[5]);
}
////
TEST(BitArrayTest, ResetTest) {
    BitArray bits(100, 255);
    bits.reset();
    EXPECT_EQ(bits.count(), 0);
}
//
TEST(BitArrayTest, ResetWithArgsTest) {
    BitArray bits(8, 255);
    bits.reset(4);
    EXPECT_EQ(bits.to_string(), "11101111");
}