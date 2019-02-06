#include "Json/BasicValue.hpp"
#include "gtest/gtest.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    cout << "sizeof(BasicValue<char>) = " << sizeof(json::BasicValue<char>)
         << endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}