#include"header.h"
volatile int flag=0;
static short
get_rssi(const char *bt_address);

int read_rssi (const char *argv)
{
	short rssi = 0;
	rssi = get_rssi(argv);
	puts("BLUETOOTH:");
	return rssi;

}

/*******desables rssi scan ******/

void desable_rssi_scan(int signum)
{
	flag=1;		
}	

/*
 * Callback function for the DeviceFound signal from the org.bluez.Adapter
 * interface.
 */
static void
device_found_handler (GDBusConnection *connection,
                        const gchar *sender_name,
                        const gchar *object_path,
                        const gchar *interface_name,
                        const gchar *signal_name,
                        GVariant *parameters,
                        gpointer user_data)
{
	char *device_address;
	gboolean res;
	short rssi;
	GVariant *property_dict;
	struct handler_data *data = (struct handler_data *)user_data;
	if(flag==1)
	{
		flag=0;
		data->rssi=0;/////making rssi 0 if the device take more than 10 sec to scan
		g_main_loop_quit(main_loop);

	}
  /*
   * Paramter format: sa{sv}
   * Only interested in the RSSI so lookup that entry in the properties
   * dictionary.
   */
	g_variant_get(parameters, "(&s*)", &device_address, &property_dict);

	if (strcmp(data->bt_address, device_address))
	{

	/******** Not the device of interest ********/

		return;
	}

	res = g_variant_lookup(property_dict, "RSSI", "n",&rssi);
	if (!res)
	{
		printf("Unable to get device address from dbus\n");
		g_main_loop_quit(main_loop);
		return;
	}
  
	data->rssi = rssi;
	g_main_loop_quit(main_loop);
}

/*
 * Gets the RSSI for a given BT device address.
 */
static short
get_rssi(const char *bt_address)
{


	alarm(10);  //////scanes only for 10 sec
	signal(SIGALRM,desable_rssi_scan);
	GError *error = NULL;
	GVariant *reply = NULL;
	char *adapter_object = NULL;
	struct handler_data data;
  
	data.bt_address = bt_address;

	main_loop = g_main_loop_new(NULL, FALSE);
	if (!main_loop)
	{
		printf("Error creating main loop\n");
		return 0;
	}

	dbus_conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
	if (error)
	{
		printf("Unable to get dbus connection\n");
		return 0;
	}

/****** Get the default BT adapter. Needed to start device discovery *******/

	reply = g_dbus_connection_call_sync(dbus_conn,
				      BLUEZ_BUS_NAME,
				      "/",
				      "org.bluez.Manager",
				      "DefaultAdapter",
				      NULL, 
				      G_VARIANT_TYPE("(o)"),
				      G_DBUS_CALL_FLAGS_NONE,
				      -1,
				      NULL,
				      &error);

	if (error)
	{
		printf("Unable to get managed objects: %s\n", error->message);
		return 0;
	}
	g_variant_get(reply, "(&o)", &adapter_object);

/******* Register a handler for DeviceFound signals to read the device RSSI *********/

	g_dbus_connection_signal_subscribe(dbus_conn,
				     NULL,
				     "org.bluez.Adapter",
				     "DeviceFound",
				     NULL,
				     NULL,
				     0,
				     device_found_handler,
				     &data,
				     NULL);

/******** Start device discovery ********/

	reply = g_dbus_connection_call_sync(dbus_conn,
				     BLUEZ_BUS_NAME,
				     adapter_object,
				     "org.bluez.Adapter",
				     "StartDiscovery",
				     NULL,
				     NULL,
				     G_DBUS_CALL_FLAGS_NONE,
				     -1,
				     NULL,
				     &error);

	if (error)
	{
		printf("Unable to start discovery: %s\n", error->message);
		return 0;
	}

	g_main_loop_run(main_loop);

	if(data.rssi!=0)
		alarm(0);/////cancelling alarm

	return data.rssi;
}
