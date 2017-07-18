PROJECT := rs

OBJS := bt_initialise.o bt_scan.o main_thread.o read_rssi.o rssi_ndevice_info.o local_time.o ble_scan.o db1.o find_mac.o

all: $(PROJECT)

CFLAGS := -Wall -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/mysql -g #-Werror

LDFLAGS := -lglib-2.0 -lgio-2.0 -lbluetooth -lpthread -lmysqlclient -g

$(PROJECT): $(OBJS)
	$(CC) $(OBJS)  $(LDFLAGS) -o $@

%.o: %.c Makefile $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o


