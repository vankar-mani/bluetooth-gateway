/* Simple C program that connects to MySQL Database server*/
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void outtime(void)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char in[200],out[200],name[20],intime[20],outtime[20];

	char *server = "192.168.4.167";
	char *user = "venkey";
	char *password = "venkey"; /* set me first */
	char *database = "smart_campus";

	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server,
				user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	printf("Enter the employ name:");
	scanf("%s",name);



	sprintf(out,"select time_stamp from ble_bt_data inner join employ_name_dump on ble_bt_data.mac_id=employ_name_dump.mac_id where employ_name_dump.name='%s' order by ble_bt_data.time_stamp desc limit 1",name);		

	/* send SQL query */
	if (mysql_query(conn,out)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	/* output table name */
	printf("employee:%s ---outtime:",name);
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s\n",row[0]);
	}
	
		/* close connection */
		mysql_free_result(res);
		mysql_close(conn);
}
