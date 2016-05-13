#include<atomic>
std::atomic<long long int> bigInt;
void* thread_func(void* param){
	int i =0;
	for( i=0;i<1000000000;i++){
		bigInt.fetch_add(1);
	}

	return 0;
}
