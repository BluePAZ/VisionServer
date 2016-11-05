#ifndef __WORK_ITEM_H__
#define __WORK_ITEM_H__
//#define _GLIBCXX_USE_CXX11_ABI 0


#include <stdint.h>
#include <string.h>

#include "../WorkTypeDefs.hpp"

class WorkItem{
	
	public:
		WorkItem(){
			params = NULL;
			params_length = 0;
			data = NULL;
			data_length = 0;
			result = NULL;
			result_length = 0;
            completed = 0;
            progress = 0;
            work_descriptor = 0;
            uuid = 0;
    		time_ms = 0;
    		time_ms_2 = 0;

		};
		~WorkItem(){
			freeParameters();
			freeResults();
			freeData();
		};

		volatile uint8_t * params;
		volatile uint32_t params_length;

		volatile uint8_t * data;
		volatile uint32_t data_length;

		volatile uint8_t * result;
		volatile uint32_t result_length;

		volatile uint32_t work_descriptor;
		volatile uint32_t uuid;
        volatile uint8_t progress;
		volatile uint8_t completed;
		volatile uint32_t time_ms;
		volatile uint32_t time_ms_2;

		void setParameters(uint32_t * inputParams, uint32_t numParams){
			freeParameters();
			params = (uint8_t *)malloc(numParams * 4);
			for(uint8_t i = 0; i<numParams; ++i){
				uint32_t tmp = inputParams[i];
				params[i * 4 + 3] = tmp >> 24;
				params[i * 4 + 2] = tmp >> 16;
				params[i * 4 + 1] = tmp >> 8;
				params[i * 4] = tmp;
			}
			params_length = numParams * 4;
		};
		void freeParameters(){
			if(params != NULL){
				free((void *)params);
				params = NULL;
			}
			params_length = 0;
		}
		void setData(uint8_t * data_in, uint32_t length, bool copy = true, bool is_ptr = true, bool is_obj = false, bool is_own = true){
			freeData();
			data_ptr = is_ptr;
			data_obj = is_obj;
			own_data = (is_own || copy);
			if(copy){
				data = (uint8_t *)malloc(length * sizeof(uint8_t));
				memcpy((void *)data, data_in, length);
			}else{
				data = data_in;
			}
			data_length = length;
		}
		void freeData(){
			if(data != NULL){
				if(data_ptr && own_data){
					if(data_obj){
						delete data;
					}else{
						free((void *)data);
					}
					data = NULL;
					data_length = 0;
				}
			}
		}
		void setResults(uint8_t * result_data, uint32_t length, bool copy = true, bool is_ptr = true, bool is_obj = false, bool is_own = true){
			freeResults();
			result_ptr = is_ptr;
			result_obj = is_obj;
			own_result = (is_own || copy);
			if(copy){
				result = (uint8_t *)malloc(length * sizeof(uint8_t));
				memcpy((void *)result, result_data, length);
			}else{
				result = result_data;
			}
			result_length = length;
		}
		void freeResults(){
			if(result != NULL){
				if(result_ptr && own_result){
					if(result_obj){
						delete result;
					}else{
						free((void *)result);
					}
					result = NULL;
					result_length = 0;
				}
			}
		}
		void (*workComplete)();
	private:
		bool data_ptr = false;
		bool result_ptr = false;
		bool own_data = false;
		bool data_obj = false;
		bool result_obj = false;
		bool own_result = false;
};
#endif
