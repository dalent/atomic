#include<atomic_ops.h>
#include<stdio.h>
extern size_t  bigInt=0;
void* thread_func(void* param){
	int i =0;
	for( i=0;i<1000000000;i++){
    	AO_fetch_and_add_full(&bigInt,  1);
	}
	return 0;
}
