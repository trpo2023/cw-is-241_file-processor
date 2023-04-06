APP_NAME = main
LIB_NAME = libfileproc

TESTFLAGS = -I thirdparty
CFLAGS = -Wall -Werror -I src
GLIB = pkg-config --cflags --libs glib-2.0
DEPSFLAGS = -MMD
CC = gcc

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a

APP_SOURCES = $(wildcard $(SRC_DIR)/fileproc/*.c)
APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SOURCES))

LIB_SOURCES =  $(wildcard $(SRC_DIR)/$(LIB_NAME)/*.c)
LIB_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_SOURCES))

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d)

.PHONY: all clean
all: $(APP_PATH)

-include $(DEPS)

# BUILD
$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(GLIB) | xargs $(CC) $(CFLAGS) -o $@ $^

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(GLIB) | xargs $(CC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $< 

# RUN
run: all
	$(APP_PATH)

# CLEAN
clean:
	$(RM) $(APP_PATH)  $(OBJ_DIR)/*/*/*.[aod] $(OBJ_DIR)/*/*.[aod]