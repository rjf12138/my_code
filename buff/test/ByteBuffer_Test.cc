#include "byte_buffer.h"
#include "thread.h"
#include "gtest/gtest.h"

using namespace my_util;

namespace my {
namespace project {
namespace {

#define MUTITHREAD 1

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

// 读写指定字节数据
struct test_stru {
    int8_t i8;
    int16_t i16;
    char str[34];
    char buf[16];
};

class TestWriteThread : public Thread {
public:
    TestWriteThread(ByteBuffer &buff, int test_cnt, int thread_cnt)
        : buff_(buff), test_cnt_(test_cnt), thread_cnt_(thread_cnt) {}
    
    int set_type(int type) {
        type_ = type;
    }

    int run_handler(void) {
        while (exit_ != true) {
            for (int i = 0; i < test_cnt_; ++i) {
                switch(type_) {
                    case 1:
                    {
                        if (MUTITHREAD)
                            buff_.write_int8_lock(vec_int8_);
                        else
                            buff_.write_int8(vec_int8_);
                    }
                    break;
                    case 2:
                    {
                        if (MUTITHREAD)
                            buff_.write_int16_lock(vec_int16_);
                        else
                            buff_.write_int16(vec_int16_);
                    }
                    break;
                    case 3:
                    {
                        if (MUTITHREAD)
                            buff_.write_int32_lock(vec_int32_);
                        else
                        {
                            buff_.write_int32(vec_int32_);
                        }
                        
                    }
                    break;
                    case 4:
                    {
                        if (MUTITHREAD)
                            buff_.write_int64_lock(vec_int64_);
                        else
                        {
                            buff_.write_int64(vec_int64_);
                        }
                        
                    }
                    break;
                    case 5:
                    {
                        if (MUTITHREAD)
                            buff_.write_string_lock(vec_string_);
                        else
                        {
                            buff_.write_string(vec_string_);
                        }
                            
                    }
                    break;
                    case 6:
                    {
                        if (MUTITHREAD)
                            buff_.write_bytes_lock(&vec_bytes_, sizeof(vec_bytes_));
                        else
                        {
                            buff_.write_bytes(&vec_bytes_, sizeof(vec_bytes_));
                        }
                    }
                    break;
                    default: {
                        std::cerr << "Unknown type." << std::endl;
                    }break;
                }
            }
            
            this->stop_handler();
        }
    }
    bool test_write_data(void) {
        for (int i = 0; i < test_cnt_ * thread_cnt_; ++i) {
            switch(type_) {
                case 1:
                {
                    int8_t tmp_val = 0;
                    buff_.read_int8_lock(tmp_val);
                    if (tmp_val != vec_int8_) {
                        return false;
                    }
                }
                break;
                case 2:
                {
                    int16_t tmp_val = 0;
                    buff_.read_int16_lock(tmp_val);
                    if (tmp_val != vec_int16_) {
                        return false;
                    }
                }
                break;
                case 3:
                {
                    int32_t tmp_val = 0;
                    buff_.read_int32_lock(tmp_val);
                    if (tmp_val != vec_int32_) {
                        return false;
                    }
                }
                break;
                case 4:
                {
                    int64_t tmp_val = 0;
                    buff_.read_int64_lock(tmp_val);
                    if (tmp_val != vec_int64_) {
                        return false;
                    }
                }
                break;
                case 5:
                {
                    string tmp_val;
                    buff_.read_string_lock(tmp_val, vec_string_.size());
                    if (tmp_val != vec_string_) {
                        std::cout << "read_size: " << tmp_val.size() << std::endl;
                        std::cout << "src_size: " << vec_string_.size() << std::endl;
                        std::cout << "read: " << tmp_val << std::endl;
                        std::cout << "src: " << vec_string_ << std::endl;
                        return false;
                    }
                }
                break;
                case 6:
                {
                    test_stru tmp_val;
                    buff_.read_bytes_lock(&tmp_val, sizeof(test_stru));
                    if (tmp_val.i8 != vec_bytes_.i8 || tmp_val.i16 != vec_bytes_.i16) {
                        std::cerr << "stru:i8 " << tmp_val.i8 << " stru:i16 " << tmp_val.i16 << std::endl;
                        return false;
                    }
                    if (strcmp(tmp_val.buf, vec_bytes_.buf) != 0 || strcmp(tmp_val.str, vec_bytes_.str) != 0) {
                        std::cerr << "stru:buf " << tmp_val.buf << " stru:str " << tmp_val.str << std::endl;
                        return false;
                    }
                }
                break;
                default: {
                    std::cerr << "Unknown type." << std::endl;
                    return false;
                }break;
            }
        }
        return true;
    }

