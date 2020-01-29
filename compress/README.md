编译在compress目录下执行
make
会在文件夹下生成一个main,test_prog可执行文件

./main -h 查看命令信息


测试程序
将main,test_prog放在同一目录下执行下面命令
./test_prog > /dev/null 2>> err
出错信息会在err文件中
测试会很慢.

