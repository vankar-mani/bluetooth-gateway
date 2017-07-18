#include"header.h"
void find_mac_id(char *mac)
{
/******* reding mac_id of gateway *******/

	FILE *fp;
	fp=fopen("/sys/class/net/eth0/address","r");
	fscanf(fp,"%s",mac);
	fclose(fp);

}
