CC = gcc
CFLAGS = 
LDFLAGS =

TARGET = http_server
INSTALL_DIR = /usr/local/bin

SRC = http_server.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

install: $(TARGET)
	install -m 755 $(TARGET) $(INSTALL_DIR)

clean:
	rm -f $(TARGET)

