#include <iostream>

#include "gtest/gtest.h"
#include "mathtools.h"
#include "logging.h"

TEST(solve_linear_test, sample_test)
{
	double root_1 = NAN;
    int n  = solve_linear(1, 1, &root_1);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(root_1, -1.0);
    // LOG("THIS IS ROOT");
    // LOG(root_1);
}

TEST(solve_linear_test, infinite_roots)
{
	double root_1 = NAN;
    int n  = solve_linear(0, 0, &root_1);
    EXPECT_EQ(n, MT_INF_ROOTS);
}


TEST(solve_linear_test, no_roots)
{
	double root_1 = NAN;
    int n  = solve_linear(0, 1, &root_1);
    EXPECT_EQ(n, 0);
}

TEST(solve_square_test, two_roots)
{
	double root_1 = NAN, root_2 = NAN;
    int n  = solve_square(1, 0, -1, &root_1, &root_2);
    EXPECT_EQ(n, 2);
    EXPECT_EQ(root_2, -1.0);
    EXPECT_EQ(root_1, 1.0);
}

TEST(solve_square_test, one_root)
{
	double root_1 = NAN, root_2 = NAN;
    int n  = solve_square(1, 2, 1, &root_1, &root_2);
    EXPECT_EQ(n, 1);
    EXPECT_EQ(root_1, -1);
}

TEST(solve_square_test, no_roots)
{
	double root_1 = NAN, root_2 = NAN;
    int n  = solve_square(1, 1, 1, &root_1, &root_2);
    EXPECT_EQ(n, 0);
    n  = solve_square(4, 3, 2, &root_1, &root_2);
    EXPECT_EQ(n, 0);
}

TEST(solve_square_test, infinite_roots)
{
	double root_1 = NAN, root_2 = NAN;
    int n  = solve_square(0, 0, 0, &root_1, &root_2);
    EXPECT_EQ(n, MT_INF_ROOTS);
}