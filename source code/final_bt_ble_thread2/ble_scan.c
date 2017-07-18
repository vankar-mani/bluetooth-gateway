#include"header.h"
int device_cnt=0;
int db_flag=0;
DEV_INFO_STRUCT *dev_store=NULL;
struct hci_request ble_hci_request(uint16_t ocf, int clen, void * status, void * cparam);
void sleep_thread(void *p);
pthread_t tid3;
void ble(int device,char *mac_id)
{
	int ret, status;
	char name[30]={0};
	if ( device < 0 ) { 
		perror("Failed to open HCI device.");
		return; 
	}

	/************ Set BLE scan parameters ************/
	
	le_set_scan_parameters_cp scan_params_cp;
	memset(&scan_params_cp, 0, sizeof(scan_params_cp));
	scan_params_cp.type 			= 0x00; 
	scan_params_cp.interval 		= htobs(0x0010);
	scan_params_cp.window 			= htobs(0x0010);
	scan_params_cp.own_bdaddr_type 	= 0x00; // Public Device Address (default).
	scan_params_cp.filter 			= 0x00; // Accept all.

	struct hci_request scan_params_rq = ble_hci_request(OCF_LE_SET_SCAN_PARAMETERS, LE_SET_SCAN_PARAMETERS_CP_SIZE, &status, &scan_params_cp);
	
	ret = hci_send_req(device, &scan_params_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to set scan parameters data.");
		return;
	}

	/********* Set BLE events report mask**********/

	le_set_event_mask_cp event_mask_cp;
	memset(&event_mask_cp, 0, sizeof(le_set_event_mask_cp));
	int i = 0;
	for ( i = 0 ; i < 8 ; i++ ) event_mask_cp.mask[i] = 0xFF;

	struct hci_request set_mask_rq = ble_hci_request(OCF_LE_SET_EVENT_MASK, LE_SET_EVENT_MASK_CP_SIZE, &status, &event_mask_cp);
	ret = hci_send_req(device, &set_mask_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to set event mask.");
		return;
	}

	/******** Enable scanning*********/

	le_set_scan_enable_cp scan_cp;
	memset(&scan_cp, 0, sizeof(scan_cp));
	scan_cp.enable 		= 0x01;	// Enable flag.
	scan_cp.filter_dup 	= 0x00; // Filtering disabled.

	struct hci_request enable_adv_rq = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, &status, &scan_cp);

	ret = hci_send_req(device, &enable_adv_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to enable scan.");
		return;
	}

	// Get Results.

	struct hci_filter nf;
	hci_filter_clear(&nf);
	hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
	hci_filter_set_event(EVT_LE_META_EVENT, &nf);
	if ( setsockopt(device, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0 ) {
		hci_close_dev(device);
		perror("Could not set socket options\n");
		return;
	}

	printf("BT Scanning....\n");

	uint8_t buf[HCI_MAX_EVENT_SIZE];
	evt_le_meta_event * meta_event;
	le_advertising_info * info;
	int len;

//	while (!abort_fun() ) {
	while (1 ) {
//		sleep(2);
		puts("BLE:");
		len = read(device, buf, sizeof(buf));
		if ( len >= HCI_EVENT_HDR_SIZE )
		{
			meta_event = (evt_le_meta_event*)(buf+HCI_EVENT_HDR_SIZE+1);
			if ( meta_event->subevent == EVT_LE_ADVERTISING_REPORT )
			{
				uint8_t reports_count = meta_event->data[0];
				void * offset = meta_event->data + 1;
				while ( reports_count-- )
				{
					info = (le_advertising_info *)offset;
					char addr[18];
					ba2str(&(info->bdaddr), addr);
					

//        if (hci_read_remote_name(device, &(info->bdaddr), sizeof(name),name, 0) < 0)
//        strcpy(name, "[unknown]");
					

					
/****************scanning for 10 sec and pushing to db***********************/

					dev_store=realloc(dev_store,(device_cnt+1)*sizeof(DEV_INFO_STRUCT));
					if(dev_store==NULL)
						puts("error");
					strcpy((dev_store+device_cnt)->dev_id,addr);
					strcpy((dev_store+device_cnt)->dev_name,"BLE");
					strcpy((dev_store+device_cnt)->gateway_id,mac_id);
					(dev_store+device_cnt)->dev_rssi=(char)info->data[info->length];
					device_cnt++;
//					printf("%s,%d,%d\n",addr,(dev_store+cnt)->dev_rssi,(char)info->data[info->length]);
					if(db_flag==0)
					{
///////					
						puts("thread created");
						pthread_create(&tid3,NULL,sleep_thread,NULL);
					}
//				push_to_database("BLE",addr,(char)info->data[info->length],mac_id);
						
					offset = info->data + info->length + 2;
				}
			}
		}
	}
	puts("BLE exiting");
	// Disable scanning.

	memset(&scan_cp, 0, sizeof(scan_cp));
	scan_cp.enable = 0x00;	// Disable flag.

	struct hci_request disable_adv_rq = ble_hci_request(OCF_LE_SET_SCAN_ENABLE, LE_SET_SCAN_ENABLE_CP_SIZE, &status, &scan_cp);
	ret = hci_send_req(device, &disable_adv_rq, 1000);
	if ( ret < 0 ) {
		hci_close_dev(device);
		perror("Failed to disable scan.");
		return;
	}

	hci_close_dev(device);
	
	return;
}
struct hci_request ble_hci_request(uint16_t ocf, int clen, void * status, void * cparam)
{
	struct hci_request rq;
	memset(&rq, 0, sizeof(rq));
	rq.ogf = OGF_LE_CTL;
	rq.ocf = ocf;
	rq.cparam = cparam;
	rq.clen = clen;
	rq.rparam = status;
	rq.rlen = 1;
	return rq;

}

/*********wait for 10 sec to read data and push to db************/

void sleep_thread(void *p)
{
	db_flag=1;
	sleep(10);
	push_to_database(dev_store,device_cnt);
	free(dev_store);
	dev_store=NULL;
	device_cnt=0;
	db_flag=0;
	pthread_exit(NULL);

}
