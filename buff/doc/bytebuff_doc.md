###ByteBuffer
####功能：
1. 以特定格式将数据临时缓冲(int8, int16 int32 int64,string, bytes)。
2. 支持多线程对同一个buff进行读写。
3. 支持迭代器遍历每个字节。

####注意事项：
1.  使用write_bytes写的是包含类成员的结构时，确保该类成员内不含指针
    思考这样一种情况：
    ```
    struct test { string str; } a = {"hello"}, b;
    buff.write_bytes(&a, sizeof(test));
    buff.read_bytes(b, sizeof(test))；
    ```
    a 和 b 相同, a 和 b 内部的指针指向同一个位置，程序结束时会对同一
    个指针释放两次，从而报错double free or corruption (fasttop)
