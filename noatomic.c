extern long long bigInt = 0 ;

void* thread_func(void* param){
	int i =0;
	for( i=0;i<1000000000;i++){
		bigInt++;
	}
	return NULL;
}
