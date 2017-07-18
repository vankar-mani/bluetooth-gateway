/* Simple C program that connects to MySQL Database server*/
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void location (void)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char str[250],name[20];

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

	sprintf(str,"select gateway_name from ble_bt_data inner join employ_name_dump right join gateway_name_dump on ble_bt_data.mac_id=employ_name_dump.mac_id where employ_name_dump.name='%s' order by ble_bt_data.time_stamp desc limit 1",name);


	/* send SQL query */
	if (mysql_query(conn,str)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	/* output table name */
	printf("employee %s location:",name);
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s\n", row[0]);

	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
}

