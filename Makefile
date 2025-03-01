# Detect OS
OS := $(shell uname -s)

# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -O0 

# Source files
SRCS = main.cpp game.cpp player.cpp timer.cpp bullet.cpp entityList.cpp geometry.cpp asteroid.cpp UIManager.cpp

# Object files directory
OBJ_DIR = obj

# Object files (with paths relative to OBJ_DIR)
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# Include directories
INCLUDES = -Iinclude

# OS-Specific configurations
ifeq ($(OS), Linux)
    LIBS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET = space_shooter
else
    LIBS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = space_shooter.exe
    CXXFLAGS += -static
endif

# Default rule
all: $(OBJ_DIR) $(TARGET)

# Create object files directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile source files into object files
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Clean up object files and executable
clean:
	-rm -rf obj/*.o $(TARGET)