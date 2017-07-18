#include"header.h"
pthread_t tid1,tid2,tid3;
inquiry_info *scaned_data;
int sock,dev_id,num_resp;
const char mac_id[20];

int abort_fun()
{
	int ch;	
	fcntl(ch,F_SETFL,O_NONBLOCK);
	ch=getchar();
	if(ch!=EOF)
		return 1;
	return 0;

}

void *bt_cal(void *p)
{
	while(!abort_fun())
	{
		scaned_data=bt_scan(dev_id,&num_resp);
		if(scaned_data!=NULL)
		{
			Process_BT_data(num_resp,scaned_data,sock,mac_id);
		}
	}
	pthread_exit(NULL);
	puts("BT exited");
}

void *ble_cal(void *p)
{
	ble(sock,mac_id);
	pthread_exit(NULL);

}

void mainfun(void)
{

	initialise(&dev_id,&sock);
	if (dev_id < 0 || sock < 0)
	{

		perror("opening socket");
		exit(1);
	}
	printf("Test1 : BT Initialization sucessful\n");
	pthread_create(&tid1,NULL,bt_cal,NULL);
	pthread_create(&tid2,NULL,ble_cal,NULL);
	pthread_join(tid1,NULL);
	printf("Test2 : BT scanning sucessful\n");
	pthread_join(tid2,NULL);
	printf("Test3 : BLE is sucessful\n");
	puts("EXITING");
}
