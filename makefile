BUILD_DIR = ./build
OBJS = $(BUILD_DIR)/server.o $(BUILD_DIR)/pool.o $(BUILD_DIR)/clientbuffer.o $(BUILD_DIR)/internet.o $(BUILD_DIR)/httpresponse.o $(BUILD_DIR)/httprequest.o
CC = gcc
INCLUDES = ./include/*
CFLAGS = -c -I ./include -lpthread

$(BUILD_DIR)/pool.o: utils/pool.c include/pool.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/clientbuffer.o: utils/clientbuffer.c include/clientbuffer.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/internet.o: connect/internet.c include/internet.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/httpresponse.o: response/httpresponse.c include/httpresponse.h include/httprequest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/httprequest.o: response/httprequest.c include/httprequest.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/server.o: response/server.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

server: $(OBJS)
	$(CC) $^ -o $@ -lpthread

.PHONY: mkdir clean all

mkdir:
	if [ ! -d $(BUILD_DIR) ]; then\
		mkdir $(BUILD_DIR);\
	fi

build: server

clean:
	if [ -d $(BUILD_DIR) ]; then\
		cd $(BUILD_DIR) && rm -rf ./*;\
	fi

all: mkdir build