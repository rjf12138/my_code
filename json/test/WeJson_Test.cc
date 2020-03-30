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
// void print(JsonObject &obj);
// void print(JsonArray &arry);
// void print(JsonArray &arry)
// {
//     for (int i = 0; i < arry.array_val_.size(); ++i) {
//         switch (arry.array_val_[i].type_)
//         {
//             case JSON_OBJECT_TYPE:
//             {
//                 print(arry.array_val_[i].jobject_val_);
//             } break;
//             case JSON_ARRAY_TYPE:
//             {
//                 print(arry.array_val_[i].jarray_val_);
//             } break;
//             case STRING_TYPE:
//             {
//                 std::cout << (string)arry.array_val_[i] << std::endl;
//             } break;
//             case BOOL_TYPE:
//             {
//                 std::cout << (bool)arry.array_val_[i] << std::endl;
//             }break;
//             case NULL_TYPE:
//             {
//                 std::cout << (string)arry.array_val_[i] << std::endl;
//             } break;
//             case NUMBER_TYPE:
//             {
//                 std::cout << (double)arry.array_val_[i] << std::endl;
//             } break;
//         default:
//             break;
//         }
//     }
// }

// void print(JsonObject &obj)
// {
//     for (auto iter = obj.object_val_.begin(); iter != obj.object_val_.end(); ++iter) {
//         switch (iter->second.type_)
//         {
//             case JSON_OBJECT_TYPE:
//             {
//                 print(iter->second.jobject_val_);
//             } break;
//             case JSON_ARRAY_TYPE:
//             {
//                 print(iter->second.jarray_val_);
//             } break;
//             case STRING_TYPE:
//             {
//                 std::cout << (string)iter->second << std::endl;
//             } break;
//             case BOOL_TYPE:
//             {
//                 std::cout << (bool)iter->second << std::endl;
//             }break;
//             case NULL_TYPE:
//             {
//                 std::cout << (string)iter->second << std::endl;
//             } break;
//             case NUMBER_TYPE:
//             {
//                 std::cout << (double)iter->second << std::endl;
//             } break;
//         default:
//             break;
//         }
//     }
// }

TEST_F(WeJson_Test, basic_test) 
{
    WeJson json;
    json.open_json("/home/ruanjian/workspace/project/my_code/build/json/test/bin/json_text/test.json");
    cout << json.generate_to_json() << std::endl;
}

}
}
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}