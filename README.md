# chatserver
集群聊天服务器和客户端源码，技术栈c++、muduo网络库、MySQL、nginx、Redis等

编译方式
cd build
rm -rf *
cmake ..
make

运行方式
需要配置nginx的TCP负载均衡，启动Mysql和Redis
cd bin
./ChatServer 127.0.0.1 6000
./ChatClient 127.0.0.1 8000
