CC := g++
CXXFLAGS := -Wall -fpic -std=c++11 -MMD -MP
LDFLAGS      = -shared
LIBS := -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lhdf5\
		  -lboost_system -lboost_filesystem

SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_DIR := ./bin

TARGET := $(TARGET_DIR)/gl_framework.so
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(filter-out $(MAINS), $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES)))

all: shared

shared: $(OBJ_FILES)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(LIBS) $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm $(BUILD_DIR)/*.o

-include $(OBJ_FILES:.o=.d)