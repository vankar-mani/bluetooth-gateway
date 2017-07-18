#include"header.h"
//void push_to_database(char *device_type,char *addr,int rssi,char *mac_id) {
//void push_to_database(char *device_type,struct temp *temp_data,char *mac_id) {
void push_to_database(DEV_INFO_STRUCT *db,int cnt)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "192.168.4.153";
	char *user = "shravya";
	char *password = "123456789"; /* set me first */
	char *database = "SMARTCAMPUS";

	conn = mysql_init(NULL);

/********* Connect to database *************/

	if (!mysql_real_connect(conn, server,
		user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

/************* send SQL query ****************/

	char statement[512];
	int index;
	for(index=0;index<cnt;index++)
	{
//		sprintf(statement,"INSERT INTO BTBLE VALUES ('%s','%s',%d,now(),'%s')",addr,device_type,rssi,mac_id);
		sprintf(statement,"INSERT INTO BTBLE VALUES ('%s','%s',%d,now(),'%s')",(db+index)->dev_id,(db+index)->dev_name,(db+index)->dev_rssi,(db+index)->gateway_id);
			
		if(mysql_query(conn,statement))
		{      
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
	}

	mysql_close(conn);
	return;

}


