CC = gcc
CFLAGS = 
LDFLAGS =

TARGET = http_server
INSTALL_DIR = /opt/app-root/bin

SRC = http_server.c

all: $(TARGET)

http_server.o: $(SRC)
	gcc -c $<

$(TARGET): http_server.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

install: $(TARGET)
	install -m 755 $(TARGET) $(INSTALL_DIR)

clean:
	rm -f $(TARGET) *.o

