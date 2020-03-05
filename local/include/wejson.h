#ifndef __WEJSON_H__
#define __WEJSON_H__

#include "basic_head.h"
//#include "byte_buffer.h"
//#include "file.h"

namespace my_util {

struct value_type {
    int type_;
    int int_val_;
    bool bool_val_;
    string str_val_;
};

template<typename T> T
ret_value(value_type val)
{
    if (val.type_ == 0) {
        std::cout << "val.int_val_" << std::endl;
        return val.int_val_;
    }
    // } else if (val.type_ == 1) {
    //     std::cout << "val.bool_val_" << std::endl;
    //     return val.bool_val_;
    // } else if (val.type_ == 2) {
    //     std::cout << "val.str_val_" << std::endl;
    //     return val.str_val_;
    // }
    // switch (val.type_) {
    //     case 0: 
    //     {
    //         std::cout << "val.int_val_" << std::endl;
    //         return val.int_val_;
    //     } break;
    //     case 1:
    //     {
    //         std::cout << "val.bool_val_" << std::endl;
    //         return val.bool_val_;
    //     } break;
    //     case 2:
    //     {
    //         std::cout << "val.str_val_" << std::endl;
    //         return val.str_val_;
    //     } break;
    //     default:
    //         return 0;
    // }
}

}

#endif