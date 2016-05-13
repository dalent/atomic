#include<iostream>
#include<pthread.h>
#include<unistd.h>
//#include"built_in.c"
//#include"noatomic.c"
//#include"atomic.cpp"

#include"libatomic.c"
	
int main(){
	int numThread = 15;
	pthread_t  tinfo[15];
	std::cout<<bigInt<<"sizeof"<< sizeof(bigInt)<<std::endl;
	int i=0;
	for(i=0;i< numThread;i++){
		pthread_create(&tinfo[i],NULL,&thread_func,NULL);
	}

	
	int status=0;
	for(i=0;i<numThread;i++){
		status = pthread_join(tinfo[i],NULL);
		if(0 != status){
			printf("status:%d",status);
			continue;
		}
	}

	std::cout<< bigInt<<std::endl;
	return 0;
}
