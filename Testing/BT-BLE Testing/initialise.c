#include"header.h"

void initialise(int *dev_id,int *sock)
{
	*dev_id = hci_get_route(NULL);
	*sock = hci_open_dev(*dev_id );

}

