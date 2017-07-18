/* Simple C program that connects to MySQL Database server*/
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void no_employs(void)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char str[250],name[20];

	char *server = "192.168.4.167";
	char *user = "venkey";
	char *password = "venkey"; /* set me first */
	char *database = "smart_campus";
	int count=0;
	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server,
				user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	printf("Enter the gateway name:");
	scanf("%s",name);

	sprintf(str,"select distinct mac_id from ble_bt_data inner join gateway_name_dump on ble_bt_data.gateway_id=gateway_name_dump.gateway_id where gateway_name_dump.gateway_name='%s floor' and ble_bt_data.time_stamp > now()-20",name);	

	printf("\nstring --%s\n",str);

	/* send SQL query */
	if (mysql_query(conn,str)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	/* output table name */
	printf("data from two taables\n\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{ 
		printf("%s\n", row[0]);
		count++;
	}	
	printf("number of employees at %s---%d\n",name,count);

	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
}

