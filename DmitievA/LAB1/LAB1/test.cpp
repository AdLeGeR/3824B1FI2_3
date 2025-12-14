#include "pch.h"
#include <stdexcept> 
#include "../BitString/tbitfield.cpp"
#include "../BitString/tset.cpp"


//  TBitField  

TEST(BitField, CreateAndSizeCheck) {
    TBitField bf(10);
    ASSERT_EQ(10, bf.GetLength());
    ASSERT_THROW(TBitField(-5), std::invalid_argument);
}

TEST(BitField, BitManipulation) {
    TBitField bf(5);

    bf.SetBit(2);
    EXPECT_TRUE(bf.GetBit(2));

    bf.ClrBit(2);
    EXPECT_FALSE(bf.GetBit(2));

    EXPECT_THROW(bf.SetBit(10), std::out_of_range);
    EXPECT_THROW(bf.GetBit(-2), std::out_of_range);
}

TEST(BitField, BitwiseOperations) {
    TBitField x(4), y(4);

    x.SetBit(1);
    y.SetBit(2);

    TBitField resOr = x | y;
    EXPECT_TRUE(resOr.GetBit(1));
    EXPECT_TRUE(resOr.GetBit(2));

    TBitField resAnd = x & y;
    EXPECT_FALSE(resAnd.GetBit(1));
    EXPECT_FALSE(resAnd.GetBit(2));

    TBitField resNot = ~x;
    EXPECT_FALSE(resNot.GetBit(1));
    for (int i = 0; i < 4; ++i) {
        if (i != 1)
            EXPECT_TRUE(resNot.GetBit(i));
    }
}

// TSet 

TEST(Set, InsertDeleteMembership) {
    TSet s(5);

    EXPECT_FALSE(s.IsMember(3));
    s.InsElem(3);
    EXPECT_TRUE(s.IsMember(3));
    s.DelElem(3);
    EXPECT_FALSE(s.IsMember(3));
}

TEST(Set, UnionAndIntersection) {
    TSet s1(4), s2(4);

    s1.InsElem(1);
    s2.InsElem(2);

    TSet uni = s1 + s2;
    EXPECT_TRUE(uni.IsMember(1));
    EXPECT_TRUE(uni.IsMember(2));

    TSet inter = s1 * s2;
    EXPECT_FALSE(inter.IsMember(1));
    EXPECT_FALSE(inter.IsMember(2));
}

TEST(Set, SetComplement) {
    TSet s(3);
    s.InsElem(0);

    TSet inv = ~s;
    EXPECT_FALSE(inv.IsMember(0));
    EXPECT_TRUE(inv.IsMember(1));
    EXPECT_TRUE(inv.IsMember(2));
}

