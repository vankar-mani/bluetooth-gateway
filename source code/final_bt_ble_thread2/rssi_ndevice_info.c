#include"header.h"
void Process_BT_data(int num_resp,inquiry_info *scaned_data,int sock,char *mac_id)
{
	int index,rssi;
	char addr[20],name[10],datestr[10],timestr[10];
	DEV_INFO_STRUCT *temp_store;
	temp_store=malloc(sizeof(DEV_INFO_STRUCT));
	printf("resp:%d sock=%d\n",num_resp,sock);

//********* parsing data ******/
	for(index=0;index<num_resp;index++)
	{
		ba2str(&(scaned_data+index)->bdaddr, addr);
//		memset(name, 0, sizeof(name));
//		if (hci_read_remote_name(sock, &(scaned_data+index)->bdaddr, sizeof(name),name, 1000) < 0)
//			strcpy(name, "[unknown]");

	/*********** reading rssi ***********/

//		rssi=read_rssi(addr);
		strcpy(temp_store->dev_id,addr);
		strcpy(temp_store->dev_name,"BT");
		strcpy(temp_store->gateway_id,mac_id);
		//temp_store->dev_rssi=rssi;
		temp_store->dev_rssi=0;
	//	push_to_database("BT",addr,rssi,mac_id);
		push_to_database(temp_store,1);
		
		//printf("%s %s %s rssi:%d\n",datestr,timestr,addr,rssi);
	}
	free(scaned_data);				

}
