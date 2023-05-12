APP_NAME = fileproc
LIB_NAME = libfileproc
TEST_NAME = main_test
INTERFACE_NAME = interface

TESTFLAGS = -I thirdparty
CFLAGS = -Wall -Werror -I src
GLIB = pkg-config --cflags --libs glib-2.0
DEPSFLAGS = -MMD
CC = gcc

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test

APP_PATH = $(BIN_DIR)/$(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a
INTERFACE_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(INTERFACE_NAME)/$(INTERFACE_NAME).a
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

APP_SOURCES = $(wildcard $(SRC_DIR)/$(APP_NAME)/*.c)
APP_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SOURCES))

LIB_SOURCES =  $(wildcard $(SRC_DIR)/$(LIB_NAME)/*.c)
INTERFACE_SOURCES = $(wildcard $(SRC_DIR)/$(INTERFACE_NAME)/*.c)
LIB_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(LIB_SOURCES)) $(patsubst %.c, $(OBJ_DIR)/%.o, $(INTERFACE_SOURCES))

TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(TEST_SOURCES))

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d) $(INTERFACE_OBJECTS:.o=.d)

.PHONY: all test clean
all: $(APP_PATH)

-include $(DEPS)

# BUILD
$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH) $(INTERFACE_PATH)
	$(GLIB) | xargs $(CC) $(CFLAGS) -o $@ $^ -lncursesw

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(INTERFACE_PATH): $(INTERFACE_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	$(GLIB) | xargs $(CC) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $< -lncursesw

# TEST
test: $(LIB_PATH) $(TEST_PATH)
	$(TEST_PATH)

$(TEST_PATH): $(TEST_OBJECTS) $(LIB_PATH)
	$(GLIB) | xargs $(CC) $(TESTFLAGS) $(CFLAGS) $(DEPSFLAGS) -o $@ $^

$(OBJ_DIR)/test/main.o: test/main.c
	$(CC) $(TESTFLAGS) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $<

$(OBJ_DIR)/test/tests.o: test/tests.c
	$(GLIB) | xargs $(CC) $(TESTFLAGS) $(CFLAGS) $(DEPSFLAGS) -c -o $@ $<

# RUN
run: all
	$(APP_PATH)

# CLEAN
clean:
	$(RM) $(APP_PATH) $(TEST_PATH) $(OBJ_DIR)/*/*/*.[aod] $(OBJ_DIR)/test/*.[aod]