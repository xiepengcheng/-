#include"slab.h"

namespace hodis{
	
	slab::slab(uint64_t _slab_size, uint64_t _slab_init, uint64_t _id):
	slab_size(_slab_size), item_size(slab_init), id(_id)
	{
		perslab=slab_size/item_size;
		std::cout<<"id: "<<id<<" item number: "<<perslab<<std::endl;
		free_count=perslab;
		alloc_count=0;
		
		start = (char*)malloc(slab_size);
		if(start == nullptr)
		{
			fprintf(stderr, "Warning: Failed to allocate requested memory in"
										" one large chunk.\nWill allocate in smaller chunks\n");
		}
		
		auto deleter = [](item* p){}
		
		for(int i = 0; i < perslab; ++i){
        item* iptr = reinterpret_cast<item*>(start+i*item_size);
        std::shared_ptr<item> tpi(new (iptr) item(start+(i*item_size)), deleter);
        tpi->set_slab_point(this);
        freeitem.push_back(std::move(tpi));
	}
	
	slab::~slab()
	{
    if(start != nullptr)
		{
			free(start);
		}
    }
	
	std::shared_ptr<item> slab::alloc_item()
	{
		std::cout << "free list size:" << freeitem.size() << std::endl;
		std::cout << "alloc list size:" << allocitem.size() << std::endl;
		std::lock_guard<std::mutex> locker(mutex);
		if(!freeitem.empty())
		{
			auto item = *(--freeitem.end());
			freeitem.pop_back();
			--free_count;
			/*
			 *  move in LRU list 
			 *  */
			allocitem.push_back(item);
			++alloc_count;
			return item;
		}
		else//freeitem 为空，需要LRU算法获取item
		{
			return nullptr;
		}
	}
	
void slab::free_item(const std::shared_ptr<item> &fitem)
{
    std::lock_guard<std::mutex> locker(mutex);
    freeitem.push_back(fitem);
    ++free_count;
    for(auto iter = allocitem.begin(); iter != allocitem.end(); ++iter)
	{
        if(*iter == fitem){
            iter = allocitem.erase(iter);
            --alloc_count;
            break;
        }
    }
}

uint64_t
slab::get_item_size()
{
    return item_size;
}

void
slab::see()
{
    std::cout << "free item" << std::endl;
    for(auto &p : freeitem)
	{
        std::cout << p->get_expiretime() << " ";
    }

    std::cout << "alloc item" << std::endl;
    for(auto &p : allocitem)
	{
        std::cout << p->get_expiretime() << " ";
    }
}

bool 
slab::trylock() {
    if(mutex.try_lock() == false) 
	{
        return false;
    }
    return true;
}

void
slab::unlock() 
{
    mutex.unlock();
}
	
};