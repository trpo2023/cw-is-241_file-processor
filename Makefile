APP_NAME = main

TESTFLAGS = -I thirdparty
CFLAGS = -Wall -Werror -I src 
GLIB = pkg-config --cflags --libs glib-2.0
DEPSFLAGS = -MMD
CC = gcc

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)

APP_SOURCES = $(wildcard $(SRC_DIR)/*.c)
APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SOURCES))


DEPS = $(APP_OBJECTS:.o=.d)

.PHONY: all clean
all: $(APP_PATH)

-include $(DEPS)

# BUILD
$(APP_PATH): $(APP_OBJECTS)
	$(GLIB) | xargs $(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	$(GLIB) | xargs $(CC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $< 

# RUN
run: all
	$(APP_PATH)

# CLEAN
clean:
	$(RM) $(APP_PATH) $(OBJ_DIR)/*/*.[aod]