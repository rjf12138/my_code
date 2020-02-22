#include "byte_buffer.h"
#include "gtest/gtest.h"

using namespace my_util;

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

// 对非锁的读写函数循环测试
TEST_F(ByteBuffer_Test, ByteBuff_none_lock_read_write) 
{
    int buff_size = 64;
    int test_cnt = 900000;
    ByteBuffer buff(buff_size);
    // 读写8位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(buff.write_int8('a'), 1);
        ASSERT_EQ(buff.data_size(), 1);
        ASSERT_EQ(buff.idle_size(), buff_size - 1);
        int8_t val_8;
        ASSERT_EQ(buff.read_int8(val_8), 1);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(val_8, 'a');
    }
    // 读写16位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(buff.write_int16(6536), 2);
        ASSERT_EQ(buff.data_size(), 2);
        ASSERT_EQ(buff.idle_size(), buff_size - 2);
        int16_t val_16;
        ASSERT_EQ(buff.read_int16(val_16), 2);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(val_16, 6536);
    }
    // 读写32位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(buff.write_int32(655536), 4);
        ASSERT_EQ(buff.data_size(), 4);
        ASSERT_EQ(buff.idle_size(), buff_size - 4);
        int32_t val_32;
        ASSERT_EQ(buff.read_int32(val_32), 4);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(val_32, 655536);
    }
    // 读写64位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(buff.write_int64(65566536), 8);
        ASSERT_EQ(buff.data_size(), 8);
        ASSERT_EQ(buff.idle_size(), buff_size - 8);
        int64_t val_64;
        ASSERT_EQ(buff.read_int64(val_64), 8);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(val_64, 65566536);
    }
    // 读写字符串
    string str = "Hello, world";
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(buff.write_string(str), str.length());
        ASSERT_EQ(buff.data_size(), str.length() + 1);
        ASSERT_EQ(buff.idle_size(), buff_size - str.length() - 1);
        string val_str;
        EXPECT_EQ(buff.read_string(val_str), str.length());
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(val_str, str);
    }
    // 读写指定字节数据
    struct test_stru {
        int8_t i8;
        int16_t i16;
        char str[34];
        char buf[16];
    };
    struct test_stru stru = {'b', 12345, "Nice to meet you", "hello"};
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(buff.write_bytes((void*)&stru, sizeof(stru)) , sizeof(stru));
        ASSERT_EQ(buff.data_size(), sizeof(stru));
        ASSERT_EQ(buff.idle_size(), buff_size - sizeof(stru));
        struct test_stru val_stru;
        ASSERT_EQ(buff.read_bytes(&val_stru, sizeof(stru)), sizeof(stru));
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.idle_size(), buff_size);
        ASSERT_EQ(val_stru.i8, stru.i8);
        ASSERT_EQ(val_stru.i16, stru.i16);
        ASSERT_EQ(strcmp(val_stru.str, stru.str), 0);
        ASSERT_EQ(strcmp(val_stru.buf, stru.buf), 0);
    }
}

// 测试 buffer 空间增长
TEST_F(ByteBuffer_Test, ByteBuffer_increase)
{
    ByteBuffer buff(1);
    uint32_t n = 2;
    for (int i = 0; i < 31; ++i) {
        std::cout << "i: " << i << " n: " << n << std::endl;
        for (int j = 0; j < n / 2; ++j) {
            buff.write_int8('a');
        }
        ASSERT_EQ(buff.data_size() + buff.idle_size(), n);
        ASSERT_EQ(buff.data_size(), n / 2);
        ASSERT_EQ(buff.idle_size(), n / 2);
        int8_t out;
        for (int j = 0; j < n / 2; ++j) {
            buff.read_int8(out);
            ASSERT_EQ(buff.data_size() + buff.idle_size(), n);
            ASSERT_EQ(buff.data_size(), n / 2 - j - 1);
            ASSERT_EQ(buff.idle_size(), n / 2 + j + 1);
        }

        ASSERT_EQ(buff.empty(), true);
        n = n * 2;
    }
}

}  // namespace
}  // namespace project
}  // namespace my


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}