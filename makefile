BUILD_DIR = ./build
OBJS =  $(BUILD_DIR)/server.o $(BUILD_DIR)/pool.o $(BUILD_DIR)/clientbuffer.o				\
		$(BUILD_DIR)/internet.o $(BUILD_DIR)/httpresponse.o $(BUILD_DIR)/httprequest.o		\
		$(BUILD_DIR)/bufferedreader.o $(BUILD_DIR)/keyvaluelist.o $(BUILD_DIR)/router.o		\
		$(BUILD_DIR)/bufferedwriter.o
SHARED_OBJS =	$(BUILD_DIR)/keyvaluelist.o $(BUILD_DIR)/router.o		\
				$(BUILD_DIR)/bufferedreader.o $(BUILD_DIR)/bufferedwriter.o	\
				$(BUILD_DIR)/httpresponse.o $(BUILD_DIR)/httprequest.o

SHARED_CFILES =	utils/keyvaluelist.c utils/router.c				\
				utils/bufferedreader.c utils/bufferedwriter.c	\
				response/httpresponse.c response/httprequest.c

CC = gcc
INCLUDES = ./include/* 
CFLAGS = -c -I ./include
SOFLAGS = -shared -fPIC -I ./include
LDFLAGS =  -lpthread -ldl

$(BUILD_DIR)/internet.o: connect/internet.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/server.o: response/server.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/httpresponse.o: response/httpresponse.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/httprequest.o: response/httprequest.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/pool.o: utils/pool.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/clientbuffer.o: utils/clientbuffer.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/bufferedreader.o: utils/bufferedreader.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/bufferedwriter.o: utils/bufferedwriter.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyvaluelist.o: utils/keyvaluelist.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/router.o: utils/router.c include/router.h $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

http.so: $(SHARED_CFILES)
	$(CC) $(SOFLAGS) $^ -o $@ $(LDFLAGS)

server: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY: mkdir clean all so

mkdir:
	if [ ! -d $(BUILD_DIR) ]; then\
		mkdir $(BUILD_DIR);\
	fi

build: server

so: http.so

clean:
	if [ -d $(BUILD_DIR) ]; then\
		cd $(BUILD_DIR) && rm -rf ./*;\
	fi

all: mkdir build