/*
    服务器类，基于muduo库开发
*/
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

class ChatServer
{
public:
    // TcpServer初始化
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // IP+Port
               const string &nameArg)         // 服务器名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));

        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));

        // 设置服务器端的线程数量 1个I/O线程，3个worker线程
        _server.setThreadNum(4);
    }

    // 开启事件循环
    void start()
    {
        _server.start();
    }

private:
    // TcpServer绑定的回调函数，当有新连接或者连接中断时调用
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:offline" << endl;
            conn->shutdown(); // close(fd)
            //_loop->quit();
        }
    }
    // TcpServer绑定的回调函数，当有新数据时调用
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time)               // 接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        conn->send(buf);
    }

private:
    TcpServer _server;
    EventLoop *_loop;
};

int main()
{
    EventLoop loop; // epoll
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); // listenfd expoll ctl=>expoll
    loop.loop();    // expoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等
    return 0;
}