ByteBuffer:
1. 测试数据增长时，ByteBuffer 的表现
2. 测试在使用迭代器时， ByteBuffer 的特性
3. 为类内的每一个函数提供一个单元测试
4. 当内部的 vector 重新扩充时，对于 ByteBuffer 内部指针的影响


vector 内存策略：
c.size()            c中有多少元素
c.shrink_to_fit()   将 capacity() 减少为与 size() 相同大小
c.capacity()        不重新分配内存空间的话，c可以保存多少元素
c.reserve(n)        分配至少能容纳n个元素的内存空间