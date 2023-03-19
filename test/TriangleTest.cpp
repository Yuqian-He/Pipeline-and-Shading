#include<iostream>
#include<gtest/gtest.h>
#include"Triangle.hpp"

TEST(Triangle,construct)
{
    Triangle t;
    EXPECT_FLOAT_EQ(t.v[1][0],0.0f);
    EXPECT_FLOAT_EQ(t.v[1][1],0.0f);
    EXPECT_FLOAT_EQ(t.v[1][2],0.0f);
    EXPECT_FLOAT_EQ(t.v[1][3],1.0f);

    EXPECT_FLOAT_EQ(t.color[1][0],0.0f);
    EXPECT_FLOAT_EQ(t.color[1][1],0.0f);
    EXPECT_FLOAT_EQ(t.color[1][2],0.0f);

    EXPECT_FLOAT_EQ(t.tex_coords[1][0],0.0f);
    EXPECT_FLOAT_EQ(t.tex_coords[1][1],0.0f);

}

TEST(Triangle, toVector4)
{
    Triangle t;
    auto test = t.toVector4();
    EXPECT_FLOAT_EQ(test[1][0],0.0f);
    EXPECT_FLOAT_EQ(test[1][1],0.0f);
    EXPECT_FLOAT_EQ(test[1][2],0.0f);
    EXPECT_FLOAT_EQ(test[1][3],1.0f);
}