#ifndef __ERROR_H__
#define __ERROR_H__

#define STR_UNKNOWN_ERROR      "unknown errror"

enum ERROR_BYTE_BUFF 
{
    BYTE_BUFF_SUCCESS = 0,
    BYTE_BUFF_EMPTY = -1000,
    BYTE_BUFF_FULL = -1001,
    BYTE_BUFF_STR_READ_FAILED = -1004,
    BYTE_BUFF_OUTPUT_BUFFER_IS_NULL = -1005,
    BYTE_BUFF_OUT_OF_RANGE = -1006,
    BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH = -1007,
    BYTE_BUFF_CANT_FIND_STRING = -1008,
    BYTE_BUFF_RUN_OUT_OF_MEMORY = -1009,
};

#define STR_BYTE_BUFF_SUCCESS                       "byte_buff success"
#define STR_BYTE_BUFF_EMPTY                         "byte_buff is empty"
#define STR_BYTE_BUFF_FULL                          "byte_buff is full"
#define STR_BYTE_BUFF_STR_READ_FAILED               "byte_buff str read failed"
#define STR_BYTE_BUFF_OUTPUT_BUFFER_IS_NULL         "output buff is NULL"
#define STR_BYTE_BUFF_OUT_OF_RANGE                  "out of range"
#define STR_BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH        "remain data is not enough"
#define STR_BYTE_BUFF_CANT_FIND_STRING              "cant find string"
#define STR_BYTE_RUN_OUT_OF_MEMORY                  "run out of memory"

#endif