#include "basic_head.h"
#include "byte_buffer.h"
#include "gtest/gtest.h"

namespace my {
namespace project {
namespace {


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
    int buff_size = 64;
    ByteBuffer buff(buff_size);
    // 读写8位数据
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(buff.write_int8('a'), 1);
    EXPECT_EQ(buff.data_size(), 1);
    EXPECT_EQ(buff.idle_size(), buff_size - 1);
    int8_t val_8;
    EXPECT_EQ(buff.read_int8(val_8), 1);
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(val_8, 'a');

    // 读写16位数据
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(buff.write_int16(6536), 2);
    EXPECT_EQ(buff.data_size(), 2);
    EXPECT_EQ(buff.idle_size(), buff_size - 2);
    int16_t val_16;
    EXPECT_EQ(buff.read_int16(val_16), 2);
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(val_16, 6536);

    // 读写32位数据
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(buff.write_int32(655536), 4);
    EXPECT_EQ(buff.data_size(), 4);
    EXPECT_EQ(buff.idle_size(), buff_size - 4);
    int32_t val_32;
    EXPECT_EQ(buff.read_int32(val_32), 4);
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(val_32, 655536);

    // 读写64位数据
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(buff.write_int64(65566536), 8);
    EXPECT_EQ(buff.data_size(), 8);
    EXPECT_EQ(buff.idle_size(), buff_size - 8);
    int64_t val_64;
    EXPECT_EQ(buff.read_int64(val_64), 8);
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(val_64, 65566536);

    // 读写字符串
    string str = "Hello, world";
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(buff.write_string(str), str.length());
    EXPECT_EQ(buff.data_size(), str.length() + 1);
    EXPECT_EQ(buff.idle_size(), buff_size - str.length() - 1);
    string val_str;
    EXPECT_EQ(buff.read_string(val_str), str.length());
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(val_str, str);

    // 读写指定字节数据
    struct test_stru {
        int8_t i8;
        int16_t i16;
        string str;
        char buf[16];
    };
    struct test_stru stru = {'b', 12345, "Nice to meet you", "hello"};

    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    EXPECT_EQ(buff.write_bytes((void*)&stru, sizeof(stru)) , sizeof(stru));
    EXPECT_EQ(buff.data_size(), sizeof(stru));
    EXPECT_EQ(buff.idle_size(), buff_size - sizeof(stru));
    struct test_stru val_stru;
    EXPECT_EQ(buff.read_bytes(&val_stru, sizeof(stru)), sizeof(stru));
    EXPECT_EQ(buff.data_size(), 0);
    EXPECT_EQ(buff.idle_size(), buff_size);
    
    EXPECT_EQ(val_stru.i8, stru.i8);
    EXPECT_EQ(val_stru.i16, stru.i16);
    EXPECT_EQ(val_stru.str, stru.str);
    EXPECT_EQ(strcmp(val_stru.buf, stru.buf), 0);
}

}  // namespace
}  // namespace project
}  // namespace my


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}