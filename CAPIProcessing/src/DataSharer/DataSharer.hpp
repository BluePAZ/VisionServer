#ifndef __DATA_SHARER_CPP__
#define __DATA_SHARER_CPP__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unordered_map>
#include <mutex>          // std::mutex, std::lock_guard


struct SharedMemoryContainer{
    uint32_t size;
    uint16_t reference_count;
    void * data_region;
};
class DataSharer{
    public:
        DataSharer * get_instance();
        void release_instance();
        struct SharedMemoryContainer * retrieve_container(uint32_t id);
        uint32_t request_container();
        bool rele6ase_container(uint32_t id);
    private: 
    	std::mutex mutex_;
        static uint32_t id_increment;
        static DataSharer controller;
        static std::unordered_map<uint32_t, struct SharedMemoryContainer *> data_map;
        uint32_t reference_count;
        DataSharer(){
            id_increment = 0;
        }
        ~DataSharer();
};


#endif