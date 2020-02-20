#ifndef __ERROR_H__
#define __ERROR_H__

#define STR_UNKNOWN_ERROR      "unknown errror"

enum ERROR_BYTE_BUFF 
{
    BYTE_BUFF_SUCCESS = 0,
    BYTE_BUFF_EMPTY = -1000,
    BYTE_BUFF_FULL = -1001,
    BYTE_BUFF_COPY_DATA_FROM_BUFFER_FAILED = -1002,
    BYTE_BUFF_COPY_DATA_TO_BUFFER_FAILED = -1003,
    BYTE_BUFF_STR_READ_FAILED = -1004,
    BYTE_BUFF_OUTPUT_BUFFER_IS_NULL = -1005,
    BYTE_BUFF_OUT_OF_RANGE = -1006,
    BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH = -1007,
    BYTE_BUFF_CANT_FIND_STRING = -1008
};

#define STR_BYTE_BUFF_SUCCESS                       "byte_buff success"
#define STR_BYTE_BUFF_EMPTY                         "byte_buff is empty"
#define STR_BYTE_BUFF_FULL                          "byte_buff is full"
#define STR_BYTE_BUFF_COPY_DATA_FROM_BUFFER_FAILED  "byte_buff copy data from buff failed"
#define STR_BYTE_BUFF_COPY_DATA_TO_BUFFER_FAILED    "byte_buff copy data to buff failed"
#define STR_BYTE_BUFF_STR_READ_FAILED               "byte_buff str read failed"
#define STR_BYTE_BUFF_OUTPUT_BUFFER_IS_NULL         "output buff is NULL"
#define STR_BYTE_BUFF_OUT_OF_RANGE                  "out of range"
#define STR_BYTE_BUFF_REMAIN_DATA_NOT_ENOUGH        "remain data is not enough"
#define STR_BYTE_BUFF_CANT_FIND_STRING              "cant find string"

#endif