#include"header.h"
void initialise(int *dev_id,int *sock)
{
	/****** opening device ******/

	*dev_id = hci_get_route(NULL);
	*sock = hci_open_dev(*dev_id );
	
}
