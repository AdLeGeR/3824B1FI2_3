#include "pch.h"
#include "../Lab2/utmatrix.h"
#include <sstream>

TEST(TVector, vector_length) {
    EXPECT_NO_THROW(TVector<int> v(3));
    EXPECT_THROW(TVector<int> v(-2), std::out_of_range);
    EXPECT_THROW(TVector<int> v(MAX_VECTOR_SIZE + 2), std::out_of_range);
}

TEST(TVector, vector_start_index) {
    EXPECT_NO_THROW(TVector<int> v(3, 1));
    EXPECT_THROW(TVector<int> v1(3, -1), std::out_of_range);
}

TEST(TVector, copied_vector_is_equal) {
    TVector<int> v(4);
    v[0] = 1; v[1] = 2; v[2] = 3; v[3] = 4;
    TVector<int> v2(v);
    EXPECT_EQ(v, v2);
}

TEST(TVector, copied_vector_not_affect_on_source) {
    TVector<int> v(3);
    v[0] = 1;
    TVector<int> v2(v);
    v2[0] = 5;
    EXPECT_NE(v[0], v2[0]);
}

TEST(TVector, getter) {
    TVector<int> v(5, 2);
    EXPECT_EQ(5, v.GetSize());
    EXPECT_EQ(2, v.GetStartIndex());
}

TEST(TVector, index_is_out_of_range) {
    TVector<int> v(5);
    EXPECT_THROW(v[-1], std::out_of_range);
    EXPECT_THROW(v[5], std::out_of_range);
}

TEST(TVector, equality_operator) {
    TVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 1; v2[1] = 2; v2[2] = 3;
    EXPECT_TRUE(v1 == v2);
    TVector<int> v1_1(3), v1_2(3);
    v1_1[0] = 1; v1_2[0] = 2;
    EXPECT_TRUE(v1_1 != v1_2);
	TVector<int> v2_1(2), v2_2(3);
	v2_1[0] = 1; v2_2[0] = 1;
	v2_1[1] = 2; v2_2[1] = 2;
	EXPECT_FALSE(v2_1 == v2_2);
}

TEST(TVector, assign_vector) {
    TVector<int> v(3);
    v[0] = 1;
    EXPECT_NO_THROW(v = v);
    TVector<int> v1(3), v2(3);
    v1[0] = 5;
    v2 = v1;
    EXPECT_EQ(v1, v2);
    TVector<int> v1_1(3), v1_2(5);
    v1[0] = 7;
    v1_2 = v1_1;
    EXPECT_EQ(v1_1, v1_2);
}

TEST(TVector, vector_and_digit) {
    TVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TVector<int> res = v + 5;
    EXPECT_EQ(6, res[0]);
    EXPECT_EQ(7, res[1]);
    EXPECT_EQ(8, res[2]);

    TVector<int> res2 = v - 1;
    EXPECT_EQ(0, res2[0]);
    EXPECT_EQ(1, res2[1]);
    EXPECT_EQ(2, res2[2]);

    TVector<int> res3 = v * 2;
    EXPECT_EQ(2, res3[0]);
    EXPECT_EQ(4, res3[1]);
    EXPECT_EQ(6, res3[2]);
}

TEST(TVector, vectors_plus_minus) {
    TVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    TVector<int> res = v1 + v2;
    EXPECT_EQ(5, res[0]);
    EXPECT_EQ(7, res[1]);
    EXPECT_EQ(9, res[2]);

    TVector<int> res2 = v2 - v1;
    EXPECT_EQ(3, res2[0]);
    EXPECT_EQ(3, res2[1]);
    EXPECT_EQ(3, res2[2]);

    TVector<int> v1_1(3), v1_2(4);
    EXPECT_THROW(v1_1 + v1_2, std::invalid_argument);
    EXPECT_THROW(v1_1 - v1_2, std::invalid_argument);
}

TEST(TVector, multiply_vectors) {
    TVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;
    int res = v1 * v2;
    EXPECT_EQ(32, res); // 1*4 + 2*5 + 3*6

    TVector<int> vec1(3), vec2(4);
    EXPECT_THROW(vec1 * vec2, std::invalid_argument);
}

TEST(TVector, output_operator) {
    TVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;
    std::ostringstream output;
    output << v;
    EXPECT_EQ("1 2 3", output.str());
}

TEST(TVector, input_operator) {
    TVector<int> v(3);
    std::istringstream input("1 2 3");
    input >> v;
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}


TEST(TMatrix, matrix_creation) {
    TMatrix<int> m(3);
    EXPECT_NO_THROW(TMatrix<int> m(3));
    EXPECT_THROW(TMatrix<int> m1(-2), std::out_of_range);
	EXPECT_THROW(TMatrix<int> m2(MAX_MATRIX_SIZE + 2), std::out_of_range);
}

TEST(TMatrix, equation_test) {
    TVector<int> v1(3), v2(3, 1), v3(3, 2);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[1] = 4; v2[2] = 5;
    v3[2] = 6;
    TVector<TVector<int>> m(3);
    m[0] = v1;
    m[1] = v2;
    m[2] = v3;
    TMatrix<int> m1(m), m2(m);
    m1 == m2;
    EXPECT_EQ(m1, m2);
}

TEST(TMatrix, non_equation_test) {
    TVector<int> v1(3), v2(3, 1), v3(3, 2);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[1] = 4; v2[2] = 5;
    v3[2] = 6;
    TVector<TVector<int>> m(3);
    m[0] = v1;
    m[1] = v2;
    m[2] = v3;
    TMatrix<int> m1(m);

    m[0][0] = 0;
    TMatrix<int> m2(m);
    EXPECT_TRUE(m1!=m2);
}

TEST(TMatrix, input_matrix) {
	TVector<TVector<int>> m(2);
	m[0] = TVector<int>(2);
	m[1] = TVector<int>(2,1);
	m[0][0] = 1; m[0][1] = 2;
    m[1][1] = 3;
	TMatrix<int> m1(m);
	std::istringstream input("1 2 3");
	TMatrix<int> m2(2);
	input >> m2;
	EXPECT_EQ(m1, m2);
}
TEST(TMatrix, output_matrix) {
    TVector<TVector<int>> m(2);
    m[0] = TVector<int>(2);
    m[1] = TVector<int>(2,1);
    m[0][0] = 1; m[0][1] = 2;
    m[1][1] = 3;
    TMatrix<int> m1(m);
    std::ostringstream output;
    output << m1;
    EXPECT_EQ("1 2\n0 3", output.str());
}

TEST(TMatrix, plus) {
    TMatrix<int> m1(2), m2(2);
    std::istringstream input1("1 2 3");
    input1 >> m1;
    std::istringstream input2("4 5 6");
    input2 >> m2;
    TMatrix<int> m3 = m1 + m2;
    std::ostringstream output;
    output << m3;
    EXPECT_EQ("5 7\n0 9", output.str());
}