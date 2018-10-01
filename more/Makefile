.PHONY: all createdir clean

UNAME := $(shell uname)

ifeq ($(UNAME),Linux)
BOOST_ROOT := /mnt/c/boost_1_67_0
LIBS_WHERE = /mnt/c/boost_lib/boost/bin.v2/libs/graph/build/gcc-6.3.0/debug/link-static/runtime-link-static/threading-multi
endif

ifeq ($(OS),Windows_NT)
BOOST_ROOT := C:\\boost_1_67_0
LIBS_WHERE = C:\\boost_lib\\boost\\bin.v2\\libs\\graph\\build\\gcc-6.3.0\\debug\\link-static\\runtime-link-static\\threading-multi
endif



SRC_DIR := src
OBJ_DIR := bin
GVW_DIR := GraphViewer/cpp
OUT_DIR := .
OUT := $(OUT_DIR)/graph.exe



SRC_CPP := $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP))

GVW_CPP := $(wildcard $(GVW_DIR)/*.cpp)
GVW_OBJ := $(patsubst $(GVW_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(GVW_CPP))
OBJECTS := $(GVW_OBJ) $(SRC_OBJ)



WARNS := -Wall -Wextra -Wno-unused-function
CXXFLAGS := --std=c++17 -O2 -march=native $(WARNS)
INCLUDE := -isystem $(BOOST_ROOT) -I $(GVW_DIR)


LIB_PATH := -L$(LIBS_WHERE)
LIBS := -lws2_32


all: createdir $(OBJECTS)
	g++ $(CXXFLAGS) $(INCLUDE) $(LIB_PATH) -o $(OUT) $(OBJECTS) $(LIBS)

createdir:
	@mkdir -p bin

$(SRC_OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

$(GVW_OBJ): $(OBJ_DIR)/%.o: $(GVW_DIR)/%.cpp
	@g++ $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

clean: clean-src clean-gvw

clean-src:
	@rm -f $(OUT) $(SRC_OBJ)

clean-gvw:
	@rm $(GVW_OBJ)
