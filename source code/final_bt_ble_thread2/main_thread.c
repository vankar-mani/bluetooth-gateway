#include"header.h"
pthread_t tid1,tid2,tid3;
inquiry_info *scaned_data;
int sock,dev_id,num_resp;
const char mac_id[20];

/////********abort BT,BLE scanning *******/////

int abort_fun()
{
	int ch;	
	fcntl(ch,F_SETFL,O_NONBLOCK);
	ch=getchar();
	if(ch!=EOF)
		return 1;
	return 0;

}

//********BT thread *******/////

void *bt_cal(void *p)
{
		while(1)
//		while(!abort_fun())
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

//******BLE thread *********/////

void *ble_cal(void *p)
{
	ble(sock,mac_id);
	pthread_exit(NULL);
}

////********* main *******////

int main()
{
	
	initialise(&dev_id,&sock);  //////////initialising device

	find_mac_id(mac_id); //////find mac of gateway

	if (dev_id < 0 || sock < 0)
        {
                perror("opening socket");
                exit(1);
        }

/////******* creating threads for BT,BLE **********/////

	pthread_create(&tid1,NULL,bt_cal,NULL);
	pthread_create(&tid2,NULL,ble_cal,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	puts("EXITING");
}
