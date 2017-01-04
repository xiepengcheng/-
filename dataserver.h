#ifndef DATASERVER_H
#define DATASERVER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <sys/eventfd.h>
#include <pthread.h>

#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include <fstream>
#include <sstream>
#include <map>
#include <list>

#define Max_conn 10000
#define BackLog  1024

namespace hodis{
	
class dataserver
{
	public:
	using ParaMap = std::map<std::string, std::string>;
    using WorkerThreadGroup = std::vector<std::unique_ptr<hodis::workthread>>;
    using WorkerItemAQ = std::unique_ptr<std::vector<std::shared_ptr<hodis::lockList<Item>>>>;
    using WorkerAQCondition = std::unique_ptr<std::vector<std::shared_ptr<std::atomic<bool>>>>;
    using WorkerEvent = std::vector<uint64_t>;
    using WorkerEventFd = std::vector<int>;
	
	using ParaMap = std::map<std::string, std::string>;
	dataserver() = delete;
	dataserver(std::ifstream &in);
	~dataserver();
	void run();
	
	private:
	int setnonblocking(int fd);
	bool event_init();
	bool work_init();
	bool master_init(ParaMap& para);
	bool analyse_parameter(std::ifstream& in, ParaMap& para);
	bool register_worker(int fd);
	int accept_connect();
	
	private:
	int thread_num;
	WorkerThreadGroup work_thread_group;
	WorkerEvent event;
    WorkerEventFd eventfds;
	
	int port;
	std::string ip;
	int listen_fd;
	int epoll_fd;
	struct sockaddr_in server_addr;
	struct epoll_event events[Max_conn];
	struct epoll_event ev;
};

};
#endif