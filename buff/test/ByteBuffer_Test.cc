#include "basic_head.h"
#include "byte_buffer.h"
#include "gtest/gtest.h"

class ByteBuffer_Test : public ::testing::Test {
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

TEST_F(ByteBuffer_Test, IsEmptyInitially) 
{
    ByteBuffer buff(30);
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), 30);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}