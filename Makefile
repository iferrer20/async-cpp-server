TARGET    := server

SRC_DIR   := src
OBJ_DIR   := build

SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.d,$(SRC_FILES))

SRC_SUBDIRS := $(shell find $(SRC_DIR) -type d)
OBJ_SUBDIRS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SRC_SUBDIRS))

CPPFLAGS += -O2 -Wall

LDFLAGS += 

ifeq ($(USE_SSL),1)
	LDFLAGS += -lwolfssl -Wl,-rpath /usr/local/lib/
	CPPFLAGS += -DUSE_SSL
endif

.PHONY: all clean

all: $(TARGET)
segf: CPPFLAGS += -Og -g -fsanitize=address
segf: LDFLAGS += -Og -g -fsanitize=address
segf: $(TARGET)

$(TARGET): $(OBJ_SUBDIRS) $(OBJ_FILES)
	g++ -o $@ $(OBJ_FILES) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -MMD -MP $(CPPFLAGS) -c $< -o $@

$(OBJ_SUBDIRS):
	mkdir -p $@

clean:
	rm -rf $(TARGET) $(OBJ_SUBDIRS)
	
-include $(DEP_FILES)
