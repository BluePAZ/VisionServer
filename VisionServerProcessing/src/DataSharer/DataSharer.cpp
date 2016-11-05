#include "DataSharer.hpp"


DataSharer * DataSharer::get_instance(){
    reference_count++;
    return &controller;
}
void DataSharer::release_instance(){
    reference_count--;
}
struct SharedMemoryContainer * DataSharer::retrieve_container(uint32_t id){
    if(data_map.count(id) > 0){
		return data_map[id];
	}else{
        return NULL;
    }
}
uint32_t DataSharer::request_container(){
    std::unique_lock<std::mutex> mlock(mutex_);
    while(data_map.count(id_increment++) > 0){ 
        //find an open slot
    }
    uint32_t ret_val = id_increment;
    data_map[ret_val] = (struct SharedMemoryContainer *)malloc(sizeof(struct SharedMemoryContainer));
    return ret_val;
}
bool DataSharer::release_container(uint32_t id){
    std::unique_lock<std::mutex> mlock(mutex_);
    if(data_map.count(id) > 0){
        struct SharedMemoryContainer * cont = data_map[id];
        if(cont->data_region != NULL){
            free(cont->data_region);
            cont->data_region = NULL;
        }
        free(cont);
        cont = NULL;
        data_map.erase(id);
        return true;
	}else{
        return false;
    }
}