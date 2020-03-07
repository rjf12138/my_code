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
    struct value_type val_stru = {0, 2655, false, "Hello, World!"};
    ValueType val("Hello, world!", 2333);
    val.set_struct(val_stru);
    int vali = val;
    string valstr = val;
    value_type valvt = val;

    ASSERT_EQ(vali, 2333);
    ASSERT_EQ(valstr, "Hello, world!");
    ASSERT_EQ(valvt.type_, 0);
    ASSERT_EQ(valvt.int_val_, 2655);
    ASSERT_EQ(valvt.bool_val_, false);
    ASSERT_EQ(valvt.str_val_, "Hello, World!");
}

}
}
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}