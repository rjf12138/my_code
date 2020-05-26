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


TEST_F(WeJson_Test, NUMBER_TEST)
{
    JsonNumber json_number_test_1(1.1214);
    JsonNumber json_number_test_2(12);
    JsonNumber json_number_test_3(json_number_test_1);
    JsonNumber json_number_test_4(json_number_test_2);
    ostringstream test_stream;
    test_stream << json_number_test_1 << endl <<  json_number_test_2 << endl << json_number_test_3 << endl << json_number_test_4;
    cout << test_stream.str() << std::endl;
}

TEST_F(WeJson_Test, TYPE_TEST)
{
    
}

}
}
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}