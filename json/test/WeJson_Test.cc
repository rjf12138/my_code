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
    WeJson json;
    json.open_json("./json_text/test.json");
    ASSERT_EQ(json["name"], ValueTypeCast("rjf12138"));
    ASSERT_EQ(json["age"], ValueTypeCast(25));
    ASSERT_EQ(json["height"], ValueTypeCast(171.2));
    ASSERT_EQ(json["weight"], ValueTypeCast(50.3));
}

}
}
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}