    int stop_handler(void) {
        exit_ = true;
    }
    int start_handler(void) {
        exit_ = false;
    }

private:
    bool exit_ = false;
    int type_;
    int test_cnt_;
    int thread_cnt_;
    ByteBuffer &buff_;
    int8_t vec_int8_ = 'a';
    int16_t vec_int16_ = 3567;
    int32_t vec_int32_ = 2147483645;
    int64_t vec_int64_ = 21474836400;
    string vec_string_ = "Hello, world!";
    test_stru vec_bytes_ = {'b', 12345, "Nice to meet you", "hello"};
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
        ASSERT_EQ(buff.write_int8('a'), 1);
        ASSERT_EQ(buff.data_size(), 1);
        int8_t val_8;
        ASSERT_EQ(buff.read_int8(val_8), 1);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(val_8, 'a');
    }
    // 读写16位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.write_int16(6536), 2);
        ASSERT_EQ(buff.data_size(), 2);
        int16_t val_16;
        ASSERT_EQ(buff.read_int16(val_16), 2);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(val_16, 6536);
    }
    // 读写32位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.write_int32(655536), 4);
        ASSERT_EQ(buff.data_size(), 4);
        int32_t val_32;
        ASSERT_EQ(buff.read_int32(val_32), 4);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(val_32, 655536);
    }
    // 读写64位数据
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.write_int64(65566536), 8);
        ASSERT_EQ(buff.data_size(), 8);
        int64_t val_64;
        ASSERT_EQ(buff.read_int64(val_64), 8);
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(val_64, 65566536);
    }
    // 读写字符串
    string str = "Hello, world";
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.write_string(str), str.length());
        ASSERT_EQ(buff.data_size(), str.length());
        string val_str;
        EXPECT_EQ(buff.read_string(val_str), str.length());
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(val_str, str);
    }

    struct test_stru stru = {'b', 12345, "Nice to meet you", "hello"};
    for (int i = 0; i < test_cnt; ++i) {
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(buff.write_bytes((void*)&stru, sizeof(stru)) , sizeof(stru));
        ASSERT_EQ(buff.data_size(), sizeof(stru));
        struct test_stru val_stru;
        ASSERT_EQ(buff.read_bytes(&val_stru, sizeof(stru)), sizeof(stru));
        ASSERT_EQ(buff.data_size(), 0);
        ASSERT_EQ(val_stru.i8, stru.i8);
        ASSERT_EQ(val_stru.i16, stru.i16);
        ASSERT_EQ(strcmp(val_stru.str, stru.str), 0);
        ASSERT_EQ(strcmp(val_stru.buf, stru.buf), 0);
    }
}

// 测试 buffer 空间增长
#define TEST_SPCE_INCREASE 26
TEST_F(ByteBuffer_Test, ByteBuffer_increase)
{
    ByteBuffer buff(1);
    BUFSIZE_T n = 2;
    for (int i = 0; i < TEST_SPCE_INCREASE; ++i) {
        for (BUFSIZE_T j = 0; j < n / 2; ++j) {
            buff.write_int8('a');
        }
        ASSERT_EQ(buff.data_size(), n / 2);
        int8_t out;
        for (BUFSIZE_T j = 0; j < n / 2; ++j) {
            buff.read_int8(out);
            ASSERT_EQ(buff.data_size(), n / 2 - j - 1);
        }

        ASSERT_EQ(buff.empty(), true);
        n = n * 2;
    }
}

#define TEST_THREAD_NUM 1000
#define TEST_COUNT 8000

TEST_F(ByteBuffer_Test, mutil_thread_read_write)
{
    ByteBuffer buff(0);

    vector<TestWriteThread> test;
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test.push_back(TestWriteThread(buff, TEST_COUNT, TEST_THREAD_NUM));
    }
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].set_type(1);
        test[i].init();
    }
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].wait_thread();
    }
    
    ASSERT_EQ(test[0].test_write_data(), true);

    buff.clear();
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].set_type(2);
        test[i].init();
    }
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].wait_thread();
    }
    
    ASSERT_EQ(test[0].test_write_data(), true);

    buff.clear();
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].set_type(3);
        test[i].init();
    }
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].wait_thread();
    }
    
    ASSERT_EQ(test[0].test_write_data(), true);

    buff.clear();
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].set_type(4);
        test[i].init();
    }
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].wait_thread();
    }
    
    ASSERT_EQ(test[0].test_write_data(), true);

    buff.clear();
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].set_type(5); //测试字符串
        test[i].init();
    }
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].wait_thread();
    }
    
    ASSERT_EQ(test[0].test_write_data(), true);

    buff.clear();
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].set_type(6);// 测试多字节，用结构体代替测试
        test[i].init();
    }
    
    for (int i = 0; i < TEST_THREAD_NUM; ++i) {
        test[i].wait_thread();
    }
    ASSERT_EQ(test[0].test_write_data(), true);

}

TEST_F(ByteBuffer_Test, boundary_test)
{
    ByteBuffer buff(-2);
    ASSERT_EQ(buff.data_size(), 0);
    ASSERT_EQ(buff.empty(), true);
    ASSERT_GE(buff.idle_size(), 1);

    test_stru test;
    buff.write_bytes((void*)&test, sizeof(test));
    ASSERT_EQ(buff.data_size(), sizeof(test));
    ASSERT_EQ(buff.empty(), false);
    
    buff.clear();
    ASSERT_EQ(buff.data_size(), 0);
    ASSERT_EQ(buff.empty(), true);
}

TEST_F(ByteBuffer_Test, copy_test)
{
    ByteBuffer src, dest;
    int start_size = 1000, end_size = 10000;
    for (int i = start_size;i < end_size; i += 100) {
        for (int j = 0;i < i; ++j) {
            src.write_int8(j % 256);
        }
        dest = src;
        ASSERT_EQ(src, dest);
        dest.clear();
        src.clear();
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