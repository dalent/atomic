extern long long bigInt=0 ;
static inline long long my_atomic_add64(long long volatile *a, long long v)
{
	  return __sync_fetch_and_add(a, v);
}

void* thread_func(void* param){
	int i =0;
	for( i=0;i<1000000000;i++){
		__sync_fetch_and_add(&bigInt,1);
	}
	return 0;
}
