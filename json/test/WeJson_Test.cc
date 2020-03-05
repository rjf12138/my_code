#include "wejson.h"
#include "gtest/gtest.h"

using namespace my_util;

namespace my {
namespace project {
namespace {

#define MUTITHREAD 1

class WeJson_Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

TEST_F(WeJson_Test, basi_test) 
{
    struct value_type val = {0, 2655, false, "Hello, World!"};
    // ASSERT_EQ(ret_value<int>(val), 2655);
    std::cout << ret_value<int>(val) << std::endl;
    val.type_ = 1;
    std::cout << ret_value<bool>(val) << std::endl;
    // ASSERT_EQ(ret_value<bool>(val), false);
    // val.type_ = 2;
    // std::cout << (ret_value<string>(val), "Hello, World!") << std::endl;
    // ASSERT_EQ(ret_value<string>(val), "Hello, World!");
}

}
}
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}