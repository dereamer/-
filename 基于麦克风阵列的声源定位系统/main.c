#include <malloc.h>
#include <pthread.h>
#include "collect_data.h"
#include <limits.h>
#include "calculate.h"
#include <signal.h>
//extern pthread_barrier_t b_t;
//extern float timeDiff[3];
extern int flag;
extern int recv_flag;
extern int fd1;
extern int fd2;
pthread_t id_m1,id_m2,id_1;
pthread_barrier_t a_t;
void catch_sigint(int sig_no)
{
    close(fd1);
    close(fd2);
    printf("system exit\n");
    exit(0);
}
int main()
{
	pthread_t id1,id2;
	void *tret;
	fd1=link_to_usb("/dev/ttyUSB0");
	fd2=link_to_usb("/dev/ttyUSB1");
	while(1)
	{
    pthread_create(&id1,NULL,data_process_1_ab,NULL);
    pthread_create(&id2,NULL,data_process_1_cd,NULL);
	pthread_join(id1,&tret);
	pthread_join(id2,&tret);
	data_process();
	pthread_cancel(id1);
	pthread_cancel(id2);
	}
}
