#include"item.h"

namespace hodis{
	
item::item(char* start){
    time_t timep;

    value = GET_VALUE_ADDRESS(start);
    timestamp = time(&timep);
    pslab = nullptr;
    expiretime = 0;
}

item::~item(){

}

void
item::set_value()//设置VALUE
{

}

char*
item::get_value(){
    return value;
}
void
item::set_expiretime(time_t time)
{
    expiretime = time;
}

time_t
item::get_expiretime(){
    return expiretime;
}

void
item::set_slab_point(void* p){
    pslab = p;
}	
void*
item::get_slab_point(){
    return pslab;
}

};