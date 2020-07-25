#include "base/Thread.h"
#include "net/EventLoop.h"
#include "base/Logging.h"
#include "net/TcpClient.h"

using namespace muduo;
using namespace muduo::net;

void threadFunc(EventLoop* loop)
{
	InetAddress serverAddr("127.0.0.1", 1080); // should succeed
	TcpClient client(loop, serverAddr, "TcpClient");
	client.connect();
	/**
	 * 注意连接的过程是非阻塞的，调用了连接的过程之后，链接上的事件都是通过 
	 * eventloop 来进行处理的，所以在 5s 钟之后，这个函数内部的局部变量将会被析构掉，
	 * 不管连接是否成功(实际上这个连接是永远不可能成功的)
	*/
	client.disconnect();
	CurrentThread::sleepUsec(5*1000*1000);
	// client destructs when connected.
}

int main(int argc, char* argv[], char* env[])
{
	Logger::setLogLevel(Logger::DEBUG);
	
	EventLoop loop;
	loop.runAfter(15.0, std::bind(&EventLoop::quit, &loop));
	/**
	 * eventloop 运行 10s 钟的时间
	*/
	Thread thr(std::bind(threadFunc, &loop));
	thr.start();
	loop.loop();

	return 0;
}