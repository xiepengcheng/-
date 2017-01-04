#ifndef LIST_H
#define LIST_H

#include<list>
#include<mutex>

namespace hodis{

template<typename T>
class lockList
{
	public:
	lockList();
	~lockList();
	
	size_t getSize();
	void putConnection(T t);
	T& getConnect();
	bool empty();
	
	private:
	bool try_lock();
	void unlock();
	
	private:
	std::list<T>list_;
	std::mutex mutex;
};

template<typename T>lockList<T>::lockList()
{
	
}
template<typename T> 
lockList<T>::~lockList() 
{

}
template<typename T>
bool lockList<T>::try_lock() 
{
    if(mutex.try_lock() == true) 
	{
        return true;
    }
	else 
	{
        return false;
    }
}
template<typename T>
void lockList<T>::unlock() 
{
    mutex.unlock();
}
template<typename T>
void lockList<T>::putConnect(T t) 
{
    while(1) {
        if(try_lock() == true) 
		{
            list_.push_back(t);
            unlock();
            return;
        }
    }
}
template<typename T>
T& lockList<T>::getConnect() 
{
    while(1) {
        if(try_lock() == true) 
		{
            auto &t = *list_.begin();
            list_.pop_front();
            unlock();
            return t;
        }
    }
}
template<typename T>
size_t lockList<T>::getSize() 
{
    return list_.size();
}

template<typename T>
bool lockList<T>::empty() 
{
    return list_.empty();
}

};
#endif