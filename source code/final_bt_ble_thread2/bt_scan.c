#include"header.h"
inquiry_info *bt_scan(int dev_id,int *num_resp)
{	
	int flags,len=8;
	inquiry_info *scaned_device_struct = NULL;
	flags = IREQ_CACHE_FLUSH;
	scaned_device_struct = (inquiry_info*)malloc(MAX_RESP * sizeof(inquiry_info));
	puts("bt scanning");
	
	/********** scanning for BT devices *********/
	
	*num_resp = hci_inquiry(dev_id, len, MAX_RESP, NULL, &scaned_device_struct, flags);////returns no.of devices found
	if( *num_resp < 0 ) perror("hci_inquiry");
	if(*num_resp>0)
		 return scaned_device_struct;
	else
		return NULL;
}
