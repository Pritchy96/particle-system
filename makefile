CC := g++
CXXSHAREDFLAGS := -Wall -fPIC -std=c++11 -MMD -MP
CXXFLAGS := -Wall -std=c++11 -g

LDFLAGS      = -shared
LIBS := -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lhdf5\
		  -lboost_system -lboost_filesystem

SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_DIR := ./bin

TARGET := $(TARGET_DIR)/gl_framework.so
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(filter-out $(MAINS), $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES)))

all: glfw-test

shared: $(OBJ_FILES)
	$(CC) $(CXXSHAREDFLAGS) -o $(TARGET) $(LIBS) $^ $(LDFLAGS)

glfw-test: $(OBJ_FILES) $(BUILD_DIR)/glfw-test.o
	$(CC) $(CXXFLAGS) -o $(TARGET_DIR)/$@ $(LIBS) $^
	cp -r $(SRC_DIR)/shaders $(TARGET_DIR)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm $(BUILD_DIR)/*.o

-include $(OBJ_FILES:.o=.d)
