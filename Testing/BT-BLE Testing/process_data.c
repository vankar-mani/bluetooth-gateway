#include"header.h"

void Process_BT_data(int num_resp,inquiry_info *scan_info,int sock,char *mac_id)
{
	int index,rssi;
	char addr[20],name[10],datestr[10],timestr[10];
	DEV_INFO_STRUCT *temp_store;
	temp_store=malloc(sizeof(DEV_INFO_STRUCT));
	printf("resp:%d sock=%d\n",num_resp,sock);
	for(index=0;index<num_resp;index++)
	{
		ba2str(&(scan_info+index)->bdaddr, addr);
		strcpy(temp_store->dev_id,addr);
		strcpy(temp_store->dev_name,"BT");
		strcpy(temp_store->gateway_id,mac_id);
		temp_store->dev_rssi=rssi;
		printf("%s %s %s\n",addr,temp_store->dev_name,temp_store->gateway_id);
	}
	free(scan_info);

}

