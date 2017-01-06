#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

#include <memory>
#include <iostream>
#include <list>
#include <atomic>

#include"list.h"
#define Max_conn 10000

typedef struct Item{
	int fd;
}Item;
namespace hodis
{
	
	class workthread
	{
		public:
		using ItemQueue = std::shared_ptr<hodis::lockList<Item>>;
		using ItemQueueCondition = std::shared_ptr<std::atomic<bool>>;
		workthread(int read_fd, int _id, ItemQueue _item_aq, ItemQueueCondition _item_aq_condition);
		~workthread();
		void run();
		
		private:
		bool work_init();
		bool handle_register_event();
		
		private:
        /* 
         * worker thread
         * responsible for handling the event 
         * */
        std::unique_ptr<pthread_t> worker;//线程标识
        /* item accept queue */
        ItemQueue item_aq;
        ItemQueueCondition item_aq_condition;

        int id;
        int notify_receive_fd;

        /* epoll */
        int epoll_fd = 0;
        struct epoll_event events[Max_conn];
        struct epoll_event ev;
	};
	
	
};
#endif
